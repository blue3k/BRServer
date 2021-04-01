////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) The Braves
// 
// Author : Generated
// 
// Description : PartyMatching Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "ServerProtocolPCH.h"
#include "SFTypedefs.h"
#include "Net/SFNetDef.h"
#include "Net/SFMessage.h"
#include "Net/SFMessageEndpoint.h"
#include "Actor/Movement/SFActorMovement.h"
#include "Protocol/SvrProtocol.h"
#include "Protocol/PartyMatchingNetPolicy.h"
#include "Protocol/PartyMatchingMsgClass.h"
#include "ProtocolTrace.h"



namespace SF
{
 

	// S2C: Event message will be sent from matching srvice when the pary game mathcing has done.
	Result NetSvrPolicyPartyMatching::PartyGameMatchedS2CEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::PartyMatching::PartyGameMatchedS2CEvt::Create(GetSystemHeap(), InRouteContext, InRouteHopCount);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetSvrPolicyPartyMatching::PartyGameMatchedS2CEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount )
	// S2C: Event message will be sent when a player game matching has successfully finished.
	Result NetSvrPolicyPartyMatching::PlayerGameMatchedS2CEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const PlayerID &InDestPlayerID, const uint64_t &InGameInsUID, const uint8_t &InRequestedRole )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::PartyMatching::PlayerGameMatchedS2CEvt::Create(GetSystemHeap(), InRouteContext, InRouteHopCount, InDestPlayerID, InGameInsUID, InRequestedRole);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetSvrPolicyPartyMatching::PlayerGameMatchedS2CEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const PlayerID &InDestPlayerID, const uint64_t &InGameInsUID, const uint8_t &InRequestedRole )


}; // namespace SF


