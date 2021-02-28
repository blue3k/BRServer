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
#include "Transaction/Transaction.h"
#include "MemoryManager/SFMemoryPool.h"
#include "Container/SFArray.h"
#include "Types/SFEngineTypedefs.h"
#include "GameConst.h"
#include "Net/SFMessage.h"

#include "Protocol/Message/GameMsgClass.h"
#include "Protocol/Policy/GameNetPolicy.h"
#include "Protocol/Message/GameInstanceMsgClass.h"
#include "Protocol/Policy/GameInstanceNetPolicy.h"
#include "Protocol/Message/PartyMatchingQueueMsgClass.h"
#include "Protocol/Policy/PartyMatchingQueueNetPolicy.h"
#include "Protocol/Message/PlayInstanceMsgClass.h"
#include "Protocol/Policy/PlayInstanceNetPolicy.h"

#include "Transaction/MessageRoute.h"

#include "GameServerClass.h"
#include "GameInstance/GamePlayerEntity.h"
//#include "PlayerInfoManager.h"


namespace SF {
namespace GameServer {


	/////////////////////////////////////////////////////////////////////////////
	//
	//	Game command transaction
	//


	class PlayerTransJoinGame : public Svr::MessageTransaction< GamePlayerEntity, Message::Game::JoinGameCmd>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Message::Game::JoinGameCmd> super;

	private:
		GameInsUID m_GameInsID;
		uint32_t m_TimeStamp;
		GameStateID m_GameState;
		uint8_t m_Day;
		uint8_t m_MaxPlayer;
		uint8_t m_PlayerIndex;
		uint8_t m_PlayerCharacter;
		PlayerRole m_Role;
		bool m_Dead;

		ArrayView<uint8_t> m_ChatHistoryData;
		ArrayView<uint8_t> m_GameLogData;

	public:
		PlayerTransJoinGame( IHeap& heap, MessageDataPtr &pIMsg );
		virtual ~PlayerTransJoinGame() {}

		Result OnJoinGameRes( Svr::TransactionResult* &pRes );
		Result OnLeavePartyRes(Svr::TransactionResult* &pRes);

		// Start Transaction
		virtual Result StartTransaction() override;

		virtual Result CloseTransaction(Result hrRes) override;

		BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(NetSvrPolicyGame, JoinGameRes,m_GameInsID, m_TimeStamp, (uint8_t)m_GameState, m_Day, m_MaxPlayer,
			m_PlayerIndex, m_PlayerCharacter, (uint8_t)m_Role, m_Dead, m_ChatHistoryData, m_GameLogData );
	};


	
	class PlayerTransLeaveGame : public Svr::MessageTransaction< GamePlayerEntity, Message::Game::LeaveGameCmd>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Message::Game::LeaveGameCmd> super;

	private:

	public:
		PlayerTransLeaveGame( IHeap& heap, MessageDataPtr &pIMsg );//  :MessageTransaction(heap,  pIMsg ) {}
		virtual ~PlayerTransLeaveGame() {}

		Result OnLeaveGameRes( Svr::TransactionResult* & pRes );

		// Start Transaction
		virtual Result StartTransaction() override;

		virtual Result CloseTransaction(Result hrRes) override;

