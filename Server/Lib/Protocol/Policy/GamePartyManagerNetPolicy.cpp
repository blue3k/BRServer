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
#include "Net/NetDef.h"
#include "Common/Message.h"
#include "Common/BrSvrTypes.h"
#include "Common/BrGameTypes.h"
#include "Protocol/Policy/GamePartyManagerIPolicy.h"
#include "Protocol/Policy/GamePartyManagerNetPolicy.h"
#include "Protocol/Message/GamePartyManagerMsgClass.h"
#include "Protocol/ProtocolTrace.h"



namespace BR
{
 	namespace Policy
	{
 		// Cmd: Create a party instance
		HRESULT NetPolicyGamePartyManager::CreatePartyCmd( const Context &InContext, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const PlayerInformation &InCreator )
		{
 			HRESULT hr = S_SYSTEM_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::GamePartyManager::CreatePartyCmd::BuildIMsg(pMsg, InContext, InRouteContext, InRouteHopCount, InCreator));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyGamePartyManager::CreatePartyCmd( const Context &InContext, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const PlayerInformation &InCreator )
		// C2S: Party instance notify of deletion
		HRESULT NetPolicyGamePartyManager::PartyDeletedC2SEvt( const RouteContext &InRouteContext, const UINT16 &InRouteHopCount )
		{
 			HRESULT hr = S_SYSTEM_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::GamePartyManager::PartyDeletedC2SEvt::BuildIMsg(pMsg, InRouteContext, InRouteHopCount));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyGamePartyManager::PartyDeletedC2SEvt( const RouteContext &InRouteContext, const UINT16 &InRouteHopCount )


		// Cmd: Create a party instance
		HRESULT NetSvrPolicyGamePartyManager::CreatePartyRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext )
		{
 			HRESULT hr = S_SYSTEM_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::GamePartyManager::CreatePartyRes::BuildIMsg(pMsg, InContext, InResult, InRouteContext));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGamePartyManager::CreatePartyRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext )


	}; // namespace Policy
}; // namespace BR


