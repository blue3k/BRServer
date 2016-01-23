////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : GameServer Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/Typedefs.h"
#include "Common/PolicyID.h"
#include "Common/BrBaseTypes.h"
#include "ServerSystem/ServerEntity.h"
#include "ServerSystem/BrServer.h"
#include "ServerSystem/BrServerUtil.h"
#include "ServerSystem/ServiceEntity/EntityInformation.h"
#include "ServerSystem/ServerService/GameServerService.h"
#include "ServerSystem/SvrTrace.h"



namespace BR
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
		HRESULT GameServerService::RegisterPlayerToJoinGameServerCmd( const Context &InContext, const PlayerID &InPlayerID, const AuthTicket &InTicket, const FacebookUID &InFBUserID, const UINT32 &InShardID )
		{
 			HRESULT hr = S_SYSTEM_OK;

			TransactionID localTransID(InContext);
			RouteContext InRouteContext( EntityUID(GetMyServerID(),localTransID.GetEntityID()), GetServiceEntityUID() );
			svrChk(GetPolicyGameServer()->RegisterPlayerToJoinGameServerCmd( InContext, InRouteContext, InPlayerID, InTicket, InFBUserID, InShardID ) );

		Proc_End:

			return hr;

		}; // HRESULT GameServerService::RegisterPlayerToJoinGameServerCmd( const Context &InContext, const PlayerID &InPlayerID, const AuthTicket &InTicket, const FacebookUID &InFBUserID, const UINT32 &InShardID )
		// Cmd: Kick
		HRESULT GameServerService::RegisterPlayerToJoinGameServerOnPlayerEntityCmd( const Context &InContext, const PlayerID &InPlayerID, const AuthTicket &InTicket, const FacebookUID &InFBUserID )
		{
 			HRESULT hr = S_SYSTEM_OK;

			TransactionID localTransID(InContext);
			RouteContext InRouteContext( EntityUID(GetMyServerID(),localTransID.GetEntityID()), GetServiceEntityUID() );
			svrChk(GetPolicyGameServer()->RegisterPlayerToJoinGameServerOnPlayerEntityCmd( InContext, InRouteContext, InPlayerID, InTicket, InFBUserID ) );

		Proc_End:

			return hr;

		}; // HRESULT GameServerService::RegisterPlayerToJoinGameServerOnPlayerEntityCmd( const Context &InContext, const PlayerID &InPlayerID, const AuthTicket &InTicket, const FacebookUID &InFBUserID )
		// C2S: Chatting message
		HRESULT GameServerService::ChatMessageC2SEvt( const EntityID &InSenderEntityID, const AccountID &InSenderID, const PlayerRole &InRole, const char* InSenderName, const char* InChatMessage )
		{
 			HRESULT hr = S_SYSTEM_OK;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
			svrChk(GetPolicyGameServer()->ChatMessageC2SEvt( InRouteContext, InSenderID, InRole, InSenderName, InChatMessage ) );

		Proc_End:

			return hr;

		}; // HRESULT GameServerService::ChatMessageC2SEvt( const EntityID &InSenderEntityID, const AccountID &InSenderID, const PlayerRole &InRole, const char* InSenderName, const char* InChatMessage )
		// C2S: Notification
		HRESULT GameServerService::NotifyC2SEvt( const EntityID &InSenderEntityID, const PlayerID &InDestPlayerID, const UINT32 &InNotificationID, const NotificationType &InMessageID, const UINT64 &InMessageParam0, const UINT64 &InMessageParam1, const char* InMessageText, const UINT64 &InTimeStamp )
		{
 			HRESULT hr = S_SYSTEM_OK;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
			svrChk(GetPolicyGameServer()->NotifyC2SEvt( InRouteContext, InDestPlayerID, InNotificationID, InMessageID, InMessageParam0, InMessageParam1, InMessageText, InTimeStamp ) );

		Proc_End:

			return hr;

		}; // HRESULT GameServerService::NotifyC2SEvt( const EntityID &InSenderEntityID, const PlayerID &InDestPlayerID, const UINT32 &InNotificationID, const NotificationType &InMessageID, const UINT64 &InMessageParam0, const UINT64 &InMessageParam1, const char* InMessageText, const UINT64 &InTimeStamp )
		// C2S: Friend Accept
		HRESULT GameServerService::FriendAcceptedC2SEvt( const EntityID &InSenderEntityID, const AccountID &InDestPlayerID, const ServerFriendInformation &InAccepter )
		{
 			HRESULT hr = S_SYSTEM_OK;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
			svrChk(GetPolicyGameServer()->FriendAcceptedC2SEvt( InRouteContext, InDestPlayerID, InAccepter ) );

		Proc_End:

			return hr;

		}; // HRESULT GameServerService::FriendAcceptedC2SEvt( const EntityID &InSenderEntityID, const AccountID &InDestPlayerID, const ServerFriendInformation &InAccepter )
		// C2S: Friend Remove
		HRESULT GameServerService::FriendRemovedC2SEvt( const EntityID &InSenderEntityID, const AccountID &InDestPlayerID, const PlayerID &InRemoverID )
		{
 			HRESULT hr = S_SYSTEM_OK;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
			svrChk(GetPolicyGameServer()->FriendRemovedC2SEvt( InRouteContext, InDestPlayerID, InRemoverID ) );

		Proc_End:

			return hr;

		}; // HRESULT GameServerService::FriendRemovedC2SEvt( const EntityID &InSenderEntityID, const AccountID &InDestPlayerID, const PlayerID &InRemoverID )
		// C2S: Request Player Status Update
		HRESULT GameServerService::RequestPlayerStatusUpdateC2SEvt( const EntityID &InSenderEntityID, const AccountID &InDestPlayerID, const PlayerID &InRequesterID )
		{
 			HRESULT hr = S_SYSTEM_OK;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
			svrChk(GetPolicyGameServer()->RequestPlayerStatusUpdateC2SEvt( InRouteContext, InDestPlayerID, InRequesterID ) );

		Proc_End:

			return hr;

		}; // HRESULT GameServerService::RequestPlayerStatusUpdateC2SEvt( const EntityID &InSenderEntityID, const AccountID &InDestPlayerID, const PlayerID &InRequesterID )
		// C2S: Notify Player Status Updated
		HRESULT GameServerService::NotifyPlayerStatusUpdatedC2SEvt( const EntityID &InSenderEntityID, const AccountID &InDestPlayerID, const UINT32 &InLatestActiveTime, const BYTE &InIsInGame )
		{
 			HRESULT hr = S_SYSTEM_OK;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
			svrChk(GetPolicyGameServer()->NotifyPlayerStatusUpdatedC2SEvt( InRouteContext, InDestPlayerID, InLatestActiveTime, InIsInGame ) );

		Proc_End:

			return hr;

		}; // HRESULT GameServerService::NotifyPlayerStatusUpdatedC2SEvt( const EntityID &InSenderEntityID, const AccountID &InDestPlayerID, const UINT32 &InLatestActiveTime, const BYTE &InIsInGame )
		// C2S: Notification
		HRESULT GameServerService::NotifyPartyInviteC2SEvt( const EntityID &InSenderEntityID, const PlayerID &InDestPlayerID, const PlayerID &InInviterID, const char* InInviterName, const EntityUID &InPartyUID )
		{
 			HRESULT hr = S_SYSTEM_OK;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
			svrChk(GetPolicyGameServer()->NotifyPartyInviteC2SEvt( InRouteContext, InDestPlayerID, InInviterID, InInviterName, InPartyUID ) );

		Proc_End:

			return hr;

		}; // HRESULT GameServerService::NotifyPartyInviteC2SEvt( const EntityID &InSenderEntityID, const PlayerID &InDestPlayerID, const PlayerID &InInviterID, const char* InInviterName, const EntityUID &InPartyUID )


	}; // namespace Svr
}; // namespace BR


