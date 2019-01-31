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
#include "Protocol/SFProtocol.h"
#include "Net/SFNetDef.h"



namespace SF
{
 	namespace Policy
	{
 		class NetPolicyGameParty 
		{
 			private:
				SharedPointerT<Net::Connection> m_pConnection;
			public:
			// Constructor
			NetPolicyGameParty ( const SharedPointerT<Net::Connection>& pConn ) : m_pConnection(pConn)
			{}
			NetPolicyGameParty ( const SharedPointerAtomicT<Net::Connection>& pConn ) : m_pConnection(pConn)
			{}

			// Cmd: Join party
			Result JoinPartyCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InInviterID, const PlayerInformation &InInvitedPlayer );
			// Cmd: Event for player left.
			Result LeavePartyCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID );
			// Cmd: Kick player
			Result KickPlayerCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick );
			// C2S: Send chat message to server.
			Result ChatMessageC2SEvt( const RouteContext &InRouteContext, const PlayerID &InPlayerID, const char* InChatMessage );
			// C2S: Quick Chatting message
			Result QuickChatMessageC2SEvt( const RouteContext &InRouteContext, const PlayerID &InPlayerID, const uint32_t &InQuickChatID );
			// Cmd: Start party game matching
			Result StartGameMatchCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID, const uint32_t &InMaxGamePlayers );
			// Cmd: Cancel game matching
			Result CancelGameMatchCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID );

		}; // class NetPolicyGameParty 


		class NetSvrPolicyGameParty
		{
 			private:
				SharedPointerT<Net::Connection> m_pConnection;
			public:
			// Constructor
			NetSvrPolicyGameParty ( const SharedPointerT<Net::Connection>& pConn ) : m_pConnection(pConn)
			{}
			NetSvrPolicyGameParty ( const SharedPointerAtomicT<Net::Connection>& pConn ) : m_pConnection(pConn)
			{}

			// Cmd: Join party
			Result JoinPartyRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const PlayerID &InPartyLeaderID, const Array<uint8_t>& InChatHistoryData );
			// S2C: Player joined event.
			Result PlayerJoinedS2CEvt( const RouteContext &InRouteContext, const PlayerInformation &InJoinedPlayer );
			// S2C: Event for party leader is changed
			Result PartyLeaderChangedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InNewLeaderID );
			// Cmd: Event for player left.
			Result LeavePartyRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult );
			// S2C: Player left
			Result PlayerLeftS2CEvt( const RouteContext &InRouteContext, const PlayerID &InLeftPlayerID );
			// Cmd: Kick player
			Result KickPlayerRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult );
			// S2C: Event for Player kicked.
			Result PlayerKickedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InKickedPlayerID );
			// S2C: Brocast chatting message
			Result ChatMessageS2CEvt( const RouteContext &InRouteContext, const PlayerID &InSenderID, const char* InSenderName, const char* InChatMessage );
			// S2C: Quick Chatting message
			Result QuickChatMessageS2CEvt( const RouteContext &InRouteContext, const PlayerID &InSenderID, const uint32_t &InQuickChatID );
			// Cmd: Start party game matching
			Result StartGameMatchRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult );
			// S2C: Event sent when the party is queued for game matching
			Result QueuedGameMatchingS2CEvt( const RouteContext &InRouteContext, const MatchingQueueTicket &InMatchingQueueTicket );
			// Cmd: Cancel game matching
			Result CancelGameMatchRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult );
			// S2C: Event sent when the game matching is canceled
			Result CanceledGameMatchingS2CEvt( const RouteContext &InRouteContext, const MatchingQueueTicket &InMatchingQueueTicket );
			// S2C: Event sent when the the matching has dequeued.
			Result MatchingItemDequeuedS2CEvt( const RouteContext &InRouteContext, const MatchingQueueTicket &InMatchingTicket );

		}; // class NetSvrPolicyGameParty


	}; // namespace Policy
}; // namespace SF


