////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : game player implementation
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Common/Typedefs.h"
#include "ServerSystem/Transaction.h"
#include "Common/MemoryPool.h"
#include "Common/BRBaseTypes.h"
#include "Common/MemoryStream.h"
#include "Protocol/Message/GameMsgClass.h"
#include "Protocol/Policy/GameIPolicy.h"
#include "Protocol/Message/GameInstanceMsgClass.h"
#include "Protocol/Policy/GameInstanceIPolicy.h"

#include "ServerSystem/MessageRoute.h"
#include "ServerSystem/ServerTransaction.h"

#include "ConspiracyGameInstanceServerClass.h"
#include "Transaction/GameInstanceTransaction.h"

#include "GameInstance/GameInstanceEntity.h"
#include "Common/Message.h"


namespace BR {
namespace ConspiracyGameInstanceServer {


	class GameEntityTransDeleteGame : public Svr::ServerEntityMessageTransaction< GameInstanceEntity, Message::GameInstance::DeleteGameC2SEvt, GameEntityTransDeleteGame, 1>
	{
	private:

	public:
		GameEntityTransDeleteGame(Message::MessageData* &pIMsg);// : ServerEntityMessageTransaction(pIMsg) {}
		virtual ~GameEntityTransDeleteGame() {}

		// Start Transaction
		virtual HRESULT StartTransaction();
	};


	class GameEntityTransJoinGame : public RoutedGamePlayerMessageTransaction< GameInstanceEntity, Policy::ISvrPolicyGameInstance, Message::GameInstance::JoinGameCmd, GameEntityTransJoinGame>
	{
	private:

		NetAddress m_Addr;
		TimeStampSec m_TimeStamp;
		GameInsUID m_GameInsUID;
		GameStateID m_GameState;
		UINT8 m_Day;
		PlayerRole m_Role;
		bool m_Dead, m_bIsFirstJoin;
		UINT m_PlayerIndex;
		UINT m_PlayerCharacter;

		StaticOutputMemoryStream<GameConst::MAX_CHATLOG_BUFFER> m_ChatHistoryBuffer;
		StaticOutputMemoryStream<GameConst::MAX_GAMELOG_BUFFER> m_GameLogBuffer;


	public:
		GameEntityTransJoinGame( Message::MessageData* &pIMsg )  :RoutedGamePlayerMessageTransaction( pIMsg ) {}
		virtual ~GameEntityTransJoinGame() {}

		// Start Transaction
		virtual HRESULT StartTransaction();

		BR_IMPLEMENT_MSGTRANS_CLOSE_ARGS( JoinGameRes, RouteContext( m_GameInsUID, GetRouteContext().From ), m_Addr, m_TimeStamp.time_since_epoch().count(), m_GameState, m_Day,
			GetMyOwner()->GetMaxPlayer(),
			m_PlayerIndex, m_PlayerCharacter, m_Role, m_Dead, m_bIsFirstJoin, 
			m_ChatHistoryBuffer.ToArray(),
			m_GameLogBuffer.ToArray());
	};


	class GameEntityTransLeaveGame : public RoutedGamePlayerMessageTransaction< GameInstanceEntity, Policy::ISvrPolicyGameInstance, Message::GameInstance::LeaveGameCmd, GameEntityTransLeaveGame>
	{
	private:

	public:
		GameEntityTransLeaveGame( Message::MessageData* &pIMsg )  :RoutedGamePlayerMessageTransaction( pIMsg ) {}
		virtual ~GameEntityTransLeaveGame() {}

		// Start Transaction
		virtual HRESULT StartTransaction();

		BR_IMPLEMENT_MSGTRANS_CLOSE_ARGS(LeaveGameRes, GetRouteContext().GetSwaped());
	};

	class GameEntityTransKickPlayer : public RoutedGamePlayerMessageTransaction< GameInstanceEntity, Policy::ISvrPolicyGameInstance, Message::GameInstance::KickPlayerCmd, GameEntityTransKickPlayer>
	{
	private:

	public:
		GameEntityTransKickPlayer( Message::MessageData* &pIMsg )  :RoutedGamePlayerMessageTransaction( pIMsg ) {}
		virtual ~GameEntityTransKickPlayer() {}

		// Start Transaction
		virtual HRESULT StartTransaction();

		BR_IMPLEMENT_MSGTRANS_CLOSE_ARGS(KickPlayerRes, GetRouteContext().GetSwaped());
	};

	

	class GameEntityTransAssignRole : public RoutedGamePlayerMessageTransaction< GameInstanceEntity, Policy::ISvrPolicyGameInstance, Message::GameInstance::AssignRoleCmd, GameEntityTransAssignRole>
	{
	private:

	public:
		GameEntityTransAssignRole( Message::MessageData* &pIMsg )  :RoutedGamePlayerMessageTransaction( pIMsg ) {}
		virtual ~GameEntityTransAssignRole() {}

		// Start Transaction
		virtual HRESULT StartTransaction();

		BR_IMPLEMENT_MSGTRANS_CLOSE_ARGS(AssignRoleRes, GetRouteContext().GetSwaped());
	};


