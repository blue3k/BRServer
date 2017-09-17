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


#include "SFTypedefs.h"
#include "Util/TimeUtil.h"
#include "ResultCode/SFResultCodeSvr.h"
#include "Net/NetDef.h"
#include "Memory/MemoryPool.h"
#include "Net/Message.h"
#include "Types/BrSvrTypes.h"

#include "SvrConst.h"
#include "SvrTrace.h"
#include "Types/BrSvrTypes.h"
#include "Service/EngineService.h"

#include "Transaction/Transaction.h"
#include "ServiceEntity/ClusteredServiceEntity.h"
#include "ServiceEntity/ClusterManagerServiceEntity.h"



namespace SF {
namespace Svr {

	class Entity;
	class TransactionResult;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// Message transaction template
	//

	// This transaction will work on server entity by default. Call SetWorkOnServerEntity to change it
	template< class OwnerEntityType, class MessageClass >
	class ServerEntityMessageTransaction : public TransactionT<OwnerEntityType>, public MessageClass
	{
	public:
		typedef TransactionT<OwnerEntityType> superTrans;

	protected:
		// Select which entity will work
		bool				m_WorkOnServerEntity;

	public:
		ServerEntityMessageTransaction(IHeap& memoryManager, MessageDataPtr &pIMsg )
			:superTrans( memoryManager, TransactionID() )
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
			if ((hr))
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

		const SharedPointerAtomicT<Net::Connection>& GetConnection()
		{
			static const SharedPointerAtomicT<Net::Connection> Dummy;
			auto serverEntity = GetServerEntity();
			if (serverEntity != nullptr)
				return serverEntity->GetConnection();

			return Dummy;
		}

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

	template< class OwnerEntityType, class MessageClass >
	class ClusterEntityMessageTransaction : public ServerEntityMessageTransaction<OwnerEntityType,MessageClass>
	{
	private:
		typedef ServerEntityMessageTransaction<OwnerEntityType, MessageClass> super;

	public:
		ClusterEntityMessageTransaction(IHeap& memMgr, MessageDataPtr &pIMsg )
			:super( memMgr, pIMsg )
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
			ClusteredServiceEntity *pMyOwner = nullptr;
			MessageDataPtr pClonedMessage;
			pMyOwner = super::GetMyOwner();


			svrChkPtr(pService);

			if( pService->GetEntityUID() == pMyOwner->GetEntityUID() )
				goto Proc_End;

			auto& pConn = pService->GetServerEntity()->GetConnection();
			if( pConn == nullptr )
			{
				svrTrace( Error, "Failed routing a message({0}) for {1}", super::GetMessage()->GetMessageHeader()->msgID, typeid(*pMyOwner).name() );
				svrErr(ResultCode::SVR_CLUSTER_NOTREADY);
			}

			if (pConn->GetConnectionState() != Net::ConnectionState::CONNECTED)
				goto Proc_End;

			MessageClass::OverrideRouteInformation( pService->GetEntityUID(), super::GetRouteHopCount() + 1 );

			pClonedMessage = super::GetMessage()->Clone(GetMemoryManager());
			if( pClonedMessage == nullptr )
			{
				svrTrace( Error, "Failed routing a message({0}) for {1}, Out of memory", super::GetMessage()->GetMessageHeader()->msgID, typeid(*pMyOwner).name() );
				goto Proc_End;
			}

			Assert(pClonedMessage->GetReferenceCount() == 1);
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
				if( pMyOwner->GetClusterType() == ClusterType::Ring && !(hrRes) )
				{
					hr = TossToNextRing();
					if( !(hr) ) 
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
				if( (hrRes) 
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
			ClusterServiceInfo* pServiceInfo = nullptr;
			ClusteredServiceEntity *pMyOwner = nullptr;
			pMyOwner = super::GetMyOwner();

			// Only the master and an entity server can broadcast in replica model
			if( pMyOwner->GetClusterType() == ClusterType::Replication && pMyOwner->GetClusterMembership() != ClusterMembership::Master )
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
			//	if( (pMyOwner->FindService( GetSender(), pSender )) )
			//	{
			//		//if( pSender->GetClusterMembership() >= ClusterMembership::StatusWatcher )
			//		//{
			//		//	return hr;
			//		//}
			//	}
			//}

			svrChk(Service::ClusterManager->GetClusterInfo(pMyOwner->GetGameID(), pMyOwner->GetClusterID(), pServiceInfo));
			for(auto itService : pServiceInfo->Services) //pServiceInfo->Services.ForeachOrder(0, pServiceInfo->Services..size(), [&]( FixedString nodeName, ServerServiceInformation* pService )
			{
				auto pService = itService;
				if( pService->GetEntityUID() == pMyOwner->GetEntityUID() )
					continue;

				// Only status write will be broadcasted to the watchers
				if( pService->GetClusterMembership() == ClusterMembership::StatusWatcher
					&& (super::GetMessageUsage() != Message::MessageUsage_ClusterStatusWrite ) )
				{
					continue;
				}

				TossMessageToTarget( pService );
			};

		Proc_End:

			return hr;
		}

		// Toss the message to next ring
		Result TossToNextRing()
		{
			Result hr = ResultCode::SUCCESS;

			RingClusterServiceEntity *pMyOwner = nullptr;
			ServerServiceInformation *pNextService = nullptr;

			svrAssert( pMyOwner->GetClusterType() != ClusterType::Ring );

			pMyOwner = dynamic_cast<RingClusterServiceEntity*>(super::GetMyOwner());
			svrChkPtr(pMyOwner);

			//pMyOwner->GetNextRing( pMyOwner->GetMyServiceInfo(), pNextService );
			svrChk(Service::ClusterManager->GetNextService(pMyOwner->GetMyServiceInfo(), pNextService));
			if( pNextService )
			{
				TossMessageToTarget( pNextService );
			}

		Proc_End:

			return hr;
		}
	};





}; // namespace Svr
}; // namespace SF




