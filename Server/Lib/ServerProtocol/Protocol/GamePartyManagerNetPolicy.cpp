////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) The Braves
// 
// Author : Generated
// 
// Description : GamePartyManager Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "ServerProtocolPCH.h"
#include "SFTypedefs.h"
#include "Net/SFNetDef.h"
#include "Net/SFMessage.h"
#include "Net/SFMessageEndpoint.h"
#include "Actor/Movement/SFActorMovement.h"
#include "Protocol/GamePartyManagerNetPolicy.h"
#include "Protocol/GamePartyManagerMsgClass.h"



namespace SF
{
 	// Cmd: Create a party instance
	Result NetPolicyGamePartyManager::CreatePartyCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const uint32_t &InGameID, const PlayerInformation &InCreator )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::GamePartyManager::CreatePartyCmd::Create(GetSystemHeap(), InRouteContext, InTransactionID, InRouteHopCount, InGameID, InCreator);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

	}; // Result NetPolicyGamePartyManager::CreatePartyCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const uint32_t &InGameID, const PlayerInformation &InCreator )
	// C2S: Party instance notify of deletion. Sent by party instance
	Result NetPolicyGamePartyManager::PartyDeletedC2SEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::GamePartyManager::PartyDeletedC2SEvt::Create(GetSystemHeap(), InRouteContext, InRouteHopCount);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

	}; // Result NetPolicyGamePartyManager::PartyDeletedC2SEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount )


	// Cmd: Create a party instance
	Result NetSvrPolicyGamePartyManager::CreatePartyRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::GamePartyManager::CreatePartyRes::Create(GetSystemHeap(), InRouteContext, InTransactionID, InResult);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

	}; // Result NetSvrPolicyGamePartyManager::CreatePartyRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )


}; // namespace SF


