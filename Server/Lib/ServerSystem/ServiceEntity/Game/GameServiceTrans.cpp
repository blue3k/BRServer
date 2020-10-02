////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : game player entity implementation
//
////////////////////////////////////////////////////////////////////////////////


#include "ServerSystemPCH.h"
#include "ResultCode/SFResultCodeLibrary.h"
#include "ResultCode/SFResultCodeGame.h"
#include "Memory/SFMemoryPool.h"
#include "Types/SFEngineTypedefs.h"
#include "GameConst.h"

#include "Protocol/Message/GameInstanceManagerMsgClass.h"
#include "Protocol/Message/GameMsgClass.h"
#include "Protocol/Policy/GameNetPolicy.h"

#include "Net/SFNetServerUDP.h"
#include "Server/BrServerUtil.h"
#include "SvrTrace.h"
#include "Server/BrServer.h"
#include "ServerEntity/ServerEntityManager.h"
#include "Entity/EntityManager.h"
#include "ServiceEntity/Game/GameServiceTrans.h"
#include "ServiceEntity/Game/GameServiceEntity.h"






namespace SF {
namespace Svr {



	GamePartyTrans::GamePartyTrans(IHeap& heap, uint startMemberCount, uint targetMemberCount)
		: TransactionT(heap, TransactionID() )
	{
		//BR_TRANS_MESSAGE( TimerResult, { return OnTimer(pRes); });
		//BR_TRANS_MESSAGE( Message::PartyGameQueue::ReserveItemRes,			{ return OnReserveItem(pRes); });
		//BR_TRANS_MESSAGE( Message::PartyGameQueue::CancelReservationRes,	{ return OnCancelReservation(pRes); });
		//BR_TRANS_MESSAGE( Message::PartyGameQueue::DequeueItemRes,			{ return OnDequeueItem(pRes); });
		//BR_TRANS_MESSAGE( Message::GameInstanceManager::CreateGameRes,			{ return OnCreateGame(pRes); });
	}

	// Start Transaction
	Result GamePartyTrans::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk( super::StartTransaction() );


	Proc_End:

		if( !(hr) )
		{
			// We need to retry until it's successed
			CloseTransaction(hr);
		}

		return hr;
	}
	

	template<class ProcessEntity>
	GameServerTransRegisterPlayerToJoinGameServer<ProcessEntity>::GameServerTransRegisterPlayerToJoinGameServer(IHeap& heap, MessageDataPtr &pIMsg)
		: super(heap, pIMsg)
		, m_PublicAddress(nullptr)
		, m_PublicAddressIPV6(nullptr)
		, m_Port(0)
	{
		super::template RegisterMessageHandler<Message::GameServer::RegisterPlayerToJoinGameServerOnPlayerEntityRes>(&GameServerTransRegisterPlayerToJoinGameServer::OnPlayerRegisteredRes);
	}

	template<class ProcessEntity>
	Result GameServerTransRegisterPlayerToJoinGameServer<ProcessEntity>::OnPlayerRegisteredRes(Svr::TransactionResult* pRes)
	{
		FunctionContext hr([this](Result hr)
			{
				super::CloseTransaction(hr);
			});

		Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;
		Message::GameServer::RegisterPlayerToJoinGameServerOnPlayerEntityRes res;

		svrCheck(pRes->GetResult());
		svrCheck(res.ParseMessage(*pMsgRes->GetMessage()));

		return ResultCode::SUCCESS;
	}

	// Start Transaction
	template<class ProcessEntity>
	Result GameServerTransRegisterPlayerToJoinGameServer<ProcessEntity>::StartTransaction()
	{
		FunctionContext hr([this](Result hr)
			{
				if (!hr)
					super::CloseTransaction(hr);
			});
		SharedPointerT<Svr::Entity> pEntity;
		GamePlayerEntity *pPlayerEntity = nullptr;
		auto pMyOwner = super::GetMyOwner();

		m_PlayerUID = 0;

		svrCheck(super::StartTransaction());

		if (super::GetPlayerID() == 0)
		{
			svrError(ResultCode::INVALID_PLAYERID);
		}

		if ((Service::EntityTable->find(super::GetRouteContext().GetTo(), pEntity))
			&& pEntity->GetEntityID().GetFacultyID() == (uint)EntityFaculty::User)
		{
			svrCheckPtr(pPlayerEntity = dynamic_cast<GamePlayerEntity*>((Svr::Entity*)pEntity));

			// If a login server has invalid login session information from the DB. the player ID will not be match
			if (pPlayerEntity->GetPlayerID() != super::GetPlayerID())
			{
				super::CloseTransaction(ResultCode::INVALID_PLAYERID);
				return hr;
			}

			svrTrace(SVR_ENTITY, "Reinitialize Player Entity UID:{0}", super::GetPlayerID());
			Assert(pPlayerEntity->GetShardID() == super::GetShardID());
		}
		else
		{
			Svr::Entity* pEntity = nullptr;
			svrTrace(SVR_ENTITY, "Create new Player Entity UID:{0}", super::GetPlayerID());

			svrCheck(Service::EntityManager->CreateEntity(ClusterID::Game, EntityFaculty::User, pEntity));
			svrCheckPtr(pPlayerEntity = dynamic_cast<GamePlayerEntity*>(pEntity));
			svrCheck(Service::EntityManager->AddEntity(EntityFaculty::User, pEntity));

			// Add Entity will Initialize entity so that AccountID is erased.
			// SetAccountID need to be set after entity is added
			pPlayerEntity->SetAccountID(super::GetPlayerID());
			pPlayerEntity->SetServerNet(pMyOwner->GetServerNet());
			pPlayerEntity->SetPublicNetConfig(pMyOwner->GetPublicNetConfig());
		}

		pPlayerEntity->SetShardID(super::GetShardID());
		m_PlayerUID = pPlayerEntity->GetEntityUID();

		m_PublicAddress = pMyOwner->GetPublicNetConfig()->IPV4;
		m_PublicAddressIPV6 = pMyOwner->GetPublicNetConfig()->IPV6;
		m_Port = pMyOwner->GetPublicNetConfig()->Port;

		if ((Svr::Entity*)pPlayerEntity == (Svr::Entity*)pMyOwner)
		{
			svrCheck(pPlayerEntity->OnJoinGameServerInitialize(super::GetTicket(), super::GetFBUserID()));
			super::CloseTransaction(hr);
		}
		else
		{
			// it's local player send message to local loop back entity
			svrCheck(Policy::NetPolicyGameServer(BrServer::GetInstance()->GetLoopbackServerEntity()->GetConnection()).RegisterPlayerToJoinGameServerOnPlayerEntityCmd(
				RouteContext(super::GetOwnerEntityUID(), m_PlayerUID), super::GetTransID(),
				super::GetPlayerID(), super::GetTicket(), super::GetFBUserID()));

		}

		return hr;
	}


	template class GameServerTransRegisterPlayerToJoinGameServer<Svr::GameServiceEntity>;
	template class GameServerTransRegisterPlayerToJoinGameServer<GamePlayerEntity>;






	// Start Transaction
	Result PlayerTransRegisterPlayerToJoinGameServerOnPlayerEntity::StartTransaction()
	{
		FunctionContext hr([this](Result hr)
			{
				CloseTransaction(hr);
			});

		svrCheck(GetMyOwner()->OnJoinGameServerInitialize(GetTicket(), GetFBUserID()));

		return hr;
	}


};// namespace Svr 
};// namespace SF 

