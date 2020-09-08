////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Game Party entity
//
////////////////////////////////////////////////////////////////////////////////



#include "ServerSystemPCH.h"
#include "SvrConst.h"
#include "SvrTrace.h"
#include "Util/SFTimeUtil.h"
#include "Types/SFEngineTypedefs.h"
#include "ResultCode/SFResultCodeLibrary.h"
#include "ResultCode/SFResultCodeGame.h"
#include "Net/SFMessage.h"

#include "Protocol/Policy/GameServerNetPolicy.h"
#include "Protocol/Policy/GamePartyNetPolicy.h"

#include "Transaction/MessageRoute.h"
#include "ServiceEntity/Party/GamePartyEntity.h"

#include "ServiceEntity/Party/GamePartyEntityTrans.h"




namespace SF {
namespace Svr {


	//////////////////////////////////////////////////////////////////////////
	//
	//	Entity server class
	//


	GamePartyEntity::GamePartyEntity(GameID gameID)
		: MasterEntity(64,64)
		, m_PartyPlayerByUID(GetHeap())
		, m_GameID(gameID)
		, m_LeaderID(0)
		, m_ChatHistory(GetHeap())
	{
		SetTickInterval(Const::PARTY_TICKTASK_INTERVAL);


		// Game party instance transactions
		BR_ENTITY_MESSAGE(Message::GameParty::JoinPartyCmd)								{ svrMemReturn(pNewTrans = new(GetHeap()) PartyTransJoinParty(GetHeap(),pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::GameParty::LeavePartyCmd)							{ svrMemReturn(pNewTrans = new(GetHeap()) PartyTransLeaveParty(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::GameParty::KickPlayerCmd)							{ svrMemReturn(pNewTrans = new(GetHeap()) PartyTransKickPlayer(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::GameParty::ChatMessageC2SEvt)						{ svrMemReturn(pNewTrans = new(GetHeap()) PartyTransChatMessage(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::GameParty::QuickChatMessageC2SEvt)					{ svrMemReturn(pNewTrans = new(GetHeap()) PartyTransQuickChatMessage(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );

		BR_ENTITY_MESSAGE(Message::GameParty::StartGameMatchCmd)						{ svrMemReturn(pNewTrans = new(GetHeap()) PartyTransStartGameMatchCmd(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::GameParty::CancelGameMatchCmd)						{ svrMemReturn(pNewTrans = new(GetHeap()) PartyTransCancelGameMatchCmd(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::PartyMatchingQueue::PartyMatchingCanceledS2CEvt)		{ svrMemReturn(pNewTrans = new(GetHeap()) PartyTransPartyMatchingCanceled(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::PartyMatchingQueue::PartyMatchingItemDequeuedS2CEvt) { svrMemReturn(pNewTrans = new(GetHeap()) PartyTransMatchingItemDequeued(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::PartyMatching::PartyGameMatchedS2CEvt)				{ svrMemReturn(pNewTrans = new(GetHeap()) PartyTransPartyGameMatchedS2CEvt(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
	}

	GamePartyEntity::~GamePartyEntity()
	{
	}


	// Initialize entity to proceed new connection
	Result GamePartyEntity::InitializeEntity( EntityID newEntityID )
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(MasterEntity::InitializeEntity( newEntityID ) );

		m_ChatHistory.InitializeLog();

		m_PartyPlayerByUID.clear();

		m_LeaderID = 0;
		m_MatchingTicket = 0;

	Proc_End:	

		return hr;
	}

	// Close entity and clear transaction
	Result GamePartyEntity::TerminateEntity()
	{
		Result hr = ResultCode::SUCCESS;

		if( GetEntityState() == EntityState::FREE )
			return ResultCode::SUCCESS;

		m_ChatHistory.TerminateLog();

		ForeachPlayer( [](PartyPlayer* pPlayer)->Result {
			Util::SafeDelete(pPlayer);
			return ResultCode::SUCCESS;
		});
		m_PartyPlayerByUID.clear();

		svrChk(MasterEntity::TerminateEntity() );

	Proc_End:	

		return hr;
	}

	
	//////////////////////////////////////////////////////////////////////////////////////////
	//
	//	foreach implementations
	//

	// foreach game player
	Result GamePartyEntity::ForeachPlayer( std::function<Result(PartyPlayer* pPlayer)> func )
	{
		for(auto itPlayer : m_PartyPlayerByUID)
		{
			PartyPlayer* pPartyPlayer = itPlayer;
			if( pPartyPlayer )
			{
				Result hrRes = func( pPartyPlayer );
				if( !(hrRes) )
					return hrRes;
			}
		}
		return ResultCode::SUCCESS;
	}

	// foreach game player with Game policy
	Result GamePartyEntity::ForeachPlayerGameServer( std::function<Result(PartyPlayer* pPlayer, Policy::NetPolicyGameServer &pPolicy)> func )
	{
		for( auto itPlayer = m_PartyPlayerByUID.begin(); itPlayer != m_PartyPlayerByUID.end(); ++ itPlayer )
		{
			PartyPlayer* pPartyPlayer = *itPlayer;
			if( pPartyPlayer == nullptr )
				continue;

			Policy::NetPolicyGameServer netPolicy(pPartyPlayer->GetConnection());
			Result hrRes = func( pPartyPlayer, netPolicy);
			if( !(hrRes) )
				return hrRes;
		}
		return ResultCode::SUCCESS;
	}

	Result GamePartyEntity::ForeachPlayerSvrGameParty( std::function<Result(PartyPlayer* pPlayer, Policy::NetSvrPolicyGameParty &pPolicy)> func )
	{
		for( auto itPlayer = m_PartyPlayerByUID.begin(); itPlayer != m_PartyPlayerByUID.end(); ++ itPlayer )
		{
			PartyPlayer* pPartyPlayer = *itPlayer;
			if( pPartyPlayer == nullptr )
				continue;

			Policy::NetSvrPolicyGameParty netPolicy(pPartyPlayer->GetConnection());
			Result hrRes = func(pPartyPlayer, netPolicy);
			if (!(hrRes))
				return hrRes;
		}
		return ResultCode::SUCCESS;
	}
	



	//////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Game Player
	//

	// Register new player to join
	Result GamePartyEntity::JoinPlayer( PartyPlayer* &pPlayer, bool bIsSilent )
	{
		Result hr = ResultCode::SUCCESS;
		PartyPlayerUIDMap::iterator itLeader;

		svrChkPtr( pPlayer );

		svrChk( m_PartyPlayerByUID.insert(pPlayer->GetPlayerID(), pPlayer ));
		
		if( !bIsSilent )
		{
			Policy::NetSvrPolicyGameParty pJoindPolicy(pPlayer->GetConnection());

			ForeachPlayerSvrGameParty( [&]( PartyPlayer* pOtherPlayer, Policy::NetSvrPolicyGameParty &pOtherPolicy )->Result {
				if( pPlayer != pOtherPlayer )
				{
					// Send others to joined
					pJoindPolicy.PlayerJoinedS2CEvt( pPlayer->GetRouteContext(GetEntityUID()), pOtherPlayer->GetPlayerInformation() );
					// Send joined to others
					pOtherPolicy.PlayerJoinedS2CEvt( pOtherPlayer->GetRouteContext(GetEntityUID()), pPlayer->GetPlayerInformation() );
				}
				return ResultCode::SUCCESS;
			});

		}

		// We need a new leader
		if( m_LeaderID == 0 || !(m_PartyPlayerByUID.find( m_LeaderID, itLeader )) )
		{
			m_LeaderID = 0;
			svrChk( SelectNewLeader(bIsSilent) );
		}

		pPlayer = nullptr;

	Proc_End:

		return hr;
	}


	// Player leave
	Result GamePartyEntity::LeavePlayer( PartyPlayer* &pPlayer, bool bIsSilent )
	{
		Result hr = ResultCode::SUCCESS;

		m_PartyPlayerByUID.erase(pPlayer->GetPlayerID() );

		if( !bIsSilent )
		{
			ForeachPlayerSvrGameParty( [&]( PartyPlayer* pOtherPlayer, Policy::NetSvrPolicyGameParty &pPolicy )->Result {
				pPolicy.PlayerLeftS2CEvt( pOtherPlayer->GetRouteContext(GetEntityUID()), pPlayer->GetPlayerID() );
				return ResultCode::SUCCESS;
			});
		}

		svrChk( OnPlayerGetOutOfParty(pPlayer, bIsSilent) );

	Proc_End:

		Util::SafeDelete( pPlayer );

		if( m_PartyPlayerByUID.size() == 0 ) // if no player remain
		{
			PendingCloseTransaction("Player left");
		}

		return hr;
	}

	// Select new leader
	Result GamePartyEntity::SelectNewLeader( bool bIsSilent )
	{
		Result hr = ResultCode::SUCCESS;

		auto itPlayer = m_PartyPlayerByUID.begin();
		if( itPlayer.IsValid() )
		{
			m_LeaderID = itPlayer->GetPlayerID();

			if( !bIsSilent )
			{
				ForeachPlayerSvrGameParty( [&]( PartyPlayer* pOtherPlayer, Policy::NetSvrPolicyGameParty &pPolicy )->Result {
					pPolicy.PartyLeaderChangedS2CEvt( pOtherPlayer->GetRouteContext(GetEntityUID()), m_LeaderID );
					return ResultCode::SUCCESS;
				});
			}
		}
		else
		{
			m_LeaderID = 0;
		}


	//Proc_End:

		return hr;
	}


	// Leave all player
	Result GamePartyEntity::LeaveAllPlayer()
	{
		auto itPlayer = m_PartyPlayerByUID.begin();
		while(itPlayer.IsValid())
		{
			PartyPlayer *pPlayer = *itPlayer;
			itPlayer = nullptr;

			LeavePlayer( pPlayer, true );

			// Leave player will remove player from the table
			itPlayer = m_PartyPlayerByUID.begin();
		}

		m_PartyPlayerByUID.clear();

		return ResultCode::SUCCESS;
	}

	// Find Player pilotid
	Result GamePartyEntity::FindPlayer( PlayerID pltID, PartyPlayer* &pPartyPlayer )
	{
		Result hr = ResultCode::SUCCESS;
		PartyPlayerUIDMap::iterator itPlayer;

		if( !(m_PartyPlayerByUID.find( pltID, itPlayer )) )
		{
			return ResultCode::SVR_PLAYER_NOT_FOUND;
		}

		pPartyPlayer = *itPlayer;

	//Proc_End:


		return hr;
	}
	
	
	// Called when a player get out of the party
	Result GamePartyEntity::OnPlayerGetOutOfParty( PartyPlayer* pPlayer, bool bIsSilent )
	{
		Result hr = ResultCode::SUCCESS;

		svrChkPtr(pPlayer);

		// hire new leader if he was the leader and someone remain in this party
		if( pPlayer->GetPlayerID() == m_LeaderID && m_PartyPlayerByUID.size() > 0 )
		{
			m_LeaderID = 0;
			svrChk( SelectNewLeader(bIsSilent) );
		}

	Proc_End:

		return hr;
	}

	// Pending close transaction
	Result GamePartyEntity::PendingCloseTransaction(const char* reason)
	{
		Result hr = ResultCode::SUCCESS;
		TransactionPtr pTrans;

		svrMem( pTrans = new(GetHeap()) PartyTransCloseInstance(GetHeap(), GetGameID()));

		svrTrace(SVR_INFO, "Closing party instance {0}, memberCount:{1}, reason:{2}", GetEntityUID(), m_PartyPlayerByUID.size(), reason);

		svrChk(PendingTransaction(GetTaskWorker()->GetThreadID(), pTrans));
		
	Proc_End:

		ReleaseTransaction(pTrans);
		//Util::SafeDelete(pTrans);

		return hr;
	}



}; // namespace Svr
}; // namespace SF


