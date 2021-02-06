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
#include "ServiceEntity/Party/GamePartyEntity.h"
#include "Transaction/ServerTransaction.h"




namespace SF {
namespace Svr {

	class PartyPlayer;

	// Message transaction template
	template< class MessageClass >
	class GamePartyMessageTransaction : public ServerEntityMessageTransaction<GamePartyEntity, MessageClass>
	{
	private:
		typedef ServerEntityMessageTransaction<GamePartyEntity, MessageClass> super;
	protected:
		// Player Player ID
		PlayerID	m_PlayerID;

	public:
		GamePartyMessageTransaction( IHeap& memMgr, MessageDataPtr &pIMsg )
			: super( memMgr, pIMsg )
			, m_PlayerID(0)
		{
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




} // Svr
} // SF




