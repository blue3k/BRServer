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
#include "GameInstance/GameClusterServiceEntity.h"
#include "GameInstance/GameEntityManager.h"
#include "Protocol/Message/GameServerMsgClass.h"


BR_MEMORYPOOL_IMPLEMENT(GameServer::GameServerTransRegisterPlayerToJoinGameServer<BR::GameServer::GameClusterServiceEntity>);
BR_MEMORYPOOL_IMPLEMENT(GameServer::GameServerTransRegisterPlayerToJoinGameServer<BR::GameServer::GamePlayerEntity>);


namespace BR {
namespace GameServer {


	template<class ProcessEntity>
	GameServerTransRegisterPlayerToJoinGameServer<ProcessEntity>::GameServerTransRegisterPlayerToJoinGameServer(Message::MessageData* &pIMsg)
		: super(pIMsg)
		, m_PublicAddress(nullptr)
		, m_PublicAddressIPV6(nullptr)
		, m_Port(0)
	{
		super::template RegisterMessageHandler<Message::GameServer::RegisterPlayerToJoinGameServerOnPlayerEntityRes>(__FILE__, __LINE__, [&](::BR::Svr::TransactionResult* pRes)->Result { return OnPlayerRegisteredRes(pRes); });
	}

	template<class ProcessEntity>
	Result GameServerTransRegisterPlayerToJoinGameServer<ProcessEntity>::OnPlayerRegisteredRes(Svr::TransactionResult* &pRes)
	{
		Result hr = ResultCode::SUCCESS;

		Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;
		Message::GameServer::RegisterPlayerToJoinGameServerOnPlayerEntityRes res;

		svrChk(pRes->GetResult());
		svrChk(res.ParseIMsg(pMsgRes->GetMessage()));


	Proc_End:

		super::CloseTransaction(hr);

		return ResultCode::SUCCESS;
	}

	// Start Transaction
	template<class ProcessEntity>
	Result GameServerTransRegisterPlayerToJoinGameServer<ProcessEntity>::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		SharedPointerT<Svr::Entity> pEntity;
		GamePlayerEntity *pPlayerEntity = nullptr;
		Policy::IPolicyGameServer *pTargetPolicy = nullptr;

		m_PlayerUID = 0;

		svrChk( super::StartTransaction() );

		if( super::GetPlayerID() == 0 )
		{
			svrErr(ResultCode::E_INVALID_PLAYERID);
		}

		if( SUCCEEDED(Svr::GetServerComponent<Svr::EntityManager>()->FindEntity(super::GetRouteContext().GetTo(), pEntity ))
			&& pEntity->GetEntityID().GetFacultyID() == (UINT)EntityFaculty::User )
		{
			svrChkPtr( pPlayerEntity = BR_DYNAMIC_CAST(GamePlayerEntity*,(Svr::Entity*)pEntity) );

			// If a login server has invalid login session information from the DB. the player ID will not be match
			if( pPlayerEntity->GetPlayerID() != super::GetPlayerID() )
			{
				super::CloseTransaction(ResultCode::E_INVALID_PLAYERID);
				goto Proc_End;
			}

			svrTrace(Svr::TRC_ENTITY, "Reinitialize Player Entity UID:{0}", super::GetPlayerID());
			Assert(pPlayerEntity->GetShardID() == super::GetShardID());
		}
		else
		{
			Svr::Entity* pEntity = nullptr;
			svrTrace(Svr::TRC_ENTITY, "Create new Player Entity UID:{0}", super::GetPlayerID());

			svrChk(Svr::GetServerComponent<GameEntityManager>()->CreateEntity(ClusterID::Game_Conspiracy, EntityFaculty::User, pEntity));
			svrChkPtr(pPlayerEntity = dynamic_cast<GamePlayerEntity*>(pEntity));
			svrChk(Svr::GetServerComponent<GameEntityManager>()->AddEntity(EntityFaculty::User, pEntity));

			// Add Entity will Initialize entity so that AccountID is erased.
			// SetAccountID need to be set after entity is added
			pPlayerEntity->SetAccountID(super::GetPlayerID());
		}

		pPlayerEntity->SetShardID(super::GetShardID());
		m_PlayerUID = pPlayerEntity->GetEntityUID();

		m_PublicAddress = GetMyServer()->GetPublicNetConfig()->IPV4.c_str();
		m_PublicAddressIPV6 = GetMyServer()->GetPublicNetConfig()->IPV6.c_str();
		m_Port = GetMyServer()->GetPublicNetConfig()->Port;

		if ((Svr::Entity*)pPlayerEntity == (Svr::Entity*)super::GetMyOwner())
		{
			svrChk(pPlayerEntity->OnJoinGameServerInitialize(super::GetTicket(), super::GetFBUserID()));
			super::CloseTransaction(hr);
		}
		else
		{
			// it's local player send message to local loopback entity
			svrChkPtr(pTargetPolicy = GetMyServer()->GetLoopbackServerEntity()->GetPolicy<Policy::IPolicyGameServer>());

			svrChk(pTargetPolicy->RegisterPlayerToJoinGameServerOnPlayerEntityCmd(
				RouteContext(super::GetOwnerEntityUID(), m_PlayerUID), super::GetTransID(),
				super::GetPlayerID(), super::GetTicket(), super::GetFBUserID()));

		}

	Proc_End:

		if (FAILED(hr))
			super::CloseTransaction(hr);

		return hr;
	}


	template class GameServerTransRegisterPlayerToJoinGameServer<GameClusterServiceEntity>;
	template class GameServerTransRegisterPlayerToJoinGameServer<GamePlayerEntity>;


};// namespace GameServer 
};// namespace BR 

