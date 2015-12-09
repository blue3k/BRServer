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
#include "ServerSystem/SvrConst.h"
#include "ServerSystem/SvrTrace.h"
#include "Common/TimeUtil.h"
#include "Common/BrBaseTypes.h"
#include "Common/HRESCommon.h"
#include "Common/HRESGame.h"
#include "Common/Message.h"

#include "Protocol/Policy/GameServerIPolicy.h"
#include "Protocol/Policy/GamePartyIPolicy.h"

#include "ServerSystem/MessageRoute.h"
#include "ServerSystem/ServiceEntity/GamePartyEntity.h"

#include "ServerSystem/ServiceEntity/GamePartyEntityTrans.h"


BR_MEMORYPOOL_IMPLEMENT(Svr::PartyPlayer);
BR_MEMORYPOOL_IMPLEMENT(Svr::GamePartyEntity);


namespace BR {
namespace Svr {


	//////////////////////////////////////////////////////////////////////////
	//
	//	Entity server class
	//


	GamePartyEntity::GamePartyEntity()
		:MasterEntity(64,64)
		,m_LeaderID(0)
	{
		SetTickInterval(DurationMS(Const::PARTY_TICKTASK_INTERVAL));
	}

	GamePartyEntity::~GamePartyEntity()
	{
	}


	// Initialize entity to proceed new connection
	HRESULT GamePartyEntity::InitializeEntity( EntityID newEntityID )
	{
		HRESULT hr = S_OK;

		svrChk(MasterEntity::InitializeEntity( newEntityID ) );

		m_ChatHistory.InitializeLog();

		m_PartyPlayerByUID.clear();

		m_LeaderID = 0;
		m_MatchingTicket = 0;

	Proc_End:	

		return hr;
	}

