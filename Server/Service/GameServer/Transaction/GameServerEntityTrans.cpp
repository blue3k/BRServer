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
#include "Server/BrService.h"
#include "SvrTrace.h"
#include "Entity/EntityManager.h"
#include "ServerEntity/ServerEntityManager.h"
#include "Transaction/MessageRoute.h"
#include "GameServerClass.h"
#include "Memory/MemoryPool.h"
#include "Types/SFEngineTypedefs.h"
#include "Protocol/Message/GameMsgClass.h"
#include "Protocol/Policy/GameNetPolicy.h"



#include "GameServerEntityTrans.h"
#include "GameInstance/GamePlayerEntity.h"
#include "ServiceEntity/Game/PlayerManagerServiceEntity.h"
#include "Protocol/Message/GameServerMsgClass.h"


SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::GameServerTransRegisterPlayerToJoinGameServer<BR::GameServer::PlayerManagerServiceEntity>);
SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::GameServerTransRegisterPlayerToJoinGameServer<BR::GameServer::GamePlayerEntity>);


namespace SF {
namespace GameServer {


	template<class ProcessEntity>
	GameServerTransRegisterPlayerToJoinGameServer<ProcessEntity>::GameServerTransRegisterPlayerToJoinGameServer(IHeap& heap, MessageDataPtr &pIMsg)
		: super(heap, pIMsg)
		, m_PublicAddress(nullptr)
		, m_PublicAddressIPV6(nullptr)
		, m_Port(0)
	{
		super::template RegisterMessageHandler<Message::GameServer::RegisterPlayerToJoinGameServerOnPlayerEntityRes>(__FILE__, __LINE__, [&](::SF::Svr::TransactionResult* pRes)->Result { return OnPlayerRegisteredRes(pRes); });
	}

	template<class ProcessEntity>
	Result GameServerTransRegisterPlayerToJoinGameServer<ProcessEntity>::OnPlayerRegisteredRes(Svr::TransactionResult* &pRes)
	{
		Result hr = ResultCode::SUCCESS;

		Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;
		Message::GameServer::RegisterPlayerToJoinGameServerOnPlayerEntityRes res;

		svrChk(pRes->GetResult());
		svrChk(res.ParseMessage(*pMsgRes->GetMessage()));


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

		m_PlayerUID = 0;

		svrChk( super::StartTransaction() );

		if( super::GetPlayerID() == 0 )
		{
			svrErr(ResultCode::INVALID_PLAYERID);
		}

		if( (Service::EntityTable->find(super::GetRouteContext().GetTo(), pEntity ))
			&& pEntity->GetEntityID().GetFacultyID() == (uint)EntityFaculty::User )
		{
			svrChkPtr( pPlayerEntity = dynamic_cast<GamePlayerEntity*>((Svr::Entity*)pEntity) );

			// If a login server has invalid login session information from the DB. the player ID will not be match
			if( pPlayerEntity->GetPlayerID() != super::GetPlayerID() )
			{
				super::CloseTransaction(ResultCode::INVALID_PLAYERID);
				goto Proc_End;
			}

			svrTrace(SVR_ENTITY, "Reinitialize Player Entity UID:{0}", super::GetPlayerID());
			Assert(pPlayerEntity->GetShardID() == super::GetShardID());
		}
		else
		{
			Svr::Entity* pEntity = nullptr;
			svrTrace(SVR_ENTITY, "Create new Player Entity UID:{0}", super::GetPlayerID());

			svrChk(Service::EntityManager->CreateEntity(ClusterID::Game, EntityFaculty::User, pEntity));
			svrChkPtr(pPlayerEntity = dynamic_cast<GamePlayerEntity*>(pEntity));
			svrChk(Service::EntityManager->AddEntity(EntityFaculty::User, pEntity));

			// Add Entity will Initialize entity so that AccountID is erased.
			// SetAccountID need to be set after entity is added
			pPlayerEntity->SetAccountID(super::GetPlayerID());
		}

		pPlayerEntity->SetShardID(super::GetShardID());
		m_PlayerUID = pPlayerEntity->GetEntityUID();

		m_PublicAddress = GetMyServer()->GetPublicNetConfig()->IPV4;
		m_PublicAddressIPV6 = GetMyServer()->GetPublicNetConfig()->IPV6;
		m_Port = GetMyServer()->GetPublicNetConfig()->Port;

		if ((Svr::Entity*)pPlayerEntity == (Svr::Entity*)super::GetMyOwner())
		{
			svrChk(pPlayerEntity->OnJoinGameServerInitialize(super::GetTicket(), super::GetFBUserID()));
			super::CloseTransaction(hr);
		}
		else
		{
			// it's local player send message to local loopback entity

			svrChk(Policy::NetPolicyGameServer(GetMyServer()->GetLoopbackServerEntity()->GetConnection()).RegisterPlayerToJoinGameServerOnPlayerEntityCmd(
				RouteContext(super::GetOwnerEntityUID(), m_PlayerUID), super::GetTransID(),
				super::GetPlayerID(), super::GetTicket(), super::GetFBUserID()));

		}

	Proc_End:

		if (!(hr))
			super::CloseTransaction(hr);

		return hr;
	}


	template class GameServerTransRegisterPlayerToJoinGameServer<Svr::PlayerManagerServiceEntity>;
	template class GameServerTransRegisterPlayerToJoinGameServer<GamePlayerEntity>;


};// namespace GameServer 
};// namespace SF 

