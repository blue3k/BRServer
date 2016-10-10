////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Server Transaction definition
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once


#include "Common/Typedefs.h"
#include "Common/TimeUtil.h"
#include "Common/ResultCode/BRResultCodeSvr.h"
#include "Net/NetDef.h"
#include "Common/MemoryPool.h"
#include "Common/Message.h"
#include "Common/BrSvrTypes.h"

#include "ServerSystem/SvrConst.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/SvrTypes.h"


#include "ServerSystem/Transaction.h"
#include "ServerSystem/ServiceEntity/ClusteredServiceEntity.h"



namespace BR {
namespace Svr {

	class Entity;
	class TransactionResult;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// Message transaction template
	//

	// This transaction will work on server entity by default. Call SetWorkOnServerEntity to change it
	template< class OwnerEntityType, class MessageClass, class MemoryPoolClass, size_t MessageHandlerBufferSize = sizeof(Svr::TransactionMessageHandlerType)*2 >
	class ServerEntityMessageTransaction : public TransactionT<OwnerEntityType,MemoryPoolClass,MessageHandlerBufferSize>, public MessageClass
	{
	public:
		typedef TransactionT<OwnerEntityType, MemoryPoolClass, MessageHandlerBufferSize> superTrans;

	protected:
		// Select which entity will work
		bool				m_WorkOnServerEntity;

	public:
		ServerEntityMessageTransaction( Message::MessageData* &pIMsg )
			:superTrans( TransactionID() )
			,MessageClass( pIMsg )
			,m_WorkOnServerEntity(true)
		{
		}

		virtual ~ServerEntityMessageTransaction()
		{
		}

		// 
		inline void SetWorkOnServerEntity( bool workOnServerEntity )
		{
			m_WorkOnServerEntity = workOnServerEntity;
		}

		Result ParseMessage()
		{
			Result hr = MessageClass::ParseMsg();
			if (SUCCEEDED(hr))
			{
				if (MessageClass::GetMessage()->GetMessageHeader()->msgID.IDs.Type == Message::MSGTYPE_COMMAND)
				{
					if(MessageClass::HasTransactionID)
					{
						superTrans::SetParentTransID(MessageClass::GetTransactionID());
					}
					else if(MessageClass::HasRouteContext)
					{
						superTrans::SetMessageRouteContext(MessageClass::GetRouteContext());
					}
				}
			}
			return hr;
		}

		// route function call
		ServerEntity* GetServerEntity() { return superTrans::GetServerEntity(); }

		// Initialize Transaction
		virtual Result InitializeTransaction( Svr::Entity* pOwner )
		{
			Result hr = ResultCode::SUCCESS;

			svrChkPtr( pOwner );

			svrChk(ParseMessage());

			assert(superTrans::GetServerEntity() != nullptr);
			svrChkPtr(superTrans::GetServerEntity());

			if( m_WorkOnServerEntity )
			{
				svrAssert(dynamic_cast<OwnerEntityType*>(pOwner));
				svrChk(superTrans::InitializeTransaction( pOwner ) );
			}
			else
			{
				if(MessageClass::HasRouteContext)
				{
					assert(pOwner->GetEntityUID() == MessageClass::GetRouteContext().GetTo());
					svrAssert(dynamic_cast<OwnerEntityType*>(pOwner));
					svrChk(superTrans::InitializeTransaction(pOwner));
				}
				else
				{
					svrErr(ResultCode::NOT_IMPLEMENTED);
				}
			}

		Proc_End:

			return hr;
		}


		template< class ServerEntityType >
		ServerEntityType *GetServerEntity()
		{
			ServerEntityType *pSvrEnt = nullptr;
			pSvrEnt = dynamic_cast<ServerEntityType*>(superTrans::GetServerEntity());
			Assert(pSvrEnt);
			return pSvrEnt;
		}

		template< class PolicyType >
		PolicyType* GetPolicy()
		{
			SharedPointerT<Net::Connection> pConn;
			superTrans::GetServerEntity()->GetConnectionShared(pConn);
			if (pConn != nullptr)
				return pConn->GetPolicy<PolicyType>();
			else
				return nullptr;
		}

