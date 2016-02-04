////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : Ranking Message debug definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Common/Typedefs.h"
#include "Common/PolicyID.h"
#include "Net/NetDef.h"
#include "Net/NetPolicy.h"
#include "Protocol/Policy/RankingIPolicy.h"



namespace BR
{
 	namespace Policy
	{
 		class NetPolicyRanking : public IPolicyRanking
		{
 			public:
			// Constructor
			NetPolicyRanking ( Net::IConnection* pConn ) : IPolicyRanking(pConn)
			{}

			// Cmd: Add a player to ranking
			HRESULT AddPlayerCmd( const RouteContext &InRouteContext, const Context &InContext, const PlayerID &InPlayerID, const UINT64 &InRankingScore, const Array<BYTE>& InPlayerInfo );
			// Cmd: Remove a player to ranking
			HRESULT RemovePlayerCmd( const RouteContext &InRouteContext, const Context &InContext, const PlayerID &InPlayerID );
			// Cmd: Remove a player to ranking
			HRESULT GetPlayerRankingCmd( const RouteContext &InRouteContext, const Context &InContext, const PlayerID &InPlayerID );
			// Cmd: Update a player to ranking
			HRESULT UpdatePlayerCmd( const RouteContext &InRouteContext, const Context &InContext, const PlayerID &InPlayerID, const UINT64 &InRankingScore, const Array<BYTE>& InPlayerInfo );
			// Cmd: Remove a player to ranking
			HRESULT GetRankingCmd( const RouteContext &InRouteContext, const Context &InContext, const UINT32 &InFrom, const UINT32 &InCount );

		}; // class NetPolicyRanking : public IPolicyRanking


		class NetSvrPolicyRanking : public ISvrPolicyRanking
		{
 			public:
			// Constructor
			NetSvrPolicyRanking ( Net::IConnection* pConn ) : ISvrPolicyRanking(pConn)
			{}

			// Cmd: Add a player to ranking
			HRESULT AddPlayerRes( const RouteContext &InRouteContext, const Context &InContext, const HRESULT &InResult, const UINT32 &InRanking );
			// Cmd: Remove a player to ranking
			HRESULT RemovePlayerRes( const RouteContext &InRouteContext, const Context &InContext, const HRESULT &InResult );
			// Cmd: Remove a player to ranking
			HRESULT GetPlayerRankingRes( const RouteContext &InRouteContext, const Context &InContext, const HRESULT &InResult, const UINT32 &InRanking );
			// Cmd: Update a player to ranking
			HRESULT UpdatePlayerRes( const RouteContext &InRouteContext, const Context &InContext, const HRESULT &InResult, const UINT32 &InRanking );
			// Cmd: Remove a player to ranking
			HRESULT GetRankingRes( const RouteContext &InRouteContext, const Context &InContext, const HRESULT &InResult, const UINT32 &InPlayerRanking );

		}; // class NetSvrPolicyRanking : public ISvrPolicyRanking


	}; // namespace Policy
}; // namespace BR


