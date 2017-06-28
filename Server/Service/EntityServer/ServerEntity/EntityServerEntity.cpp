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
#include "Common/StrUtil.h"
#include "Common/TimeUtil.h"
#include "Common/Trace.h"
#include "Common/Thread.h"
#include "ServerSystem/SvrConstDefault.h"
#include "ServerSystem/ServerEntity.h"
#include "ServerSystem/Transaction.h"
//#include "ServerSystem/PlugIn.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/ServiceEntity/ClusterManagerServiceEntity.h"
#include "EntityServerEntity.h"
#include "EntitySvrConst.h"

#include "Protocol/Policy/LoginServerIPolicy.h"
#include "Transaction/EntityTransactionServer.h"
#include "Transaction/EntityTransactionUser.h"



namespace BR {
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
	Result EntityServerEntity::ProcessConnectionEvent( const BR::Net::IConnection::Event& conEvent )
	{
		Result hr = ResultCode::SUCCESS;
		Svr::ClusterManagerServiceEntity *pClusterManager = nullptr;
		Svr::ClusteredServiceEntity *pServiceEntity = nullptr;
		Svr::ServerServiceInformation *pServiceInfo = nullptr;

		Svr::EntityServerEntity::ProcessConnectionEvent( conEvent );

		switch( conEvent.EventType )
		{
		case Net::IConnection::Event::EVT_CONNECTION_RESULT:
			pClusterManager = Svr::GetServerComponent<Svr::ClusterManagerServiceEntity>();
			
			// Add all expected cluster service objects
			for( ClusterID clusterID = ClusterID::ClusterManager; clusterID < ClusterID::Max; clusterID++ )
			{
				EntityUID expectedUID( GetServerID(), EntityID(EntityFaculty::Service,(UINT)clusterID) );
				if (!(pClusterManager->GetClusterServiceEntity(clusterID, pServiceEntity)))
					continue;

				if( !(pServiceEntity->FindService( expectedUID, pServiceInfo )) )
				{
					Svr::ServerServiceInformation *pService = nullptr;
					svrTrace( Svr::TRC_CLUSTER, "Adding remote cluster info EntityID:{0}, ClusterID:{1}", expectedUID, clusterID );
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
}; // namespace BR



