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
#include "GameServerEntity.h"
#include "EntitySvrConst.h"

#include "Protocol/Policy/GameServerIPolicy.h"
#include "Transaction/EntityTransactionServer.h"
#include "Transaction/EntityTransactionUser.h"



namespace BR {
namespace EntityServer {
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Game server entity
	//


	GameServerEntity::GameServerEntity()
		:Svr::GameServerEntity( Const::ENTITY_WORLD_TRANS_QUEUE, Const::ENTITY_WORLD_TRANSRES_QUEUE )
	{
	}

	GameServerEntity::~GameServerEntity()
	{
	}

	
	// Initialize entity to proceed new connection
	HRESULT GameServerEntity::InitializeEntity( EntityID newEntityID )
	{
		HRESULT hr = S_SYSTEM_OK;

		svrChk(Svr::GameServerEntity::InitializeEntity( newEntityID ) );

		BR_ENTITY_MESSAGE( Message::Server::ServerConnectedC2SEvt)			{ pNewTrans = new TransGameServerStarted( pMsgData ); return S_SYSTEM_OK; } );
		//BR_ENTITY_MESSAGE( Message::EntityServer::RegisterUserEntityCmd)	{ pNewTrans = new TransRegisterUserEntity( pMsgData );return S_SYSTEM_OK; } );
		//BR_ENTITY_MESSAGE( Message::EntityServer::UnregisterUserEntityCmd)	{ pNewTrans = new TransUnregisterUserEntity( pMsgData );return S_SYSTEM_OK;  } );
		//BR_ENTITY_MESSAGE( Message::EntityServer::UpdateUserEntityCmd)		{ pNewTrans = new TransUpdateUserEntity( pMsgData );return S_SYSTEM_OK;  } );
		//BR_ENTITY_MESSAGE( Message::EntityServer::FindUserEntityCmd)		{ pNewTrans = new TransFindUserEntity( pMsgData );return S_SYSTEM_OK;  } );

	Proc_End:

		return hr;
	}


}; // namespace EntityServer
}; // namespace BR