		BR_IMPLEMENT_USERMSGTRANS_CLOSE(NetSvrPolicyGame, LeaveGameRes);
	};


	class PlayerTransKickPlayer : public Svr::MessageTransaction< GamePlayerEntity, Message::Game::KickPlayerCmd>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Message::Game::KickPlayerCmd> super;

	private:

	public:
		PlayerTransKickPlayer( IHeap& heap, MessageDataPtr &pIMsg );
		virtual ~PlayerTransKickPlayer() {}

		Result OnKickPlayerRes( Svr::TransactionResult* & pRes );

		// Start Transaction
		virtual Result StartTransaction() override;

		virtual Result CloseTransaction(Result hrRes) override;

		BR_IMPLEMENT_USERMSGTRANS_CLOSE(NetSvrPolicyGame, KickPlayerRes);
	};
	//
	//// game end event from game instance server
	//class PlayerTransPlayerKickedS2SEvt : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Message::GameInstance::PlayerKickedS2CEvt>
	//{
	//public:
	//	typedef Svr::UserTransactionS2SEvt< GamePlayerEntity, Message::GameInstance::PlayerKickedS2CEvt> super;

	//public:
	//	PlayerTransPlayerKickedS2SEvt( IHeap& heap, MessageDataPtr &pIMsg )  :UserTransactionS2SEvt(heap,  pIMsg ) {}
	//	virtual ~PlayerTransPlayerKickedS2SEvt() {}

	//	// Start Transaction
	//	virtual Result StartTransaction() override;
	//};

	
	class PlayerTransAssignRole : public Svr::MessageTransaction< GamePlayerEntity, Message::Game::AssignRoleCmd>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Message::Game::AssignRoleCmd> super;

	private:

	public:
		PlayerTransAssignRole( IHeap& heap, MessageDataPtr &pIMsg );//  :MessageTransaction(heap,  pIMsg ) {}
		virtual ~PlayerTransAssignRole() {}

		Result OnAssignRoleRes( Svr::TransactionResult* &pRes );

		// Start Transaction
		virtual Result StartTransaction() override;

		BR_IMPLEMENT_USERMSGTRANS_CLOSE(NetSvrPolicyGame, AssignRoleRes);
	};

	
	// game advanced event from game instance server
	class PlayerTransRoleAssignedS2SEvt : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Message::GameInstance::RoleAssignedS2CEvt>
	{
	public:
		typedef Svr::UserTransactionS2SEvt< GamePlayerEntity, Message::GameInstance::RoleAssignedS2CEvt> super;

	public:
		PlayerTransRoleAssignedS2SEvt( IHeap& heap, MessageDataPtr &pIMsg )  :UserTransactionS2SEvt(heap,  pIMsg ) {}
		virtual ~PlayerTransRoleAssignedS2SEvt() {}

		// Start Transaction
		virtual Result StartTransaction() override;
	};

	
	
	class PlayerTransChatMessage : public Svr::MessageTransaction< GamePlayerEntity, Message::Game::ChatMessageCmd>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Message::Game::ChatMessageCmd> super;

	public:
		PlayerTransChatMessage( IHeap& heap, MessageDataPtr &pIMsg )  :MessageTransaction(heap, std::forward<MessageDataPtr>(pIMsg) ) {}
		virtual ~PlayerTransChatMessage() {}

		// Start Transaction
		virtual Result StartTransaction() override;

		BR_IMPLEMENT_USERMSGTRANS_CLOSE(NetSvrPolicyGame, ChatMessageRes);
	};

	// Chat message from other entity	
	class PlayerTransChatMessageFromOtherEntity : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Message::GameServer::ChatMessageC2SEvt>
	{
	public:
		typedef Svr::UserTransactionS2SEvt< GamePlayerEntity, Message::GameServer::ChatMessageC2SEvt> super;

	public:
		PlayerTransChatMessageFromOtherEntity( IHeap& heap, MessageDataPtr &pIMsg )  :UserTransactionS2SEvt(heap,  pIMsg ) {}
		virtual ~PlayerTransChatMessageFromOtherEntity() {}

		// Start Transaction
		virtual Result StartTransaction() override;
	};

	

	class PlayerTransVoteGameAdvance : public Svr::MessageTransaction< GamePlayerEntity, Message::Game::VoteGameAdvanceCmd>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Message::Game::VoteGameAdvanceCmd> super;

	private:

	public:
		PlayerTransVoteGameAdvance( IHeap& heap, MessageDataPtr &pIMsg );
		virtual ~PlayerTransVoteGameAdvance() {}

		Result OnVoteRes( Svr::TransactionResult* &pRes );

		// Start Transaction
		virtual Result StartTransaction() override;

		BR_IMPLEMENT_USERMSGTRANS_CLOSE(NetSvrPolicyGame, VoteGameAdvanceRes);
	};
	
	// game end event from game instance server
	class PlayerTransGameAdvanceVotedS2SEvt : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Message::GameInstance::GameAdvanceVotedS2CEvt>
	{
	public:
		typedef Svr::UserTransactionS2SEvt< GamePlayerEntity, Message::GameInstance::GameAdvanceVotedS2CEvt> super;

	public:
		PlayerTransGameAdvanceVotedS2SEvt( IHeap& heap, MessageDataPtr &pIMsg )  :UserTransactionS2SEvt(heap,  pIMsg ) {}
		virtual ~PlayerTransGameAdvanceVotedS2SEvt() {}

		// Start Transaction
		virtual Result StartTransaction() override;
	};


	class PlayerTransVote : public Svr::MessageTransaction< GamePlayerEntity, Message::Game::VoteCmd>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Message::Game::VoteCmd> super;

	private:

	public:
		PlayerTransVote( IHeap& heap, MessageDataPtr &pIMsg );
		virtual ~PlayerTransVote() {}

		Result OnVoteRes( Svr::TransactionResult* &pRes );

		// Start Transaction
		virtual Result StartTransaction() override;

		BR_IMPLEMENT_USERMSGTRANS_CLOSE(NetSvrPolicyGame, VoteRes);
	};
	
	// game end event from game instance server
	class PlayerTransVoteEndS2SEvt : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Message::GameInstance::VoteEndS2CEvt>
	{
	public:
		typedef Svr::UserTransactionS2SEvt< GamePlayerEntity, Message::GameInstance::VoteEndS2CEvt> super;

	public:
		PlayerTransVoteEndS2SEvt( IHeap& heap, MessageDataPtr &pIMsg )  :UserTransactionS2SEvt(heap,  pIMsg ) {}
		virtual ~PlayerTransVoteEndS2SEvt() {}

		// Start Transaction
		virtual Result StartTransaction() override;
	};


	// game end event from game instance server
	class PlayerTransPlayerRevealedS2SEvt : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Message::GameInstance::PlayerRevealedS2CEvt>
	{
	public:
		typedef Svr::UserTransactionS2SEvt< GamePlayerEntity, Message::GameInstance::PlayerRevealedS2CEvt> super;

	public:
		PlayerTransPlayerRevealedS2SEvt( IHeap& heap, MessageDataPtr &pIMsg )  :UserTransactionS2SEvt(heap,  pIMsg ) {}
		virtual ~PlayerTransPlayerRevealedS2SEvt() {}

		// Start Transaction
		virtual Result StartTransaction() override;
	};


	
	class PlayerTransAdvanceGame : public Svr::MessageTransaction< GamePlayerEntity, Message::Game::AdvanceGameCmd>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Message::Game::AdvanceGameCmd> super;

	public:
		PlayerTransAdvanceGame( IHeap& heap, MessageDataPtr &pIMsg );
		virtual ~PlayerTransAdvanceGame() {}

		Result OnAdvanceGameRes( Svr::TransactionResult* &pRes );

		// Start Transaction
		virtual Result StartTransaction() override;

		BR_IMPLEMENT_USERMSGTRANS_CLOSE(NetSvrPolicyGame, AdvanceGameRes);
	};
	
	// game advanced event from game instance server
	class PlayerTransGameAdvancedS2SEvt : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Message::GameInstance::GameAdvancedS2CEvt>
	{
	public:
		typedef Svr::UserTransactionS2SEvt< GamePlayerEntity, Message::GameInstance::GameAdvancedS2CEvt> super;

	public:
		PlayerTransGameAdvancedS2SEvt( IHeap& heap, MessageDataPtr &pIMsg )  :UserTransactionS2SEvt(heap,  pIMsg ) {}
		virtual ~PlayerTransGameAdvancedS2SEvt() {}

		// Start Transaction
		virtual Result StartTransaction() override;
	};

	// game end event from game instance server
	class PlayerTransGameEndedS2SEvt : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Message::GameInstance::GameEndedS2CEvt>
	{
	public:
		typedef Svr::UserTransactionS2SEvt< GamePlayerEntity, Message::GameInstance::GameEndedS2CEvt> super;

	public:
		PlayerTransGameEndedS2SEvt( IHeap& heap, MessageDataPtr &pIMsg );//  :UserTransactionS2SEvt(heap,  pIMsg ) {}
		virtual ~PlayerTransGameEndedS2SEvt() {}

		Result OnUpdateDBRes(  Svr::TransactionResult* &pRes );

		// Start Transaction
		virtual Result StartTransaction() override;
	};
	
	// Game player killed event
	class PlayerTransPlayerKilledS2SEvt : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Message::GameInstance::PlayerKilledS2CEvt>
	{
	public:
		typedef Svr::UserTransactionS2SEvt< GamePlayerEntity, Message::GameInstance::PlayerKilledS2CEvt> super;

	public:
		PlayerTransPlayerKilledS2SEvt( IHeap& heap, MessageDataPtr &pIMsg )  :UserTransactionS2SEvt(heap,  pIMsg ) {}
		virtual ~PlayerTransPlayerKilledS2SEvt() {}

		// Start Transaction
		virtual Result StartTransaction() override;
	};

	
	// Player voted event
	class PlayerTransPlayerVotedS2SEvt : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Message::GameInstance::VotedS2CEvt>
	{
	public:
		typedef Svr::UserTransactionS2SEvt< GamePlayerEntity, Message::GameInstance::VotedS2CEvt> super;

	public:
		PlayerTransPlayerVotedS2SEvt( IHeap& heap, MessageDataPtr &pIMsg )  :UserTransactionS2SEvt(heap,  pIMsg ) {}
		virtual ~PlayerTransPlayerVotedS2SEvt() {}

		// Start Transaction
		virtual Result StartTransaction() override;
	};


	

	class PlayerTransPlayAgain : public Svr::MessageTransaction< GamePlayerEntity, Message::Game::GamePlayAgainCmd>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Message::Game::GamePlayAgainCmd> super;

	private:
		uint64_t m_TotalGem;
		uint64_t m_TotalGameMoney;

	public:
		PlayerTransPlayAgain(IHeap& heap, MessageDataPtr &pIMsg);
		virtual ~PlayerTransPlayAgain() {}

		Result OnPlayAgainRes(Svr::TransactionResult* &pRes);
		Result OnCreatePartyRes(Svr::TransactionResult* &pRes);

		Result RequestPlayAgain();
		Result RequestCreateParty();

		// Start Transaction
		virtual Result StartTransaction() override;

		BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(NetSvrPolicyGame, GamePlayAgainRes, m_TotalGem, m_TotalGameMoney);
	};


	// Player voted event
	class PlayerTransPlayAgainS2SEvt : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Message::GameInstance::GamePlayAgainS2CEvt>
	{
	public:
		typedef Svr::UserTransactionS2SEvt< GamePlayerEntity, Message::GameInstance::GamePlayAgainS2CEvt> super;

	private:
		PartyUID m_PartyUID;
		PlayerID m_LeadPlayer;

	public:
		PlayerTransPlayAgainS2SEvt(IHeap& heap, MessageDataPtr &pIMsg);// : UserTransactionS2SEvt(heap, pIMsg) {}
		virtual ~PlayerTransPlayAgainS2SEvt() {}

		Result OnJoinPartyRes(Svr::TransactionResult* &pRes);

		// Start Transaction
		virtual Result StartTransaction() override;

		//BR_IMPLEMENT_EVTTRANS_CLOSE_ARGS(NetSvrPolicyGame, GamePlayAgainS2CEvt, m_PartyUID, m_LeadPlayer);

	};



	class PlayerTransGameRevealPlayer : public Svr::MessageTransaction< GamePlayerEntity, Message::Game::GameRevealPlayerCmd>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Message::Game::GameRevealPlayerCmd> super;

	private:

		StaticArray<PlayerID,4> m_RevealedPlayerID;
		StaticArray<uint8_t,4> m_RevealedPlayerRole;
		uint64_t m_TotalGem;
		uint64_t m_TotalGameMoney;

	public:
		PlayerTransGameRevealPlayer(IHeap& heap, MessageDataPtr &pIMsg);
		virtual ~PlayerTransGameRevealPlayer() {}

		Result OnUpdatePlayerRes(Svr::TransactionResult* &pRes);
		Result OnGameRevealPlayerRes(Svr::TransactionResult* &pRes);

		// Start Transaction
		virtual Result StartTransaction() override;

		BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(NetSvrPolicyGame, GameRevealPlayerRes, m_RevealedPlayerID, m_RevealedPlayerRole, m_TotalGem, m_TotalGameMoney);
	};




	class PlayerTransGamePlayerRevive : public Svr::MessageTransaction< GamePlayerEntity, Message::Game::GamePlayerReviveCmd>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Message::Game::GamePlayerReviveCmd> super;

	private:
		uint64_t m_TotalGem;
		uint64_t m_TotalGameMoney;

	public:
		PlayerTransGamePlayerRevive(IHeap& heap, MessageDataPtr &pIMsg);
		virtual ~PlayerTransGamePlayerRevive() {}

		Result OnUpdatePlayerRes(Svr::TransactionResult* &pRes);
		Result OnGamePlayerReviveRes(Svr::TransactionResult* &pRes);

		// Start Transaction
		virtual Result StartTransaction() override;

		BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(NetSvrPolicyGame, GamePlayerReviveRes, m_TotalGem, m_TotalGameMoney);
	};



	// Player voted event
	class PlayerTransGamePlayerRevivedS2SEvt : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Message::GameInstance::GamePlayerRevivedS2CEvt>
	{
	public:
		typedef Svr::UserTransactionS2SEvt< GamePlayerEntity, Message::GameInstance::GamePlayerRevivedS2CEvt> super;

	public:
		PlayerTransGamePlayerRevivedS2SEvt(IHeap& heap, MessageDataPtr &pIMsg) : UserTransactionS2SEvt(heap, pIMsg) {}
		virtual ~PlayerTransGamePlayerRevivedS2SEvt() {}

		// Start Transaction
		virtual Result StartTransaction() override;
	};



	class PlayerTransGamePlayerResetRank : public Svr::MessageTransaction< GamePlayerEntity, Message::Game::GamePlayerResetRankCmd>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Message::Game::GamePlayerResetRankCmd> super;

	private:
		uint64_t m_TotalGem;
		uint64_t m_TotalGameMoney;

	public:
		PlayerTransGamePlayerResetRank(IHeap& heap, MessageDataPtr &pIMsg) : MessageTransaction(heap, std::forward<MessageDataPtr>(pIMsg)) {}
		virtual ~PlayerTransGamePlayerResetRank() {}

		// Start Transaction
		virtual Result StartTransaction() override;

		BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(NetSvrPolicyGame, GamePlayerResetRankRes, m_TotalGem, m_TotalGameMoney);
	};


} // namespace GameServer 
} // namespace SF 

