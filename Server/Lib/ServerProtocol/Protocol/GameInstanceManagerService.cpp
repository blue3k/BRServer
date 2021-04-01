////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) Kyungkun Ko
// 
// Author : Generated
// 
// Description : GameInstanceManager Server service
// 
////////////////////////////////////////////////////////////////////////////////


#include "ServerProtocolPCH.h"
#include "SFTypedefs.h"
#include "Protocol/SFProtocol.h"
#include "Net/SFMessageEndpoint.h"
#include "Server/BrServer.h"
#include "Server/BrServerUtil.h"
#include "Protocol/GameInstanceManagerService.h"
#include "SvrTrace.h"



namespace SF
{
 	GameInstanceManagerService::GameInstanceManagerService( ServerServiceInformation* pService)
		: ServerServiceBase(pService)
	{
	}


	// Cmd: Create a game instance
	Result GameInstanceManagerService::CreateGameInstanceCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const uint32_t &InZoneTableId, const VariableTable &InAttributes )
	{
 		ScopeContext hr;

		RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
		svrCheck(NetPolicyGameInstanceManager(GetTargetEndpoint()).CreateGameInstanceCmd( InRouteContext, InTransactionID, InRouteHopCount, InZoneTableId, InAttributes ) );

		return hr;

	}; // Result GameInstanceManagerService::CreateGameInstanceCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const uint32_t &InZoneTableId, const VariableTable &InAttributes )


}; // namespace SF


