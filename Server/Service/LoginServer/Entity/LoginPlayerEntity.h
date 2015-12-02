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

#include "Common/TypeDefs.h"
#include "Common/ClassUtil.h"
#include "Common/TimeUtil.h"
#include "Common/HashTable.h"
#include "Common/MemoryPool.h"
#include "ServerSystem/Entity.h"
#include "ServerSystem/MessageRoute.h"
#include "ServerSystem/SimpleUserEntity.h"
#include "Common/GameConst.h"
#include "Common/BrGameTypes.h"
#include "LoginSvrConst.h"



namespace BR {
namespace Net {
	class Connection;
};

namespace Policy {
	class ISvrPolicyLogin;
};
};



namespace BR {
namespace LoginServer {


	class LoginServer;


	//////////////////////////////////////////////////////////////////////////
	//
	//	Game Instance entity class
	//

	class LoginPlayerEntity : public Svr::SimpleUserEntity, public MemoryPoolObject<LoginPlayerEntity>
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


		//////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Player Info
		//

		StaticAllocator< sizeof(Svr::EntityMessageHandlerItem)*4 >	m_Allocator;

	protected:

		virtual MemoryAllocator& GetAllocator()						{ return m_Allocator; }

	public:

		LoginPlayerEntity();
		virtual ~LoginPlayerEntity();

		// Initialize entity to proceed new connection
		virtual HRESULT InitializeEntity( EntityID newEntityID );

		// Set connection for pilot
		virtual HRESULT SetConnection( Net::Connection* &pCon );

		// Release connection if has
		//virtual void ReleaseConnection();

		void HeartBit();

		// pending close transaction
		HRESULT PendingCloseTransaction();

		//////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	attributes
		//

		const char* GetGCMKey()								{ return m_GCMKeys; }
		HRESULT SetGCMKey(const char* gcmKey)				{ return StrUtil::StringCpy(m_GCMKeys, gcmKey); }

		//const char* GetGameNick()							{ return m_GameNick; }
		//HRESULT SetGameNick(const char* gameNick)			{ return StrUtil::StringCpy(m_GameNick, gameNick); }


		//////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Entity process
		//

		// register message handlers
		virtual HRESULT RegisterMessageHandlers();

		// clear transaction
		virtual HRESULT ClearEntity() override;

		// Run the task
		virtual HRESULT TickUpdate(Svr::TimerAction *pAction = nullptr) override;


		//////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Update
		//

		// Update Game Player 
		HRESULT UpdateLoginPlayer(TimeStampMS CurTime );


		//////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	
		//

		//virtual HRESULT SetAccountID( AccountID accID )				{ m_PlayerInformation.PlayerID = accID; return super::SetAccountID(accID); }

	};

#include "LoginPlayerEntity.inl"

}; // namespace LoginServer
}; // namespace BR

