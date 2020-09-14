////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2017 StromFOrge
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
#include "Net/SFConnection.h"
#include "Protocol/SvrProtocol.h"
#include "Protocol/Policy/GamePartyManagerNetPolicy.h"
#include "Protocol/Message/GamePartyManagerMsgClass.h"
#include "ProtocolTrace.h"



namespace SF
{
 	namespace Policy
	{
 		// Cmd: Create a party instance
		Result NetPolicyGamePartyManager::CreatePartyCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const uint32_t &InGameID, const PlayerInformation &InCreator )
		{
 			FunctionContext hr;

			 MessageDataPtr pMessage;
			 protocolCheckPtr(m_pConnection);

			 pMessage = SF::Message::GamePartyManager::CreatePartyCmd::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InRouteHopCount, InGameID, InCreator);
			 protocolCheckPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

			return hr;

		}; // Result NetPolicyGamePartyManager::CreatePartyCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const uint32_t &InGameID, const PlayerInformation &InCreator )
		// C2S: Party instance notify of deletion. Sent by party instance
		Result NetPolicyGamePartyManager::PartyDeletedC2SEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount )
		{
 			FunctionContext hr;

			 MessageDataPtr pMessage;
			 protocolCheckPtr(m_pConnection);

			 pMessage = SF::Message::GamePartyManager::PartyDeletedC2SEvt::Create(m_pConnection->GetIOHeap(), InRouteContext, InRouteHopCount);
			 protocolCheckPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

			return hr;

		}; // Result NetPolicyGamePartyManager::PartyDeletedC2SEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount )


		// Cmd: Create a party instance
		Result NetSvrPolicyGamePartyManager::CreatePartyRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
		{
 			FunctionContext hr;

			 MessageDataPtr pMessage;
			 protocolCheckPtr(m_pConnection);

			 pMessage = SF::Message::GamePartyManager::CreatePartyRes::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InResult);
			 protocolCheckPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

			return hr;

		}; // Result NetSvrPolicyGamePartyManager::CreatePartyRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )


	}; // namespace Policy
}; // namespace SF


