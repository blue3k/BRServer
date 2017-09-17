////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 Madk
// 
// Author : KyungKun Ko
//
// Description : Server service Component entity implementation
//	
//
////////////////////////////////////////////////////////////////////////////////



#include "stdafx.h"
#include "String/StrUtil.h"
#include "ServerLog/SvrLog.h"
#include "Thread/Thread.h"
#include "Types/BrSvrTypes.h"
#include "Net/NetDef.h"
#include "Server/BrServer.h"
#include "Entity/Entity.h"
#include "Entity/EntityManager.h"
#include "Component/ServerComponent.h"
#include "ServerService/ServerServiceBase.h"
#include "ServerEntity/ServerEntity.h"
#include "ServiceEntity/ClusterManagerServiceEntity.h"
#include "SvrTrace.h"
#include "Service/ServerService.h"

#include "Protocol/Message/ClusterServerMsgClass.h"
#include "ServiceEntity/ClusterServiceTrans.h"
#include "ServiceEntity/ClusterManagerServiceTrans.h"
#include "Protocol/ServerService/ClusterServerService.h"


namespace SF {
namespace Svr {



	//////////////////////////////////////////////////////////////////////////
	//
	//	 informations
	//


	ClusterServiceInfo::ClusterServiceInfo(IHeap& heap, GameID gameID, ClusterID clusterID, bool activelyConnect)
		: ZooKeeperWatcher(heap)
		, m_Heap(heap)
		, m_ActivelyConnect(activelyConnect)
		, m_ClusterKey(gameID, clusterID)
		, m_ClusterPath(heap)
		, m_LatestSelected(0)
		, Services(heap)
		, LocalServiceEntites(heap)
	{
		if (m_ClusterKey.GameClusterID != GameID::Invalid)
		{
			m_ClusterPath.Format("{0}/{1}/{2}", ClusterManagerServiceEntity::ServiceBasePath, Enum<GameID>().GetValueName(gameID), Enum<ClusterID>().GetValueName(clusterID));
		}
		else
		{
			m_ClusterPath.Format("{0}/{1}", ClusterManagerServiceEntity::ServiceBasePath, Enum<ClusterID>().GetValueName(clusterID));
		}

		InitZK();
	}

	ClusterServiceInfo::~ClusterServiceInfo()
	{
		for (auto itService : Services)
		{
			IHeap::Delete(itService);
		}
		Services.Clear();
	}

	void ClusterServiceInfo::InitZK()
	{
		auto& zkSession = Service::ZKSession->GetZooKeeperSession();
		if (!zkSession.IsConnected())
			return;

		if (!zkSession.Exists(m_ClusterPath))
		{
			String outPath(GetHeap());

			// Create game cluster path if not exist
			if (m_ClusterKey.GameClusterID != GameID::Invalid)
			{
				String gameClusterPath(GetHeap());
				gameClusterPath.Format("{0}/{1}", ClusterManagerServiceEntity::ServiceBasePath, Enum<GameID>().GetValueName(m_ClusterKey.GameClusterID));
				if(!zkSession.Exists(gameClusterPath))
					zkSession.Create(gameClusterPath, "", nullptr, 0, outPath);
			}

			Result result = zkSession.Create(m_ClusterPath, "", nullptr, 0, outPath);
			if (!result)
				return;
		}

		// set up watcher and get children
		GetChildrenTask = zkSession.AGetChildren(m_ClusterPath, this);

		m_ZKInitialized = true;
	}


	Result ClusterServiceInfo::GetNodeValue(const String& nodePath, Json::Value& jsonValue)
	{
		StaticArray<uint8_t, 1024> valueBuffer(GetHeap());
		auto& zkSession = Service::ZKSession->GetZooKeeperSession();
		if (!zkSession.IsConnected())
			return ResultCode::FAIL;

		Result result = zkSession.Get(nodePath, valueBuffer, nullptr);
		if (!result) return result;

		Json::Reader reader;

		auto bRes = reader.parse(reinterpret_cast<const char*>(valueBuffer.data()), reinterpret_cast<const char*>(valueBuffer.data()) + valueBuffer.GetItemCount(), jsonValue, false);
		if (!bRes)
			return ResultCode::FAIL;

		return result;
	}