		FORCEINLINE OwnerEntityType* GetMyOwner()
		{
			Assert(superTrans::GetOwnerEntity() );
			return dynamic_cast<OwnerEntityType*>(superTrans::GetOwnerEntity());
		}

	};


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// Message transaction template
	//

	template< class OwnerEntityType, class MessageClass, class MemoryPoolClass, size_t MessageHandlerBufferSize = sizeof(Svr::TransactionMessageHandlerType)*2 >
	class ClusterEntityMessageTransaction : public ServerEntityMessageTransaction<OwnerEntityType,MessageClass,MemoryPoolClass,MessageHandlerBufferSize>
	{
	private:
		typedef ServerEntityMessageTransaction<OwnerEntityType, MessageClass, MemoryPoolClass, MessageHandlerBufferSize> super;

	public:
		ClusterEntityMessageTransaction( Message::MessageData* &pIMsg )
			:super( pIMsg )
		{
			super::m_WorkOnServerEntity = false;
		}

		// 
		bool IsThisDataMessageTransaction()
		{
			return super::GetMessageUsage() == Message::MessageUsage_ClusterDataWrite || super::GetMessageUsage() == Message::MessageUsage_ClusterDataRead;
		}

		// Toss a message to a target
		Result TossMessageToTarget( ServerServiceInformation* pService )
		{
			Result hr = ResultCode::SUCCESS;
			Net::IConnection *pConn = nullptr;
			ClusteredServiceEntity *pMyOwner = nullptr;
			Message::MessageData *pClonedMessage = nullptr;
			pMyOwner = super::GetMyOwner();


			svrChkPtr(pService);

			if( pService->GetEntityUID() == pMyOwner->GetEntityUID() )
				goto Proc_End;

			pConn = pService->GetServerEntity()->GetConnection();
			if( pConn == nullptr )
			{
				svrTrace( Trace::TRC_ERROR, "Failed routing a message({0}) for {1}", super::GetMessage()->GetMessageHeader()->msgID, typeid(*pMyOwner).name() );
				svrErr(ResultCode::E_SVR_CLUSTER_NOTREADY);
			}

			if (pConn->GetConnectionState() != Net::IConnection::STATE_CONNECTED)
				goto Proc_End;

			MessageClass::OverrideRouteInformation( pService->GetEntityUID(), super::GetRouteHopCount() + 1 );

			pClonedMessage = super::GetMessage()->Clone();
			if( pClonedMessage == nullptr )
			{
				svrTrace( Trace::TRC_ERROR, "Failed routing a message({0}) for {1}, Out of memory", super::GetMessage()->GetMessageHeader()->msgID, typeid(*pMyOwner).name() );
				goto Proc_End;
			}

			Assert(pClonedMessage->GetRefCount() == 1);
			pConn->Send( pClonedMessage );

			//Util::SafeRelease( pClonedMessage );

		Proc_End:

			return hr;
		}

