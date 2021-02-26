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
#include "Util/SFStringCrcDB.h"
#include "Variable/SFVariableTable.h"



namespace SF {

	class ServerServiceInformation;
	class EntityInformation;
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
		virtual Result GetRandomService(GameID gameID, ClusterID clusterID, SharedPointerT<ServerServiceInformation> &pServiceInfo) { return ResultCode::NOT_IMPLEMENTED; }

		// Create a watcher for cluster, replacement for CreateWatcherForCluster
		virtual Result WatchForService(GameID gameID, ClusterID clusterID) { return ResultCode::NOT_IMPLEMENTED; }

		// Search object list
		// works only for object cluster
		virtual Result FindObjects(GameID gameID, ClusterID clusterID, const VariableTable& searchAttributes, Array<SharedPointerT<EntityInformation>>& foundObjects) { return ResultCode::NOT_IMPLEMENTED; }

		// register local service to directory
		virtual Result RegisterLocalService(GameID gameID, ClusterID clusterID, EntityUID entityUID, const EndpointAddress& endpoint = {}, const VariableTable& customAttributes = {}) { return ResultCode::NOT_IMPLEMENTED; }
		virtual Result RemoveLocalService(GameID gameID, ClusterID clusterID, EntityUID entityUID) { return ResultCode::NOT_IMPLEMENTED; }
		virtual Result RegisterLocalService(ServiceEntity* pServiceEntity) { return ResultCode::NOT_IMPLEMENTED; }
		virtual Result RemoveLocalService(ServiceEntity* pServiceEntity) { return ResultCode::NOT_IMPLEMENTED; }
	};

	

} // namespace SF

