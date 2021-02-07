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

#include "Memory/SFMemory.h"
#include "Types/SFEngineTypedefs.h"
#include "Types/BrSvrTypes.h"
#include "Entity/Entity.h"
#include "Component/ServerComponent.h"
#include "ServerService/ServerServiceBase.h"
#include "Container/SFHashTable.h"
#include "Container/SFIndexing.h"

#include "Entity/EntityInformation.h"
#include "Service/ServiceDirectoryService.h"
#include "Zookeeper/SFZookeeper.h"
#include "json/json.h"


namespace SF {

	namespace Svr {
		class Entity;
	}


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



	// Service cluster information
	class ServiceCluster : public ZookeeperWatcher
	{

	private:
		IHeap& m_Heap;

		bool m_ZKInitialized = false;

		ServiceClusterSearchKey m_ClusterKey;
		String m_ClusterPath;

		Atomic<uint> m_LatestSelected;

		SharedPointerT<ZookeeperWatcher::StringsTask> m_GetChildrenTask;

		SortedArray<StringCrc64, ServerServiceInformation*> m_Services;

		friend class ServiceDirectoryManager;

	private:

		void InitZK();

		static Result GetNodeValue(const String& nodePath, Json::Value& jsonValue);
		static Result SetNodeValue(const String& nodePath, const Json::Value& jsonValue);

		static Result ParseMessageEndpoint(const Json::Value& jsonObject, const char* keyName, ServerConfig::MessageEndpoint& outMessageEndpoint);

		Result AddServiceInfo(const char* nodeName, StringCrc64 nodeNameCrc);


	public:
		ServiceCluster(IHeap& heap, GameID gameID, ClusterID clusterID);
		~ServiceCluster();

		IHeap& GetHeap() { return m_Heap; }

		bool IsZKInitialized() { return m_ZKInitialized; }

		void DownloadServiceInfo();


		/////////////////////////////////////////////
		//
		//	Overridable Event handling
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

		struct LocalServiceInformation
		{
			GameID GameID;
			ClusterID ClusterID;
			EntityUID EntityUID;
			ServerConfig::MessageEndpoint Endpoint;
			String NodePath;
			Json::Value JsonAttributes;
		};


	private:

		// cluster information by clusterID
		ClusterInfomationMap			m_ClusterInfoMap;

		DynamicArray<LocalServiceInformation*> m_LocalServices;

	private:

		static Result ToJsonMessageEndpoint(Json::Value& jsonObject, const char* keyName, const ServerConfig::MessageEndpoint& messageEndpoint);

		Result RegisterLocalServices();

	public:
		// Constructor/Destructor
		ServiceDirectoryManager();
		virtual ~ServiceDirectoryManager();

		Result InitializeComponent();
		// Terminate component
		void DeinitializeComponent();

		// Get random cluster service
		virtual Result GetRandomService(GameID gameID, ClusterID clusterID, ServerServiceInformation*& pServiceInfo) override;
		virtual Result GetShardService(GameID gameID, ClusterID clusterID, uint64_t shardKey, ServerServiceInformation*& pServiceInfo) override;
		virtual Result GetNextService(ServerServiceInformation* pServiceInfo, ServerServiceInformation*& pNextServiceInfo) override;

		// Set watch state for cluster
		virtual Result WatchForService(GameID gameID, ClusterID clusterID) override;

		virtual Result RegisterLocalService(GameID gameID, ClusterID clusterID, EntityUID entityUID, const ServerConfig::MessageEndpoint& endpoint) override;
		virtual Result RegisterLocalService(ServiceEntity* pServiceEntity) override;

	};

} // namespace SF

