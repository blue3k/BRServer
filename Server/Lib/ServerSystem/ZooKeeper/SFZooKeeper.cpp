////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2017 StormForge
// 
// Author : KyungKun Ko
//
// Description : ZooKeeper warper
//	
//
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ZooKeeper/SFZooKeeper.h"
#include "zookeeper.h"



static_assert(SF::ZOOKEEPER_STAT_BUFFER_SIZE >= sizeof(Stat), "We need enough stat buffer size");
static_assert(SF::ZOOKEEPER_CLIENTID_BUFFER_SIZE >= sizeof(clientid_t), "We need enough stat buffer size");


namespace SF
{


	void ZooKeeperWatcher::ZooKeeperTask::SetResult(int zkResult)
	{
		Result = ZooKeeper::ToResult(zkResult);
		if (!Result && GetState() == State::Pending)
		{
			TaskOperator().Finished(this);
		}
	}


	ZooKeeperWatcher::ZooKeeperWatcher(IMemoryManager& memoryManager)
		: m_EventQueue(memoryManager)
		, m_State(0)
	{
	}

	void ZooKeeperWatcher::ZKWatcherCB(zhandle_t *zkHandle, int type, int state, const char *path, void*v)
	{
		auto zooKeeper = (ZooKeeperWatcher*)v;

		zooKeeper->m_State.store(state, std::memory_order_relaxed);

		if (type != ZOO_SESSION_EVENT) {
			ZKEvent evt;
			evt.NodePath = path;
			evt.EventType = type;
			zooKeeper->m_EventQueue.Enqueue(evt.Composited);
		}
	}

	void ZooKeeperWatcher::ZKWatcherCBComlition(int rc, const void *data)
	{
		auto pTask = (ZooKeeperTask*)data;
		TaskOperator().StartWorking(pTask);
		pTask->Result = ZooKeeper::ToResult(rc);
		TaskOperator().Finished(pTask);
		SharedReferenceDec(static_cast<SharedObject*>(pTask));
	}

	void ZooKeeperWatcher::ZKWatcherCBStatComlition(int rc, const struct Stat *stat, const void *data)
	{
		auto pTask = (StatTask*)data;
		TaskOperator().StartWorking(pTask);
		pTask->Result = ZooKeeper::ToResult(rc);
		*pTask->ResultStat = *stat;
		TaskOperator().Finished(pTask);
		SharedReferenceDec(static_cast<SharedObject*>(pTask));
	}

	void ZooKeeperWatcher::ZKWatcherCBDataComlition(int rc, const char *value, int value_len, const struct Stat *stat, const void *data)
	{
		auto pTask = (DataTask*)data;
		TaskOperator().StartWorking(pTask);
		pTask->Result = ZooKeeper::ToResult(rc);
		pTask->ResultData.SetItemCount(value_len);
		memcpy(pTask->ResultData.data(), value, value_len);
		TaskOperator().Finished(pTask);
		SharedReferenceDec(static_cast<SharedObject*>(pTask));
	}

	void ZooKeeperWatcher::ZKWatcherCBStringsComlition(int rc, const struct String_vector *strings, const void *data)
	{
		auto pTask = (StringsTask*)data;
		TaskOperator().StartWorking(pTask);
		pTask->Result = ZooKeeper::ToResult(rc);
		pTask->ResultStrings.SetItemCount(strings->count);
		for (int iStr = 0; iStr < strings->count; iStr++)
		{
			pTask->ResultStrings[iStr] = strings->data[iStr];
		}
		TaskOperator().Finished(pTask);
		SharedReferenceDec(static_cast<SharedObject*>(pTask));
	}

	void ZooKeeperWatcher::ZKWatcherCBStringsStatComlition(int rc, const struct String_vector *strings, const struct Stat *stat, const void *data)
	{
		auto pTask = (StringsStatTask*)data;
		TaskOperator().StartWorking(pTask);
		pTask->Result = ZooKeeper::ToResult(rc);
		pTask->ResultStrings.SetItemCount(strings->count);
		for (int iStr = 0; iStr < strings->count; iStr++)
		{
			pTask->ResultStrings[iStr] = strings->data[iStr];
		}
		*pTask->ResultStat = *stat;
		TaskOperator().Finished(pTask);
		SharedReferenceDec(static_cast<SharedObject*>(pTask));
	}

