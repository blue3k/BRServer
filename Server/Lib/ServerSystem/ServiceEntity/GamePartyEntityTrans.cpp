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
#include "ResultCode/SFResultCodeSvr.h"

#include "Memory/SFMemoryPool.h"
#include "Types/BrSvrTypes.h"
#include "GameConst.h"

#include "Server/BrServerUtil.h"
#include "SvrTrace.h"
#include "Protocol/Message/GameServerMsgClass.h"
#include "Protocol/Policy/GameServerNetPolicy.h"
#include "Protocol/Message/GamePartyMsgClass.h"
#include "Protocol/Policy/GamePartyNetPolicy.h"

#include "Protocol/Policy/PartyMatchingNetPolicy.h"
#include "Protocol/Message/PartyMatchingQueueMsgClass.h"
#include "Protocol/Policy/PartyMatchingQueueNetPolicy.h"
#include "ServiceEntity/MatchingServiceUtil.h"
#include "Protocol/ServerService/PartyMatchingQueueService.h"
#include "ServiceEntity/MatchingQueueServiceEntity.h"

#include "Protocol/Message/GameInstanceManagerMsgClass.h"
#include "Protocol/ServerService/GamePartyManagerService.h"
#include "Protocol/ServerService/GameInstanceManagerService.h"

#include "ServiceEntity/GamePartyManagerServiceEntity.h"
#include "ServiceEntity/Game/GameInstanceManagerServiceEntity.h"


#include "Server/BrServer.h"

#include "Entity/EntityManager.h"
#include "ServerEntity/ServerEntityManager.h"
#include "ServiceEntity/GamePartyEntity.h"
#include "ServiceEntity/GamePartyEntityTrans.h"

//#include "DB/GameDB.h"
#include "DB/AccountDB.h"
#include "DB/AccountQuery.h"



SF_MEMORYPOOL_IMPLEMENT(SF::Svr::PartyTransCloseInstance);
//SF_MEMORYPOOL_IMPLEMENT(SF::Svr::PartyTransCreateParty);
SF_MEMORYPOOL_IMPLEMENT(SF::Svr::PartyTransJoinParty);
SF_MEMORYPOOL_IMPLEMENT(SF::Svr::PartyTransLeaveParty);
SF_MEMORYPOOL_IMPLEMENT(SF::Svr::PartyTransKickPlayer);
SF_MEMORYPOOL_IMPLEMENT(SF::Svr::PartyTransChatMessage);
SF_MEMORYPOOL_IMPLEMENT(SF::Svr::PartyTransQuickChatMessage);
SF_MEMORYPOOL_IMPLEMENT(SF::Svr::PartyTransStartGameMatchCmd);
SF_MEMORYPOOL_IMPLEMENT(SF::Svr::PartyTransCancelGameMatchCmd);
SF_MEMORYPOOL_IMPLEMENT(SF::Svr::PartyTransPartyMatchingCanceled);
SF_MEMORYPOOL_IMPLEMENT(SF::Svr::PartyTransMatchingItemDequeued);
SF_MEMORYPOOL_IMPLEMENT(SF::Svr::PartyTransPartyGameMatchedS2CEvt);



namespace SF {
namespace Svr {


	Result PartyTransCloseInstance::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		GamePartyEntity* pOwner = (GamePartyEntity*)GetOwnerEntity();
		ServerServiceInformation *pService = nullptr;

		svrChk(Transaction::StartTransaction() );

		svrChk(Service::ClusterManager->GetRandomService(m_GameID, ClusterID::GamePartyManager, pService));
		svrChk(pService->GetService<Svr::GamePartyManagerService>()->PartyDeletedC2SEvt(pOwner->GetEntityUID(), 0));

		Service::EntityManager->RemoveEntity( pOwner );

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
			Policy::NetSvrPolicyGameParty *pPolicy = nullptr;

