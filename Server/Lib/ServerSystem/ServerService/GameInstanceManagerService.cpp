////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : GameInstanceManager Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "SFTypedefs.h"
#include "Protocol/Protocol.h"
#include "Types/BrBaseTypes.h"
#include "ServerSystem/ServerEntity.h"
#include "ServerSystem/BrServer.h"
#include "ServerSystem/BrServerUtil.h"
#include "ServerSystem/ServiceEntity/EntityInformation.h"
#include "ServerSystem/ServerService/GameInstanceManagerService.h"
#include "ServerSystem/SvrTrace.h"



namespace BR
{
 	namespace Svr
	{
 		GameInstanceManagerService::GameInstanceManagerService( ServerServiceInformation* pService)
			: ServerServiceBase(pService, Policy::IPolicyGameInstanceManager::ID_POLICY)
		{
			static_assert((UINT)Policy::IPolicyGameInstanceManager::ID_POLICY == (UINT)ID_SERVICEPOLICY,"Invalid Policy ID for a Servicebase ");
			Assert(GetPolicyGameInstanceManager());
		}


		// Cmd: Create a game instance
		Result GameInstanceManagerService::CreateGameCmd( const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const uint16_t &InNumberOfBotPlayer, const uint16_t &InMaxPlayer )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(GetPolicyGameInstanceManager()->CreateGameCmd( InRouteContext, InTransactionID, InRouteHopCount, InNumberOfBotPlayer, InMaxPlayer ) );

		Proc_End:

			return hr;

		}; // Result GameInstanceManagerService::CreateGameCmd( const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const uint16_t &InNumberOfBotPlayer, const uint16_t &InMaxPlayer )
		// C2S: Game instance notify of deletion
		Result GameInstanceManagerService::GameDeletedC2SEvt( const EntityID &InSenderEntityID, const uint16_t &InRouteHopCount )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
			svrChk(GetPolicyGameInstanceManager()->GameDeletedC2SEvt( InRouteContext, InRouteHopCount ) );

		Proc_End:

			return hr;

		}; // Result GameInstanceManagerService::GameDeletedC2SEvt( const EntityID &InSenderEntityID, const uint16_t &InRouteHopCount )


	}; // namespace Svr
}; // namespace BR


