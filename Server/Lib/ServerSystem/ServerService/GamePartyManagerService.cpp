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
		Result GamePartyManagerService::CreatePartyCmd( const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const PlayerInformation &InCreator )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(GetPolicyGamePartyManager()->CreatePartyCmd( InRouteContext, InTransactionID, InRouteHopCount, InCreator ) );

		Proc_End:

			return hr;

		}; // Result GamePartyManagerService::CreatePartyCmd( const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const PlayerInformation &InCreator )
		// C2S: Party instance notify of deletion
		Result GamePartyManagerService::PartyDeletedC2SEvt( const EntityID &InSenderEntityID, const uint16_t &InRouteHopCount )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
			svrChk(GetPolicyGamePartyManager()->PartyDeletedC2SEvt( InRouteContext, InRouteHopCount ) );

		Proc_End:

			return hr;

		}; // Result GamePartyManagerService::PartyDeletedC2SEvt( const EntityID &InSenderEntityID, const uint16_t &InRouteHopCount )


	}; // namespace Svr
}; // namespace BR