	class GameEntityTransChatMessage : public RoutedGamePlayerMessageTransaction< GameInstanceEntity, Policy::ISvrPolicyGameInstance, Message::GameInstance::ChatMessageC2SEvt, GameEntityTransChatMessage>
	{
	private:

	public:
		GameEntityTransChatMessage( Message::MessageData* &pIMsg )  :RoutedGamePlayerMessageTransaction( pIMsg ) {}
		virtual ~GameEntityTransChatMessage() {}

		// Start Transaction
		virtual HRESULT StartTransaction();

	};


	class GameEntityTransVoteGameAdvance : public RoutedGamePlayerMessageTransaction< GameInstanceEntity, Policy::ISvrPolicyGameInstance, Message::GameInstance::VoteGameAdvanceCmd, GameEntityTransVoteGameAdvance>
	{
	private:

	public:
		GameEntityTransVoteGameAdvance( Message::MessageData* &pIMsg )  :RoutedGamePlayerMessageTransaction( pIMsg ) {}
		virtual ~GameEntityTransVoteGameAdvance() {}

		// Start Transaction
		virtual HRESULT StartTransaction();

		BR_IMPLEMENT_MSGTRANS_CLOSE_ARGS(VoteGameAdvanceRes, GetRouteContext().GetSwaped());
	};



	class GameEntityTransVote : public RoutedGamePlayerMessageTransaction< GameInstanceEntity, Policy::ISvrPolicyGameInstance, Message::GameInstance::VoteCmd, GameEntityTransVote>
	{
	private:

	public:
		GameEntityTransVote( Message::MessageData* &pIMsg )  :RoutedGamePlayerMessageTransaction( pIMsg ) {}
		virtual ~GameEntityTransVote() {}

		// Start Transaction
		virtual HRESULT StartTransaction();

		BR_IMPLEMENT_MSGTRANS_CLOSE_ARGS(VoteRes, GetRouteContext().GetSwaped());
	};

	class GameEntityTransAdvanceGame : public RoutedGamePlayerMessageTransaction< GameInstanceEntity, Policy::ISvrPolicyGameInstance, Message::GameInstance::AdvanceGameCmd, GameEntityTransAdvanceGame>
	{
	private:

	public:
		GameEntityTransAdvanceGame( Message::MessageData* &pIMsg )  :RoutedGamePlayerMessageTransaction( pIMsg ) {}
		virtual ~GameEntityTransAdvanceGame() {}

		// Start Transaction
		virtual HRESULT StartTransaction();

		BR_IMPLEMENT_MSGTRANS_CLOSE_ARGS(AdvanceGameRes, GetRouteContext().GetSwaped());
	};
	


	class GameEntityTransGamePlayAgain : public RoutedGamePlayerMessageTransaction< GameInstanceEntity, Policy::ISvrPolicyGameInstance, Message::GameInstance::GamePlayAgainCmd, GameEntityTransGamePlayAgain>
	{
	private:
		UINT m_MemberCount;

	public:
		GameEntityTransGamePlayAgain(Message::MessageData* &pIMsg) :RoutedGamePlayerMessageTransaction(pIMsg) {}
		virtual ~GameEntityTransGamePlayAgain() {}

		// Start Transaction
		virtual HRESULT StartTransaction();

		BR_IMPLEMENT_MSGTRANS_CLOSE_ARGS(GamePlayAgainRes, GetRouteContext().GetSwaped(), m_MemberCount);
	};



	class GameEntityTransGameRevealPlayer : public RoutedGamePlayerMessageTransaction< GameInstanceEntity, Policy::ISvrPolicyGameInstance, Message::GameInstance::GameRevealPlayerCmd, GameEntityTransGameRevealPlayer>
	{
	private:
		StaticArray<PlayerID, 4> m_RevealedPlayerID;
		StaticArray<PlayerRole, 4> m_RevealedPlayerRole;

	public:
		GameEntityTransGameRevealPlayer(Message::MessageData* &pIMsg) :RoutedGamePlayerMessageTransaction(pIMsg) {}
		virtual ~GameEntityTransGameRevealPlayer() {}

		// Start Transaction
		virtual HRESULT StartTransaction();

		BR_IMPLEMENT_MSGTRANS_CLOSE_ARGS(GameRevealPlayerRes, GetRouteContext().GetSwaped(), m_RevealedPlayerID, m_RevealedPlayerRole);
	};



	class GameEntityTransGamePlayerRevive : public RoutedGamePlayerMessageTransaction< GameInstanceEntity, Policy::ISvrPolicyGameInstance, Message::GameInstance::GamePlayerReviveCmd, GameEntityTransGamePlayerRevive>
	{
	private:

	public:
		GameEntityTransGamePlayerRevive(Message::MessageData* &pIMsg) :RoutedGamePlayerMessageTransaction(pIMsg) {}
		virtual ~GameEntityTransGamePlayerRevive() {}

		// Start Transaction
		virtual HRESULT StartTransaction();

		BR_IMPLEMENT_MSGTRANS_CLOSE_ARGS(GamePlayerReviveRes, GetRouteContext().GetSwaped());
	};



		

};// namespace ConspiracyGameInstanceServer 
};// namespace BR 

