////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 StormForge
// 
// Author : Generated
// 
// Description : GamePartyManager Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "SFTypedefs.h"
#include "Protocol/SFProtocol.h"
#include "ServerEntity/ServerEntity.h"
#include "Server/BrServer.h"
#include "Server/BrServerUtil.h"
#include "Entity/EntityInformation.h"
#include "Protocol/ServerService/GamePartyManagerService.h"
#include "SvrTrace.h"



namespace SF
{
 	namespace Svr
	{
 		GamePartyManagerService::GamePartyManagerService( ServerServiceInformation* pService)
			: ServerServiceBase(pService)
		{
		}


		// Cmd: Create a party instance
		Result GamePartyManagerService::CreatePartyCmd( const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const uint32_t &InGameID, const PlayerInformation &InCreator )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyGamePartyManager(GetConnection()).CreatePartyCmd( InRouteContext, InTransactionID, InRouteHopCount, InGameID, InCreator ) );

		Proc_End:

			return hr;

		}; // Result GamePartyManagerService::CreatePartyCmd( const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const uint32_t &InGameID, const PlayerInformation &InCreator )
		// C2S: Party instance notify of deletion. Sent by party instance
		Result GamePartyManagerService::PartyDeletedC2SEvt( const EntityID &InSenderEntityID, const uint16_t &InRouteHopCount )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyGamePartyManager(GetConnection()).PartyDeletedC2SEvt( InRouteContext, InRouteHopCount ) );

		Proc_End:

			return hr;

		}; // Result GamePartyManagerService::PartyDeletedC2SEvt( const EntityID &InSenderEntityID, const uint16_t &InRouteHopCount )


	}; // namespace Svr
}; // namespace SF


