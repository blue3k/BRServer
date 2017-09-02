////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2017 StromFOrge
// 
// Author : Generated
// 
// Description : GameInstanceManager Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "SFTypedefs.h"
#include "Net/NetDef.h"
#include "Net/Message.h"
#include "Net/Connection.h"
#include "Protocol/SvrProtocol.h"
#include "Protocol/Policy/GameInstanceManagerNetPolicy.h"
#include "Protocol/Message/GameInstanceManagerMsgClass.h"
#include "ProtocolTrace.h"



namespace SF
{
 	namespace Policy
	{
 		// Cmd: Create a game instance
		Result NetPolicyGameInstanceManager::CreateGameCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const uint16_t &InNumberOfBotPlayer, const uint16_t &InMaxPlayer )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::GameInstanceManager::CreateGameCmd::Create(m_pConnection->GetMemoryManager(), InRouteContext, InTransactionID, InRouteHopCount, InNumberOfBotPlayer, InMaxPlayer);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetPolicyGameInstanceManager::CreateGameCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const uint16_t &InNumberOfBotPlayer, const uint16_t &InMaxPlayer )
		// C2S: Game instance notify of deletion
		Result NetPolicyGameInstanceManager::GameDeletedC2SEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::GameInstanceManager::GameDeletedC2SEvt::Create(m_pConnection->GetMemoryManager(), InRouteContext, InRouteHopCount);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetPolicyGameInstanceManager::GameDeletedC2SEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount )


		// Cmd: Create a game instance
		Result NetSvrPolicyGameInstanceManager::CreateGameRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::GameInstanceManager::CreateGameRes::Create(m_pConnection->GetMemoryManager(), InRouteContext, InTransactionID, InResult);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGameInstanceManager::CreateGameRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )


	}; // namespace Policy
}; // namespace SF


