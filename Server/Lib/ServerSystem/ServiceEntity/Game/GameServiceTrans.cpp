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



SF_MEMORYPOOL_IMPLEMENT(SF::Svr::GamePartyTrans);



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

		svrChk(super::StartTransaction());

		if (super::GetPlayerID() == 0)
		{
			svrErr(ResultCode::INVALID_PLAYERID);
		}

		if ((Service::EntityTable->find(super::GetRouteContext().GetTo(), pEntity))
			&& pEntity->GetEntityID().GetFacultyID() == (uint)EntityFaculty::User)
		{
			svrChkPtr(pPlayerEntity = dynamic_cast<GamePlayerEntity*>((Svr::Entity*)pEntity));

			// If a login server has invalid login session information from the DB. the player ID will not be match
			if (pPlayerEntity->GetPlayerID() != super::GetPlayerID())
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
			pPlayerEntity->SetServerNet(GetMyOwner()->GetServerNet());
			pPlayerEntity->SetPublicNetConfig(GetMyOwner()->GetPublicNetConfig());
		}

		pPlayerEntity->SetShardID(super::GetShardID());
		m_PlayerUID = pPlayerEntity->GetEntityUID();

		m_PublicAddress = GetMyOwner()->GetPublicNetConfig()->IPV4;
		m_PublicAddressIPV6 = GetMyOwner()->GetPublicNetConfig()->IPV6;
		m_Port = GetMyOwner()->GetPublicNetConfig()->Port;

		if ((Svr::Entity*)pPlayerEntity == (Svr::Entity*)super::GetMyOwner())
		{
			svrChk(pPlayerEntity->OnJoinGameServerInitialize(super::GetTicket(), super::GetFBUserID()));
			super::CloseTransaction(hr);
		}
		else
		{
			// it's local player send message to local loop back entity
			svrChk(Policy::NetPolicyGameServer(BrServer::GetInstance()->GetLoopbackServerEntity()->GetConnection()).RegisterPlayerToJoinGameServerOnPlayerEntityCmd(
				RouteContext(super::GetOwnerEntityUID(), m_PlayerUID), super::GetTransID(),
				super::GetPlayerID(), super::GetTicket(), super::GetFBUserID()));

		}

	Proc_End:

		if (!(hr))
			super::CloseTransaction(hr);

		return hr;
	}


	template class GameServerTransRegisterPlayerToJoinGameServer<Svr::GameServiceEntity>;
	template class GameServerTransRegisterPlayerToJoinGameServer<GamePlayerEntity>;






	// Start Transaction
	Result PlayerTransRegisterPlayerToJoinGameServerOnPlayerEntity::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(GetMyOwner()->OnJoinGameServerInitialize(GetTicket(), GetFBUserID()));

	Proc_End:

		CloseTransaction(hr);

		return hr;
	}


};// namespace Svr 
};// namespace SF 

