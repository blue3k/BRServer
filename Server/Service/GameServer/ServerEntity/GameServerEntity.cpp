////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Remote Entity implementation
//	
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/StrUtil.h"
#include "Common/TimeUtil.h"
#include "Common/Trace.h"
#include "Common/Thread.h"
#include "ServerSystem/BrServer.h"
#include "ServerSystem/SvrConstDefault.h"
#include "ServerSystem/ServerEntity.h"
#include "ServerSystem/Transaction.h"
//#include "ServerSystem/PlugIn.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/Transaction/ServerTransactionGeneric.h"

#include "GameServerEntity.h"
#include "GameSvrConst.h"

#include "Protocol/Message/GameServerMsgClass.h"
#include "Protocol/Policy/GameServerIPolicy.h"
#include "Protocol/Message/GameInstanceMsgClass.h"
#include "Protocol/Policy/GameInstanceIPolicy.h"
#include "Protocol/Message/GamePartyMsgClass.h"
#include "Protocol/Policy/GamePartyIPolicy.h"
#include "Protocol/Message/PartyMatchingMsgClass.h"
#include "Protocol/Policy/PartyMatchingIPolicy.h"
#include "Protocol/Message/PartyMatchingQueueMsgClass.h"
#include "Protocol/Policy/PartyMatchingQueueIPolicy.h"

#include "Transaction/GamePlayerEntityTrans.h"
#include "Transaction/GamePlayerEntityTransClose.h"
#include "Transaction/GamePlayerEntityTransGame.h"
#include "Transaction/GamePlayerEntityTransParty.h"
#include "Transaction/GamePlayerEntityTransFriend.h"

#include "Transaction/GameServerEntityTrans.h"

#include "Protocol/Policy/GameIPolicy.h"



namespace BR {
namespace GameServer {


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Game server entity
	//

	GameServerEntity::GameServerEntity()
		:Svr::GameServerEntity( Const::ENTITY_GAME_TRANS_QUEUE, Const::ENTITY_GAME_TRANSRES_QUEUE )
	{
	}

	GameServerEntity::~GameServerEntity()
	{
	}

	
	// Initialize entity to proceed new connection
	HRESULT GameServerEntity::InitializeEntity( EntityID newEntityID )
	{
		HRESULT hr = S_OK;

		svrChk(Svr::GameServerEntity::InitializeEntity( newEntityID ) );

	Proc_End:

		return hr;
	}

	// Process Connection event
	HRESULT GameServerEntity::ProcessConnectionEvent( const BR::Net::IConnection::Event& conEvent )
	{
		HRESULT hr = S_OK;

		switch( conEvent.EventType )
		{
		case Net::IConnection::Event::EVT_CONNECTION_RESULT:
			if( SUCCEEDED(conEvent.Value.hr) && IsInitialConnection()
				&& GetPolicy<Policy::IPolicyGameServer>() )
			{
				svrChkPtr( Svr::BrServer::GetInstance() );

				//svrChk( GetPolicy()->ServerStartedC2SEvt( Svr::BrServer::GetInstance()->GetServerUID(), Svr::BrServer::GetInstance()->GetGameID() ) );
			}
			break;
		case Net::IConnection::Event::EVT_DISCONNECTED:

			break;
		case Net::IConnection::Event::EVT_STATE_CHANGE:

			break;
		};

	Proc_End:

		return Svr::GameServerEntity::ProcessConnectionEvent( conEvent );
	}

	// register message handlers
	HRESULT GameServerEntity::RegisterMessageHandlers()
	{
		HRESULT hr = S_OK;

		svrChk(Svr::GameServerEntity::RegisterMessageHandlers() );

		BR_ENTITY_MESSAGE( Message::Server::ServerConnectedC2SEvt)					{ svrMemReturn(pNewTrans = new Svr::GenericServerStartedTrans( pMsgData )); return S_OK; } );

		// player transaction
		BR_ENTITY_MESSAGE(Message::GameServer::RegisterPlayerToJoinGameServerCmd)	{ svrMemReturn( pNewTrans = new GameServerTransRegisterPlayerToJoinGameServer(pMsgData) ); return S_OK; } );

		BR_ENTITY_MESSAGE(Message::GameServer::RegisterPlayerToJoinGameServerOnPlayerEntityCmd)	{ svrMemReturn(pNewTrans = new PlayerTransRegisterPlayerToJoinGameServerOnPlayerEntity(pMsgData)); return S_OK; } );

