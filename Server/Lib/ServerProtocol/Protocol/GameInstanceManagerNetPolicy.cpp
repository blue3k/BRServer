////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) The Braves
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
#include "Net/SFMessageEndpoint.h"
#include "Actor/Movement/SFActorMovement.h"
#include "Protocol/GameInstanceManagerNetPolicy.h"
#include "Protocol/GameInstanceManagerMsgClass.h"



namespace SF
{
 	// Cmd: Create a game instance
	Result NetPolicyGameInstanceManager::CreateGameInstanceCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const uint32_t &InZoneTableId, const VariableTable &InAttributes )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::GameInstanceManager::CreateGameInstanceCmd::Create(GetSystemHeap(), InRouteContext, InTransactionID, InRouteHopCount, InZoneTableId, InAttributes);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

	}; // Result NetPolicyGameInstanceManager::CreateGameInstanceCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const uint32_t &InZoneTableId, const VariableTable &InAttributes )


	// Cmd: Create a game instance
	Result NetSvrPolicyGameInstanceManager::CreateGameInstanceRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::GameInstanceManager::CreateGameInstanceRes::Create(GetSystemHeap(), InRouteContext, InTransactionID, InResult);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

	}; // Result NetSvrPolicyGameInstanceManager::CreateGameInstanceRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )


}; // namespace SF


