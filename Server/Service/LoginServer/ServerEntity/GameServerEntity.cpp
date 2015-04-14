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
#include "GameServerEntity.h"
#include "LoginSvrConst.h"

#include "Protocol/Policy/EntityServerIPolicy.h"
#include "Protocol/Message/LoginServerMsgClass.h"
#include "Protocol/Message/ServerMsgClass.h"

#include "ServerEntity/ServerEntityTrans.h"
#include "Entity/LoginPlayerEntityTrans.h"


namespace BR {
namespace LoginServer {

	
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Entity server entity
	//


	GameServerEntity::GameServerEntity()
		:Svr::ServerEntity( Const::ENTITY_ENTITY_TRANS_QUEUE, Const::ENTITY_ENTITY_TRANSRES_QUEUE )
		,m_Allocator(STDAllocator::GetInstance())
	{
	}

	GameServerEntity::~GameServerEntity()
	{
	}
			

	
	// Initialize entity to proceed new connection
	HRESULT GameServerEntity::InitializeEntity( EntityID newEntityID )
	{
		HRESULT hr = S_OK;

		svrChk( __super::InitializeEntity( newEntityID ) );

		BR_ENTITY_MESSAGE( Message::Server::ServerConnectedC2SEvt)							{ svrMemReturn(pNewTrans = new TransGameServerStarted( pMsgData )); return S_OK; } );
		BR_ENTITY_MESSAGE(Message::LoginServer::PlayerJoinedToGameServerCmd)				{ svrMemReturn(pNewTrans = new LoginPlayerJoinedToGameServerTrans(pMsgData)); return S_OK; } );


	Proc_End:

		return hr;
	}







}; // namespace LoginServer
}; // namespace BR



