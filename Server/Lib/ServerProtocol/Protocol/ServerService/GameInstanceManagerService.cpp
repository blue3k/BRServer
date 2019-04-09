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
		Result GameInstanceManagerService::CreateGameCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const uint16_t &InNumberOfBotPlayer, const uint16_t &InMaxPlayer )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyGameInstanceManager(GetConnection()).CreateGameCmd( InRouteContext, InTransactionID, InRouteHopCount, InNumberOfBotPlayer, InMaxPlayer ) );

		Proc_End:

			return hr;

		}; // Result GameInstanceManagerService::CreateGameCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const uint16_t &InNumberOfBotPlayer, const uint16_t &InMaxPlayer )
		// C2S: Game instance notification of deletion
		Result GameInstanceManagerService::GameDeletedC2SEvt( const EntityID &InSenderEntityID, const uint16_t &InRouteHopCount )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyGameInstanceManager(GetConnection()).GameDeletedC2SEvt( InRouteContext, InRouteHopCount ) );

		Proc_End:

			return hr;

		}; // Result GameInstanceManagerService::GameDeletedC2SEvt( const EntityID &InSenderEntityID, const uint16_t &InRouteHopCount )


	}; // namespace Svr
}; // namespace SF


