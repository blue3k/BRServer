////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) Kyungkun Ko
// 
// Author : KyungKun Ko
//
// Description : service directory implementation
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
#include "ServiceEntity/ServiceEntity.h"
#include "SvrTrace.h"
#include "Service/ServerService.h"
#include "ServiceEntity/ServiceDirectoryManager.h"
#include "String/SFStringFormat.h"
#include "ServerConfig/SFEnum.h"
#include "Variable/SFVariableTable.h"


namespace SF {


	//////////////////////////////////////////////////////////////////////////
	//
	//	 informations
	//


	String ServiceCluster::GetClusterPath(GameID gameID, ClusterID clusterID)
	{
		String clusterPath;
		if (gameID != nullptr)
		{
			clusterPath.Format("{0}/{1}/{2}", Service::ServerConfig->DataCenter.Path, gameID, Enum<ClusterID>().GetValueName(clusterID));
		}
		else
		{
			clusterPath.Format("{0}/{1}", Service::ServerConfig->DataCenter.Path, Enum<ClusterID>().GetValueName(clusterID));
		}

		return Forward<String>(clusterPath);
	}

	ServiceCluster::ServiceCluster(IHeap& heap, GameID gameID, ClusterID clusterID)
		: ZookeeperWatcher(heap)
		, m_Heap(heap)
		, m_ClusterKey(gameID, clusterID)
		, m_ClusterPath(heap)
		, m_LatestSelected(0)
		, m_Services(GetHeap())
	{
		m_ClusterPath = GetClusterPath(gameID, clusterID);

		InitZK();
	}

	ServiceCluster::~ServiceCluster()
	{
		for (auto itService : m_Services)
		{
			IHeap::Delete(itService.GetValue());
		}
		m_Services.Clear();
	}

	void ServiceCluster::InitZK()
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

	Result ServiceCluster::GetNodeValue(const String& nodePath, Json::Value& jsonValue)
	{
		StaticArray<uint8_t, 1024> valueBuffer(GetSystemHeap());
		auto zkSession = Service::ZKSession->GetZookeeperSession();
		if (zkSession == nullptr || !zkSession->IsConnected())
			return ResultCode::FAIL;

		Result result = zkSession->Get(nodePath, jsonValue);

		return result;
	}

	Result ServiceCluster::SetNodeValue(const String& nodePath, const Json::Value& jsonValue)
	{
		auto zkSession = Service::ZKSession->GetZookeeperSession();
		if (zkSession == nullptr || !zkSession->IsConnected())
			return ResultCode::FAIL;

		Json::StreamWriterBuilder builder;
		builder["indentation"] = "";
		auto stringValue = std::forward<std::string>(Json::writeString(builder, jsonValue));

		return zkSession->Set(nodePath, stringValue);
	}

	Result ServiceCluster::ParseMessageEndpoint(const Json::Value& jsonObject, const char* keyName, EndpointAddress& outMessageEndpoint)
	{
		auto stringValue = jsonObject.get(keyName, Json::Value(Json::stringValue));
		auto splitIndex = StrUtil::Indexof(stringValue.asCString(), '/');
		if (splitIndex < 0)
			return ResultCode::FAIL;

		String MessageEndpointrString = stringValue.asCString();
		outMessageEndpoint.MessageServer = MessageEndpointrString.SubString(0, splitIndex);
		outMessageEndpoint.Channel = MessageEndpointrString.SubString(splitIndex + 1, MessageEndpointrString.length());

		return ResultCode::SUCCESS;
	}

	void ServiceCluster::DownloadServiceInfo()
	{
		auto zkSession = Service::ZKSession->GetZookeeperSession();
		if (zkSession == nullptr || !zkSession->IsConnected())
			return;

		svrTrace(Debug, "ServiceCluster requesting service list, GameID:{0} ClusterID:{1}", m_ClusterKey.Components.GameClusterID, Enum<ClusterID>().GetValueName(m_ClusterKey.Components.ServiceClusterID));

		zkSession->AGetChildren(m_ClusterPath, this);
	}



	/////////////////////////////////////////////
	//
	//	Overridable Event handling
	//

