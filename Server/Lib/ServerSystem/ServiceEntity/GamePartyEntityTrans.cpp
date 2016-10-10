////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : game player entity implementation
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"

#include "Common/ResultCode/BRResultCodeCommon.h"
#include "Common/ResultCode/BRResultCodeGame.h"
#include "Common/ResultCode/BRResultCodeSvr.h"

#include "Common/MemoryPool.h"
#include "Common/BrSvrTypes.h"
#include "Common/GameConst.h"

#include "ServerSystem/BrServerUtil.h"
#include "ServerSystem/SvrTrace.h"
#include "Protocol/Message/GameServerMsgClass.h"
#include "Protocol/Policy/GameServerIPolicy.h"
#include "Protocol/Message/GamePartyMsgClass.h"
#include "Protocol/Policy/GamePartyIPolicy.h"

#include "Protocol/Policy/PartyMatchingIPolicy.h"
#include "Protocol/Message/PartyMatchingQueueMsgClass.h"
#include "Protocol/Policy/PartyMatchingQueueIPolicy.h"
#include "ServerSystem/ServiceEntity/MatchingServiceUtil.h"
#include "ServerSystem/ServerService/PartyMatchingQueueService.h"
#include "ServerSystem/ServiceEntity/MatchingQueueServiceEntity.h"

#include "Protocol/Message/GameInstanceManagerMsgClass.h"
#include "ServerSystem/ServiceEntity/GamePartyManagerServiceEntity.h"
#include "ServerSystem/ServerService/GamePartyManagerService.h"
#include "ServerSystem/ServiceEntity/Game/GameInstanceManagerServiceEntity.h"
#include "ServerSystem/ServerService/GameInstanceManagerService.h"

#include "ServerSystem/BrServer.h"

#include "ServerSystem/EntityManager.h"
#include "ServerSystem/ServerEntityManager.h"
#include "ServerSystem/ServiceEntity/GamePartyEntity.h"
#include "ServerSystem/ServiceEntity/GamePartyEntityTrans.h"

//#include "DB/GameDB.h"
#include "DB/AccountDB.h"
#include "DB/AccountQuery.h"



BR_MEMORYPOOL_IMPLEMENT(Svr::PartyTransCloseInstance);
//BR_MEMORYPOOL_IMPLEMENT(Svr::PartyTransCreateParty);
BR_MEMORYPOOL_IMPLEMENT(Svr::PartyTransJoinParty);
BR_MEMORYPOOL_IMPLEMENT(Svr::PartyTransLeaveParty);
BR_MEMORYPOOL_IMPLEMENT(Svr::PartyTransKickPlayer);
BR_MEMORYPOOL_IMPLEMENT(Svr::PartyTransChatMessage);
BR_MEMORYPOOL_IMPLEMENT(Svr::PartyTransQuickChatMessage);
BR_MEMORYPOOL_IMPLEMENT(Svr::PartyTransStartGameMatchCmd);
BR_MEMORYPOOL_IMPLEMENT(Svr::PartyTransCancelGameMatchCmd);
BR_MEMORYPOOL_IMPLEMENT(Svr::PartyTransPartyMatchingCanceled);
BR_MEMORYPOOL_IMPLEMENT(Svr::PartyTransMatchingItemDequeued);
BR_MEMORYPOOL_IMPLEMENT(Svr::PartyTransPartyGameMatchedS2CEvt);



namespace BR {
namespace Svr {


	Result PartyTransCloseInstance::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		GamePartyEntity* pOwner = (GamePartyEntity*)GetOwnerEntity();
		ServerServiceInformation *pService = nullptr;

		svrChk(Transaction::StartTransaction() );


		svrChk(GetServerComponent<GamePartyManagerServiceEntity>()->GetService(pService));

		// 2. Get service entity list in the cluster
		svrChk(pService->GetService<Svr::GamePartyManagerService>()->PartyDeletedC2SEvt(pOwner->GetEntityUID(), 0));


		Svr::GetServerComponent<EntityManager>()->RemoveEntity( pOwner );

	Proc_End:

