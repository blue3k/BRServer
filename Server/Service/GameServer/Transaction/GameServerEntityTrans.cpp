////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves Corporation.
// 
// Author : KyungKun Ko
//
// Description : Entity server implementation
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "GameServer.h"
#include "Net/NetServerUDP.h"
#include "ServerSystem/BrService.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/EntityManager.h"
#include "ServerSystem/ServerEntityManager.h"
#include "ServerSystem/MessageRoute.h"
#include "GameServerClass.h"
#include "Common/MemoryPool.h"
#include "Common/BrBaseTypes.h"
#include "Protocol/Message/GameMsgClass.h"
#include "Protocol/Policy/GameIPolicy.h"

#include "ServerSystem/ServiceEntity/ClusterManagerServiceEntity.h"

#include "GameServerEntityTrans.h"
#include "GameInstance/GamePlayerEntity.h"
#include "GameInstance/GameEntityManager.h"
#include "Protocol/Message/GameServerMsgClass.h"


BR_MEMORYPOOL_IMPLEMENT(GameServer::GameServerTransRegisterPlayerToJoinGameServer);


namespace BR {
namespace GameServer {



	GameServerTransRegisterPlayerToJoinGameServer::GameServerTransRegisterPlayerToJoinGameServer(Message::MessageData* &pIMsg)
		: ServerEntityMessageTransaction(pIMsg)
	{
		SetWorkOnServerEntity(true);

		BR_TRANS_MESSAGE(Message::GameServer::RegisterPlayerToJoinGameServerOnPlayerEntityRes, { return OnPlayerRegisteredRes(pRes); });
	}

	HRESULT GameServerTransRegisterPlayerToJoinGameServer::OnPlayerRegisteredRes(Svr::TransactionResult* &pRes)
	{
		HRESULT hr = S_SYSTEM_OK;

		Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;
		Message::GameServer::RegisterPlayerToJoinGameServerOnPlayerEntityRes res;

		svrChk(pRes->GetHRESULT());
		svrChk(res.ParseIMsg(pMsgRes->GetMessage()));

		m_PublicAddress = GetMyServer()->GetNetPublic()->GetLocalAddress();
		m_PublicAddressIPV4 = GetMyServer()->GetPublicNetAddressIPv4();

	Proc_End:

		CloseTransaction(hr);

		return S_SYSTEM_OK;
	}

	// Start Transaction
	HRESULT GameServerTransRegisterPlayerToJoinGameServer::StartTransaction()
	{
		HRESULT hr = S_SYSTEM_OK;
		SharedPointerT<Svr::Entity> pEntity;
		GamePlayerEntity *pPlayerEntity = nullptr;
		Policy::IPolicyGameServer *pTargetPolicy = nullptr;

		m_PlayerUID = 0;

		svrChk( super::StartTransaction() );

		if( GetPlayerID() == 0 )
		{
			svrErr(E_INVALID_PLAYERID);
		}

		if( SUCCEEDED(Svr::GetServerComponent<Svr::EntityManager>()->FindEntity( GetRouteContext().GetTo(), pEntity ))
			&& pEntity->GetEntityID().GetFacultyID() == (UINT)EntityFaculty::User )
		{
			svrChkPtr( pPlayerEntity = BR_DYNAMIC_CAST(GamePlayerEntity*,(Svr::Entity*)pEntity) );

			// If a login server has invalid login session information from the DB. the player ID will not be match
			if( pPlayerEntity->GetPlayerID() != GetPlayerID() )
			{
				svrErrClose(E_INVALID_PLAYERID);
			}

			svrTrace(Svr::TRC_ENTITY, "Reinitialize Player Entity UID:{0}", GetPlayerID());
			Assert(pPlayerEntity->GetShardID() == GetShardID());
		}
		else
		{
			svrTrace(Svr::TRC_ENTITY, "Create new Player Entity UID:{0}", GetPlayerID());
			svrChk(Svr::GetServerComponent<GameEntityManager>()->CreateGamePlayer(GetPlayerID(), pPlayerEntity));

		}

		pPlayerEntity->SetShardID(GetShardID());
		m_PlayerUID = pPlayerEntity->GetEntityUID();

		// it's local player send message to local loopback entity
		svrChkPtr(pTargetPolicy = GetMyServer()->GetLoopbackServerEntity()->GetPolicy<Policy::IPolicyGameServer>());

		svrChk(pTargetPolicy->RegisterPlayerToJoinGameServerOnPlayerEntityCmd(GetTransID(), 
			RouteContext(GetOwnerEntityUID(),m_PlayerUID), 
			GetPlayerID(), GetTicket(), GetFBUserID() ));


	Proc_End:

		if (FAILED(hr))
			CloseTransaction(hr);

		return hr;
	}




};// namespace GameServer 
};// namespace BR 

