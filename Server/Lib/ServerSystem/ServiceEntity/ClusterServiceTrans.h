////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : game player entity implementation
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "SFTypedefs.h"
#include "Transaction/Transaction.h"
#include "Memory/MemoryPool.h"
#include "Container/SFArray.h"
#include "Types/SFEngineTypedefs.h"
#include "GameConst.h"
#include "Net/Message.h"
#include "Protocol/Message/ClusterServerMsgClass.h"
#include "Protocol/Policy/ClusterServerNetPolicy.h"
#include "Transaction/MessageRoute.h"
#include "ServiceEntity/ClusteredServiceEntity.h"
#include "ServiceEntity/ClusterManagerServiceEntity.h"
#include "Transaction/ServerTransaction.h"



namespace SF {
namespace Svr {



	class ClusterInitializationTrans : public TransactionT<ClusteredServiceEntity>
	{
	public:
		typedef TransactionT<ClusteredServiceEntity> super;

	private:
		enum Step {
			//Step_GetMemberList,
			Step_JoinCluster,
			Step_RequestDataSync,
			Step_Done,
		} m_Step;

		ServiceInformation m_currentMaster;

		Result m_hr;

	public:
		ClusterInitializationTrans(IHeap& heap);
		virtual ~ClusterInitializationTrans();

		//virtual void Dispose() override;

		// Set timer when it fails
		void SetFailRetryTimer(Result hrRes);

		// Timer handling
		Result OnTimer(TransactionResult* pRes);

		//Result GetClusterMemberList();
		//Result OnGetClusterMemberList(TransactionResult* pRes);

		Result JoinCluster(  );
		Result OnClusterJoined(TransactionResult* pRes);

		Result RequestDataSync();
		Result OnClusterDataSync(TransactionResult* pRes);

		// Add other services to me
		Result AddOtherServicesToMe( uint numServices, const ServiceInformation *pServiceInformations );

		virtual Result OnCloseTransaction( Result hrRes ) override;

		// Start Transaction
		virtual Result StartTransaction() override;
	};


	/////////////////////////////////////////////////////////////////////////////
	//
	//	Cluster related transactions
	//


	class RequestDataSyncTrans : public ClusterEntityMessageTransaction< ClusteredServiceEntity, Message::ClusterServer::RequestDataSyncCmd>
	{
	public:
		typedef ClusterEntityMessageTransaction< ClusteredServiceEntity, Message::ClusterServer::RequestDataSyncCmd> super;

	private:

	public:
		RequestDataSyncTrans(IHeap& heap, MessageDataPtr &pIMsg ) : ClusterEntityMessageTransaction(heap, pIMsg )
		{
			// let's enable runtime data sync
			//SetExclusive( true );
		}
		virtual ~RequestDataSyncTrans() {}

		// Start Transaction
		virtual Result StartTransaction();

		BR_SVR_MSGTRANS_CLOSE(Policy::NetSvrPolicyClusterServer, RequestDataSyncRes,GetRouteContext().GetSwaped());
	};



	class ClusterMasterAssignedTrans : public ClusterEntityMessageTransaction< ClusteredServiceEntity, Message::ClusterServer::ClusterMasterAssignedS2CEvt>
	{
	public:
		typedef ClusterEntityMessageTransaction< ClusteredServiceEntity, Message::ClusterServer::ClusterMasterAssignedS2CEvt> super;

	private:

	public:
		ClusterMasterAssignedTrans(IHeap& heap, MessageDataPtr &pIMsg ) : ClusterEntityMessageTransaction(heap, pIMsg ) {}
		virtual ~ClusterMasterAssignedTrans() {}

		// Start Transaction
		virtual Result StartTransaction();
	};



	class ClusterMasterVoteTrans : public ClusterEntityMessageTransaction< ClusteredServiceEntity, Message::ClusterServer::ClusterMasterVoteC2SEvt>
	{
	public:
		typedef ClusterEntityMessageTransaction< ClusteredServiceEntity, Message::ClusterServer::ClusterMasterVoteC2SEvt> super;

	private:

	public:
		ClusterMasterVoteTrans(IHeap& heap, MessageDataPtr &pIMsg ) : ClusterEntityMessageTransaction(heap, pIMsg ) {}
		virtual ~ClusterMasterVoteTrans() {}

