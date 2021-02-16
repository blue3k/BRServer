////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 Kyungkun Ko
// 
// Author : KyungKun Ko
//
// Description : Server service Component entity implementation
//	
//
////////////////////////////////////////////////////////////////////////////////



#include "ServerSystemPCH.h"
#include "String/SFStrUtil.h"
#include "ServerLog/SvrLog.h"
#include "Multithread/SFThread.h"
#include "Types/BrSvrTypes.h"
#include "Net/SFNetDef.h"
#include "Server/BrServer.h"
#include "Entity/Entity.h"
#include "Entity/EntityManager.h"
#include "Component/ServerComponent.h"
#include "ServerService/ServerServiceBase.h"
#include "ServerEntity/ServerEntity.h"
#include "SvrTrace.h"
#include "Service/ServerService.h"
#include "String/SFStringFormat.h"
#include "Protocol/Message/ClusterServerMsgClass.h"
#include "ServiceEntity/ClusterServiceTrans.h"
#include "ServiceEntity/ClusterManagerServiceEntity.h"
#include "Protocol/ServerService/ClusterServerService.h"


namespace SF {
namespace Svr {



	//////////////////////////////////////////////////////////////////////////
	//
	//	 informations
	//


	ClusterServiceInfo_Impl::ClusterServiceInfo_Impl(IHeap& heap, GameID gameID, ClusterID clusterID)
		: ZookeeperWatcher(heap)
		, ClusterServiceInfo(heap)
		, m_Heap(heap)
		, m_ClusterKey(gameID, clusterID)
		, m_ClusterPath(heap)
		, m_LatestSelected(0)
	{
		if (m_ClusterKey.Components.GameClusterID != nullptr)
		{
			m_ClusterPath.Format("{0}/{1}/{2}", Service::ServerConfig->DataCenter.Path, gameID, Enum<ClusterID>().GetValueName(clusterID));
		}
		else
		{
			m_ClusterPath.Format("{0}/{1}", Service::ServerConfig->DataCenter.Path, Enum<ClusterID>().GetValueName(clusterID));
		}

		InitZK();
	}

	ClusterServiceInfo_Impl::~ClusterServiceInfo_Impl()
	{
		for (auto itService : Services)
		{
			IHeap::Delete(itService.GetValue());
		}
		Services.Clear();
	}