	void ZooKeeperWatcher::ZKWatcherCBStringComlition(int rc, const char *value, const void *data)
	{
		auto pTask = (StringTask*)data;
		TaskOperator().StartWorking(pTask);
		pTask->Result = ZooKeeper::ToResult(rc);
		pTask->ResultString = value;
		TaskOperator().Finished(pTask);
		SharedReferenceDec(static_cast<SharedObject*>(pTask));
	}

	void ZooKeeperWatcher::ZKWatcherCBACLComlition(int rc, struct ACL_vector *acl, struct Stat *stat, const void *data)
	{
		auto pTask = (ACLTask*)data;
		TaskOperator().StartWorking(pTask);
		pTask->Result = ZooKeeper::ToResult(rc);
		pTask->ResultACL.SetItemCount(acl->count);
		memcpy(pTask->ResultACL.data(), acl->data, sizeof(ACL)*acl->count);
		TaskOperator().Finished(pTask);
		SharedReferenceDec(static_cast<SharedObject*>(pTask));
	}



	Result ZooKeeperWatcher::DequeueEvent(ZKEvent& eventOut)
	{
		return m_EventQueue.Dequeue(eventOut.Composited);
	}



	ZooKeeper::ZooKeeper(IMemoryManager& memoryManager)
		: m_MemoryManager(memoryManager)
		, m_ZKHandle(nullptr)
		, m_State(0)
		, m_ZKWatcher(memoryManager)
		, m_LogLevel(ZOO_LOG_LEVEL_WARN)
	{
		m_ClientID = (clientid_t*)ClientIDBuffer;
		memset(m_ClientID, 0, sizeof(clientid_t));
	}

	ZooKeeper::~ZooKeeper()
	{
		Close();
	}

	bool ZooKeeper::IsConnected() const
	{
		return m_ZKWatcher.GetState() == ZOO_CONNECTED_STATE;
	}

	void ZooKeeper::Close()
	{
		if (m_ZKHandle != nullptr)
			zookeeper_close(m_ZKHandle);
		m_ZKHandle = nullptr;
	}

	Result ZooKeeper::Connect(const char* connectionString)
	{
		Close();

		zoo_set_debug_level((ZooLogLevel)m_LogLevel);

		clientid_t* previousClientID = m_ClientID->client_id != 0 ? m_ClientID : nullptr;
		m_ZKHandle = zookeeper_init(connectionString, ZooKeeperWatcher::ZKWatcherCB, 10000, previousClientID, &m_ZKWatcher, 0);
		if (m_ZKHandle == nullptr)
		{
			return ResultCode::UNEXPECTED;
		}

		*m_ClientID = *zoo_client_id(m_ZKHandle);

		return ResultCode::SUCCESS;
	}



	void ZooKeeper::yield(DurationMS duration)
	{
		if (m_ZKHandle == nullptr)
			return;

#if 1
		ThisThread::SleepFor(duration);
#else

#ifdef WIN32
		SOCKET fd;
#else
		int fd;
#endif
		int interest;
		int events;
		struct timeval tv;
		int rc;
		time_t expires = time(0) + duration.count() / 1000;
		time_t timeLeft = duration.count() / 1000;
		fd_set rfds, wfds, efds;
		FD_ZERO(&rfds);
		FD_ZERO(&wfds);
		FD_ZERO(&efds);

		while (timeLeft >= 0)
		{
			zookeeper_interest(m_ZKHandle, &fd, &interest, &tv);
			if (fd != -1) {
				if (interest&ZOOKEEPER_READ) {
					FD_SET(fd, &rfds);
				}
				else {
					FD_CLR(fd, &rfds);
				}
				if (interest&ZOOKEEPER_WRITE) {
					FD_SET(fd, &wfds);
				}
				else {
					FD_CLR(fd, &wfds);
				}
			}
			else {
				fd = 0;
			}
			FD_SET(0, &rfds);
			if (tv.tv_sec > timeLeft) {
				tv.tv_sec = timeLeft;
			}
			rc = select(fd + 1, &rfds, &wfds, &efds, &tv);
			timeLeft = expires - time(0);
			events = 0;
			if (FD_ISSET(fd, &rfds)) {
				events |= ZOOKEEPER_READ;
			}
			if (FD_ISSET(fd, &wfds)) {
				events |= ZOOKEEPER_WRITE;
			}
			zookeeper_process(m_ZKHandle, events);
		}
#endif
	}

