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

	class ClusteredServiceEntity;
	
	/////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Server cluster manager service
	//

	class ServerClusterService
	{
	protected:

	public:

		ServerClusterService()
		{}

		virtual ~ServerClusterService()
		{
		}


		// Get cluster service entity
		virtual Result GetServiceEntity(ClusterID clusterID, ClusteredServiceEntity* &pServiceEntity) { return ResultCode::NOT_IMPLEMENTED; }
		virtual Result GetServiceEntity(FixedString gameClusterID, FixedString clusterName, ClusteredServiceEntity* &pServiceEntity) { return ResultCode::NOT_IMPLEMENTED; }

		template<class ServiceType>
		Result GetRandomService(FixedString gameClusterID, FixedString clusterName, ServiceType* &pServiceInterface)
		{
			Result hr;
			Svr::ServerServiceInformation *pServiceInfo = nullptr;

			pServiceInterface = nullptr;

			hr = GetServiceEntity(gameClusterID, clusterName, pServiceEntity);
			if (!hr)
				return hr;

			hr = pServiceEntity->FindRandomService(pServiceInfo);
			if (!hr)
				return hr;

			pServiceInterface = pServiceInfo->GetService<Svr::GameServerService>();

		Proc_End:

			return pServiceInterface != nullptr ? hr : ResultCode::SVR_SERVICE_FAILED;
		}

		// Add cluster service entity
		virtual Result AddServiceEntity( ClusteredServiceEntity* pServiceEntity ) { return ResultCode::NOT_IMPLEMENTED; }

		// Create a watcher for cluster, replacement for CreateWatcherForCluster
		virtual Result WatchForService(FixedString gameClusterID, FixedString clusterName) { return ResultCode::NOT_IMPLEMENTED; }

	};

	

}; // namespace SF

