////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) The Braves
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
#include "Util/Memento.h"
#include "Container/SFArray.h"
#include "Types/SFEngineTypedefs.h"
#include "GameConst.h"
#include "Protocol/Message/GameMsgClass.h"
#include "Protocol/Policy/GameNetPolicy.h"

#include "Transaction/MessageRoute.h"
#include "ServiceEntity/Game/GamePlayerEntity.h"
#include "Net/SFMessage.h"


namespace SF {

	namespace Svr {

		/////////////////////////////////////////////////////////////////////////////
		//
		//	transactions
		//

		class PlayerTransCreateCharacter : public Svr::MessageTransaction< GamePlayerEntity, Message::Game::CreateCharacterCmd>
		{
		public:
			using super = Svr::MessageTransaction< GamePlayerEntity, Message::Game::CreateCharacterCmd>;

		private:
			uint32_t m_CharacterId{};

		public:
			PlayerTransCreateCharacter(IHeap& heap, const MessageDataPtr& pIMsg);

			Result RequestCreateCharacterDB();
			Result OnCreateCharacterRes(Svr::TransactionResult* pRes);

			// Start Transaction
			virtual Result StartTransaction() override;

			BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(NetSvrPolicyGame, CreateCharacterRes, m_CharacterId);
		};



		class PlayerTransDeleteCharacter : public Svr::MessageTransaction<GamePlayerEntity, Message::Game::DeleteCharacterCmd>
		{
		public:
			using super = Svr::MessageTransaction<GamePlayerEntity, Message::Game::DeleteCharacterCmd>;

		public:
			PlayerTransDeleteCharacter(IHeap& heap, const MessageDataPtr& pIMsg);

			Result RequestDeleteCharacterDB();
			Result OnDeleteCharacterRes(Svr::TransactionResult* pRes);

			// Start Transaction
			virtual Result StartTransaction() override;

			BR_IMPLEMENT_USERMSGTRANS_CLOSE(NetSvrPolicyGame, DeleteCharacterRes);
		};


		class PlayerTransGetCharacter : public Svr::MessageTransaction<GamePlayerEntity, Message::Game::GetCharacterDataCmd>
		{
		public:
			using super = Svr::MessageTransaction<GamePlayerEntity, Message::Game::GetCharacterDataCmd>;

		private:
			VariableTable m_CharacterData;

		public:
			PlayerTransGetCharacter(IHeap& heap, const MessageDataPtr& pIMsg);

			Result RequestGetCharacterDB();
			Result OnGetCharacterRes(Svr::TransactionResult* pRes);

			// Start Transaction
			virtual Result StartTransaction() override;

			BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(NetSvrPolicyGame, GetCharacterDataRes, m_CharacterData);
		};


		class PlayerTransGetCharacterList : public Svr::MessageTransaction<GamePlayerEntity, Message::Game::GetCharacterListCmd>
		{
		public:
			using super = Svr::MessageTransaction<GamePlayerEntity, Message::Game::GetCharacterListCmd>;

		private:
			DynamicArray<VariableTable> m_Characters;
			

		public:
			PlayerTransGetCharacterList(IHeap& heap, const MessageDataPtr& pIMsg);

			Result RequestGetCharacterListDB();
			Result OnGetCharacterListRes(Svr::TransactionResult* pRes);

			// Start Transaction
			virtual Result StartTransaction() override;

			BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(NetSvrPolicyGame, GetCharacterListRes, m_Characters);
		};


		class PlayerTransSelectCharacter : public Svr::MessageTransaction<GamePlayerEntity, Message::Game::SelectCharacterCmd>
		{
		public:
			using super = Svr::MessageTransaction<GamePlayerEntity, Message::Game::SelectCharacterCmd>;

			VariableTable m_CharacterDataAll;

		public:
			PlayerTransSelectCharacter(IHeap& heap, const MessageDataPtr& pIMsg);

			Result RequestSaveCharacterDB();
			Result OnSaveCharacterRes(Svr::TransactionResult* pRes);

			Result RequestGetCharacterDB();
			Result OnGetCharacterRes(Svr::TransactionResult* pRes);

			// Start Transaction
			virtual Result StartTransaction() override;

			BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(NetSvrPolicyGame, SelectCharacterRes, GetMyOwner()->GetCharacterID(), m_CharacterDataAll);
		};


	} // namespace Svr 
} // namespace SF 