	Result ServiceCluster::OnNewEvent(const ZKEvent& eventOut)
	{
		svrTrace(Info, "ServiceCluster ZKEvent:{0}, GameID:{1} ClusterID:{2}", Zookeeper::EventToString(eventOut.Components.EventType), m_ClusterKey.Components.GameClusterID, Enum<ClusterID>().GetValueName(m_ClusterKey.Components.ServiceClusterID));

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

	void ServiceCluster::OnComplition(ZookeeperTask& pTask)
	{
		if (!pTask.ZKResult)
		{
			assert(false);
			return;
		}
	}

	void ServiceCluster::OnStatComplition(StatTask& pTask)
	{

	}

	void ServiceCluster::OnDataComplition(DataTask& pTask)
	{

	}

	Result ServiceCluster::AddServiceInfo(const char* nodeName, StringCrc64 nodeNameCrc)
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

		EndpointAddress messageEndpointConfig;
		svrCheck(ParseMessageEndpoint(jsonValue, "Endpoint", messageEndpointConfig));
		entityUID.UID = jsonValue.get("EntityUID", Json::Value(0)).asUInt64();
		Json::Value customValue = jsonValue.get("Custom", Json::Value(Json::objectValue));

		MessageEndpoint* pEndpoint{};
		svrCheck(Service::MessageEndpointManager->AddOrGetRemoteEndpoint(entityUID, messageEndpointConfig, pEndpoint));

		svrCheckMem(pNewServiceInfo = new(GetHeap()) ServerServiceInformation(m_ClusterKey.Components.GameClusterID, m_ClusterKey.Components.ServiceClusterID, entityUID, pEndpoint, customValue));

		svrCheck(m_Services.Insert(nodeNameCrc, pNewServiceInfo));

		svrTrace(Info, "ServiceCluster ServiceInfo Added({0}), GameID:{1} ClusterID:{2}, entityUID:{3}", nodeName, m_ClusterKey.Components.GameClusterID, Enum<ClusterID>().GetValueName(m_ClusterKey.Components.ServiceClusterID), entityUID);

		return hr;
	}

	void ServiceCluster::OnStringsComplition(StringsTask& pTask)
	{
		if (!pTask.ZKResult)
		{
			//assert(false);
			return;
		}

		SortedSet<StringCrc64> listAlreadyHave(GetHeap(), static_cast<int>(m_Services.size()));
		for (auto itService : m_Services)
		{
			listAlreadyHave.Insert(itService.GetValue()->GetNodeNameCrc());
		}

		// This should be getchildren list because that's the only thing have string list result
		for (auto& nodeName : pTask.ResultStrings)
		{
			StringCrc64 nodeNameCrc = (const char*)nodeName;
			ServerServiceInformation* pFound = nullptr;
			m_Services.Find(nodeNameCrc, pFound);
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
			if (!m_Services.Find(itRemove, pRemove))
				continue;

			svrTrace(Info, "ServiceCluster service removed ({0}), GameID:{1} ClusterID:{2}", pRemove->GetNodeName(), m_ClusterKey.Components.GameClusterID, Enum<ClusterID>().GetValueName(m_ClusterKey.Components.ServiceClusterID));
			m_Services.Remove(itRemove, pRemove);
			if (pRemove != nullptr)
			{
				IHeap::Delete(pRemove);
			}
		}

		m_GetChildrenTask = nullptr;
	}

	void ServiceCluster::OnStringsStatComplition(StringsStatTask& pTask)
	{
		if (!pTask.ZKResult)
		{
			assert(false);
			return;
		}


	}

	void ServiceCluster::OnStringComplition(StringTask& pTask)
	{
		if (!pTask.ZKResult)
		{
			assert(false);
			return;
		}

		// This should be create for new path

	}

	//void ServiceCluster::OnACLComplition(ACLTask& pTask)
	//{

	//}





	//////////////////////////////////////////////////////////////////////////
	//
	//	Service directory manager
	//

	constexpr StringCrc64 ServiceDirectoryManager::TypeName;


	ServiceDirectoryManager::ServiceDirectoryManager()
		: m_ClusterInfoMap(GetSystemHeap())
	{
		Service::ServiceDirectory = this;
	}


	ServiceDirectoryManager::~ServiceDirectoryManager()
	{
		Service::ServiceDirectory = nullptr;
	}

	Result ServiceDirectoryManager::InitializeComponent()
	{
		auto zkSession = Service::ZKSession->GetZookeeperSession();

		if (zkSession == nullptr || !zkSession->IsConnected())
		{
			svrTrace(Error, "Zookeeper session server hasn't ready!");
			return ResultCode::UNEXPECTED;
		}

		String outPath;
		if (!zkSession->Exists(Service::ServerConfig->DataCenter.Path))
		{
			// Create game cluster path if not exist
			zkSession->Create(Service::ServerConfig->DataCenter.Path, Json::Value(Json::objectValue), nullptr, 0, outPath);
		}

		String gameClusterPath;
		gameClusterPath.Format("{0}/{1}", Service::ServerConfig->DataCenter.Path, Service::ServerConfig->GameClusterID);
		if (!zkSession->Exists(gameClusterPath))
		{
			// Create game cluster path if not exist
			zkSession->Create(gameClusterPath, Json::Value(Json::objectValue), nullptr, 0, outPath);
		}


		zkSession->GetWatcher().OnSessionEvent.AddDelegate(uintptr_t(this), [this](int newState)
			{
				if (newState == Zookeeper::STATE_CONNECTED)
				{
					RegisterLocalServices();
				}
				else if (newState == Zookeeper::STATE_SESSION_EXPIRED)
				{
					// timed out
				}
			});

		// Always watch for server endpoints
		WatchForService(Service::ServerConfig->GameClusterID, ClusterID::None);

		return ResultCode::SUCCESS;
	}
	// Terminate component
	void ServiceDirectoryManager::DeinitializeComponent()
	{
	}

