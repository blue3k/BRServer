////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) The Braves
// 
// Author : Generated
// 
// Description : ChatChannelManager Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "ServerProtocolPCH.h"
#include "SFTypedefs.h"
#include "Net/SFNetDef.h"
#include "Net/SFMessage.h"
#include "Net/SFMessageEndpoint.h"
#include "Actor/Movement/SFActorMovement.h"
#include "Protocol/SvrProtocol.h"
#include "Protocol/ChatChannelManagerNetPolicy.h"
#include "Protocol/ChatChannelManagerMsgClass.h"



namespace SF
{
 	// Cmd: Create a channel instance
	Result NetPolicyChatChannelManager::CreateChannelCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const char* InChannelName, const char* InPasscode, const PlayerInformation &InCreator )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::ChatChannelManager::CreateChannelCmd::Create(GetSystemHeap(), InRouteContext, InTransactionID, InRouteHopCount, InChannelName, InPasscode, InCreator);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetPolicyChatChannelManager::CreateChannelCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const char* InChannelName, const char* InPasscode, const PlayerInformation &InCreator )
	// Cmd: Find a channel instance with name
	Result NetPolicyChatChannelManager::FindChannelCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const char* InChannelName )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::ChatChannelManager::FindChannelCmd::Create(GetSystemHeap(), InRouteContext, InTransactionID, InRouteHopCount, InChannelName);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetPolicyChatChannelManager::FindChannelCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const char* InChannelName )
	// C2S: Notification that a chat channel instance has deleted
	Result NetPolicyChatChannelManager::ChatChannelDeletedC2SEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::ChatChannelManager::ChatChannelDeletedC2SEvt::Create(GetSystemHeap(), InRouteContext, InRouteHopCount);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetPolicyChatChannelManager::ChatChannelDeletedC2SEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount )


	// Cmd: Create a channel instance
	Result NetSvrPolicyChatChannelManager::CreateChannelRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const uint64_t &InChannelUID )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::ChatChannelManager::CreateChannelRes::Create(GetSystemHeap(), InRouteContext, InTransactionID, InResult, InChannelUID);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetSvrPolicyChatChannelManager::CreateChannelRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const uint64_t &InChannelUID )
	// Cmd: Find a channel instance with name
	Result NetSvrPolicyChatChannelManager::FindChannelRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const uint64_t &InChannelUID )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::ChatChannelManager::FindChannelRes::Create(GetSystemHeap(), InRouteContext, InTransactionID, InResult, InChannelUID);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetSvrPolicyChatChannelManager::FindChannelRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const uint64_t &InChannelUID )


}; // namespace SF


