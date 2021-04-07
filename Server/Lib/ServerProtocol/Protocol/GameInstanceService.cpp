////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) Kyungkun Ko
// 
// Author : Generated
// 
// Description : GameInstance Server service
// 
////////////////////////////////////////////////////////////////////////////////


#include "ServerProtocolPCH.h"
#include "SFTypedefs.h"
#include "Protocol/SFProtocol.h"
#include "Net/SFMessageEndpoint.h"
#include "Server/BrServer.h"
#include "Server/BrServerUtil.h"
#include "Protocol/GameInstanceService.h"
#include "SvrTrace.h"



namespace SF
{
 	GameInstanceService::GameInstanceService( ServerServiceInformation* pService)
		: ServerServiceBase(pService)
	{
	}


	// Cmd: Join to a game instance. You can call multiple times, but it would be a waste
	Result GameInstanceService::JoinGameInstanceCmd( const uint64_t &InTransactionID, const PlayerInformation &InPlayer, const VariableTable &InCharacterVisual, const VariableTable &InCharacterAttribute )
	{
 		ScopeContext hr;

		RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
		svrCheck(NetPolicyGameInstance(GetTargetEndpoint()).JoinGameInstanceCmd( InRouteContext, InTransactionID, InPlayer, InCharacterVisual, InCharacterAttribute ) );

		return hr;

	}; // Result GameInstanceService::JoinGameInstanceCmd( const uint64_t &InTransactionID, const PlayerInformation &InPlayer, const VariableTable &InCharacterVisual, const VariableTable &InCharacterAttribute )
	// Cmd: Leave game instance.
	Result GameInstanceService::LeaveGameInstanceCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID )
	{
 		ScopeContext hr;

		RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
		svrCheck(NetPolicyGameInstance(GetTargetEndpoint()).LeaveGameInstanceCmd( InRouteContext, InTransactionID, InPlayerID ) );

		return hr;

	}; // Result GameInstanceService::LeaveGameInstanceCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID )
	// Cmd: Player. revive himself
	Result GameInstanceService::GamePlayerReviveCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID )
	{
 		ScopeContext hr;

		RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
		svrCheck(NetPolicyGameInstance(GetTargetEndpoint()).GamePlayerReviveCmd( InRouteContext, InTransactionID, InPlayerID ) );

		return hr;

	}; // Result GameInstanceService::GamePlayerReviveCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID )


}; // namespace SF


