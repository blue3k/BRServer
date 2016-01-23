////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Game master server
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
#include "GameMasterServerEntity.h"
#include "EntitySvrConst.h"

#include "Protocol/Policy/GameMasterServerIPolicy.h"

#include "Transaction/EntityTransactionServer.h"


namespace BR {
namespace EntityServer {

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Game server entity
	//


	GameMasterServerEntity::GameMasterServerEntity()
		:Svr::GameMasterServerEntity( Const::ENTITY_ZONE_TRANS_QUEUE, Const::ENTITY_ZONE_TRANSRES_QUEUE )
	{
	}

	GameMasterServerEntity::~GameMasterServerEntity()
	{
	}


	// Initialize entity to proceed new connection
	HRESULT GameMasterServerEntity::InitializeEntity( EntityID newEntityID )
	{
		HRESULT hr = S_SYSTEM_OK;

		svrChk(Svr::GameMasterServerEntity::InitializeEntity( newEntityID ) );

		BR_ENTITY_MESSAGE( Message::Server::ServerConnectedC2SEvt)		{ pNewTrans = new TransServerStarted( pMsgData ); return S_SYSTEM_OK; } );

	Proc_End:

		return hr;
	}


} // namespace EntityServer
} // namespace BR