	bool ZooKeeper::WaitForConnected()
	{
		if (m_ZKHandle == nullptr)
			return false;

		time_t expires = time(0) + 10;
		while (!IsConnected() && time(0) < expires)
		{
			yield(DurationMS(500));
		}

		return IsConnected();
	}

	bool ZooKeeper::WaitForDisconnected()
	{
		if (m_ZKHandle == nullptr)
			return false;

		time_t expires = time(0) + 15;
		while (IsConnected() && time(0) < expires)
		{
			yield(DurationMS(500));
		}
		return !IsConnected();
	}

	Result ZooKeeper::ToResult(int zkResult)
	{
		switch (zkResult)
		{
		case (int)ZOO_ERRORS::ZOK:						return ResultCode::SUCCESS;
		case (int)ZOO_ERRORS::ZSYSTEMERROR:				return ResultCode::ZK_SYSTEM;
		case (int)ZOO_ERRORS::ZRUNTIMEINCONSISTENCY:	return ResultCode::ZK_ZRUNTIMEINCONSISTENCY;
		case (int)ZOO_ERRORS::ZDATAINCONSISTENCY:		return ResultCode::ZK_ZDATAINCONSISTENCY;
		case (int)ZOO_ERRORS::ZCONNECTIONLOSS:			return ResultCode::ZK_ZCONNECTIONLOSS;
		case (int)ZOO_ERRORS::ZMARSHALLINGERROR:		return ResultCode::ZK_ZMARSHALLINGERROR;
		case (int)ZOO_ERRORS::ZUNIMPLEMENTED:			return ResultCode::ZK_ZUNIMPLEMENTED;
		case (int)ZOO_ERRORS::ZOPERATIONTIMEOUT:		return ResultCode::ZK_ZOPERATIONTIMEOUT;
		case (int)ZOO_ERRORS::ZBADARGUMENTS:			return ResultCode::ZK_ZBADARGUMENTS;
		case (int)ZOO_ERRORS::ZINVALIDSTATE:			return ResultCode::ZK_ZINVALIDSTATE;

		case (int)ZOO_ERRORS::ZAPIERROR:				return ResultCode::ZK_ZAPIERROR;
		case (int)ZOO_ERRORS::ZNONODE:					return ResultCode::ZK_ZNONODE;
		case (int)ZOO_ERRORS::ZNOAUTH:					return ResultCode::ZK_ZNOAUTH;
		case (int)ZOO_ERRORS::ZBADVERSION:				return ResultCode::ZK_ZBADVERSION;
		case (int)ZOO_ERRORS::ZNOCHILDRENFOREPHEMERALS:	return ResultCode::ZK_ZNOCHILDRENFOREPHEMERALS;
		case (int)ZOO_ERRORS::ZNODEEXISTS:				return ResultCode::ZK_ZNODEEXISTS;
		case (int)ZOO_ERRORS::ZNOTEMPTY:				return ResultCode::ZK_ZNOTEMPTY;
		case (int)ZOO_ERRORS::ZSESSIONEXPIRED:			return ResultCode::ZK_ZSESSIONEXPIRED;
		case (int)ZOO_ERRORS::ZINVALIDCALLBACK:			return ResultCode::ZK_ZINVALIDCALLBACK;
		case (int)ZOO_ERRORS::ZINVALIDACL:				return ResultCode::ZK_ZINVALIDACL;
		case (int)ZOO_ERRORS::ZAUTHFAILED:				return ResultCode::ZK_ZAUTHFAILED;
		case (int)ZOO_ERRORS::ZCLOSING:					return ResultCode::ZK_ZCLOSING;
		case (int)ZOO_ERRORS::ZNOTHING:					return ResultCode::ZK_ZNOTHING;
		case (int)ZOO_ERRORS::ZSESSIONMOVED:			return ResultCode::ZK_ZSESSIONMOVED;
		default:
			return ResultCode::UNEXPECTED;
		}
	}


	/////////////////////////////////////////////////////////////
	//
	//	Operations
	//

	// The one with watcher works as asynchronous

