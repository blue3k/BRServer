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
#include "Protocol/Policy/ChatChannelNetPolicy.h"

#include "Transaction/MessageRoute.h"
#include "ServiceEntity/Chat/ChatChannelEntity.h"

#include "ServiceEntity/Chat/ChatChannelEntityTrans.h"


SF_MEMORYPOOL_IMPLEMENT(SF::Svr::ChatChannelPlayer);
SF_MEMORYPOOL_IMPLEMENT(SF::Svr::ChatChannelEntity);


namespace SF {
namespace Svr {


	//////////////////////////////////////////////////////////////////////////
	//
	//	Entity server class
	//


	ChatChannelEntity::ChatChannelEntity(const FixedString& chatChannelName)
		: MasterEntity(64, 64)
		, m_ChatChannelPlayerByUID(GetHeap())
		, m_ChatChannelName(chatChannelName)
		, m_LeaderID(0)
	{
		SetTickInterval(Const::PARTY_TICKTASK_INTERVAL);


		// Game party instance transactions
		BR_ENTITY_MESSAGE(Message::ChatChannel::JoinCmd)					{ svrMemReturn(pNewTrans = new(GetHeap()) ChatChannelTransJoin(GetHeap(),pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::ChatChannel::LeaveCmd)					{ svrMemReturn(pNewTrans = new(GetHeap()) ChatChannelTransLeave(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::ChatChannel::KickPlayerCmd)				{ svrMemReturn(pNewTrans = new(GetHeap()) ChatChannelTransKickPlayer(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::ChatChannel::ChatMessageC2SEvt)			{ svrMemReturn(pNewTrans = new(GetHeap()) ChatChannelTransChatMessage(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
	}

	ChatChannelEntity::~ChatChannelEntity()
	{
	}


	void ChatChannelEntity::SetPasscode(const char* passCode)
	{
		StrUtil::StringCopy(m_PassCode, passCode);
	}

	// Initialize entity to proceed new connection
	Result ChatChannelEntity::InitializeEntity( EntityID newEntityID )
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(MasterEntity::InitializeEntity( newEntityID ) );

		m_ChatChannelPlayerByUID.clear();

		m_LeaderID = 0;

	Proc_End:	

		return hr;
	}

	// Close entity and clear transaction
	Result ChatChannelEntity::TerminateEntity()
	{
		Result hr = ResultCode::SUCCESS;

		if( GetEntityState() == EntityState::FREE )
			return ResultCode::SUCCESS;

		ForeachPlayer( [](ChatChannelPlayer* pPlayer)->Result {
			Util::SafeDelete(pPlayer);
			return ResultCode::SUCCESS;
		});
		m_ChatChannelPlayerByUID.clear();

		svrChk(MasterEntity::TerminateEntity() );

	Proc_End:	

		return hr;
	}

	
	//////////////////////////////////////////////////////////////////////////////////////////
	//
	//	foreach implementations
	//

	// foreach game player
	Result ChatChannelEntity::ForeachPlayer( std::function<Result(ChatChannelPlayer* pPlayer)> func )
	{
		for(auto itPlayer : m_ChatChannelPlayerByUID)
		{
			auto* pChatChannelPlayer = itPlayer;
			if( pChatChannelPlayer )
			{
				Result hrRes = func( pChatChannelPlayer );
				if( !(hrRes) )
					return hrRes;
			}
		}
		return ResultCode::SUCCESS;
	}

	// foreach game player with Game policy
	Result ChatChannelEntity::ForeachPlayerGameServer( std::function<Result(ChatChannelPlayer* pPlayer, Policy::NetPolicyGameServer &pPolicy)> func )
	{
		for( auto itPlayer = m_ChatChannelPlayerByUID.begin(); itPlayer != m_ChatChannelPlayerByUID.end(); ++ itPlayer )
		{
			ChatChannelPlayer* pChatChannelPlayer = *itPlayer;
			if( pChatChannelPlayer == nullptr )
				continue;

			Result hrRes = func( pChatChannelPlayer, Policy::NetPolicyGameServer(pChatChannelPlayer->GetConnection()));
			if( !(hrRes) )
				return hrRes;
		}
		return ResultCode::SUCCESS;
	}

	Result ChatChannelEntity::ForeachPlayerSvrChatChannel( std::function<Result(ChatChannelPlayer* pPlayer, Policy::NetSvrPolicyChatChannel &pPolicy)> func )
	{
		for(auto itPlayer : m_ChatChannelPlayerByUID)
		{
			if(itPlayer == nullptr )
				continue;

			Result hrRes = func(itPlayer, Policy::NetSvrPolicyChatChannel(itPlayer->GetConnection()));
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
	Result ChatChannelEntity::JoinPlayer( ChatChannelPlayer* &pPlayer, bool bIsSilent )
	{
		Result hr = ResultCode::SUCCESS;
		ChatChannelPlayerUIDMap::iterator itLeader;

		svrChkPtr( pPlayer );

		svrChk( m_ChatChannelPlayerByUID.insert(pPlayer->GetPlayerID(), pPlayer ));
		
		if( !bIsSilent )
		{
			Policy::NetSvrPolicyChatChannel pJoindPolicy(pPlayer->GetConnection());

			ForeachPlayerSvrChatChannel( [&]( ChatChannelPlayer* pOtherPlayer, Policy::NetSvrPolicyChatChannel &pOtherPolicy )->Result {
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
		if( m_LeaderID == 0 || !(m_ChatChannelPlayerByUID.find( m_LeaderID, itLeader )) )
		{
			m_LeaderID = 0;
			svrChk( SelectNewLeader(bIsSilent) );
		}

		pPlayer = nullptr;

	Proc_End:

		return hr;
	}


	// Player leave
	Result ChatChannelEntity::LeavePlayer( ChatChannelPlayer* &pPlayer, bool bIsSilent )
	{
		Result hr = ResultCode::SUCCESS;

		m_ChatChannelPlayerByUID.erase(pPlayer->GetPlayerID() );

		if( !bIsSilent )
		{
			ForeachPlayerSvrChatChannel( [&]( ChatChannelPlayer* pOtherPlayer, Policy::NetSvrPolicyChatChannel &pPolicy )->Result {
				pPolicy.PlayerLeftS2CEvt( pOtherPlayer->GetRouteContext(GetEntityUID()), pPlayer->GetPlayerID() );
				return ResultCode::SUCCESS;
			});
		}

		svrChk(OnPlayerGetOutOfChatChannel(pPlayer, bIsSilent) );

	Proc_End:

		Util::SafeDelete( pPlayer );

		if( m_ChatChannelPlayerByUID.size() == 0 ) // if no player remain
		{
			PendingCloseTransaction("Player left");
		}

		return hr;
	}

	// Select new leader
	Result ChatChannelEntity::SelectNewLeader( bool bIsSilent )
	{
		Result hr = ResultCode::SUCCESS;

		auto itPlayer = m_ChatChannelPlayerByUID.begin();
		if( itPlayer.IsValid() )
		{
			m_LeaderID = itPlayer->GetPlayerID();

			if( !bIsSilent )
			{
				ForeachPlayerSvrChatChannel( [&]( ChatChannelPlayer* pOtherPlayer, Policy::NetSvrPolicyChatChannel &pPolicy )->Result {
					pPolicy.LeaderChangedS2CEvt( pOtherPlayer->GetRouteContext(GetEntityUID()), m_LeaderID );
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
	Result ChatChannelEntity::LeaveAllPlayer()
	{
		auto itPlayer = m_ChatChannelPlayerByUID.begin();
		while(itPlayer.IsValid())
		{
			ChatChannelPlayer *pPlayer = *itPlayer;
			itPlayer = nullptr;

			LeavePlayer( pPlayer, true );

			// Leave player will remove player from the table
			itPlayer = m_ChatChannelPlayerByUID.begin();
		}

		m_ChatChannelPlayerByUID.clear();

		return ResultCode::SUCCESS;
	}

	// Find Player pilotid
	Result ChatChannelEntity::FindPlayer( PlayerID pltID, ChatChannelPlayer* &pChatChannelPlayer )
	{
		Result hr = ResultCode::SUCCESS;
		ChatChannelPlayerUIDMap::iterator itPlayer;

		if( !(m_ChatChannelPlayerByUID.find( pltID, itPlayer )) )
		{
			return ResultCode::SVR_PLAYER_NOT_FOUND;
		}

		pChatChannelPlayer = *itPlayer;

	//Proc_End:


		return hr;
	}
	
	
	// Called when a player get out of the party
	Result ChatChannelEntity::OnPlayerGetOutOfChatChannel( ChatChannelPlayer* pPlayer, bool bIsSilent )
	{
		Result hr = ResultCode::SUCCESS;

		svrChkPtr(pPlayer);

		// hire new leader if he was the leader and someone remain in this party
		if( pPlayer->GetPlayerID() == m_LeaderID && m_ChatChannelPlayerByUID.size() > 0 )
		{
			m_LeaderID = 0;
			svrChk( SelectNewLeader(bIsSilent) );
		}

	Proc_End:

		return hr;
	}

	// Pending close transaction
	Result ChatChannelEntity::PendingCloseTransaction(const char* reason)
	{
		Result hr = ResultCode::SUCCESS;
		TransactionPtr pTrans;

		svrMem( pTrans = new(GetHeap()) ChatChannelTransCloseInstance(GetHeap()));

		svrTrace(SVR_INFO, "Closing party instance {0}, memberCount:{1}, reason:{2}", GetEntityUID(), m_ChatChannelPlayerByUID.size(), reason);

		svrChk(PendingTransaction(GetTaskWorker()->GetThreadID(), pTrans));
		
	Proc_End:

		ReleaseTransaction(pTrans);
		//Util::SafeDelete(pTrans);

		return hr;
	}



}; // namespace Svr
}; // namespace SF


