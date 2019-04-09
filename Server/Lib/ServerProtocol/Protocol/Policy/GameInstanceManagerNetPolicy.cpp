////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2017 StromFOrge
// 
// Author : Generated
// 
// Description : GameInstanceManager Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "ServerProtocolPCH.h"
#include "SFTypedefs.h"
#include "Net/SFNetDef.h"
#include "Net/SFMessage.h"
#include "Net/SFConnection.h"
#include "Protocol/SvrProtocol.h"
#include "Protocol/Policy/GameInstanceManagerNetPolicy.h"
#include "Protocol/Message/GameInstanceManagerMsgClass.h"
#include "ProtocolTrace.h"



namespace SF
{
 	namespace Policy
	{
 		// Cmd: Create a game instance
		Result NetPolicyGameInstanceManager::CreateGameCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const uint16_t &InNumberOfBotPlayer, const uint16_t &InMaxPlayer )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::GameInstanceManager::CreateGameCmd::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InRouteHopCount, InNumberOfBotPlayer, InMaxPlayer);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetPolicyGameInstanceManager::CreateGameCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const uint16_t &InNumberOfBotPlayer, const uint16_t &InMaxPlayer )
		// C2S: Game instance notification of deletion
		Result NetPolicyGameInstanceManager::GameDeletedC2SEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::GameInstanceManager::GameDeletedC2SEvt::Create(m_pConnection->GetIOHeap(), InRouteContext, InRouteHopCount);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetPolicyGameInstanceManager::GameDeletedC2SEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount )


		// Cmd: Create a game instance
		Result NetSvrPolicyGameInstanceManager::CreateGameRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::GameInstanceManager::CreateGameRes::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InResult);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGameInstanceManager::CreateGameRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )


	}; // namespace Policy
}; // namespace SF


