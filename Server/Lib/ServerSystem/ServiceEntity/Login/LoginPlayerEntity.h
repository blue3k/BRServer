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
#include "Common/ClassUtil.h"
#include "Util/TimeUtil.h"
#include "Common/HashTable.h"
#include "Memory/MemoryPool.h"
#include "ServerSystem/Entity.h"
#include "ServerSystem/MessageRoute.h"
#include "ServerSystem/SimpleUserEntity.h"
#include "Common/GameConst.h"
#include "Types/BrGameTypes.h"
#include "Common/GlobalUIDGenerator.h"




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

	class LoginPlayerEntity : public SimpleUserEntity, public MemoryPoolObject<LoginPlayerEntity>
	{
	public:

	private:
		typedef Svr::SimpleUserEntity super;

		BRCLASS_ATTRIBUTE(FacebookUID,FacebookUID);
		BRCLASS_ATTRIBUTE(AuthTicket,AuthTicket);
		BRCLASS_ATTRIBUTE(UINT,ShardID);
		BRCLASS_ATTRIBUTE(bool, IsTicketOwner);

		char m_UserName[GameConst::MAX_NAME];
		char m_GCMKeys[GameConst::MAX_GCMKEYS];


		PlayerID GetPlayerID() { return GetAccountID(); }

		// Time for kill this entity
		BRCLASS_ATTRIBUTE_CONST(Util::TimeStampTimer,TimeToKill);

		static GlobalUIDGenerator stm_AuthTicketGenerator;


		//////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Player Info
		//


	protected:


	public:

		LoginPlayerEntity();
		virtual ~LoginPlayerEntity();

		// Initialize entity to proceed new connection
		virtual Result InitializeEntity( EntityID newEntityID ) override;

		// Set connection for pilot
		virtual Result SetConnection(SharedPointerT<Net::Connection>&& pCon ) override;

		void HeartBit();


		// pending close transaction
		Result PendingCloseTransaction();

		//////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	attributes
		//

		const char* GetGCMKey()								{ return m_GCMKeys; }
		Result SetGCMKey(const char* gcmKey)				{ return StrUtil::StringCpy(m_GCMKeys, gcmKey); }

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

