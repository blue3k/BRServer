////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2015 The Braves
// 
// Author : Generated
// 
// Description : GameServer Message debug definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Common/Typedefs.h"
#include "Common/BrSvrTypes.h"
#include "Common/BrGameTypes.h"
#include "Common/PolicyID.h"
#include "Net/NetDef.h"
#include "Net/NetPolicy.h"
#include "Common/SvrPolicyID.h"



namespace BR
{
 	namespace Policy
	{
 
		class ISvrPolicyGameServer : public Net::INetPolicy
		{
 			public:
			enum { ID_POLICY = POLICY_SVR_GAMESERVER };

			// Constructor
			ISvrPolicyGameServer( Net::IConnection* pConn ) : Net::INetPolicy(ID_POLICY, pConn)
			{}

			// Cmd: Kick
			virtual Result RegisterPlayerToJoinGameServerRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const char* InPublicAddress, const char* InPublicAddressV6, const UINT32 &InPort ) = 0;
			// Cmd: Kick
			virtual Result RegisterPlayerToJoinGameServerOnPlayerEntityRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult ) = 0;
			// C2S: Chatting message
			// C2S: Notification
			// C2S: Friend Accept
			// C2S: Friend Remove
			// C2S: Request Player Status Update
			// C2S: Notify Player Status Updated
			// C2S: Notification

		}; // class ISvrPolicyGameServer : public Net::INetPolicy


		class IPolicyGameServer : public Net::INetPolicy
		{
 			public:
			enum { ID_POLICY = POLICY_GAMESERVER };

			// Constructor
			IPolicyGameServer( Net::IConnection* pConn ) : Net::INetPolicy(ID_POLICY, pConn)
			{}

			// Cmd: Kick
			virtual Result RegisterPlayerToJoinGameServerCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID, const AuthTicket &InTicket, const FacebookUID &InFBUserID, const UINT32 &InShardID ) = 0;
			// Cmd: Kick
			virtual Result RegisterPlayerToJoinGameServerOnPlayerEntityCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID, const AuthTicket &InTicket, const FacebookUID &InFBUserID ) = 0;
			// C2S: Chatting message
			virtual Result ChatMessageC2SEvt( const RouteContext &InRouteContext, const AccountID &InSenderID, const PlayerRole &InRole, const char* InSenderName, const char* InChatMessage ) = 0;
			// C2S: Notification
			virtual Result NotifyC2SEvt( const RouteContext &InRouteContext, const PlayerID &InDestPlayerID, const UINT32 &InNotificationID, const NotificationType &InMessageID, const UINT64 &InMessageParam0, const UINT64 &InMessageParam1, const char* InMessageText, const UINT64 &InTimeStamp ) = 0;
			// C2S: Friend Accept
			virtual Result FriendAcceptedC2SEvt( const RouteContext &InRouteContext, const AccountID &InDestPlayerID, const ServerFriendInformation &InAccepter ) = 0;
			// C2S: Friend Remove
			virtual Result FriendRemovedC2SEvt( const RouteContext &InRouteContext, const AccountID &InDestPlayerID, const PlayerID &InRemoverID ) = 0;
			// C2S: Request Player Status Update
			virtual Result RequestPlayerStatusUpdateC2SEvt( const RouteContext &InRouteContext, const AccountID &InDestPlayerID, const PlayerID &InRequesterID ) = 0;
			// C2S: Notify Player Status Updated
			virtual Result NotifyPlayerStatusUpdatedC2SEvt( const RouteContext &InRouteContext, const AccountID &InDestPlayerID, const UINT32 &InLatestActiveTime, const BYTE &InIsInGame ) = 0;
			// C2S: Notification
			virtual Result NotifyPartyInviteC2SEvt( const RouteContext &InRouteContext, const PlayerID &InDestPlayerID, const PlayerID &InInviterID, const char* InInviterName, const EntityUID &InPartyUID ) = 0;

		}; // class IPolicyGameServer : public Net::INetPolicy


	}; // namespace Policy
}; // namespace BR


