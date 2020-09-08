////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : CharacterData Transaction Utilities
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once


#include "SFTypedefs.h"
#include "Transaction/Transaction.h"
#include "ServiceEntity/Character/CharacterDataManagerServiceEntity.h"
#include "Transaction/ServerTransaction.h"




namespace SF {
namespace Svr {

	class PartyPlayer;

	// Message transaction template
	template< class MessageClass >
	class CharacterDataMessageTransaction : public ClusterEntityMessageTransaction<CharacterDataManagerServiceEntity, MessageClass>
	{
	private:
		typedef ServerEntityMessageTransaction<CharacterDataManagerServiceEntity, MessageClass> super;
	protected:
		// Player Player ID
		PlayerID	m_PlayerID;

	public:
		CharacterDataMessageTransaction( IHeap& memMgr, MessageDataPtr &pIMsg )
			:super( memMgr, pIMsg )
			,m_PlayerID(0)
		{
			super::m_WorkOnServerEntity = false;
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