	Result ClusterServiceInfo::SetNodeValue(const String& nodePath, const Json::Value& jsonValue)
	{
		Json::FastWriter writer;
		auto& zkSession = Service::ZKSession->GetZooKeeperSession();
		if (!zkSession.IsConnected())
			return ResultCode::FAIL;

		auto stringValue = std::forward<std::string>(writer.write(jsonValue));

		return zkSession.Set(nodePath, stringValue);
	}




	Json::Value ClusterServiceInfo::ToJsonNetPrivate(const NetAddress& privateNet) const
	{
		Json::Value itemValue(Json::objectValue);

		itemValue["IP"] = privateNet.Address;
		itemValue["Port"] = Json::Value(privateNet.Port);

		return itemValue;
	}


	Result ClusterServiceInfo::ParseNetPrivate(const Json::Value& itemValue, NetAddress& privateNet)
	{
		StrUtil::StringCpy(privateNet.Address, itemValue.get("IP", Json::Value("")).asCString());
		privateNet.Port = itemValue.get("Port", Json::Value("")).asUInt();

		return ResultCode::SUCCESS;
	}

	ServerServiceInformation* ClusterServiceInfo::NewLocalService(ClusteredServiceEntity* pServiceEntity)
	{
		Result hr;
		ServerServiceInformation* pNewServiceInfo = nullptr;
		char nodePath[512];
		std::string stringValue;
		Json::FastWriter writer;
		Json::Value nodeValue(Json::objectValue);

		pNewServiceInfo = new(GetHeap()) ServerServiceInformation(
			pServiceEntity->GetGameID(),
			pServiceEntity->GetClusterID(),
			pServiceEntity->GetEntityUID(),
			pServiceEntity->GetServerEntity(),
			pServiceEntity->GetClusterMembership()
		);
		svrChk(LocalServiceEntites.push_back(pServiceEntity));
		svrChk(Services.Insert(pNewServiceInfo->GetNodeNameCrc(), pNewServiceInfo));

		// add to zk
		auto& zkSession = Service::ZKSession->GetZooKeeperSession();
		if (!zkSession.IsConnected())
			return pNewServiceInfo;

		StrUtil::Format(nodePath, "{0}/{1}_{2}", m_ClusterPath, pNewServiceInfo->GetNodeName());

		nodeValue["EntityID"] = Json::Value(pNewServiceInfo->GetEntityUID().UID);
		nodeValue["NetAddress"] = ToJsonNetPrivate(pServiceEntity->GetServerEntity()->GetPrivateNetAddress());

		stringValue = std::forward<std::string>(writer.write(nodeValue));

		zkSession.ACreate(nodePath, stringValue, nullptr, zkSession.NODE_FLAG_EPHEMERAL);

	Proc_End:

		return pNewServiceInfo;
	}

	void ClusterServiceInfo::DownloadServiceInfo()
	{
		auto& zkSession = Service::ZKSession->GetZooKeeperSession();
		if (!zkSession.IsConnected())
			return;

		zkSession.AGetChildren(m_ClusterPath, this);
	}

	void ClusterServiceInfo::UploadLocalServiceInfo()
	{
		auto& zkSession = Service::ZKSession->GetZooKeeperSession();
		if (!zkSession.IsConnected())
			return;

		
//		zkSession.
	}


	/////////////////////////////////////////////
	//
	//	Overridable Event handling
	//

	Result ClusterServiceInfo::OnNewEvent(const ZKEvent& eventOut)
	{
		if (eventOut.EventType == ZooKeeper::EVENT_CHILD)
		{
			DownloadServiceInfo();
		}
		else
		{

		}

		return ResultCode::SUCCESS;
	}

	void ClusterServiceInfo::OnComlition(ZooKeeperTask& pTask)
	{
		if (!pTask.Result)
		{
			assert(false);
			return;
		}


	}

	void ClusterServiceInfo::OnStatComlition(StatTask& pTask)
	{

	}

	void ClusterServiceInfo::OnDataComlition(DataTask& pTask)
	{

	}


