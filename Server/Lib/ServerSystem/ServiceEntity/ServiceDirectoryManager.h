////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) The Braves
// 
// Author : KyungKun Ko
//
// Description : Service directory
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once


#include "SFTypedefs.h"

#include "MemoryManager/SFMemory.h"
#include "Types/SFEngineTypedefs.h"
#include "Types/BrSvrTypes.h"
#include "Entity/Entity.h"
#include "Component/ServerComponent.h"
#include "ServerService/ServerServiceBase.h"
#include "Container/SFHashTable.h"
#include "Container/SFIndexing.h"
#include "Object/SFSharedObject.h"
#include "Object/SFSharedPointer.h"
#include "Entity/EntityInformation.h"
#include "Service/ServiceDirectoryService.h"
#include "Zookeeper/SFZookeeper.h"
#include "Mongo/SFMongoDB.h"
#include "json/json.h"
#include "Task/SFTimerScheduler.h"
#include "Task/SFTimerSchedulerAction.h"


namespace SF {

	namespace Svr {
		class Entity;
	}

	class ServiceDirectoryManager;

#pragma pack(push,4)
	union ServiceClusterSearchKey
	{
		struct {
			ClusterID ServiceClusterID;
			GameID GameClusterID;
		} Components;
		uint64_t Composited;

		ServiceClusterSearchKey() : Composited(0) {}
		ServiceClusterSearchKey(GameID gameID, ClusterID clusterID)
		{
			Components.GameClusterID = (gameID);
			Components.ServiceClusterID = (clusterID);
		}

		operator uint64_t() { return Composited; }
	};
#pragma pack(pop)


	class ServiceCluster
	{
	private:
		IHeap& m_Heap;
		ServiceClusterSearchKey m_ClusterKey;

		friend class ServiceDirectoryManager;

	protected:

		static Result ParseMessageEndpoint(const Json::Value& jsonObject, const char* keyName, EndpointAddress& outMessageEndpoint);


	public:

		ServiceCluster(IHeap& heap, GameID gameID, ClusterID clusterID);
		virtual ~ServiceCluster();

		IHeap& GetHeap() { return m_Heap; }

		const ServiceClusterSearchKey& GetClusterKey() const { return m_ClusterKey; }
		GameID GetGameID() const { return m_ClusterKey.Components.GameClusterID; }
		ClusterID GetClusterID() const { return m_ClusterKey.Components.ServiceClusterID; }

		virtual SharedPointerT<ServerServiceInformation> GetRandomService() = 0;
		virtual Result FindObjects(const VariableTable& searchAttributes, Array<SharedPointerT<EntityInformation>>& foundObjects) { return ResultCode::NOT_IMPLEMENTED; }
	};

	// Service cluster information
	class ServiceClusterZookeeper : public ServiceCluster, public ZookeeperWatcher
	{

	private:

		bool m_ZKInitialized = false;

		Atomic<uint> m_LatestSelected;
		String m_ClusterPath;

		SortedArray<StringCrc64, SharedPointerT<ServerServiceInformation>> m_Services;

		SharedPointerT<ZookeeperWatcher::StringsTask> m_GetChildrenTask;

	private:

		void InitZK();

		Result AddServiceInfo(const char* nodeName, StringCrc64 nodeNameCrc, const Json::Value& nodeValue);

		static Result GetNodeValue(const String& nodePath, Json::Value& jsonValue);
		static Result SetNodeValue(const String& nodePath, const Json::Value& jsonValue);


	public:
		ServiceClusterZookeeper(IHeap& heap, GameID gameID, ClusterID clusterID);
		~ServiceClusterZookeeper();

		virtual SharedPointerT<ServerServiceInformation> GetRandomService() override;

		bool IsZKInitialized() { return m_ZKInitialized; }

		void DownloadServiceInfo();
		//void NodeUpdated(const String& nodePath);

		auto begin() { return m_Services.begin(); }
		auto end() { return m_Services.end(); }

		virtual Result FindObjects(const VariableTable& searchAttributes, Array<SharedPointerT<EntityInformation>>& foundObjects) override;

		/////////////////////////////////////////////
		//
		//	Event handling
		//

		virtual Result OnNewEvent(const ZKEvent& eventOut) override;
		virtual void OnComplition(ZookeeperTask& pTask) override;
		virtual void OnStatComplition(StatTask& pTask) override;
		virtual void OnDataComplition(DataTask& pTask) override;
		virtual void OnStringsComplition(StringsTask& pTask) override;
		virtual void OnStringsStatComplition(StringsStatTask& pTask) override;
		virtual void OnStringComplition(StringTask& pTask) override;
//		virtual void OnACLComplition(ACLTask& pTask) override;

		
		// Static helper functions
		static String GetClusterPath(GameID gameID, ClusterID clusterID);

	};

	class ServiceClusterMongo : public ServiceCluster
	{
	private:

		ServiceDirectoryManager& m_Owner;
		SharedPointerT<MongoDBCollection> m_DBCollection;



	public:
		ServiceClusterMongo(ServiceDirectoryManager& owner, GameID gameID, ClusterID clusterID);
		~ServiceClusterMongo();


		Result ParseObject(const bson_t* object, SharedPointerT<ServerServiceInformation>& outParsed);

		virtual SharedPointerT<ServerServiceInformation> GetRandomService() override;