		// Start Transaction
		virtual Result StartTransaction();
	};


	class ClusterUpdateStatusTrans : public ClusterEntityMessageTransaction< ClusteredServiceEntity, Message::ClusterServer::ClusterUpdateStatusC2SEvt>
	{
	public:
		typedef ClusterEntityMessageTransaction< ClusteredServiceEntity, Message::ClusterServer::ClusterUpdateStatusC2SEvt> super;

	private:

	public:
		ClusterUpdateStatusTrans(IHeap& heap, MessageDataPtr &pIMsg) : ClusterEntityMessageTransaction(heap, pIMsg) {}
		virtual ~ClusterUpdateStatusTrans() {}

		// Start Transaction
		virtual Result StartTransaction();
	};



	class ClusterUpdateWorkloadTrans : public ClusterEntityMessageTransaction< ClusteredServiceEntity, Message::ClusterServer::ClusterUpdateWorkloadC2SEvt>
	{
	public:
		typedef ClusterEntityMessageTransaction< ClusteredServiceEntity, Message::ClusterServer::ClusterUpdateWorkloadC2SEvt> super;

	private:

	public:
		ClusterUpdateWorkloadTrans(IHeap& heap, MessageDataPtr &pIMsg) : ClusterEntityMessageTransaction(heap, pIMsg) {}
		virtual ~ClusterUpdateWorkloadTrans() {}

		// Start Transaction
		virtual Result StartTransaction();
	};


	class GetLowestWorkloadClusterMemberTrans : public ClusterEntityMessageTransaction< ClusteredServiceEntity, Message::ClusterServer::GetLowestWorkloadClusterMemberCmd>
	{
	public:
		typedef ClusterEntityMessageTransaction< ClusteredServiceEntity, Message::ClusterServer::GetLowestWorkloadClusterMemberCmd> super;

	private:
		ServiceInformation m_LowestMemberInfo;

	public:
		GetLowestWorkloadClusterMemberTrans(IHeap& heap, MessageDataPtr &pIMsg) : ClusterEntityMessageTransaction(heap, pIMsg)
		{
			// let's enable runtime data sync
			//SetExclusive( true );
		}
		virtual ~GetLowestWorkloadClusterMemberTrans() {}

		// Start Transaction
		virtual Result StartTransaction();

		BR_SVR_MSGTRANS_CLOSE_ARGS(Policy::NetSvrPolicyClusterServer, GetLowestWorkloadClusterMemberRes, GetRouteContext().GetSwaped(), m_LowestMemberInfo);
	};



	class ClusterNewServerServiceJoinedC2SEvtEntityTrans : public ClusterEntityMessageTransaction< ClusteredServiceEntity, Message::ClusterServer::NewServerServiceJoinedC2SEvt>
	{
	public:
		typedef ClusterEntityMessageTransaction< ClusteredServiceEntity, Message::ClusterServer::NewServerServiceJoinedC2SEvt> super;

	private:

	public:
		ClusterNewServerServiceJoinedC2SEvtEntityTrans(IHeap& heap, MessageDataPtr &pIMsg) : ClusterEntityMessageTransaction(heap, pIMsg) {}
		virtual ~ClusterNewServerServiceJoinedC2SEvtEntityTrans() {}

		// Start Transaction
		virtual Result StartTransaction();
	};


	class ClusterNewServerServiceJoinedC2SEvtTrans : public ClusterEntityMessageTransaction< ClusteredServiceEntity, Message::ClusterServer::NewServerServiceJoinedC2SEvt>
	{
	public:
		typedef ClusterEntityMessageTransaction< ClusteredServiceEntity, Message::ClusterServer::NewServerServiceJoinedC2SEvt> super;

	private:

	public:
		ClusterNewServerServiceJoinedC2SEvtTrans(IHeap& heap, MessageDataPtr &pIMsg) : ClusterEntityMessageTransaction(heap, pIMsg) {}
		virtual ~ClusterNewServerServiceJoinedC2SEvtTrans() {}

		// Start Transaction
		virtual Result StartTransaction();
	};



} // namespace GameServer 
} // namespace SF 

