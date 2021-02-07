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
#include "GameInstance/GameInstanceEntity.h"


namespace SF {
namespace Net {
	class IConnection;
	class Connection;
}};


namespace SF {
namespace ConspiracyGameInstanceServer {

	class GamePlayer;
	class GameInstanceEntity;



	// Message transaction template
	template< class OwnerType, class MessageClass >
	class RoutedGamePlayerMessageTransaction : public Svr::TransactionT<OwnerType>, public MessageClass
	{
	public:
		using MessageClassType = MessageClass;
		using super = Svr::TransactionT<OwnerType>;

	protected:
		// Player Player ID
		PlayerID	m_PlayerID;

	public:
		RoutedGamePlayerMessageTransaction(IHeap& heap, MessageDataPtr &pIMsg )
			: super(heap, TransactionID() )
			, MessageClass(std::forward<MessageDataPtr>(pIMsg))
			, m_PlayerID(0)
		{
		}

		Result ParseMessage()
		{
			Result hr = MessageClass::ParseMsg();
			if (hr)
			{
				if (MessageClass::GetMessage()->GetMessageHeader()->msgID.IDs.Type == Message::MSGTYPE_COMMAND)
				{
					if(MessageClass::HasTransactionID)
					{
						super::SetParentTransID(MessageClass::GetTransactionID());
					}
					if(MessageClass::HasRouteContext)
					{
						super::SetMessageRouteContext(MessageClass::GetRouteContext());
					}
				}
			}
			return hr;
		}

		// Initialize Transaction
		virtual Result InitializeTransaction( Svr::Entity* pOwner )
		{
			Result hr = ResultCode::SUCCESS;
			SharedPointerT<Svr::Entity> pEntity;

			svrChkPtr( pOwner );

			svrChk(ParseMessage());

			if(Service::ServerConfig->UID != MessageClass::GetRouteContext().GetTo().GetServerID())
			{
				svrErr( ResultCode::SVR_INVALID_SERVERID );
			}

			hr = Service::EntityTable->find(MessageClass::GetRouteContext().GetTo().GetEntityID(), pEntity);
			if (!(hr))
			{
				svrTrace(Warning, "Can't find transaction target instance:{0}", MessageClass::GetRouteContext().GetTo());
				goto Proc_End;
			}

			svrChk(super::InitializeTransaction((Svr::Entity*)pEntity));


		Proc_End:

			return hr;
		}


		SF_FORCEINLINE GameInstanceEntity* GetMyOwner()
		{
			Assert( super::GetOwnerEntity() );
			return (GameInstanceEntity*)super::GetOwnerEntity();
		}

		Result GetMyPlayer( GamePlayer* &pPlayer )
		{
			if( MessageClass::HasPlayerID )
			{
				return GetMyOwner()->FindPlayer(MessageClass::GetPlayerID(), pPlayer );
			}
			else
			{
				pPlayer = nullptr;
				return ResultCode::FAIL;
			}
		}
	};




} // GameServer
} // SF