	Result ZooKeeper::Create(const char* path, const Array<uint8_t>& value, const struct ACL_vector *acl, int flags, String& outPath)
	{
		if (m_ZKHandle == nullptr)
			return ResultCode::NOT_INITIALIZED;

		if (!IsConnected())
			return ResultCode::INVALID_STATE;

		auto valueData = value.GetItemCount() > 0 ? value.data() : nullptr;
		int valueSize = value.GetItemCount() > 0 ? (int)value.GetItemCount() : -1;

		if (acl == nullptr)
			acl = &ZOO_OPEN_ACL_UNSAFE;

		StaticArray<char, 1024> nameBuffer(m_MemoryManager);
		auto zkResult = zoo_create(m_ZKHandle, path, (const char*)valueData, valueSize, acl, flags, nameBuffer.data(), (int)nameBuffer.GetAllocatedSize());
		if (zkResult == ZOK)
			outPath = nameBuffer.data();

		return ToResult(zkResult);
	}

	Result ZooKeeper::Create(const char* path, const std::string& value, const struct ACL_vector *acl, int flags, String& outPath)
	{
		if (m_ZKHandle == nullptr)
			return ResultCode::NOT_INITIALIZED;

		if (!IsConnected())
			return ResultCode::INVALID_STATE;

		auto valueData = value.size() > 0 ? value.data() : nullptr;
		int valueSize = value.size() > 0 ? (int)value.size() : -1;

		if (acl == nullptr)
			acl = &ZOO_OPEN_ACL_UNSAFE;

		StaticArray<char, 1024> nameBuffer(m_MemoryManager);
		auto zkResult = zoo_create(m_ZKHandle, path, (const char*)valueData, valueSize, acl, flags, nameBuffer.data(), (int)nameBuffer.GetAllocatedSize());
		if (zkResult == ZOK)
			outPath = nameBuffer.data();

		return ToResult(zkResult);
	}

	SharedPointerT<ZooKeeper::StringTask> ZooKeeper::ACreate(const char* path, const Array<uint8_t>& value, const struct ACL_vector *acl, int flags)
	{
		if (m_ZKHandle == nullptr)
			return SharedPointerT<ZooKeeper::StringTask>();

		if (!IsConnected())
			return SharedPointerT<ZooKeeper::StringTask>();

		auto valueData = value.GetItemCount() > 0 ? value.data() : nullptr;
		int valueSize = value.GetItemCount() > 0 ? (int)value.GetItemCount() : -1;

		if (acl == nullptr)
			acl = &ZOO_OPEN_ACL_UNSAFE;

		auto pTask = SharedPointerT<StringTask>(new(m_MemoryManager) StringTask(m_MemoryManager));
		TaskOperator().Requested(*pTask);
		SharedReferenceInc(static_cast<SharedObject*>(*pTask));
		auto zkResult = zoo_acreate(m_ZKHandle, path, (const char*)valueData, valueSize, acl, flags, ZooKeeperWatcher::ZKWatcherCBStringComlition, *pTask);
		pTask->SetResult(zkResult);
		if (zkResult != ZOK) SharedReferenceDec(static_cast<SharedObject*>(*pTask));
		return std::forward<SharedPointerT<StringTask>>(pTask);
	}

	Result ZooKeeper::Delete(const char* path, int version)
	{
		if (m_ZKHandle == nullptr)
			return ResultCode::NOT_INITIALIZED;

		if (!IsConnected())
			return ResultCode::INVALID_STATE;

		auto zkResult = zoo_delete(m_ZKHandle, path, version);

		return ToResult(zkResult);
	}

	SharedPointerT<ZooKeeper::ZooKeeperTask> ZooKeeper::ADelete(const char* path, int version)
	{
		if (m_ZKHandle == nullptr)
			return SharedPointerT<ZooKeeper::ZooKeeperTask>();

		if (!IsConnected())
			return SharedPointerT<ZooKeeper::ZooKeeperTask>();

		auto pTask = SharedPointerT<ZooKeeperTask>(new(m_MemoryManager) ZooKeeperTask());
		TaskOperator().Requested(*pTask);
		SharedReferenceInc(static_cast<SharedObject*>(*pTask));
		auto zkResult = zoo_adelete(m_ZKHandle, path, version, ZooKeeperWatcher::ZKWatcherCBComlition, *pTask);
		pTask->SetResult(zkResult);
		if(zkResult != ZOK) SharedReferenceDec(static_cast<SharedObject*>(*pTask));
		return std::forward<SharedPointerT<ZooKeeper::ZooKeeperTask>>(pTask);
	}

	Result ZooKeeper::DeleteTree(const char* path)
	{
		DynamicArray<String> children(GetMemoryManager());
		Result result = GetChildren(path, children);
		if (!result)
			return result;

		for (auto& itChild : children)
		{
			String childPath(GetMemoryManager());
			childPath.Format("{0}/{1}", path, itChild);
			result = DeleteTree(childPath);
			if (!result)
				return result;
		}

		return Delete(path);
	}