		// Override for the automatic broad cast
		virtual Result CloseTransaction( Result hrRes )
		{
			Result hr = ResultCode::SUCCESS;

			ClusteredServiceEntity *pMyOwner = nullptr;

			svrChkPtr(super::GetOwnerEntity());

			pMyOwner = super::GetMyOwner();

			if(super::IsClosed() ) return hr;

			switch(super::GetMessageUsage() )
			{
			case Message::MessageUsage_ClusterStatusRead:
				// No broadcast in any case
				this->OnCloseTransaction(hrRes);
				break;
			case Message::MessageUsage_ClusterStatusWrite:
				// broadcast all the time
				svrChk( BroadcastToChildren() );
				if( MessageClass::HasRouteHopCount )
				{
					// Hop count 1 is the maximum number with replica clustring model
					if(super::GetRouteHopCount() >= 1 )
						break;

					Assert(super::GetRouteHopCount() == 0 );
				}
				this->OnCloseTransaction(hrRes);
				break;
			case Message::MessageUsage_ClusterDataRead:
				// If data read failed on this we need to send it to Next ring
				if( pMyOwner->GetClusterType() == ClusterType::Ring && FAILED(hrRes) )
				{
					hr = TossToNextRing();
					if( FAILED(hr) ) 
						this->OnCloseTransaction(hrRes);
					svrChk( hr );
				}
				else
				{
					this->OnCloseTransaction(hrRes);
				}
				break;
			case Message::MessageUsage_ClusterDataWrite:
				// All success write must be replicated to slaves
				if( SUCCEEDED(hrRes) 
					&& (pMyOwner->GetClusterType() == ClusterType::Replication || pMyOwner->GetClusterType() == ClusterType::FreeReplication)  )
				{
					svrChk( BroadcastToChildren() );
				}
				if( MessageClass::HasRouteHopCount )
				{
					// Hop count 1 is the maximum number with replica clustring model
					if(super::GetRouteHopCount() >= 1 )
						break;
				}
				this->OnCloseTransaction(hrRes);
				break;
			default:
				this->OnCloseTransaction(hrRes);
				break;
			}

		Proc_End:

			super::SetClosed();

			return hr;
		}

	private:

		Result BroadcastToChildren()
		{
			Result hr = ResultCode::SUCCESS;

			ClusteredServiceEntity *pMyOwner = nullptr;
			pMyOwner = super::GetMyOwner();

			// Only the master and an entity server can broadcast in replica model
			if( pMyOwner->GetClusterType() == ClusterType::Replication && pMyOwner->GetClusterMembership() != ClusterMembership::Master
				&& BrServer::GetInstance()->GetNetClass() != NetClass::Entity )
				return hr;

			if( MessageClass::HasRouteHopCount )
			{
				// Hop count 1 is the maximum number with replica clustring model
				if(super::GetRouteHopCount() >= 1 )
					return hr;
			}

			// Watcher's write request will not be broadcasted
			//if( MessageClass::HasSender )
			//{
			//	ServerServiceInformation *pSender = nullptr;
			//	if( SUCCEEDED(pMyOwner->FindService( GetSender(), pSender )) )
			//	{
			//		//if( pSender->GetClusterMembership() >= ClusterMembership::StatusWatcher )
			//		//{
			//		//	return hr;
			//		//}
			//	}
			//}

			pMyOwner->ForEach( [&]( ServerServiceInformation* pService ) 
			{
				if( pService->GetEntityUID() == pMyOwner->GetEntityUID() )
					return;

				//if( MessageClass::HasSender )
				//{
				//	// Only master will recei
				//	if( pSender != nullptr && pSender->GetClusterMembership() >= ClusterMembership::StatusWatcher
				//		&& pService->GetClusterMembership() != ClusterMembership::Master )
				//		return;
				//}

				// Only status write will be broadcasted to the watchers
				if( pService->GetClusterMembership() == ClusterMembership::StatusWatcher
					&& (super::GetMessageUsage() != Message::MessageUsage_ClusterStatusWrite ) )
				{
					return;
				}

				TossMessageToTarget( pService );
			});

		//Proc_End:

			return hr;
		}

		// Toss to the message to next ring
		Result TossToNextRing()
		{
			Result hr = ResultCode::SUCCESS;

			RingClusterServiceEntity *pMyOwner = nullptr;
			ServerServiceInformation *pNextService = nullptr;

			svrAssert( pMyOwner->GetClusterType() != ClusterType::Ring );

			pMyOwner = dynamic_cast<RingClusterServiceEntity *>(super::GetMyOwner());
			svrChkPtr(pMyOwner);
			pMyOwner->GetNextRing( pMyOwner->GetMyServiceInfo(), pNextService );

			if( pNextService )
			{
				TossMessageToTarget( pNextService );
			}

		Proc_End:

			return hr;
		}
	};






#include "ServerTransaction.inl"

}; // namespace Svr
}; // namespace BR




