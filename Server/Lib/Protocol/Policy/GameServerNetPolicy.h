////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : GameServer Message debug definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Common/Typedefs.h"
#include "Common/PolicyID.h"
#include "Net/NetDef.h"
#include "Net/NetPolicy.h"
#include "Protocol/Policy/GameServerIPolicy.h"



namespace BR
{
 	namespace Policy
	{
 		class NetPolicyGameServer : public IPolicyGameServer
		{
 			public:
			// Constructor
			NetPolicyGameServer ( Net::IConnection* pConn ) : IPolicyGameServer(pConn)
			{}

			// Cmd: Kick
			HRESULT RegisterPlayerToJoinGameServerCmd( const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID, const AuthTicket &InTicket, const FacebookUID &InFBUserID, const UINT32 &InShardID );
			// Cmd: Kick
			HRESULT RegisterPlayerToJoinGameServerOnPlayerEntityCmd( const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID, const AuthTicket &InTicket, const FacebookUID &InFBUserID );
			// C2S: Chatting message
			HRESULT ChatMessageC2SEvt( const RouteContext &InRouteContext, const AccountID &InSenderID, const PlayerRole &InRole, const char* InSenderName, const char* InChatMessage );
			// C2S: Notification
			HRESULT NotifyC2SEvt( const RouteContext &InRouteContext, const PlayerID &InDestPlayerID, const UINT32 &InNotificationID, const NotificationType &InMessageID, const UINT64 &InMessageParam0, const UINT64 &InMessageParam1, const char* InMessageText, const UINT64 &InTimeStamp );
			// C2S: Friend Accept
			HRESULT FriendAcceptedC2SEvt( const RouteContext &InRouteContext, const AccountID &InDestPlayerID, const ServerFriendInformation &InAccepter );
			// C2S: Friend Remove
			HRESULT FriendRemovedC2SEvt( const RouteContext &InRouteContext, const AccountID &InDestPlayerID, const PlayerID &InRemoverID );
			// C2S: Request Player Status Update
			HRESULT RequestPlayerStatusUpdateC2SEvt( const RouteContext &InRouteContext, const AccountID &InDestPlayerID, const PlayerID &InRequesterID );
			// C2S: Notify Player Status Updated
			HRESULT NotifyPlayerStatusUpdatedC2SEvt( const RouteContext &InRouteContext, const AccountID &InDestPlayerID, const UINT32 &InLatestActiveTime, const BYTE &InIsInGame );
			// C2S: Notification
			HRESULT NotifyPartyInviteC2SEvt( const RouteContext &InRouteContext, const PlayerID &InDestPlayerID, const PlayerID &InInviterID, const char* InInviterName, const EntityUID &InPartyUID );

		}; // class NetPolicyGameServer : public IPolicyGameServer


		class NetSvrPolicyGameServer : public ISvrPolicyGameServer
		{
 			public:
			// Constructor
			NetSvrPolicyGameServer ( Net::IConnection* pConn ) : ISvrPolicyGameServer(pConn)
			{}

			// Cmd: Kick
			HRESULT RegisterPlayerToJoinGameServerRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext, const NetAddress &InPublicAddress, const NetAddress &InPublicAddressIPV4 );
			// Cmd: Kick
			HRESULT RegisterPlayerToJoinGameServerOnPlayerEntityRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext );

		}; // class NetSvrPolicyGameServer : public ISvrPolicyGameServer


	}; // namespace Policy
}; // namespace BR


