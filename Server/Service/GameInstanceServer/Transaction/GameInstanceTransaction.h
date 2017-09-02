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
	template< class OwnerType, class PolicyClass, class MessageClass, class MemoryPoolClass, size_t MessageHandlerBufferSize = sizeof(Svr::TransactionMessageHandlerType)*2 >
	class RoutedGamePlayerMessageTransaction : public Svr::TransactionT<OwnerType,MemoryPoolClass,MessageHandlerBufferSize>, public MessageClass
	{
	private:
		typedef Svr::TransactionT<OwnerType, MemoryPoolClass, MessageHandlerBufferSize> super;

	protected:
		// Player Player ID
		PlayerID	m_PlayerID;

	public:
		RoutedGamePlayerMessageTransaction( Message::MessageData* &pIMsg )
			:super( TransactionID() )
			,MessageClass( pIMsg )
			,m_PlayerID(0)
		{
		}

		Result ParseMessage()
		{
			Result hr = MessageClass::ParseMsg();
			if ((hr))
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

			assert(super::GetServerEntity()!= nullptr);
			svrChkPtr(super::GetServerEntity());// = dynamic_cast<Svr::ServerEntity*>(pOwner) );

			if(GetMyServer()->GetServerUID() != MessageClass::GetRouteContext().GetTo().GetServerID())
			{
				svrErr( ResultCode::SVR_INVALID_SERVERID );
			}

			hr = FindEntity(MessageClass::GetRouteContext().GetTo().GetEntityID(), pEntity);
			if (!(hr))
			{
				svrTrace(Trace::TRC_WARN, "Can't find transaction target instance:{0}", MessageClass::GetRouteContext().GetTo());
				goto Proc_End;
			}

			svrChk(super::InitializeTransaction((Svr::Entity*)pEntity));


		Proc_End:

			return hr;
		}


		virtual void Release()
		{
			delete this;
		}

		template< class ServerEntityType >
		ServerEntityType *GetServerEntity()
		{
			ServerEntityType *pSvrEnt = nullptr;
			pSvrEnt = dynamic_cast<ServerEntityType*>(super::GetServerEntity());
			Assert(pSvrEnt);
			return pSvrEnt;
		}

		PolicyClass* GetInterface()
		{
			return GetInterface<PolicyClass>();
		}


		template< class PolicyType >
		PolicyType* GetInterface()
		{
			SharedPointerT<Net::Connection> pConn;
			super::GetServerEntity()->GetConnectionShared(pConn);
			if (pConn != nullptr)
				return pConn->GetInterface<PolicyType>();
			return nullptr;
		}

		FORCEINLINE GameInstanceEntity* GetMyOwner()
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




}; // GameServer
}; // BR




