////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2017 StromFOrge
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
#include "Net/SFConnection.h"
#include "Protocol/SvrProtocol.h"
#include "Protocol/Policy/PartyMatchingNetPolicy.h"
#include "Protocol/Message/PartyMatchingMsgClass.h"
#include "ProtocolTrace.h"



namespace SF
{
 	namespace Policy
	{
 

		// S2C: Event message will be sent from matching srvice when the pary game mathcing has done.
		Result NetSvrPolicyPartyMatching::PartyGameMatchedS2CEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount )
		{
 			FunctionContext hr;

			 MessageDataPtr pMessage;
			 protocolCheckPtr(m_pConnection);

			 pMessage = SF::Message::PartyMatching::PartyGameMatchedS2CEvt::Create(m_pConnection->GetIOHeap(), InRouteContext, InRouteHopCount);
			 protocolCheckPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

			return hr;

		}; // Result NetSvrPolicyPartyMatching::PartyGameMatchedS2CEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount )
		// S2C: Event message will be sent when a player game matching has successfully finished.
		Result NetSvrPolicyPartyMatching::PlayerGameMatchedS2CEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const PlayerID &InDestPlayerID, const uint64_t &InGameInsUID, const uint8_t &InRequestedRole )
		{
 			FunctionContext hr;

			 MessageDataPtr pMessage;
			 protocolCheckPtr(m_pConnection);

			 pMessage = SF::Message::PartyMatching::PlayerGameMatchedS2CEvt::Create(m_pConnection->GetIOHeap(), InRouteContext, InRouteHopCount, InDestPlayerID, InGameInsUID, InRequestedRole);
			 protocolCheckPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

			return hr;

		}; // Result NetSvrPolicyPartyMatching::PlayerGameMatchedS2CEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const PlayerID &InDestPlayerID, const uint64_t &InGameInsUID, const uint8_t &InRequestedRole )


	}; // namespace Policy
}; // namespace SF


