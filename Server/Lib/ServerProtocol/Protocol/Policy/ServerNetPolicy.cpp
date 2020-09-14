////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2017 StromFOrge
// 
// Author : Generated
// 
// Description : Server Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "ServerProtocolPCH.h"
#include "SFTypedefs.h"
#include "Net/SFNetDef.h"
#include "Net/SFMessage.h"
#include "Net/SFConnection.h"
#include "Protocol/SvrProtocol.h"
#include "Protocol/Policy/ServerNetPolicy.h"
#include "Protocol/Message/ServerMsgClass.h"
#include "ProtocolTrace.h"



namespace SF
{
 	namespace Policy
	{
 		// Cmd: Generic failure message
		Result NetPolicyServer::GenericFailureCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID )
		{
 			FunctionContext hr;

			 MessageDataPtr pMessage;
			 protocolCheckPtr(m_pConnection);

			 pMessage = SF::Message::Server::GenericFailureCmd::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID);
			 protocolCheckPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

			return hr;

		}; // Result NetPolicyServer::GenericFailureCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID )
		// C2S: Server Started or Connected
		Result NetPolicyServer::ServerConnectedC2SEvt( const RouteContext &InRouteContext, const uint32_t &InStartUpTime, const NetAddress &InPrivateAddress )
		{
 			FunctionContext hr;

			 MessageDataPtr pMessage;
			 protocolCheckPtr(m_pConnection);

			 pMessage = SF::Message::Server::ServerConnectedC2SEvt::Create(m_pConnection->GetIOHeap(), InRouteContext, InStartUpTime, InPrivateAddress);
			 protocolCheckPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

			return hr;

		}; // Result NetPolicyServer::ServerConnectedC2SEvt( const RouteContext &InRouteContext, const uint32_t &InStartUpTime, const NetAddress &InPrivateAddress )


		// Cmd: Generic failure message
		Result NetSvrPolicyServer::GenericFailureRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
		{
 			FunctionContext hr;

			 MessageDataPtr pMessage;
			 protocolCheckPtr(m_pConnection);

			 pMessage = SF::Message::Server::GenericFailureRes::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InResult);
			 protocolCheckPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

			return hr;

		}; // Result NetSvrPolicyServer::GenericFailureRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )


	}; // namespace Policy
}; // namespace SF


