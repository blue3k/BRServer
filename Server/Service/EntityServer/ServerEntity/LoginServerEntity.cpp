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
#include "ServerSystem/SvrConstDefault.h"
#include "ServerSystem/ServerEntity.h"
#include "ServerSystem/Transaction.h"
//#include "ServerSystem/PlugIn.h"
#include "ServerSystem/SvrTrace.h"
#include "LoginServerEntity.h"
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


	LoginServerEntity::LoginServerEntity()
		:Svr::ServerEntity( Const::ENTITY_WORLD_TRANS_QUEUE, Const::ENTITY_WORLD_TRANSRES_QUEUE )
	{
	}

	LoginServerEntity::~LoginServerEntity()
	{
	}


	// Initialize entity to proceed new connection
	HRESULT LoginServerEntity::InitializeEntity( EntityID newEntityID )
	{
		HRESULT hr = S_OK;

		svrChk( __super::InitializeEntity( newEntityID ) );

		BR_ENTITY_MESSAGE( Message::Server::ServerConnectedC2SEvt)			{ pNewTrans = new TransServerStarted( pMsgData ); return S_OK; } );


	Proc_End:

		return hr;
	}


}; // namespace EntityServer
}; // namespace BR



