////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 StormForge
// 
// Author : Generated
// 
// Description : GameServer Service definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "SFTypedefs.h"
#include "Protocol/Protocol.h"
#include "ServerSystem/ServerServiceBase.h"
#include "Net/NetDef.h"
#include "Net/NetPolicy.h"
#include "Protocol/Policy/GameServerIPolicy.h"



namespace SF
{
 	namespace Svr
	{
 		class GameServerService : public ServerServiceBase
		{
 			private:
				Policy::IPolicyGameServer* GetPolicyGameServer() { return GetPolicy<Policy::IPolicyGameServer>(); }
			public:
				enum { ID_SERVICEPOLICY = Policy::IPolicyGameServer::ID_POLICY };
			// Constructor
			GameServerService ( ServerServiceInformation* pService );

			// Cmd: Kick
			Result RegisterPlayerToJoinGameServerCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID, const AuthTicket &InTicket, const FacebookUID &InFBUserID, const uint32_t &InShardID );
			// Cmd: Kick
			Result RegisterPlayerToJoinGameServerOnPlayerEntityCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID, const AuthTicket &InTicket, const FacebookUID &InFBUserID );
			// C2S: Chatting message
			Result ChatMessageC2SEvt( const EntityID &InSenderEntityID, const AccountID &InSenderID, const uint8_t &InRole, const char* InSenderName, const char* InChatMessage );
			// C2S: Notification
			Result NotifyC2SEvt( const EntityID &InSenderEntityID, const PlayerID &InDestPlayerID, const uint32_t &InNotificationID, const NotificationType &InMessageID, const uint64_t &InMessageParam0, const uint64_t &InMessageParam1, const char* InMessageText, const uint64_t &InTimeStamp );
			// C2S: Friend Accept
			Result FriendAcceptedC2SEvt( const EntityID &InSenderEntityID, const AccountID &InDestPlayerID, const ServerFriendInformation &InAccepter );
			// C2S: Friend Remove
			Result FriendRemovedC2SEvt( const EntityID &InSenderEntityID, const AccountID &InDestPlayerID, const PlayerID &InRemoverID );
			// C2S: Request Player Status Update
			Result RequestPlayerStatusUpdateC2SEvt( const EntityID &InSenderEntityID, const AccountID &InDestPlayerID, const PlayerID &InRequesterID );
			// C2S: Notify Player Status Updated
			Result NotifyPlayerStatusUpdatedC2SEvt( const EntityID &InSenderEntityID, const AccountID &InDestPlayerID, const uint32_t &InLatestActiveTime, const uint8_t &InIsInGame );
			// C2S: Notification
			Result NotifyPartyInviteC2SEvt( const EntityID &InSenderEntityID, const PlayerID &InDestPlayerID, const PlayerID &InInviterID, const char* InInviterName, const uint64_t &InPartyUID );

		}; // class GameServerService : public ServerServiceBase


	}; // namespace Svr
}; // namespace SF