	Result ClusterServiceInfo::AddNewService(const char* nodeName, FixedString nodeNameCrc)
	{
		Result hr;
		String nodePath(GetHeap());
		Json::Value jsonValue(Json::objectValue);
		NetAddress netAddress;
		EntityUID entityUID;
		ServerServiceInformation* pNewServiceInfo = nullptr;
		ServerEntity *pServerEntity = nullptr;

		nodePath.Format("{0}/{1}", m_ClusterPath, nodeName);

		svrChk(GetNodeValue(nodePath, jsonValue));

		svrChk(ParseNetPrivate(jsonValue, netAddress));
		entityUID.UID = jsonValue.get("EntityID", Json::Value(0)).asUInt64();

		svrChk(Service::ServerEntityManager->GetOrRegisterServer(entityUID.GetServerID(), NetClass::Server, netAddress, pServerEntity));

		svrMem(pNewServiceInfo = new(GetHeap()) ServerServiceInformation(m_ClusterKey.GameClusterID, m_ClusterKey.ServiceClusterID, entityUID, pServerEntity, ClusterMembership::Slave));

		svrChk(Services.Insert(nodeNameCrc, pNewServiceInfo));

		// Connect to the service entity
		if (m_ActivelyConnect)
		{
			// TODO: Serverentity will try to connect anyway. Need to change it
			//nodePath
		}

	Proc_End:

		return hr;
	}

	void ClusterServiceInfo::OnStringsComlition(StringsTask& pTask)
	{
		if (!pTask.Result)
		{
			assert(false);
			return;
		}

		SortedSet<FixedString> listAlreadyHave(GetHeap(), static_cast<int>(Services.size()));
		for (auto itService : Services)
		{
			listAlreadyHave.Insert(itService->GetNodeNameCrc());
		}

		// This should be getchildren list because that's the only thing have string list result
		for (auto& itItem : pTask.ResultStrings)
		{
			String nodeName = ZooKeeper::GetLeapNodeName(itItem);
			FixedString nodeNameCrc = *nodeName;
			ServerServiceInformation* pFound = nullptr;
			Services.Find(nodeNameCrc, pFound);
			if (pFound != nullptr)
			{
				listAlreadyHave.Remove(nodeNameCrc);
				continue;
			}
			else
			{
				// New Service I need to add this to the list
				AddNewService(itItem, nodeNameCrc);
			}
		}

		// If anything not in the list should be removed
		for (auto itRemove : listAlreadyHave)
		{
			ServerServiceInformation* pRemoved = nullptr;
			Services.Remove(itRemove, pRemoved);
			if (pRemoved != nullptr)
			{
				assert(pRemoved->GetEntityUID().GetServerID() != BrServer::GetInstance()->GetServerUID());
				IHeap::Delete(pRemoved);
			}
		}

		GetChildrenTask = nullptr;
	}

	void ClusterServiceInfo::OnStringsStatComlition(StringsStatTask& pTask)
	{
		if (!pTask.Result)
		{
			assert(false);
			return;
		}


	}

	void ClusterServiceInfo::OnStringComlition(StringTask& pTask)
	{
		if (!pTask.Result)
		{
			assert(false);
			return;
		}

		// This should be create for new path

	}

	void ClusterServiceInfo::OnACLComlition(ACLTask& pTask)
	{

	}





	//////////////////////////////////////////////////////////////////////////
	//
	//	Entity informations
	//

	constexpr FixedString ClusterManagerServiceEntity::TypeName;
	constexpr const char* ClusterManagerServiceEntity::ServiceBasePath;


