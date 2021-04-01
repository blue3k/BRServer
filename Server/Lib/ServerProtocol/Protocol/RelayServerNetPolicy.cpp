////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) The Braves
// 
// Author : Generated
// 
// Description : RelayServer Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "ServerProtocolPCH.h"
#include "SFTypedefs.h"
#include "Net/SFNetDef.h"
#include "Net/SFMessage.h"
#include "Net/SFMessageEndpoint.h"
#include "Actor/Movement/SFActorMovement.h"
#include "Protocol/SvrProtocol.h"
#include "Protocol/RelayServerNetPolicy.h"
#include "Protocol/RelayServerMsgClass.h"
#include "ProtocolTrace.h"



namespace SF
{
 	// Cmd: Relay Instance
	Result NetPolicyRelayServer::CreateRelayInstanceCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerInformation &InPlayerInfo, const uint64_t &InRelayScore )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::RelayServer::CreateRelayInstanceCmd::Create(GetSystemHeap(), InRouteContext, InTransactionID, InPlayerInfo, InRelayScore);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetPolicyRelayServer::CreateRelayInstanceCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerInformation &InPlayerInfo, const uint64_t &InRelayScore )
	// Cmd: Add a player to Relay
	Result NetPolicyRelayServer::AddPlayerCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerInformation &InPlayerInfo, const uint64_t &InRelayScore )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::RelayServer::AddPlayerCmd::Create(GetSystemHeap(), InRouteContext, InTransactionID, InPlayerInfo, InRelayScore);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetPolicyRelayServer::AddPlayerCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerInformation &InPlayerInfo, const uint64_t &InRelayScore )
	// Cmd: Remove a player to Relay
	Result NetPolicyRelayServer::RemovePlayerCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::RelayServer::RemovePlayerCmd::Create(GetSystemHeap(), InRouteContext, InTransactionID, InPlayerID);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetPolicyRelayServer::RemovePlayerCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID )


	// Cmd: Relay Instance
	Result NetSvrPolicyRelayServer::CreateRelayInstanceRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const uint32_t &InRelay )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::RelayServer::CreateRelayInstanceRes::Create(GetSystemHeap(), InRouteContext, InTransactionID, InResult, InRelay);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetSvrPolicyRelayServer::CreateRelayInstanceRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const uint32_t &InRelay )
	// Cmd: Add a player to Relay
	Result NetSvrPolicyRelayServer::AddPlayerRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const uint32_t &InRelay )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::RelayServer::AddPlayerRes::Create(GetSystemHeap(), InRouteContext, InTransactionID, InResult, InRelay);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetSvrPolicyRelayServer::AddPlayerRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const uint32_t &InRelay )
	// Cmd: Remove a player to Relay
	Result NetSvrPolicyRelayServer::RemovePlayerRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::RelayServer::RemovePlayerRes::Create(GetSystemHeap(), InRouteContext, InTransactionID, InResult);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetSvrPolicyRelayServer::RemovePlayerRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )


}; // namespace SF


