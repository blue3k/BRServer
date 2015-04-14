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



	//class ClusterManagerInitializationTrans : public Svr::TransactionT<ClusterManagerServiceEntity, ClusterManagerInitializationTrans, sizeof(TransactionMessageHandlerType)*5>
	//{
	//private:
	//	enum Step {
	//		Step_GetMemberList,
	//		Step_JoinCluster,
	//		Step_RequestDataSync,
	//	} m_Step;

	//	ServiceInformation m_currentMaster;

	//public:
	//	ClusterManagerInitializationTrans();
	//	virtual ~ClusterManagerInitializationTrans() {}

	//	// Set timer when it fails
	//	void SetFailRetryTimer(HRESULT hrRes);

	//	// Timer handling
	//	HRESULT OnTimer(TransactionResult* pRes);

	//	HRESULT GetClusterMemberList();
	//	HRESULT OnGetClusterMemberList(TransactionResult* pRes);

	//	HRESULT JoinCluster(  );
	//	HRESULT OnClusterJoined(TransactionResult* pRes);

	//	HRESULT RequestDataSync();
	//	HRESULT OnClusterDataSync(TransactionResult* pRes);

	//	// Add other services to me
	//	HRESULT AddOtherServicesToMe( UINT numServices, const ServiceInformation *pServiceInformations );

	//	virtual HRESULT OnCloseTransaction( HRESULT hrRes ) override;

	//	// Start Transaction
	//	virtual HRESULT StartTransaction();
	//};


	/////////////////////////////////////////////////////////////////////////////
	//
	//	Cluster related transactions
	//
	
	class ClusterGetMemberListTrans : public ClusterEntityMessageTransaction< ClusterManagerServiceEntity, Message::ClusterServer::GetClusterMemberListCmd, ClusterGetMemberListTrans>
	{
	private:
		StaticArray<ServiceInformation,50> m_MemberList;

	public:
		ClusterGetMemberListTrans( Message::MessageData* &pIMsg ) : ClusterEntityMessageTransaction( pIMsg ) {}
		virtual ~ClusterGetMemberListTrans() {}

		// Start Transaction
		virtual HRESULT StartTransaction();

		Policy::ISvrPolicyClusterServer* GetPolicy()	{ return ServerEntityMessageTransaction::GetPolicy<Policy::ISvrPolicyClusterServer>(); }

		BR_IMPLEMENT_MSGTRANS_CLOSE_ARGS(GetClusterMemberListRes,GetRouteContext().GetSwaped(), m_MemberList);
	};


	class JoinClusterTrans : public ClusterEntityMessageTransaction< ClusteredServiceEntity, Message::ClusterServer::JoinClusterCmd, JoinClusterTrans>
	{
	private:
		StaticArray<ServiceInformation,50> m_MemberList;

	public:
		JoinClusterTrans( Message::MessageData* &pIMsg ) : ClusterEntityMessageTransaction( pIMsg ) {}
		virtual ~JoinClusterTrans() {}

		// Start Transaction
		virtual HRESULT StartTransaction();

		Policy::ISvrPolicyClusterServer* GetPolicy()	{ return ServerEntityMessageTransaction::GetPolicy<Policy::ISvrPolicyClusterServer>(); }

		BR_IMPLEMENT_MSGTRANS_CLOSE_ARGS(JoinClusterRes,GetRouteContext().GetSwaped(), m_MemberList);
	};
	

	class JoinClusterTransForEntityServer : public ClusterEntityMessageTransaction< ClusterManagerServiceEntity, Message::ClusterServer::JoinClusterCmd, JoinClusterTransForEntityServer>
	{
	private:
		StaticArray<ServiceInformation,50> m_MemberList;

	public:
		JoinClusterTransForEntityServer( Message::MessageData* &pIMsg ) : ClusterEntityMessageTransaction( pIMsg ) {}
		virtual ~JoinClusterTransForEntityServer() {}

		// Start Transaction
		virtual HRESULT StartTransaction();

		Policy::ISvrPolicyClusterServer* GetPolicy()	{ return ServerEntityMessageTransaction::GetPolicy<Policy::ISvrPolicyClusterServer>(); }

		BR_IMPLEMENT_MSGTRANS_CLOSE_ARGS(JoinClusterRes,GetRouteContext().GetSwaped(), m_MemberList);
	};



	class ClusterUpdateStatusTrans : public ClusterEntityMessageTransaction< ClusteredServiceEntity, Message::ClusterServer::ClusterUpdateStatusC2SEvt, ClusterUpdateStatusTrans, 1>
	{
	private:

	public:
		ClusterUpdateStatusTrans( Message::MessageData* &pIMsg ) : ClusterEntityMessageTransaction( pIMsg ) {}
		virtual ~ClusterUpdateStatusTrans() {}

		// Start Transaction
		virtual HRESULT StartTransaction();
	};



	class ClusterUpdateWorkloadTrans : public ClusterEntityMessageTransaction< ClusteredServiceEntity, Message::ClusterServer::ClusterUpdateWorkloadC2SEvt, ClusterUpdateWorkloadTrans, 1>
	{
	private:

	public:
		ClusterUpdateWorkloadTrans( Message::MessageData* &pIMsg ) : ClusterEntityMessageTransaction( pIMsg ) {}
		virtual ~ClusterUpdateWorkloadTrans() {}

		// Start Transaction
		virtual HRESULT StartTransaction();
	};
	

	class GetLowestWorkloadClusterMemberTrans : public ClusterEntityMessageTransaction< ClusteredServiceEntity, Message::ClusterServer::GetLowestWorkloadClusterMemberCmd, GetLowestWorkloadClusterMemberTrans>
	{
	private:
		ServiceInformation m_LowestMemberInfo;

	public:
		GetLowestWorkloadClusterMemberTrans( Message::MessageData* &pIMsg ) : ClusterEntityMessageTransaction( pIMsg )
		{
			// let's enable runtime data sync
			//SetExclusive( true );
		}
		virtual ~GetLowestWorkloadClusterMemberTrans() {}

		// Start Transaction
		virtual HRESULT StartTransaction();

		Policy::ISvrPolicyClusterServer* GetPolicy()	{ return ServerEntityMessageTransaction::GetPolicy<Policy::ISvrPolicyClusterServer>(); }

		BR_IMPLEMENT_MSGTRANS_CLOSE_ARGS(GetLowestWorkloadClusterMemberRes,GetRouteContext().GetSwaped(), m_LowestMemberInfo);
	};
	


	class ClusterNewServerServiceJoinedC2SEvtEntityTrans : public ClusterEntityMessageTransaction< ClusteredServiceEntity, Message::ClusterServer::NewServerServiceJoinedC2SEvt, ClusterNewServerServiceJoinedC2SEvtEntityTrans, 1>
	{
	private:

	public:
		ClusterNewServerServiceJoinedC2SEvtEntityTrans(Message::MessageData* &pIMsg) : ClusterEntityMessageTransaction(pIMsg) {}
		virtual ~ClusterNewServerServiceJoinedC2SEvtEntityTrans() {}

		// Start Transaction
		virtual HRESULT StartTransaction();
	};


	class ClusterNewServerServiceJoinedC2SEvtTrans : public ClusterEntityMessageTransaction< ClusteredServiceEntity, Message::ClusterServer::NewServerServiceJoinedC2SEvt, ClusterNewServerServiceJoinedC2SEvtTrans, 1>
	{
	private:

	public:
		ClusterNewServerServiceJoinedC2SEvtTrans(Message::MessageData* &pIMsg) : ClusterEntityMessageTransaction(pIMsg) {}
		virtual ~ClusterNewServerServiceJoinedC2SEvtTrans() {}

		// Start Transaction
		virtual HRESULT StartTransaction();
	};

	
	class SyncClusterServiceTrans : public ClusterEntityMessageTransaction< ClusterManagerServiceEntity, Message::ClusterServer::SyncClusterServiceC2SEvt, SyncClusterServiceTrans, 1>
	{
	private:

	public:
		SyncClusterServiceTrans( Message::MessageData* &pIMsg ) : ClusterEntityMessageTransaction( pIMsg ) {}
		virtual ~SyncClusterServiceTrans() {}

		// Start Transaction
		virtual HRESULT StartTransaction();
	};


} // namespace GameServer 
} // namespace BR 

