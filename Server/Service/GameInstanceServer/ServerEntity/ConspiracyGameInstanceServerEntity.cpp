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


#include "StdAfx.h"
#include "Common/StrUtil.h"
#include "Common/TimeUtil.h"
#include "Common/Trace.h"
#include "Common/Thread.h"
#include "ServerSystem/BrServer.h"
#include "ServerSystem/SvrConstDefault.h"
#include "ServerSystem/ServerEntity.h"
#include "ServerSystem/Transaction.h"
//#include "ServerSystem/PlugIn.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/Transaction/ServerTransactionGeneric.h"


#include "ConspiracyGameInstanceServerEntity.h"
#include "ConspiracyGameInstanceSvrConst.h"

#include "Protocol/Message/GameServerMsgClass.h"
#include "Protocol/Policy/GameServerIPolicy.h"
#include "Protocol/Message/GameInstanceMsgClass.h"
#include "Protocol/Policy/GameInstanceIPolicy.h"




namespace BR {
namespace ConspiracyGameInstanceServer {


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Game server entity
	//

	ConspiracyGameInstanceServerEntity::ConspiracyGameInstanceServerEntity()
		:Svr::ServerEntity( Const::ENTITY_GAME_TRANS_QUEUE, Const::ENTITY_GAME_TRANSRES_QUEUE )
	{
	}

	ConspiracyGameInstanceServerEntity::~ConspiracyGameInstanceServerEntity()
	{
	}


	// Process Connection event
	HRESULT ConspiracyGameInstanceServerEntity::ProcessConnectionEvent( const BR::Net::IConnection::Event& conEvent )
	{
		HRESULT hr = S_OK;

		switch( conEvent.EventType )
		{
		case BR::Net::IConnection::Event::EVT_CONNECTION_RESULT:
			break;
		case BR::Net::IConnection::Event::EVT_DISCONNECTED:

			break;
		case BR::Net::IConnection::Event::EVT_STATE_CHANGE:

			break;
		};

	Proc_End:

		return __super::ProcessConnectionEvent( conEvent );
	}

	// register message handlers
	HRESULT ConspiracyGameInstanceServerEntity::RegisterMessageHandlers()
	{
		HRESULT hr = S_OK;

		svrChk( __super::RegisterMessageHandlers() );

		BR_ENTITY_MESSAGE( Message::Server::ServerConnectedC2SEvt)					{ svrMemReturn(pNewTrans = new Svr::GenericServerStartedTrans( pMsgData )); return S_OK; } );


	Proc_End:

		return hr;
	}


}; // namespace ConspiracyGameInstanceServer
}; // namespace BR



