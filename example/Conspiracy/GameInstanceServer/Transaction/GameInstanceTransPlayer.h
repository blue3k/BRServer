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
#include "MemoryManager/SFMemoryPool.h"
#include "Types/SFEngineTypedefs.h"
#include "Stream/SFMemoryStream.h"
#include "Protocol/Message/GameMsgClass.h"
#include "Protocol/Policy/GameNetPolicy.h"
#include "Protocol/Message/GameInstanceMsgClass.h"
#include "Protocol/Policy/GameInstanceNetPolicy.h"

#include "Transaction/MessageRoute.h"
#include "Transaction/ServerTransaction.h"

#include "ConspiracyGameInstanceServerClass.h"
#include "Transaction/GameInstanceTransaction.h"

#include "GameInstance/GameInstanceEntity.h"
#include "Net/SFMessage.h"


namespace SF {
	namespace ConspiracyGameInstanceServer {



		class GameEntityTransJoinGame : public RoutedGamePlayerMessageTransaction< GameInstanceEntity, Message::GameInstance::JoinGameCmd>
		{
		public:
			using super = RoutedGamePlayerMessageTransaction< GameInstanceEntity, Message::GameInstance::JoinGameCmd>;

		private:

			NetAddress m_Addr;
			UTCTimeStampMS m_TimeStamp;
			GameInsUID m_GameInsUID;
			GameStateID m_GameState;
			uint8_t m_Day;
			PlayerRole m_Role;
			bool m_Dead, m_bIsFirstJoin;
			uint m_PlayerIndex;
			uint m_PlayerCharacter;

			StaticOutputMemoryStream<GameConst::MAX_CHATLOG_BUFFER> m_ChatHistoryBuffer;
			StaticOutputMemoryStream<GameConst::MAX_GAMELOG_BUFFER> m_GameLogBuffer;


		public:
			GameEntityTransJoinGame(IHeap& heap, MessageDataPtr& pIMsg)
				: RoutedGamePlayerMessageTransaction(heap, pIMsg)
				, m_ChatHistoryBuffer(heap)
				, m_GameLogBuffer(heap)
			{}
			virtual ~GameEntityTransJoinGame() {}

			// Start Transaction
			virtual Result StartTransaction() override;
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
			BR_SVR_MSGTRANS_CLOSE_ARGS(NetSvrPolicyGameInstance, JoinGameRes, RouteContext(m_GameInsUID, GetRouteContext().GetFrom()), m_Addr, (uint32_t)(DurationCast<DurationSec>(m_TimeStamp.time_since_epoch()).count()), (uint8_t)m_GameState, (uint8_t)m_Day,
				(uint8_t)GetMyOwner()->GetMaxPlayer(),
				(uint8_t)m_PlayerIndex, (uint8_t)m_PlayerCharacter, (uint8_t)m_Role, (uint8_t)m_Dead, (uint8_t)m_bIsFirstJoin,
				m_ChatHistoryBuffer.GetBuffer(),
				m_GameLogBuffer.GetBuffer());
		};


		class GameEntityTransLeaveGame : public RoutedGamePlayerMessageTransaction< GameInstanceEntity, Message::GameInstance::LeaveGameInstanceCmd>
		{
		public:
			using super = RoutedGamePlayerMessageTransaction< GameInstanceEntity, Message::GameInstance::LeaveGameInstanceCmd>;

		private:

		public:
			GameEntityTransLeaveGame(IHeap& heap, MessageDataPtr& pIMsg) :RoutedGamePlayerMessageTransaction(heap, pIMsg) {}
			virtual ~GameEntityTransLeaveGame() {}

			// Start Transaction
			virtual Result StartTransaction() override;

			BR_SVR_MSGTRANS_CLOSE(NetSvrPolicyGameInstance, LeaveGameInstanceRes, GetRouteContext().GetSwaped());
		};

		class GameEntityTransKickPlayer : public RoutedGamePlayerMessageTransaction< GameInstanceEntity, Message::GameInstance::KickPlayerCmd>
		{
		public:
			using super = RoutedGamePlayerMessageTransaction< GameInstanceEntity, Message::GameInstance::KickPlayerCmd>;

		private:

		public:
			GameEntityTransKickPlayer(IHeap& heap, MessageDataPtr& pIMsg) :RoutedGamePlayerMessageTransaction(heap, pIMsg) {}
			virtual ~GameEntityTransKickPlayer() {}

			// Start Transaction
			virtual Result StartTransaction() override;

			BR_SVR_MSGTRANS_CLOSE(NetSvrPolicyGameInstance, KickPlayerRes, GetRouteContext().GetSwaped());
		};



		class GameEntityTransAssignRole : public RoutedGamePlayerMessageTransaction< GameInstanceEntity, Message::GameInstance::AssignRoleCmd>
		{
		public:
			using super = RoutedGamePlayerMessageTransaction< GameInstanceEntity, Message::GameInstance::AssignRoleCmd>;

		private:

		public:
			GameEntityTransAssignRole(IHeap& heap, MessageDataPtr& pIMsg) :RoutedGamePlayerMessageTransaction(heap, pIMsg) {}
			virtual ~GameEntityTransAssignRole() {}

			// Start Transaction
			virtual Result StartTransaction() override;

			BR_SVR_MSGTRANS_CLOSE(NetSvrPolicyGameInstance, AssignRoleRes, GetRouteContext().GetSwaped());
		};


		class GameEntityTransChatMessage : public RoutedGamePlayerMessageTransaction< GameInstanceEntity, Message::GameInstance::ChatMessageC2SEvt>
		{
		public:
			using super = RoutedGamePlayerMessageTransaction< GameInstanceEntity, Message::GameInstance::ChatMessageC2SEvt>;

