////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) Kyungkun Ko
// 
// Author : Generated
// 
// Description : ChatChannelManager Server service
// 
////////////////////////////////////////////////////////////////////////////////


#include "ServerProtocolPCH.h"
#include "SFTypedefs.h"
#include "Protocol/SFProtocol.h"
#include "Net/SFMessageEndpoint.h"
#include "Server/BrServer.h"
#include "Server/BrServerUtil.h"
#include "Protocol/ChatChannelManagerService.h"
#include "SvrTrace.h"



namespace SF
{
 	ChatChannelManagerService::ChatChannelManagerService( ServerServiceInformation* pService)
		: ServerServiceBase(pService)
	{
	}


	// Cmd: Create a channel instance
	Result ChatChannelManagerService::CreateChannelCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const char* InChannelName, const char* InPasscode, const PlayerInformation &InCreator )
	{
 		ScopeContext hr;

		RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
		svrCheck(NetPolicyChatChannelManager(GetTargetEndpoint()).CreateChannelCmd( InRouteContext, InTransactionID, InRouteHopCount, InChannelName, InPasscode, InCreator ) );

		return hr;

	}; // Result ChatChannelManagerService::CreateChannelCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const char* InChannelName, const char* InPasscode, const PlayerInformation &InCreator )
	// Cmd: Find a channel instance with name
	Result ChatChannelManagerService::FindChannelCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const char* InChannelName )
	{
 		ScopeContext hr;

		RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
		svrCheck(NetPolicyChatChannelManager(GetTargetEndpoint()).FindChannelCmd( InRouteContext, InTransactionID, InRouteHopCount, InChannelName ) );

		return hr;

	}; // Result ChatChannelManagerService::FindChannelCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const char* InChannelName )
	// C2S: Notification that a chat channel instance has deleted
	Result ChatChannelManagerService::ChatChannelDeletedC2SEvt( const EntityID &InSenderEntityID, const uint16_t &InRouteHopCount )
	{
 		ScopeContext hr;

		RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
		svrCheck(NetPolicyChatChannelManager(GetTargetEndpoint()).ChatChannelDeletedC2SEvt( InRouteContext, InRouteHopCount ) );

		return hr;

	}; // Result ChatChannelManagerService::ChatChannelDeletedC2SEvt( const EntityID &InSenderEntityID, const uint16_t &InRouteHopCount )


}; // namespace SF