	Result ZooKeeper::Exists(const char* path)
	{
		if (m_ZKHandle == nullptr)
			return ResultCode::NOT_INITIALIZED;

		if (!IsConnected())
			return ResultCode::INVALID_STATE;

		auto zkResult = zoo_exists(m_ZKHandle, path, 0, nullptr);

		return ToResult(zkResult);
	}

	SharedPointerT<ZooKeeper::StatTask> ZooKeeper::AExists(const char* path)
	{
		if (m_ZKHandle == nullptr)
			return SharedPointerT<ZooKeeper::StatTask>();

		if (!IsConnected())
			return SharedPointerT<ZooKeeper::StatTask>();

		auto pTask = SharedPointerT<StatTask>(new(m_MemoryManager) StatTask());
		TaskOperator().Requested(*pTask);
		SharedReferenceInc(static_cast<SharedObject*>(*pTask));
		auto zkResult = zoo_aexists(m_ZKHandle, path, 0, ZooKeeperWatcher::ZKWatcherCBStatComlition, *pTask);
		pTask->SetResult(zkResult);
		if (zkResult != ZOK) SharedReferenceDec(static_cast<SharedObject*>(*pTask));
		return std::forward<SharedPointerT<ZooKeeper::StatTask>>(pTask);
	}

	Result ZooKeeper::Get(const char *path, Array<uint8_t>& valueBuffer, struct Stat *stat)
	{
		if (m_ZKHandle == nullptr)
			return ResultCode::NOT_INITIALIZED;

		if (!IsConnected())
			return ResultCode::INVALID_STATE;

		int buffLen = (int)valueBuffer.GetAllocatedSize();
		auto zkResult = zoo_get(m_ZKHandle, path, 0, (char*)valueBuffer.data(), &buffLen, stat);
		valueBuffer.SetItemCount(buffLen);

		return ToResult(zkResult);
	}

	SharedPointerT<ZooKeeper::DataTask> ZooKeeper::AGet(const char *path)
	{
		if (m_ZKHandle == nullptr)
			return SharedPointerT<ZooKeeper::DataTask>();

		if (!IsConnected())
			return SharedPointerT<ZooKeeper::DataTask>();

		auto pTask = SharedPointerT<DataTask>(new(m_MemoryManager) DataTask(m_MemoryManager));
		TaskOperator().Requested(*pTask);
		SharedReferenceInc(static_cast<SharedObject*>(*pTask));
		auto zkResult = zoo_aget(m_ZKHandle, path, 0, ZooKeeperWatcher::ZKWatcherCBDataComlition, *pTask);
		pTask->SetResult(zkResult);
		if (zkResult != ZOK) SharedReferenceDec(static_cast<SharedObject*>(*pTask));
		return std::forward<SharedPointerT<ZooKeeper::DataTask>>(pTask);
	}

	Result ZooKeeper::Set(const char *path, const Array<uint8_t>& valueBuffer, int version)
	{
		if (m_ZKHandle == nullptr)
			return ResultCode::NOT_INITIALIZED;

		if (!IsConnected())
			return ResultCode::INVALID_STATE;

		auto zkResult = zoo_set(m_ZKHandle, path, (char*)valueBuffer.data(), (int)valueBuffer.GetItemCount(), version);

		return ToResult(zkResult);
	}

	Result ZooKeeper::Set(const char *path, const std::string& valueBuffer, int version)
	{
		if (m_ZKHandle == nullptr)
			return ResultCode::NOT_INITIALIZED;

		if (!IsConnected())
			return ResultCode::INVALID_STATE;

		auto zkResult = zoo_set(m_ZKHandle, path, (char*)valueBuffer.data(), (int)valueBuffer.size(), version);

		return ToResult(zkResult);
	}

