////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Game Transaction Utilities
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once


#include "SFTypedefs.h"
#include "Transaction/Transaction.h"
#include "ServerSystem/ServiceEntity/GamePartyEntity.h"
#include "Transaction/ServerTransaction.h"




namespace SF {
namespace Svr {

	class PartyPlayer;

	// Message transaction template
	template< class PolicyClass, class MessageClass, class MemoryPoolClass >
	class GamePartyMessageTransaction : public ServerEntityMessageTransaction<GamePartyEntity, MessageClass, MemoryPoolClass>
	{
	private:
		typedef ServerEntityMessageTransaction<GamePartyEntity, MessageClass, MemoryPoolClass> super;
		typedef PolicyClass PolicyClassType;
	protected:
		// Player Player ID
		PlayerID	m_PlayerID;

	public:
		GamePartyMessageTransaction( IMemoryManager& memMgr, MessageDataPtr &pIMsg )
			:super( memMgr, pIMsg )
			,m_PlayerID(0)
		{
			super::m_WorkOnServerEntity = false;
		}
		
		Net::Connection* GetConnection()
		{
			return super::GetServerEntity()->GetConnection();
		}

		Result GetMyPlayer( PartyPlayer* &pPlayer )
		{
			if( MessageClass::HasGetPlayerID )
			{
				return super::GetMyOwner()->FindPlayer(super::GetPlayerID(), pPlayer );
			}
			else
			{
				pPlayer = nullptr;
				return ResultCode::FAIL;
			}
		}
	};




}; // Svr
}; // BR




