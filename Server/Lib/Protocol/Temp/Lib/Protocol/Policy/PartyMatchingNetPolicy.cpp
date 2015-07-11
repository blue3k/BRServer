////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : PartyMatching Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/Typedefs.h"
#include "Common/PolicyID.h"
#include "Net/Netdef.h"
#include "Common/Message.h"
#include "Common/BrSvrTypes.h"
#include "Common/BrGameTypes.h"
#include "Protocol/Policy/PartyMatchingIPolicy.h"
#include "Protocol/Policy/PartyMatchingNetPolicy.h"
#include "Protocol/Message/PartyMatchingMsgClass.h"
#include "Protocol/ProtocolTrace.h"



namespace BR
{
 	namespace Policy
	{
 

		// S2C: 
		HRESULT NetSvrPolicyPartyMatching::PartyGameMatchedS2CEvt( const RouteContext &InRouteContext, const UINT16 &InRouteHopCount )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::PartyMatching::PartyGameMatchedS2CEvt::BuildIMsg(pMsg, InRouteContext, InRouteHopCount));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyPartyMatching::PartyGameMatchedS2CEvt( const RouteContext &InRouteContext, const UINT16 &InRouteHopCount )
		// S2C: 
		HRESULT NetSvrPolicyPartyMatching::PlayerGameMatchedS2CEvt( const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const PlayerID &InDestPlayerID, const GameInsUID &InGameInsUID, const PlayerRole &InRequestedRole )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::PartyMatching::PlayerGameMatchedS2CEvt::BuildIMsg(pMsg, InRouteContext, InRouteHopCount, InDestPlayerID, InGameInsUID, InRequestedRole));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyPartyMatching::PlayerGameMatchedS2CEvt( const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const PlayerID &InDestPlayerID, const GameInsUID &InGameInsUID, const PlayerRole &InRequestedRole )


	}; // namespace Policy
}; // namespace BR