		virtual Result FindObjects(const VariableTable& searchAttributes, Array<SharedPointerT<EntityInformation>>& foundObjects) override;

	};


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	ServiceDirectoryManager class
	//

	class ServiceDirectoryManager : public ServiceDirectoryService, public SharedObject
	{
	public:

		using super = ServiceDirectoryService;

		static constexpr StringCrc64 TypeName = "ServiceDirectoryManager";

		// Service Entity table
		using ClusterInfomationMap = HashTable2<uint64_t, ServiceCluster*>;

		class LocalService : public SharedObject
		{
		public:
			ServiceDirectoryManager& Owner;
			GameID GameId;
			ClusterID ClusterId;
			EntityUID EntityUid;
			EndpointAddress Endpoint;
			VariableTable Attributes;

		private:

			TimeStampMS m_ExpectedTickTime;

		public:
			LocalService(ServiceDirectoryManager& owner, GameID gameID, ClusterID clusterID, EntityUID entityUID, const EndpointAddress& endpoint, const VariableTable& customAttributes);
			virtual ~LocalService() = default;

			GameID GetGameID() const { return GameId; }
			ClusterID GetClusterID() const { return ClusterId; }
			EntityUID GetEntityUID() const { return EntityUid; }
			const VariableTable& GetAttributes() const { return Attributes; }

			const TimeStampMS& GetExpectedTickTime() const { return m_ExpectedTickTime; }
			Result ScheduleTickUpdate(DurationMS delay);

			virtual Result Register() = 0;
			virtual Result Deregister() = 0;

			virtual bool OnTimerTick();
		};


		class LocalServiceZookeeper : public LocalService
		{
		public:
			String NodePath;
			Json::Value JsonAttributes;

		public:
			LocalServiceZookeeper(ServiceDirectoryManager& owner, GameID gameID, ClusterID clusterID, EntityUID entityUID, const EndpointAddress& endpoint, const VariableTable& customAttributes);
			virtual ~LocalServiceZookeeper() = default;

			virtual Result Register() override;
			virtual Result Deregister() override;
		};


		class LocalServiceMongo : public LocalService
		{
		public:

			using super = LocalService;

			BsonUniquePtr BsonAttributes;

		public:
			LocalServiceMongo(ServiceDirectoryManager& owner, GameID gameID, ClusterID clusterID, EntityUID entityUID, const EndpointAddress& endpoint, const VariableTable& customAttributes);
			virtual ~LocalServiceMongo() = default;

			Result CreateBsonAttribute();

			virtual Result Register() override;
			virtual Result Deregister() override;
		};

	private:

		// cluster information by clusterID
		ClusterInfomationMap			m_ClusterInfoMap;

		CriticalSection m_ServiceLock;
		DynamicArray<SharedPointerT<LocalService>> m_LocalServices;

		SFUniquePtr<Thread> m_TickThread;
		WeakPointerT<LocalService> m_LastTickItem;
		CircularPageQueue<WeakPointerT<LocalService>> m_TickQueue;

		SharedPointerT<MongoDB> m_MongoDB;

	private:

		static Result ToJsonMessageEndpoint(Json::Value& jsonObject, const char* keyName, const EndpointAddress& messageEndpoint);
		static Result ToJson(Json::Value& jsonObject, const char* keyName, const VariableTable& customAttributes);

		Result RegisterLocalServices();
		
		void ScheduleTickUpdate(LocalService* localService);

		friend class LocalService;

	public:
		// Constructor/Destructor
		ServiceDirectoryManager();
		virtual ~ServiceDirectoryManager();

		IHeap& GetHeap() { return GetSystemHeap(); }

		Result InitializeComponent();
		// Terminate component
		void DeinitializeComponent();

		void TickUpdate();


		///////////////////////////////////////////////////////////////////////////////////////////////
		// Utility


		MongoDBCollection* GetObjectCollection(GameID gameID, ClusterID clusterID);

		bool UseObjectCluster(ClusterID clusterId) const;

		Result CreateBsonAttribute(GameID gameID, ClusterID clusterID, EntityUID entityUID, const VariableTable& customAttributes, BsonUniquePtr& BsonAttributes);


		///////////////////////////////////////////////////////////////////////////////////////////////
		// Interface implementation

		// Get random cluster service
		virtual Result GetRandomService(GameID gameID, ClusterID clusterID, SharedPointerT<ServerServiceInformation>& pServiceInfo) override;

		// Set watch state for cluster
		virtual Result WatchForService(GameID gameID, ClusterID clusterID) override;

		virtual Result FindObjects(GameID gameID, ClusterID clusterID, const VariableTable& searchAttributes, Array<SharedPointerT<EntityInformation>>& foundObjects) override;

		virtual Result PingObjectDirectory(GameID gameID, ClusterID clusterID, EntityUID entityUID, const VariableTable& customAttributes) override;

		virtual Result RegisterLocalService(GameID gameID, ClusterID clusterID, EntityUID entityUID, const EndpointAddress& endpoint, const VariableTable& customAttributes) override;
		virtual Result RegisterLocalService(ServiceEntity* pServiceEntity) override;

		virtual Result RemoveLocalService(GameID gameID, ClusterID clusterID, EntityUID entityUID) override;
		virtual Result RemoveLocalService(ServiceEntity* pServiceEntity) override;
	};

} // namespace SF