//		BR_ENTITY_MESSAGE(Message::GameServer::KickPlayerCmd)						{ svrMemReturn( pNewTrans = new PlayerTransKicked(pMsgData) ); return S_OK; } );
		BR_ENTITY_MESSAGE(Message::GameServer::ChatMessageC2SEvt)					{ svrMemReturn( pNewTrans = new PlayerTransChatMessageFromOtherEntity(pMsgData) ); return S_OK; } );

		////////////////////////////////////////////////////////////////////////////////////////////////////
		// Game Player

		BR_ENTITY_MESSAGE(Message::GameServer::FriendAcceptedC2SEvt)				{ svrMemReturn( pNewTrans = new PlayerTransFriendAcceptedS2S(pMsgData) ); return S_OK; } );
		BR_ENTITY_MESSAGE(Message::GameServer::FriendRemovedC2SEvt)					{ svrMemReturn( pNewTrans = new PlayerTransFriendRemovedS2S(pMsgData) ); return S_OK; } );
		BR_ENTITY_MESSAGE(Message::GameServer::NotifyC2SEvt)						{ svrMemReturn( pNewTrans = new PlayerTransNotifyS2S(pMsgData) ); return S_OK; } );
		//BR_ENTITY_MESSAGE(Message::GameServer::GiveStaminaC2SEvt)					{ svrMemReturn( pNewTrans = new PlayerTransGiveStaminaS2S(pMsgData) ); return S_OK; } );

		BR_ENTITY_MESSAGE(Message::GameServer::RequestPlayerStatusUpdateC2SEvt)		{ svrMemReturn( pNewTrans = new PlayerTransRequestPlayerStatusUpdateS2S(pMsgData) ); return S_OK; } );
		BR_ENTITY_MESSAGE(Message::GameServer::NotifyPlayerStatusUpdatedC2SEvt)		{ svrMemReturn( pNewTrans = new PlayerTransNotifyPlayerStatusUpdatedS2S(pMsgData) ); return S_OK; } );

		BR_ENTITY_MESSAGE(Message::GameServer::NotifyPartyInviteC2SEvt)				{ svrMemReturn( pNewTrans = new PlayerTransNotifyPartyInviteS2SEvt(pMsgData) ); return S_OK; } );

		////////////////////////////////////////////////////////////////////////////////////////////////////
		// Game instance server to player

		BR_ENTITY_MESSAGE(Message::GameInstance::PlayerJoinedS2CEvt)				{ svrMemReturn(pNewTrans = new PlayerTransJoinedS2SEvt(pMsgData)); return S_OK; } );
		BR_ENTITY_MESSAGE(Message::GameInstance::PlayerLeftS2CEvt)					{ svrMemReturn(pNewTrans = new PlayerTransLeftS2SEvt(pMsgData)); return S_OK; } );
		BR_ENTITY_MESSAGE(Message::GameInstance::PlayerKickedS2CEvt)				{ svrMemReturn(pNewTrans = new PlayerTransPlayerKickedS2SEvt(pMsgData)); return S_OK; } );
		BR_ENTITY_MESSAGE(Message::GameInstance::RoleAssignedS2CEvt)				{ svrMemReturn(pNewTrans = new PlayerTransRoleAssignedS2SEvt(pMsgData)); return S_OK; } );
		BR_ENTITY_MESSAGE(Message::GameInstance::GameAdvanceVotedS2CEvt)			{ svrMemReturn(pNewTrans = new PlayerTransGameAdvanceVotedS2SEvt(pMsgData)); return S_OK; } );
		BR_ENTITY_MESSAGE(Message::GameInstance::VoteEndS2CEvt)						{ svrMemReturn(pNewTrans = new PlayerTransVoteEndS2SEvt(pMsgData)); return S_OK; } );
		BR_ENTITY_MESSAGE(Message::GameInstance::PlayerRevealedS2CEvt)				{ svrMemReturn(pNewTrans = new PlayerTransPlayerRevealedS2SEvt(pMsgData)); return S_OK; } );
		BR_ENTITY_MESSAGE(Message::GameInstance::GameAdvancedS2CEvt)				{ svrMemReturn(pNewTrans = new PlayerTransGameAdvancedS2SEvt(pMsgData)); return S_OK; });
		BR_ENTITY_MESSAGE(Message::GameInstance::GameEndedS2CEvt)					{ svrMemReturn(pNewTrans = new PlayerTransGameEndedS2SEvt(pMsgData)); return S_OK; } );
		BR_ENTITY_MESSAGE(Message::GameInstance::PlayerKilledS2CEvt)				{ svrMemReturn(pNewTrans = new PlayerTransPlayerKilledS2SEvt(pMsgData)); return S_OK; } );
		BR_ENTITY_MESSAGE(Message::GameInstance::VotedS2CEvt)						{ svrMemReturn(pNewTrans = new PlayerTransPlayerVotedS2SEvt(pMsgData)); 	return S_OK; } );
		BR_ENTITY_MESSAGE(Message::GameInstance::GamePlayAgainS2CEvt)				{ svrMemReturn(pNewTrans = new PlayerTransPlayAgainS2SEvt(pMsgData)); return S_OK; } );
		BR_ENTITY_MESSAGE(Message::GameInstance::GamePlayerRevivedS2CEvt)			{ svrMemReturn(pNewTrans = new PlayerTransGamePlayerRevivedS2SEvt(pMsgData)); return S_OK; } );

		//////////////////////////////////////////////////////////////////////////////////////////////////
		// PartyServer to player 
		BR_ENTITY_MESSAGE(Message::GameParty::PlayerJoinedS2CEvt)					{ svrMemReturn( pNewTrans = new PlayerTransPartyPlayerJoinedS2SEvt(pMsgData) ); return S_OK; } );
		BR_ENTITY_MESSAGE(Message::GameParty::PartyLeaderChangedS2CEvt)				{ svrMemReturn( pNewTrans = new PlayerTransPartyLeaderChangedS2SEvt(pMsgData) ); return S_OK; } );
		BR_ENTITY_MESSAGE(Message::GameParty::PlayerLeftS2CEvt)						{ svrMemReturn( pNewTrans = new PlayerTransPartyPlayerLeftS2SEvt(pMsgData) ); return S_OK; } );
		BR_ENTITY_MESSAGE(Message::GameParty::PlayerKickedS2CEvt)					{ svrMemReturn( pNewTrans = new PlayerTransPartyPlayerKickedS2SEvt(pMsgData) ); return S_OK; } );
		BR_ENTITY_MESSAGE(Message::GameParty::ChatMessageS2CEvt)					{ svrMemReturn( pNewTrans = new PlayerTransPartyChatMessageS2CEvt(pMsgData) ); return S_OK; } );
		BR_ENTITY_MESSAGE(Message::GameParty::QuickChatMessageS2CEvt)				{ svrMemReturn( pNewTrans = new PlayerTransPartyQuickChatMessageS2CEvt(pMsgData) ); return S_OK; } );
		BR_ENTITY_MESSAGE(Message::GameParty::QueuedGameMatchingS2CEvt)				{ svrMemReturn( pNewTrans = new PlayerTransPartyQueuedGameMatchingS2CEvt(pMsgData) ); return S_OK; } );
		BR_ENTITY_MESSAGE(Message::GameParty::CanceledGameMatchingS2CEvt)			{ svrMemReturn( pNewTrans = new PlayerTransPartyCanceledGameMatchingS2CEvt(pMsgData) ); return S_OK; } );
		BR_ENTITY_MESSAGE(Message::GameParty::MatchingItemDequeuedS2CEvt)			{ svrMemReturn( pNewTrans = new PlayerTransPartyMatchingItemDequeuedS2CEvt(pMsgData) ); return S_OK; } );

		////////////////////////////////////////////////////////////////////////////////////////////////////
		// GameMatching to player
		BR_ENTITY_MESSAGE(Message::PartyMatching::PlayerGameMatchedS2CEvt)					{ svrMemReturn( pNewTrans = new PlayerTransGameMatchedS2SEvt(pMsgData) ); return S_OK; } );
		BR_ENTITY_MESSAGE(Message::PartyMatchingQueue::PlayerMatchingCanceledS2CEvt)		{ svrMemReturn( pNewTrans = new PlayerTransPlayerMatchingCanceledS2CEvt(pMsgData) ); return S_OK; } );
		BR_ENTITY_MESSAGE(Message::PartyMatchingQueue::PlayerMatchingItemDequeuedS2CEvt)	{ svrMemReturn( pNewTrans = new PlayerTransPlayerMatchingItemDequeuedS2CEvt(pMsgData) ); return S_OK; } );



	Proc_End:

		return hr;
	}


}; // namespace GameServer
}; // namespace BR



