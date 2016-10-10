////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : GameServer Service definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Common/Typedefs.h"
#include "Common/PolicyID.h"
#include "ServerSystem/ServerServiceBase.h"
#include "Net/NetDef.h"
#include "Net/NetPolicy.h"
#include "Protocol/Policy/GameServerIPolicy.h"



namespace BR
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
			Result RegisterPlayerToJoinGameServerCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID, const AuthTicket &InTicket, const FacebookUID &InFBUserID, const UINT32 &InShardID );
			// Cmd: Kick
			Result RegisterPlayerToJoinGameServerOnPlayerEntityCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID, const AuthTicket &InTicket, const FacebookUID &InFBUserID );
			// C2S: Chatting message
			Result ChatMessageC2SEvt( const EntityID &InSenderEntityID, const AccountID &InSenderID, const PlayerRole &InRole, const char* InSenderName, const char* InChatMessage );
			// C2S: Notification
			Result NotifyC2SEvt( const EntityID &InSenderEntityID, const PlayerID &InDestPlayerID, const UINT32 &InNotificationID, const NotificationType &InMessageID, const UINT64 &InMessageParam0, const UINT64 &InMessageParam1, const char* InMessageText, const UINT64 &InTimeStamp );
			// C2S: Friend Accept
			Result FriendAcceptedC2SEvt( const EntityID &InSenderEntityID, const AccountID &InDestPlayerID, const ServerFriendInformation &InAccepter );
			// C2S: Friend Remove
			Result FriendRemovedC2SEvt( const EntityID &InSenderEntityID, const AccountID &InDestPlayerID, const PlayerID &InRemoverID );
			// C2S: Request Player Status Update
			Result RequestPlayerStatusUpdateC2SEvt( const EntityID &InSenderEntityID, const AccountID &InDestPlayerID, const PlayerID &InRequesterID );
			// C2S: Notify Player Status Updated
			Result NotifyPlayerStatusUpdatedC2SEvt( const EntityID &InSenderEntityID, const AccountID &InDestPlayerID, const UINT32 &InLatestActiveTime, const BYTE &InIsInGame );
			// C2S: Notification
			Result NotifyPartyInviteC2SEvt( const EntityID &InSenderEntityID, const PlayerID &InDestPlayerID, const PlayerID &InInviterID, const char* InInviterName, const EntityUID &InPartyUID );

		}; // class GameServerService : public ServerServiceBase


	}; // namespace Svr
}; // namespace BR