	SharedPointerT<ZooKeeper::StatTask> ZooKeeper::ASet(const char *path, const Array<uint8_t>& valueBuffer, int version)
	{
		if (m_ZKHandle == nullptr)
			return SharedPointerT<ZooKeeper::StatTask>();

		if (!IsConnected())
			return SharedPointerT<ZooKeeper::StatTask>();

		auto pTask = SharedPointerT<StatTask>(new(m_MemoryManager) StatTask());
		TaskOperator().Requested(*pTask);
		SharedReferenceInc(static_cast<SharedObject*>(*pTask));
		auto zkResult = zoo_aset(m_ZKHandle, path, (char*)valueBuffer.data(), (int)valueBuffer.GetItemCount(), version, ZooKeeperWatcher::ZKWatcherCBStatComlition, *pTask);
		pTask->SetResult(zkResult);
		if (zkResult != ZOK) SharedReferenceDec(static_cast<SharedObject*>(*pTask));
		return std::forward<SharedPointerT<ZooKeeper::StatTask>>(pTask);
	}


	Result ZooKeeper::GetChildren(const char *path, Array<String>& strings)
	{
		if (m_ZKHandle == nullptr)
			return ResultCode::NOT_INITIALIZED;

		if (!IsConnected())
			return ResultCode::INVALID_STATE;

		String_vector tempList;
		memset(&tempList, 0, sizeof(tempList));
		auto zkResult = zoo_get_children(m_ZKHandle, path, 0, &tempList);
		for (int iStr = 0; iStr < tempList.count; iStr++)
		{
			strings.Append(String(m_MemoryManager, tempList.data[iStr]));
		}

		return ToResult(zkResult);
	}

	SharedPointerT<ZooKeeper::StringsTask> ZooKeeper::AGetChildren(const char *path)
	{
		if (m_ZKHandle == nullptr)
			return SharedPointerT<ZooKeeper::StringsTask>();

		if (!IsConnected())
			return SharedPointerT<ZooKeeper::StringsTask>();

		auto pTask = SharedPointerT<StringsTask>(new(m_MemoryManager) StringsTask(m_MemoryManager));
		TaskOperator().Requested(*pTask);
		SharedReferenceInc(static_cast<SharedObject*>(*pTask));
		auto zkResult = zoo_aget_children(m_ZKHandle, path, 0, ZooKeeperWatcher::ZKWatcherCBStringsComlition, *pTask);
		pTask->SetResult(zkResult);
		if (zkResult != ZOK) SharedReferenceDec(static_cast<SharedObject*>(*pTask));
		return std::forward<SharedPointerT<ZooKeeper::StringsTask>>(pTask);
	}

	SharedPointerT<ZooKeeper::StringsStatTask> ZooKeeper::AGetChildren2(const char *path)
	{
		if (m_ZKHandle == nullptr)
			return SharedPointerT<ZooKeeper::StringsStatTask>();

		if (!IsConnected())
			return SharedPointerT<ZooKeeper::StringsStatTask>();

		auto pTask = SharedPointerT<StringsStatTask>(new(m_MemoryManager) StringsStatTask(m_MemoryManager));
		TaskOperator().Requested(*pTask);
		SharedReferenceInc(static_cast<SharedObject*>(*pTask));
		auto zkResult = zoo_aget_children2(m_ZKHandle, path, 0, ZooKeeperWatcher::ZKWatcherCBStringsStatComlition, *pTask);
		pTask->SetResult(zkResult);
		if (zkResult != ZOK) SharedReferenceDec(static_cast<SharedObject*>(*pTask));
		return std::forward<SharedPointerT<ZooKeeper::StringsStatTask>>(pTask);
	}



	Result ZooKeeper::GetAcl(const char *path, struct ACL_vector *acl, struct Stat *stat)
	{
		if (m_ZKHandle == nullptr)
			return ResultCode::NOT_INITIALIZED;

		if (!IsConnected())
			return ResultCode::INVALID_STATE;

		auto zkResult = zoo_get_acl(m_ZKHandle, path, acl, stat);

		return ToResult(zkResult);
	}


	Result ZooKeeper::SetAcl(const char *path, int version, struct ACL_vector *acl)
	{
		if (m_ZKHandle == nullptr)
			return ResultCode::NOT_INITIALIZED;

		if (!IsConnected())
			return ResultCode::INVALID_STATE;

		auto zkResult = zoo_set_acl(m_ZKHandle, path, version, acl);

		return ToResult(zkResult);
	}


	Result ZooKeeper::BatchRun(int count, const zoo_op_t *ops, zoo_op_result_t *results)
	{
		if (m_ZKHandle == nullptr)
			return ResultCode::NOT_INITIALIZED;

		if (!IsConnected())
			return ResultCode::INVALID_STATE;

		auto zkResult = zoo_multi(m_ZKHandle, count, ops, results);

		return ToResult(zkResult);
	}

}

