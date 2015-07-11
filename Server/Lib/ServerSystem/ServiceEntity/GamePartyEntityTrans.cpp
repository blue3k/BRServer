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

#include "Common/HRESCommon.h"
#include "Common/HRESGame.h"
#include "Common/HRESSvrSys.h"
#include "Common/HRESLogin.h"
#include "Common/MemoryPool.h"
#include "Common/BRSvrTypes.h"
#include "Common/GameConst.h"

#include "ServerSystem/BRServerUtil.h"
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
#include "ServerSystem/ServiceEntity/GameInstanceManagerServiceEntity.h"
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


	HRESULT PartyTransCloseInstance::StartTransaction()
	{
		HRESULT hr = S_OK;
		GamePartyEntity* pOwner = (GamePartyEntity*)GetOwnerEntity();
		ServerServiceInformation *pService = nullptr;

		svrChk( __super::StartTransaction() );


		svrChk(GetServerComponent<GamePartyManagerServiceEntity>()->GetService(pService));

		// 2. Get service entity list in the cluster
		svrChk(pService->GetService<Svr::GamePartyManagerService>()->PartyDeletedC2SEvt(pOwner->GetEntityUID(), 0));


		Svr::GetServerComponent<EntityManager>()->RemoveEntity( pOwner );

	Proc_End:

		return hr;
	}


	
	/////////////////////////////////////////////////////////////////////////////
	//
	//	GCM register/unregister transaction
	//


	//// Start Transaction
	//HRESULT PartyTransCreateParty::StartTransaction()
	//{
	//	HRESULT hr = S_OK;
	//	EntityManager *pEntityManager = nullptr;
	//	GamePartyEntity *pParty = nullptr;
	//	PartyPlayer *pPlayer = nullptr;

	//	m_PartyUID = 0;

	//	svrChk( __super::StartTransaction() );

	//	svrChkPtr( pEntityManager = Svr::GetServerComponent<EntityManager>() );

	//	svrChk( pEntityManager->CreateGameParty( pParty ) );

	//	svrMem( pPlayer = new PartyPlayer( GetCreatorID(), nullptr ) );
	//	svrChk( pPlayer->SetGameServerEntity( GetMyOwner(), GetRouteContext().From ) );
	//	svrChk( pParty->JoinPlayer( pPlayer ) );


	//	m_PartyUID = pParty->GetPartyUID();
	//	pParty = nullptr;

	//Proc_End:

	//	if( pParty )
	//		pParty->PendingCloseTransaction();

	//	Util::SafeDelete( pPlayer );

	//	CloseTransaction( hr );

	//	return hr;
	//}
	//
	

	// Start Transaction
	HRESULT PartyTransJoinParty::StartTransaction()
	{
		HRESULT hr = S_OK;
		PartyPlayer *pPlayer = nullptr;

		m_LeaderID = 0;

		svrChk( __super::StartTransaction() );


		if( GetInviterID() == 0 )
		{
			// This should be rejoin by relogging
			Policy::ISvrPolicyGameParty *pPolicy = nullptr;

			svrChk( GetMyOwner()->FindPlayer( GetInvitedPlayer().PlayerID, pPlayer ) );
			svrChk( pPlayer->SetServerEntity( GetServerEntity<Svr::ServerEntity>(), GetRouteContext().From ) );

			pPolicy = pPlayer->GetPolicy<Policy::ISvrPolicyGameParty>();

			// Send others to joined
			GetMyOwner()->ForeachPlayer( [&]( PartyPlayer* pOtherPlayer )->HRESULT {
				if( pPlayer != pOtherPlayer )
				{
					pPolicy->PlayerJoinedS2CEvt( pPlayer->GetRouteContext(GetOwnerEntityUID()), pOtherPlayer->GetPlayerInformation() );
				}
				return S_OK;
			});


			pPlayer = nullptr;
		}
		else
		{
			// New member can't be joined during matching
			if( GetMyOwner()->GetMatchingTicket() != 0 )
				svrErrClose(E_SVR_ALREADY_INQUEUE);

			svrMem( pPlayer = new PartyPlayer( GetInvitedPlayer() ) );
			svrChk( pPlayer->SetServerEntity( GetServerEntity<ServerEntity>(), GetRouteContext().From ) );
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
	HRESULT PartyTransLeaveParty::StartTransaction()
	{
		HRESULT hr = S_OK;
		PartyPlayer *pPlayer = nullptr;

		svrChk( __super::StartTransaction() );

		// Any party operation is prohebited during matching
		if( GetMyOwner()->GetMatchingTicket() != 0 )
			svrErrClose(E_SVR_ALREADY_INQUEUE);

		svrChk( GetMyOwner()->FindPlayer( GetPlayerID(), pPlayer ) );
		svrChk( GetMyOwner()->LeavePlayer( pPlayer, false ) );

	Proc_End:

		Util::SafeDelete( pPlayer );
		CloseTransaction( hr );

		return hr;
	}


	// Start Transaction
	HRESULT PartyTransKickPlayer::StartTransaction()
	{
		HRESULT hr = S_OK;
		PartyPlayer *pPlayer = nullptr;

		svrChk( __super::StartTransaction() );

		// Any party operation is prohebited during matching
		if( GetMyOwner()->GetMatchingTicket() != 0 )
			svrErrClose(E_SVR_ALREADY_INQUEUE);

		svrChkClose( GetMyOwner()->FindPlayer( GetPlayerToKick(), pPlayer ) );

		GetMyOwner()->ForeachPlayerSvrGameParty( [&]( PartyPlayer* pOtherPlayer, Policy::ISvrPolicyGameParty *pPolicy )->HRESULT {
			pPolicy->PlayerKickedS2CEvt( pOtherPlayer->GetRouteContext(GetOwnerEntityUID()), GetPlayerToKick() );
			return S_OK;
		});

		svrChk( GetMyOwner()->LeavePlayer( pPlayer, true ) );

	Proc_End:

		Util::SafeDelete( pPlayer );
		CloseTransaction( hr );

		return hr;
	}
	

	// Start Transaction
	HRESULT PartyTransChatMessage::StartTransaction()
	{
		HRESULT hr = S_OK;
		PartyPlayer *pPlayer = nullptr;

		svrChk( __super::StartTransaction() );

		svrChk( GetMyOwner()->FindPlayer( GetPlayerID(), pPlayer ) );

		svrChk( GetMyOwner()->GetChatHistory().AddChattingLog( Util::Time.GetTimeUTCSec32(), pPlayer->GetPlayerID(), 0, ChatType::Normal, GetChatMessage() ) );

		GetMyOwner()->ForeachPlayerSvrGameParty( [&]( PartyPlayer* pOtherPlayer, Policy::ISvrPolicyGameParty *pPolicy )->HRESULT {
				pPolicy->ChatMessageS2CEvt( pOtherPlayer->GetRouteContext(GetOwnerEntityUID()), GetPlayerID(), pPlayer->GetPlayerName(), GetChatMessage() );
				return S_OK;
			});

	Proc_End:

		CloseTransaction( hr );

		return hr;
	}
	

	// Start Transaction
	HRESULT PartyTransQuickChatMessage::StartTransaction()
	{
		HRESULT hr = S_OK;
		PartyPlayer *pPlayer = nullptr;

		svrChk( __super::StartTransaction() );

		svrChk( GetMyOwner()->FindPlayer( GetPlayerID(), pPlayer ) );

		GetMyOwner()->ForeachPlayerSvrGameParty( [&]( PartyPlayer* pOtherPlayer, Policy::ISvrPolicyGameParty *pPolicy )->HRESULT {
				pPolicy->QuickChatMessageS2CEvt( pOtherPlayer->GetRouteContext(GetOwnerEntityUID()), GetPlayerID(), GetQuickChatID() );
				return S_OK;
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

	HRESULT PartyTransStartGameMatchCmd::OnPartyMatchingQueued( Svr::TransactionResult* &pRes )
	{
		HRESULT hr = S_OK;

		Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;
		Message::PartyMatchingQueue::RegisterPartyMatchingRes res;

		svrChk(pRes->GetHRESULT());
		svrChk( res.ParseIMsg( pMsgRes->GetMessage() ) );

		GetMyOwner()->SetMatchingTicket( res.GetMatchingTicket() );
		

		GetMyOwner()->ForeachPlayerSvrGameParty( [&]( PartyPlayer* pOtherPlayer, Policy::ISvrPolicyGameParty *pPolicy )->HRESULT {
			pPolicy->QueuedGameMatchingS2CEvt( pOtherPlayer->GetRouteContext(GetOwnerEntityUID()), res.GetMatchingTicket() );
			return S_OK;
		});


	Proc_End:

		CloseTransaction(hr);

		return hr; 
	}
	
	HRESULT PartyTransStartGameMatchCmd::OnCreateGame(TransactionResult* pRes)
	{
		HRESULT hr = S_OK;
		Message::GameInstanceManager::CreateGameRes msgRes;
		const ServiceInformation *currentMaster = nullptr, *currentSlave = nullptr;
		GameInsUID gameUID;

		svrChk(pRes->GetHRESULT());

		svrChk( msgRes.ParseIMsg( ((MessageResult*)pRes)->GetMessage() ) );

		gameUID = msgRes.GetRouteContext().From;

		// early close for packet order
		CloseTransaction(hr);

		GetMyOwner()->ForeachPlayer( [&]( PartyPlayer* pPlayer )->HRESULT {
			if( pPlayer->GetServerEntity() == nullptr ) return S_OK;

			Policy::ISvrPolicyPartyMatching *pPolicy = pPlayer->GetServerEntity()->GetPolicy<Policy::ISvrPolicyPartyMatching>();
			if( pPolicy == nullptr ) return S_OK;

			pPolicy->PlayerGameMatchedS2CEvt( RouteContext(GetOwnerEntityUID(), pPlayer->GetPlayerEntityUID()), 0, pPlayer->GetPlayerID(), gameUID, PlayerRole::None );

			return S_OK;
		});

	Proc_End:

		return hr;
	}

	HRESULT PartyTransStartGameMatchCmd::StartTransaction()
	{
		HRESULT hr = S_OK;
		Svr::ServerServiceInformation *pService = nullptr;
		StaticArray<MatchingPlayerInformation,GameConst::MAX_GAMEPLAYER> m_matchingPlayers;

		svrChk( __super::StartTransaction() );

		if( GetMyOwner()->GetMatchingTicket() != 0 )
			svrErrClose(E_SVR_ALREADY_INQUEUE);

		if( GetMaxGamePlayers() < GetMyOwner()->GetNumPlayer() )
		{
			svrErr(E_INVALID_PLAYER_COUNT);
		}
		else if( GetMaxGamePlayers() == GetMyOwner()->GetNumPlayer() )
		{
			// We already have full memeber, just create game and let them know to join
			ServerServiceInformation *pService = nullptr;

			svrChk( GetServerComponent<GameInstanceManagerWatcherServiceEntity>()->GetService( pService ) );

			// 2. Get service entity list in the cluster
			svrChk( pService->GetService<GameInstanceManagerService>()->CreateGameCmd( GetTransID(), 0, 0, GetMaxGamePlayers()) );

			//CloseTransaction(hr);
			goto Proc_End;
		}

		auto matchingCompID = Svr::MatchingUtil::GetQueueComponentID(GetMaxGamePlayers(), GetMyOwner()->GetNumPlayer(), PlayerRole::None);
		auto matchingQueueService = Svr::GetServerComponent<Svr::MatchingQueueWatcherServiceEntity>(matchingCompID);
		if (matchingQueueService == nullptr)
		{
			svrTrace(Trace::TRC_ERROR, "Failed to get matching queue service MaxGamePlayer:%0%, NumberOfPlayer:%1%, matchingCompID:%2%", GetMaxGamePlayers(), GetMyOwner()->GetNumPlayer(), matchingCompID);
			svrErr(E_SVR_INVALID_CLUSTERID);
		}
		else
		{
			svrChk(matchingQueueService->GetService(pService));
		}

		GetMyOwner()->ForeachPlayer( [&]( PartyPlayer* pPlayer )->HRESULT {
			m_matchingPlayers.push_back(MatchingPlayerInformation(pPlayer->GetPlayerEntityUID(),pPlayer->GetPlayerID(), PlayerRole::None));
			return S_OK;
		});

		svrChk( pService->GetService<Svr::PartyMatchingQueueService>()->RegisterPartyMatchingCmd( GetTransID(), 0, m_matchingPlayers ) );

		
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
	
	HRESULT PartyTransCancelGameMatchCmd::OnPartyMatchingCanceled( Svr::TransactionResult* &pRes )
	{
		HRESULT hr = S_OK;

		Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;
		Message::PartyMatchingQueue::UnregisterMatchingRes res;

		switch( pRes->GetHRESULT() )
		{
		case E_SVR_RESERVED_QUEUEITEM:
			svrErrClose(E_SVR_RESERVED_QUEUEITEM);
			break;
		case E_SVR_QUEUEITEM_CANCELED:
		case E_SVR_INVALID_QUEUEITEM:
			GetMyOwner()->ForeachPlayerSvrGameParty( [&]( PartyPlayer* pOtherPlayer, Policy::ISvrPolicyGameParty *pPolicy )->HRESULT {
				pPolicy->CanceledGameMatchingS2CEvt( pOtherPlayer->GetRouteContext(GetOwnerEntityUID()), GetMyOwner()->GetMatchingTicket() );
				return S_OK;
			});
			GetMyOwner()->SetMatchingTicket( 0 );
			break;
		default:
			svrChk(pRes->GetHRESULT());
			svrChk( res.ParseIMsg( pMsgRes->GetMessage() ) );
			GetMyOwner()->ForeachPlayerSvrGameParty( [&]( PartyPlayer* pOtherPlayer, Policy::ISvrPolicyGameParty *pPolicy )->HRESULT {
				pPolicy->CanceledGameMatchingS2CEvt( pOtherPlayer->GetRouteContext(GetOwnerEntityUID()), GetMyOwner()->GetMatchingTicket() );
				return S_OK;
			});
			GetMyOwner()->SetMatchingTicket( 0 );
			break;
		};


	Proc_End:

		CloseTransaction(hr);

		return hr; 
	}

	HRESULT PartyTransCancelGameMatchCmd::StartTransaction()
	{
		HRESULT hr = S_OK;
		ServerEntity *pServer = nullptr;

		svrChk( __super::StartTransaction() );

		if( GetMyOwner()->GetMatchingTicket() == 0 )
		{
			svrErrClose(E_SVR_INVALID_QUEUEITEM);
		}


		svrChk( Svr::GetServerComponent<Svr::ServerEntityManager>()->GetServerEntity( GetMyOwner()->GetMatchingTicket().QueueUID.SvrID, pServer ) );

		svrChk( pServer->GetPolicy<Policy::IPolicyPartyMatchingQueue>()->UnregisterMatchingCmd( GetTransID(), RouteContext(GetOwnerEntityUID(), GetMyOwner()->GetMatchingTicket().QueueUID), 0, 
			GetMyOwner()->GetMatchingTicket() ) );

		
	Proc_End:

		CloseTransaction( hr );

		return hr;
	}
	

	
	HRESULT PartyTransPartyMatchingCanceled::StartTransaction()
	{
		HRESULT hr = S_OK;
		ServerEntity *pServer = nullptr;

		svrChk( __super::StartTransaction() );

		if( GetMyOwner()->GetMatchingTicket() != 0 )
		{
			svrErrClose(E_SVR_INVALID_QUEUEITEM);
		}

		GetMyOwner()->ForeachPlayerSvrGameParty( [&]( PartyPlayer* pOtherPlayer, Policy::ISvrPolicyGameParty *pPolicy )->HRESULT {
			pPolicy->CanceledGameMatchingS2CEvt( pOtherPlayer->GetRouteContext(GetOwnerEntityUID()), GetMyOwner()->GetMatchingTicket() );
			return S_OK;
		});

		GetMyOwner()->SetMatchingTicket( 0 );
		
	Proc_End:

		CloseTransaction( hr );

		return hr;
	}
	

	
	HRESULT PartyTransMatchingItemDequeued::StartTransaction()
	{
		HRESULT hr = S_OK;
		ServerEntity *pServer = nullptr;

		svrChk( __super::StartTransaction() );

		if( GetMyOwner()->GetMatchingTicket() == 0 )
		{
			svrErrClose(E_SVR_INVALID_QUEUEITEM);
		}

		GetMyOwner()->ForeachPlayerSvrGameParty( [&]( PartyPlayer* pOtherPlayer, Policy::ISvrPolicyGameParty *pPolicy )->HRESULT {
			pPolicy->MatchingItemDequeuedS2CEvt( pOtherPlayer->GetRouteContext(GetOwnerEntityUID()), GetMyOwner()->GetMatchingTicket() );
			return S_OK;
		});

		GetMyOwner()->SetMatchingTicket( 0 );
		
	Proc_End:

		CloseTransaction( hr );

		return hr;
	}
	
	
	HRESULT PartyTransPartyGameMatchedS2CEvt::StartTransaction()
	{
		HRESULT hr = S_OK;
		ServerEntity *pServer = nullptr;

		svrChk( __super::StartTransaction() );

		GetMyOwner()->SetMatchingTicket( 0 );
		
	Proc_End:

		CloseTransaction( hr );

		return hr;
	}
	



};// namespace Svr 
};// namespace BR 

