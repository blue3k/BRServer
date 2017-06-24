////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2015 The Braves
// 
// Author : Generated
// 
// Description : GameParty Message debug definitions
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
 
		class ISvrPolicyGameParty : public Net::INetPolicy
		{
 			public:
			enum { ID_POLICY = POLICY_SVR_GAMEPARTY };

			// Constructor
			ISvrPolicyGameParty( Net::IConnection* pConn ) : Net::INetPolicy(ID_POLICY, pConn)
			{}

			// Cmd: Join Party
			virtual Result JoinPartyRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const PlayerID &InPartyLeaderID, const Array<uint8_t>& InChatHistoryData ) = 0;
			// S2C: Player Joined
			virtual Result PlayerJoinedS2CEvt( const RouteContext &InRouteContext, const PlayerInformation &InJoinedPlayer ) = 0;
			// S2C: Player Joined
			virtual Result PartyLeaderChangedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InNewLeaderID ) = 0;
			// Cmd: Kick player
			virtual Result LeavePartyRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult ) = 0;
			// S2C: Player left
			virtual Result PlayerLeftS2CEvt( const RouteContext &InRouteContext, const PlayerID &InLeftPlayerID ) = 0;
			// Cmd: Kick player
			virtual Result KickPlayerRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult ) = 0;
			// S2C: Player kicked
			virtual Result PlayerKickedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InKickedPlayerID ) = 0;
			// C2S: Chatting message
			// S2C: Chatting message
			virtual Result ChatMessageS2CEvt( const RouteContext &InRouteContext, const PlayerID &InSenderID, const char* InSenderName, const char* InChatMessage ) = 0;
			// C2S: Quick Chatting message
			// S2C: Quick Chatting message
			virtual Result QuickChatMessageS2CEvt( const RouteContext &InRouteContext, const PlayerID &InSenderID, const uint32_t &InQuickChatID ) = 0;
			// Cmd: Match
			virtual Result StartGameMatchRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult ) = 0;
			// S2C: Enqueued at a game matching
			virtual Result QueuedGameMatchingS2CEvt( const RouteContext &InRouteContext, const MatchingQueueTicket &InMatchingQueueTicket ) = 0;
			// Cmd: Match
			virtual Result CancelGameMatchRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult ) = 0;
			// S2C: Canceled at a game matching
			virtual Result CanceledGameMatchingS2CEvt( const RouteContext &InRouteContext, const MatchingQueueTicket &InMatchingQueueTicket ) = 0;
			// S2C: Enqueued item is dequeued
			virtual Result MatchingItemDequeuedS2CEvt( const RouteContext &InRouteContext, const MatchingQueueTicket &InMatchingTicket ) = 0;

		}; // class ISvrPolicyGameParty : public Net::INetPolicy


		class IPolicyGameParty : public Net::INetPolicy
		{
 			public:
			enum { ID_POLICY = POLICY_GAMEPARTY };

			// Constructor
			IPolicyGameParty( Net::IConnection* pConn ) : Net::INetPolicy(ID_POLICY, pConn)
			{}

			// Cmd: Join Party
			virtual Result JoinPartyCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InInviterID, const PlayerInformation &InInvitedPlayer ) = 0;
			// S2C: Player Joined
			// S2C: Player Joined
			// Cmd: Kick player
			virtual Result LeavePartyCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID ) = 0;
			// S2C: Player left
			// Cmd: Kick player
			virtual Result KickPlayerCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick ) = 0;
			// S2C: Player kicked
			// C2S: Chatting message
			virtual Result ChatMessageC2SEvt( const RouteContext &InRouteContext, const PlayerID &InPlayerID, const char* InChatMessage ) = 0;
			// S2C: Chatting message
			// C2S: Quick Chatting message
			virtual Result QuickChatMessageC2SEvt( const RouteContext &InRouteContext, const PlayerID &InPlayerID, const uint32_t &InQuickChatID ) = 0;
			// S2C: Quick Chatting message
			// Cmd: Match
			virtual Result StartGameMatchCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID, const uint32_t &InMaxGamePlayers ) = 0;
			// S2C: Enqueued at a game matching
			// Cmd: Match
			virtual Result CancelGameMatchCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID ) = 0;
			// S2C: Canceled at a game matching
			// S2C: Enqueued item is dequeued

		}; // class IPolicyGameParty : public Net::INetPolicy


	}; // namespace Policy
}; // namespace BR


