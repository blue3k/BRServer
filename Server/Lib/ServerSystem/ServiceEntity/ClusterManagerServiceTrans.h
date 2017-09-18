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

#include "Transaction/ServerTransaction.h"



namespace SF {
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
	//	void SetFailRetryTimer(Result hrRes);

	//	// Timer handling
	//	Result OnTimer(TransactionResult* pRes);

	//	Result GetClusterMemberList();
	//	Result OnGetClusterMemberList(TransactionResult* pRes);

	//	Result JoinCluster(  );
	//	Result OnClusterJoined(TransactionResult* pRes);

	//	Result RequestDataSync();
	//	Result OnClusterDataSync(TransactionResult* pRes);

	//	// Add other services to me
	//	Result AddOtherServicesToMe( uint numServices, const ServiceInformation *pServiceInformations );

	//	virtual Result OnCloseTransaction( Result hrRes ) override;

	//	// Start Transaction
	//	virtual Result StartTransaction();
	//};


	///////////////////////////////////////////////////////////////////////////////
	////
	////	Cluster related transactions
	////
	//
	//class ClusterGetMemberListTrans : public ServerEntityMessageTransaction< ClusterManagerServiceEntity, Message::ClusterServer::GetClusterMemberListCmd>
	//{
	//public:

	//	typedef ServerEntityMessageTransaction< ClusterManagerServiceEntity, Message::ClusterServer::GetClusterMemberListCmd> super;

	//private:
	//	StaticArray<ServiceInformation,50> m_MemberList;

	//public:
	//	ClusterGetMemberListTrans(IHeap& heap, MessageDataPtr &pIMsg )
	//		: ServerEntityMessageTransaction(heap, pIMsg )
	//		, m_MemberList(heap)
	//	{}
	//	virtual ~ClusterGetMemberListTrans() {}

	//	// Start Transaction
	//	virtual Result StartTransaction();

	//	
	//	BR_SVR_MSGTRANS_CLOSE_ARGS(Policy::NetSvrPolicyClusterServer, GetClusterMemberListRes,GetRouteContext().GetSwaped(), m_MemberList);
	//};


	//class JoinClusterTrans : public ServerEntityMessageTransaction< ClusterManagerServiceEntity, Message::ClusterServer::JoinClusterCmd>
	//{
	//public:
	//	typedef ServerEntityMessageTransaction< ClusterManagerServiceEntity, Message::ClusterServer::JoinClusterCmd> super;

	//private:
	//	StaticArray<ServiceInformation,50> m_MemberList;

	//public:
	//	JoinClusterTrans(IHeap& heap, MessageDataPtr &pIMsg )
	//		: ServerEntityMessageTransaction(heap, pIMsg )
	//		, m_MemberList(heap)
	//	{}
	//	virtual ~JoinClusterTrans() {}

	//	// Start Transaction
	//	virtual Result StartTransaction();


	//	BR_SVR_MSGTRANS_CLOSE_ARGS(Policy::NetSvrPolicyClusterServer, JoinClusterRes, GetRouteContext().GetSwaped(), m_MemberList);
	//};
	//

	//class JoinClusterTransForEntityServer : public ServerEntityMessageTransaction< ClusterManagerServiceEntity, Message::ClusterServer::JoinClusterCmd>
	//{
	//public:
	//	typedef ServerEntityMessageTransaction< ClusterManagerServiceEntity, Message::ClusterServer::JoinClusterCmd> super;

	//private:
	//	StaticArray<ServiceInformation,50> m_MemberList;

	//public:
	//	JoinClusterTransForEntityServer(IHeap& heap, MessageDataPtr &pIMsg )
	//		: ServerEntityMessageTransaction( heap, pIMsg )
	//		, m_MemberList(heap)
	//	{}
	//	virtual ~JoinClusterTransForEntityServer() {}

	//	// Start Transaction
	//	virtual Result StartTransaction();


	//	BR_SVR_MSGTRANS_CLOSE_ARGS(Policy::NetSvrPolicyClusterServer, JoinClusterRes,GetRouteContext().GetSwaped(), m_MemberList);
	//};



	//
	//class SyncClusterServiceTrans : public ServerEntityMessageTransaction< ClusterManagerServiceEntity, Message::ClusterServer::SyncClusterServiceC2SEvt>
	//{
	//private:

	//public:
	//	SyncClusterServiceTrans(IHeap& heap, MessageDataPtr &pIMsg ) : ServerEntityMessageTransaction( heap, pIMsg ) {}
	//	virtual ~SyncClusterServiceTrans() {}

	//	// Start Transaction
	//	virtual Result StartTransaction();
	//};


} // namespace GameServer 
} // namespace SF 