	ClusterManagerServiceEntity::ClusterManagerServiceEntity()
		: ServiceEntity()
		, m_ClusterInfoMap(GetHeap())
	{
		// only entity has some special operations
		//if (BrServer::GetInstance()->GetNetClass() == NetClass::Entity)
		//{
		//	BR_ENTITY_MESSAGE(Message::ClusterServer::GetClusterMemberListCmd)				{ svrMemReturn(pNewTrans = new(GetHeap()) ClusterGetMemberListTrans(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		//	BR_ENTITY_MESSAGE(Message::ClusterServer::JoinClusterCmd)						{ svrMemReturn(pNewTrans = new(GetHeap()) JoinClusterTransForEntityServer(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		//	BR_ENTITY_MESSAGE(Message::ClusterServer::GetLowestWorkloadClusterMemberCmd)	{ svrMemReturn(pNewTrans = new(GetHeap()) GetLowestWorkloadClusterMemberTrans(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		//	BR_ENTITY_MESSAGE(Message::ClusterServer::NewServerServiceJoinedC2SEvt)			{ svrMemReturn(pNewTrans = new(GetHeap()) ClusterNewServerServiceJoinedC2SEvtEntityTrans(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		//}
		//else
		//{
			//BR_ENTITY_MESSAGE(Message::ClusterServer::JoinClusterCmd)						{ svrMemReturn(pNewTrans = new(GetHeap()) JoinClusterTrans(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
			//BR_ENTITY_MESSAGE(Message::ClusterServer::NewServerServiceJoinedC2SEvt)			{ svrMemReturn(pNewTrans = new(GetHeap()) ClusterNewServerServiceJoinedC2SEvtTrans(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		//}
		//BR_ENTITY_MESSAGE(Message::ClusterServer::SyncClusterServiceC2SEvt)					{ svrMemReturn(pNewTrans = new(GetHeap()) SyncClusterServiceTrans(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		//BR_ENTITY_MESSAGE(Message::ClusterServer::ClusterMasterAssignedS2CEvt)				{ svrMemReturn(pNewTrans = new(GetHeap()) ClusterMasterAssignedTrans(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		//BR_ENTITY_MESSAGE(Message::ClusterServer::ClusterMasterVoteC2SEvt)					{ svrMemReturn(pNewTrans = new(GetHeap()) ClusterMasterVoteTrans(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		//BR_ENTITY_MESSAGE(Message::ClusterServer::RequestDataSyncCmd)						{ svrMemReturn(pNewTrans = new(GetHeap()) RequestDataSyncTrans(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		//BR_ENTITY_MESSAGE(Message::ClusterServer::ClusterUpdateStatusC2SEvt)				{ svrMemReturn(pNewTrans = new(GetHeap()) ClusterUpdateStatusTrans(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		//BR_ENTITY_MESSAGE(Message::ClusterServer::ClusterUpdateWorkloadC2SEvt)				{ svrMemReturn(pNewTrans = new(GetHeap()) ClusterUpdateWorkloadTrans(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );

		Service::ClusterManager = this;
	}


	ClusterManagerServiceEntity::~ClusterManagerServiceEntity()
	{
		Service::ClusterManager = nullptr;
	}

	void ClusterManagerServiceEntity::Dispose()
	{
		super::Dispose();

		Clear();
	}

	bool ClusterManagerServiceEntity::GetIsInitialized()
	{
		int waitingCount = 0;
		m_ClusterInfoMap.for_each([&](const uint64_t& key, ClusterServiceInfo* pValue)-> bool
		{
			if (!pValue->IsZKInitialized()) waitingCount++;
			return true;
		});

		return waitingCount == 0;
	}

	void ClusterManagerServiceEntity::Clear()
	{
		m_ClusterInfoMap.for_each([](const uint64_t& key, ClusterServiceInfo* pValue)-> bool
		{
			IHeap::Delete(pValue);
			return true;
		});
		m_ClusterInfoMap.Clear();
	}



	Result ClusterManagerServiceEntity::InitializeEntity( EntityID newEntityID )
	{
		Result hr = ResultCode::SUCCESS;
		TransactionPtr pTrans;

		svrChk(super::InitializeEntity(newEntityID) );

		// Push initialization transaction
		//svrMem(pTrans = new(GetHeap()) ClusterInitializationTrans(GetHeap()));
		//svrChk(pTrans->InitializeTransaction(this));

		//svrChk(PendingTransaction(ThisThread::GetThreadID(), pTrans));

		pTrans = nullptr;

		

	Proc_End:

		return hr;
	}

	// Clear resources and change entity state to closed
	Result ClusterManagerServiceEntity::TerminateEntity()
	{
		Result hr = ResultCode::SUCCESS;
		TransactionPtr pTrans;

		svrChk(super::TerminateEntity());

		Dispose();

	Proc_End:

		return hr;
	}



