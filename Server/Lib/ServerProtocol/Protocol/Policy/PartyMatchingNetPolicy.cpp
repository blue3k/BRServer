////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2017 StromFOrge
// 
// Author : Generated
// 
// Description : PartyMatching Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "SFTypedefs.h"
#include "Net/NetDef.h"
#include "Net/Message.h"
#include "Net/Connection.h"
#include "Protocol/SvrProtocol.h"
#include "Protocol/Policy/PartyMatchingNetPolicy.h"
#include "Protocol/Message/PartyMatchingMsgClass.h"
#include "ProtocolTrace.h"



namespace SF
{
 	namespace Policy
	{
 

		// S2C: 
		Result NetSvrPolicyPartyMatching::PartyGameMatchedS2CEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::PartyMatching::PartyGameMatchedS2CEvt::Create(m_pConnection->GetNetIOHandler().GetIOHeap(), InRouteContext, InRouteHopCount);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyPartyMatching::PartyGameMatchedS2CEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount )
		// S2C: 
		Result NetSvrPolicyPartyMatching::PlayerGameMatchedS2CEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const PlayerID &InDestPlayerID, const uint64_t &InGameInsUID, const uint8_t &InRequestedRole )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::PartyMatching::PlayerGameMatchedS2CEvt::Create(m_pConnection->GetNetIOHandler().GetIOHeap(), InRouteContext, InRouteHopCount, InDestPlayerID, InGameInsUID, InRequestedRole);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyPartyMatching::PlayerGameMatchedS2CEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const PlayerID &InDestPlayerID, const uint64_t &InGameInsUID, const uint8_t &InRequestedRole )


	}; // namespace Policy
}; // namespace SF