	// Set watch state for cluster
	Result ServiceDirectoryManager::WatchForService(GameID gameID, ClusterID clusterID)
	{
		ServiceCluster* pServiceInfo = nullptr;
		ServiceClusterSearchKey key(gameID, clusterID);

		if (m_ClusterInfoMap.find(key, pServiceInfo))
			return ResultCode::SUCCESS;

		svrTrace(Info, "Adding service watcher for cluster, GameID:{0} ClusterID:{1}, {2}", gameID, Enum<ClusterID>().GetValueName(clusterID), clusterID);

		pServiceInfo = new(GetSystemHeap()) ServiceCluster(GetSystemHeap(), gameID, clusterID);
		if (pServiceInfo == nullptr || !m_ClusterInfoMap.insert(key, pServiceInfo))
		{
			svrTrace(Error, "Failed to Add service watcher for cluster, GameID:{0} ClusterID:{1}, {2}", gameID, Enum<ClusterID>().GetValueName(clusterID), clusterID);

			IHeap::Delete(pServiceInfo);
			return ResultCode::OUT_OF_MEMORY;
		}

		return pServiceInfo != nullptr ? ResultCode::SUCCESS : ResultCode::UNEXPECTED;
	}


	// Get cluster service entity
	Result ServiceDirectoryManager::GetRandomService(GameID gameID, ClusterID clusterID, ServerServiceInformation* &pServiceInfo )
	{
		Result hr = ResultCode::SUCCESS;
		ServiceCluster* pClusterServiceInfo = nullptr;
		ServiceClusterSearchKey key(gameID, clusterID);

		hr = m_ClusterInfoMap.find(key, pClusterServiceInfo);
		if (!hr)
			return hr;

		if (pClusterServiceInfo->m_Services.size() == 0)
			return ResultCode::FAIL;

		auto selected = pClusterServiceInfo->m_LatestSelected.fetch_add(1, std::memory_order_relaxed);
		pServiceInfo = pClusterServiceInfo->m_Services.GetValueAt(selected % pClusterServiceInfo->m_Services.size());

		return hr;
	}

	// Get cluster service entity
	Result ServiceDirectoryManager::GetShardService(GameID gameID, ClusterID clusterID, uint64_t shardKey, ServerServiceInformation* &pServiceInfo)
	{
		Result hr = ResultCode::SUCCESS;
		ServiceCluster* pClusterServiceInfo = nullptr;
		ServiceClusterSearchKey key(gameID, clusterID);

		hr = m_ClusterInfoMap.find(key, pClusterServiceInfo);
		if ((!hr))
			return hr;

		if (pClusterServiceInfo->m_Services.size() == 0)
			return ResultCode::FAIL;

		pServiceInfo = pClusterServiceInfo->m_Services.GetValueAt(shardKey % pClusterServiceInfo->m_Services.size());

		return hr;
	}

	// Get cluster service entity
	Result ServiceDirectoryManager::GetNextService(ServerServiceInformation* pServiceInfo, ServerServiceInformation* &pNextServiceInfo)
	{
		Result hr = ResultCode::SUCCESS;
		ServiceCluster* pClusterServiceInfo = nullptr;
		ServiceClusterSearchKey key(pServiceInfo->GetGameID(), pServiceInfo->GetClusterID());

		pNextServiceInfo = nullptr;

		hr = m_ClusterInfoMap.find(key, pClusterServiceInfo);
		if (!hr)
			return hr;

		if (pClusterServiceInfo->m_Services.size() == 0)
			return ResultCode::FAIL;

		pNextServiceInfo = pServiceInfo;
		auto itService = pClusterServiceInfo->m_Services[pServiceInfo->GetNodeNameCrc()];
		if (!itService.IsValid())
		{
			return ResultCode::UNEXPECTED;
		}

		auto selectedIndex = (itService.GetIndex() + 1 % pClusterServiceInfo->m_Services.size());

		pNextServiceInfo = pClusterServiceInfo->m_Services.GetValueAt(selectedIndex);

		return hr;
	}

