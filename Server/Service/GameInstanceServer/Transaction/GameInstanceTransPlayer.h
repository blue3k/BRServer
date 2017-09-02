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

#include "SFTypedefs.h"
#include "Transaction/Transaction.h"
#include "Memory/MemoryPool.h"
#include "Types/SFEngineTypedefs.h"
#include "Common/MemoryStream.h"
#include "Protocol/Message/GameMsgClass.h"
#include "Protocol/Policy/GameNetPolicy.h"
#include "Protocol/Message/GameInstanceMsgClass.h"
#include "Protocol/Policy/GameInstanceNetPolicy.h"

#include "Transaction/MessageRoute.h"
#include "ServerSystem/ServerTransaction.h"

#include "ConspiracyGameInstanceServerClass.h"
#include "Transaction/GameInstanceTransaction.h"

#include "GameInstance/GameInstanceEntity.h"
#include "Net/Message.h"


namespace SF {
namespace ConspiracyGameInstanceServer {


	class GameEntityTransDeleteGame : public Svr::ServerEntityMessageTransaction< GameInstanceEntity, Message::GameInstance::DeleteGameC2SEvt, GameEntityTransDeleteGame, 1>
	{
	public:
		typedef Svr::ServerEntityMessageTransaction< GameInstanceEntity, Message::GameInstance::DeleteGameC2SEvt, GameEntityTransDeleteGame, 1> super;

	private:

	public:
		GameEntityTransDeleteGame(MessageDataPtr &pIMsg);// : ServerEntityMessageTransaction(pIMsg) {}
		virtual ~GameEntityTransDeleteGame() {}

		// Start Transaction
		virtual Result StartTransaction();
	};


	class GameEntityTransJoinGame : public RoutedGamePlayerMessageTransaction< GameInstanceEntity, Policy::NetSvrPolicyGameInstance, Message::GameInstance::JoinGameCmd, GameEntityTransJoinGame>
	{
	public:
		typedef RoutedGamePlayerMessageTransaction< GameInstanceEntity, Policy::NetSvrPolicyGameInstance, Message::GameInstance::JoinGameCmd, GameEntityTransJoinGame> super;

	private:

		NetAddress m_Addr;
		TimeStampSec m_TimeStamp;
		GameInsUID m_GameInsUID;
		GameStateID m_GameState;
		UINT8 m_Day;
		PlayerRole m_Role;
		bool m_Dead, m_bIsFirstJoin;
		uint m_PlayerIndex;
		uint m_PlayerCharacter;

		StaticOutputMemoryStream<GameConst::MAX_CHATLOG_BUFFER> m_ChatHistoryBuffer;
		StaticOutputMemoryStream<GameConst::MAX_GAMELOG_BUFFER> m_GameLogBuffer;


	public:
		GameEntityTransJoinGame( MessageDataPtr &pIMsg )  :RoutedGamePlayerMessageTransaction( pIMsg ) {}
		virtual ~GameEntityTransJoinGame() {}

		// Start Transaction
		virtual Result StartTransaction();
/*
		virtual Result OnCloseTransaction(Result hrRes) override
		{
			Result hr = ResultCode::SUCCESS; 
			auto pPolicy = GetInterface(); 
			if (pPolicy != nullptr) {
				
				svrChk(pPolicy->JoinGameRes(RouteContext(m_GameInsUID, GetRouteContext().GetFrom()), super::GetTransactionID(), hrRes,
					m_Addr, (uint32_t)(m_TimeStamp.time_since_epoch().count()), m_GameState, (uint8_t)m_Day,
					(uint8_t)GetMyOwner()->GetMaxPlayer(),
					(uint8_t)m_PlayerIndex, (uint8_t)m_PlayerCharacter, (PlayerRole)m_Role, (uint8_t)m_Dead, (uint8_t)m_bIsFirstJoin,
					m_ChatHistoryBuffer.ToArray(),
					m_GameLogBuffer.ToArray()));
			}
		Proc_End:
			super::OnCloseTransaction(hrRes);
			return hr;
		}
*/
		BR_SVR_MSGTRANS_CLOSE_ARGS( JoinGameRes, RouteContext( m_GameInsUID, GetRouteContext().GetFrom()), m_Addr, (uint32_t)(m_TimeStamp.time_since_epoch().count()), m_GameState, (uint8_t)m_Day,
			(uint8_t)GetMyOwner()->GetMaxPlayer(),
			(uint8_t)m_PlayerIndex, (uint8_t)m_PlayerCharacter, (PlayerRole)m_Role, (uint8_t)m_Dead, (uint8_t)m_bIsFirstJoin,
			m_ChatHistoryBuffer.ToArray(),
			m_GameLogBuffer.ToArray());
	};


