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
#include "Protocol/Protocol.h"
#include "ServerSystem/ServerEntity.h"
#include "ServerSystem/BrServer.h"
#include "ServerSystem/BrServerUtil.h"
#include "ServerSystem/ServiceEntity/EntityInformation.h"
#include "ServerSystem/ServerService/GameServerService.h"
#include "ServerSystem/SvrTrace.h"



namespace SF
{
 	namespace Svr
	{
 		GameServerService::GameServerService( ServerServiceInformation* pService)
			: ServerServiceBase(pService, Policy::IPolicyGameServer::ID_POLICY)
		{
			static_assert((UINT)Policy::IPolicyGameServer::ID_POLICY == (UINT)ID_SERVICEPOLICY,"Invalid Policy ID for a Servicebase ");
			Assert(GetPolicyGameServer());
		}


		// Cmd: Kick
		Result GameServerService::RegisterPlayerToJoinGameServerCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID, const AuthTicket &InTicket, const FacebookUID &InFBUserID, const uint32_t &InShardID )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(GetPolicyGameServer()->RegisterPlayerToJoinGameServerCmd( InRouteContext, InTransactionID, InPlayerID, InTicket, InFBUserID, InShardID ) );

		Proc_End:

			return hr;

		}; // Result GameServerService::RegisterPlayerToJoinGameServerCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID, const AuthTicket &InTicket, const FacebookUID &InFBUserID, const uint32_t &InShardID )
		// Cmd: Kick
		Result GameServerService::RegisterPlayerToJoinGameServerOnPlayerEntityCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID, const AuthTicket &InTicket, const FacebookUID &InFBUserID )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(GetPolicyGameServer()->RegisterPlayerToJoinGameServerOnPlayerEntityCmd( InRouteContext, InTransactionID, InPlayerID, InTicket, InFBUserID ) );

		Proc_End:

			return hr;

		}; // Result GameServerService::RegisterPlayerToJoinGameServerOnPlayerEntityCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID, const AuthTicket &InTicket, const FacebookUID &InFBUserID )
		// C2S: Chatting message
		Result GameServerService::ChatMessageC2SEvt( const EntityID &InSenderEntityID, const AccountID &InSenderID, const PlayerRole &InRole, const char* InSenderName, const char* InChatMessage )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
			svrChk(GetPolicyGameServer()->ChatMessageC2SEvt( InRouteContext, InSenderID, InRole, InSenderName, InChatMessage ) );

		Proc_End:

			return hr;

		}; // Result GameServerService::ChatMessageC2SEvt( const EntityID &InSenderEntityID, const AccountID &InSenderID, const PlayerRole &InRole, const char* InSenderName, const char* InChatMessage )
		// C2S: Notification
		Result GameServerService::NotifyC2SEvt( const EntityID &InSenderEntityID, const PlayerID &InDestPlayerID, const uint32_t &InNotificationID, const NotificationType &InMessageID, const uint64_t &InMessageParam0, const uint64_t &InMessageParam1, const char* InMessageText, const uint64_t &InTimeStamp )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
			svrChk(GetPolicyGameServer()->NotifyC2SEvt( InRouteContext, InDestPlayerID, InNotificationID, InMessageID, InMessageParam0, InMessageParam1, InMessageText, InTimeStamp ) );

		Proc_End:

			return hr;

		}; // Result GameServerService::NotifyC2SEvt( const EntityID &InSenderEntityID, const PlayerID &InDestPlayerID, const uint32_t &InNotificationID, const NotificationType &InMessageID, const uint64_t &InMessageParam0, const uint64_t &InMessageParam1, const char* InMessageText, const uint64_t &InTimeStamp )
		// C2S: Friend Accept
		Result GameServerService::FriendAcceptedC2SEvt( const EntityID &InSenderEntityID, const AccountID &InDestPlayerID, const ServerFriendInformation &InAccepter )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
			svrChk(GetPolicyGameServer()->FriendAcceptedC2SEvt( InRouteContext, InDestPlayerID, InAccepter ) );

		Proc_End:

			return hr;

		}; // Result GameServerService::FriendAcceptedC2SEvt( const EntityID &InSenderEntityID, const AccountID &InDestPlayerID, const ServerFriendInformation &InAccepter )
		// C2S: Friend Remove
		Result GameServerService::FriendRemovedC2SEvt( const EntityID &InSenderEntityID, const AccountID &InDestPlayerID, const PlayerID &InRemoverID )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
			svrChk(GetPolicyGameServer()->FriendRemovedC2SEvt( InRouteContext, InDestPlayerID, InRemoverID ) );

		Proc_End:

			return hr;

		}; // Result GameServerService::FriendRemovedC2SEvt( const EntityID &InSenderEntityID, const AccountID &InDestPlayerID, const PlayerID &InRemoverID )
		// C2S: Request Player Status Update
		Result GameServerService::RequestPlayerStatusUpdateC2SEvt( const EntityID &InSenderEntityID, const AccountID &InDestPlayerID, const PlayerID &InRequesterID )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
			svrChk(GetPolicyGameServer()->RequestPlayerStatusUpdateC2SEvt( InRouteContext, InDestPlayerID, InRequesterID ) );

		Proc_End:

			return hr;

		}; // Result GameServerService::RequestPlayerStatusUpdateC2SEvt( const EntityID &InSenderEntityID, const AccountID &InDestPlayerID, const PlayerID &InRequesterID )
		// C2S: Notify Player Status Updated
		Result GameServerService::NotifyPlayerStatusUpdatedC2SEvt( const EntityID &InSenderEntityID, const AccountID &InDestPlayerID, const uint32_t &InLatestActiveTime, const uint8_t &InIsInGame )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
			svrChk(GetPolicyGameServer()->NotifyPlayerStatusUpdatedC2SEvt( InRouteContext, InDestPlayerID, InLatestActiveTime, InIsInGame ) );

		Proc_End:

			return hr;

		}; // Result GameServerService::NotifyPlayerStatusUpdatedC2SEvt( const EntityID &InSenderEntityID, const AccountID &InDestPlayerID, const uint32_t &InLatestActiveTime, const uint8_t &InIsInGame )
		// C2S: Notification
		Result GameServerService::NotifyPartyInviteC2SEvt( const EntityID &InSenderEntityID, const PlayerID &InDestPlayerID, const PlayerID &InInviterID, const char* InInviterName, const EntityUID &InPartyUID )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
			svrChk(GetPolicyGameServer()->NotifyPartyInviteC2SEvt( InRouteContext, InDestPlayerID, InInviterID, InInviterName, InPartyUID ) );

		Proc_End:

			return hr;

		}; // Result GameServerService::NotifyPartyInviteC2SEvt( const EntityID &InSenderEntityID, const PlayerID &InDestPlayerID, const PlayerID &InInviterID, const char* InInviterName, const EntityUID &InPartyUID )


	}; // namespace Svr
}; // namespace SF


