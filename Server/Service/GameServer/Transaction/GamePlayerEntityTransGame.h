////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : game player entity implementation
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "SFTypedefs.h"
#include "ServerSystem/Transaction.h"
#include "Memory/MemoryPool.h"
#include "Common/ArrayUtil.h"
#include "Types/BrBaseTypes.h"
#include "Common/GameConst.h"
#include "Net/Message.h"

#include "Protocol/Message/GameMsgClass.h"
#include "Protocol/Policy/GameIPolicy.h"
#include "Protocol/Message/GameInstanceMsgClass.h"
#include "Protocol/Policy/GameInstanceIPolicy.h"
#include "Protocol/Message/PartyMatchingQueueMsgClass.h"
#include "Protocol/Policy/PartyMatchingQueueIPolicy.h"

#include "ServerSystem/MessageRoute.h"

#include "GameServerClass.h"
#include "GameInstance/GamePlayerEntity.h"
//#include "PlayerInfoManager.h"


namespace BR {
namespace GameServer {


	/////////////////////////////////////////////////////////////////////////////
	//
	//	Game command transaction
	//


	class PlayerTransJoinGame : public Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::JoinGameCmd, PlayerTransJoinGame>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::JoinGameCmd, PlayerTransJoinGame> super;

	private:
		GameInsUID m_GameInsID;
		uint32_t m_TimeStamp;
		GameStateID m_GameState;
		UINT8 m_Day;
		UINT8 m_MaxPlayer;
		UINT8 m_PlayerIndex;
		UINT8 m_PlayerCharacter;
		PlayerRole m_Role;
		bool m_Dead;

		LinkedArray<uint8_t> m_ChatHistoryData;
		LinkedArray<uint8_t> m_GameLogData;

	public:
		PlayerTransJoinGame( Message::MessageData* &pIMsg );
		virtual ~PlayerTransJoinGame() {}

		Result OnJoinGameRes( Svr::TransactionResult* &pRes );
		Result OnLeavePartyRes(Svr::TransactionResult* &pRes);

		// Start Transaction
		virtual Result StartTransaction();

		virtual Result CloseTransaction(Result hrRes) override;

		BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(JoinGameRes,m_GameInsID, m_TimeStamp, m_GameState, m_Day, m_MaxPlayer,
			m_PlayerIndex, m_PlayerCharacter, m_Role, m_Dead, m_ChatHistoryData, m_GameLogData );
	};


	// Chat message from other entitity	
	class PlayerTransJoinedS2SEvt : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::ISvrPolicyGame, Message::GameInstance::PlayerJoinedS2CEvt, PlayerTransJoinedS2SEvt>
	{
	public:
		typedef Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::ISvrPolicyGame, Message::GameInstance::PlayerJoinedS2CEvt, PlayerTransJoinedS2SEvt> super;

	public:
		PlayerTransJoinedS2SEvt( Message::MessageData* &pIMsg )  :UserTransactionS2SEvt( pIMsg ) {}
		virtual ~PlayerTransJoinedS2SEvt() {}

		// Start Transaction
		virtual Result StartTransaction();
	};

	
	class PlayerTransLeaveGame : public Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::LeaveGameCmd, PlayerTransLeaveGame>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::LeaveGameCmd, PlayerTransLeaveGame> super;

	private:

	public:
		PlayerTransLeaveGame( Message::MessageData* &pIMsg );//  :MessageTransaction( pIMsg ) {}
		virtual ~PlayerTransLeaveGame() {}

		Result OnLeaveGameRes( Svr::TransactionResult* & pRes );

		// Start Transaction
		virtual Result StartTransaction();

		virtual Result CloseTransaction(Result hrRes) override;