	void ClusterServiceInfo_Impl::InitZK()
	{
		auto zkSession = Service::ZKSession->GetZookeeperSession();
		if (zkSession == nullptr || !zkSession->IsConnected())
			return;

		if (!zkSession->Exists(m_ClusterPath))
		{
			String outPath(GetHeap());
			Result result = zkSession->Create(m_ClusterPath, Json::Value(Json::objectValue), nullptr, 0, outPath);
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
		auto zkSession = Service::ZKSession->GetZookeeperSession();
		if (zkSession == nullptr || !zkSession->IsConnected())
			return ResultCode::FAIL;

		Result result = zkSession->Get(nodePath, jsonValue);

		return result;
	}

	Result ClusterServiceInfo_Impl::SetNodeValue(const String& nodePath, const Json::Value& jsonValue)
	{
		auto zkSession = Service::ZKSession->GetZookeeperSession();
		if (zkSession == nullptr || !zkSession->IsConnected())
			return ResultCode::FAIL;

		Json::StreamWriterBuilder builder;
		builder["indentation"] = "";
		auto stringValue = std::forward<std::string>(Json::writeString(builder, jsonValue));

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

		StrUtil::StringCopy(privateNet.Address, IP.asCString());
		privateNet.Port = itemValue.get("Port", Json::Value(0)).asUInt();

		return ResultCode::SUCCESS;
	}

	void ClusterServiceInfo_Impl::DownloadServiceInfo()
	{
		auto zkSession = Service::ZKSession->GetZookeeperSession();
		if (zkSession == nullptr || !zkSession->IsConnected())
			return;

		svrTrace(Debug, "ZK requesting service list, GameID:{0} ClusterID:{1}", m_ClusterKey.Components.GameClusterID, Enum<ClusterID>().GetValueName(m_ClusterKey.Components.ServiceClusterID));

		zkSession->AGetChildren(m_ClusterPath, this);
	}

	void ClusterServiceInfo_Impl::UploadLocalServiceInfo()
	{
		auto zkSession = Service::ZKSession->GetZookeeperSession();
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
		svrTrace(Debug, "ZKEvent:{0}, GameID:{1} ClusterID:{2}", eventOut.Components.EventType, m_ClusterKey.Components.GameClusterID, Enum<ClusterID>().GetValueName(m_ClusterKey.Components.ServiceClusterID));
		if (eventOut.Components.EventType == Zookeeper::EVENT_CHILD)
		{
			DownloadServiceInfo();
		}
		else if (eventOut.Components.EventType == Zookeeper::EVENT_SESSION)
		{
			if (eventOut.Components.State == Zookeeper::STATE_CONNECTED)
			{
				DownloadServiceInfo();
			}
		}
		else
		{

		}

		return ResultCode::SUCCESS;
	}

	void ClusterServiceInfo_Impl::OnComplition(ZookeeperTask& pTask)
	{
		if (!pTask.ZKResult)
		{
			assert(false);
			return;
		}
	}

	void ClusterServiceInfo_Impl::OnStatComplition(StatTask& pTask)
	{

	}

	void ClusterServiceInfo_Impl::OnDataComplition(DataTask& pTask)
	{

	}


	Result ClusterServiceInfo_Impl::AddServiceInfo(const char* nodeName, StringCrc64 nodeNameCrc)
	{
		Result hr;
		String nodePath(GetHeap());
		Json::Value jsonValue(Json::objectValue);
		NetAddress netAddress;
		EntityUID entityUID;
		ServerServiceInformation* pNewServiceInfo = nullptr;
		ServerEntity *pServerEntity = nullptr;

		nodePath.Format("{0}/{1}", m_ClusterPath, nodeName);

		svrCheck(GetNodeValue(nodePath, jsonValue));

		auto endpointAddress = jsonValue.get("EndpointAddress", "");
		entityUID.UID = jsonValue.get("EntityUID", Json::Value(0)).asUInt64();
		auto customAttributes = jsonValue["Custom"];

		MessageEndpoint* pEndpoint{};

		svrCheckMem(pNewServiceInfo = new(GetHeap()) ServerServiceInformation(m_ClusterKey.Components.GameClusterID, m_ClusterKey.Components.ServiceClusterID, entityUID, pEndpoint, customAttributes));

		svrCheck(Services.Insert(nodeNameCrc, pNewServiceInfo));

		svrTrace(Info, "ClusterManager ServiceInfo Added({0}), GameID:{1} ClusterID:{2}, entityUID:{3}", nodeName, m_ClusterKey.Components.GameClusterID, Enum<ClusterID>().GetValueName(m_ClusterKey.Components.ServiceClusterID), entityUID);

		return hr;
	}

	void ClusterServiceInfo_Impl::OnStringsComplition(StringsTask& pTask)
	{
		if (!pTask.ZKResult)
		{
			//assert(false);
			return;
		}

		SortedSet<StringCrc64> listAlreadyHave(GetHeap(), static_cast<int>(Services.size()));
		for (auto itService : Services)
		{
			listAlreadyHave.Insert(itService.GetValue()->GetNodeNameCrc());
		}

		// This should be getchildren list because that's the only thing have string list result
		for (auto& nodeName : pTask.ResultStrings)
		{
			StringCrc64 nodeNameCrc = (const char*)nodeName;
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

			svrTrace(Info, "ZK service removed ({0}), GameID:{1} ClusterID:{2}", pRemove->GetNodeName(), m_ClusterKey.Components.GameClusterID, Enum<ClusterID>().GetValueName(m_ClusterKey.Components.ServiceClusterID));
			Services.Remove(itRemove, pRemove);
			if (pRemove != nullptr)
			{
				IHeap::Delete(pRemove);
			}
		}

		m_GetChildrenTask = nullptr;
	}

	void ClusterServiceInfo_Impl::OnStringsStatComplition(StringsStatTask& pTask)
	{
		if (!pTask.ZKResult)
		{
			assert(false);
			return;
		}


	}

	void ClusterServiceInfo_Impl::OnStringComplition(StringTask& pTask)
	{
		if (!pTask.ZKResult)
		{
			assert(false);
			return;
		}

		// This should be create for new path

	}

	//void ClusterServiceInfo_Impl::OnACLComplition(ACLTask& pTask)
	//{

	//}





	//////////////////////////////////////////////////////////////////////////
	//
	//	Entity informations
	//

	constexpr StringCrc64 ClusterManagerServiceEntity::TypeName;


	ClusterManagerServiceEntity::ClusterManagerServiceEntity()
		: MasterEntity()
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

		//Service::ClusterManager = this;
	}


