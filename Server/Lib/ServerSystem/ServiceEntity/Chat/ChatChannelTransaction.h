////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2018 The Braves
// 
// Author : KyungKun Ko
//
// Description : Chatting channel Transaction Utilities
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once


#include "SFTypedefs.h"
#include "Transaction/Transaction.h"
#include "ServiceEntity/Chat/ChatChannelEntity.h"
#include "Transaction/ServerTransaction.h"




namespace SF {
namespace Svr {

	class PartyPlayer;

	// Message transaction template
	template< class MessageClass >
	class ChatChannelMessageTransaction : public ServerEntityMessageTransaction<ChatChannelEntity, MessageClass>
	{
	private:
		typedef ServerEntityMessageTransaction<ChatChannelEntity, MessageClass> super;
	protected:
		// Player Player ID
		PlayerID	m_PlayerID;

	public:
		ChatChannelMessageTransaction( IHeap& memMgr, MessageDataPtr &pIMsg )
			:super( memMgr, pIMsg )
			,m_PlayerID(0)
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




