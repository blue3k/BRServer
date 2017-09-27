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
#include "SvrTrace.h"
#include "Service/ServerService.h"

#include "Protocol/Message/ClusterServerMsgClass.h"
#include "ServiceEntity/ClusterServiceTrans.h"
#include "ServiceEntity/ClusterManagerServiceEntity.h"
#include "ServiceEntity/ClusterManagerServiceTrans.h"
#include "Protocol/ServerService/ClusterServerService.h"


namespace SF {
namespace Svr {



	//////////////////////////////////////////////////////////////////////////
	//
	//	 informations
	//


	ClusterServiceInfo_Impl::ClusterServiceInfo_Impl(IHeap& heap, GameID gameID, ClusterID clusterID, bool activelyConnect)
		: ZooKeeperWatcher(heap)
		, ClusterServiceInfo(heap)
		, m_Heap(heap)
		, m_ActivelyConnect(activelyConnect)
		, m_ClusterKey(gameID, clusterID)
		, m_ClusterPath(heap)
		, m_LatestSelected(0)
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

	ClusterServiceInfo_Impl::~ClusterServiceInfo_Impl()
	{
		for (auto itService : Services)
		{
			IHeap::Delete(itService);
		}
		Services.Clear();

		LocalServiceEntites.Clear();
	}

	void ClusterServiceInfo_Impl::InitZK()
	{
		auto zkSession = Service::ZKSession->GetZooKeeperSession();
		if (zkSession == nullptr || !zkSession->IsConnected())
			return;

		if (!zkSession->Exists(m_ClusterPath))
		{
			String outPath(GetHeap());
			Result result = zkSession->Create(m_ClusterPath, std::string(""), nullptr, 0, outPath);
			if (!result)
				return;
		}

		// set up watcher and get children
		DownloadServiceInfo();

		m_ZKInitialized = true;
	}


	Result ClusterServiceInfo_Impl::GetNodeValue(const String& nodePath, Json::Value& jsonValue)
	{
		StaticArray<uint8_t, 1024> valueBuffer(GetHeap());
		auto zkSession = Service::ZKSession->GetZooKeeperSession();
		if (zkSession == nullptr || !zkSession->IsConnected())
			return ResultCode::FAIL;

		Result result = zkSession->Get(nodePath, jsonValue);

		return result;
	}

	Result ClusterServiceInfo_Impl::SetNodeValue(const String& nodePath, const Json::Value& jsonValue)
	{
		Json::FastWriter writer;
		auto zkSession = Service::ZKSession->GetZooKeeperSession();
		if (zkSession == nullptr || !zkSession->IsConnected())
			return ResultCode::FAIL;

		auto stringValue = std::forward<std::string>(writer.write(jsonValue));

		return zkSession->Set(nodePath, stringValue);
	}




	Json::Value ClusterServiceInfo_Impl::ToJsonNetPrivate(const NetAddress& privateNet) const
	{
		Json::Value itemValue(Json::objectValue);

		itemValue["IP"] = privateNet.Address;
		itemValue["Port"] = Json::Value(privateNet.Port);

		return itemValue;
	}


	Result ClusterServiceInfo_Impl::ParseNetPrivate(const Json::Value& itemValue, NetAddress& privateNet)
	{
		auto IP = itemValue.get("IP", Json::Value(""));

		StrUtil::StringCpy(privateNet.Address, IP.asCString());
		privateNet.Port = itemValue.get("Port", Json::Value(0)).asUInt();

		return ResultCode::SUCCESS;
	}