	Result ClusterManagerServiceEntity::InitializeComponent()
	{
		return Service::EntityManager->AddEntity(
			EntityID(EntityFaculty::Service, (uint)ClusterID::ClusterManager),
			this);

		//return ResultCode::SUCCESS;
	}
	// Terminate component
	void ClusterManagerServiceEntity::DeinitializeComponent()
	{
	}

	// Set watch state for cluster
	Result ClusterManagerServiceEntity::SetWatchForCluster(GameID gameID, ClusterID clusterID, bool activelyConnect)
	{
		ClusterServiceInfo* pServiceInfo = GetOrSetWatchForCluster(gameID, clusterID, activelyConnect);

		return pServiceInfo != nullptr ? ResultCode::SUCCESS : ResultCode::UNEXPECTED;
	}

	ClusterServiceInfo* ClusterManagerServiceEntity::GetOrSetWatchForCluster(GameID gameID, ClusterID clusterID, bool activelyConnect)
	{
		ClusterServiceInfo* pServiceInfo = nullptr;
		ClusterSearchKey key(gameID, clusterID);

		if (m_ClusterInfoMap.find(key, pServiceInfo))
			return pServiceInfo;

		pServiceInfo = new(GetHeap()) ClusterServiceInfo(GetHeap(), gameID, clusterID, activelyConnect);
		if (pServiceInfo == nullptr || !m_ClusterInfoMap.insert(key, pServiceInfo))
		{
			IHeap::Delete(pServiceInfo);
			return nullptr;
		}

		return pServiceInfo;
	}


	// Initialize not initialized cluster entities
	// This need to be called after clusterManagerService is initialized
	Result ClusterManagerServiceEntity::InitializeNotInitializedClusterEntities()
	{
		m_ClusterInfoMap.for_each([&](const uint64_t& key, ClusterServiceInfo* pValue)-> bool
		{
			for (auto itLocal : pValue->LocalServiceEntites)
			{
				itLocal->StartInitializeTransaction();
			}
			return true;
		});

		return ResultCode::SUCCESS;
	}

	// Get cluster info
	Result ClusterManagerServiceEntity::GetClusterInfo(GameID gameID, ClusterID clusterID, ClusterServiceInfo* &pServiceInfo)
	{
		Result hr = ResultCode::SUCCESS;
		ClusterSearchKey key(gameID, clusterID);

		hr = m_ClusterInfoMap.find(key, pServiceInfo);
		if (hr)
			return hr;

		// Add new one if not exists
		// We might want to request some service after this call. connect actively
		pServiceInfo = GetOrSetWatchForCluster(gameID, clusterID, true);

	Proc_End:

		return hr;
	}


	// Get cluster service entity
	Result ClusterManagerServiceEntity::GetRandomService(GameID gameID, ClusterID clusterID, ServerServiceInformation* &pServiceInfo )
	{
		Result hr = ResultCode::SUCCESS;
		ClusterServiceInfo* pClusterServiceInfo = nullptr;
		ClusterSearchKey key(gameID, clusterID);

		hr = m_ClusterInfoMap.find(key, pClusterServiceInfo);
		if ((!hr))
			return hr;

		if (pClusterServiceInfo->Services.size() == 0)
			return ResultCode::FAIL;

		auto selected = pClusterServiceInfo->m_LatestSelected.fetch_add(1, std::memory_order_relaxed);
		pServiceInfo = pClusterServiceInfo->Services.GetValueAt(selected % pClusterServiceInfo->Services.size());

		return hr;
	}

	// Get cluster service entity
	Result ClusterManagerServiceEntity::GetShardService(GameID gameID, ClusterID clusterID, uint64_t shardKey, ServerServiceInformation* &pServiceInfo)
	{
		Result hr = ResultCode::SUCCESS;
		ClusterServiceInfo* pClusterServiceInfo = nullptr;
		ClusterSearchKey key(gameID, clusterID);

		hr = m_ClusterInfoMap.find(key, pClusterServiceInfo);
		if ((!hr))
			return hr;

		if (pClusterServiceInfo->Services.size() == 0)
			return ResultCode::FAIL;

		pServiceInfo = pClusterServiceInfo->Services.GetValueAt(shardKey % pClusterServiceInfo->Services.size());

		return hr;
	}