		return hr;
	}


	

	// Start Transaction
	Result PartyTransJoinParty::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		PartyPlayer *pPlayer = nullptr;

		m_LeaderID = 0;

		svrChk( super::StartTransaction() );


		if( GetInviterID() == 0 )
		{
			// This should be rejoin by relogging
			Policy::ISvrPolicyGameParty *pPolicy = nullptr;

			svrChk( GetMyOwner()->FindPlayer( GetInvitedPlayer().PlayerID, pPlayer ) );
			svrChk( pPlayer->SetServerEntity( GetServerEntity<Svr::ServerEntity>(), GetRouteContext().GetFrom()) );

			pPolicy = pPlayer->GetPolicy<Policy::ISvrPolicyGameParty>();

			// Send others to joined
			GetMyOwner()->ForeachPlayer( [&]( PartyPlayer* pOtherPlayer )->Result {
				if( pPlayer != pOtherPlayer )
				{
					pPolicy->PlayerJoinedS2CEvt( pPlayer->GetRouteContext(GetOwnerEntityUID()), pOtherPlayer->GetPlayerInformation() );
				}
				return ResultCode::SUCCESS;
			});


			pPlayer = nullptr;
		}
		else
		{
			// New member can't be joined during matching
			if( GetMyOwner()->GetMatchingTicket() != 0 )
				svrErrClose(ResultCode::E_SVR_ALREADY_INQUEUE);

			svrMem( pPlayer = new PartyPlayer( GetInvitedPlayer() ) );
			svrChk( pPlayer->SetServerEntity( GetServerEntity<ServerEntity>(), GetRouteContext().GetFrom()) );
			svrChk( GetMyOwner()->JoinPlayer( pPlayer, false ) );
		}

		svrChk( GetMyOwner()->GetChatHistory().GetGameLogBinary( m_MessageBuffer, 0, GameConst::MAX_CHATSYNC ) );

		m_LeaderID = GetMyOwner()->GetLeaderID();

	Proc_End:

		Util::SafeDelete( pPlayer );
		CloseTransaction( hr );

