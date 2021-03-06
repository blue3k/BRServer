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

#include "MemoryManager/SFMemoryPool.h"
#include "Types/BrSvrTypes.h"
#include "GameConst.h"

#include "Server/BrServerUtil.h"
#include "SvrTrace.h"
#include "Protocol/GameServerMsgClass.h"
#include "Protocol/GameServerNetPolicy.h"
#include "Protocol/GamePartyMsgClass.h"
#include "Protocol/GamePartyNetPolicy.h"

#include "Protocol/PartyMatchingNetPolicy.h"
#include "Protocol/PartyMatchingQueueMsgClass.h"
#include "Protocol/PartyMatchingQueueNetPolicy.h"
#include "ServiceEntity/MatchingServiceUtil.h"
#include "Protocol/PartyMatchingQueueService.h"
#include "ServiceEntity/MatchingQueueServiceEntity.h"

#include "Protocol/GameInstanceManagerMsgClass.h"
#include "Protocol/GamePartyManagerService.h"
#include "Protocol/GameInstanceManagerService.h"

#include "ServiceEntity/Party/GamePartyManagerServiceEntity.h"
#include "ServiceEntity/Game/GameInstanceManagerServiceEntity.h"


#include "Server/BrServer.h"

#include "Entity/EntityManager.h"
#include "ServiceEntity/Party/GamePartyEntity.h"
#include "ServiceEntity/Party/GamePartyEntityTrans.h"

//#include "DB/GameDB.h"
#include "DB/AccountDB.h"
#include "DB/AccountQuery.h"





namespace SF {
namespace Svr {


	Result PartyTransCloseInstance::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		GamePartyEntity* pOwner = (GamePartyEntity*)GetOwnerEntity();
		SharedPointerT<ServerServiceInformation> pService;

		svrCheck(Transaction::StartTransaction() );

		svrCheck(Service::ServiceDirectory->GetRandomService(m_GameID, ClusterID::GamePartyManager, pService));
		svrCheck(pService->GetService<GamePartyManagerService>()->PartyDeletedC2SEvt(pOwner->GetEntityUID(), 0));

		Service::EntityManager->RemoveEntity( pOwner );

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
			svrChk( GetMyOwner()->FindPlayer( GetInvitedPlayer().PlayerID, pPlayer ) );
			svrChk( pPlayer->SetRemoteEndpoint(GetRouteContext().GetFrom(), GetRemoteEndpoint()) );

			NetSvrPolicyGameParty policy(pPlayer->GetRemoteEndpoint());

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

			svrMem( pPlayer = new(GetHeap()) PartyPlayer(GetMyOwner()->GetHeap(), GetRouteContext().GetFrom(), GetInvitedPlayer() ) );
			svrChk( pPlayer->SetRemoteEndpoint(GetRouteContext().GetFrom(), GetRemoteEndpoint()));
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