		private:

		public:
			GameEntityTransChatMessage(IHeap& heap, MessageDataPtr& pIMsg) :RoutedGamePlayerMessageTransaction(heap, pIMsg) {}
			virtual ~GameEntityTransChatMessage() {}

			// Start Transaction
			virtual Result StartTransaction() override;

		};


		class GameEntityTransVoteGameAdvance : public RoutedGamePlayerMessageTransaction< GameInstanceEntity, Message::GameInstance::VoteGameAdvanceCmd>
		{
		public:
			using super = RoutedGamePlayerMessageTransaction< GameInstanceEntity, Message::GameInstance::VoteGameAdvanceCmd>;

		private:

		public:
			GameEntityTransVoteGameAdvance(IHeap& heap, MessageDataPtr& pIMsg) :RoutedGamePlayerMessageTransaction(heap, pIMsg) {}
			virtual ~GameEntityTransVoteGameAdvance() {}

			// Start Transaction
			virtual Result StartTransaction() override;

			BR_SVR_MSGTRANS_CLOSE(NetSvrPolicyGameInstance, VoteGameAdvanceRes, GetRouteContext().GetSwaped());
		};



		class GameEntityTransVote : public RoutedGamePlayerMessageTransaction< GameInstanceEntity, Message::GameInstance::VoteCmd>
		{
		public:
			using super = RoutedGamePlayerMessageTransaction< GameInstanceEntity, Message::GameInstance::VoteCmd>;

		private:

		public:
			GameEntityTransVote(IHeap& heap, MessageDataPtr& pIMsg) :RoutedGamePlayerMessageTransaction(heap, pIMsg) {}
			virtual ~GameEntityTransVote() {}

			// Start Transaction
			virtual Result StartTransaction() override;

			BR_SVR_MSGTRANS_CLOSE(NetSvrPolicyGameInstance, VoteRes, GetRouteContext().GetSwaped());
		};

		class GameEntityTransAdvanceGame : public RoutedGamePlayerMessageTransaction< GameInstanceEntity, Message::GameInstance::AdvanceGameCmd>
		{
		public:
			using super = RoutedGamePlayerMessageTransaction< GameInstanceEntity, Message::GameInstance::AdvanceGameCmd>;

		private:

		public:
			GameEntityTransAdvanceGame(IHeap& heap, MessageDataPtr& pIMsg) :RoutedGamePlayerMessageTransaction(heap, pIMsg) {}
			virtual ~GameEntityTransAdvanceGame() {}

			// Start Transaction
			virtual Result StartTransaction() override;

			BR_SVR_MSGTRANS_CLOSE(NetSvrPolicyGameInstance, AdvanceGameRes, GetRouteContext().GetSwaped());
		};



		class GameEntityTransGamePlayAgain : public RoutedGamePlayerMessageTransaction< GameInstanceEntity, Message::GameInstance::GamePlayAgainCmd>
		{
		public:
			using super = RoutedGamePlayerMessageTransaction< GameInstanceEntity, Message::GameInstance::GamePlayAgainCmd>;

		private:
			uint m_MemberCount;

		public:
			GameEntityTransGamePlayAgain(IHeap& heap, MessageDataPtr& pIMsg) :RoutedGamePlayerMessageTransaction(heap, pIMsg) {}
			virtual ~GameEntityTransGamePlayAgain() {}

			// Start Transaction
			virtual Result StartTransaction() override;

			BR_SVR_MSGTRANS_CLOSE_ARGS(NetSvrPolicyGameInstance, GamePlayAgainRes, GetRouteContext().GetSwaped(), m_MemberCount);
		};



		class GameEntityTransGameRevealPlayer : public RoutedGamePlayerMessageTransaction< GameInstanceEntity, Message::GameInstance::GameRevealPlayerCmd>
		{
		public:
			using super = RoutedGamePlayerMessageTransaction< GameInstanceEntity, Message::GameInstance::GameRevealPlayerCmd>;

		private:
			StaticArray<PlayerID, 4> m_RevealedPlayerID;
			StaticArray<uint8_t, 4> m_RevealedPlayerRole;

		public:
			GameEntityTransGameRevealPlayer(IHeap& heap, MessageDataPtr& pIMsg)
				: RoutedGamePlayerMessageTransaction(heap, pIMsg)
				, m_RevealedPlayerID(heap)
				, m_RevealedPlayerRole(heap)
			{}
			virtual ~GameEntityTransGameRevealPlayer() {}

			// Start Transaction
			virtual Result StartTransaction() override;

			BR_SVR_MSGTRANS_CLOSE_ARGS(NetSvrPolicyGameInstance, GameRevealPlayerRes, GetRouteContext().GetSwaped(), m_RevealedPlayerID, m_RevealedPlayerRole);
		};



		class GameEntityTransGamePlayerRevive : public RoutedGamePlayerMessageTransaction< GameInstanceEntity, Message::GameInstance::GamePlayerReviveCmd>
		{
		public:
			using super = RoutedGamePlayerMessageTransaction< GameInstanceEntity, Message::GameInstance::GamePlayerReviveCmd>;

		private:

		public:
			GameEntityTransGamePlayerRevive(IHeap& heap, MessageDataPtr& pIMsg) :RoutedGamePlayerMessageTransaction(heap, pIMsg) {}
			virtual ~GameEntityTransGamePlayerRevive() {}

			// Start Transaction
			virtual Result StartTransaction() override;

			BR_SVR_MSGTRANS_CLOSE(NetSvrPolicyGameInstance, GamePlayerReviveRes, GetRouteContext().GetSwaped());
		};





	}// namespace ConspiracyGameInstanceServer 
}// namespace SF 

