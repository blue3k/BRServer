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
#include "Util/SFStrUtil.h"
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
#include "Util/SFStringFormat.h"
#include "Variable/SFVariableTable.h"

#include <bson/bson.h>


namespace SF {


	static constexpr DurationMS ServiceDirectory_ObjectPingInterval(10000);
	static constexpr DurationMS ServiceDirectory_ObjectRetryDelay(2000);
	static constexpr DurationMSDouble ServiceDirectory_ObjectTimeout(30000);

	//////////////////////////////////////////////////////////////////////////
	//
	//	 informations
	//

	ServiceCluster::ServiceCluster(IHeap& heap, GameID gameID, ClusterID clusterID)
		: m_Heap(heap)
		, m_ClusterKey(gameID, clusterID)
	{
	}

	ServiceCluster::~ServiceCluster()
	{
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


	//////////////////////////////////////////////////////////////////////////
	//
	//	 informations
	//

	String ServiceClusterZookeeper::GetClusterPath(GameID gameID, ClusterID clusterID)
	{
		String clusterPath;
		if (gameID != nullptr)
		{
			clusterPath.Format("{0}/{1}/{2}", Service::ServerConfig->DataCenter.Path, gameID, ToString(clusterID));
		}
		else
		{
			clusterPath.Format("{0}/{1}", Service::ServerConfig->DataCenter.Path, ToString(clusterID));
		}

		return Forward<String>(clusterPath);
	}

	ServiceClusterZookeeper::ServiceClusterZookeeper(IHeap& heap, GameID gameID, ClusterID clusterID)
		: ServiceCluster(heap, gameID, clusterID)
		, ZookeeperWatcher(heap)
		, m_ClusterPath(heap)
		, m_LatestSelected(0)
		, m_Services(GetHeap())
	{
		m_ClusterPath = GetClusterPath(gameID, clusterID);

		InitZK();
	}

	ServiceClusterZookeeper::~ServiceClusterZookeeper()
	{
		for (auto itService : m_Services)
		{
			itService.GetValue()->Dispose();
		}
		m_Services.Clear();
	}


	Result ServiceClusterZookeeper::AddServiceInfo(const char* nodeName, StringCrc64 nodeNameCrc, const Json::Value& nodeValue)
	{
		Result hr;
		NetAddress netAddress;
		EntityUID entityUID;
		ServerServiceInformation* pNewServiceInfo = nullptr;
		ServerEntity* pServerEntity = nullptr;


		EndpointAddress messageEndpointConfig;
		svrCheck(ParseMessageEndpoint(nodeValue, "Endpoint", messageEndpointConfig));
		entityUID.UID = nodeValue.get("EntityUID", Json::Value(0)).asUInt64();
		Json::Value customValue = nodeValue.get("Custom", Json::Value(Json::objectValue));

		MessageEndpoint* pEndpoint{};
		svrCheck(Service::MessageEndpointManager->AddOrGetRemoteEndpoint(entityUID, messageEndpointConfig, pEndpoint));

		svrCheckMem(pNewServiceInfo = new(GetHeap()) ServerServiceInformation(GetGameID(), GetClusterID(), entityUID, pEndpoint, customValue));

		svrCheck(m_Services.Insert(nodeNameCrc, pNewServiceInfo));

		svrTrace(Info, "ServiceClusterZookeeper ServiceInfo Added({0}), GameID:{1} ClusterID:{2}, entityUID:{3}", nodeName, GetGameID(), ToString(GetClusterID()), entityUID);

		return hr;
	}

	SharedPointerT<ServerServiceInformation> ServiceClusterZookeeper::GetRandomService()
	{
		if (m_Services.size() == 0)
			return nullptr;

		auto selected = m_LatestSelected.fetch_add(1, std::memory_order_relaxed);
		return m_Services.GetValueAt(selected % m_Services.size());
	}


	void ServiceClusterZookeeper::InitZK()
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

	Result ServiceClusterZookeeper::GetNodeValue(const String& nodePath, Json::Value& jsonValue)
	{
		StaticArray<uint8_t, 1024> valueBuffer(GetSystemHeap());
		auto zkSession = Service::ZKSession->GetZookeeperSession();
		if (zkSession == nullptr || !zkSession->IsConnected())
			return ResultCode::FAIL;

		Result result = zkSession->Get(nodePath, jsonValue);

		return result;
	}

	Result ServiceClusterZookeeper::SetNodeValue(const String& nodePath, const Json::Value& jsonValue)
	{
		auto zkSession = Service::ZKSession->GetZookeeperSession();
		if (zkSession == nullptr || !zkSession->IsConnected())
			return ResultCode::FAIL;

		Json::StreamWriterBuilder builder;
		builder["indentation"] = "";
		auto stringValue = std::forward<std::string>(Json::writeString(builder, jsonValue));

		return zkSession->Set(nodePath, stringValue);
	}

	void ServiceClusterZookeeper::DownloadServiceInfo()
	{
		// We download full list on reconnect, just skip it
		auto zkSession = Service::ZKSession->GetZookeeperSession();
		if (zkSession == nullptr || !zkSession->IsConnected())
			return;

		svrTrace(Debug, "ServiceClusterZookeeper requesting service list, GameID:{0} ClusterID:{1}", GetGameID(), ToString(GetClusterID()));

		zkSession->AGetChildren(m_ClusterPath, this);
	}

	Result ServiceClusterZookeeper::FindObjects(const VariableTable& searchAttributes, Array<SharedPointerT<EntityInformation>>& foundObjects)
	{
		// TODO: check search attributes
		svrTrace(Warning, "ServiceClusterZookeeper:FindObjects not fully implemented, GameID:{0} ClusterID:{1}", GetGameID(), ToString(GetClusterID()));

		for (auto itService : m_Services)
		{
			foundObjects.push_back(itService.GetValue().get());
		}

		return ResultCode::SUCCESS;
	}



	//void ServiceClusterZookeeper::NodeUpdated(const String& nodePath)
	//{
	//	// We download full list on reconnect, just skip it
	//	auto zkSession = Service::ZKSession->GetZookeeperSession();
	//	if (zkSession == nullptr || !zkSession->IsConnected())
	//		return;

	//	if (zkSession->Exists(nodePath))
	//	{
	//		svrTrace(Debug, "ServiceClusterZookeeper updating service node state:{0}, removed", GetClusterKey().Components.GameClusterID, nodePath);
	//		ServerServiceInformation* pServiceInfo{};
	//		m_Services.Remove(StringCrc64(nodePath), pServiceInfo);
	//		if (pServiceInfo)
	//			IHeap::Delete(pServiceInfo);
	//	}
	//	else
	//	{
	//		svrTrace(Debug, "ServiceClusterZookeeper updating service node state:{0}, added", GetClusterKey().Components.GameClusterID, nodePath);
	//		AddServiceInfo(nodePath, StringCrc64(nodePath));
	//	}
	//}


	/////////////////////////////////////////////
	//
	//	Overridable Event handling
	//

	Result ServiceClusterZookeeper::OnNewEvent(const ZKEvent& eventOut)
	{
		svrTrace(Info, "ServiceClusterZookeeper ZKEvent:{0}, GameID:{1} ClusterID:{2}, path:{3}", Zookeeper::EventToString(eventOut.Components.EventType), GetGameID(), ToString(GetClusterID()), eventOut.Components.NodePath);

		if (eventOut.Components.EventType == Zookeeper::EVENT_CHILD)
		{
			// TODO: optimize with child path
			//NodeUpdated(eventOut.Components.NodePath.ToString());
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

	void ServiceClusterZookeeper::OnComplition(ZookeeperTask& pTask)
	{
		if (!pTask.ZKResult)
		{
			assert(false);
			return;
		}
	}

	void ServiceClusterZookeeper::OnStatComplition(StatTask& pTask)
	{

	}

	void ServiceClusterZookeeper::OnDataComplition(DataTask& pTask)
	{

	}

	void ServiceClusterZookeeper::OnStringsComplition(StringsTask& pTask)
	{
		if (!pTask.ZKResult)
		{
			svrTrace(Info, "ServiceClusterZookeeper requesting service list failed gameid:{0}, clusterid:{1}, hr:{2}", GetGameID(), GetClusterID(), pTask.ZKResult);
			return;
		}

		SortedSet<StringCrc64> newSet(GetHeap(), static_cast<int>(pTask.ResultStrings.size()));
		SortedSet<StringCrc64> removedSet(GetHeap(), static_cast<int>(m_Services.size()));

		// This should be getchildren list because that's the only thing have string list result
		for (auto& nodeName : pTask.ResultStrings)
		{
			StringCrc64 nodeNameCrc = (const char*)nodeName;
			newSet.Insert(nodeNameCrc);
		}

		for (auto itService : m_Services)
		{
			int64_t order{};
			if (!newSet.Find(itService.GetKey()))
			{
				removedSet.Insert(itService.GetKey());
			}
		}

		for (auto itRemoved : removedSet)
		{
			SharedPointerT<ServerServiceInformation> pRemove;
			if (m_Services.Remove(itRemoved, pRemove))
			{
				svrTrace(Info, "ServiceClusterZookeeper service removed gameid:{0}, clusterid:{1}, entityUID:{2}", pRemove->GetGameID(), pRemove->GetClusterID(), pRemove->GetEntityUID());
			}
			else
			{
				// the value should be exist
				assert(false);
			}
		}

		for (auto& nodeName : pTask.ResultStrings)
		{
			SharedPointerT<ServerServiceInformation> pFound;
			StringCrc64 nodeNameCrc = (const char*)nodeName;
			m_Services.Find(nodeNameCrc, pFound);
			if (pFound != nullptr)
			{
				continue;
			}
			else
			{
				svrTrace(Info, "ServiceClusterZookeeper service added ({0})", nodeName);

				Json::Value jsonValue;
				String nodePath(GetHeap());
				nodePath.Format("{0}/{1}", m_ClusterPath, nodeName);

				GetNodeValue(nodePath, jsonValue);

				// New Service I need to add this to the list
				AddServiceInfo(nodeName, nodeNameCrc, jsonValue);
			}
		}

		m_GetChildrenTask = nullptr;
	}

	void ServiceClusterZookeeper::OnStringsStatComplition(StringsStatTask& pTask)
	{
		if (!pTask.ZKResult)
		{
			assert(false);
			return;
		}


	}

	void ServiceClusterZookeeper::OnStringComplition(StringTask& pTask)
	{
		if (!pTask.ZKResult)
		{
			assert(false);
			return;
		}

		// This should be create for new path

	}

	//void ServiceClusterZookeeper::OnACLComplition(ACLTask& pTask)
	//{
	//}




	ServiceClusterMongo::ServiceClusterMongo(ServiceDirectoryManager& owner, GameID gameID, ClusterID clusterID)
		: ServiceCluster(owner.GetHeap(), gameID, clusterID)
		, m_Owner(owner)
	{
		m_DBCollection = m_Owner.GetObjectCollection(gameID, clusterID);
	}

	ServiceClusterMongo::~ServiceClusterMongo()
	{

	}

	Result ServiceClusterMongo::ParseObject(const bson_t* value, SharedPointerT<ServerServiceInformation>& outParsed)
	{
		Result hr;

		if (value == nullptr)
			return ResultCode::INVALID_ARG;

		EntityUID entityUID;
		EndpointAddress endpointAddress;
		UTCTimeStampMS updatedTimeStamp{};
		VariableTable attributes;
		//bson_append_date_time()

		bson_iter_t iter;
		if (!bson_iter_init(&iter, value))
		{
			svrTrace(Error, "ServiceClusterMongo::ParseObject, failed bson parsing error");
			return ResultCode::INVALID_FORMAT;
		}

		while (bson_iter_next(&iter))
		{
			bson_iter_t* curIter = &iter;
			StringCrc32 key = bson_iter_key(curIter);
			if (key == "_id"_crc)
			{
				entityUID = bson_iter_as_int64(curIter);
			}
			else if (key == "Updated"_crc)
			{
				updatedTimeStamp = UTCTimeStampMS(DurationMSDouble(bson_iter_date_time(curIter)));
			}
			else if (key == "Endpoint"_crc)
			{
				uint32_t strLen{};
				endpointAddress = bson_iter_utf8(curIter, &strLen);
			}
			else if (key == "Custom"_crc)
			{
				bson_iter_t child;
				if (BSON_ITER_HOLDS_DOCUMENT(curIter) && bson_iter_recurse(curIter, &child))
				{
					curIter = &child;
					uint32_t strLen{};
					while (bson_iter_next(curIter))
					{
						auto keyString = bson_iter_key(curIter);
						switch (bson_iter_type(curIter))
						{
						case BSON_TYPE_BOOL:
							attributes.SetValue(keyString, bson_iter_bool(curIter));
							break;
						case BSON_TYPE_INT32:
							attributes.SetValue(keyString, bson_iter_int32(curIter));
							break;
						case BSON_TYPE_INT64:
							attributes.SetValue(keyString, bson_iter_int64(curIter));
							break;
						case BSON_TYPE_DOUBLE:
							attributes.SetValue(keyString, bson_iter_double(curIter));
							break;
						case BSON_TYPE_UTF8:
							attributes.SetValue(keyString, bson_iter_utf8(curIter, &strLen));
							break;
						default:
							svrTrace(Error, "Not handled bson value type: key:{0}, type:{1}", keyString, (uint32_t)bson_iter_type(curIter));
							break;
						}
					}
					curIter = &iter;
				}
			}
		}

		if (entityUID.UID == 0)
		{
			svrTrace(Error, "ServiceClusterMongo::ParseObject, failed bson parsing error, invalid entityUID");
			return ResultCode::INVALID_FORMAT;
		}

		MessageEndpoint* endpoint{};
		if (!endpointAddress.MessageServer.IsNullOrEmpty())
		{
			Service::MessageEndpointManager->AddOrGetRemoteEndpoint(entityUID, endpointAddress, endpoint);
		}

		outParsed = new(GetHeap()) ServerServiceInformation(GetGameID(), GetClusterID(), entityUID, endpoint, attributes);

		return hr;
	}

	SharedPointerT<ServerServiceInformation> ServiceClusterMongo::GetRandomService()
	{
		if (m_DBCollection == nullptr)
			return nullptr;

		bson_t request;
		bson_t requestmatch;
		bson_t requestmatchRule;
		bson_t requestSize;
		bson_init(&request);
		BsonUniquePtr requestPtr(&request);


		auto UTCTimeout = Util::Time.GetRawUTCMs().time_since_epoch().count() - DurationMSDouble(15 * 1000).count(); // 15sec timeout

		bson_append_document_begin(&request, "match", -1, &requestmatch);
			bson_append_document_begin(&requestmatch, "Updated", -1, &requestmatchRule);
				bson_append_int32(&requestmatchRule, "$gte", -1, UTCTimeout);
			bson_append_document_end(&requestmatch, &requestmatchRule);
		bson_append_document_end(&request, &requestmatch);

		bson_append_document_begin(&request, "$sample", -1, &requestSize);
			bson_append_int32(&requestSize, "size", -1, 1);
		bson_append_document_end(&request, &requestSize);

		MongoDBCursor cursor;
		if (!m_DBCollection->Aggregate(&request, cursor))
		{
			svrTrace(Error, "ServiceClusterMongo::GetRandomService, failed get data from object directory");
			return nullptr;
		}

		auto* value = cursor.Next();
		SharedPointerT<ServerServiceInformation> service;
		if (!ParseObject(value, service))
			return nullptr;

		return Forward<SharedPointerT<ServerServiceInformation>>(service);
	}

	Result ServiceClusterMongo::FindObjects(const VariableTable& searchAttributes, Array<SharedPointerT<EntityInformation>>& foundObjects)
	{
		Result hr;

		bson_t request;
		bson_t requestmatch;
		bson_t requestmatchRule;
		bson_t requestSize;
		bson_init(&request);
		BsonUniquePtr requestPtr(&request);

		auto UTCTimeout = Util::Time.GetRawUTCMs().time_since_epoch().count() - ServiceDirectory_ObjectTimeout.count();

		bson_t* curBase = &request;
		//bson_append_document_begin(&request, "match", -1, &requestmatch);
		{
			bson_append_document_begin(curBase, "Updated", -1, &requestmatch);
			{
				curBase = &requestmatch;
				bson_append_date_time(curBase, "$gte", -1, UTCTimeout);
				curBase = &request;
			}
			bson_append_document_end(curBase, &requestmatch);

			for (auto itArrtibute : searchAttributes)
			{
				auto keyString = itArrtibute.GetKey().ToString();
				auto pVariable = itArrtibute.GetValue();
				switch (pVariable->GetTypeName())
				{
					case VariableBool::TYPE_NAME:
						bson_append_bool(curBase, keyString, -1, pVariable->GetValueBool());
						break;
					case VariableResult::TYPE_NAME:
					case VariableInt::TYPE_NAME:
					case VariableUInt::TYPE_NAME:
						bson_append_int32(curBase, keyString, -1, pVariable->GetValueInt32());
						break;
					case VariableInt64::TYPE_NAME:
					case VariableUInt64::TYPE_NAME:
						bson_append_int64(curBase, keyString, -1, pVariable->GetValueInt64());
						break;
					case VariableVoidP::TYPE_NAME:
						// not supported value type
						return ResultCode::NOT_SUPPORTED;
						break;
					case VariableFloat::TYPE_NAME:
					case VariableDouble::TYPE_NAME:
						bson_append_double(curBase, keyString, -1, pVariable->GetValueDouble());
						break;
					default:
						bson_append_utf8(curBase, keyString, -1, pVariable->GetValueCharString(), -1);
						break;
				}
			}
		}
		//bson_append_document_end(&request, &requestmatch);
		//curBase = &request;


		MongoDBCursor cursor;
		if (!m_DBCollection->Find(&request, cursor))
		{
			svrTrace(Error, "ServiceClusterMongo::GetRandomService, failed get data from object directory");
			return ResultCode::FAIL;
		}

		auto* value = cursor.Next();
		while (value)
		{
			SharedPointerT<ServerServiceInformation> service;
			if (ParseObject(value, service))
			{
				foundObjects.push_back(service.get());
			}
			value = cursor.Next();
		}

		return hr;
	}

	//////////////////////////////////////////////////////////////////////////
	//
	//	Local Service
	//

	ServiceDirectoryManager::LocalService::LocalService(ServiceDirectoryManager& owner, GameID gameID, ClusterID clusterID, EntityUID entityUID, const EndpointAddress& endpoint, const VariableTable& customAttributes)
		: Owner(owner)
		, GameId(gameID)
		, ClusterId(clusterID)
		, EntityUid(entityUID)
		, Endpoint(endpoint)
		, Attributes(owner.GetHeap())
	{
		Attributes = customAttributes;
	}

	Result ServiceDirectoryManager::LocalService::ScheduleTickUpdate(DurationMS delay)
	{
		m_ExpectedTickTime = Util::Time.GetRawTimeMs() + delay;

		Owner.ScheduleTickUpdate(this);
		return ResultCode::SUCCESS;
	}

	bool ServiceDirectoryManager::LocalService::OnTimerTick()
	{
		Register();
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	//
	//	Local Service Zookeeper
	//

	ServiceDirectoryManager::LocalServiceZookeeper::LocalServiceZookeeper(ServiceDirectoryManager& owner, GameID gameID, ClusterID clusterID, EntityUID entityUID, const EndpointAddress& endpoint, const VariableTable& customAttributes)
		: LocalService(owner, gameID, clusterID, entityUID, endpoint, customAttributes)
	{
		String clusterPath = ServiceClusterZookeeper::GetClusterPath(gameID, clusterID);

		Result hr;
		Json::Value attributes(Json::objectValue);
		hr = ToJsonMessageEndpoint(attributes, "Endpoint", endpoint);
		if (!hr)
		{
			svrTrace(Error, "ToJsonMessageEndpoint has failed {0}:{1}", endpoint.MessageServer, endpoint.Channel);
		}

		attributes["EntityUID"] = entityUID.UID;
		hr = ToJson(attributes, "Custom", customAttributes);
		if (!hr)
		{
			svrTrace(Error, "ToJsonMessageEndpoint has failed {0}:{1}", endpoint.MessageServer, endpoint.Channel);
		}

		JsonAttributes = attributes;

		auto zkSession = Service::ZKSession->GetZookeeperSession();
		if (zkSession != nullptr && !zkSession->Exists(clusterPath))
		{
			zkSession->ACreate(clusterPath, Json::Value(Json::objectValue), nullptr, 0);
		}

		NodePath.Format("{0}/{1:X}", clusterPath, entityUID);
	}

	//LocalServiceZookeeper::~LocalServiceZookeeper() = default;

	Result ServiceDirectoryManager::LocalServiceZookeeper::Register()
	{
		Result hr;
		auto zkSession = Service::ZKSession->GetZookeeperSession();
		svrCheckPtr(zkSession);

		if (zkSession == nullptr || !zkSession->IsConnected())
		{
			svrTrace(Error, "RegisterLocalService: Zookeeper session server hasn't ready!");
			return ResultCode::UNEXPECTED;
		}

		String outPath;
		hr = zkSession->Create(NodePath, JsonAttributes, nullptr, Zookeeper::NODE_FLAG_EPHEMERAL, outPath);
		if (hr == ResultCode::ZK_ZNODEEXISTS)
		{
			svrTrace(Error, "RegisterLocalService: duplicated local service gameId:{0}, clusterId:{1}, entityUID:{2}", GameId, ClusterId, EntityUid);
			// Probably from previous execution
			zkSession->ADelete(NodePath);
			ScheduleTickUpdate(ServiceDirectory_ObjectRetryDelay);
		}

		return hr;
	}

	Result ServiceDirectoryManager::LocalServiceZookeeper::Deregister()
	{
		Result hr;
		auto zkSession = Service::ZKSession->GetZookeeperSession();
		svrCheckPtr(zkSession);

		if (zkSession == nullptr || !zkSession->IsConnected())
		{
			svrTrace(Error, "RemoveLocalService: Zookeeper session is not ready! {0}", NodePath);
			return ResultCode::UNEXPECTED;
		}

		zkSession->ADelete(NodePath);

		return hr;
	}

	//////////////////////////////////////////////////////////////////////////
	//
	//	MongoDB local service
	//

	ServiceDirectoryManager::LocalServiceMongo::LocalServiceMongo(ServiceDirectoryManager& owner, GameID gameID, ClusterID clusterID, EntityUID entityUID, const EndpointAddress& endpoint, const VariableTable& customAttributes)
		: LocalService(owner, gameID, clusterID, entityUID, endpoint, customAttributes)
	{
	}

	Result ServiceDirectoryManager::LocalServiceMongo::CreateBsonAttribute()
	{
		return Owner.CreateBsonAttribute(GetGameID(), GetClusterID(), GetEntityUID(), GetAttributes(), BsonAttributes);
	}

	Result ServiceDirectoryManager::LocalServiceMongo::Register()
	{
		CreateBsonAttribute();

		SharedPointerT<MongoDBCollection> pCollection = Owner.GetObjectCollection(GetGameID(), GetClusterID());
		if (pCollection == nullptr)
		{
			svrTrace(Error, "LocalServiceMongo::Register server hasn't ready!, GameId:{0}, clusterId:{1}", GetGameID(), GetClusterID());
			ScheduleTickUpdate(ServiceDirectory_ObjectRetryDelay);
			return ResultCode::FAIL;
		}

		Result hr = pCollection->AddOrUpdate(GetEntityUID(), BsonAttributes.get());
		if (!hr)
		{
			svrTrace(Error, "LocalServiceMongo::Register failed to update record, GameId:{0}, clusterId:{1}", GetGameID(), GetClusterID());
			ScheduleTickUpdate(ServiceDirectory_ObjectRetryDelay);
			return hr;
		}

		svrTrace(Debug3, "LocalServiceMongo::Register GameId:{0}, clusterId:{1}, entity{2}", GetGameID(), GetClusterID(), GetEntityUID());

		// schedule next update 
		ScheduleTickUpdate(ServiceDirectory_ObjectPingInterval);

		return hr;
	}

	Result ServiceDirectoryManager::LocalServiceMongo::Deregister()
	{
		auto* pCollection = Owner.GetObjectCollection(GetGameID(), GetClusterID());
		if (pCollection == nullptr)
		{
			// Timeout will help you
			//RetryDeregister(DurationMS(2000));
			return ResultCode::FAIL;
		}

		return pCollection->Remove(GetEntityUID());
	}


	//////////////////////////////////////////////////////////////////////////
	//
	//	Service directory manager
	//

	constexpr StringCrc64 ServiceDirectoryManager::TypeName;


	ServiceDirectoryManager::ServiceDirectoryManager()
		: m_ClusterInfoMap(GetSystemHeap())
		, m_TickQueue(GetSystemHeap(), 2048, 1024)
	{
		Service::ServiceDirectory = this;
	}


	ServiceDirectoryManager::~ServiceDirectoryManager()
	{
		Service::ServiceDirectory = nullptr;
	}

	Result ServiceDirectoryManager::InitializeComponent()
	{
		Result hr;
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

		// Initialize mongo db
		if (Service::ServerConfig->ObjectDirectory.IsNullOrEmpty())
		{
			svrTrace(Error, "Failed to initialize ObjectDirectory, no config found hr:{0}", hr);
		}
		else
		{
			m_MongoDB = new(GetHeap()) MongoDB(GetHeap());
			hr = m_MongoDB->Initialize(Service::ServerConfig->ObjectDirectory);
			if (!hr)
			{
				svrTrace(Error, "Failed to connect ObjectDirectory:{0}, hr:{1}", Service::ServerConfig->ObjectDirectory, hr);
			}
		}
		
		// Always watch for server endpoints
		WatchForService(Service::ServerConfig->GameClusterID, ClusterID::None);

		// Initializing thread
		m_TickThread.reset(new(GetHeap()) FunctorTickThread([this](Thread* pThread) 
			{
				TickUpdate();
				return true;
			}));

		m_TickThread->Start();

		return ResultCode::SUCCESS;
	}

	// Terminate component
	void ServiceDirectoryManager::DeinitializeComponent()
	{
		if (m_TickThread != nullptr)
		{
			m_TickThread->Stop(true);
			m_TickThread.reset();
		}
	}

	void ServiceDirectoryManager::TickUpdate()
	{
		if (m_LastTickItem == nullptr)
			m_TickQueue.Dequeue(m_LastTickItem);


		while (m_LastTickItem != nullptr)
		{
			auto localService = m_LastTickItem.AsSharedPtr<LocalService>();
			if (localService == nullptr)
			{
				m_TickQueue.Dequeue(m_LastTickItem);
				continue;
			}

			if (localService->GetExpectedTickTime() >= Util::Time.GetRawTimeMs())
				break; // this isn't ideal, but will give reasonable update rate

			localService->OnTimerTick();
			m_LastTickItem.reset();
			m_TickQueue.Dequeue(m_LastTickItem);
		}

		
	}

	MongoDBCollection* ServiceDirectoryManager::GetObjectCollection(GameID gameID, ClusterID clusterID)
	{
		String channelCollection;
		channelCollection.Format("{0}_{1}", gameID.ToString(), ToString(clusterID));

		// We need to create new one for every request because MongoDB client isn't thread safe
		auto pCollection = new(GetHeap()) MongoDBCollection(GetHeap());
		pCollection->Initialize(m_MongoDB->GetClientFromPool(), "ObjectDirectory", channelCollection);

		return pCollection;
	}

	bool ServiceDirectoryManager::UseObjectCluster(ClusterID clusterId) const
	{
		switch (clusterId)
		{
		case ClusterID::GamePlayer:
		case ClusterID::GameInstance:
			return true;
		default:
			break;
		}

		return false;
	}

	// Set watch state for cluster
	Result ServiceDirectoryManager::WatchForService(GameID gameID, ClusterID clusterID)
	{
		ServiceCluster* pServiceInfo = nullptr;
		ServiceClusterSearchKey key(gameID, clusterID);

		if (m_ClusterInfoMap.find(key, pServiceInfo))
			return ResultCode::SUCCESS;

		svrTrace(Info, "Adding service watcher for cluster, GameID:{0} ClusterID:{1}, {2}", gameID, ToString(clusterID), clusterID);

		if (UseObjectCluster(clusterID))
		{
			pServiceInfo = new(GetSystemHeap()) ServiceClusterMongo(*this, gameID, clusterID);
		}
		else
		{
			pServiceInfo = new(GetSystemHeap()) ServiceClusterZookeeper(GetSystemHeap(), gameID, clusterID);
		}
		if (pServiceInfo == nullptr || !m_ClusterInfoMap.insert(key, pServiceInfo))
		{
			svrTrace(Error, "Failed to Add service watcher for cluster, GameID:{0} ClusterID:{1}, {2}", gameID, ToString(clusterID), clusterID);

			IHeap::Delete(pServiceInfo);
			return ResultCode::OUT_OF_MEMORY;
		}

		return pServiceInfo != nullptr ? ResultCode::SUCCESS : ResultCode::UNEXPECTED;
	}

	Result ServiceDirectoryManager::FindObjects(GameID gameID, ClusterID clusterID, const VariableTable& searchAttributes, Array<SharedPointerT<EntityInformation>>& foundObjects)
	{
		Result hr;
		ServiceCluster* pServiceInfo = nullptr;
		ServiceClusterSearchKey key(gameID, clusterID);

		if (!m_ClusterInfoMap.find(key, pServiceInfo))
			return hr;

		pServiceInfo->FindObjects(searchAttributes, foundObjects);

		return hr;
	}


	// Get cluster service entity
	Result ServiceDirectoryManager::GetRandomService(GameID gameID, ClusterID clusterID, SharedPointerT<ServerServiceInformation> &pServiceInfo )
	{
		Result hr = ResultCode::SUCCESS;
		ServiceCluster* pClusterServiceInfo = nullptr;
		ServiceClusterSearchKey key(gameID, clusterID);

		hr = m_ClusterInfoMap.find(key, pClusterServiceInfo);
		if (!hr)
			return hr;

		pServiceInfo = Forward<SharedPointerT<ServerServiceInformation>>(pClusterServiceInfo->GetRandomService());

		return pServiceInfo != nullptr ? ResultCode::SUCCESS : ResultCode::SERVICE_NOT_AVAILABLE;
	}


	Result ServiceDirectoryManager::CreateBsonAttribute(GameID gameID, ClusterID clusterID, EntityUID entityUID, const VariableTable& customAttributes, BsonUniquePtr& bsonAttributes)
	{
		// TODO: I just need to update time stamp
		//if (bsonAttributes != nullptr)
		//	return ResultCode::SUCCESS;

		bson_t objectValue;
		bson_t customValue;
		auto UTCNow = Util::Time.GetRawUTCMs().time_since_epoch().count();

		bsonAttributes.reset(bson_new());

		//bson_append_document_begin(bsonAttributes.get(), "$set", -1, &objectValue);
		bson_append_int64(bsonAttributes.get(), "_id", 3, (uint64_t)entityUID);
		bson_append_date_time(bsonAttributes.get(), "Updated", -1, UTCNow);
		if (customAttributes.size() > 0)
		{
			bson_append_document_begin(bsonAttributes.get(), "Custom", 6, &customValue);
			for (auto& itVariable : customAttributes)
			{
				auto keyString = itVariable.GetKey().ToString();
				auto pVariable = itVariable.GetValue();
				switch (itVariable.GetValue()->GetTypeName())
				{
				case VariableBool::TYPE_NAME:
					bson_append_bool(&customValue, keyString, -1, pVariable->GetValueBool());
					break;
				case VariableResult::TYPE_NAME:
				case VariableInt::TYPE_NAME:
				case VariableUInt::TYPE_NAME:
					bson_append_int32(&customValue, keyString, -1, pVariable->GetValueInt32());
					break;
				case VariableInt64::TYPE_NAME:
				case VariableUInt64::TYPE_NAME:
					bson_append_int64(&customValue, keyString, -1, pVariable->GetValueInt64());
					break;
				case VariableVoidP::TYPE_NAME:
					// not supported value type
					return ResultCode::NOT_SUPPORTED;
					break;
				case VariableFloat::TYPE_NAME:
				case VariableDouble::TYPE_NAME:
					bson_append_double(&customValue, keyString, -1, pVariable->GetValueDouble());
					break;
				default:
					bson_append_utf8(&customValue, keyString, -1, pVariable->GetValueCharString(), -1);
					break;
				}
			}
			bson_append_document_end(bsonAttributes.get(), &customValue);
		}
		//bson_append_document_end(bsonAttributes.get(), &objectValue);

		return ResultCode::SUCCESS;
	}

	Result ServiceDirectoryManager::PingObjectDirectory(GameID gameID, ClusterID clusterID, EntityUID entityUID, const VariableTable& customAttributes)
	{
		Result hr;

		BsonUniquePtr bsonAttribute;
		svrCheck(CreateBsonAttribute(gameID, clusterID, entityUID, customAttributes, bsonAttribute));

		SharedPointerT<MongoDBCollection> pCollection = GetObjectCollection(gameID, clusterID);
		if (pCollection == nullptr)
		{
			svrTrace(Error, "LocalServiceMongo::Register server hasn't ready!, GameId:{0}, clusterId:{1}", gameID, clusterID);
			return ResultCode::FAIL;
		}

		hr = pCollection->AddOrUpdate(entityUID, bsonAttribute.get());
		if (!hr)
		{
			svrTrace(Error, "LocalServiceMongo::Register failed to update record, GameId:{0}, clusterId:{1}", gameID, clusterID);
			return hr;
		}

		svrTrace(Debug4, "LocalServiceMongo::Register GameId:{0}, clusterId:{1}, entity{2}", gameID, clusterID, entityUID);

		return hr;
	}

	Result ServiceDirectoryManager::RegisterLocalService(GameID gameID, ClusterID clusterID, EntityUID entityUID, const EndpointAddress& endpoint, const VariableTable& customAttributes)
	{
		Result hr;

		LocalService* pLocalInfo{};
		if (UseObjectCluster(clusterID))
		{
			pLocalInfo = new(GetHeap()) LocalServiceMongo(*this, gameID, clusterID, entityUID, endpoint, customAttributes);
		}
		else
		{
			pLocalInfo = new(GetHeap()) LocalServiceZookeeper(*this, gameID, clusterID, entityUID, endpoint, customAttributes);
		}

		if (pLocalInfo == nullptr)
			return ResultCode::OUT_OF_MEMORY;

		{
			MutexScopeLock lock(m_ServiceLock);
			m_LocalServices.push_back(pLocalInfo);
		}

		pLocalInfo->Register();

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
			pServiceEntity->GetCustomAttributes());
	}

	Result ServiceDirectoryManager::RemoveLocalService(GameID gameID, ClusterID clusterID, EntityUID entityUID)
	{
		Result hr;
		MutexScopeLock lock(m_ServiceLock);
		int iService = 0;
		for (int iService = 0; iService < m_LocalServices.size(); iService++)
		{
			auto itLocalService = m_LocalServices[iService];
			if (itLocalService->ClusterId != clusterID
				|| itLocalService->GameId != gameID
				|| itLocalService->EntityUid != entityUID)
				continue;

			itLocalService->Deregister();

			m_LocalServices.RemoveAt(iService);
			break;
		}

		if (iService >= m_LocalServices.size())
			return ResultCode::SUCCESS_FALSE;

		return hr;
	}

	Result ServiceDirectoryManager::RemoveLocalService(ServiceEntity* pServiceEntity)
	{
		if (pServiceEntity == nullptr)
			return ResultCode::INVALID_POINTER;

		return RemoveLocalService(
			pServiceEntity->GetGameID(),
			pServiceEntity->GetClusterID(),
			pServiceEntity->GetEntityUID());
	}

	Result ServiceDirectoryManager::RegisterLocalServices()
	{
		Result hr;
		auto zkSession = Service::ZKSession->GetZookeeperSession();

		String outPath;
		for (auto itLocalService : m_LocalServices)
		{
			hr = itLocalService->Register();
			if (!hr)
			{
				svrTrace(Error, "RegisterLocalServices: create failed:{0}, clusterId:{1}", hr, itLocalService->GetClusterID());
			}
		}

		return hr;
	}

	void ServiceDirectoryManager::ScheduleTickUpdate(LocalService* localService)
	{
		m_TickQueue.Enqueue(localService);
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

