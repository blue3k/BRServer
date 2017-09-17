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
#include "Types/BrSvrTypes.h"
#include "Entity/Entity.h"
#include "Component/ServerComponent.h"
#include "ServerService/ServerServiceBase.h"
#include "Container/HashTable.h"
#include "Container/Indexing.h"

#include "Entity/EntityInformation.h"
#include "ServiceEntity/ClusteredServiceEntity.h"
#include "ZooKeeper/SFZooKeeper.h"
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
		};
		uint64_t Composited;

		ClusterSearchKey() : Composited(0) {}
		ClusterSearchKey(GameID gameID, ClusterID clusterID) : GameClusterID(gameID), ServiceClusterID(clusterID) {}

		operator uint64_t() { return Composited; }
	};
#pragma pack(pop)

	class ClusterServiceInfo : public ZooKeeperWatcher
	{
	public:

	private:
		IHeap& m_Heap;
		bool m_ZKInitialized = false;
		bool m_ActivelyConnect = false;
		ClusterSearchKey m_ClusterKey;
		String m_ClusterPath;
		std::atomic<uint> m_LatestSelected;

		friend class ClusterManagerServiceEntity;

	public:

		SortedArray<FixedString, ServerServiceInformation*> Services;
		DynamicArray<SharedPointerT<ClusteredServiceEntity>> LocalServiceEntites;

		// Tasks

		SharedPointerT<ZooKeeperWatcher::StringsTask> GetChildrenTask;

	private:

		void InitZK();

		Result GetNodeValue(const String& nodePath, Json::Value& jsonValue);
		Result SetNodeValue(const String& nodePath, const Json::Value& jsonValue);


		Json::Value ToJsonNetPrivate(const NetAddress& privateNet) const;

		Result ParseNetPrivate(const Json::Value& json, NetAddress& privateNet);

		Result AddNewService(const char* nodeName, FixedString nodeNameCrc);


	public:
		ClusterServiceInfo(IHeap& heap, GameID gameID, ClusterID clusterID, bool activelyConnect);
		~ClusterServiceInfo();

		IHeap& GetHeap() { return m_Heap; }

		bool IsZKInitialized() { return m_ZKInitialized; }

		void DownloadServiceInfo();
		void UploadLocalServiceInfo();

		ServerServiceInformation* NewLocalService(ClusteredServiceEntity* pServiceEntity);

		/////////////////////////////////////////////
		//
		//	Overridable Event handling
		//

		virtual Result OnNewEvent(const ZKEvent& eventOut) override;
		virtual void OnComlition(ZooKeeperTask& pTask) override;
		virtual void OnStatComlition(StatTask& pTask) override;
		virtual void OnDataComlition(DataTask& pTask) override;
		virtual void OnStringsComlition(StringsTask& pTask) override;
		virtual void OnStringsStatComlition(StringsStatTask& pTask) override;
		virtual void OnStringComlition(StringTask& pTask) override;
		virtual void OnACLComlition(ACLTask& pTask) override;

	};


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	ClusterManagerServiceEntity class
	//

	class ClusterManagerServiceEntity : public ServiceEntity, public ClusterManagerService
	{
	public:

		typedef ServiceEntity super;

		static constexpr FixedString TypeName = "ClusterManagerService";

		static constexpr const char* ServiceBasePath = "/BRServices";

		// Server Entity table
		typedef HashTable2<	uint64_t, ClusterServiceInfo* >  ClusterInfomationMap;


	private:

		// cluster information by ID map
		ClusterInfomationMap			m_ClusterInfoMap;


	private:

		Result AddNewService(const char* nodeName, FixedString nodeNameCrc);

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

		// Initialize not initialized cluster entities
		// This need to be called after clusterManagerService is initialized
		virtual Result InitializeNotInitializedClusterEntities() override;


		// Set watch state for cluster
		virtual Result SetWatchForCluster(GameID gameID, ClusterID clusterID, bool activelyConnect) override;
		ClusterServiceInfo* GetOrSetWatchForCluster(GameID gameID, ClusterID clusterID, bool activelyConnect);


		// Get cluster info
		virtual Result GetClusterInfo(GameID gameID, ClusterID clusterID, ClusterServiceInfo* &pServiceInfo) override;

		// Get random cluster service
		virtual Result GetRandomService(GameID gameID, ClusterID clusterID, ServerServiceInformation* &pServiceInfo) override;
		virtual Result GetShardService(GameID gameID, ClusterID clusterID, uint64_t shardKey, ServerServiceInformation* &pServiceInfo) override;
		virtual Result GetNextService(Svr::ServerServiceInformation* pServiceInfo, Svr::ServerServiceInformation* &pNextServiceInfo) override;

		// Add cluster service entity
		virtual Result AddClusterServiceEntity( ClusteredServiceEntity* pServiceEntity, ServerServiceInformation* &pServiceInfo) override;

		virtual Result UpdateWorkLoad( ClusteredServiceEntity* pServiceEntity) override;
		virtual Result UpdateServiceStatus(ClusteredServiceEntity* pServiceEntity) override;


		virtual Result RegisterServiceMessageHandler( ServerEntity *pServerEntity ) override;


		virtual Result TickUpdate(TimerAction *pAction = nullptr) override;


		Result InitializeComponent();
		// Terminate component
		void DeinitializeComponent();

	};




}; // namespace Svr
}; // namespace SF