	ClusterManagerServiceEntity::~ClusterManagerServiceEntity()
	{
		//Service::ClusterManager = nullptr;
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
		auto zkSession = Service::ZKSession->GetZookeeperSession();

		svrCheck(super::InitializeEntity(newEntityID) );

		if (zkSession == nullptr || !zkSession->IsConnected())
		{
			svrTrace(Error, "Zookeeper session server hasn't ready!");
			return ResultCode::UNEXPECTED;
		}

		if (!zkSession->Exists(Service::ServerConfig->DataCenter.Path))
		{
			// Create game cluster path if not exist
			zkSession->Create(Service::ServerConfig->DataCenter.Path, Json::Value(Json::objectValue), nullptr, 0, outPath);
		}

		pTrans = nullptr;

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
	Result ClusterManagerServiceEntity::WatchForService(GameID gameID, ClusterID clusterID)
	{
		ClusterServiceInfo_Impl* pServiceInfo = static_cast<ClusterServiceInfo_Impl*>(GetOrSetWatchForCluster(gameID, clusterID));

		return pServiceInfo != nullptr ? ResultCode::SUCCESS : ResultCode::UNEXPECTED;
	}

	ClusterServiceInfo* ClusterManagerServiceEntity::GetOrSetWatchForCluster(GameID gameID, ClusterID clusterID)
	{
		ClusterServiceInfo_Impl* pServiceInfo = nullptr;
		ClusterSearchKey key(gameID, clusterID);

		if (m_ClusterInfoMap.find(key, pServiceInfo))
			return pServiceInfo;

		svrTrace(Info, "Adding service watcher for cluster, GameID:{0} ClusterID:{1}, {2}", gameID, Enum<ClusterID>().GetValueName(clusterID), clusterID);

		pServiceInfo = new(GetHeap()) ClusterServiceInfo_Impl(GetHeap(), gameID, clusterID);
		if (pServiceInfo == nullptr || !m_ClusterInfoMap.insert(key, pServiceInfo))
		{
			svrTrace(Error, "Failed to Add service watcher for cluster, GameID:{0} ClusterID:{1}, {2}", gameID, Enum<ClusterID>().GetValueName(clusterID), clusterID);
			IHeap::Delete(pServiceInfo);
			return nullptr;
		}

		return pServiceInfo;
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
		return GetOrSetWatchForCluster(gameID, clusterID);
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
	Result ClusterManagerServiceEntity::GetNextService(ServerServiceInformation* pServiceInfo, ServerServiceInformation* &pNextServiceInfo)
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

	Result ClusterManagerServiceEntity::RegisterServiceMessageHandler()
	{
		// Do not use parent service mapping
		//ReplicaClusterServiceEntity::RegisterServiceMessageHandler();

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