	class GameEntityTransLeaveGame : public RoutedGamePlayerMessageTransaction< GameInstanceEntity, Policy::NetSvrPolicyGameInstance, Message::GameInstance::LeaveGameCmd, GameEntityTransLeaveGame>
	{
	public:
		typedef RoutedGamePlayerMessageTransaction< GameInstanceEntity, Policy::NetSvrPolicyGameInstance, Message::GameInstance::LeaveGameCmd, GameEntityTransLeaveGame> super;

	private:

	public:
		GameEntityTransLeaveGame( MessageDataPtr &pIMsg )  :RoutedGamePlayerMessageTransaction( pIMsg ) {}
		virtual ~GameEntityTransLeaveGame() {}

		// Start Transaction
		virtual Result StartTransaction();

		BR_SVR_MSGTRANS_CLOSE(LeaveGameRes, GetRouteContext().GetSwaped());
	};

	class GameEntityTransKickPlayer : public RoutedGamePlayerMessageTransaction< GameInstanceEntity, Policy::NetSvrPolicyGameInstance, Message::GameInstance::KickPlayerCmd, GameEntityTransKickPlayer>
	{
	public:
		typedef RoutedGamePlayerMessageTransaction< GameInstanceEntity, Policy::NetSvrPolicyGameInstance, Message::GameInstance::KickPlayerCmd, GameEntityTransKickPlayer> super;

	private:

	public:
		GameEntityTransKickPlayer( MessageDataPtr &pIMsg )  :RoutedGamePlayerMessageTransaction( pIMsg ) {}
		virtual ~GameEntityTransKickPlayer() {}

		// Start Transaction
		virtual Result StartTransaction();

		BR_SVR_MSGTRANS_CLOSE(KickPlayerRes, GetRouteContext().GetSwaped());
	};

	

	class GameEntityTransAssignRole : public RoutedGamePlayerMessageTransaction< GameInstanceEntity, Policy::NetSvrPolicyGameInstance, Message::GameInstance::AssignRoleCmd, GameEntityTransAssignRole>
	{
	public:
		typedef RoutedGamePlayerMessageTransaction< GameInstanceEntity, Policy::NetSvrPolicyGameInstance, Message::GameInstance::AssignRoleCmd, GameEntityTransAssignRole> super;

	private:

	public:
		GameEntityTransAssignRole( MessageDataPtr &pIMsg )  :RoutedGamePlayerMessageTransaction( pIMsg ) {}
		virtual ~GameEntityTransAssignRole() {}

		// Start Transaction
		virtual Result StartTransaction();

		BR_SVR_MSGTRANS_CLOSE(AssignRoleRes, GetRouteContext().GetSwaped());
	};


	class GameEntityTransChatMessage : public RoutedGamePlayerMessageTransaction< GameInstanceEntity, Policy::NetSvrPolicyGameInstance, Message::GameInstance::ChatMessageC2SEvt, GameEntityTransChatMessage>
	{
	public:
		typedef RoutedGamePlayerMessageTransaction< GameInstanceEntity, Policy::NetSvrPolicyGameInstance, Message::GameInstance::ChatMessageC2SEvt, GameEntityTransChatMessage> super;

	private:

	public:
		GameEntityTransChatMessage( MessageDataPtr &pIMsg )  :RoutedGamePlayerMessageTransaction( pIMsg ) {}
		virtual ~GameEntityTransChatMessage() {}

		// Start Transaction
		virtual Result StartTransaction();

	};


	class GameEntityTransVoteGameAdvance : public RoutedGamePlayerMessageTransaction< GameInstanceEntity, Policy::NetSvrPolicyGameInstance, Message::GameInstance::VoteGameAdvanceCmd, GameEntityTransVoteGameAdvance>
	{
	public:
		typedef RoutedGamePlayerMessageTransaction< GameInstanceEntity, Policy::NetSvrPolicyGameInstance, Message::GameInstance::VoteGameAdvanceCmd, GameEntityTransVoteGameAdvance> super;

	private:

	public:
		GameEntityTransVoteGameAdvance( MessageDataPtr &pIMsg )  :RoutedGamePlayerMessageTransaction( pIMsg ) {}
		virtual ~GameEntityTransVoteGameAdvance() {}

		// Start Transaction
		virtual Result StartTransaction();

		BR_SVR_MSGTRANS_CLOSE(VoteGameAdvanceRes, GetRouteContext().GetSwaped());
	};



	class GameEntityTransVote : public RoutedGamePlayerMessageTransaction< GameInstanceEntity, Policy::NetSvrPolicyGameInstance, Message::GameInstance::VoteCmd, GameEntityTransVote>
	{
	public:
		typedef RoutedGamePlayerMessageTransaction< GameInstanceEntity, Policy::NetSvrPolicyGameInstance, Message::GameInstance::VoteCmd, GameEntityTransVote> super;