	ServerServiceInformation* ClusterServiceInfo_Impl::NewLocalService(ClusteredServiceEntity* pServiceEntity)
	{
		Result hr;
		ServerServiceInformation* pNewServiceInfo = nullptr;
		std::string stringValue;
		Json::FastWriter writer;

		pNewServiceInfo = new(GetHeap()) ServerServiceInformation(
			pServiceEntity->GetGameID(),
			pServiceEntity->GetClusterID(),
			pServiceEntity->GetEntityUID(),
			pServiceEntity->GetServerEntity(),
			pServiceEntity->GetClusterMembership()
		);

		auto oldInfo = Services.find(pNewServiceInfo->GetNodeNameCrc());
		if (oldInfo != nullptr)
		{
			// Sometimes zookeeper has node information from previous execution
			IHeap::Delete(pNewServiceInfo);
			pNewServiceInfo = oldInfo;
			assert(pServiceEntity->GetEntityUID() == oldInfo->GetEntityUID());
			assert(pServiceEntity->GetServerEntity() == oldInfo->GetServerEntity());
		}
		else
		{
			hr = Services.Insert(pNewServiceInfo->GetNodeNameCrc(), pNewServiceInfo);
			if (!hr)
			{
				assert(false); // Huh?
				if (oldInfo == nullptr)
					IHeap::Delete(pNewServiceInfo);
				return nullptr;
			}
		}

		for (auto itLocalService : LocalServiceEntites)
		{
			if (itLocalService == pServiceEntity)
			{
				assert(false);
				return pNewServiceInfo; // insert twice?
			}
		}

		svrChk(LocalServiceEntites.push_back(pServiceEntity));

		pServiceEntity->SetMyServiceInfo(pNewServiceInfo);


		AddLocalServiceInfoToServer(pNewServiceInfo);

	Proc_End:

		if (!hr)
		{
			IHeap::Delete(pNewServiceInfo);
			return nullptr;
		}

		return pNewServiceInfo;
	}

	void ClusterServiceInfo_Impl::DownloadServiceInfo()
	{
		auto zkSession = Service::ZKSession->GetZooKeeperSession();
		if (zkSession == nullptr || !zkSession->IsConnected())
			return;

		svrTrace(Debug, "ZK requesting service list, GameID:{0} ClusterID:{1}", Enum<GameID>().GetValueName(m_ClusterKey.GameClusterID), Enum<ClusterID>().GetValueName(m_ClusterKey.ServiceClusterID));

		zkSession->AGetChildren(m_ClusterPath, this);
	}

	void ClusterServiceInfo_Impl::UploadLocalServiceInfo()
	{
		auto zkSession = Service::ZKSession->GetZooKeeperSession();
		if (zkSession == nullptr || !zkSession->IsConnected())
			return;

		
//		zkSession->
	}


	/////////////////////////////////////////////
	//
	//	Overridable Event handling
	//

	Result ClusterServiceInfo_Impl::OnNewEvent(const ZKEvent& eventOut)
	{
		svrTrace(Debug, "ZKEvent:{0}, GameID:{1} ClusterID:{2}", eventOut.EventType, Enum<GameID>().GetValueName(m_ClusterKey.GameClusterID), Enum<ClusterID>().GetValueName(m_ClusterKey.ServiceClusterID));
		if (eventOut.EventType == ZooKeeper::EVENT_CHILD)
		{
			DownloadServiceInfo();
		}
		else if (eventOut.EventType == ZooKeeper::EVENT_SESSION)
		{
			if (eventOut.State == ZooKeeper::STATE_CONNECTED)
			{
				DownloadServiceInfo();
			}
		}
		else
		{

		}

		return ResultCode::SUCCESS;
	}

	void ClusterServiceInfo_Impl::OnComlition(ZooKeeperTask& pTask)
	{
		if (!pTask.Result)
		{
			assert(false);
			return;
		}
	}

	void ClusterServiceInfo_Impl::OnStatComlition(StatTask& pTask)
	{

	}

	void ClusterServiceInfo_Impl::OnDataComlition(DataTask& pTask)
	{

	}


	Result ClusterServiceInfo_Impl::AddServiceInfo(const char* nodeName, FixedString nodeNameCrc)
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

		svrChk(ParseNetPrivate(jsonValue.get("NetAddress", ""), netAddress));
		entityUID.UID = jsonValue.get("EntityUID", Json::Value(0)).asUInt64();

