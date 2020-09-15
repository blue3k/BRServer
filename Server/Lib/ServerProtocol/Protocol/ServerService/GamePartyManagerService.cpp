////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 Kyungkun Ko
// 
// Author : Generated
// 
// Description : GamePartyManager Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "ServerProtocolPCH.h"
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
		Result GamePartyManagerService::CreatePartyCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const uint32_t &InGameID, const PlayerInformation &InCreator )
		{
 			FunctionContext hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
			svrCheck(Policy::NetPolicyGamePartyManager(GetConnection()).CreatePartyCmd( InRouteContext, InTransactionID, InRouteHopCount, InGameID, InCreator ) );

			return hr;

		}; // Result GamePartyManagerService::CreatePartyCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const uint32_t &InGameID, const PlayerInformation &InCreator )
		// C2S: Party instance notify of deletion. Sent by party instance
		Result GamePartyManagerService::PartyDeletedC2SEvt( const EntityID &InSenderEntityID, const uint16_t &InRouteHopCount )
		{
 			FunctionContext hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
			svrCheck(Policy::NetPolicyGamePartyManager(GetConnection()).PartyDeletedC2SEvt( InRouteContext, InRouteHopCount ) );

			return hr;

		}; // Result GamePartyManagerService::PartyDeletedC2SEvt( const EntityID &InSenderEntityID, const uint16_t &InRouteHopCount )


	}; // namespace Svr
}; // namespace SF


