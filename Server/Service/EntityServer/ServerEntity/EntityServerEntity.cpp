////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Remote Entity implementation
//	
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "String/StrUtil.h"
#include "Util/TimeUtil.h"
#include "ServerLog/SvrLog.h"
#include "Thread/Thread.h"
#include "SvrConst.h"
#include "ServerEntity/ServerEntity.h"
#include "Transaction/Transaction.h"
//#include "ServerSystem/PlugIn.h"
#include "SvrTrace.h"
#include "ServiceEntity/ClusterManagerServiceEntity.h"
#include "EntityServerEntity.h"
#include "EntitySvrConst.h"

#include "Protocol/Policy/LoginServerNetPolicy.h"
#include "Transaction/EntityTransactionServer.h"
#include "Transaction/EntityTransactionUser.h"



namespace SF {
namespace EntityServer {
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Game server entity
	//


	EntityServerEntity::EntityServerEntity()
		:Svr::EntityServerEntity( Const::ENTITY_WORLD_TRANS_QUEUE, Const::ENTITY_WORLD_TRANSRES_QUEUE )
	{
	}

	EntityServerEntity::~EntityServerEntity()
	{
	}


	// Initialize entity to proceed new connection
	Result EntityServerEntity::ProcessConnectionEvent( const BR::Net::ConnectionEvent& conEvent )
	{
		Result hr = ResultCode::SUCCESS;
		Svr::ClusterManagerServiceEntity *pClusterManager = nullptr;
		Svr::ClusteredServiceEntity *pServiceEntity = nullptr;
		Svr::ServerServiceInformation *pServiceInfo = nullptr;

		Svr::EntityServerEntity::ProcessConnectionEvent( conEvent );

		switch( conEvent.EventType )
		{
		case Net::ConnectionEvent::EVT_CONNECTION_RESULT:
			pClusterManager = Svr::GetServerComponent<Svr::ClusterManagerServiceEntity>();
			
			// Add all expected cluster service objects
			for( ClusterID clusterID = ClusterID::ClusterManager; clusterID < ClusterID::Max; clusterID++ )
			{
				EntityUID expectedUID( GetServerID(), EntityID(EntityFaculty::Service,(uint)clusterID) );
				if (!(pClusterManager->GetClusterServiceEntity(clusterID, pServiceEntity)))
					continue;

				if( !(pServiceEntity->FindService( expectedUID, pServiceInfo )) )
				{
					Svr::ServerServiceInformation *pService = nullptr;
					svrTrace( SVR_CLUSTER, "Adding remote cluster info EntityID:{0}, ClusterID:{1}", expectedUID, clusterID );
					svrChk( pServiceEntity->NewServerService( expectedUID, this, clusterID == ClusterID::ClusterManager ? ClusterMembership::Slave : ClusterMembership::StatusWatcher, ServiceStatus::Ready, pService ) );
				}
			}
			break;
		default:
			break;
		};

	Proc_End:
		
		return hr;
	}


}; // namespace EntityServer
}; // namespace SF



