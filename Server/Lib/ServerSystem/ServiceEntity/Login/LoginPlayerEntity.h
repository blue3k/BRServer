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

#include "Common/Typedefs.h"
#include "Common/ClassUtil.h"
#include "Common/TimeUtil.h"
#include "Common/HashTable.h"
#include "Common/MemoryPool.h"
#include "ServerSystem/Entity.h"
#include "ServerSystem/MessageRoute.h"
#include "ServerSystem/SimpleUserEntity.h"
#include "Common/GameConst.h"
#include "Common/BrGameTypes.h"
#include "Common/GlobalUIDGenerator.h"




namespace BR {
namespace Net {
	class Connection;
};

namespace Policy {
	class ISvrPolicyLogin;
};
};



namespace BR {
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

		//char m_GameNick[GameConst::MAX_NAME];
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
		virtual Result InitializeEntity( EntityID newEntityID );

		// Set connection for pilot
		virtual Result SetConnection( Net::Connection* &pCon );

		void HeartBit();

		// pending close transaction
		Result PendingCloseTransaction();

		//////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	attributes
		//

		const char* GetGCMKey()								{ return m_GCMKeys; }
		Result SetGCMKey(const char* gcmKey)				{ return StrUtil::StringCpy(m_GCMKeys, gcmKey); }

		static GlobalUIDGenerator& GetAuthTicketGenerator() { return stm_AuthTicketGenerator; }

		//////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Entity process
		//

		// register message handlers
		virtual Result RegisterMessageHandlers();

		// clear transaction
		virtual Result ClearEntity() override;

		// Run the task
		virtual Result TickUpdate(Svr::TimerAction *pAction = nullptr) override;


		//////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Update
		//

		// Update Game Player 
		Result UpdateLoginPlayer(TimeStampMS CurTime );

	};



}; // namespace Svr
}; // namespace BR

