////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 StormForge
// 
// Author : KyungKun Ko
//
// Description : Server cluster service
//
////////////////////////////////////////////////////////////////////////////////


#pragma once 

#include "SFTypedefs.h"
#include "String/FixedStringDB.h"




namespace SF {

	namespace Svr {
		class ClusteredServiceEntity;
		class ServerServiceInformation;
		class ClusterServiceInfo;
	}
	

	/////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Server cluster manager service
	//

	class ClusterManagerService
	{
	protected:

	public:

		ClusterManagerService()
		{}

		virtual ~ClusterManagerService()
		{
		}

		// Returns whether the initial sync is finished or not
		virtual bool GetIsInitialized() { return false; }

		virtual void Clear() {}

		// Get cluster info
		virtual Result GetClusterInfo(GameID gameID, ClusterID clusterID, Svr::ClusterServiceInfo* &pServiceInfo) { return ResultCode::NOT_IMPLEMENTED; }

		// Get random cluster service
		virtual Result GetRandomService(GameID gameID, ClusterID clusterID, Svr::ServerServiceInformation* &pServiceInfo) { return ResultCode::NOT_IMPLEMENTED; }
		virtual Result GetShardService(GameID gameID, ClusterID clusterID, uint64_t shardKey, Svr::ServerServiceInformation* &pServiceInfo) { return ResultCode::NOT_IMPLEMENTED; }
		virtual Result GetNextService(Svr::ServerServiceInformation* pServiceInfo, Svr::ServerServiceInformation* &pNextServiceInfo) { return ResultCode::NOT_IMPLEMENTED; }

		// Add cluster service entity
		virtual Result AddClusterServiceEntity(Svr::ClusteredServiceEntity* pServiceEntity, Svr::ServerServiceInformation* &pServiceInfo) { return ResultCode::NOT_IMPLEMENTED; }
		virtual Result AddClusterServiceEntity(Svr::ClusteredServiceEntity* pServiceEntity) { Svr::ServerServiceInformation* pServiceInfo = nullptr; return AddClusterServiceEntity(pServiceEntity, pServiceInfo); }

		// Create a watcher for cluster, replacement for CreateWatcherForCluster
		virtual Result SetWatchForCluster(GameID gameID, ClusterID clusterID, bool activelyConnect) { return ResultCode::NOT_IMPLEMENTED; }

		virtual Result UpdateWorkLoad(Svr::ClusteredServiceEntity* pServiceEntity) { return ResultCode::NOT_IMPLEMENTED; }
		virtual Result UpdateServiceStatus(Svr::ClusteredServiceEntity* pServiceEntity) { return ResultCode::NOT_IMPLEMENTED; }

	};

	

}; // namespace SF

