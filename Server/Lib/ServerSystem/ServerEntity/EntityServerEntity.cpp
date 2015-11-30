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
#include "Common/StrUtil.h"
#include "Common/TimeUtil.h"
#include "Common/Trace.h"
#include "Common/Thread.h"

#include "ServerSystem/SvrConstDefault.h"
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

#include "Protocol/Policy/EntityServerIPolicy.h"
#include "Protocol/Policy/ServerIPolicy.h"

#include "Protocol/Message/ServerMsgClass.h"
#include "Protocol/Policy/ServerIPolicy.h"


namespace BR {
namespace Svr
{
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Entity server entity
	//


	EntityServerEntity::EntityServerEntity( UINT uiTransQueueSize, UINT TransResQueueSize )
		:ServerEntity(	uiTransQueueSize > 64 ? uiTransQueueSize : Const::ENTITY_ENTITY_TRANS_QUEUE, 
						TransResQueueSize > 64 ? TransResQueueSize : Const::ENTITY_ENTITY_TRANSRES_QUEUE )
		,m_Allocator(STDAllocator::GetInstance())
	{
	}

	EntityServerEntity::~EntityServerEntity()
	{
	}
		

	// Initialize entity to proceed new connection
	HRESULT EntityServerEntity::InitializeEntity( EntityID newEntityID )
	{
		HRESULT hr = S_OK;

		svrChk(ServerEntity::InitializeEntity( newEntityID ) );

		BR_ENTITY_MESSAGE( Message::Server::ServerConnectedC2SEvt)		{ pNewTrans = new EntityServerStartedTrans( pMsgData ); return S_OK; } );

	Proc_End:

		return hr;
	}



	// Process Connection event
	HRESULT EntityServerEntity::ProcessConnectionEvent( const BR::Net::IConnection::Event& conEvent )
	{
		HRESULT hr = S_OK;

		switch( conEvent.EventType )
		{
		case BR::Net::IConnection::Event::EVT_CONNECTION_RESULT:
			if( SUCCEEDED(conEvent.Value.hr) // && IsInitialConnection()
				&& GetPolicy<Policy::IPolicyEntityServer>() )
			{
				// Register entity manager server
				svrChk( BrServer::GetInstance()->GetComponent<Svr::ServerEntityManager>()->UpdateEntityManagerServerEntity( this ) );
			}
			break;
		case BR::Net::IConnection::Event::EVT_DISCONNECTED:
			break;
		case BR::Net::IConnection::Event::EVT_STATE_CHANGE:
			break;
		default:
			break;
		};

	Proc_End:
		
		return ServerEntity::ProcessConnectionEvent( conEvent );
	}



}; // namespace Svr
}; // namespace BR



