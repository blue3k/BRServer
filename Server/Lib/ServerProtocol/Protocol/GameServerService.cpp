////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) Kyungkun Ko
// 
// Author : Generated
// 
// Description : GameServer Server service
// 
////////////////////////////////////////////////////////////////////////////////


#include "ServerProtocolPCH.h"
#include "SFTypedefs.h"
#include "Protocol/SFProtocol.h"
#include "Net/SFMessageEndpoint.h"
#include "Server/BrServer.h"
#include "Server/BrServerUtil.h"
#include "Protocol/GameServerService.h"
#include "SvrTrace.h"



namespace SF
{
 	GameServerService::GameServerService( ServerServiceInformation* pService)
		: ServerServiceBase(pService)
	{
	}


	// Cmd: Register player so that the player can connection and join to the game server.
	Result GameServerService::RegisterPlayerToJoinGameServerCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID, const AuthTicket &InTicket, const FacebookUID &InFBUserID, const uint32_t &InShardID )
	{
 		ScopeContext hr;

		RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
		svrCheck(NetPolicyGameServer(GetTargetEndpoint()).RegisterPlayerToJoinGameServerCmd( InRouteContext, InTransactionID, InPlayerID, InTicket, InFBUserID, InShardID ) );

		return hr;

	}; // Result GameServerService::RegisterPlayerToJoinGameServerCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID, const AuthTicket &InTicket, const FacebookUID &InFBUserID, const uint32_t &InShardID )
	// Cmd: Same to RegisterPlayerToJoinGameServer, but can run on player entity. only works when the player entity exists. If player disconnected for some reason, you can run this method to connect.
	Result GameServerService::RegisterPlayerToJoinGameServerOnPlayerEntityCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID, const AuthTicket &InTicket, const FacebookUID &InFBUserID )
	{
 		ScopeContext hr;

		RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
		svrCheck(NetPolicyGameServer(GetTargetEndpoint()).RegisterPlayerToJoinGameServerOnPlayerEntityCmd( InRouteContext, InTransactionID, InPlayerID, InTicket, InFBUserID ) );

		return hr;

	}; // Result GameServerService::RegisterPlayerToJoinGameServerOnPlayerEntityCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID, const AuthTicket &InTicket, const FacebookUID &InFBUserID )
	// C2S: Chatting message event.
	Result GameServerService::ChatMessageC2SEvt( const EntityID &InSenderEntityID, const AccountID &InSenderID, const uint8_t &InRole, const char* InSenderName, const char* InChatMessage )
	{
 		ScopeContext hr;

		RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
		svrCheck(NetPolicyGameServer(GetTargetEndpoint()).ChatMessageC2SEvt( InRouteContext, InSenderID, InRole, InSenderName, InChatMessage ) );

		return hr;

	}; // Result GameServerService::ChatMessageC2SEvt( const EntityID &InSenderEntityID, const AccountID &InSenderID, const uint8_t &InRole, const char* InSenderName, const char* InChatMessage )
	// C2S: Notification event for P2P
	Result GameServerService::NotifyC2SEvt( const EntityID &InSenderEntityID, const PlayerID &InDestPlayerID, const uint32_t &InNotificationID, const NotificationType &InMessageID, const uint64_t &InMessageParam0, const uint64_t &InMessageParam1, const char* InMessageText, const uint64_t &InTimeStamp )
	{
 		ScopeContext hr;

		RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
		svrCheck(NetPolicyGameServer(GetTargetEndpoint()).NotifyC2SEvt( InRouteContext, InDestPlayerID, InNotificationID, InMessageID, InMessageParam0, InMessageParam1, InMessageText, InTimeStamp ) );

		return hr;

	}; // Result GameServerService::NotifyC2SEvt( const EntityID &InSenderEntityID, const PlayerID &InDestPlayerID, const uint32_t &InNotificationID, const NotificationType &InMessageID, const uint64_t &InMessageParam0, const uint64_t &InMessageParam1, const char* InMessageText, const uint64_t &InTimeStamp )
	// C2S: Friend Accepted
	Result GameServerService::FriendAcceptedC2SEvt( const EntityID &InSenderEntityID, const AccountID &InDestPlayerID, const ServerFriendInformation &InAccepter )
	{
 		ScopeContext hr;

		RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
		svrCheck(NetPolicyGameServer(GetTargetEndpoint()).FriendAcceptedC2SEvt( InRouteContext, InDestPlayerID, InAccepter ) );

		return hr;

	}; // Result GameServerService::FriendAcceptedC2SEvt( const EntityID &InSenderEntityID, const AccountID &InDestPlayerID, const ServerFriendInformation &InAccepter )
	// C2S: Friend Remove
	Result GameServerService::FriendRemovedC2SEvt( const EntityID &InSenderEntityID, const AccountID &InDestPlayerID, const PlayerID &InRemoverID )
	{
 		ScopeContext hr;

		RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
		svrCheck(NetPolicyGameServer(GetTargetEndpoint()).FriendRemovedC2SEvt( InRouteContext, InDestPlayerID, InRemoverID ) );

		return hr;

	}; // Result GameServerService::FriendRemovedC2SEvt( const EntityID &InSenderEntityID, const AccountID &InDestPlayerID, const PlayerID &InRemoverID )
	// C2S: Request Player Status Update
	Result GameServerService::RequestPlayerStatusUpdateC2SEvt( const EntityID &InSenderEntityID, const AccountID &InDestPlayerID, const PlayerID &InRequesterID )
	{
 		ScopeContext hr;

		RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
		svrCheck(NetPolicyGameServer(GetTargetEndpoint()).RequestPlayerStatusUpdateC2SEvt( InRouteContext, InDestPlayerID, InRequesterID ) );

		return hr;

	}; // Result GameServerService::RequestPlayerStatusUpdateC2SEvt( const EntityID &InSenderEntityID, const AccountID &InDestPlayerID, const PlayerID &InRequesterID )
	// C2S: Notify Player Status Updated
	Result GameServerService::NotifyPlayerStatusUpdatedC2SEvt( const EntityID &InSenderEntityID, const AccountID &InDestPlayerID, const uint32_t &InLatestActiveTime, const uint8_t &InIsInGame )
	{
 		ScopeContext hr;

		RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
		svrCheck(NetPolicyGameServer(GetTargetEndpoint()).NotifyPlayerStatusUpdatedC2SEvt( InRouteContext, InDestPlayerID, InLatestActiveTime, InIsInGame ) );

		return hr;

	}; // Result GameServerService::NotifyPlayerStatusUpdatedC2SEvt( const EntityID &InSenderEntityID, const AccountID &InDestPlayerID, const uint32_t &InLatestActiveTime, const uint8_t &InIsInGame )
	// C2S: Party invite Notification
	Result GameServerService::NotifyPartyInviteC2SEvt( const EntityID &InSenderEntityID, const PlayerID &InDestPlayerID, const PlayerID &InInviterID, const char* InInviterName, const uint64_t &InPartyUID )
	{
 		ScopeContext hr;

		RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
		svrCheck(NetPolicyGameServer(GetTargetEndpoint()).NotifyPartyInviteC2SEvt( InRouteContext, InDestPlayerID, InInviterID, InInviterName, InPartyUID ) );

		return hr;

	}; // Result GameServerService::NotifyPartyInviteC2SEvt( const EntityID &InSenderEntityID, const PlayerID &InDestPlayerID, const PlayerID &InInviterID, const char* InInviterName, const uint64_t &InPartyUID )


}; // namespace SF


