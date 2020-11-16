////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 Kyungkun Ko
// 
// Author : Generated
// 
// Description : GameInstanceManager Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "ServerProtocolPCH.h"
#include "SFTypedefs.h"
#include "Protocol/SFProtocol.h"
#include "ServerEntity/ServerEntity.h"
#include "Server/BrServer.h"
#include "Server/BrServerUtil.h"
#include "Entity/EntityInformation.h"
#include "Protocol/ServerService/GameInstanceManagerService.h"
#include "SvrTrace.h"



namespace SF
{
 	namespace Svr
	{
 		GameInstanceManagerService::GameInstanceManagerService( ServerServiceInformation* pService)
			: ServerServiceBase(pService)
		{
		}


		// Cmd: Create a game instance
		Result GameInstanceManagerService::CreateGameInstanceCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const VariableTable &InAttributes )
		{
 			ScopeContext hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
			svrCheck(Policy::NetPolicyGameInstanceManager(GetConnection()).CreateGameInstanceCmd( InRouteContext, InTransactionID, InRouteHopCount, InAttributes ) );

			return hr;

		}; // Result GameInstanceManagerService::CreateGameInstanceCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const VariableTable &InAttributes )
		// Cmd: Search game instance
		Result GameInstanceManagerService::SearchGameInstanceCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const char* InSearchKeyword )
		{
 			ScopeContext hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
			svrCheck(Policy::NetPolicyGameInstanceManager(GetConnection()).SearchGameInstanceCmd( InRouteContext, InTransactionID, InRouteHopCount, InSearchKeyword ) );

			return hr;

		}; // Result GameInstanceManagerService::SearchGameInstanceCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const char* InSearchKeyword )
		// C2S: Game instance notification of deletion
		Result GameInstanceManagerService::GameInstanceDeletedC2SEvt( const EntityID &InSenderEntityID, const uint16_t &InRouteHopCount )
		{
 			ScopeContext hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
			svrCheck(Policy::NetPolicyGameInstanceManager(GetConnection()).GameInstanceDeletedC2SEvt( InRouteContext, InRouteHopCount ) );

			return hr;

		}; // Result GameInstanceManagerService::GameInstanceDeletedC2SEvt( const EntityID &InSenderEntityID, const uint16_t &InRouteHopCount )


	}; // namespace Svr
}; // namespace SF