		return hr;
	}


	// Start Transaction
	Result PartyTransLeaveParty::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		PartyPlayer *pPlayer = nullptr;

		svrChk( super::StartTransaction() );

		// Any party operation is prohebited during matching
		if( GetMyOwner()->GetMatchingTicket() != 0 )
			svrErrClose(ResultCode::E_SVR_ALREADY_INQUEUE);

		svrChk( GetMyOwner()->FindPlayer( GetPlayerID(), pPlayer ) );
		svrChk( GetMyOwner()->LeavePlayer( pPlayer, false ) );

	Proc_End:

		Util::SafeDelete( pPlayer );
		CloseTransaction( hr );

		return hr;
	}


	// Start Transaction
	Result PartyTransKickPlayer::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		PartyPlayer *pPlayer = nullptr;

		svrChk( super::StartTransaction() );

		// Any party operation is prohebited during matching
		if( GetMyOwner()->GetMatchingTicket() != 0 )
			svrErrClose(ResultCode::E_SVR_ALREADY_INQUEUE);

		svrChkClose( GetMyOwner()->FindPlayer( GetPlayerToKick(), pPlayer ) );

		GetMyOwner()->ForeachPlayerSvrGameParty( [&]( PartyPlayer* pOtherPlayer, Policy::ISvrPolicyGameParty *pPolicy )->Result {
			pPolicy->PlayerKickedS2CEvt( pOtherPlayer->GetRouteContext(GetOwnerEntityUID()), GetPlayerToKick() );
			return ResultCode::SUCCESS;
		});

		svrChk( GetMyOwner()->LeavePlayer( pPlayer, true ) );

	Proc_End:

		Util::SafeDelete( pPlayer );
		CloseTransaction( hr );

		return hr;
	}
	

	// Start Transaction
	Result PartyTransChatMessage::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		PartyPlayer *pPlayer = nullptr;

		svrChk( super::StartTransaction() );

		svrChk( GetMyOwner()->FindPlayer( GetPlayerID(), pPlayer ) );

		svrChk( GetMyOwner()->GetChatHistory().AddChattingLog( Util::Time.GetTimeUTCSec(), pPlayer->GetPlayerID(), 0, ChatType::Normal, GetChatMessage() ) );

		GetMyOwner()->ForeachPlayerSvrGameParty( [&]( PartyPlayer* pOtherPlayer, Policy::ISvrPolicyGameParty *pPolicy )->Result {
				pPolicy->ChatMessageS2CEvt( pOtherPlayer->GetRouteContext(GetOwnerEntityUID()), GetPlayerID(), pPlayer->GetPlayerName(), GetChatMessage() );
				return ResultCode::SUCCESS;
			});

	Proc_End:

		CloseTransaction( hr );

		return hr;
	}
	

	// Start Transaction
	Result PartyTransQuickChatMessage::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		PartyPlayer *pPlayer = nullptr;

		svrChk( super::StartTransaction() );

		svrChk( GetMyOwner()->FindPlayer( GetPlayerID(), pPlayer ) );

		GetMyOwner()->ForeachPlayerSvrGameParty( [&]( PartyPlayer* pOtherPlayer, Policy::ISvrPolicyGameParty *pPolicy )->Result {
				pPolicy->QuickChatMessageS2CEvt( pOtherPlayer->GetRouteContext(GetOwnerEntityUID()), GetPlayerID(), GetQuickChatID() );
				return ResultCode::SUCCESS;
			});

	Proc_End:

		CloseTransaction( hr );

		return hr;
	}
	

	PartyTransStartGameMatchCmd::PartyTransStartGameMatchCmd(Message::MessageData* &pIMsg )
		:GamePartyMessageTransaction( pIMsg )
	{
		SetExclusive(true);
		BR_TRANS_MESSAGE( Message::PartyMatchingQueue::RegisterPartyMatchingRes, { return OnPartyMatchingQueued(pRes); } );
		BR_TRANS_MESSAGE( Message::GameInstanceManager::CreateGameRes,			{ return OnCreateGame(pRes); });
	}

	Result PartyTransStartGameMatchCmd::OnPartyMatchingQueued( Svr::TransactionResult* &pRes )
	{
		Result hr = ResultCode::SUCCESS;

		Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;
		Message::PartyMatchingQueue::RegisterPartyMatchingRes res;

		svrChk(pRes->GetResult());
		svrChk( res.ParseIMsg( pMsgRes->GetMessage() ) );

		GetMyOwner()->SetMatchingTicket( res.GetMatchingTicket() );
		

		GetMyOwner()->ForeachPlayerSvrGameParty( [&]( PartyPlayer* pOtherPlayer, Policy::ISvrPolicyGameParty *pPolicy )->Result {
			pPolicy->QueuedGameMatchingS2CEvt( pOtherPlayer->GetRouteContext(GetOwnerEntityUID()), res.GetMatchingTicket() );
			return ResultCode::SUCCESS;
		});


	Proc_End:

		CloseTransaction(hr);

		return hr; 
	}
	
	Result PartyTransStartGameMatchCmd::OnCreateGame(TransactionResult* pRes)
	{
		Result hr = ResultCode::SUCCESS;
		Message::GameInstanceManager::CreateGameRes msgRes;
		GameInsUID gameUID;

		svrChk(pRes->GetResult());

		svrChk( msgRes.ParseIMsg( ((MessageResult*)pRes)->GetMessage() ) );

		gameUID = msgRes.GetRouteContext().GetFrom();

		// early close for packet order
		CloseTransaction(hr);

		GetMyOwner()->ForeachPlayer( [&]( PartyPlayer* pPlayer )->Result {
			if( pPlayer->GetServerEntity() == nullptr ) return ResultCode::SUCCESS;

			Policy::ISvrPolicyPartyMatching *pPolicy = pPlayer->GetServerEntity()->GetPolicy<Policy::ISvrPolicyPartyMatching>();
			if( pPolicy == nullptr ) return ResultCode::SUCCESS;

			pPolicy->PlayerGameMatchedS2CEvt( RouteContext(GetOwnerEntityUID(), pPlayer->GetPlayerEntityUID()), 0, pPlayer->GetPlayerID(), gameUID, PlayerRole::None );

			return ResultCode::SUCCESS;
		});

	Proc_End:

		return hr;
	}

	Result PartyTransStartGameMatchCmd::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		Svr::ServerServiceInformation *pService = nullptr;
		StaticArray<MatchingPlayerInformation,GameConst::MAX_GAMEPLAYER> m_matchingPlayers;

		svrChk( super::StartTransaction() );

		if( GetMyOwner()->GetMatchingTicket() != 0 )
			svrErrClose(ResultCode::E_SVR_ALREADY_INQUEUE);

		if( GetMaxGamePlayers() < GetMyOwner()->GetNumPlayer() )
		{
			svrErr(ResultCode::E_INVALID_PLAYER_COUNT);
		}
		else if( GetMaxGamePlayers() == GetMyOwner()->GetNumPlayer() )
		{
			// We already have full memeber, just create game and let them know to join
			ServerServiceInformation *pService = nullptr;

			svrChk( GetServerComponent<GameInstanceManagerWatcherServiceEntity>()->GetService( pService ) );

			// 2. Get service entity list in the cluster
			svrChk( pService->GetService<GameInstanceManagerService>()->CreateGameCmd(GetTransID(), 0, 0, GetMaxGamePlayers()) );

			//CloseTransaction(hr);
			goto Proc_End;
		}

		{
			auto matchingCompID = Svr::MatchingUtil::GetQueueComponentID(GetMaxGamePlayers(), GetMyOwner()->GetNumPlayer(), PlayerRole::None);
			auto matchingQueueService = Svr::GetServerComponent<Svr::RingClusterServiceEntity>(matchingCompID);
			if (matchingQueueService == nullptr)
			{
				svrTrace(Trace::TRC_ERROR, "Failed to get matching queue service MaxGamePlayer:{0}, NumberOfPlayer:{1}, matchingCompID:{2}", GetMaxGamePlayers(), GetMyOwner()->GetNumPlayer(), matchingCompID);
				svrErr(ResultCode::E_SVR_INVALID_CLUSTERID);
			}
			else
			{
				svrChk(matchingQueueService->GetService(pService));
			}

			GetMyOwner()->ForeachPlayer([&](PartyPlayer* pPlayer)->Result {
				m_matchingPlayers.push_back(MatchingPlayerInformation(pPlayer->GetPlayerEntityUID(), pPlayer->GetPlayerID(), PlayerRole::None));
				return ResultCode::SUCCESS;
			});

			svrChk(pService->GetService<Svr::PartyMatchingQueueService>()->RegisterPartyMatchingCmd(GetTransID(), 0, m_matchingPlayers));
		}

		
	Proc_End:

		if( FAILED(hr) )
			CloseTransaction( hr );

		return hr;
	}
	



	PartyTransCancelGameMatchCmd::PartyTransCancelGameMatchCmd(Message::MessageData* &pIMsg )
		:GamePartyMessageTransaction( pIMsg )
	{
		SetExclusive(true);
		BR_TRANS_MESSAGE( Message::PartyMatchingQueue::UnregisterMatchingRes, { return OnPartyMatchingCanceled(pRes); } );
	}
	
	Result PartyTransCancelGameMatchCmd::OnPartyMatchingCanceled( Svr::TransactionResult* &pRes )
	{
		Result hr = ResultCode::SUCCESS;

		Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;
		Message::PartyMatchingQueue::UnregisterMatchingRes res;

		switch( (int32_t)pRes->GetResult() )
		{
		case ResultCode::E_SVR_RESERVED_QUEUEITEM:
			svrErrClose(ResultCode::E_SVR_RESERVED_QUEUEITEM);
			break;
		case ResultCode::E_SVR_QUEUEITEM_CANCELED:
		case ResultCode::E_SVR_INVALID_QUEUEITEM:
			GetMyOwner()->ForeachPlayerSvrGameParty( [&]( PartyPlayer* pOtherPlayer, Policy::ISvrPolicyGameParty *pPolicy )->Result {
				pPolicy->CanceledGameMatchingS2CEvt( pOtherPlayer->GetRouteContext(GetOwnerEntityUID()), GetMyOwner()->GetMatchingTicket() );
				return ResultCode::SUCCESS;
			});
			GetMyOwner()->SetMatchingTicket( 0 );
			break;
		default:
			svrChk(pRes->GetResult());
			svrChk( res.ParseIMsg( pMsgRes->GetMessage() ) );
			GetMyOwner()->ForeachPlayerSvrGameParty( [&]( PartyPlayer* pOtherPlayer, Policy::ISvrPolicyGameParty *pPolicy )->Result {
				pPolicy->CanceledGameMatchingS2CEvt( pOtherPlayer->GetRouteContext(GetOwnerEntityUID()), GetMyOwner()->GetMatchingTicket() );
				return ResultCode::SUCCESS;
			});
			GetMyOwner()->SetMatchingTicket( 0 );
			break;
		};


	Proc_End:

		CloseTransaction(hr);

		return hr; 
	}

	Result PartyTransCancelGameMatchCmd::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		ServerEntity *pServer = nullptr;

		svrChk( super::StartTransaction() );

		if( GetMyOwner()->GetMatchingTicket() == 0 )
		{
			svrErrClose(ResultCode::E_SVR_INVALID_QUEUEITEM);
		}


		svrChk( Svr::GetServerComponent<Svr::ServerEntityManager>()->GetServerEntity( GetMyOwner()->GetMatchingTicket().QueueUID.GetServerID(), pServer ) );

		svrChk( pServer->GetPolicy<Policy::IPolicyPartyMatchingQueue>()->UnregisterMatchingCmd( RouteContext(GetOwnerEntityUID(), GetMyOwner()->GetMatchingTicket().QueueUID), GetTransID(), 0,
			GetMyOwner()->GetMatchingTicket() ) );

		
	Proc_End:

		CloseTransaction( hr );

		return hr;
	}
	

	
	Result PartyTransPartyMatchingCanceled::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk( super::StartTransaction() );

		if( GetMyOwner()->GetMatchingTicket() != 0 )
		{
			svrErrClose(ResultCode::E_SVR_INVALID_QUEUEITEM);
		}

		GetMyOwner()->ForeachPlayerSvrGameParty( [&]( PartyPlayer* pOtherPlayer, Policy::ISvrPolicyGameParty *pPolicy )->Result {
			pPolicy->CanceledGameMatchingS2CEvt( pOtherPlayer->GetRouteContext(GetOwnerEntityUID()), GetMyOwner()->GetMatchingTicket() );
			return ResultCode::SUCCESS;
		});

		GetMyOwner()->SetMatchingTicket( 0 );
		
	Proc_End:

		CloseTransaction( hr );

		return hr;
	}
	

	
	Result PartyTransMatchingItemDequeued::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk( super::StartTransaction() );

		if( GetMyOwner()->GetMatchingTicket() == 0 )
		{
			svrErrClose(ResultCode::E_SVR_INVALID_QUEUEITEM);
		}

		GetMyOwner()->ForeachPlayerSvrGameParty( [&]( PartyPlayer* pOtherPlayer, Policy::ISvrPolicyGameParty *pPolicy )->Result {
			pPolicy->MatchingItemDequeuedS2CEvt( pOtherPlayer->GetRouteContext(GetOwnerEntityUID()), GetMyOwner()->GetMatchingTicket() );
			return ResultCode::SUCCESS;
		});

		GetMyOwner()->SetMatchingTicket( 0 );
		
	Proc_End:

		CloseTransaction( hr );

		return hr;
	}
	
	
	Result PartyTransPartyGameMatchedS2CEvt::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk( super::StartTransaction() );

		GetMyOwner()->SetMatchingTicket( 0 );
		
	Proc_End:

		CloseTransaction( hr );

		return hr;
	}
	



};// namespace Svr 
};// namespace BR 

