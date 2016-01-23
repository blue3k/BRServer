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
		HRESULT GameInstanceManagerService::CreateGameCmd( const Context &InContext, const UINT16 &InRouteHopCount, const UINT16 &InNumberOfBotPlayer, const UINT16 &InMaxPlayer )
		{
 			HRESULT hr = S_SYSTEM_OK;

			TransactionID localTransID(InContext);
			RouteContext InRouteContext( EntityUID(GetMyServerID(),localTransID.GetEntityID()), GetServiceEntityUID() );
			svrChk(GetPolicyGameInstanceManager()->CreateGameCmd( InContext, InRouteContext, InRouteHopCount, InNumberOfBotPlayer, InMaxPlayer ) );

		Proc_End:

			return hr;

		}; // HRESULT GameInstanceManagerService::CreateGameCmd( const Context &InContext, const UINT16 &InRouteHopCount, const UINT16 &InNumberOfBotPlayer, const UINT16 &InMaxPlayer )
		// C2S: Game instance notify of deletion
		HRESULT GameInstanceManagerService::GameDeletedC2SEvt( const EntityID &InSenderEntityID, const UINT16 &InRouteHopCount )
		{
 			HRESULT hr = S_SYSTEM_OK;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
			svrChk(GetPolicyGameInstanceManager()->GameDeletedC2SEvt( InRouteContext, InRouteHopCount ) );

		Proc_End:

			return hr;

		}; // HRESULT GameInstanceManagerService::GameDeletedC2SEvt( const EntityID &InSenderEntityID, const UINT16 &InRouteHopCount )


	}; // namespace Svr
}; // namespace BR


