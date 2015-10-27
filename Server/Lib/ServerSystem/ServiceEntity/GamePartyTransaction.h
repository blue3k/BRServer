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


#include "Common/Typedefs.h"
#include "ServerSystem/Transaction.h"
#include "ServerSystem/ServiceEntity/GamePartyEntity.h"
#include "ServerSystem/ServerTransaction.h"




namespace BR {
namespace Svr {

	class PartyPlayer;

	// Message transaction template
	template< class PolicyClass, class MessageClass, class MemoryPoolClass, size_t MessageHandlerBufferSize = sizeof(Svr::TransactionMessageHandlerType)*2 >
	class GamePartyMessageTransaction : public Svr::ServerEntityMessageTransaction<GamePartyEntity, MessageClass, MemoryPoolClass,MessageHandlerBufferSize>
	{
	protected:
		// Player Player ID
		PlayerID	m_PlayerID;

	public:
		GamePartyMessageTransaction( Message::MessageData* &pIMsg )
			:ServerEntityMessageTransaction( pIMsg )
			,m_PlayerID(0)
		{
			m_WorkOnServerEntity = false;
		}
		
		FORCEINLINE PolicyClass* GetPolicy()
		{
			return m_ServerEntity->GetConnection()->GetPolicy<PolicyClass>();
		}

		HRESULT GetMyPlayer( PartyPlayer* &pPlayer )
		{
			if( MessageClass::HasGetPlayerID )
			{
				return GetMyOwner()->FindPlayer( GetPlayerID(), pPlayer );
			}
			else
			{
				pPlayer = nullptr;
				return E_FAIL;
			}
		}
	};




}; // Svr
}; // BR




