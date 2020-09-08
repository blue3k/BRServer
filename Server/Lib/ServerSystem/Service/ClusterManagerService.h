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

	namespace Svr {
		class ClusteredServiceEntity;
		class ServerServiceInformation;
	}
	

	////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ClusterService information
	struct ClusterServiceInfo
	{
		SortedArray<StringCrc64, Svr::ServerServiceInformation*> Services;
		DynamicArray<SharedPointerT<Svr::ClusteredServiceEntity>> LocalServiceEntites;

		ClusterServiceInfo(IHeap& heap)
			: Services(heap)
			, LocalServiceEntites(heap)
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

		// Initialize not initialized cluster entities
		// This need to be called after clusterManagerService is initialized
		virtual Result InitializeNotInitializedClusterEntities() { return ResultCode::NOT_IMPLEMENTED; }

		// Get cluster info
		virtual ClusterServiceInfo* GetClusterInfo(GameID gameID, ClusterID clusterID) { return nullptr; }

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

		virtual Result RegisterClustereWatchers(GameID gameID, ClusterID clusterID, ClusterID clusterIDEnd)
		{
			Result hr = ResultCode::SUCCESS;

			if (clusterID != ClusterID::Invalid && clusterIDEnd != ClusterID::Invalid)
			{
				for (; (int)clusterID <= (int)clusterIDEnd; clusterID++)
				{
					hr = SetWatchForCluster(gameID, clusterID, true);
					if (!hr)
						return hr;
				}
			}

		Proc_End:

			return hr;
		}

	};

	

}; // namespace SF

