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
#include "Container/SFArray.h"
#include "Types/BrBaseTypes.h"
#include "Common/GameConst.h"
#include "Net/Message.h"
#include "Protocol/Message/ClusterServerMsgClass.h"
#include "Protocol/Policy/ClusterServerIPolicy.h"
#include "ServerSystem/MessageRoute.h"
#include "ServerSystem/ServiceEntity/ClusteredServiceEntity.h"
#include "ServerSystem/ServiceEntity/ClusterManagerServiceEntity.h"
#include "ServerSystem/ServerTransaction.h"



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
	//	Result AddOtherServicesToMe( UINT numServices, const ServiceInformation *pServiceInformations );

	//	virtual Result OnCloseTransaction( Result hrRes ) override;

	//	// Start Transaction
	//	virtual Result StartTransaction();
	//};


	/////////////////////////////////////////////////////////////////////////////
	//
	//	Cluster related transactions
	//
	
	class ClusterGetMemberListTrans : public ClusterEntityMessageTransaction< ClusterManagerServiceEntity, Message::ClusterServer::GetClusterMemberListCmd, ClusterGetMemberListTrans>
	{
	public:

		typedef ClusterEntityMessageTransaction< ClusterManagerServiceEntity, Message::ClusterServer::GetClusterMemberListCmd, ClusterGetMemberListTrans> super;

	private:
		StaticArray<ServiceInformation,50> m_MemberList;

	public:
		ClusterGetMemberListTrans( Message::MessageData* &pIMsg ) : ClusterEntityMessageTransaction( pIMsg ) {}
		virtual ~ClusterGetMemberListTrans() {}

		// Start Transaction
		virtual Result StartTransaction();

		Policy::ISvrPolicyClusterServer* GetPolicy()	{ return ServerEntityMessageTransaction::GetPolicy<Policy::ISvrPolicyClusterServer>(); }

		BR_SVR_MSGTRANS_CLOSE_ARGS(GetClusterMemberListRes,GetRouteContext().GetSwaped(), m_MemberList);
	};


	class JoinClusterTrans : public ClusterEntityMessageTransaction< ClusterManagerServiceEntity, Message::ClusterServer::JoinClusterCmd, JoinClusterTrans>
	{
	public:
		typedef ClusterEntityMessageTransaction< ClusterManagerServiceEntity, Message::ClusterServer::JoinClusterCmd, JoinClusterTrans> super;

	private:
		StaticArray<ServiceInformation,50> m_MemberList;

	public:
		JoinClusterTrans( Message::MessageData* &pIMsg ) : ClusterEntityMessageTransaction( pIMsg ) {}
		virtual ~JoinClusterTrans() {}

		// Start Transaction
		virtual Result StartTransaction();

		Policy::ISvrPolicyClusterServer* GetPolicy()	{ return ServerEntityMessageTransaction::GetPolicy<Policy::ISvrPolicyClusterServer>(); }

		BR_SVR_MSGTRANS_CLOSE_ARGS(JoinClusterRes,GetRouteContext().GetSwaped(), m_MemberList);
	};
	

	class JoinClusterTransForEntityServer : public ClusterEntityMessageTransaction< ClusterManagerServiceEntity, Message::ClusterServer::JoinClusterCmd, JoinClusterTransForEntityServer>
	{
	public:
		typedef ClusterEntityMessageTransaction< ClusterManagerServiceEntity, Message::ClusterServer::JoinClusterCmd, JoinClusterTransForEntityServer> super;

	private:
		StaticArray<ServiceInformation,50> m_MemberList;

	public:
		JoinClusterTransForEntityServer( Message::MessageData* &pIMsg ) : ClusterEntityMessageTransaction( pIMsg ) {}
		virtual ~JoinClusterTransForEntityServer() {}

		// Start Transaction
		virtual Result StartTransaction();

		Policy::ISvrPolicyClusterServer* GetPolicy()	{ return ServerEntityMessageTransaction::GetPolicy<Policy::ISvrPolicyClusterServer>(); }

		BR_SVR_MSGTRANS_CLOSE_ARGS(JoinClusterRes,GetRouteContext().GetSwaped(), m_MemberList);
	};



	
	class SyncClusterServiceTrans : public ClusterEntityMessageTransaction< ClusterManagerServiceEntity, Message::ClusterServer::SyncClusterServiceC2SEvt, SyncClusterServiceTrans, 1>
	{
	private:

	public:
		SyncClusterServiceTrans( Message::MessageData* &pIMsg ) : ClusterEntityMessageTransaction( pIMsg ) {}
		virtual ~SyncClusterServiceTrans() {}

		// Start Transaction
		virtual Result StartTransaction();
	};


} // namespace GameServer 
} // namespace SF 

