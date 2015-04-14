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
#include "Net/Netdef.h"
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
			virtual HRESULT JoinPartyRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext, const PlayerID &InPartyLeaderID, const Array<BYTE>& InChatHistoryData ) = 0;
			// S2C: Player Joined
			virtual HRESULT PlayerJoinedS2CEvt( const RouteContext &InRouteContext, const PlayerInformation &InJoinedPlayer ) = 0;
			// S2C: Player Joined
			virtual HRESULT PartyLeaderChangedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InNewLeaderID ) = 0;
			// Cmd: Kick player
			virtual HRESULT LeavePartyRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext ) = 0;
			// S2C: Player left
			virtual HRESULT PlayerLeftS2CEvt( const RouteContext &InRouteContext, const PlayerID &InLeftPlayerID ) = 0;
			// Cmd: Kick player
			virtual HRESULT KickPlayerRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext ) = 0;
			// S2C: Player kicked
			virtual HRESULT PlayerKickedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InKickedPlayerID ) = 0;
			// C2S: Chatting message
			// S2C: Chatting message
			virtual HRESULT ChatMessageS2CEvt( const RouteContext &InRouteContext, const PlayerID &InSenderID, const char* InSenderName, const char* InChatMessage ) = 0;
			// C2S: Quick Chatting message
			// S2C: Quick Chatting message
			virtual HRESULT QuickChatMessageS2CEvt( const RouteContext &InRouteContext, const PlayerID &InSenderID, const UINT32 &InQuickChatID ) = 0;
			// Cmd: Match
			virtual HRESULT StartGameMatchRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext ) = 0;
			// S2C: Enqueued at a game matching
			virtual HRESULT QueuedGameMatchingS2CEvt( const RouteContext &InRouteContext, const MatchingQueueTicket &InMatchingQueueTicket ) = 0;
			// Cmd: Match
			virtual HRESULT CancelGameMatchRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext ) = 0;
			// S2C: Canceled at a game matching
			virtual HRESULT CanceledGameMatchingS2CEvt( const RouteContext &InRouteContext, const MatchingQueueTicket &InMatchingQueueTicket ) = 0;
			// S2C: Enqueued item is dequeued
			virtual HRESULT MatchingItemDequeuedS2CEvt( const RouteContext &InRouteContext, const MatchingQueueTicket &InMatchingTicket ) = 0;

		}; // class ISvrPolicyGameParty : public Net::INetPolicy


		class IPolicyGameParty : public Net::INetPolicy
		{
 			public:
			enum { ID_POLICY = POLICY_GAMEPARTY };

			// Constructor
			IPolicyGameParty( Net::IConnection* pConn ) : Net::INetPolicy(ID_POLICY, pConn)
			{}

			// Cmd: Join Party
			virtual HRESULT JoinPartyCmd( const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InInviterID, const PlayerInformation &InInvitedPlayer ) = 0;
			// S2C: Player Joined
			// S2C: Player Joined
			// Cmd: Kick player
			virtual HRESULT LeavePartyCmd( const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID ) = 0;
			// S2C: Player left
			// Cmd: Kick player
			virtual HRESULT KickPlayerCmd( const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick ) = 0;
			// S2C: Player kicked
			// C2S: Chatting message
			virtual HRESULT ChatMessageC2SEvt( const RouteContext &InRouteContext, const PlayerID &InPlayerID, const char* InChatMessage ) = 0;
			// S2C: Chatting message
			// C2S: Quick Chatting message
			virtual HRESULT QuickChatMessageC2SEvt( const RouteContext &InRouteContext, const PlayerID &InPlayerID, const UINT32 &InQuickChatID ) = 0;
			// S2C: Quick Chatting message
			// Cmd: Match
			virtual HRESULT StartGameMatchCmd( const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID, const UINT32 &InMaxGamePlayers ) = 0;
			// S2C: Enqueued at a game matching
			// Cmd: Match
			virtual HRESULT CancelGameMatchCmd( const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID ) = 0;
			// S2C: Canceled at a game matching
			// S2C: Enqueued item is dequeued

		}; // class IPolicyGameParty : public Net::INetPolicy


	}; // namespace Policy
}; // namespace BR


