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
		Result NetPolicyGamePartyManager::CreatePartyCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const UINT16 &InRouteHopCount, const PlayerInformation &InCreator )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::GamePartyManager::CreatePartyCmd::BuildIMsg(pMsg, InRouteContext, InTransactionID, InRouteHopCount, InCreator));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyGamePartyManager::CreatePartyCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const UINT16 &InRouteHopCount, const PlayerInformation &InCreator )
		// C2S: Party instance notify of deletion
		Result NetPolicyGamePartyManager::PartyDeletedC2SEvt( const RouteContext &InRouteContext, const UINT16 &InRouteHopCount )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::GamePartyManager::PartyDeletedC2SEvt::BuildIMsg(pMsg, InRouteContext, InRouteHopCount));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyGamePartyManager::PartyDeletedC2SEvt( const RouteContext &InRouteContext, const UINT16 &InRouteHopCount )


		// Cmd: Create a party instance
		Result NetSvrPolicyGamePartyManager::CreatePartyRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::GamePartyManager::CreatePartyRes::BuildIMsg(pMsg, InRouteContext, InTransactionID, InResult));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGamePartyManager::CreatePartyRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )


	}; // namespace Policy
}; // namespace BR


