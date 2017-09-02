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
#include "ServerSystem/Transaction.h"
#include "ServerSystem/ServiceEntity/GamePartyEntity.h"
#include "ServerSystem/ServerTransaction.h"




namespace SF {
namespace Svr {

	class PartyPlayer;

	// Message transaction template
	template< class PolicyClass, class MessageClass, class MemoryPoolClass, size_t MessageHandlerBufferSize = sizeof(Svr::TransactionMessageHandlerType)*2 >
	class GamePartyMessageTransaction : public ServerEntityMessageTransaction<GamePartyEntity, MessageClass, MemoryPoolClass,MessageHandlerBufferSize>
	{
	private:
		typedef ServerEntityMessageTransaction<GamePartyEntity, MessageClass, MemoryPoolClass, MessageHandlerBufferSize> super;
		typedef PolicyClass PolicyClassType;
	protected:
		// Player Player ID
		PlayerID	m_PlayerID;

	public:
		GamePartyMessageTransaction( Message::MessageData* &pIMsg )
			:super( pIMsg )
			,m_PlayerID(0)
		{
			super::m_WorkOnServerEntity = false;
		}
		
		PolicyClassType* GetPolicy()
		{
			return super::GetServerEntity()->GetConnection()->template GetPolicy<PolicyClassType>();
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




