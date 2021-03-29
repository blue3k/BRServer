////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) The Braves
// 
// Author : Generated
// 
// Description : ChatChannelManager Message debug definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "SFTypedefs.h"
#include "Protocol/SFProtocol.h"
#include "Net/SFNetDef.h"



namespace SF
{
    class MessageEndpoint;
   class VariableTable;
   struct ActorMovement;


	class NetPolicyChatChannelManager 
	{
 		private:
			SharedPointerT<MessageEndpoint> m_Endpoint;
		public:
		// Constructor
		NetPolicyChatChannelManager (const SharedPointerT<MessageEndpoint>& pEndpoint ) : m_Endpoint(pEndpoint)
		{}

		// Cmd: Create a channel instance
		Result CreateChannelCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const char* InChannelName, const char* InPasscode, const PlayerInformation &InCreator );
		// Cmd: Find a channel instance with name
		Result FindChannelCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const char* InChannelName );
		// C2S: Notification that a chat channel instance has deleted
		Result ChatChannelDeletedC2SEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount );

	}; // class NetPolicyChatChannelManager 


	class NetSvrPolicyChatChannelManager
	{
 		private:
			SharedPointerT<MessageEndpoint> m_Endpoint;
		public:
		// Constructor
		NetSvrPolicyChatChannelManager ( const SharedPointerT<MessageEndpoint>& pEndpoint ) : m_Endpoint(pEndpoint)
		{}

		// Cmd: Create a channel instance
		Result CreateChannelRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const uint64_t &InChannelUID );
		// Cmd: Find a channel instance with name
		Result FindChannelRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const uint64_t &InChannelUID );

	}; // class NetSvrPolicyChatChannelManager


}; // namespace SF


