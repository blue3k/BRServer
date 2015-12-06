////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : GamePartyManager Message debug implementations
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
#include "ServerSystem/ServerService/GamePartyManagerService.h"
#include "ServerSystem/SvrTrace.h"



namespace BR
{
 	namespace Svr
	{
 		GamePartyManagerService::GamePartyManagerService( ServerServiceInformation* pService)
			: ServerServiceBase(pService, Policy::IPolicyGamePartyManager::ID_POLICY)
		{
			static_assert((UINT)Policy::IPolicyGamePartyManager::ID_POLICY == (UINT)ID_SERVICEPOLICY,"Invalid Policy ID for a Servicebase ");
			Assert(GetPolicyGamePartyManager());
		}


		// Cmd: Create a party instance
		HRESULT GamePartyManagerService::CreatePartyCmd( const Context &InContext, const UINT16 &InRouteHopCount, const PlayerInformation &InCreator )
		{
 			HRESULT hr = S_OK;

			TransactionID localTransID(InContext);
			RouteContext InRouteContext( EntityUID(GetMyServerID(),localTransID.GetEntityID()), GetServiceEntityUID() );
			svrChk(GetPolicyGamePartyManager()->CreatePartyCmd( InContext, InRouteContext, InRouteHopCount, InCreator ) );

		Proc_End:

			return hr;

		}; // HRESULT GamePartyManagerService::CreatePartyCmd( const Context &InContext, const UINT16 &InRouteHopCount, const PlayerInformation &InCreator )
		// C2S: Party instance notify of deletion
		HRESULT GamePartyManagerService::PartyDeletedC2SEvt( const EntityID &InSenderEntityID, const UINT16 &InRouteHopCount )
		{
 			HRESULT hr = S_OK;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
			svrChk(GetPolicyGamePartyManager()->PartyDeletedC2SEvt( InRouteContext, InRouteHopCount ) );

		Proc_End:

			return hr;

		}; // HRESULT GamePartyManagerService::PartyDeletedC2SEvt( const EntityID &InSenderEntityID, const UINT16 &InRouteHopCount )


	}; // namespace Svr
}; // namespace BR


