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

#pragma once

#include "String/FixedString32.h"
#include "String/SFString.h"
#include "Container/CircularPageQueue.h"
#include "Thread/Task.h"

#include "zookeeper.h"



namespace SF
{

	///////////////////////////////////////////////////////////////////////////////////
	//
	//	Watcher class for the ZooKeeper
	//

	class ZooKeeperWatcher
	{
	public:

#pragma pack(push, 4)
		union ZKEvent
		{
			struct {
				FixedString32 NodePath;
				int EventType;
			};

			uint64_t Composited;

			ZKEvent() {}
		};
#pragma pack(pop)



		/////////////////////////////////////////////////////////////////
		//
		//	Tasks for each result type
		//

		class ZooKeeperTask : public Task
		{
		public:
			Result Result;

			ZooKeeperTask()
			{}

			void SetResult(int zkResult);

			virtual void Run() override {}
		};


		class StringTask : public ZooKeeperTask
		{
		public:
			String ResultString;

			StringTask(IMemoryManager& memoryManager)
				: ResultString(memoryManager)
			{}
		};


		class StringsTask : public ZooKeeperTask
		{
		public:
			DynamicArray<String> ResultStrings;

			StringsTask(IMemoryManager& memoryManager)
				: ResultStrings(memoryManager)
			{}
		};


		class StringsStatTask : public ZooKeeperTask
		{
		public:
			DynamicArray<String> ResultStrings;
			struct Stat ResultStat;

			StringsStatTask(IMemoryManager& memoryManager)
				: ResultStrings(memoryManager)
			{
				memset(&ResultStat, 0, sizeof(ResultStat));
			}
		};


		class StatTask : public ZooKeeperTask
		{
		public:
			struct Stat ResultStat;

			StatTask()
			{
				memset(&ResultStat, 0, sizeof(ResultStat));
			}
		};


		class DataTask : public ZooKeeperTask
		{
		public:
			DynamicArray<uint8_t> ResultData;

			DataTask(IMemoryManager& memoryManager)
				: ResultData(memoryManager)
			{}
		};


		class ACLTask : public ZooKeeperTask
		{
		public:
			StaticArray<ACL, 10> ResultACL;

			ACLTask(IMemoryManager& memoryManager)
				: ResultACL(memoryManager)
			{}
		};

	private:

		// State of zoo keeper server
		std::atomic<int> m_State;

		// Zookeeper event queue
		CircularPageQueueAtomic<uint64_t> m_EventQueue;

	private:

		static void ZKWatcherCB(zhandle_t *zkHandle, int type, int state, const char *path, void*v);
		static void ZKWatcherCBComlition(int rc, const void *data);
		static void ZKWatcherCBStatComlition(int rc, const struct Stat *stat, const void *data);
		static void ZKWatcherCBDataComlition(int rc, const char *value, int value_len, const struct Stat *stat, const void *data);
		static void ZKWatcherCBStringsComlition(int rc, const struct String_vector *strings, const void *data);
		static void ZKWatcherCBStringsStatComlition(int rc, const struct String_vector *strings, const struct Stat *stat, const void *data);
		static void ZKWatcherCBStringComlition(int rc, const char *value, const void *data);
		static void ZKWatcherCBACLComlition(int rc, struct ACL_vector *acl, struct Stat *stat, const void *data);

		friend class ZooKeeper;

	public:

		ZooKeeperWatcher(IMemoryManager& memoryManager);


		int GetState() const { return m_State.load(std::memory_order_relaxed); }

		// Dequeue ZooKeeper event
		Result DequeueEvent(ZKEvent& eventOut);

	};



	///////////////////////////////////////////////////////////////////////////////////
	//
	//	ZooKeeper API wrapper
	//

	class ZooKeeper
	{
	public:

		typedef ZooKeeperWatcher::ZKEvent			ZKEvent;
		typedef ZooKeeperWatcher::ZooKeeperTask		ZooKeeperTask;
		typedef ZooKeeperWatcher::ACLTask			ACLTask;
		typedef ZooKeeperWatcher::StringTask		StringTask;
		typedef ZooKeeperWatcher::StringsTask		StringsTask;
		typedef ZooKeeperWatcher::StringsStatTask	StringsStatTask;
		typedef ZooKeeperWatcher::StatTask			StatTask;
		typedef ZooKeeperWatcher::DataTask			DataTask;

	private:

		IMemoryManager& m_MemoryManager;

		// ZooKeeper handle
		zhandle_t* m_ZKHandle;

		// State of zoo keeper server
		std::atomic<int> m_State;

		// ZooKeeper client id
		clientid_t m_ClientID;

		// Watcher for main Zookeeper connection
		ZooKeeperWatcher m_ZKWatcher;

		// Zookeeper log level
		ZooLogLevel m_LogLevel = ZOO_LOG_LEVEL_WARN;

	public:

		ZooKeeper(IMemoryManager& memoryManager);
		~ZooKeeper();

		IMemoryManager& GetMemoryManager() { return m_MemoryManager; }

		/////////////////////////////////////////////////////////////
		//
		//	
		//

		ZooKeeperWatcher& GetWatcher() { return m_ZKWatcher; }

		// check connection status
		bool IsConnected() const { return m_ZKWatcher.GetState() == ZOO_CONNECTED_STATE; }
		int GetState() const { return m_ZKWatcher.GetState(); }

		// Close
		void Close();

		// Connect to ZooKeeper server
		Result Connect(const char* connectionString);

		// Dequeue ZooKeeper event
		Result DequeueEvent(ZKEvent& eventOut) { return m_ZKWatcher.DequeueEvent(eventOut); }

		// Yield until it has something or time out
		void yield(DurationMS duration);

		// Wait for connected state
		bool WaitForConnected();

		// Wait for disconnected state
		bool WaitForDisconnected();


		static Result ToResult(int zkResult);


		/////////////////////////////////////////////////////////////
		//
		//	Operations
		//

		// The one 'A' prefix works as asynchronous

		Result Create(const char* path, const Array<uint8_t>& value, const struct ACL_vector *acl, int flags, String& outPath);
		Result Create(const char* path, const std::string& value, const struct ACL_vector *acl, int flags, String& outPath);
		SharedPointerT<StringTask> ACreate(const char* path, const Array<uint8_t>& value, const struct ACL_vector *acl, int flags);

		Result Delete(const char* path, int version = -1);
		SharedPointerT<ZooKeeperTask> ADelete(const char* path, int version = -1);

		Result DeleteTree(const char* path);

		Result Exists(const char* path);
		SharedPointerT<StatTask> AExists(const char* path);

		Result Get(const char *path, Array<uint8_t>& valueBuffer, struct Stat *stat = nullptr);
		SharedPointerT<DataTask> AGet(const char *path);

		Result Set(const char *path, const Array<uint8_t>& valueBuffer, int version = -1);
		Result Set(const char *path, const std::string& valueBuffer, int version = -1);
		SharedPointerT<StatTask> ASet(const char *path, const Array<uint8_t>& valueBuffer, int version);

		Result GetChildren(const char *path, Array<String>& strings);
		SharedPointerT<StringsTask> AGetChildren(const char *path);
		SharedPointerT<StringsStatTask> AGetChildren2(const char *path);

		Result GetAcl(const char *path, struct ACL_vector *acl, struct Stat *stat);

		Result SetAcl(const char *path, int version, struct ACL_vector *acl);

		Result BatchRun(int count, const zoo_op_t *ops, zoo_op_result_t *results);
	};

}


