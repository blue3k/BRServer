////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) The Braves
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
#include "Net/SFMessageEndpoint.h"
#include "Actor/Movement/SFActorMovement.h"
#include "Protocol/SvrProtocol.h"
#include "Protocol/ServerNetPolicy.h"
#include "Protocol/ServerMsgClass.h"
#include "ProtocolTrace.h"



namespace SF
{
 	// Cmd: Generic failure message
	Result NetPolicyServer::GenericFailureCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::Server::GenericFailureCmd::Create(GetSystemHeap(), InRouteContext, InTransactionID);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetPolicyServer::GenericFailureCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID )
	// C2S: Server Started or Connected
	Result NetPolicyServer::ServerConnectedC2SEvt( const RouteContext &InRouteContext, const uint32_t &InStartUpTime, const NetAddress &InPrivateAddress )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::Server::ServerConnectedC2SEvt::Create(GetSystemHeap(), InRouteContext, InStartUpTime, InPrivateAddress);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetPolicyServer::ServerConnectedC2SEvt( const RouteContext &InRouteContext, const uint32_t &InStartUpTime, const NetAddress &InPrivateAddress )


	// Cmd: Generic failure message
	Result NetSvrPolicyServer::GenericFailureRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::Server::GenericFailureRes::Create(GetSystemHeap(), InRouteContext, InTransactionID, InResult);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetSvrPolicyServer::GenericFailureRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )


}; // namespace SF


