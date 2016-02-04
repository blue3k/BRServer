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

#include "Common/Typedefs.h"
#include "Common/PolicyID.h"
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
			NetPolicyGameParty ( Net::IConnection* pConn ) : IPolicyGameParty(pConn)
			{}

			// Cmd: Join Party
			HRESULT JoinPartyCmd( const RouteContext &InRouteContext, const Context &InContext, const PlayerID &InInviterID, const PlayerInformation &InInvitedPlayer );
			// Cmd: Kick player
			HRESULT LeavePartyCmd( const RouteContext &InRouteContext, const Context &InContext, const PlayerID &InPlayerID );
			// Cmd: Kick player
			HRESULT KickPlayerCmd( const RouteContext &InRouteContext, const Context &InContext, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick );
			// C2S: Chatting message
			HRESULT ChatMessageC2SEvt( const RouteContext &InRouteContext, const PlayerID &InPlayerID, const char* InChatMessage );
			// C2S: Quick Chatting message
			HRESULT QuickChatMessageC2SEvt( const RouteContext &InRouteContext, const PlayerID &InPlayerID, const UINT32 &InQuickChatID );
			// Cmd: Match
			HRESULT StartGameMatchCmd( const RouteContext &InRouteContext, const Context &InContext, const PlayerID &InPlayerID, const UINT32 &InMaxGamePlayers );
			// Cmd: Match
			HRESULT CancelGameMatchCmd( const RouteContext &InRouteContext, const Context &InContext, const PlayerID &InPlayerID );

		}; // class NetPolicyGameParty : public IPolicyGameParty


		class NetSvrPolicyGameParty : public ISvrPolicyGameParty
		{
 			public:
			// Constructor
			NetSvrPolicyGameParty ( Net::IConnection* pConn ) : ISvrPolicyGameParty(pConn)
			{}

			// Cmd: Join Party
			HRESULT JoinPartyRes( const RouteContext &InRouteContext, const Context &InContext, const HRESULT &InResult, const PlayerID &InPartyLeaderID, const Array<BYTE>& InChatHistoryData );
			// S2C: Player Joined
			HRESULT PlayerJoinedS2CEvt( const RouteContext &InRouteContext, const PlayerInformation &InJoinedPlayer );
			// S2C: Player Joined
			HRESULT PartyLeaderChangedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InNewLeaderID );
			// Cmd: Kick player
			HRESULT LeavePartyRes( const RouteContext &InRouteContext, const Context &InContext, const HRESULT &InResult );
			// S2C: Player left
			HRESULT PlayerLeftS2CEvt( const RouteContext &InRouteContext, const PlayerID &InLeftPlayerID );
			// Cmd: Kick player
			HRESULT KickPlayerRes( const RouteContext &InRouteContext, const Context &InContext, const HRESULT &InResult );
			// S2C: Player kicked
			HRESULT PlayerKickedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InKickedPlayerID );
			// S2C: Chatting message
			HRESULT ChatMessageS2CEvt( const RouteContext &InRouteContext, const PlayerID &InSenderID, const char* InSenderName, const char* InChatMessage );
			// S2C: Quick Chatting message
			HRESULT QuickChatMessageS2CEvt( const RouteContext &InRouteContext, const PlayerID &InSenderID, const UINT32 &InQuickChatID );
			// Cmd: Match
			HRESULT StartGameMatchRes( const RouteContext &InRouteContext, const Context &InContext, const HRESULT &InResult );
			// S2C: Enqueued at a game matching
			HRESULT QueuedGameMatchingS2CEvt( const RouteContext &InRouteContext, const MatchingQueueTicket &InMatchingQueueTicket );
			// Cmd: Match
			HRESULT CancelGameMatchRes( const RouteContext &InRouteContext, const Context &InContext, const HRESULT &InResult );
			// S2C: Canceled at a game matching
			HRESULT CanceledGameMatchingS2CEvt( const RouteContext &InRouteContext, const MatchingQueueTicket &InMatchingQueueTicket );
			// S2C: Enqueued item is dequeued
			HRESULT MatchingItemDequeuedS2CEvt( const RouteContext &InRouteContext, const MatchingQueueTicket &InMatchingTicket );

		}; // class NetSvrPolicyGameParty : public ISvrPolicyGameParty


	}; // namespace Policy
}; // namespace BR


