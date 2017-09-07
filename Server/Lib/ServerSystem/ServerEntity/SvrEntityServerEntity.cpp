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
#include "SvrConst.h"
#include "ServerEntity/ServerEntity.h"
#include "Transaction/Transaction.h"
//#include "ServerSystem/PlugIn.h"
#include "SvrTrace.h"
#include "Server/BrServer.h"
#include "ServerEntity/ServerEntityManager.h"


#include "ServerEntity/SvrEntityServerEntity.h"
#include "ServiceEntity/ClusterManagerServiceEntity.h"
#include "Transaction/ServerTransactionGeneric.h"

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

		BR_ENTITY_MESSAGE( Message::Server::ServerConnectedC2SEvt)		{ pNewTrans = new(GetHeap()) EntityServerStartedTrans(GetHeap(), pMsgData ); return ResultCode::SUCCESS; } );

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
				&& GetConnection() != nullptr )
			{
				// Register entity manager server
				svrChk( GetServerComponent<Svr::ServerEntityManager>()->UpdateEntityManagerServerEntity( this ) );
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



