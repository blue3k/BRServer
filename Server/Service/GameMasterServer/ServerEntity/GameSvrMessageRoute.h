////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Game Master server message route
//
////////////////////////////////////////////////////////////////////////////////


#pragma once


#include "ServerSystem/MessageRoute.h"
#include "ServerSystem/ServerEntity.h"
#include "PlayerInfoManager.h"


namespace BR {
namespace GameMasterServer {


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Utilities for route message
	//
	
	// Route Message utility func
	template< class MsgClass >
	HRESULT RouteGameServerMessage( Message::MessageData* &pMsg )
	{
		HRESULT hr = S_OK;
		Svr::ServerEntity* pServerEntity = nullptr;
		PlayerInfoManager::PlayerIDMap::iterator itPlayer;
		MsgClass msg;
		RouteContext ctxRoute;

		svrChk( msg.ParseIMsg(pMsg) );

		__if_exists(MsgClass::GetDestUID)
		{
			svrChk( PlayerInfoManager::FindPlayerInfoByID(msg.GetDestUID().PID, itPlayer) );
		}

		__if_exists(MsgClass::GetDestID)
		{
			svrChk( PlayerInfoManager::FindPlayerInfoByID(msg.GetDestID(), itPlayer) );
		}

		ctxRoute = msg.GetRouteContext();
		ctxRoute.To = itPlayer->GetServerID();

		svrChkErr( E_SVR_ROUTEMSG_FAILED, Svr::GetServerEntityManager()->GetServerEntity( msg.GetRouteContext().To, pServerEntity ) );

		svrChkPtr( pServerEntity->GetConnection() );

		svrChk( pServerEntity->GetConnection()->Send( pMsg ) );

	Proc_End:

		Util::SafeRelease( pMsg );

		return hr;
	}



}; // namespace GameMasterServer
}; // namespace BR





