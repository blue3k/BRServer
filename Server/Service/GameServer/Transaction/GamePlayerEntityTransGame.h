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

#include "Common/Typedefs.h"
#include "ServerSystem/Transaction.h"
#include "Common/MemoryPool.h"
#include "Common/ArrayUtil.h"
#include "Common/BRBaseTypes.h"
#include "Common/GameConst.h"
#include "Common/Message.h"

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
	private:
		GameInsUID m_GameInsID;
		UINT32 m_TimeStamp;
		GameStateID m_GameState;
		UINT8 m_Day;
		UINT m_MaxPlayer;
		UINT m_PlayerIndex;
		UINT m_PlayerCharacter;
		PlayerRole m_Role;
		bool m_Dead;

		LinkedArray<BYTE> m_ChatHistoryData;
		LinkedArray<BYTE> m_GameLogData;

	public:
		PlayerTransJoinGame( Message::MessageData* &pIMsg );
		virtual ~PlayerTransJoinGame() {}

		HRESULT OnJoinGameRes( Svr::TransactionResult* &pRes );
		HRESULT OnLeavePartyRes(Svr::TransactionResult* &pRes);

		// Start Transaction
		virtual HRESULT StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(JoinGameRes,m_GameInsID, m_TimeStamp, m_GameState, m_Day, m_MaxPlayer,
			m_PlayerIndex, m_PlayerCharacter, m_Role, m_Dead, m_ChatHistoryData, m_GameLogData );
	};


	// Chat message from other entitity	
	class PlayerTransJoinedS2SEvt : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::ISvrPolicyGame, Message::GameInstance::PlayerJoinedS2CEvt, PlayerTransJoinedS2SEvt>
	{
	public:
		PlayerTransJoinedS2SEvt( Message::MessageData* &pIMsg )  :UserTransactionS2SEvt( pIMsg ) {}
		virtual ~PlayerTransJoinedS2SEvt() {}

		// Start Transaction
		virtual HRESULT StartTransaction();
	};

	
	class PlayerTransLeaveGame : public Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::LeaveGameCmd, PlayerTransLeaveGame>
	{
	private:

	public:
		PlayerTransLeaveGame( Message::MessageData* &pIMsg );//  :MessageTransaction( pIMsg ) {}
		virtual ~PlayerTransLeaveGame() {}

		HRESULT OnLeaveGameRes( Svr::TransactionResult* & pRes );

		// Start Transaction
		virtual HRESULT StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE(LeaveGameRes);
	};


	// Chat message from other entitity	
	class PlayerTransLeftS2SEvt : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::ISvrPolicyGame, Message::GameInstance::PlayerLeftS2CEvt, PlayerTransLeftS2SEvt>
	{
	public:
		PlayerTransLeftS2SEvt( Message::MessageData* &pIMsg )  :UserTransactionS2SEvt( pIMsg ) {}
		virtual ~PlayerTransLeftS2SEvt() {}

		// Start Transaction
		virtual HRESULT StartTransaction();
	};


	class PlayerTransKickPlayer : public Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::KickPlayerCmd, PlayerTransLeaveGame>
	{
	private:

	public:
		PlayerTransKickPlayer( Message::MessageData* &pIMsg );
		virtual ~PlayerTransKickPlayer() {}

		HRESULT OnKickPlayerRes( Svr::TransactionResult* & pRes );

		// Start Transaction
		virtual HRESULT StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE(KickPlayerRes);
	};
	
	// game end event from game instance server
	class PlayerTransPlayerKickedS2SEvt : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::ISvrPolicyGame, Message::GameInstance::PlayerKickedS2CEvt, PlayerTransPlayerKickedS2SEvt>
	{
	public:
		PlayerTransPlayerKickedS2SEvt( Message::MessageData* &pIMsg )  :UserTransactionS2SEvt( pIMsg ) {}
		virtual ~PlayerTransPlayerKickedS2SEvt() {}

		// Start Transaction
		virtual HRESULT StartTransaction();
	};

	
	class PlayerTransAssignRole : public Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::AssignRoleCmd, PlayerTransAssignRole>
	{
	private:

	public:
		PlayerTransAssignRole( Message::MessageData* &pIMsg );//  :MessageTransaction( pIMsg ) {}
		virtual ~PlayerTransAssignRole() {}

		HRESULT OnAssignRoleRes( Svr::TransactionResult* &pRes );

		// Start Transaction
		virtual HRESULT StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE(AssignRoleRes);
	};

	
	// game advanced event from game instance server
	class PlayerTransRoleAssignedS2SEvt : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::ISvrPolicyGame, Message::GameInstance::RoleAssignedS2CEvt, PlayerTransRoleAssignedS2SEvt>
	{
	public:
		PlayerTransRoleAssignedS2SEvt( Message::MessageData* &pIMsg )  :UserTransactionS2SEvt( pIMsg ) {}
		virtual ~PlayerTransRoleAssignedS2SEvt() {}

		// Start Transaction
		virtual HRESULT StartTransaction();
	};

	
	
	class PlayerTransChatMessage : public Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::ChatMessageCmd, PlayerTransChatMessage>
	{
	private:


	public:
		PlayerTransChatMessage( Message::MessageData* &pIMsg )  :MessageTransaction( pIMsg ) {}
		virtual ~PlayerTransChatMessage() {}

		// Start Transaction
		virtual HRESULT StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE(ChatMessageRes);
	};

	// Chat message from other entitity	
	class PlayerTransChatMessageFromOtherEntity : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::ISvrPolicyGame, Message::GameServer::ChatMessageC2SEvt, PlayerTransChatMessageFromOtherEntity>
	{
	public:
		PlayerTransChatMessageFromOtherEntity( Message::MessageData* &pIMsg )  :UserTransactionS2SEvt( pIMsg ) {}
		virtual ~PlayerTransChatMessageFromOtherEntity() {}

		// Start Transaction
		virtual HRESULT StartTransaction();
	};

	

	class PlayerTransVoteGameAdvance : public Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::VoteGameAdvanceCmd, PlayerTransVoteGameAdvance>
	{
	private:

	public:
		PlayerTransVoteGameAdvance( Message::MessageData* &pIMsg );
		virtual ~PlayerTransVoteGameAdvance() {}

		HRESULT OnVoteRes( Svr::TransactionResult* &pRes );

		// Start Transaction
		virtual HRESULT StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE(VoteGameAdvanceRes);
	};
	
	// game end event from game instance server
	class PlayerTransGameAdvanceVotedS2SEvt : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::ISvrPolicyGame, Message::GameInstance::GameAdvanceVotedS2CEvt, PlayerTransGameAdvanceVotedS2SEvt>
	{
	public:
		PlayerTransGameAdvanceVotedS2SEvt( Message::MessageData* &pIMsg )  :UserTransactionS2SEvt( pIMsg ) {}
		virtual ~PlayerTransGameAdvanceVotedS2SEvt() {}

		// Start Transaction
		virtual HRESULT StartTransaction();
	};


	class PlayerTransVote : public Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::VoteCmd, PlayerTransVote>
	{
	private:

	public:
		PlayerTransVote( Message::MessageData* &pIMsg );
		virtual ~PlayerTransVote() {}

		HRESULT OnVoteRes( Svr::TransactionResult* &pRes );

		// Start Transaction
		virtual HRESULT StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE(VoteRes);
	};
	
	// game end event from game instance server
	class PlayerTransVoteEndS2SEvt : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::ISvrPolicyGame, Message::GameInstance::VoteEndS2CEvt, PlayerTransVoteEndS2SEvt>
	{
	public:
		PlayerTransVoteEndS2SEvt( Message::MessageData* &pIMsg )  :UserTransactionS2SEvt( pIMsg ) {}
		virtual ~PlayerTransVoteEndS2SEvt() {}

		// Start Transaction
		virtual HRESULT StartTransaction();
	};


	// game end event from game instance server
	class PlayerTransPlayerRevealedS2SEvt : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::ISvrPolicyGame, Message::GameInstance::PlayerRevealedS2CEvt, PlayerTransPlayerRevealedS2SEvt>
	{
	public:
		PlayerTransPlayerRevealedS2SEvt( Message::MessageData* &pIMsg )  :UserTransactionS2SEvt( pIMsg ) {}
		virtual ~PlayerTransPlayerRevealedS2SEvt() {}

		// Start Transaction
		virtual HRESULT StartTransaction();
	};


	
	class PlayerTransAdvanceGame : public Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::AdvanceGameCmd, PlayerTransAdvanceGame>
	{
	private:

	public:
		PlayerTransAdvanceGame( Message::MessageData* &pIMsg );
		virtual ~PlayerTransAdvanceGame() {}

		HRESULT OnAdvanceGameRes( Svr::TransactionResult* &pRes );

		// Start Transaction
		virtual HRESULT StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE(AdvanceGameRes);
	};
	
	// game advanced event from game instance server
	class PlayerTransGameAdvancedS2SEvt : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::ISvrPolicyGame, Message::GameInstance::GameAdvancedS2CEvt, PlayerTransGameAdvancedS2SEvt>
	{
	public:
		PlayerTransGameAdvancedS2SEvt( Message::MessageData* &pIMsg )  :UserTransactionS2SEvt( pIMsg ) {}
		virtual ~PlayerTransGameAdvancedS2SEvt() {}

		// Start Transaction
		virtual HRESULT StartTransaction();
	};

	// game end event from game instance server
	class PlayerTransGameEndedS2SEvt : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::ISvrPolicyGame, Message::GameInstance::GameEndedS2CEvt, PlayerTransGameEndedS2SEvt>
	{
	public:
		PlayerTransGameEndedS2SEvt( Message::MessageData* &pIMsg );//  :UserTransactionS2SEvt( pIMsg ) {}
		virtual ~PlayerTransGameEndedS2SEvt() {}

		HRESULT OnUpdateDBRes(  Svr::TransactionResult* &pRes );

		// Start Transaction
		virtual HRESULT StartTransaction();
	};
	
	// Game player killed event
	class PlayerTransPlayerKilledS2SEvt : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::ISvrPolicyGame, Message::GameInstance::PlayerKilledS2CEvt, PlayerTransPlayerKilledS2SEvt>
	{
	public:
		PlayerTransPlayerKilledS2SEvt( Message::MessageData* &pIMsg )  :UserTransactionS2SEvt( pIMsg ) {}
		virtual ~PlayerTransPlayerKilledS2SEvt() {}

		// Start Transaction
		virtual HRESULT StartTransaction();
	};

	
	// Player voted event
	class PlayerTransPlayerVotedS2SEvt : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::ISvrPolicyGame, Message::GameInstance::VotedS2CEvt, PlayerTransPlayerVotedS2SEvt>
	{
	public:
		PlayerTransPlayerVotedS2SEvt( Message::MessageData* &pIMsg )  :UserTransactionS2SEvt( pIMsg ) {}
		virtual ~PlayerTransPlayerVotedS2SEvt() {}

		// Start Transaction
		virtual HRESULT StartTransaction();
	};


	
	
	class PlayerTransRequestGameMatch : public Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::RequestGameMatchCmd, PlayerTransRequestGameMatch>
	{
	private:
		UINT64 m_TotalGem;
		UINT64 m_TotalGameMoney;

	public:
		PlayerTransRequestGameMatch( Message::MessageData* &pIMsg );
		virtual ~PlayerTransRequestGameMatch() {}

		HRESULT OnRequestPartyMatchingRes( Svr::TransactionResult* &pRes );
		HRESULT OnRequestPlayerMatchingRes( Svr::TransactionResult* &pRes );

		// Start Transaction
		virtual HRESULT StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(RequestGameMatchRes, m_TotalGem, m_TotalGameMoney);
	};
	
	class PlayerTransCancelGameMatch : public Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::CancelGameMatchCmd, PlayerTransCancelGameMatch>
	{
	private:

	public:
		PlayerTransCancelGameMatch( Message::MessageData* &pIMsg );
		virtual ~PlayerTransCancelGameMatch() {}

		HRESULT OnCancelPartyMatchingRes( Svr::TransactionResult* &pRes );
		HRESULT OnCancelPlayerMatchingRes( Svr::TransactionResult* &pRes );

		// Start Transaction
		virtual HRESULT StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE(CancelGameMatchRes);
	};
	


	//////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Matching queue
	//
	class PlayerTransPlayerMatchingCanceledS2CEvt : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::ISvrPolicyGame, Message::PartyMatchingQueue::PlayerMatchingCanceledS2CEvt, PlayerTransPlayerMatchingCanceledS2CEvt>
	{
	public:
		PlayerTransPlayerMatchingCanceledS2CEvt( Message::MessageData* &pIMsg )  :UserTransactionS2SEvt( pIMsg ) {}
		virtual ~PlayerTransPlayerMatchingCanceledS2CEvt() {}

		// Start Transaction
		virtual HRESULT StartTransaction();
	};



	class PlayerTransPlayerMatchingItemDequeuedS2CEvt : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::ISvrPolicyGame, Message::PartyMatchingQueue::PlayerMatchingItemDequeuedS2CEvt, PlayerTransPlayerMatchingItemDequeuedS2CEvt>
	{
	public:
		PlayerTransPlayerMatchingItemDequeuedS2CEvt( Message::MessageData* &pIMsg )  :UserTransactionS2SEvt( pIMsg ) {}
		virtual ~PlayerTransPlayerMatchingItemDequeuedS2CEvt() {}

		// Start Transaction
		virtual HRESULT StartTransaction();
	};


	class PlayerTransPlayAgain : public Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::GamePlayAgainCmd, PlayerTransPlayAgain>
	{
	private:
		UINT64 m_TotalGem;
		UINT64 m_TotalGameMoney;

	public:
		PlayerTransPlayAgain(Message::MessageData* &pIMsg);
		virtual ~PlayerTransPlayAgain() {}

		HRESULT OnPlayAgainRes(Svr::TransactionResult* &pRes);
		HRESULT OnCreatePartyRes(Svr::TransactionResult* &pRes);

		HRESULT RequestPlayAgain();
		HRESULT RequestCreateParty();

		// Start Transaction
		virtual HRESULT StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(GamePlayAgainRes, m_TotalGem, m_TotalGameMoney);
	};


	// Player voted event
	class PlayerTransPlayAgainS2SEvt : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::ISvrPolicyGame, Message::GameInstance::GamePlayAgainS2CEvt, PlayerTransPlayAgainS2SEvt>
	{
	private:
		PartyUID m_PartyUID;
		PlayerID m_LeadPlayer;

	public:
		PlayerTransPlayAgainS2SEvt(Message::MessageData* &pIMsg);// : UserTransactionS2SEvt(pIMsg) {}
		virtual ~PlayerTransPlayAgainS2SEvt() {}

		HRESULT OnJoinPartyRes(Svr::TransactionResult* &pRes);

		// Start Transaction
		virtual HRESULT StartTransaction();

		//BR_IMPLEMENT_EVTTRANS_CLOSE_ARGS(GamePlayAgainS2CEvt, m_PartyUID, m_LeadPlayer);

	};



	class PlayerTransGameRevealPlayer : public Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::GameRevealPlayerCmd, PlayerTransGameRevealPlayer>
	{
	private:

		StaticArray<PlayerID,4> m_RevealedPlayerID;
		StaticArray<PlayerRole,4> m_RevealedPlayerRole;
		UINT64 m_TotalGem;
		UINT64 m_TotalGameMoney;

	public:
		PlayerTransGameRevealPlayer(Message::MessageData* &pIMsg);
		virtual ~PlayerTransGameRevealPlayer() {}

		HRESULT OnUpdatePlayerRes(Svr::TransactionResult* &pRes);
		HRESULT OnGameRevealPlayerRes(Svr::TransactionResult* &pRes);

		// Start Transaction
		virtual HRESULT StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(GameRevealPlayerRes, m_RevealedPlayerID, m_RevealedPlayerRole, m_TotalGem, m_TotalGameMoney);
	};




	class PlayerTransGamePlayerRevive : public Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::GamePlayerReviveCmd, PlayerTransGamePlayerRevive>
	{
	private:
		UINT64 m_TotalGem;
		UINT64 m_TotalGameMoney;

	public:
		PlayerTransGamePlayerRevive(Message::MessageData* &pIMsg);
		virtual ~PlayerTransGamePlayerRevive() {}

		HRESULT OnUpdatePlayerRes(Svr::TransactionResult* &pRes);
		HRESULT OnGamePlayerReviveRes(Svr::TransactionResult* &pRes);

		// Start Transaction
		virtual HRESULT StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(GamePlayerReviveRes, m_TotalGem, m_TotalGameMoney);
	};



	// Player voted event
	class PlayerTransGamePlayerRevivedS2SEvt : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::ISvrPolicyGame, Message::GameInstance::GamePlayerRevivedS2CEvt, PlayerTransGamePlayerRevivedS2SEvt>
	{
	private:

	public:
		PlayerTransGamePlayerRevivedS2SEvt(Message::MessageData* &pIMsg) : UserTransactionS2SEvt(pIMsg) {}
		virtual ~PlayerTransGamePlayerRevivedS2SEvt() {}

		// Start Transaction
		virtual HRESULT StartTransaction();
	};



	class PlayerTransGamePlayerResetRank : public Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::GamePlayerResetRankCmd, PlayerTransGamePlayerResetRank>
	{
	private:
		UINT64 m_TotalGem;
		UINT64 m_TotalGameMoney;

	public:
		PlayerTransGamePlayerResetRank(Message::MessageData* &pIMsg) : MessageTransaction(pIMsg) {}
		virtual ~PlayerTransGamePlayerResetRank() {}

		// Start Transaction
		virtual HRESULT StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(GamePlayerResetRankRes, m_TotalGem, m_TotalGameMoney);
	};


} // namespace GameServer 
} // namespace BR 

