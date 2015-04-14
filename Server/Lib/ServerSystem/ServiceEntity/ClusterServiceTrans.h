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

#include "Common/Typedefs.h"
#include "ServerSystem/Transaction.h"
#include "Common/MemoryPool.h"
#include "Common/ArrayUtil.h"
#include "Common/BRBaseTypes.h"
#include "Common/GameConst.h"
#include "Common/Message.h"
#include "Protocol/Message/ClusterServerMsgClass.h"
#include "Protocol/Policy/ClusterServerIPolicy.h"
#include "ServerSystem/MessageRoute.h"
#include "ServerSystem/ServiceEntity/ClusteredServiceEntity.h"
#include "ServerSystem/ServiceEntity/ClusterManagerServiceEntity.h"
#include "ServerSystem/ServerTransaction.h"



namespace BR {
namespace Svr {



	class ClusterInitializationTrans : public Svr::TransactionT<ClusteredServiceEntity, ClusterInitializationTrans, sizeof(TransactionMessageHandlerType)*5>
	{
	private:
		enum Step {
			Step_GetMemberList,
			Step_JoinCluster,
			Step_RequestDataSync,
			Step_Done,
		} m_Step;

		ServiceInformation m_currentMaster;

		HRESULT m_hr;

	public:
		ClusterInitializationTrans();
		virtual ~ClusterInitializationTrans() {}

		//virtual void Dispose() override;

		// Set timer when it fails
		void SetFailRetryTimer(HRESULT hrRes);

		// Timer handling
		HRESULT OnTimer(TransactionResult* pRes);

		HRESULT GetClusterMemberList();
		HRESULT OnGetClusterMemberList(TransactionResult* pRes);

		HRESULT JoinCluster(  );
		HRESULT OnClusterJoined(TransactionResult* pRes);

		HRESULT RequestDataSync();
		HRESULT OnClusterDataSync(TransactionResult* pRes);

		// Add other services to me
		HRESULT AddOtherServicesToMe( UINT numServices, const ServiceInformation *pServiceInformations );

		virtual HRESULT OnCloseTransaction( HRESULT hrRes ) override;

		// Start Transaction
		virtual HRESULT StartTransaction();
	};


	/////////////////////////////////////////////////////////////////////////////
	//
	//	Cluster related transactions
	//


	class RequestDataSyncTrans : public ClusterEntityMessageTransaction< ClusteredServiceEntity, Message::ClusterServer::RequestDataSyncCmd, RequestDataSyncTrans, 1>
	{
	private:

	public:
		RequestDataSyncTrans( Message::MessageData* &pIMsg ) : ClusterEntityMessageTransaction( pIMsg )
		{
			// let's enable runtime data sync
			//SetExclusive( true );
		}
		virtual ~RequestDataSyncTrans() {}

		// Start Transaction
		virtual HRESULT StartTransaction();

		Policy::ISvrPolicyClusterServer* GetPolicy()	{ return ServerEntityMessageTransaction::GetPolicy<Policy::ISvrPolicyClusterServer>(); }

		BR_IMPLEMENT_MSGTRANS_CLOSE_ARGS(RequestDataSyncRes,GetRouteContext().GetSwaped());
	};



	class ClusterMasterAssignedTrans : public ClusterEntityMessageTransaction< ClusteredServiceEntity, Message::ClusterServer::ClusterMasterAssignedS2CEvt, ClusterMasterAssignedTrans, 1>
	{
	private:

	public:
		ClusterMasterAssignedTrans( Message::MessageData* &pIMsg ) : ClusterEntityMessageTransaction( pIMsg ) {}
		virtual ~ClusterMasterAssignedTrans() {}

		// Start Transaction
		virtual HRESULT StartTransaction();
	};



	class ClusterMasterVoteTrans : public ClusterEntityMessageTransaction< ClusteredServiceEntity, Message::ClusterServer::ClusterMasterVoteC2SEvt, ClusterMasterVoteTrans, 1>
	{
	private:

	public:
		ClusterMasterVoteTrans( Message::MessageData* &pIMsg ) : ClusterEntityMessageTransaction( pIMsg ) {}
		virtual ~ClusterMasterVoteTrans() {}

		// Start Transaction
		virtual HRESULT StartTransaction();
	};




} // namespace GameServer 
} // namespace BR 

