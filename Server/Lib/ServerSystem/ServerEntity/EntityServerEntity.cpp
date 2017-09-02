////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Entity server entity
//	
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "String/StrUtil.h"
#include "Util/TimeUtil.h"
#include "ServerLog/SvrLog.h"
#include "Thread/Thread.h"

#include "SvrConst.h"
#include "ServerSystem/SvrConst.h"
#include "ServerSystem/ServerEntity.h"
#include "ServerSystem/Transaction.h"
//#include "ServerSystem/PlugIn.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/BrServer.h"
#include "ServerSystem/ServerEntityManager.h"


#include "ServerSystem/ServerEntity/EntityServerEntity.h"
#include "ServerSystem/ServiceEntity/ClusterManagerServiceEntity.h"
#include "ServerSystem/Transaction/ServerTransactionGeneric.h"

#include "Protocol/Policy/EntityServerNetPolicy.h"
#include "Protocol/Policy/ServerNetPolicy.h"

#include "Protocol/Message/ServerMsgClass.h"
#include "Protocol/Policy/ServerNetPolicy.h"


namespace SF {
namespace Svr
{
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Entity server entity
	//


	EntityServerEntity::EntityServerEntity( uint uiTransQueueSize, uint TransResQueueSize )
		:ServerEntity(	uiTransQueueSize > 64 ? uiTransQueueSize : Const::ENTITY_ENTITY_TRANS_QUEUE, 
						TransResQueueSize > 64 ? TransResQueueSize : Const::ENTITY_ENTITY_TRANSRES_QUEUE )
	{
	}

	EntityServerEntity::~EntityServerEntity()
	{
	}
		

	// Initialize entity to proceed new connection
	Result EntityServerEntity::InitializeEntity( EntityID newEntityID )
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(ServerEntity::InitializeEntity( newEntityID ) );

		BR_ENTITY_MESSAGE( Message::Server::ServerConnectedC2SEvt)		{ pNewTrans = new EntityServerStartedTrans( pMsgData ); return ResultCode::SUCCESS; } );

	Proc_End:

		return hr;
	}



	// Process Connection event
	Result EntityServerEntity::ProcessConnectionEvent( const Net::ConnectionEvent& conEvent )
	{
		Result hr = ResultCode::SUCCESS;

		switch( conEvent.EventType )
		{
		case Net::ConnectionEvent::EVT_CONNECTION_RESULT:
			if( (conEvent.hr) // && IsInitialConnection()
				&& GetPolicy<Policy::IPolicyEntityServer>() )
			{
				// Register entity manager server
				svrChk( BrServer::GetInstance()->GetComponent<Svr::ServerEntityManager>()->UpdateEntityManagerServerEntity( this ) );
			}
			break;
		case Net::ConnectionEvent::EVT_DISCONNECTED:
			break;
		case Net::ConnectionEvent::EVT_STATE_CHANGE:
			break;
		default:
			break;
		};

	Proc_End:
		
		return ServerEntity::ProcessConnectionEvent( conEvent );
	}



}; // namespace Svr
}; // namespace SF