		svrChk(Service::ServerEntityManager->GetOrRegisterServer(entityUID.GetServerID(), NetClass::Server, netAddress, pServerEntity));

		svrMem(pNewServiceInfo = new(GetHeap()) ServerServiceInformation(m_ClusterKey.GameClusterID, m_ClusterKey.ServiceClusterID, entityUID, pServerEntity, ClusterMembership::Slave));

		svrChk(Services.Insert(nodeNameCrc, pNewServiceInfo));

		svrTrace(Debug, "ZK ServiceInfo Added({0}), GameID:{1} ClusterID:{2}", nodeName, Enum<GameID>().GetValueName(m_ClusterKey.GameClusterID), Enum<ClusterID>().GetValueName(m_ClusterKey.ServiceClusterID));

		// Connect to the service entity
		if (m_ActivelyConnect)
		{
			// TODO: Serverentity will try to connect anyway. Need to change it
			//nodePath
		}

	Proc_End:

		return hr;
	}

	void ClusterServiceInfo_Impl::AddLocalServiceInfoToServer(ServerServiceInformation* pLocalServiceInfo)
	{
		char nodePath[512];
		Json::Value nodeValue(Json::objectValue);
		NetAddress privateAddress;

		// add to zk
		auto zkSession = Service::ZKSession->GetZooKeeperSession();
		if (zkSession == nullptr || !zkSession->IsConnected())
			return;

		privateAddress = pLocalServiceInfo->GetServerEntity()->GetPrivateNetAddress();
		assert(privateAddress.Port != 0 && !StrUtil::IsNullOrEmpty(privateAddress.Address));

		StrUtil::Format(nodePath, "{0}/{1}", m_ClusterPath, pLocalServiceInfo->GetNodeName());

		nodeValue["EntityUID"] = Json::Value(pLocalServiceInfo->GetEntityUID().UID);
		nodeValue["NetAddress"] = ToJsonNetPrivate(privateAddress);

		svrTrace(Debug, "ZK local service added ({0}), GameID:{1} ClusterID:{2}", pLocalServiceInfo->GetNodeName(), Enum<GameID>().GetValueName(m_ClusterKey.GameClusterID), Enum<ClusterID>().GetValueName(m_ClusterKey.ServiceClusterID));

		zkSession->ADelete(nodePath); // In case something is there, delete it
		zkSession->ACreate(nodePath, nodeValue, nullptr, zkSession->NODE_FLAG_EPHEMERAL);
	}

	void ClusterServiceInfo_Impl::OnStringsComlition(StringsTask& pTask)
	{
		if (!pTask.Result)
		{
			//assert(false);
			return;
		}

		SortedSet<FixedString> listAlreadyHave(GetHeap(), static_cast<int>(Services.size()));
		for (auto itService : Services)
		{
			listAlreadyHave.Insert(itService->GetNodeNameCrc());
		}

		// This should be getchildren list because that's the only thing have string list result
		for (auto& nodeName : pTask.ResultStrings)
		{
			FixedString nodeNameCrc = nodeName;
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
				AddServiceInfo(nodeName, nodeNameCrc);
			}
		}

		// If anything not in the list should be removed
		auto serverID = GetServerUID();
		for (auto itRemove : listAlreadyHave)
		{
			ServerServiceInformation* pRemove = nullptr;
			if (!Services.Find(itRemove, pRemove))
				continue;

			// local service, but missing from server. we need to add them
			if (pRemove->GetServerID() == serverID)
			{
				AddLocalServiceInfoToServer(pRemove);
				continue;
			}

			svrTrace(Debug, "ZK service removed ({0}), GameID:{1} ClusterID:{2}", pRemove->GetNodeName(), Enum<GameID>().GetValueName(m_ClusterKey.GameClusterID), Enum<ClusterID>().GetValueName(m_ClusterKey.ServiceClusterID));
			Services.Remove(itRemove, pRemove);
			if (pRemove != nullptr)
			{
				IHeap::Delete(pRemove);
			}
		}