	private:

	public:
		GameEntityTransVote( MessageDataPtr &pIMsg )  :RoutedGamePlayerMessageTransaction( pIMsg ) {}
		virtual ~GameEntityTransVote() {}

		// Start Transaction
		virtual Result StartTransaction();

		BR_SVR_MSGTRANS_CLOSE(VoteRes, GetRouteContext().GetSwaped());
	};

	class GameEntityTransAdvanceGame : public RoutedGamePlayerMessageTransaction< GameInstanceEntity, Policy::NetSvrPolicyGameInstance, Message::GameInstance::AdvanceGameCmd, GameEntityTransAdvanceGame>
	{
	public:
		typedef RoutedGamePlayerMessageTransaction< GameInstanceEntity, Policy::NetSvrPolicyGameInstance, Message::GameInstance::AdvanceGameCmd, GameEntityTransAdvanceGame> super;

	private:

	public:
		GameEntityTransAdvanceGame( MessageDataPtr &pIMsg )  :RoutedGamePlayerMessageTransaction( pIMsg ) {}
		virtual ~GameEntityTransAdvanceGame() {}

		// Start Transaction
		virtual Result StartTransaction();

		BR_SVR_MSGTRANS_CLOSE(AdvanceGameRes, GetRouteContext().GetSwaped());
	};
	


	class GameEntityTransGamePlayAgain : public RoutedGamePlayerMessageTransaction< GameInstanceEntity, Policy::NetSvrPolicyGameInstance, Message::GameInstance::GamePlayAgainCmd, GameEntityTransGamePlayAgain>
	{
	public:
		typedef RoutedGamePlayerMessageTransaction< GameInstanceEntity, Policy::NetSvrPolicyGameInstance, Message::GameInstance::GamePlayAgainCmd, GameEntityTransGamePlayAgain> super;

	private:
		uint m_MemberCount;

	public:
		GameEntityTransGamePlayAgain(MessageDataPtr &pIMsg) :RoutedGamePlayerMessageTransaction(pIMsg) {}
		virtual ~GameEntityTransGamePlayAgain() {}

		// Start Transaction
		virtual Result StartTransaction();

		BR_SVR_MSGTRANS_CLOSE_ARGS(GamePlayAgainRes, GetRouteContext().GetSwaped(), m_MemberCount);
	};



	class GameEntityTransGameRevealPlayer : public RoutedGamePlayerMessageTransaction< GameInstanceEntity, Policy::NetSvrPolicyGameInstance, Message::GameInstance::GameRevealPlayerCmd, GameEntityTransGameRevealPlayer>
	{
	public:
		typedef RoutedGamePlayerMessageTransaction< GameInstanceEntity, Policy::NetSvrPolicyGameInstance, Message::GameInstance::GameRevealPlayerCmd, GameEntityTransGameRevealPlayer> super;

	private:
		StaticArray<PlayerID, 4> m_RevealedPlayerID;
		StaticArray<PlayerRole, 4> m_RevealedPlayerRole;

	public:
		GameEntityTransGameRevealPlayer(MessageDataPtr &pIMsg) :RoutedGamePlayerMessageTransaction(pIMsg) {}
		virtual ~GameEntityTransGameRevealPlayer() {}

		// Start Transaction
		virtual Result StartTransaction();

		BR_SVR_MSGTRANS_CLOSE_ARGS(GameRevealPlayerRes, GetRouteContext().GetSwaped(), m_RevealedPlayerID, m_RevealedPlayerRole);
	};



	class GameEntityTransGamePlayerRevive : public RoutedGamePlayerMessageTransaction< GameInstanceEntity, Policy::NetSvrPolicyGameInstance, Message::GameInstance::GamePlayerReviveCmd, GameEntityTransGamePlayerRevive>
	{
	public:
		typedef RoutedGamePlayerMessageTransaction< GameInstanceEntity, Policy::NetSvrPolicyGameInstance, Message::GameInstance::GamePlayerReviveCmd, GameEntityTransGamePlayerRevive> super;

	private:

	public:
		GameEntityTransGamePlayerRevive(MessageDataPtr &pIMsg) :RoutedGamePlayerMessageTransaction(pIMsg) {}
		virtual ~GameEntityTransGamePlayerRevive() {}

		// Start Transaction
		virtual Result StartTransaction();

		BR_SVR_MSGTRANS_CLOSE(GamePlayerReviveRes, GetRouteContext().GetSwaped());
	};



		

};// namespace ConspiracyGameInstanceServer 
};// namespace SF 