	Result ServiceDirectoryManager::RegisterLocalService(GameID gameID, ClusterID clusterID, EntityUID entityUID, const EndpointAddress& endpoint, const VariableTable& customAttributes)
	{
		Result hr;

		auto zkSession = Service::ZKSession->GetZookeeperSession();
		svrCheckPtr(zkSession);

		String clusterPath = ServiceCluster::GetClusterPath(gameID, clusterID);

		if (zkSession == nullptr || !zkSession->IsConnected())
		{
			svrTrace(Error, "RegisterLocalService: Zookeeper session server hasn't ready!");
			return ResultCode::UNEXPECTED;
		}

		String outPath;
		if (!zkSession->Exists(clusterPath))
		{
			svrCheck(zkSession->Create(clusterPath, Json::Value(Json::objectValue), nullptr, 0, outPath));
		}



		Json::Value attributes(Json::objectValue);
		svrCheck(ToJsonMessageEndpoint(attributes, "Endpoint", endpoint));
		attributes["EntityUID"] = entityUID.UID;
		svrCheck(ToJson(attributes, "Custom", customAttributes));

		LocalServiceInformation* plocalInfo = new LocalServiceInformation;
		plocalInfo->GameId = gameID;
		plocalInfo->ClusterId = clusterID;
		plocalInfo->EntityUid = entityUID;
		plocalInfo->Endpoint = endpoint;
		plocalInfo->JsonAttributes = attributes;
		m_LocalServices.push_back(plocalInfo);

		plocalInfo->NodePath.Format("{0}/{1:X}", clusterPath, entityUID);

		hr = zkSession->Create(plocalInfo->NodePath, plocalInfo->JsonAttributes, nullptr, Zookeeper::NODE_FLAG_EPHEMERAL, outPath);
		if (!hr)
			return hr;

		return hr;
	}

	Result ServiceDirectoryManager::RegisterLocalService(ServiceEntity* pServiceEntity)
	{
		if (pServiceEntity == nullptr)
			return ResultCode::INVALID_POINTER;

		return RegisterLocalService(
			pServiceEntity->GetGameID(), 
			pServiceEntity->GetClusterID(), 
			pServiceEntity->GetEntityUID(), 
			pServiceEntity->GetMessageEndpointConfig(),
			pServiceEntity->CustomAttributes());
	}

	Result ServiceDirectoryManager::RegisterLocalServices()
	{
		Result hr;
		auto zkSession = Service::ZKSession->GetZookeeperSession();

		String outPath;
		for (auto itLocalService : m_LocalServices)
		{
			if (!zkSession->Exists(itLocalService->NodePath))
			{
				hr = zkSession->Create(itLocalService->NodePath, itLocalService->JsonAttributes, nullptr, Zookeeper::NODE_FLAG_EPHEMERAL, outPath);
				if (!hr)
				{
					svrTrace(Error, "RegisterLocalServices: create failed:{0}", hr);
				}
			}
		}

		return hr;
	}

	Result ServiceDirectoryManager::ToJsonMessageEndpoint(Json::Value& jsonObject, const char* keyName, const EndpointAddress& messageEndpoint)
	{
		String messageEndpointrString;
		messageEndpointrString.Format("{0}/{1}", messageEndpoint.MessageServer, messageEndpoint.Channel);

		jsonObject[keyName] = messageEndpointrString.data();

		return ResultCode::SUCCESS;
	}

	Result ServiceDirectoryManager::ToJson(Json::Value& jsonObject, const char* keyName, const VariableTable& customAttributes)
	{
		if (customAttributes.size() == 0)
			return ResultCode::SUCCESS;

		Json::Value jsonObjectValue(Json::objectValue);

		for (auto& itVariable : customAttributes)
		{
			switch (itVariable.GetValue()->GetTypeName())
			{
			case VariableBool::TYPE_NAME:
				jsonObjectValue[itVariable.GetKey().ToString()] = Json::Value(itVariable.GetValue()->GetValueBool());
				break;
			case VariableResult::TYPE_NAME:
			case VariableInt::TYPE_NAME:
			case VariableInt64::TYPE_NAME:
				jsonObjectValue[itVariable.GetKey().ToString()] = Json::Value(itVariable.GetValue()->GetValueInt32());
				break;
			case VariableVoidP::TYPE_NAME:
			case VariableUInt64::TYPE_NAME:
			case VariableUInt::TYPE_NAME:
				jsonObjectValue[itVariable.GetKey().ToString()] = Json::Value(itVariable.GetValue()->GetValueUInt32());
				break;
			case VariableFloat::TYPE_NAME:
			case VariableDouble::TYPE_NAME:
				jsonObjectValue[itVariable.GetKey().ToString()] = Json::Value(itVariable.GetValue()->GetValueDouble());
				break;
			default:
				jsonObjectValue[itVariable.GetKey().ToString()] = Json::Value(itVariable.GetValue()->GetValueString());
				break;
			}
		}

		jsonObject[keyName] = jsonObjectValue;

		return ResultCode::SUCCESS;
	}

} // namespace SF 

