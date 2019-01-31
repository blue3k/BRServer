////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 StormForge
// 
// Author : Generated
// 
// Description : GameServer Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "SFTypedefs.h"
#include "Protocol/SFProtocol.h"
#include "ServerEntity/ServerEntity.h"
#include "Server/BrServer.h"
#include "Server/BrServerUtil.h"
#include "Entity/EntityInformation.h"
#include "Protocol/ServerService/GameServerService.h"
#include "SvrTrace.h"



namespace SF
{
 	namespace Svr
	{
 		GameServerService::GameServerService( ServerServiceInformation* pService)
			: ServerServiceBase(pService)
		{
		}


		// Cmd: Register player so that the player can connection and join to the game server.
		Result GameServerService::RegisterPlayerToJoinGameServerCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID, const AuthTicket &InTicket, const FacebookUID &InFBUserID, const uint32_t &InShardID )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyGameServer(GetConnection()).RegisterPlayerToJoinGameServerCmd( InRouteContext, InTransactionID, InPlayerID, InTicket, InFBUserID, InShardID ) );

		Proc_End:

			return hr;

		}; // Result GameServerService::RegisterPlayerToJoinGameServerCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID, const AuthTicket &InTicket, const FacebookUID &InFBUserID, const uint32_t &InShardID )
		// Cmd: Same to RegisterPlayerToJoinGameServer, but can run on player entity. only works when the player entity exists. If player disconnected for some reason, you can run this method to connect.
		Result GameServerService::RegisterPlayerToJoinGameServerOnPlayerEntityCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID, const AuthTicket &InTicket, const FacebookUID &InFBUserID )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyGameServer(GetConnection()).RegisterPlayerToJoinGameServerOnPlayerEntityCmd( InRouteContext, InTransactionID, InPlayerID, InTicket, InFBUserID ) );

		Proc_End:

			return hr;

		}; // Result GameServerService::RegisterPlayerToJoinGameServerOnPlayerEntityCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID, const AuthTicket &InTicket, const FacebookUID &InFBUserID )
		// C2S: Chatting message event.
		Result GameServerService::ChatMessageC2SEvt( const EntityID &InSenderEntityID, const AccountID &InSenderID, const uint8_t &InRole, const char* InSenderName, const char* InChatMessage )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyGameServer(GetConnection()).ChatMessageC2SEvt( InRouteContext, InSenderID, InRole, InSenderName, InChatMessage ) );

		Proc_End:

			return hr;

		}; // Result GameServerService::ChatMessageC2SEvt( const EntityID &InSenderEntityID, const AccountID &InSenderID, const uint8_t &InRole, const char* InSenderName, const char* InChatMessage )
		// C2S: Notification event for P2P
		Result GameServerService::NotifyC2SEvt( const EntityID &InSenderEntityID, const PlayerID &InDestPlayerID, const uint32_t &InNotificationID, const NotificationType &InMessageID, const uint64_t &InMessageParam0, const uint64_t &InMessageParam1, const char* InMessageText, const uint64_t &InTimeStamp )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyGameServer(GetConnection()).NotifyC2SEvt( InRouteContext, InDestPlayerID, InNotificationID, InMessageID, InMessageParam0, InMessageParam1, InMessageText, InTimeStamp ) );

		Proc_End:

			return hr;

		}; // Result GameServerService::NotifyC2SEvt( const EntityID &InSenderEntityID, const PlayerID &InDestPlayerID, const uint32_t &InNotificationID, const NotificationType &InMessageID, const uint64_t &InMessageParam0, const uint64_t &InMessageParam1, const char* InMessageText, const uint64_t &InTimeStamp )
		// C2S: Friend Accepted
		Result GameServerService::FriendAcceptedC2SEvt( const EntityID &InSenderEntityID, const AccountID &InDestPlayerID, const ServerFriendInformation &InAccepter )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyGameServer(GetConnection()).FriendAcceptedC2SEvt( InRouteContext, InDestPlayerID, InAccepter ) );

		Proc_End:

			return hr;

		}; // Result GameServerService::FriendAcceptedC2SEvt( const EntityID &InSenderEntityID, const AccountID &InDestPlayerID, const ServerFriendInformation &InAccepter )
		// C2S: Friend Remove
		Result GameServerService::FriendRemovedC2SEvt( const EntityID &InSenderEntityID, const AccountID &InDestPlayerID, const PlayerID &InRemoverID )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyGameServer(GetConnection()).FriendRemovedC2SEvt( InRouteContext, InDestPlayerID, InRemoverID ) );

		Proc_End:

			return hr;

		}; // Result GameServerService::FriendRemovedC2SEvt( const EntityID &InSenderEntityID, const AccountID &InDestPlayerID, const PlayerID &InRemoverID )
		// C2S: Request Player Status Update
		Result GameServerService::RequestPlayerStatusUpdateC2SEvt( const EntityID &InSenderEntityID, const AccountID &InDestPlayerID, const PlayerID &InRequesterID )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyGameServer(GetConnection()).RequestPlayerStatusUpdateC2SEvt( InRouteContext, InDestPlayerID, InRequesterID ) );

		Proc_End:

			return hr;

		}; // Result GameServerService::RequestPlayerStatusUpdateC2SEvt( const EntityID &InSenderEntityID, const AccountID &InDestPlayerID, const PlayerID &InRequesterID )
		// C2S: Notify Player Status Updated
		Result GameServerService::NotifyPlayerStatusUpdatedC2SEvt( const EntityID &InSenderEntityID, const AccountID &InDestPlayerID, const uint32_t &InLatestActiveTime, const uint8_t &InIsInGame )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyGameServer(GetConnection()).NotifyPlayerStatusUpdatedC2SEvt( InRouteContext, InDestPlayerID, InLatestActiveTime, InIsInGame ) );

		Proc_End:

			return hr;

		}; // Result GameServerService::NotifyPlayerStatusUpdatedC2SEvt( const EntityID &InSenderEntityID, const AccountID &InDestPlayerID, const uint32_t &InLatestActiveTime, const uint8_t &InIsInGame )
		// C2S: Party invite Notification
		Result GameServerService::NotifyPartyInviteC2SEvt( const EntityID &InSenderEntityID, const PlayerID &InDestPlayerID, const PlayerID &InInviterID, const char* InInviterName, const uint64_t &InPartyUID )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyGameServer(GetConnection()).NotifyPartyInviteC2SEvt( InRouteContext, InDestPlayerID, InInviterID, InInviterName, InPartyUID ) );

		Proc_End:

			return hr;

		}; // Result GameServerService::NotifyPartyInviteC2SEvt( const EntityID &InSenderEntityID, const PlayerID &InDestPlayerID, const PlayerID &InInviterID, const char* InInviterName, const uint64_t &InPartyUID )


	}; // namespace Svr
}; // namespace SF


