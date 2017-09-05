////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Game Party entity
//
////////////////////////////////////////////////////////////////////////////////



#include "stdafx.h"
#include "SvrConst.h"
#include "SvrTrace.h"
#include "Util/TimeUtil.h"
#include "Types/SFEngineTypedefs.h"
#include "ResultCode/SFResultCodeLibrary.h"
#include "ResultCode/SFResultCodeGame.h"
#include "Net/Message.h"

#include "Protocol/Policy/GameServerNetPolicy.h"
#include "Protocol/Policy/GamePartyNetPolicy.h"

#include "Transaction/MessageRoute.h"
#include "ServiceEntity/GamePartyEntity.h"

#include "ServiceEntity/GamePartyEntityTrans.h"


SF_MEMORYPOOL_IMPLEMENT(SF::Svr::PartyPlayer);
SF_MEMORYPOOL_IMPLEMENT(SF::Svr::GamePartyEntity);


namespace SF {
namespace Svr {


	//////////////////////////////////////////////////////////////////////////
	//
	//	Entity server class
	//


	GamePartyEntity::GamePartyEntity()
		:MasterEntity(64,64)
		,m_LeaderID(0)
	{
		SetTickInterval(Const::PARTY_TICKTASK_INTERVAL);


		// Game party instance transactions
		BR_ENTITY_MESSAGE(Message::GameParty::JoinPartyCmd)								{ svrMemReturn(pNewTrans = new(GetMemoryManager()) PartyTransJoinParty(pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::GameParty::LeavePartyCmd)							{ svrMemReturn(pNewTrans = new(GetMemoryManager()) PartyTransLeaveParty(pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::GameParty::KickPlayerCmd)							{ svrMemReturn(pNewTrans = new(GetMemoryManager()) PartyTransKickPlayer(pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::GameParty::ChatMessageC2SEvt)						{ svrMemReturn(pNewTrans = new(GetMemoryManager()) PartyTransChatMessage(pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::GameParty::QuickChatMessageC2SEvt)					{ svrMemReturn(pNewTrans = new(GetMemoryManager()) PartyTransQuickChatMessage(pMsgData)); return ResultCode::SUCCESS; } );

		BR_ENTITY_MESSAGE(Message::GameParty::StartGameMatchCmd)						{ svrMemReturn(pNewTrans = new(GetMemoryManager()) PartyTransStartGameMatchCmd(pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::GameParty::CancelGameMatchCmd)						{ svrMemReturn(pNewTrans = new(GetMemoryManager()) PartyTransCancelGameMatchCmd(pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::PartyMatchingQueue::PartyMatchingCanceledS2CEvt)		{ svrMemReturn(pNewTrans = new(GetMemoryManager()) PartyTransPartyMatchingCanceled(pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::PartyMatchingQueue::PartyMatchingItemDequeuedS2CEvt) { svrMemReturn(pNewTrans = new(GetMemoryManager()) PartyTransMatchingItemDequeued(pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::PartyMatching::PartyGameMatchedS2CEvt)				{ svrMemReturn(pNewTrans = new(GetMemoryManager()) PartyTransPartyGameMatchedS2CEvt(pMsgData)); return ResultCode::SUCCESS; } );
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
		PartyPlayerUIDMap::iterator itPlayer;

		m_PartyPlayerByUID.begin( itPlayer );

		for( ; itPlayer.IsValid(); ++ itPlayer )
		{
			PartyPlayer* pPartyPlayer = *itPlayer;
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
	Result GamePartyEntity::ForeachPlayerGameServer( std::function<Result(PartyPlayer* pPlayer, Policy::IPolicyGameServer *pPolicy)> func )
	{
		for( auto itPlayer = m_PartyPlayerByUID.begin(); itPlayer != m_PartyPlayerByUID.end(); ++ itPlayer )
		{
			PartyPlayer* pPartyPlayer = *itPlayer;
			if( pPartyPlayer == nullptr )
				continue;

			Policy::IPolicyGameServer *pPolicy = pPartyPlayer->GetInterface<Policy::IPolicyGameServer>();
			if( pPolicy )
			{
				Result hrRes = func( pPartyPlayer, pPolicy );
				if( !(hrRes) )
					return hrRes;
			}
		}
		return ResultCode::SUCCESS;
	}

	Result GamePartyEntity::ForeachPlayerSvrGameParty( std::function<Result(PartyPlayer* pPlayer, Policy::NetSvrPolicyGameParty *pPolicy)> func )
	{
		for( auto itPlayer = m_PartyPlayerByUID.begin(); itPlayer != m_PartyPlayerByUID.end(); ++ itPlayer )
		{
			PartyPlayer* pPartyPlayer = *itPlayer;
			if( pPartyPlayer == nullptr )
				continue;

			Policy::NetSvrPolicyGameParty *pPolicy = pPartyPlayer->GetInterface<Policy::NetSvrPolicyGameParty>();
			if( pPolicy )
			{
				Result hrRes = func( pPartyPlayer, pPolicy );
				if( !(hrRes) )
					return hrRes;
			}
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
			Policy::NetSvrPolicyGameParty* pJoindPolicy = pPlayer->GetInterface<Policy::NetSvrPolicyGameParty>();
			svrChkPtr(pJoindPolicy);

			ForeachPlayerSvrGameParty( [&]( PartyPlayer* pOtherPlayer, Policy::NetSvrPolicyGameParty *pOtherPolicy )->Result {
				if( pPlayer != pOtherPlayer )
				{
					// Send others to joined
					pJoindPolicy->PlayerJoinedS2CEvt( pPlayer->GetRouteContext(GetEntityUID()), pOtherPlayer->GetPlayerInformation() );
					// Send joined to others
					pOtherPolicy->PlayerJoinedS2CEvt( pOtherPlayer->GetRouteContext(GetEntityUID()), pPlayer->GetPlayerInformation() );
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
			ForeachPlayerSvrGameParty( [&]( PartyPlayer* pOtherPlayer, Policy::NetSvrPolicyGameParty *pPolicy )->Result {
				pPolicy->PlayerLeftS2CEvt( pOtherPlayer->GetRouteContext(GetEntityUID()), pPlayer->GetPlayerID() );
				return ResultCode::SUCCESS;
			});
		}

		svrChk( OnPlayerGetOutOfParty(pPlayer, bIsSilent) );

	Proc_End:

		Util::SafeDelete( pPlayer );

		if( m_PartyPlayerByUID.size() == 0 ) // if no player remain
		{
			PendingCloseTransaction();
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
				ForeachPlayerSvrGameParty( [&]( PartyPlayer* pOtherPlayer, Policy::NetSvrPolicyGameParty *pPolicy )->Result {
					pPolicy->PartyLeaderChangedS2CEvt( pOtherPlayer->GetRouteContext(GetEntityUID()), m_LeaderID );
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
		PartyPlayerUIDMap::iterator itPlayer;

		m_PartyPlayerByUID.begin(itPlayer);
		while( itPlayer.IsValid() )
		{
			PartyPlayer *pPlayer = *itPlayer;
			itPlayer = nullptr;

			LeavePlayer( pPlayer, true );

			m_PartyPlayerByUID.begin( itPlayer );
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
	Result GamePartyEntity::PendingCloseTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		Svr::Transaction *pTrans = nullptr;

		svrMem( pTrans = new(GetMemoryManager()) PartyTransCloseInstance(GetMemoryManager()));

		svrTrace(SVR_INFO, "Closing party instance {0}, memberCount:{1}", GetEntityUID(), m_PartyPlayerByUID.size());

		svrChk(PendingTransaction(GetTaskWorker()->GetThreadID(), pTrans));
		
	Proc_End:

		ReleaseTransaction(pTrans);
		//Util::SafeDelete(pTrans);

		return hr;
	}



}; // namespace Svr
}; // namespace SF


