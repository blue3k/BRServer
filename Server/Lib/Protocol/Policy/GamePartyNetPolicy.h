////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : GameParty Message debug definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "SFTypedefs.h"
#include "Protocol/Protocol.h"
#include "Net/NetDef.h"
#include "Net/NetPolicy.h"
#include "Protocol/Policy/GamePartyIPolicy.h"



namespace BR
{
 	namespace Policy
	{
 		class NetPolicyGameParty : public IPolicyGameParty
		{
 			public:
			// Constructor
			NetPolicyGameParty ( Net::Connection* pConn ) : IPolicyGameParty(pConn)
			{}

			// Cmd: Join Party
			Result JoinPartyCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InInviterID, const PlayerInformation &InInvitedPlayer );
			// Cmd: Kick player
			Result LeavePartyCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID );
			// Cmd: Kick player
			Result KickPlayerCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick );
			// C2S: Chatting message
			Result ChatMessageC2SEvt( const RouteContext &InRouteContext, const PlayerID &InPlayerID, const char* InChatMessage );
			// C2S: Quick Chatting message
			Result QuickChatMessageC2SEvt( const RouteContext &InRouteContext, const PlayerID &InPlayerID, const uint32_t &InQuickChatID );
			// Cmd: Match
			Result StartGameMatchCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID, const uint32_t &InMaxGamePlayers );
			// Cmd: Match
			Result CancelGameMatchCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID );

		}; // class NetPolicyGameParty : public IPolicyGameParty


		class NetSvrPolicyGameParty : public ISvrPolicyGameParty
		{
 			public:
			// Constructor
			NetSvrPolicyGameParty ( Net::Connection* pConn ) : ISvrPolicyGameParty(pConn)
			{}

			// Cmd: Join Party
			Result JoinPartyRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const PlayerID &InPartyLeaderID, const Array<uint8_t>& InChatHistoryData );
			// S2C: Player Joined
			Result PlayerJoinedS2CEvt( const RouteContext &InRouteContext, const PlayerInformation &InJoinedPlayer );
			// S2C: Player Joined
			Result PartyLeaderChangedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InNewLeaderID );
			// Cmd: Kick player
			Result LeavePartyRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult );
			// S2C: Player left
			Result PlayerLeftS2CEvt( const RouteContext &InRouteContext, const PlayerID &InLeftPlayerID );
			// Cmd: Kick player
			Result KickPlayerRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult );
			// S2C: Player kicked
			Result PlayerKickedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InKickedPlayerID );
			// S2C: Chatting message
			Result ChatMessageS2CEvt( const RouteContext &InRouteContext, const PlayerID &InSenderID, const char* InSenderName, const char* InChatMessage );
			// S2C: Quick Chatting message
			Result QuickChatMessageS2CEvt( const RouteContext &InRouteContext, const PlayerID &InSenderID, const uint32_t &InQuickChatID );
			// Cmd: Match
			Result StartGameMatchRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult );
			// S2C: Enqueued at a game matching
			Result QueuedGameMatchingS2CEvt( const RouteContext &InRouteContext, const MatchingQueueTicket &InMatchingQueueTicket );
			// Cmd: Match
			Result CancelGameMatchRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult );
			// S2C: Canceled at a game matching
			Result CanceledGameMatchingS2CEvt( const RouteContext &InRouteContext, const MatchingQueueTicket &InMatchingQueueTicket );
			// S2C: Enqueued item is dequeued
			Result MatchingItemDequeuedS2CEvt( const RouteContext &InRouteContext, const MatchingQueueTicket &InMatchingTicket );

		}; // class NetSvrPolicyGameParty : public ISvrPolicyGameParty


	}; // namespace Policy
}; // namespace BR


