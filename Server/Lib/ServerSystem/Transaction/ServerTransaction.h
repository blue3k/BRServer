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
#include "Util/SFTimeUtil.h"
#include "ResultCode/SFResultCodeSvr.h"
#include "Net/SFNetDef.h"
#include "Memory/SFMemoryPool.h"
#include "Net/SFMessage.h"
#include "Types/BrSvrTypes.h"

#include "SvrConst.h"
#include "SvrTrace.h"
#include "Types/BrSvrTypes.h"
#include "Service/SFEngineService.h"

#include "Transaction/Transaction.h"
#include "ServiceEntity/ClusteredServiceEntity.h"




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

		using MessageClassType = MessageClass;
		using superTrans = TransactionT<OwnerEntityType>;

	public:
		ServerEntityMessageTransaction(IHeap& memoryManager, MessageDataPtr &pIMsg )
			: superTrans( memoryManager, TransactionID() )
			, MessageClass( std::forward<MessageDataPtr>(pIMsg) )
		{
		}

		virtual ~ServerEntityMessageTransaction()
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

		// Initialize Transaction
		virtual Result InitializeTransaction( Svr::Entity* pOwner )
		{
			Result hr = ResultCode::SUCCESS;

			svrCheckPtr( pOwner );

			svrCheck(ParseMessage());

			if(MessageClass::HasRouteContext)
			{
				assert(pOwner->GetEntityUID() == MessageClass::GetRouteContext().GetTo());
				svrCheckPtr(dynamic_cast<OwnerEntityType*>(pOwner));
				svrCheck(superTrans::InitializeTransaction(pOwner));
			}
			else
			{
				svrCheck(ResultCode::NOT_IMPLEMENTED);
			}

			return hr;
		}


		SF_FORCEINLINE OwnerEntityType* GetMyOwner()
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


			if (pService == nullptr)
				return hr;

			// Skip unexpected message
			if( pService->GetEntityUID() == pMyOwner->GetEntityUID() )
				return hr;

			auto remoteEndpoint = pService->GetTargetEndpoint();
			if(remoteEndpoint == nullptr )
			{
				svrTrace( Error, "Failed routing a message({0}) for {1}", super::GetMessage()->GetMessageHeader()->msgID, typeid(*pMyOwner).name() );
				svrErr(ResultCode::SVR_CLUSTER_NOTREADY);
			}

			MessageClass::OverrideRouteInformation( pService->GetEntityUID(), super::GetRouteHopCount() + 1 );

			pClonedMessage = super::GetMessage()->Clone(Transaction::GetHeap());
			if( pClonedMessage == nullptr )
			{
				svrTrace( Error, "Failed routing a message({0}) for {1}, Out of memory", super::GetMessage()->GetMessageHeader()->msgID, typeid(*pMyOwner).name() );
				goto Proc_End;
			}

			Assert(pClonedMessage->GetReferenceCount() == 1);
			remoteEndpoint->Send( pClonedMessage );

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

			this->OnCloseTransaction(hrRes);
			// TODO: FIXME. For now I am disabling automatic message forwarding
			//switch(super::GetMessageUsage() )
			//{
			//case Message::MessageUsage_ClusterStatusRead:
			//	// No broadcast in any case
			//	this->OnCloseTransaction(hrRes);
			//	break;
			//case Message::MessageUsage_ClusterStatusWrite:
			//	// broadcast all the time
			//	svrChk( BroadcastToChildren() );
			//	if( MessageClass::HasRouteHopCount )
			//	{
			//		// Hop count 1 is the maximum number with replica clustring model
			//		if(super::GetRouteHopCount() >= 1 )
			//			break;

			//		Assert(super::GetRouteHopCount() == 0 );
			//	}
			//	this->OnCloseTransaction(hrRes);
			//	break;
			//case Message::MessageUsage_ClusterDataRead:
			//	// If data read failed on this we need to send it to Next ring
			//	if( pMyOwner->GetClusterType() == ClusterType::Ring && !(hrRes) )
			//	{
			//		hr = TossToNextRing();
			//		if( !(hr) ) 
			//			this->OnCloseTransaction(hrRes);
			//		svrChk( hr );
			//	}
			//	else
			//	{
			//		this->OnCloseTransaction(hrRes);
			//	}
			//	break;
			//case Message::MessageUsage_ClusterDataWrite:
			//	// All success write must be replicated to slaves
			//	if( (hrRes) 
			//		&& (pMyOwner->GetClusterType() == ClusterType::Replication || pMyOwner->GetClusterType() == ClusterType::FreeReplication)  )
			//	{
			//		svrChk( BroadcastToChildren() );
			//	}
			//	if( MessageClass::HasRouteHopCount )
			//	{
			//		// Hop count 1 is the maximum number with replica clustring model
			//		if(super::GetRouteHopCount() >= 1 )
			//			break;
			//	}
			//	this->OnCloseTransaction(hrRes);
			//	break;
			//default:
			//	this->OnCloseTransaction(hrRes);
			//	break;
			//}

		Proc_End:

			super::SetClosed();

			return hr;
		}

	private:

		//Result BroadcastToChildren()
		//{
		//	Result hr = ResultCode::SUCCESS;
		//	ClusterServiceInfo* pServiceInfo = nullptr;
		//	ClusteredServiceEntity *pMyOwner = nullptr;
		//	pMyOwner = super::GetMyOwner();

		//	// Only the master and an entity server can broadcast in replica model
		//	if( pMyOwner->GetClusterType() == ClusterType::Replication && pMyOwner->GetClusterMembership() != ClusterMembership::Master )
		//		return hr;

		//	if( MessageClass::HasRouteHopCount )
		//	{
		//		// Hop count 1 is the maximum number with replica clustring model
		//		if(super::GetRouteHopCount() >= 1 )
		//			return hr;
		//	}

		//	svrChkPtr(pServiceInfo = Service::ServiceDirectory->GetClusterInfo(pMyOwner->GetGameID(), pMyOwner->GetClusterID()));
		//	for(auto itService : pServiceInfo->Services)
		//	{
		//		auto pService = itService.GetValue();
		//		if( pService->GetEntityUID() == pMyOwner->GetEntityUID() )
		//			continue;

		//		//// Only status write will be broadcasted to the watchers
		//		//if( pService->GetClusterMembership() == ClusterMembership::StatusWatcher
		//		//	&& (super::GetMessageUsage() != Message::MessageUsage_ClusterStatusWrite ) )
		//		//{
		//		//	continue;
		//		//}

		//		TossMessageToTarget( pService );
		//	};

		//Proc_End:

		//	return hr;
		//}

		//// Toss the message to next ring
		//Result TossToNextRing()
		//{
		//	Result hr = ResultCode::SUCCESS;

		//	RingClusterServiceEntity *pMyOwner = nullptr;
		//	ServerServiceInformation *pNextService = nullptr;

		//	svrAssert( pMyOwner->GetClusterType() != ClusterType::Ring );

		//	pMyOwner = dynamic_cast<RingClusterServiceEntity*>(super::GetMyOwner());
		//	svrChkPtr(pMyOwner);

		//	//pMyOwner->GetNextRing( pMyOwner->GetMyServiceInfo(), pNextService );
		//	svrChk(Service::ServiceDirectory->GetNextService(pMyOwner->GetMyServiceInfo(), pNextService));
		//	if( pNextService )
		//	{
		//		TossMessageToTarget( pNextService );
		//	}

		//Proc_End:

		//	return hr;
		//}
	};





}; // namespace Svr
}; // namespace SF




