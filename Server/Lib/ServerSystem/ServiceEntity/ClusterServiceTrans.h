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
#include "ServerSystem/Transaction.h"
#include "Memory/MemoryPool.h"
#include "Common/ArrayUtil.h"
#include "Types/BrBaseTypes.h"
#include "Common/GameConst.h"
#include "Net/Message.h"
#include "Protocol/Message/ClusterServerMsgClass.h"
#include "Protocol/Policy/ClusterServerIPolicy.h"
#include "ServerSystem/MessageRoute.h"
#include "ServerSystem/ServiceEntity/ClusteredServiceEntity.h"
#include "ServerSystem/ServiceEntity/ClusterManagerServiceEntity.h"
#include "ServerSystem/ServerTransaction.h"



namespace BR {
namespace Svr {



	class ClusterInitializationTrans : public TransactionT<ClusteredServiceEntity, ClusterInitializationTrans, sizeof(TransactionMessageHandlerType)*5>
	{
	public:
		typedef TransactionT<ClusteredServiceEntity, ClusterInitializationTrans, sizeof(TransactionMessageHandlerType) * 5> super;

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
		ClusterInitializationTrans();
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
		Result AddOtherServicesToMe( UINT numServices, const ServiceInformation *pServiceInformations );

		virtual Result OnCloseTransaction( Result hrRes ) override;

		// Start Transaction
		virtual Result StartTransaction() override;
	};


	/////////////////////////////////////////////////////////////////////////////
	//
	//	Cluster related transactions
	//


	class RequestDataSyncTrans : public ClusterEntityMessageTransaction< ClusteredServiceEntity, Message::ClusterServer::RequestDataSyncCmd, RequestDataSyncTrans, 1>
	{
	public:
		typedef ClusterEntityMessageTransaction< ClusteredServiceEntity, Message::ClusterServer::RequestDataSyncCmd, RequestDataSyncTrans, 1> super;

	private:

	public:
		RequestDataSyncTrans( Message::MessageData* &pIMsg ) : ClusterEntityMessageTransaction( pIMsg )
		{
			// let's enable runtime data sync
			//SetExclusive( true );
		}
		virtual ~RequestDataSyncTrans() {}

		// Start Transaction
		virtual Result StartTransaction();

		Policy::ISvrPolicyClusterServer* GetPolicy()	{ return ServerEntityMessageTransaction::GetPolicy<Policy::ISvrPolicyClusterServer>(); }

		BR_SVR_MSGTRANS_CLOSE(RequestDataSyncRes,GetRouteContext().GetSwaped());
	};



	class ClusterMasterAssignedTrans : public ClusterEntityMessageTransaction< ClusteredServiceEntity, Message::ClusterServer::ClusterMasterAssignedS2CEvt, ClusterMasterAssignedTrans, 1>
	{
	public:
		typedef ClusterEntityMessageTransaction< ClusteredServiceEntity, Message::ClusterServer::ClusterMasterAssignedS2CEvt, ClusterMasterAssignedTrans, 1> super;

	private:

	public:
		ClusterMasterAssignedTrans( Message::MessageData* &pIMsg ) : ClusterEntityMessageTransaction( pIMsg ) {}
		virtual ~ClusterMasterAssignedTrans() {}

		// Start Transaction
		virtual Result StartTransaction();
	};



	class ClusterMasterVoteTrans : public ClusterEntityMessageTransaction< ClusteredServiceEntity, Message::ClusterServer::ClusterMasterVoteC2SEvt, ClusterMasterVoteTrans, 1>
	{
	public:
		typedef ClusterEntityMessageTransaction< ClusteredServiceEntity, Message::ClusterServer::ClusterMasterVoteC2SEvt, ClusterMasterVoteTrans, 1> super;

	private:

	public:
		ClusterMasterVoteTrans( Message::MessageData* &pIMsg ) : ClusterEntityMessageTransaction( pIMsg ) {}
		virtual ~ClusterMasterVoteTrans() {}

		// Start Transaction
		virtual Result StartTransaction();
	};


	class ClusterUpdateStatusTrans : public ClusterEntityMessageTransaction< ClusteredServiceEntity, Message::ClusterServer::ClusterUpdateStatusC2SEvt, ClusterUpdateStatusTrans, 1>
	{
	public:
		typedef ClusterEntityMessageTransaction< ClusteredServiceEntity, Message::ClusterServer::ClusterUpdateStatusC2SEvt, ClusterUpdateStatusTrans, 1> super;

