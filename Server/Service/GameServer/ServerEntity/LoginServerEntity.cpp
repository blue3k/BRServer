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
#include "ServerSystem/ServerEntity.h"
#include "ServerSystem/Transaction.h"
#include "ServerSystem/Transaction/ServerTransactionGeneric.h"
//#include "ServerSystem/PlugIn.h"
#include "ServerSystem/SvrTrace.h"
#include "LoginServerEntity.h"
#include "GameSvrConst.h"

#include "Protocol/Policy/EntityServerIPolicy.h"



namespace BR {
namespace GameServer {


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Entity server entity
	//


	LoginServerEntity::LoginServerEntity()
		:Svr::ServerEntity( Const::ENTITY_ENTITY_TRANS_QUEUE, Const::ENTITY_ENTITY_TRANSRES_QUEUE )
	{
	}

	LoginServerEntity::~LoginServerEntity()
	{
	}



	// Initialize entity to proceed new connection
	HRESULT LoginServerEntity::InitializeEntity( EntityID newEntityID )
	{
		HRESULT hr = S_OK;

		svrChk(Svr::ServerEntity::InitializeEntity( newEntityID ) );

		BR_ENTITY_MESSAGE( Message::Server::ServerConnectedC2SEvt)							{ svrMemReturn(pNewTrans = new Svr::GenericServerStartedTrans( pMsgData )); return S_OK; } );

	Proc_End:

		return hr;
	}







}; // namespace GameServer
}; // namespace BR