	// Get cluster service entity
	Result ClusterManagerServiceEntity::GetNextService(Svr::ServerServiceInformation* pServiceInfo, Svr::ServerServiceInformation* &pNextServiceInfo)
	{
		Result hr = ResultCode::SUCCESS;
		ClusterServiceInfo* pClusterServiceInfo = nullptr;
		ClusterSearchKey key(pServiceInfo->GetGameID(), pServiceInfo->GetClusterID());

		pNextServiceInfo = nullptr;

		hr = m_ClusterInfoMap.find(key, pClusterServiceInfo);
		if (!hr)
			return hr;

		if (pClusterServiceInfo->Services.size() == 0)
			return ResultCode::FAIL;

		pNextServiceInfo = pServiceInfo;
		auto itService = pClusterServiceInfo->Services[pServiceInfo->GetNodeNameCrc()];
		if (!itService.IsValid())
		{
			return ResultCode::UNEXPECTED;
		}

		auto selectedIndex = (itService.GetIndex() + 1 % pClusterServiceInfo->Services.size());

		pNextServiceInfo = pClusterServiceInfo->Services.GetValueAt(selectedIndex);

		return hr;
	}

	// Add cluster service entity
	Result ClusterManagerServiceEntity::AddClusterServiceEntity( ClusteredServiceEntity* pServiceEntity, ServerServiceInformation* &pServiceInfo)
	{
		Result hr = ResultCode::SUCCESS;
		ClusterServiceInfo* pClusterServiceInfo = nullptr;
		ClusterSearchKey key;

		svrChkPtr(pServiceEntity);

		key = ClusterSearchKey(pServiceEntity->GetGameID(), pServiceEntity->GetClusterID());
		pClusterServiceInfo = GetOrSetWatchForCluster(pServiceEntity->GetGameID(), pServiceEntity->GetClusterID(), pServiceEntity->GetActivelyConnectRemote());
		if (pClusterServiceInfo == nullptr)
			return ResultCode::UNEXPECTED;

		pServiceInfo = pClusterServiceInfo->NewLocalService(pServiceEntity);

		// TODO: add to zk

	Proc_End:

		return hr;
	}

	Result ClusterManagerServiceEntity::UpdateWorkLoad(ClusteredServiceEntity* pServiceEntity)
	{
		Result hr = ResultCode::SUCCESS;
		ClusterServiceInfo* pClusterServiceInfo = nullptr;
		ClusterSearchKey key;
		ServerServiceInformation* pServiceInfo = nullptr;

		svrChkPtr(pServiceEntity);

		key = ClusterSearchKey(pServiceEntity->GetGameID(), pServiceEntity->GetClusterID());
		pClusterServiceInfo = GetOrSetWatchForCluster(pServiceEntity->GetGameID(), pServiceEntity->GetClusterID(), pServiceEntity->GetActivelyConnectRemote());
		if (pClusterServiceInfo == nullptr)
			return ResultCode::UNEXPECTED;

		svrChk(pClusterServiceInfo->Services.find(pServiceEntity->GetMyServiceInfo()->GetNodeNameCrc(), pServiceInfo));

		pServiceEntity->GetMyServiceInfo()->GetWorkload();
		// TODO: update to zk


	Proc_End:

		return hr;
	}
	
	
	Result ClusterManagerServiceEntity::UpdateServiceStatus(ClusteredServiceEntity* pServiceEntity)
	{
		Result hr = ResultCode::SUCCESS;
		ClusterServiceInfo* pClusterServiceInfo = nullptr;
		ClusterSearchKey key;
		ServerServiceInformation* pServiceInfo = nullptr;

		svrChkPtr(pServiceEntity);

		key = ClusterSearchKey(pServiceEntity->GetGameID(), pServiceEntity->GetClusterID());
		pClusterServiceInfo = GetOrSetWatchForCluster(pServiceEntity->GetGameID(), pServiceEntity->GetClusterID(), pServiceEntity->GetActivelyConnectRemote());
		if (pClusterServiceInfo == nullptr)
			return ResultCode::UNEXPECTED;

		svrChk(pClusterServiceInfo->Services.find(pServiceEntity->GetMyServiceInfo()->GetNodeNameCrc(), pServiceInfo));

		pServiceEntity->GetMyServiceInfo()->GetServiceStatus();
		// TODO: update to zk


	Proc_End:

		return hr;
	}
	
