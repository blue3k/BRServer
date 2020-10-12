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
 			FunctionContext hr;

			 MessageDataPtr pMessage;
			 protocolCheckPtr(m_pConnection);

			 pMessage = SF::Message::GameInstanceManager::CreateGameCmd::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InRouteHopCount, InNumberOfBotPlayer, InMaxPlayer);
			 protocolCheckPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

			return hr;

		}; // Result NetPolicyGameInstanceManager::CreateGameCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const uint16_t &InNumberOfBotPlayer, const uint16_t &InMaxPlayer )
		// Cmd: Search game instance
		Result NetPolicyGameInstanceManager::SearchGameInstanceCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const char* InSearchKeyword )
		{
 			FunctionContext hr;

			 MessageDataPtr pMessage;
			 protocolCheckPtr(m_pConnection);

			 pMessage = SF::Message::GameInstanceManager::SearchGameInstanceCmd::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InRouteHopCount, InSearchKeyword);
			 protocolCheckPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

			return hr;

		}; // Result NetPolicyGameInstanceManager::SearchGameInstanceCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const char* InSearchKeyword )
		// C2S: Game instance notification of deletion
		Result NetPolicyGameInstanceManager::GameDeletedC2SEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount )
		{
 			FunctionContext hr;

			 MessageDataPtr pMessage;
			 protocolCheckPtr(m_pConnection);

			 pMessage = SF::Message::GameInstanceManager::GameDeletedC2SEvt::Create(m_pConnection->GetIOHeap(), InRouteContext, InRouteHopCount);
			 protocolCheckPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

			return hr;

		}; // Result NetPolicyGameInstanceManager::GameDeletedC2SEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount )


		// Cmd: Create a game instance
		Result NetSvrPolicyGameInstanceManager::CreateGameRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
		{
 			FunctionContext hr;

			 MessageDataPtr pMessage;
			 protocolCheckPtr(m_pConnection);

			 pMessage = SF::Message::GameInstanceManager::CreateGameRes::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InResult);
			 protocolCheckPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

			return hr;

		}; // Result NetSvrPolicyGameInstanceManager::CreateGameRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
		// Cmd: Search game instance
		Result NetSvrPolicyGameInstanceManager::SearchGameInstanceRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const Array<uint64_t>& InGameInstances )
		{
 			FunctionContext hr;

			 MessageDataPtr pMessage;
			 protocolCheckPtr(m_pConnection);

			 pMessage = SF::Message::GameInstanceManager::SearchGameInstanceRes::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InResult, InGameInstances);
			 protocolCheckPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

			return hr;

		}; // Result NetSvrPolicyGameInstanceManager::SearchGameInstanceRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const Array<uint64_t>& InGameInstances )


	}; // namespace Policy
}; // namespace SF


