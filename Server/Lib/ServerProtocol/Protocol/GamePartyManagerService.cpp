////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) Kyungkun Ko
// 
// Author : Generated
// 
// Description : GamePartyManager Server service
// 
////////////////////////////////////////////////////////////////////////////////


#include "ServerProtocolPCH.h"
#include "SFTypedefs.h"
#include "Protocol/SFProtocol.h"
#include "Net/SFMessageEndpoint.h"
#include "Server/BrServer.h"
#include "Server/BrServerUtil.h"
#include "Protocol/GamePartyManagerService.h"
#include "SvrTrace.h"



namespace SF
{
 	GamePartyManagerService::GamePartyManagerService( ServerServiceInformation* pService)
		: ServerServiceBase(pService)
	{
	}


	// Cmd: Create a party instance
	Result GamePartyManagerService::CreatePartyCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const uint32_t &InGameID, const PlayerInformation &InCreator )
	{
 		ScopeContext hr;

		RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
		svrCheck(NetPolicyGamePartyManager(GetTargetEndpoint()).CreatePartyCmd( InRouteContext, InTransactionID, InRouteHopCount, InGameID, InCreator ) );

		return hr;

	}; // Result GamePartyManagerService::CreatePartyCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const uint32_t &InGameID, const PlayerInformation &InCreator )
	// C2S: Party instance notify of deletion. Sent by party instance
	Result GamePartyManagerService::PartyDeletedC2SEvt( const EntityID &InSenderEntityID, const uint16_t &InRouteHopCount )
	{
 		ScopeContext hr;

		RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
		svrCheck(NetPolicyGamePartyManager(GetTargetEndpoint()).PartyDeletedC2SEvt( InRouteContext, InRouteHopCount ) );

		return hr;

	}; // Result GamePartyManagerService::PartyDeletedC2SEvt( const EntityID &InSenderEntityID, const uint16_t &InRouteHopCount )


}; // namespace SF