		BR_IMPLEMENT_USERMSGTRANS_CLOSE(LeaveGameRes);
	};


	// Chat message from other entitity	
	class PlayerTransLeftS2SEvt : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::ISvrPolicyGame, Message::GameInstance::PlayerLeftS2CEvt, PlayerTransLeftS2SEvt>
	{
	public:
		typedef Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::ISvrPolicyGame, Message::GameInstance::PlayerLeftS2CEvt, PlayerTransLeftS2SEvt> super;

	public:
		PlayerTransLeftS2SEvt( Message::MessageData* &pIMsg )  :UserTransactionS2SEvt( pIMsg ) {}
		virtual ~PlayerTransLeftS2SEvt() {}

		// Start Transaction
		virtual Result StartTransaction();
	};


	class PlayerTransKickPlayer : public Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::KickPlayerCmd, PlayerTransLeaveGame>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::KickPlayerCmd, PlayerTransLeaveGame> super;

	private:

	public:
		PlayerTransKickPlayer( Message::MessageData* &pIMsg );
		virtual ~PlayerTransKickPlayer() {}

		Result OnKickPlayerRes( Svr::TransactionResult* & pRes );

		// Start Transaction
		virtual Result StartTransaction();

		virtual Result CloseTransaction(Result hrRes) override;

		BR_IMPLEMENT_USERMSGTRANS_CLOSE(KickPlayerRes);
	};
	
	// game end event from game instance server
	class PlayerTransPlayerKickedS2SEvt : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::ISvrPolicyGame, Message::GameInstance::PlayerKickedS2CEvt, PlayerTransPlayerKickedS2SEvt>
	{
	public:
		typedef Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::ISvrPolicyGame, Message::GameInstance::PlayerKickedS2CEvt, PlayerTransPlayerKickedS2SEvt> super;

	public:
		PlayerTransPlayerKickedS2SEvt( Message::MessageData* &pIMsg )  :UserTransactionS2SEvt( pIMsg ) {}
		virtual ~PlayerTransPlayerKickedS2SEvt() {}

		// Start Transaction
		virtual Result StartTransaction();
	};

	
	class PlayerTransAssignRole : public Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::AssignRoleCmd, PlayerTransAssignRole>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::AssignRoleCmd, PlayerTransAssignRole> super;

	private:

	public:
		PlayerTransAssignRole( Message::MessageData* &pIMsg );//  :MessageTransaction( pIMsg ) {}
		virtual ~PlayerTransAssignRole() {}

		Result OnAssignRoleRes( Svr::TransactionResult* &pRes );

		// Start Transaction
		virtual Result StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE(AssignRoleRes);
	};

	
	// game advanced event from game instance server
	class PlayerTransRoleAssignedS2SEvt : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::ISvrPolicyGame, Message::GameInstance::RoleAssignedS2CEvt, PlayerTransRoleAssignedS2SEvt>
	{
	public:
		typedef Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::ISvrPolicyGame, Message::GameInstance::RoleAssignedS2CEvt, PlayerTransRoleAssignedS2SEvt> super;

	public:
		PlayerTransRoleAssignedS2SEvt( Message::MessageData* &pIMsg )  :UserTransactionS2SEvt( pIMsg ) {}
		virtual ~PlayerTransRoleAssignedS2SEvt() {}

		// Start Transaction
		virtual Result StartTransaction();
	};

	
	
	class PlayerTransChatMessage : public Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::ChatMessageCmd, PlayerTransChatMessage>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::ChatMessageCmd, PlayerTransChatMessage> super;

	public:
		PlayerTransChatMessage( Message::MessageData* &pIMsg )  :MessageTransaction( pIMsg ) {}
		virtual ~PlayerTransChatMessage() {}

		// Start Transaction
		virtual Result StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE(ChatMessageRes);
	};

	// Chat message from other entitity	
	class PlayerTransChatMessageFromOtherEntity : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::ISvrPolicyGame, Message::GameServer::ChatMessageC2SEvt, PlayerTransChatMessageFromOtherEntity>
	{
	public:
		typedef Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::ISvrPolicyGame, Message::GameServer::ChatMessageC2SEvt, PlayerTransChatMessageFromOtherEntity> super;

	public:
		PlayerTransChatMessageFromOtherEntity( Message::MessageData* &pIMsg )  :UserTransactionS2SEvt( pIMsg ) {}
		virtual ~PlayerTransChatMessageFromOtherEntity() {}

		// Start Transaction
		virtual Result StartTransaction();
	};

	

	class PlayerTransVoteGameAdvance : public Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::VoteGameAdvanceCmd, PlayerTransVoteGameAdvance>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::VoteGameAdvanceCmd, PlayerTransVoteGameAdvance> super;

	private:

	public:
		PlayerTransVoteGameAdvance( Message::MessageData* &pIMsg );
		virtual ~PlayerTransVoteGameAdvance() {}

		Result OnVoteRes( Svr::TransactionResult* &pRes );

		// Start Transaction
		virtual Result StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE(VoteGameAdvanceRes);
	};
	
	// game end event from game instance server
	class PlayerTransGameAdvanceVotedS2SEvt : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::ISvrPolicyGame, Message::GameInstance::GameAdvanceVotedS2CEvt, PlayerTransGameAdvanceVotedS2SEvt>
	{
	public:
		typedef Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::ISvrPolicyGame, Message::GameInstance::GameAdvanceVotedS2CEvt, PlayerTransGameAdvanceVotedS2SEvt> super;

	public:
		PlayerTransGameAdvanceVotedS2SEvt( Message::MessageData* &pIMsg )  :UserTransactionS2SEvt( pIMsg ) {}
		virtual ~PlayerTransGameAdvanceVotedS2SEvt() {}

		// Start Transaction
		virtual Result StartTransaction();
	};


	class PlayerTransVote : public Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::VoteCmd, PlayerTransVote>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::VoteCmd, PlayerTransVote> super;

	private:

	public:
		PlayerTransVote( Message::MessageData* &pIMsg );
		virtual ~PlayerTransVote() {}

		Result OnVoteRes( Svr::TransactionResult* &pRes );

		// Start Transaction
		virtual Result StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE(VoteRes);
	};
	
	// game end event from game instance server
	class PlayerTransVoteEndS2SEvt : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::ISvrPolicyGame, Message::GameInstance::VoteEndS2CEvt, PlayerTransVoteEndS2SEvt>
	{
	public:
		typedef Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::ISvrPolicyGame, Message::GameInstance::VoteEndS2CEvt, PlayerTransVoteEndS2SEvt> super;

	public:
		PlayerTransVoteEndS2SEvt( Message::MessageData* &pIMsg )  :UserTransactionS2SEvt( pIMsg ) {}
		virtual ~PlayerTransVoteEndS2SEvt() {}

		// Start Transaction
		virtual Result StartTransaction();
	};


	// game end event from game instance server
	class PlayerTransPlayerRevealedS2SEvt : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::ISvrPolicyGame, Message::GameInstance::PlayerRevealedS2CEvt, PlayerTransPlayerRevealedS2SEvt>
	{
	public:
		typedef Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::ISvrPolicyGame, Message::GameInstance::PlayerRevealedS2CEvt, PlayerTransPlayerRevealedS2SEvt> super;

	public:
		PlayerTransPlayerRevealedS2SEvt( Message::MessageData* &pIMsg )  :UserTransactionS2SEvt( pIMsg ) {}
		virtual ~PlayerTransPlayerRevealedS2SEvt() {}

		// Start Transaction
		virtual Result StartTransaction();
	};


	
	class PlayerTransAdvanceGame : public Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::AdvanceGameCmd, PlayerTransAdvanceGame>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::AdvanceGameCmd, PlayerTransAdvanceGame> super;

	public:
		PlayerTransAdvanceGame( Message::MessageData* &pIMsg );
		virtual ~PlayerTransAdvanceGame() {}

		Result OnAdvanceGameRes( Svr::TransactionResult* &pRes );

		// Start Transaction
		virtual Result StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE(AdvanceGameRes);
	};
	
	// game advanced event from game instance server
	class PlayerTransGameAdvancedS2SEvt : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::ISvrPolicyGame, Message::GameInstance::GameAdvancedS2CEvt, PlayerTransGameAdvancedS2SEvt>
	{
	public:
		typedef Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::ISvrPolicyGame, Message::GameInstance::GameAdvancedS2CEvt, PlayerTransGameAdvancedS2SEvt> super;

	public:
		PlayerTransGameAdvancedS2SEvt( Message::MessageData* &pIMsg )  :UserTransactionS2SEvt( pIMsg ) {}
		virtual ~PlayerTransGameAdvancedS2SEvt() {}

		// Start Transaction
		virtual Result StartTransaction();
	};

	// game end event from game instance server
	class PlayerTransGameEndedS2SEvt : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::ISvrPolicyGame, Message::GameInstance::GameEndedS2CEvt, PlayerTransGameEndedS2SEvt>
	{
	public:
		typedef Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::ISvrPolicyGame, Message::GameInstance::GameEndedS2CEvt, PlayerTransGameEndedS2SEvt> super;

	public:
		PlayerTransGameEndedS2SEvt( Message::MessageData* &pIMsg );//  :UserTransactionS2SEvt( pIMsg ) {}
		virtual ~PlayerTransGameEndedS2SEvt() {}

		Result OnUpdateDBRes(  Svr::TransactionResult* &pRes );

		// Start Transaction
		virtual Result StartTransaction();
	};
	
	// Game player killed event
	class PlayerTransPlayerKilledS2SEvt : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::ISvrPolicyGame, Message::GameInstance::PlayerKilledS2CEvt, PlayerTransPlayerKilledS2SEvt>
	{
	public:
		typedef Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::ISvrPolicyGame, Message::GameInstance::PlayerKilledS2CEvt, PlayerTransPlayerKilledS2SEvt> super;

	public:
		PlayerTransPlayerKilledS2SEvt( Message::MessageData* &pIMsg )  :UserTransactionS2SEvt( pIMsg ) {}
		virtual ~PlayerTransPlayerKilledS2SEvt() {}

		// Start Transaction
		virtual Result StartTransaction();
	};

	
	// Player voted event
	class PlayerTransPlayerVotedS2SEvt : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::ISvrPolicyGame, Message::GameInstance::VotedS2CEvt, PlayerTransPlayerVotedS2SEvt>
	{
	public:
		typedef Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::ISvrPolicyGame, Message::GameInstance::VotedS2CEvt, PlayerTransPlayerVotedS2SEvt> super;

	public:
		PlayerTransPlayerVotedS2SEvt( Message::MessageData* &pIMsg )  :UserTransactionS2SEvt( pIMsg ) {}
		virtual ~PlayerTransPlayerVotedS2SEvt() {}

		// Start Transaction
		virtual Result StartTransaction();
	};


	
	
	class PlayerTransRequestGameMatch : public Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::RequestGameMatchCmd, PlayerTransRequestGameMatch>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::RequestGameMatchCmd, PlayerTransRequestGameMatch> super;

	private:
		uint64_t m_TotalGem;
		uint64_t m_TotalGameMoney;

	public:
		PlayerTransRequestGameMatch( Message::MessageData* &pIMsg );
		virtual ~PlayerTransRequestGameMatch() {}

		Result OnRequestPartyMatchingRes( Svr::TransactionResult* &pRes );
		Result OnRequestPlayerMatchingRes( Svr::TransactionResult* &pRes );

		// Start Transaction
		virtual Result StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(RequestGameMatchRes, m_TotalGem, m_TotalGameMoney);
	};
	
	class PlayerTransCancelGameMatch : public Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::CancelGameMatchCmd, PlayerTransCancelGameMatch>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::CancelGameMatchCmd, PlayerTransCancelGameMatch> super;

	public:
		PlayerTransCancelGameMatch( Message::MessageData* &pIMsg );
		virtual ~PlayerTransCancelGameMatch() {}

		Result OnCancelPartyMatchingRes( Svr::TransactionResult* &pRes );
		Result OnCancelPlayerMatchingRes( Svr::TransactionResult* &pRes );

		// Start Transaction
		virtual Result StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE(CancelGameMatchRes);
	};
	


	//////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Matching queue
	//
	class PlayerTransPlayerMatchingCanceledS2CEvt : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::ISvrPolicyGame, Message::PartyMatchingQueue::PlayerMatchingCanceledS2CEvt, PlayerTransPlayerMatchingCanceledS2CEvt>
	{
	public:
		typedef Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::ISvrPolicyGame, Message::PartyMatchingQueue::PlayerMatchingCanceledS2CEvt, PlayerTransPlayerMatchingCanceledS2CEvt> super;

	public:
		PlayerTransPlayerMatchingCanceledS2CEvt( Message::MessageData* &pIMsg )  :UserTransactionS2SEvt( pIMsg ) {}
		virtual ~PlayerTransPlayerMatchingCanceledS2CEvt() {}

		// Start Transaction
		virtual Result StartTransaction();
	};



	class PlayerTransPlayerMatchingItemDequeuedS2CEvt : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::ISvrPolicyGame, Message::PartyMatchingQueue::PlayerMatchingItemDequeuedS2CEvt, PlayerTransPlayerMatchingItemDequeuedS2CEvt>
	{
	public:
		typedef Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::ISvrPolicyGame, Message::PartyMatchingQueue::PlayerMatchingItemDequeuedS2CEvt, PlayerTransPlayerMatchingItemDequeuedS2CEvt> super;

	public:
		PlayerTransPlayerMatchingItemDequeuedS2CEvt( Message::MessageData* &pIMsg )  :UserTransactionS2SEvt( pIMsg ) {}
		virtual ~PlayerTransPlayerMatchingItemDequeuedS2CEvt() {}

		// Start Transaction
		virtual Result StartTransaction();
	};


	class PlayerTransPlayAgain : public Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::GamePlayAgainCmd, PlayerTransPlayAgain>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::GamePlayAgainCmd, PlayerTransPlayAgain> super;

	private:
		uint64_t m_TotalGem;
		uint64_t m_TotalGameMoney;

	public:
		PlayerTransPlayAgain(Message::MessageData* &pIMsg);
		virtual ~PlayerTransPlayAgain() {}

		Result OnPlayAgainRes(Svr::TransactionResult* &pRes);
		Result OnCreatePartyRes(Svr::TransactionResult* &pRes);

		Result RequestPlayAgain();
		Result RequestCreateParty();

		// Start Transaction
		virtual Result StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(GamePlayAgainRes, m_TotalGem, m_TotalGameMoney);
	};


	// Player voted event
	class PlayerTransPlayAgainS2SEvt : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::ISvrPolicyGame, Message::GameInstance::GamePlayAgainS2CEvt, PlayerTransPlayAgainS2SEvt>
	{
	public:
		typedef Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::ISvrPolicyGame, Message::GameInstance::GamePlayAgainS2CEvt, PlayerTransPlayAgainS2SEvt> super;

	private:
		PartyUID m_PartyUID;
		PlayerID m_LeadPlayer;

	public:
		PlayerTransPlayAgainS2SEvt(Message::MessageData* &pIMsg);// : UserTransactionS2SEvt(pIMsg) {}
		virtual ~PlayerTransPlayAgainS2SEvt() {}

		Result OnJoinPartyRes(Svr::TransactionResult* &pRes);

		// Start Transaction
		virtual Result StartTransaction();

		//BR_IMPLEMENT_EVTTRANS_CLOSE_ARGS(GamePlayAgainS2CEvt, m_PartyUID, m_LeadPlayer);

	};



	class PlayerTransGameRevealPlayer : public Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::GameRevealPlayerCmd, PlayerTransGameRevealPlayer>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::GameRevealPlayerCmd, PlayerTransGameRevealPlayer> super;

	private:

		StaticArray<PlayerID,4> m_RevealedPlayerID;
		StaticArray<PlayerRole,4> m_RevealedPlayerRole;
		uint64_t m_TotalGem;
		uint64_t m_TotalGameMoney;

	public:
		PlayerTransGameRevealPlayer(Message::MessageData* &pIMsg);
		virtual ~PlayerTransGameRevealPlayer() {}

		Result OnUpdatePlayerRes(Svr::TransactionResult* &pRes);
		Result OnGameRevealPlayerRes(Svr::TransactionResult* &pRes);

		// Start Transaction
		virtual Result StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(GameRevealPlayerRes, m_RevealedPlayerID, m_RevealedPlayerRole, m_TotalGem, m_TotalGameMoney);
	};




	class PlayerTransGamePlayerRevive : public Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::GamePlayerReviveCmd, PlayerTransGamePlayerRevive>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::GamePlayerReviveCmd, PlayerTransGamePlayerRevive> super;

	private:
		uint64_t m_TotalGem;
		uint64_t m_TotalGameMoney;

	public:
		PlayerTransGamePlayerRevive(Message::MessageData* &pIMsg);
		virtual ~PlayerTransGamePlayerRevive() {}

		Result OnUpdatePlayerRes(Svr::TransactionResult* &pRes);
		Result OnGamePlayerReviveRes(Svr::TransactionResult* &pRes);

		// Start Transaction
		virtual Result StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(GamePlayerReviveRes, m_TotalGem, m_TotalGameMoney);
	};



	// Player voted event
	class PlayerTransGamePlayerRevivedS2SEvt : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::ISvrPolicyGame, Message::GameInstance::GamePlayerRevivedS2CEvt, PlayerTransGamePlayerRevivedS2SEvt>
	{
	public:
		typedef Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::ISvrPolicyGame, Message::GameInstance::GamePlayerRevivedS2CEvt, PlayerTransGamePlayerRevivedS2SEvt> super;

	public:
		PlayerTransGamePlayerRevivedS2SEvt(Message::MessageData* &pIMsg) : UserTransactionS2SEvt(pIMsg) {}
		virtual ~PlayerTransGamePlayerRevivedS2SEvt() {}

		// Start Transaction
		virtual Result StartTransaction();
	};



	class PlayerTransGamePlayerResetRank : public Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::GamePlayerResetRankCmd, PlayerTransGamePlayerResetRank>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::GamePlayerResetRankCmd, PlayerTransGamePlayerResetRank> super;

	private:
		uint64_t m_TotalGem;
		uint64_t m_TotalGameMoney;

	public:
		PlayerTransGamePlayerResetRank(Message::MessageData* &pIMsg) : MessageTransaction(pIMsg) {}
		virtual ~PlayerTransGamePlayerResetRank() {}

		// Start Transaction
		virtual Result StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(GamePlayerResetRankRes, m_TotalGem, m_TotalGameMoney);
	};


} // namespace GameServer 
} // namespace BR 

