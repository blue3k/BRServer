////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 Kyungkun Ko
// 
// Author : Generated
// 
// Description : RelayServer Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "ServerProtocolPCH.h"
#include "SFTypedefs.h"
#include "Protocol/SFProtocol.h"
#include "ServerEntity/ServerEntity.h"
#include "Server/BrServer.h"
#include "Server/BrServerUtil.h"
#include "Entity/EntityInformation.h"
#include "Protocol/ServerService/RelayServerService.h"
#include "SvrTrace.h"



namespace SF
{
 	namespace Svr
	{
 		RelayServerService::RelayServerService( ServerServiceInformation* pService)
			: ServerServiceBase(pService)
		{
		}


		// Cmd: Relay Instance
		Result RelayServerService::CreateRelayInstanceCmd( const uint64_t &InTransactionID, const PlayerInformation &InPlayerInfo, const uint64_t &InRelayScore )
		{
 			FunctionContext hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyRelayServer(GetConnection()).CreateRelayInstanceCmd( InRouteContext, InTransactionID, InPlayerInfo, InRelayScore ) );

			return hr;

		}; // Result RelayServerService::CreateRelayInstanceCmd( const uint64_t &InTransactionID, const PlayerInformation &InPlayerInfo, const uint64_t &InRelayScore )
		// Cmd: Add a player to Relay
		Result RelayServerService::AddPlayerCmd( const uint64_t &InTransactionID, const PlayerInformation &InPlayerInfo, const uint64_t &InRelayScore )
		{
 			FunctionContext hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyRelayServer(GetConnection()).AddPlayerCmd( InRouteContext, InTransactionID, InPlayerInfo, InRelayScore ) );

			return hr;

		}; // Result RelayServerService::AddPlayerCmd( const uint64_t &InTransactionID, const PlayerInformation &InPlayerInfo, const uint64_t &InRelayScore )
		// Cmd: Remove a player to Relay
		Result RelayServerService::RemovePlayerCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID )
		{
 			FunctionContext hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyRelayServer(GetConnection()).RemovePlayerCmd( InRouteContext, InTransactionID, InPlayerID ) );

			return hr;

		}; // Result RelayServerService::RemovePlayerCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID )


	}; // namespace Svr
}; // namespace SF


