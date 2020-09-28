﻿////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : ChatChannel Message debug definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "SFTypedefs.h"
#include "Protocol/SFProtocol.h"
#include "Net/SFNetDef.h"



class VariableTable;



namespace SF
{
 	namespace Policy
	{
 		class NetPolicyChatChannel 
		{
 			private:
				SharedPointerT<Net::Connection> m_pConnection;
			public:
			// Constructor
			NetPolicyChatChannel ( const SharedPointerT<Net::Connection>& pConn ) : m_pConnection(pConn)
			{}
			NetPolicyChatChannel ( const SharedPointerAtomicT<Net::Connection>& pConn ) : m_pConnection(pConn)
			{}

			// Cmd: Request to join chat channel
			Result JoinCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const char* InPasscode, const PlayerInformation &InJoiningPlayer );
			// Cmd: Leave chat channel
			Result LeaveCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID );
			// Cmd: Kick a player
			Result KickPlayerCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick );
			// C2S: Sending a chatting message
			Result ChatMessageC2SEvt( const RouteContext &InRouteContext, const PlayerID &InSenderID, const char* InChatMessage );

		}; // class NetPolicyChatChannel 


		class NetSvrPolicyChatChannel
		{
 			private:
				SharedPointerT<Net::Connection> m_pConnection;
			public:
			// Constructor
			NetSvrPolicyChatChannel ( const SharedPointerT<Net::Connection>& pConn ) : m_pConnection(pConn)
			{}
			NetSvrPolicyChatChannel ( const SharedPointerAtomicT<Net::Connection>& pConn ) : m_pConnection(pConn)
			{}

			// Cmd: Request to join chat channel
			Result JoinRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const PlayerID &InChatChannelLeaderID );
			// S2C: Server envent to notify joined player information
			Result PlayerJoinedS2CEvt( const RouteContext &InRouteContext, const PlayerInformation &InJoinedPlayer );
			// S2C: Chat channel leader changed
			Result LeaderChangedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InNewLeaderID );
			// Cmd: Leave chat channel
			Result LeaveRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult );
			// S2C: Notification event when a player left
			Result PlayerLeftS2CEvt( const RouteContext &InRouteContext, const PlayerID &InLeftPlayerID );
			// Cmd: Kick a player
			Result KickPlayerRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult );
			// S2C: Notification event when a player kicked
			Result PlayerKickedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InKickedPlayerID );
			// S2C: brocasting event for a chatting message
			Result ChatMessageS2CEvt( const RouteContext &InRouteContext, const PlayerID &InSenderID, const char* InSenderName, const char* InChatMessage );

		}; // class NetSvrPolicyChatChannel


	}; // namespace Policy
}; // namespace SF


