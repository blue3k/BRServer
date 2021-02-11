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
#include "Variable/SFVariableTable.h"



namespace SF {

	class ServerServiceInformation;
	class ServiceEntity;
	class VariableTable;

	namespace ServerConfig
	{
		struct MessageEndpoint;
	}


	/////////////////////////////////////////////////////////////////////////////////////////
	//
	//	service directory manager service
	//

	class ServiceDirectoryService
	{
	protected:

	public:

		ServiceDirectoryService() = default;
		virtual ~ServiceDirectoryService() = default;

		// Get random cluster service
		virtual Result GetRandomService(GameID gameID, ClusterID clusterID, ServerServiceInformation* &pServiceInfo) { return ResultCode::NOT_IMPLEMENTED; }
		virtual Result GetShardService(GameID gameID, ClusterID clusterID, uint64_t shardKey, ServerServiceInformation* &pServiceInfo) { return ResultCode::NOT_IMPLEMENTED; }
		virtual Result GetNextService(ServerServiceInformation* pServiceInfo, ServerServiceInformation* &pNextServiceInfo) { return ResultCode::NOT_IMPLEMENTED; }

		// Create a watcher for cluster, replacement for CreateWatcherForCluster
		virtual Result WatchForService(GameID gameID, ClusterID clusterID) { return ResultCode::NOT_IMPLEMENTED; }

		// register local service to directory
		virtual Result RegisterLocalService(GameID gameID, ClusterID clusterID, EntityUID entityUID, const EndpointAddress& endpoint, const VariableTable& customAttributes) { return ResultCode::NOT_IMPLEMENTED; }
		virtual Result RegisterLocalService(ServiceEntity* pServiceEntity) { return ResultCode::NOT_IMPLEMENTED; }
		virtual Result RemoveLocalService(ServiceEntity* pServiceEntity) { return ResultCode::NOT_IMPLEMENTED; }
	};

	

} // namespace SF

