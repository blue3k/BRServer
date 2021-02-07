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
#include "String/SFStringCrcDB.h"




namespace SF {

	class ServerServiceInformation;
	namespace Svr {
		class ClusteredServiceEntity;
	}
	

	////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ClusterService information
	struct ClusterServiceInfo
	{
		SortedArray<StringCrc64, ServerServiceInformation*> Services;

		ClusterServiceInfo(IHeap& heap)
			: Services(heap)
		{
		}
	};


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
		virtual ClusterServiceInfo* GetClusterInfo(GameID gameID, ClusterID clusterID) { return nullptr; }

		// Get random cluster service
		virtual Result GetRandomService(GameID gameID, ClusterID clusterID, ServerServiceInformation* &pServiceInfo) { return ResultCode::NOT_IMPLEMENTED; }
		virtual Result GetShardService(GameID gameID, ClusterID clusterID, uint64_t shardKey, ServerServiceInformation* &pServiceInfo) { return ResultCode::NOT_IMPLEMENTED; }
		virtual Result GetNextService(ServerServiceInformation* pServiceInfo, ServerServiceInformation* &pNextServiceInfo) { return ResultCode::NOT_IMPLEMENTED; }

		// Add cluster service entity
		virtual Result AddClusterServiceEntity(Svr::ClusteredServiceEntity* pServiceEntity, ServerServiceInformation* &pServiceInfo) { return ResultCode::NOT_IMPLEMENTED; }
		virtual Result AddClusterServiceEntity(Svr::ClusteredServiceEntity* pServiceEntity) { ServerServiceInformation* pServiceInfo = nullptr; return AddClusterServiceEntity(pServiceEntity, pServiceInfo); }

		// Create a watcher for cluster, replacement for CreateWatcherForCluster
		virtual Result WatchForService(GameID gameID, ClusterID clusterID) { return ResultCode::NOT_IMPLEMENTED; }

		virtual Result UpdateWorkLoad(Svr::ClusteredServiceEntity* pServiceEntity) { return ResultCode::NOT_IMPLEMENTED; }
		virtual Result UpdateServiceStatus(Svr::ClusteredServiceEntity* pServiceEntity) { return ResultCode::NOT_IMPLEMENTED; }

		virtual Result RegisterClustereWatchers(GameID gameID, ClusterID clusterID, ClusterID clusterIDEnd)
		{
			Result hr = ResultCode::SUCCESS;

			if (clusterID != ClusterID::Invalid && clusterIDEnd != ClusterID::Invalid)
			{
				for (; (int)clusterID <= (int)clusterIDEnd; clusterID++)
				{
					hr = WatchForService(gameID, clusterID);
					if (!hr)
						return hr;
				}
			}

		Proc_End:

			return hr;
		}

	};

	

}; // namespace SF

