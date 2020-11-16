////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Game player Entity
//	
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "SFTypedefs.h"

#include "Util/SFTimeUtil.h"
#include "Container/SFHashTable.h"
#include "Memory/SFMemoryPool.h"
#include "Entity/Entity.h"
#include "Transaction/MessageRoute.h"
#include "Entity/SimpleUserEntity.h"
#include "GameConst.h"
#include "Types/BrGameTypes.h"
#include "Util/GlobalUIDGenerator.h"




namespace SF {
namespace Net {
	class Connection;
};

namespace Policy {
	class ISvrPolicyLogin;
};
};



namespace SF {
namespace Svr {



	//////////////////////////////////////////////////////////////////////////
	//
	//	Game Instance entity class
	//

	class LoginPlayerEntity : public SimpleUserEntity
	{
	public:

	private:
		typedef Svr::SimpleUserEntity super;

		GameID m_GameID = nullptr;
		FacebookUID m_FacebookUID;
		AuthTicket m_AuthTicket;
		uint m_ShardID;
		bool m_IsTicketOwner;

		char m_UserName[GameConst::MAX_NAME];
		char m_GCMKeys[GameConst::MAX_GCMKEYS];



		static GlobalUIDGenerator stm_AuthTicketGenerator;

		bool m_ClosingPended = false;

		//////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Player Info
		//


	protected:


	public:

		LoginPlayerEntity();
		virtual ~LoginPlayerEntity();

		GameID GetGameID() { return m_GameID; }
		void SetGameID(GameID value) { m_GameID = value; }

		PlayerID GetPlayerID() { return GetAccountID(); }
		void SetPlayerID(PlayerID value) { SetAccountID(value); }

		FacebookUID GetFacebookUID() { return m_FacebookUID; }
		void SetFacebookUID(FacebookUID value) { m_FacebookUID = value; }

		AuthTicket GetAuthTicket() { return m_AuthTicket; }
		void SetAuthTicket(AuthTicket value) { m_AuthTicket = value; }

		uint GetShardID() { return m_ShardID; }
		void SetShardID(uint value) { m_ShardID = value; }

		bool GetIsTicketOwner() { return m_IsTicketOwner; }
		void SetIsTicketOwner(bool value) { m_IsTicketOwner = value; }


		// Initialize entity to proceed new connection
		virtual Result InitializeEntity( EntityID newEntityID ) override;

		// Set connection for pilot
		virtual Result SetConnection(SharedPointerT<Net::Connection>&& pCon ) override;


		// pending close transaction
		virtual Result PendingCloseTransaction(const char* reason) override;

		//////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	attributes
		//

		const char* GetGCMKey()								{ return m_GCMKeys; }
		Result SetGCMKey(const char* gcmKey)				{ return StrUtil::StringCopy(m_GCMKeys, gcmKey); }

		const char* GetUserName() { return m_UserName; }
		void SetUserName(const char* userName);

		static GlobalUIDGenerator& GetAuthTicketGenerator() { return stm_AuthTicketGenerator; }


		//////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Entity process
		//

		// register message handlers
		virtual Result RegisterMessageHandlers() override;

		// clear transaction
		virtual Result ClearEntity() override;

		// Run the task
		virtual Result TickUpdate(TimerAction *pAction = nullptr) override;


		//////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Update
		//

		// Update Game Player 
		Result UpdateLoginPlayer(TimeStampMS CurTime );

	};



}; // namespace Svr
}; // namespace SF

