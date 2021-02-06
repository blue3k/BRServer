////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) Kyungkun Ko
// 
// Author : Generated
// 
// Description : ChatChannel Server service
// 
////////////////////////////////////////////////////////////////////////////////


#include "ServerProtocolPCH.h"
#include "SFTypedefs.h"
#include "Protocol/SFProtocol.h"
#include "Net/SFMessageEndpoint.h"
#include "Server/BrServer.h"
#include "Server/BrServerUtil.h"
#include "Protocol/ServerService/ChatChannelService.h"
#include "SvrTrace.h"



namespace SF
{
 	ChatChannelService::ChatChannelService( ServerServiceInformation* pService)
		: ServerServiceBase(pService)
	{
	}


	// Cmd: Request to join chat channel
	Result ChatChannelService::JoinCmd( const uint64_t &InTransactionID, const char* InPasscode, const PlayerInformation &InJoiningPlayer )
	{
 		ScopeContext hr;

		RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
		svrCheck(NetPolicyChatChannel(GetTargetEndpoint()).JoinCmd( InRouteContext, InTransactionID, InPasscode, InJoiningPlayer ) );

		return hr;

	}; // Result ChatChannelService::JoinCmd( const uint64_t &InTransactionID, const char* InPasscode, const PlayerInformation &InJoiningPlayer )
	// Cmd: Leave chat channel
	Result ChatChannelService::LeaveCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID )
	{
 		ScopeContext hr;

		RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
		svrCheck(NetPolicyChatChannel(GetTargetEndpoint()).LeaveCmd( InRouteContext, InTransactionID, InPlayerID ) );

		return hr;

	}; // Result ChatChannelService::LeaveCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID )
	// Cmd: Kick a player
	Result ChatChannelService::KickPlayerCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick )
	{
 		ScopeContext hr;

		RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
		svrCheck(NetPolicyChatChannel(GetTargetEndpoint()).KickPlayerCmd( InRouteContext, InTransactionID, InPlayerID, InPlayerToKick ) );

		return hr;

	}; // Result ChatChannelService::KickPlayerCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick )
	// C2S: Sending a chatting message
	Result ChatChannelService::ChatMessageC2SEvt( const EntityID &InSenderEntityID, const PlayerID &InSenderID, const char* InChatMessage )
	{
 		ScopeContext hr;

		RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
		svrCheck(NetPolicyChatChannel(GetTargetEndpoint()).ChatMessageC2SEvt( InRouteContext, InSenderID, InChatMessage ) );

		return hr;

	}; // Result ChatChannelService::ChatMessageC2SEvt( const EntityID &InSenderEntityID, const PlayerID &InSenderID, const char* InChatMessage )


}; // namespace SF