		m_GetChildrenTask = nullptr;
	}

	void ClusterServiceInfo_Impl::OnStringsStatComlition(StringsStatTask& pTask)
	{
		if (!pTask.Result)
		{
			assert(false);
			return;
		}


	}

	void ClusterServiceInfo_Impl::OnStringComlition(StringTask& pTask)
	{
		if (!pTask.Result)
		{
			assert(false);
			return;
		}

		// This should be create for new path

	}

	void ClusterServiceInfo_Impl::OnACLComlition(ACLTask& pTask)
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
		m_ClusterInfoMap.for_each([&](const uint64_t& key, ClusterServiceInfo_Impl* pValue)-> bool
		{
			if (!pValue->IsZKInitialized()) waitingCount++;
			return true;
		});

		return waitingCount == 0;
	}

	void ClusterManagerServiceEntity::Clear()
	{
		m_ClusterInfoMap.for_each([](const uint64_t& key, ClusterServiceInfo_Impl* pValue)-> bool
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
		String outPath(GetHeap());
		String gameClusterPath(GetHeap());

		svrChk(super::InitializeEntity(newEntityID) );

		auto zkSession = Service::ZKSession->GetZooKeeperSession();
		if (zkSession == nullptr || !zkSession->IsConnected())
		{
			svrTrace(Error, "Zookeeper session server hasn't ready!");
			return ResultCode::UNEXPECTED;
		}


		if (!zkSession->Exists(ClusterManagerServiceEntity::ServiceBasePath))
		{
			// Create game cluster path if not exist
			zkSession->Create(ClusterManagerServiceEntity::ServiceBasePath, std::string(""), nullptr, 0, outPath);
		}

		// Create game cluster path if not exist
		for(int iGame = 0; iGame < Enum<GameID>().GetNumberOfValues(); iGame++)
		{
			if ((GameID)iGame == GameID::Invalid)
				continue;

			auto gameName = Enum<GameID>().GetValueName((GameID)iGame);
			gameClusterPath.Format("{0}/{1}", ClusterManagerServiceEntity::ServiceBasePath, gameName);
			if (!zkSession->Exists(gameClusterPath))
				zkSession->Create(gameClusterPath, std::string(""), nullptr, 0, outPath);
		}

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
		return ResultCode::SUCCESS;
	}
	// Terminate component
	void ClusterManagerServiceEntity::DeinitializeComponent()
	{
	}

	// Set watch state for cluster
	Result ClusterManagerServiceEntity::SetWatchForCluster(GameID gameID, ClusterID clusterID, bool activelyConnect)
	{
		ClusterServiceInfo_Impl* pServiceInfo = static_cast<ClusterServiceInfo_Impl*>(GetOrSetWatchForCluster(gameID, clusterID, activelyConnect));

		return pServiceInfo != nullptr ? ResultCode::SUCCESS : ResultCode::UNEXPECTED;
	}

	ClusterServiceInfo* ClusterManagerServiceEntity::GetOrSetWatchForCluster(GameID gameID, ClusterID clusterID, bool activelyConnect)
	{
		ClusterServiceInfo_Impl* pServiceInfo = nullptr;
		ClusterSearchKey key(gameID, clusterID);

		if (m_ClusterInfoMap.find(key, pServiceInfo))
			return pServiceInfo;

		pServiceInfo = new(GetHeap()) ClusterServiceInfo_Impl(GetHeap(), gameID, clusterID, activelyConnect);
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
		m_ClusterInfoMap.for_each([&](const uint64_t& key, ClusterServiceInfo_Impl* pValue)-> bool
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
	ClusterServiceInfo* ClusterManagerServiceEntity::GetClusterInfo(GameID gameID, ClusterID clusterID)
	{
		Result hr = ResultCode::SUCCESS;
		ClusterSearchKey key(gameID, clusterID);
		ClusterServiceInfo_Impl* pServiceInfo = nullptr;

		hr = m_ClusterInfoMap.find(key, pServiceInfo);
		if (hr)
		{
			return pServiceInfo;
		}

		// Add new one if not exists
		// We might want to request some service after this call. connect actively
		return GetOrSetWatchForCluster(gameID, clusterID, true);
	}


	// Get cluster service entity
	Result ClusterManagerServiceEntity::GetRandomService(GameID gameID, ClusterID clusterID, ServerServiceInformation* &pServiceInfo )
	{
		Result hr = ResultCode::SUCCESS;
		ClusterServiceInfo_Impl* pClusterServiceInfo = nullptr;
		ClusterSearchKey key(gameID, clusterID);

		hr = m_ClusterInfoMap.find(key, pClusterServiceInfo);
		if (!hr)
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
		ClusterServiceInfo_Impl* pClusterServiceInfo = nullptr;
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
		ClusterServiceInfo_Impl* pClusterServiceInfo = nullptr;
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
		ClusterServiceInfo_Impl* pClusterServiceInfo = nullptr;
		ClusterSearchKey key;

		svrChkPtr(pServiceEntity);

		key = ClusterSearchKey(pServiceEntity->GetGameID(), pServiceEntity->GetClusterID());
		pClusterServiceInfo = static_cast<ClusterServiceInfo_Impl*>(GetOrSetWatchForCluster(pServiceEntity->GetGameID(), pServiceEntity->GetClusterID(), pServiceEntity->GetActivelyConnectRemote()));
		if (pClusterServiceInfo == nullptr)
			return ResultCode::UNEXPECTED;

		pServiceInfo = pClusterServiceInfo->NewLocalService(pServiceEntity);


	Proc_End:

		return hr;
	}

	Result ClusterManagerServiceEntity::UpdateWorkLoad(ClusteredServiceEntity* pServiceEntity)
	{
		Result hr = ResultCode::SUCCESS;
		ClusterServiceInfo_Impl* pClusterServiceInfo = nullptr;
		ClusterSearchKey key;
		ServerServiceInformation* pServiceInfo = nullptr;

		svrChkPtr(pServiceEntity);

		key = ClusterSearchKey(pServiceEntity->GetGameID(), pServiceEntity->GetClusterID());
		pClusterServiceInfo = static_cast<ClusterServiceInfo_Impl*>(GetOrSetWatchForCluster(pServiceEntity->GetGameID(), pServiceEntity->GetClusterID(), pServiceEntity->GetActivelyConnectRemote()));
		if (pClusterServiceInfo == nullptr)
			return ResultCode::UNEXPECTED;

		if (pServiceEntity->GetMyServiceInfo() != nullptr)
		{
			svrChk(pClusterServiceInfo->Services.find(pServiceEntity->GetMyServiceInfo()->GetNodeNameCrc(), pServiceInfo));

			// TODO: update to zk
			pServiceEntity->GetMyServiceInfo()->GetWorkload();
		}


	Proc_End:

		return hr;
	}
	
	
	Result ClusterManagerServiceEntity::UpdateServiceStatus(ClusteredServiceEntity* pServiceEntity)
	{
		Result hr = ResultCode::SUCCESS;
		ClusterServiceInfo_Impl* pClusterServiceInfo = nullptr;
		ClusterSearchKey key;
		ServerServiceInformation* pServiceInfo = nullptr;

		svrChkPtr(pServiceEntity);

		key = ClusterSearchKey(pServiceEntity->GetGameID(), pServiceEntity->GetClusterID());
		pClusterServiceInfo = static_cast<ClusterServiceInfo_Impl*>(GetOrSetWatchForCluster(pServiceEntity->GetGameID(), pServiceEntity->GetClusterID(), pServiceEntity->GetActivelyConnectRemote()));
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



