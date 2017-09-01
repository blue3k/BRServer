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

#include "SFTypedefs.h"
#include "Protocol/Protocol.h"
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
			Result RegisterPlayerToJoinGameServerCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID, const AuthTicket &InTicket, const FacebookUID &InFBUserID, const uint32_t &InShardID );
			// Cmd: Kick
			Result RegisterPlayerToJoinGameServerOnPlayerEntityCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID, const AuthTicket &InTicket, const FacebookUID &InFBUserID );
			// C2S: Chatting message
			Result ChatMessageC2SEvt( const RouteContext &InRouteContext, const AccountID &InSenderID, const PlayerRole &InRole, const char* InSenderName, const char* InChatMessage );
			// C2S: Notification
			Result NotifyC2SEvt( const RouteContext &InRouteContext, const PlayerID &InDestPlayerID, const uint32_t &InNotificationID, const NotificationType &InMessageID, const uint64_t &InMessageParam0, const uint64_t &InMessageParam1, const char* InMessageText, const uint64_t &InTimeStamp );
			// C2S: Friend Accept
			Result FriendAcceptedC2SEvt( const RouteContext &InRouteContext, const AccountID &InDestPlayerID, const ServerFriendInformation &InAccepter );
			// C2S: Friend Remove
			Result FriendRemovedC2SEvt( const RouteContext &InRouteContext, const AccountID &InDestPlayerID, const PlayerID &InRemoverID );
			// C2S: Request Player Status Update
			Result RequestPlayerStatusUpdateC2SEvt( const RouteContext &InRouteContext, const AccountID &InDestPlayerID, const PlayerID &InRequesterID );
			// C2S: Notify Player Status Updated
			Result NotifyPlayerStatusUpdatedC2SEvt( const RouteContext &InRouteContext, const AccountID &InDestPlayerID, const uint32_t &InLatestActiveTime, const uint8_t &InIsInGame );
			// C2S: Notification
			Result NotifyPartyInviteC2SEvt( const RouteContext &InRouteContext, const PlayerID &InDestPlayerID, const PlayerID &InInviterID, const char* InInviterName, const EntityUID &InPartyUID );

		}; // class NetPolicyGameServer : public IPolicyGameServer


		class NetSvrPolicyGameServer : public ISvrPolicyGameServer
		{
 			public:
			// Constructor
			NetSvrPolicyGameServer ( Net::IConnection* pConn ) : ISvrPolicyGameServer(pConn)
			{}

			// Cmd: Kick
			Result RegisterPlayerToJoinGameServerRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const char* InPublicAddress, const char* InPublicAddressV6, const uint32_t &InPort );
			// Cmd: Kick
			Result RegisterPlayerToJoinGameServerOnPlayerEntityRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult );

		}; // class NetSvrPolicyGameServer : public ISvrPolicyGameServer


	}; // namespace Policy
}; // namespace BR