		GetMyOwner()->ForeachPlayerSvrGameParty( [&]( PartyPlayer* pOtherPlayer, NetSvrPolicyGameParty &pPolicy )->Result
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

		GetMyOwner()->ForeachPlayerSvrGameParty( [&]( PartyPlayer* pOtherPlayer, NetSvrPolicyGameParty &pPolicy )->Result {
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

		GetMyOwner()->ForeachPlayerSvrGameParty( [&]( PartyPlayer* pOtherPlayer, NetSvrPolicyGameParty &pPolicy )->Result {
				pPolicy.QuickChatMessageS2CEvt( pOtherPlayer->GetRouteContext(GetOwnerEntityUID()), GetPlayerID(), GetQuickChatID() );
				return ResultCode::SUCCESS;
			});

	Proc_End:

		CloseTransaction( hr );

		return hr;
	}
	

	PartyTransStartGameMatchCmd::PartyTransStartGameMatchCmd(IHeap& memHeap, const MessageDataPtr &pIMsg )
		:GamePartyMessageTransaction(memHeap, pIMsg )
	{
		SetExclusive(true);
		BR_TRANS_MESSAGE( Message::PartyMatchingQueue::RegisterPartyMatchingRes, { return OnPartyMatchingQueued(pRes); } );
		BR_TRANS_MESSAGE( Message::GameInstanceManager::CreateGameInstanceRes,	{ return OnCreateGame(pRes); });
	}

	Result PartyTransStartGameMatchCmd::OnPartyMatchingQueued( Svr::TransactionResult* &pRes )
	{
		Result hr = ResultCode::SUCCESS;

		Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;
		Message::PartyMatchingQueue::RegisterPartyMatchingRes res;

		svrChk(pRes->GetResult());
		svrChk( res.ParseMessage( *pMsgRes->GetMessage() ) );

		GetMyOwner()->SetMatchingQueueTicket( res.GetMatchingTicket() );
		

		GetMyOwner()->ForeachPlayerSvrGameParty( [&]( PartyPlayer* pOtherPlayer, NetSvrPolicyGameParty &pPolicy )->Result {
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
		Message::GameInstanceManager::CreateGameInstanceRes msgRes;
		GameInsUID gameUID;

		svrChk(pRes->GetResult());

		svrChk( msgRes.ParseMessage( *((MessageResult*)pRes)->GetMessage() ) );

		gameUID = msgRes.GetRouteContext().GetFrom();

		// early close for packet order
		CloseTransaction(hr);

		GetMyOwner()->ForeachPlayer( [&]( PartyPlayer* pPlayer )->Result {
			if( pPlayer->GetRemoteEndpoint() == nullptr ) return ResultCode::SUCCESS;

			NetSvrPolicyPartyMatching pPolicy( pPlayer->GetRemoteEndpoint());
			pPolicy.PlayerGameMatchedS2CEvt( RouteContext(GetOwnerEntityUID(), pPlayer->GetPlayerEntityUID()), 0, pPlayer->GetPlayerID(), gameUID, (uint8_t)PlayerRole::None );

			return ResultCode::SUCCESS;
		});

	Proc_End:

		return hr;
	}

	Result PartyTransStartGameMatchCmd::StartTransaction()
	{
		ScopeContext hr([this](Result hr)
			{
				if (!hr)
					CloseTransaction(hr);

			});
		StaticArray<MatchingPlayerInformation, 16> m_matchingPlayers(GetHeap());

		svrCheck( super::StartTransaction() );

		if( GetMyOwner()->GetMatchingQueueTicket() != 0 )
			svrErrorClose(ResultCode::SVR_ALREADY_INQUEUE);

		if( GetMaxGamePlayers() < GetMyOwner()->GetNumPlayer() )
		{
			svrError(ResultCode::INVALID_PLAYER_COUNT);
		}
		else if( GetMaxGamePlayers() == GetMyOwner()->GetNumPlayer() )
		{
			// We already have full member, just create game and let them know to join
			SharedPointerT<ServerServiceInformation> pService;

			svrCheck(Service::ServiceDirectory->GetRandomService(GetMyOwner()->GetGameID(), ClusterID::GameInstanceManager, pService));

			VariableTable attributes(GetHeap());
			attributes.SetValue("MaxPlayer"_crc, GetMaxGamePlayers());
			// 2. Get service entity list in the cluster
			uint32_t ZoneTableId = 0;
			svrCheck( pService->GetService<GameInstanceManagerService>()->CreateGameInstanceCmd(GetTransID(), 0, ZoneTableId, attributes) );

			return hr;
		}

		{
			SharedPointerT<ServerServiceInformation> pService;
			auto queueClusterID = Svr::MatchingUtil::GetQueueClusterID(GetMaxGamePlayers(), GetMyOwner()->GetNumPlayer(), PlayerRole::None);

			svrCheck(Service::ServiceDirectory->GetRandomService(Service::ServerConfig->GameClusterID, queueClusterID, pService));

			//auto matchingCompID = Svr::MatchingUtil::GetQueueComponentID(GetMaxGamePlayers(), GetMyOwner()->GetNumPlayer(), PlayerRole::None);
			//auto matchingQueueService = Svr::GetServerComponent<Svr::RingClusterServiceEntity>(matchingCompID);
			if (pService == nullptr)
			{
				svrTrace(Error, "Failed to get matching queue service MaxGamePlayer:{0}, NumberOfPlayer:{1}, queueClusterID:{2}", GetMaxGamePlayers(), GetMyOwner()->GetNumPlayer(), queueClusterID);
				svrError(ResultCode::SVR_INVALID_CLUSTERID);
			}

			GetMyOwner()->ForeachPlayer([&](PartyPlayer* pPlayer)->Result {
				m_matchingPlayers.push_back(MatchingPlayerInformation(pPlayer->GetPlayerEntityUID(), pPlayer->GetPlayerID(), PlayerRole::None));
				return ResultCode::SUCCESS;
			});

			svrCheck(pService->GetService<PartyMatchingQueueService>()->RegisterPartyMatchingCmd(GetTransID(), 0, m_matchingPlayers));
		}

		return hr;
	}
	



	PartyTransCancelGameMatchCmd::PartyTransCancelGameMatchCmd(IHeap& memHeap, const MessageDataPtr &pIMsg )
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
		case (int32_t)ResultCode::SVR_RESERVED_QUEUEITEM:
			svrErrClose(ResultCode::SVR_RESERVED_QUEUEITEM);
			break;
		case (int32_t)ResultCode::SVR_QUEUEITEM_CANCELED:
		case (int32_t)ResultCode::SVR_INVALID_QUEUEITEM:
			GetMyOwner()->ForeachPlayerSvrGameParty( [&]( PartyPlayer* pOtherPlayer, NetSvrPolicyGameParty &pPolicy )->Result {
				pPolicy.CanceledGameMatchingS2CEvt( pOtherPlayer->GetRouteContext(GetOwnerEntityUID()), GetMyOwner()->GetMatchingQueueTicket() );
				return ResultCode::SUCCESS;
			});
			GetMyOwner()->SetMatchingQueueTicket( 0 );
			break;
		default:
			svrChk(pRes->GetResult());
			svrChk( res.ParseMessage( *pMsgRes->GetMessage() ) );
			GetMyOwner()->ForeachPlayerSvrGameParty( [&]( PartyPlayer* pOtherPlayer, NetSvrPolicyGameParty &pPolicy )->Result {
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
		ScopeContext hr([this](Result hr) 
			{
				CloseTransaction(hr);
			});

		SharedPointerT<MessageEndpoint> pEndpoint;

		svrCheck( super::StartTransaction() );

		if( GetMyOwner()->GetMatchingQueueTicket() == 0 )
		{
			svrCheckClose(ResultCode::SVR_INVALID_QUEUEITEM);
		}

		pEndpoint = Service::MessageEndpointManager->GetEndpoint(GetMyOwner()->GetMatchingQueueTicket().QueueUID);
		svrCheckPtr(pEndpoint);
		//svrChk( Service::ServerEntityManager->GetServerEntity( GetMyOwner()->GetMatchingQueueTicket().QueueUID.GetServerID(), pServer ) );

		svrCheck(NetPolicyPartyMatchingQueue(pEndpoint).UnregisterMatchingCmd( RouteContext(GetOwnerEntityUID(), GetMyOwner()->GetMatchingQueueTicket().QueueUID), GetTransID(), 0,
			GetMyOwner()->GetMatchingQueueTicket() ) );

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

		GetMyOwner()->ForeachPlayerSvrGameParty( [&]( PartyPlayer* pOtherPlayer, NetSvrPolicyGameParty &pPolicy )->Result {
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

		GetMyOwner()->ForeachPlayerSvrGameParty( [&]( PartyPlayer* pOtherPlayer, NetSvrPolicyGameParty &pPolicy )->Result {
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

