////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) Kyungkun Ko
// 
// Author : Generated
// 
// Description : RelayServer Server service
// 
////////////////////////////////////////////////////////////////////////////////


#include "ServerProtocolPCH.h"
#include "SFTypedefs.h"
#include "Protocol/SFProtocol.h"
#include "Net/SFMessageEndpoint.h"
#include "Server/BrServer.h"
#include "Server/BrServerUtil.h"
#include "Protocol/RelayServerService.h"
#include "SvrTrace.h"



namespace SF
{
 	RelayServerService::RelayServerService( ServerServiceInformation* pService)
		: ServerServiceBase(pService)
	{
	}


	// Cmd: Relay Instance
	Result RelayServerService::CreateRelayInstanceCmd( const uint64_t &InTransactionID, const PlayerInformation &InPlayerInfo, const uint64_t &InRelayScore )
	{
 		ScopeContext hr;

		RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
		svrCheck(NetPolicyRelayServer(GetTargetEndpoint()).CreateRelayInstanceCmd( InRouteContext, InTransactionID, InPlayerInfo, InRelayScore ) );

		return hr;

	}; // Result RelayServerService::CreateRelayInstanceCmd( const uint64_t &InTransactionID, const PlayerInformation &InPlayerInfo, const uint64_t &InRelayScore )
	// Cmd: Add a player to Relay
	Result RelayServerService::AddPlayerCmd( const uint64_t &InTransactionID, const PlayerInformation &InPlayerInfo, const uint64_t &InRelayScore )
	{
 		ScopeContext hr;

		RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
		svrCheck(NetPolicyRelayServer(GetTargetEndpoint()).AddPlayerCmd( InRouteContext, InTransactionID, InPlayerInfo, InRelayScore ) );

		return hr;

	}; // Result RelayServerService::AddPlayerCmd( const uint64_t &InTransactionID, const PlayerInformation &InPlayerInfo, const uint64_t &InRelayScore )
	// Cmd: Remove a player to Relay
	Result RelayServerService::RemovePlayerCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID )
	{
 		ScopeContext hr;

		RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
		svrCheck(NetPolicyRelayServer(GetTargetEndpoint()).RemovePlayerCmd( InRouteContext, InTransactionID, InPlayerID ) );

		return hr;

	}; // Result RelayServerService::RemovePlayerCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID )


}; // namespace SF