	Result ClusterManagerServiceEntity::RegisterServiceMessageHandler( ServerEntity *pServerEntity )
	{
		// Do not use parent service mapping
		//ReplicaClusterServiceEntity::RegisterServiceMessageHandler( pServerEntity );

		//// only entity has some special operations
		//if( BrServer::GetInstance()->GetNetClass() == NetClass::Entity )
		//{
		//	pServerEntity->BR_ENTITY_MESSAGE(Message::ClusterServer::GetClusterMemberListCmd)				{ svrMemReturn(pNewTrans = new(GetHeap()) ClusterGetMemberListTrans(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		//	pServerEntity->BR_ENTITY_MESSAGE(Message::ClusterServer::JoinClusterCmd)						{ svrMemReturn(pNewTrans = new(GetHeap()) JoinClusterTransForEntityServer(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		//	pServerEntity->BR_ENTITY_MESSAGE(Message::ClusterServer::GetLowestWorkloadClusterMemberCmd)		{ svrMemReturn(pNewTrans = new(GetHeap()) GetLowestWorkloadClusterMemberTrans(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		//	pServerEntity->BR_ENTITY_MESSAGE(Message::ClusterServer::NewServerServiceJoinedC2SEvt)			{ svrMemReturn(pNewTrans = new(GetHeap()) ClusterNewServerServiceJoinedC2SEvtEntityTrans(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		//}
		//else
		//{
			//pServerEntity->BR_ENTITY_MESSAGE(Message::ClusterServer::JoinClusterCmd)						{ svrMemReturn(pNewTrans = new(GetHeap()) JoinClusterTrans(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
			//pServerEntity->BR_ENTITY_MESSAGE(Message::ClusterServer::NewServerServiceJoinedC2SEvt)			{ svrMemReturn(pNewTrans = new(GetHeap()) ClusterNewServerServiceJoinedC2SEvtTrans(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		//}
		//pServerEntity->BR_ENTITY_MESSAGE(Message::ClusterServer::SyncClusterServiceC2SEvt)					{ svrMemReturn(pNewTrans = new(GetHeap()) SyncClusterServiceTrans(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		//pServerEntity->BR_ENTITY_MESSAGE(Message::ClusterServer::ClusterMasterAssignedS2CEvt)				{ svrMemReturn(pNewTrans = new(GetHeap()) ClusterMasterAssignedTrans(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		//pServerEntity->BR_ENTITY_MESSAGE(Message::ClusterServer::ClusterMasterVoteC2SEvt)					{ svrMemReturn(pNewTrans = new(GetHeap()) ClusterMasterVoteTrans(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		//pServerEntity->BR_ENTITY_MESSAGE(Message::ClusterServer::RequestDataSyncCmd)						{ svrMemReturn(pNewTrans = new(GetHeap()) RequestDataSyncTrans(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		//pServerEntity->BR_ENTITY_MESSAGE(Message::ClusterServer::ClusterUpdateStatusC2SEvt)					{ svrMemReturn(pNewTrans = new(GetHeap()) ClusterUpdateStatusTrans(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		//pServerEntity->BR_ENTITY_MESSAGE(Message::ClusterServer::ClusterUpdateWorkloadC2SEvt)				{ svrMemReturn(pNewTrans = new(GetHeap()) ClusterUpdateWorkloadTrans(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );

		return ResultCode::SUCCESS;
	}


	Result ClusterManagerServiceEntity::TickUpdate(TimerAction *pAction)
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(super::TickUpdate(pAction) );

	Proc_End:

		return hr;

	}
	
}; // namespace Svr {
}; // namespace SF {



