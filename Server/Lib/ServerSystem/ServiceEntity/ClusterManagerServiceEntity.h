////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Component class definition
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
#include "ServiceEntity/ClusteredServiceEntity.h"
#include "Zookeeper/SFZookeeper.h"
#include "json/json.h"


namespace SF {
namespace Svr {

	class Entity;
	class ServerEntity;


#pragma pack(push,4)
	union ClusterSearchKey
	{
		struct {
			ClusterID ServiceClusterID;
			GameID GameClusterID;
		} Components;
		uint64_t Composited;

		ClusterSearchKey() : Composited(0) {}
		ClusterSearchKey(GameID gameID, ClusterID clusterID)
		{
			Components.GameClusterID = (gameID);
			Components.ServiceClusterID = (clusterID);
		}

		operator uint64_t() { return Composited; }
	};
#pragma pack(pop)



	// Cluster service information implementation
	class ClusterServiceInfo_Impl : public ZookeeperWatcher, public ClusterServiceInfo
	{
	public:

	private:
		IHeap& m_Heap;
		bool m_ZKInitialized = false;
		ClusterSearchKey m_ClusterKey;
		String m_ClusterPath;
		std::atomic<uint> m_LatestSelected;

		SharedPointerT<ZookeeperWatcher::StringsTask> m_GetChildrenTask;

		friend class ClusterManagerServiceEntity;

	private:

		void InitZK();

		Result GetNodeValue(const String& nodePath, Json::Value& jsonValue);
		Result SetNodeValue(const String& nodePath, const Json::Value& jsonValue);


		Json::Value ToJsonNetPrivate(const NetAddress& privateNet) const;

		Result ParseNetPrivate(const Json::Value& json, NetAddress& privateNet);

		Result AddServiceInfo(const char* nodeName, StringCrc64 nodeNameCrc);


	public:
		ClusterServiceInfo_Impl(IHeap& heap, GameID gameID, ClusterID clusterID);
		~ClusterServiceInfo_Impl();

		IHeap& GetHeap() { return m_Heap; }

		bool IsZKInitialized() { return m_ZKInitialized; }

		void DownloadServiceInfo();
		void UploadLocalServiceInfo();

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

	};


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	ClusterManagerServiceEntity class
	//

	class ClusterManagerServiceEntity : public ServiceEntity, public ClusterManagerService
	{
	public:

		using super = ServiceEntity;

		static constexpr StringCrc64 TypeName = "ClusterManagerService";

		// Server Entity table
		using ClusterInfomationMap = HashTable2<uint64_t, ClusterServiceInfo_Impl*>;


	private:

		// cluster information by clusterID
		ClusterInfomationMap			m_ClusterInfoMap;


	public:
		// Constructor/Destructor
		ClusterManagerServiceEntity();
		virtual ~ClusterManagerServiceEntity();

		virtual void Dispose() override;


		virtual bool GetIsInitialized() override;

		virtual void Clear() override;

		virtual Result InitializeEntity( EntityID newEntityID ) override;
		// Clear resources and change entity state to closed
		virtual Result TerminateEntity() override;

		// Set watch state for cluster
		virtual Result SetWatchForCluster(GameID gameID, ClusterID clusterID) override;
		ClusterServiceInfo* GetOrSetWatchForCluster(GameID gameID, ClusterID clusterID);


		// Get cluster info
		virtual ClusterServiceInfo* GetClusterInfo(GameID gameID, ClusterID clusterID) override;

		// Get random cluster service
		virtual Result GetRandomService(GameID gameID, ClusterID clusterID, ServerServiceInformation* &pServiceInfo) override;
		virtual Result GetShardService(GameID gameID, ClusterID clusterID, uint64_t shardKey, ServerServiceInformation* &pServiceInfo) override;
		virtual Result GetNextService(ServerServiceInformation* pServiceInfo, ServerServiceInformation* &pNextServiceInfo) override;

		virtual Result RegisterServiceMessageHandler() override;


		virtual Result TickUpdate(TimerAction *pAction = nullptr) override;


		Result InitializeComponent();
		// Terminate component
		void DeinitializeComponent();

	};

} // namespace Svr
} // namespace SF