	// Close entity and clear transaction
	HRESULT GamePartyEntity::TerminateEntity()
	{
		HRESULT hr = S_OK;

		if( GetEntityState() == EntityState::FREE )
			return S_OK;

		m_ChatHistory.TerminateLog();

		ForeachPlayer( [](PartyPlayer* pPlayer)->HRESULT {
			Util::SafeDelete(pPlayer);
			return S_OK;
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
	HRESULT GamePartyEntity::ForeachPlayer( std::function<HRESULT(PartyPlayer* pPlayer)> func )
	{
		PartyPlayerUIDMap::iterator itPlayer;

		m_PartyPlayerByUID.begin( itPlayer );

		for( ; itPlayer.IsValid(); ++ itPlayer )
		{
			PartyPlayer* pPartyPlayer = *itPlayer;
			if( pPartyPlayer )
			{
				HRESULT hrRes = func( pPartyPlayer );
				if( FAILED(hrRes) )
					return hrRes;
			}
		}
		return S_OK;
	}

	// foreach game player with Game policy
	HRESULT GamePartyEntity::ForeachPlayerGameServer( std::function<HRESULT(PartyPlayer* pPlayer, Policy::IPolicyGameServer *pPolicy)> func )
	{
		for( auto itPlayer = m_PartyPlayerByUID.begin(); itPlayer != m_PartyPlayerByUID.end(); ++ itPlayer )
		{
			PartyPlayer* pPartyPlayer = *itPlayer;
			if( pPartyPlayer == nullptr )
				continue;

			Policy::IPolicyGameServer *pPolicy = pPartyPlayer->GetPolicy<Policy::IPolicyGameServer>();
			if( pPolicy )
			{
				HRESULT hrRes = func( pPartyPlayer, pPolicy );
				if( FAILED(hrRes) )
					return hrRes;
			}
		}
		return S_OK;
	}

	HRESULT GamePartyEntity::ForeachPlayerSvrGameParty( std::function<HRESULT(PartyPlayer* pPlayer, Policy::ISvrPolicyGameParty *pPolicy)> func )
	{
		for( auto itPlayer = m_PartyPlayerByUID.begin(); itPlayer != m_PartyPlayerByUID.end(); ++ itPlayer )
		{
			PartyPlayer* pPartyPlayer = *itPlayer;
			if( pPartyPlayer == nullptr )
				continue;

			Policy::ISvrPolicyGameParty *pPolicy = pPartyPlayer->GetPolicy<Policy::ISvrPolicyGameParty>();
			if( pPolicy )
			{
				HRESULT hrRes = func( pPartyPlayer, pPolicy );
				if( FAILED(hrRes) )
					return hrRes;
			}
		}
		return S_OK;
	}
	



	//////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Game Player
	//

	// Register new player to join
	HRESULT GamePartyEntity::JoinPlayer( PartyPlayer* &pPlayer, bool bIsSilent )
	{
		HRESULT hr = S_OK;
		PartyPlayerUIDMap::iterator itLeader;

		svrChkPtr( pPlayer );

		svrChk( m_PartyPlayerByUID.insert(pPlayer->GetPlayerID(), pPlayer ));
		
		if( !bIsSilent )
		{
			Policy::ISvrPolicyGameParty* pJoindPolicy = pPlayer->GetPolicy<Policy::ISvrPolicyGameParty>();
			svrChkPtr(pJoindPolicy);

			ForeachPlayerSvrGameParty( [&]( PartyPlayer* pOtherPlayer, Policy::ISvrPolicyGameParty *pOtherPolicy )->HRESULT {
				if( pPlayer != pOtherPlayer )
				{
					// Send others to joined
					pJoindPolicy->PlayerJoinedS2CEvt( pPlayer->GetRouteContext(GetEntityUID()), pOtherPlayer->GetPlayerInformation() );
					// Send joined to others
					pOtherPolicy->PlayerJoinedS2CEvt( pOtherPlayer->GetRouteContext(GetEntityUID()), pPlayer->GetPlayerInformation() );
				}
				return S_OK;
			});

		}

		// We need a new leader
		if( m_LeaderID == 0 || FAILED(m_PartyPlayerByUID.find( m_LeaderID, itLeader )) )
		{
			m_LeaderID = 0;
			svrChk( SelectNewLeader(bIsSilent) );
		}

		pPlayer = nullptr;

	Proc_End:

		return hr;
	}


	// Player leave
	HRESULT GamePartyEntity::LeavePlayer( PartyPlayer* &pPlayer, bool bIsSilent )
	{
		HRESULT hr = S_OK;

		m_PartyPlayerByUID.erase(pPlayer->GetPlayerID() );

		if( !bIsSilent )
		{
			ForeachPlayerSvrGameParty( [&]( PartyPlayer* pOtherPlayer, Policy::ISvrPolicyGameParty *pPolicy )->HRESULT {
				pPolicy->PlayerLeftS2CEvt( pOtherPlayer->GetRouteContext(GetEntityUID()), pPlayer->GetPlayerID() );
				return S_OK;
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
	HRESULT GamePartyEntity::SelectNewLeader( bool bIsSilent )
	{
		HRESULT hr = S_OK;

		auto itPlayer = m_PartyPlayerByUID.begin();
		if( itPlayer.IsValid() )
		{
			m_LeaderID = itPlayer->GetPlayerID();

			if( !bIsSilent )
			{
				ForeachPlayerSvrGameParty( [&]( PartyPlayer* pOtherPlayer, Policy::ISvrPolicyGameParty *pPolicy )->HRESULT {
					pPolicy->PartyLeaderChangedS2CEvt( pOtherPlayer->GetRouteContext(GetEntityUID()), m_LeaderID );
					return S_OK;
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
	HRESULT GamePartyEntity::LeaveAllPlayer()
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

		return S_OK;
	}

	// Find Player pilotid
	HRESULT GamePartyEntity::FindPlayer( PlayerID pltID, PartyPlayer* &pPartyPlayer )
	{
		HRESULT hr = S_OK;
		PartyPlayerUIDMap::iterator itPlayer;

		if( FAILED(m_PartyPlayerByUID.find( pltID, itPlayer )) )
		{
			return E_PLAYER_NOT_FOUND;
		}

		pPartyPlayer = *itPlayer;

	//Proc_End:


		return hr;
	}
	
	
	// Called when a player get out of the party
	HRESULT GamePartyEntity::OnPlayerGetOutOfParty( PartyPlayer* pPlayer, bool bIsSilent )
	{
		HRESULT hr = S_OK;

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
	HRESULT GamePartyEntity::PendingCloseTransaction()
	{
		HRESULT hr = S_OK;
		Svr::Transaction *pTrans = nullptr;

		svrMem( pTrans = new PartyTransCloseInstance );

		svrTrace(Svr::TRC_INFO, "Closing party instance %0%, memberCount:%1%", GetEntityUID(), m_PartyPlayerByUID.size());

		svrChk(PendingTransaction(GetTaskWorker()->GetThreadID(), pTrans));
		
	Proc_End:

		ReleaseTransaction(pTrans);
		//Util::SafeDelete(pTrans);

		return hr;
	}



}; // namespace Svr
}; // namespace BR


