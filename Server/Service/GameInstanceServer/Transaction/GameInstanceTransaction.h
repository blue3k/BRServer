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
#include "GameInstance/GameInstanceEntity.h"


namespace BR {
namespace Net {
	class IConnection;
	class Connection;
}};


namespace BR {
namespace ConspiracyGameInstanceServer {

	class GamePlayer;
	class GameInstanceEntity;



	// Message transaction template
	template< class OwnerType, class PolicyClass, class MessageClass, class MemoryPoolClass, size_t MessageHandlerBufferSize = sizeof(Svr::TransactionMessageHandlerType)*2 >
	class RoutedGamePlayerMessageTransaction : public Svr::TransactionT<OwnerType,MemoryPoolClass,MessageHandlerBufferSize>, public MessageClass
	{
	protected:
		// Player Player ID
		PlayerID	m_PlayerID;
		// Route entity
		Svr::ServerEntity	*m_ServerEntity;

	public:
		RoutedGamePlayerMessageTransaction( Message::MessageData* &pIMsg )
			:TransactionT( TransactionID() )
			,MessageClass( pIMsg )
			,m_PlayerID(0)
			,m_ServerEntity(nullptr)
		{
		}

		HRESULT ParseMessage()
		{
			HRESULT hr = ParseMsg();
			if (SUCCEEDED(hr))
			{
				if (GetMessage()->GetMessageHeader()->msgID.IDs.Type == BR::Message::MSGTYPE_COMMAND)
				{
					__if_exists(MessageClass::GetContext)
					{
						SetParentTransID(GetContext());
					};
					__if_exists(MessageClass::GetRouteContext)
					{
						SetMessageRouteContext(GetRouteContext());
					};
				}
			}
			return hr;
		}

		// Initialize Transaction
		virtual HRESULT InitializeTransaction( Svr::Entity* pOwner )
		{
			HRESULT hr = S_OK;
			SharedPointerT<Svr::Entity> pEntity;

			svrChkPtr( pOwner );

			svrChk(ParseMessage());

			svrChkPtr( m_ServerEntity = dynamic_cast<Svr::ServerEntity*>(pOwner) );

			if( GetMyServer()->GetServerUID() != GetRouteContext().To.SvrID )
			{
				svrErr( E_SVR_INVALID_SERVERID );
			}

			hr = GetMyServer()->GetEntityTable().Find(GetRouteContext().To.EntityID, pEntity);
			if (FAILED(hr))
			{
				svrTrace(Trace::TRC_WARN, "Can't find transaction target instance:%0%", GetRouteContext().To);
				goto Proc_End;
			}

			svrChk(__super::InitializeTransaction((Svr::Entity*)pEntity));


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
			pSvrEnt = dynamic_cast<ServerEntityType*>(m_ServerEntity);
			Assert(pSvrEnt);
			return pSvrEnt;
		}

		FORCEINLINE PolicyClass* GetPolicy()
		{
			return GetPolicy<PolicyClass>();
		}


		template< class PolicyType >
		PolicyType* GetPolicy()
		{
			SharedPointerT<Net::Connection> pConn;
			m_ServerEntity->GetConnectionShared(pConn);
			if (pConn != nullptr)
				return pConn->GetPolicy<PolicyType>();
			return nullptr;
			//return m_ServerEntity->GetConnection()->GetPolicy<PolicyType>();
		}

		FORCEINLINE GameInstanceEntity* GetMyOwner()
		{
			Assert( GetOwnerEntity() );
			return (GameInstanceEntity*)GetOwnerEntity();
		}

		HRESULT GetMyPlayer( GamePlayer* &pPlayer )
		{
			__if_exists( MessageClass::GetPlayerID )
			{
				return GetMyOwner()->FindPlayer( GetPlayerID(), pPlayer );
			}
			__if_not_exists( MessageClass::GetPlayerID )
			{
				pPlayer = nullptr;
				return E_FAIL;
			}
		}
	};




}; // GameServer
}; // BR




