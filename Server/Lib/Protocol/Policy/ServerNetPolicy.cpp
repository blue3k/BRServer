﻿////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : Server Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "SFTypedefs.h"
#include "Protocol/Protocol.h"
#include "Net/NetDef.h"
#include "Net/Message.h"
#include "Types/BrSvrTypes.h"
#include "Types/BrGameTypes.h"
#include "Protocol/Policy/ServerIPolicy.h"
#include "Protocol/Policy/ServerNetPolicy.h"
#include "Protocol/Message/ServerMsgClass.h"
#include "Protocol/ProtocolTrace.h"



namespace BR
{
 	namespace Policy
	{
 		// Cmd: 
		Result NetPolicyServer::GenericFailureCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Server::GenericFailureCmd::BuildIMsg(pMsg, InRouteContext, InTransactionID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyServer::GenericFailureCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID )
		// C2S: Server Started or Connected
		Result NetPolicyServer::ServerConnectedC2SEvt( const RouteContext &InRouteContext, const ServiceInformation &InClusterManagerServiceInformation, const uint32_t &InStartUpTime, const NetAddress &InPrivateAddress )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Server::ServerConnectedC2SEvt::BuildIMsg(pMsg, InRouteContext, InClusterManagerServiceInformation, InStartUpTime, InPrivateAddress));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyServer::ServerConnectedC2SEvt( const RouteContext &InRouteContext, const ServiceInformation &InClusterManagerServiceInformation, const uint32_t &InStartUpTime, const NetAddress &InPrivateAddress )


		// Cmd: 
		Result NetSvrPolicyServer::GenericFailureRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Server::GenericFailureRes::BuildIMsg(pMsg, InRouteContext, InTransactionID, InResult));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyServer::GenericFailureRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )


	}; // namespace Policy
}; // namespace BR