			svrChk( GetMyOwner()->FindPlayer( GetInvitedPlayer().PlayerID, pPlayer ) );
			svrChk( pPlayer->SetServerEntity( GetServerEntity<Svr::ServerEntity>(), GetRouteContext().GetFrom()) );

			Policy::NetSvrPolicyGameParty policy(pPlayer->GetConnection());

			// Send others to joined
			GetMyOwner()->ForeachPlayer( [&]( PartyPlayer* pOtherPlayer )->Result {
				if( pPlayer != pOtherPlayer )
				{
					policy.PlayerJoinedS2CEvt( pPlayer->GetRouteContext(GetOwnerEntityUID()), pOtherPlayer->GetPlayerInformation() );
				}
				return ResultCode::SUCCESS;
			});


			pPlayer = nullptr;
		}
		else
		{
			// New member can't be joined during matching
			if( GetMyOwner()->GetMatchingQueueTicket() != 0 )
				svrErrClose(ResultCode::SVR_ALREADY_INQUEUE);

			svrMem( pPlayer = new(GetHeap()) PartyPlayer( GetInvitedPlayer() ) );
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
		if( GetMyOwner()->GetMatchingQueueTicket() != 0 )
			svrErrClose(ResultCode::SVR_ALREADY_INQUEUE);

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
		if( GetMyOwner()->GetMatchingQueueTicket() != 0 )
			svrErrClose(ResultCode::SVR_ALREADY_INQUEUE);

		svrChkClose( GetMyOwner()->FindPlayer( GetPlayerToKick(), pPlayer ) );

		GetMyOwner()->ForeachPlayerSvrGameParty( [&]( PartyPlayer* pOtherPlayer, Policy::NetSvrPolicyGameParty &pPolicy )->Result
		{
			pPolicy.PlayerKickedS2CEvt( pOtherPlayer->GetRouteContext(GetOwnerEntityUID()), GetPlayerToKick() );
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

		GetMyOwner()->ForeachPlayerSvrGameParty( [&]( PartyPlayer* pOtherPlayer, Policy::NetSvrPolicyGameParty &pPolicy )->Result {
				pPolicy.ChatMessageS2CEvt( pOtherPlayer->GetRouteContext(GetOwnerEntityUID()), GetPlayerID(), pPlayer->GetPlayerName(), GetChatMessage() );
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

		GetMyOwner()->ForeachPlayerSvrGameParty( [&]( PartyPlayer* pOtherPlayer, Policy::NetSvrPolicyGameParty &pPolicy )->Result {
				pPolicy.QuickChatMessageS2CEvt( pOtherPlayer->GetRouteContext(GetOwnerEntityUID()), GetPlayerID(), GetQuickChatID() );
				return ResultCode::SUCCESS;
			});

	Proc_End:

		CloseTransaction( hr );

		return hr;
	}
	

	PartyTransStartGameMatchCmd::PartyTransStartGameMatchCmd(IHeap& memHeap, MessageDataPtr &pIMsg )
		:GamePartyMessageTransaction(memHeap, pIMsg )
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
		svrChk( res.ParseMessage( *pMsgRes->GetMessage() ) );

		GetMyOwner()->SetMatchingQueueTicket( res.GetMatchingTicket() );
		

		GetMyOwner()->ForeachPlayerSvrGameParty( [&]( PartyPlayer* pOtherPlayer, Policy::NetSvrPolicyGameParty &pPolicy )->Result {
			pPolicy.QueuedGameMatchingS2CEvt( pOtherPlayer->GetRouteContext(GetOwnerEntityUID()), res.GetMatchingTicket() );
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

		svrChk( msgRes.ParseMessage( *((MessageResult*)pRes)->GetMessage() ) );

		gameUID = msgRes.GetRouteContext().GetFrom();

		// early close for packet order
		CloseTransaction(hr);

		GetMyOwner()->ForeachPlayer( [&]( PartyPlayer* pPlayer )->Result {
			if( pPlayer->GetServerEntity() == nullptr ) return ResultCode::SUCCESS;

			Policy::NetSvrPolicyPartyMatching pPolicy( pPlayer->GetConnection());
			pPolicy.PlayerGameMatchedS2CEvt( RouteContext(GetOwnerEntityUID(), pPlayer->GetPlayerEntityUID()), 0, pPlayer->GetPlayerID(), gameUID, (uint8_t)PlayerRole::None );

			return ResultCode::SUCCESS;
		});

	Proc_End:

		return hr;
	}

	Result PartyTransStartGameMatchCmd::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		Svr::ServerServiceInformation *pService = nullptr;
		StaticArray<MatchingPlayerInformation,GameConst::MAX_GAMEPLAYER> m_matchingPlayers(GetHeap());

		svrChk( super::StartTransaction() );

		if( GetMyOwner()->GetMatchingQueueTicket() != 0 )
			svrErrClose(ResultCode::SVR_ALREADY_INQUEUE);

		if( GetMaxGamePlayers() < GetMyOwner()->GetNumPlayer() )
		{
			svrErr(ResultCode::INVALID_PLAYER_COUNT);
		}
		else if( GetMaxGamePlayers() == GetMyOwner()->GetNumPlayer() )
		{
			// We already have full member, just create game and let them know to join
			ServerServiceInformation *pService = nullptr;

			svrChk(Service::ClusterManager->GetRandomService(GetMyOwner()->GetGameID(), ClusterID::GameInstanceManager, pService));
			//svrChk( GetServerComponent<GameInstanceManagerWatcherServiceEntity>()->GetService( pService ) );

			// 2. Get service entity list in the cluster
			svrChk( pService->GetService<GameInstanceManagerService>()->CreateGameCmd(GetTransID(), 0, 0, (USHORT)GetMaxGamePlayers()) );

			//CloseTransaction(hr);
			goto Proc_End;
		}

		{
			ClusteredServiceEntity *matchingQueueService = nullptr;
			auto queueClusterID = Svr::MatchingUtil::GetQueueClusterID(GetMaxGamePlayers(), GetMyOwner()->GetNumPlayer(), PlayerRole::None);

			svrChk(Service::ClusterManager->GetRandomService(GetServerGameID(), queueClusterID, pService));

			//auto matchingCompID = Svr::MatchingUtil::GetQueueComponentID(GetMaxGamePlayers(), GetMyOwner()->GetNumPlayer(), PlayerRole::None);
			//auto matchingQueueService = Svr::GetServerComponent<Svr::RingClusterServiceEntity>(matchingCompID);
			if (pService == nullptr)
			{
				svrTrace(Error, "Failed to get matching queue service MaxGamePlayer:{0}, NumberOfPlayer:{1}, queueClusterID:{2}", GetMaxGamePlayers(), GetMyOwner()->GetNumPlayer(), queueClusterID);
				svrErr(ResultCode::SVR_INVALID_CLUSTERID);
			}

			GetMyOwner()->ForeachPlayer([&](PartyPlayer* pPlayer)->Result {
				m_matchingPlayers.push_back(MatchingPlayerInformation(pPlayer->GetPlayerEntityUID(), pPlayer->GetPlayerID(), PlayerRole::None));
				return ResultCode::SUCCESS;
			});

			svrChk(pService->GetService<Svr::PartyMatchingQueueService>()->RegisterPartyMatchingCmd(GetTransID(), 0, m_matchingPlayers));
		}

		
	Proc_End:

		if( !(hr) )
			CloseTransaction( hr );

		return hr;
	}
	



	PartyTransCancelGameMatchCmd::PartyTransCancelGameMatchCmd(IHeap& memHeap, MessageDataPtr &pIMsg )
		:GamePartyMessageTransaction(memHeap, pIMsg )
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
		case ResultCode::SVR_RESERVED_QUEUEITEM:
			svrErrClose(ResultCode::SVR_RESERVED_QUEUEITEM);
			break;
		case ResultCode::SVR_QUEUEITEM_CANCELED:
		case ResultCode::SVR_INVALID_QUEUEITEM:
			GetMyOwner()->ForeachPlayerSvrGameParty( [&]( PartyPlayer* pOtherPlayer, Policy::NetSvrPolicyGameParty &pPolicy )->Result {
				pPolicy.CanceledGameMatchingS2CEvt( pOtherPlayer->GetRouteContext(GetOwnerEntityUID()), GetMyOwner()->GetMatchingQueueTicket() );
				return ResultCode::SUCCESS;
			});
			GetMyOwner()->SetMatchingQueueTicket( 0 );
			break;
		default:
			svrChk(pRes->GetResult());
			svrChk( res.ParseMessage( *pMsgRes->GetMessage() ) );
			GetMyOwner()->ForeachPlayerSvrGameParty( [&]( PartyPlayer* pOtherPlayer, Policy::NetSvrPolicyGameParty &pPolicy )->Result {
				pPolicy.CanceledGameMatchingS2CEvt( pOtherPlayer->GetRouteContext(GetOwnerEntityUID()), GetMyOwner()->GetMatchingQueueTicket() );
				return ResultCode::SUCCESS;
			});
			GetMyOwner()->SetMatchingQueueTicket( 0 );
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

		if( GetMyOwner()->GetMatchingQueueTicket() == 0 )
		{
			svrErrClose(ResultCode::SVR_INVALID_QUEUEITEM);
		}


		svrChk( Service::ServerEntityManager->GetServerEntity( GetMyOwner()->GetMatchingQueueTicket().QueueUID.GetServerID(), pServer ) );

		svrChk(Policy::NetPolicyPartyMatchingQueue(pServer->GetConnection()).UnregisterMatchingCmd( RouteContext(GetOwnerEntityUID(), GetMyOwner()->GetMatchingQueueTicket().QueueUID), GetTransID(), 0,
			GetMyOwner()->GetMatchingQueueTicket() ) );

		
	Proc_End:

		CloseTransaction( hr );

		return hr;
	}
	

	
	Result PartyTransPartyMatchingCanceled::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk( super::StartTransaction() );

		if( GetMyOwner()->GetMatchingQueueTicket() != 0 )
		{
			svrErrClose(ResultCode::SVR_INVALID_QUEUEITEM);
		}

		GetMyOwner()->ForeachPlayerSvrGameParty( [&]( PartyPlayer* pOtherPlayer, Policy::NetSvrPolicyGameParty &pPolicy )->Result {
			pPolicy.CanceledGameMatchingS2CEvt( pOtherPlayer->GetRouteContext(GetOwnerEntityUID()), GetMyOwner()->GetMatchingQueueTicket() );
			return ResultCode::SUCCESS;
		});

		GetMyOwner()->SetMatchingQueueTicket( 0 );
		
	Proc_End:

		CloseTransaction( hr );

		return hr;
	}
	

	
	Result PartyTransMatchingItemDequeued::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk( super::StartTransaction() );

		if( GetMyOwner()->GetMatchingQueueTicket() == 0 )
		{
			svrErrClose(ResultCode::SVR_INVALID_QUEUEITEM);
		}

		GetMyOwner()->ForeachPlayerSvrGameParty( [&]( PartyPlayer* pOtherPlayer, Policy::NetSvrPolicyGameParty &pPolicy )->Result {
			pPolicy.MatchingItemDequeuedS2CEvt( pOtherPlayer->GetRouteContext(GetOwnerEntityUID()), GetMyOwner()->GetMatchingQueueTicket() );
			return ResultCode::SUCCESS;
		});

		GetMyOwner()->SetMatchingQueueTicket( 0 );
		
	Proc_End:

		CloseTransaction( hr );

		return hr;
	}
	
	
	Result PartyTransPartyGameMatchedS2CEvt::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk( super::StartTransaction() );

		GetMyOwner()->SetMatchingQueueTicket( 0 );
		
	Proc_End:

		CloseTransaction( hr );

		return hr;
	}
	



};// namespace Svr 
};// namespace SF 