	private:

	public:
		ClusterUpdateStatusTrans(Message::MessageData* &pIMsg) : ClusterEntityMessageTransaction(pIMsg) {}
		virtual ~ClusterUpdateStatusTrans() {}

		// Start Transaction
		virtual Result StartTransaction();
	};



	class ClusterUpdateWorkloadTrans : public ClusterEntityMessageTransaction< ClusteredServiceEntity, Message::ClusterServer::ClusterUpdateWorkloadC2SEvt, ClusterUpdateWorkloadTrans, 1>
	{
	public:
		typedef ClusterEntityMessageTransaction< ClusteredServiceEntity, Message::ClusterServer::ClusterUpdateWorkloadC2SEvt, ClusterUpdateWorkloadTrans, 1> super;

	private:

	public:
		ClusterUpdateWorkloadTrans(Message::MessageData* &pIMsg) : ClusterEntityMessageTransaction(pIMsg) {}
		virtual ~ClusterUpdateWorkloadTrans() {}

		// Start Transaction
		virtual Result StartTransaction();
	};


	class GetLowestWorkloadClusterMemberTrans : public ClusterEntityMessageTransaction< ClusteredServiceEntity, Message::ClusterServer::GetLowestWorkloadClusterMemberCmd, GetLowestWorkloadClusterMemberTrans>
	{
	public:
		typedef ClusterEntityMessageTransaction< ClusteredServiceEntity, Message::ClusterServer::GetLowestWorkloadClusterMemberCmd, GetLowestWorkloadClusterMemberTrans> super;

	private:
		ServiceInformation m_LowestMemberInfo;

	public:
		GetLowestWorkloadClusterMemberTrans(Message::MessageData* &pIMsg) : ClusterEntityMessageTransaction(pIMsg)
		{
			// let's enable runtime data sync
			//SetExclusive( true );
		}
		virtual ~GetLowestWorkloadClusterMemberTrans() {}

		// Start Transaction
		virtual Result StartTransaction();

		Policy::ISvrPolicyClusterServer* GetPolicy() { return ServerEntityMessageTransaction::GetPolicy<Policy::ISvrPolicyClusterServer>(); }

		BR_SVR_MSGTRANS_CLOSE_ARGS(GetLowestWorkloadClusterMemberRes, GetRouteContext().GetSwaped(), m_LowestMemberInfo);
	};



	class ClusterNewServerServiceJoinedC2SEvtEntityTrans : public ClusterEntityMessageTransaction< ClusteredServiceEntity, Message::ClusterServer::NewServerServiceJoinedC2SEvt, ClusterNewServerServiceJoinedC2SEvtEntityTrans, 1>
	{
	public:
		typedef ClusterEntityMessageTransaction< ClusteredServiceEntity, Message::ClusterServer::NewServerServiceJoinedC2SEvt, ClusterNewServerServiceJoinedC2SEvtEntityTrans, 1> super;

	private:

	public:
		ClusterNewServerServiceJoinedC2SEvtEntityTrans(Message::MessageData* &pIMsg) : ClusterEntityMessageTransaction(pIMsg) {}
		virtual ~ClusterNewServerServiceJoinedC2SEvtEntityTrans() {}

		// Start Transaction
		virtual Result StartTransaction();
	};


	class ClusterNewServerServiceJoinedC2SEvtTrans : public ClusterEntityMessageTransaction< ClusteredServiceEntity, Message::ClusterServer::NewServerServiceJoinedC2SEvt, ClusterNewServerServiceJoinedC2SEvtTrans, 1>
	{
	public:
		typedef ClusterEntityMessageTransaction< ClusteredServiceEntity, Message::ClusterServer::NewServerServiceJoinedC2SEvt, ClusterNewServerServiceJoinedC2SEvtTrans, 1> super;

	private:

	public:
		ClusterNewServerServiceJoinedC2SEvtTrans(Message::MessageData* &pIMsg) : ClusterEntityMessageTransaction(pIMsg) {}
		virtual ~ClusterNewServerServiceJoinedC2SEvtTrans() {}

		// Start Transaction
		virtual Result StartTransaction();
	};



} // namespace GameServer 
} // namespace BR 

