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
#include "Common/Typedefs.h"
#include "Common/PolicyID.h"
#include "Common/BrBaseTypes.h"
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
		Result GameInstanceManagerService::CreateGameCmd( const TransactionID &InTransactionID, const UINT16 &InRouteHopCount, const UINT16 &InNumberOfBotPlayer, const UINT16 &InMaxPlayer )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(GetPolicyGameInstanceManager()->CreateGameCmd( InRouteContext, InTransactionID, InRouteHopCount, InNumberOfBotPlayer, InMaxPlayer ) );

		Proc_End:

			return hr;

		}; // Result GameInstanceManagerService::CreateGameCmd( const TransactionID &InTransactionID, const UINT16 &InRouteHopCount, const UINT16 &InNumberOfBotPlayer, const UINT16 &InMaxPlayer )
		// C2S: Game instance notify of deletion
		Result GameInstanceManagerService::GameDeletedC2SEvt( const EntityID &InSenderEntityID, const UINT16 &InRouteHopCount )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
			svrChk(GetPolicyGameInstanceManager()->GameDeletedC2SEvt( InRouteContext, InRouteHopCount ) );

		Proc_End:

			return hr;

		}; // Result GameInstanceManagerService::GameDeletedC2SEvt( const EntityID &InSenderEntityID, const UINT16 &InRouteHopCount )


	}; // namespace Svr
}; // namespace BR


