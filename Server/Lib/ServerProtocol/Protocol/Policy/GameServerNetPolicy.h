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
#include "Protocol/SFProtocol.h"
#include "Net/SFNetDef.h"



namespace SF
{
 	namespace Policy
	{
 		class NetPolicyGameServer 
		{
 			private:
				SharedPointerT<Net::Connection> m_pConnection;
			public:
			// Constructor
			NetPolicyGameServer ( const SharedPointerT<Net::Connection>& pConn ) : m_pConnection(pConn)
			{}
			NetPolicyGameServer ( const SharedPointerAtomicT<Net::Connection>& pConn ) : m_pConnection(pConn)
			{}

			// Cmd: Register player so that the player can connection and join to the game server.
			Result RegisterPlayerToJoinGameServerCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const AuthTicket &InTicket, const FacebookUID &InFBUserID, const uint32_t &InShardID );
			// Cmd: Same to RegisterPlayerToJoinGameServer, but can run on player entity. only works when the player entity exists. If player disconnected for some reason, you can run this method to connect.
			Result RegisterPlayerToJoinGameServerOnPlayerEntityCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const AuthTicket &InTicket, const FacebookUID &InFBUserID );
			// C2S: Chatting message event.
			Result ChatMessageC2SEvt( const RouteContext &InRouteContext, const AccountID &InSenderID, const uint8_t &InRole, const char* InSenderName, const char* InChatMessage );
			// C2S: Notification event for P2P
			Result NotifyC2SEvt( const RouteContext &InRouteContext, const PlayerID &InDestPlayerID, const uint32_t &InNotificationID, const NotificationType &InMessageID, const uint64_t &InMessageParam0, const uint64_t &InMessageParam1, const char* InMessageText, const uint64_t &InTimeStamp );
			// C2S: Friend Accepted
			Result FriendAcceptedC2SEvt( const RouteContext &InRouteContext, const AccountID &InDestPlayerID, const ServerFriendInformation &InAccepter );
			// C2S: Friend Remove
			Result FriendRemovedC2SEvt( const RouteContext &InRouteContext, const AccountID &InDestPlayerID, const PlayerID &InRemoverID );
			// C2S: Request Player Status Update
			Result RequestPlayerStatusUpdateC2SEvt( const RouteContext &InRouteContext, const AccountID &InDestPlayerID, const PlayerID &InRequesterID );
			// C2S: Notify Player Status Updated
			Result NotifyPlayerStatusUpdatedC2SEvt( const RouteContext &InRouteContext, const AccountID &InDestPlayerID, const uint32_t &InLatestActiveTime, const uint8_t &InIsInGame );
			// C2S: Party invite Notification
			Result NotifyPartyInviteC2SEvt( const RouteContext &InRouteContext, const PlayerID &InDestPlayerID, const PlayerID &InInviterID, const char* InInviterName, const uint64_t &InPartyUID );

		}; // class NetPolicyGameServer 


		class NetSvrPolicyGameServer
		{
 			private:
				SharedPointerT<Net::Connection> m_pConnection;
			public:
			// Constructor
			NetSvrPolicyGameServer ( const SharedPointerT<Net::Connection>& pConn ) : m_pConnection(pConn)
			{}
			NetSvrPolicyGameServer ( const SharedPointerAtomicT<Net::Connection>& pConn ) : m_pConnection(pConn)
			{}

			// Cmd: Register player so that the player can connection and join to the game server.
			Result RegisterPlayerToJoinGameServerRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const char* InPublicAddress, const char* InPublicAddressV6, const uint32_t &InPort );
			// Cmd: Same to RegisterPlayerToJoinGameServer, but can run on player entity. only works when the player entity exists. If player disconnected for some reason, you can run this method to connect.
			Result RegisterPlayerToJoinGameServerOnPlayerEntityRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult );

		}; // class NetSvrPolicyGameServer


	}; // namespace Policy
}; // namespace SF


