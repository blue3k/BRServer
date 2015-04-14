////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : GameMatchServer Message debug definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Common/Typedefs.h"
#include "Common/PolicyID.h"
#include "Net/Netdef.h"
#include "Net/NetPolicy.h"
#include "Protocol/Policy/GameMatchServerIPolicy.h"



namespace BR
{
 	namespace Policy
	{
 		class NetPolicyGameMatchServer : public IPolicyGameMatchServer
		{
 			public:
			// Constructor
			NetPolicyGameMatchServer ( Net::IConnection* pConn ) : IPolicyGameMatchServer(pConn)
			{}

			// Cmd: Match
			HRESULT RegisterPartyMatchCmd( const Context &InContext, const RouteContext &InRouteContext, const PartyUID &InPartyUID, const UINT32 &InNumGamePlayer );
			// Cmd: Match
			HRESULT RegisterPlayerMatchCmd( const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID, const UINT32 &InNumGamePlayer );

		}; // class NetPolicyGameMatchServer : public IPolicyGameMatchServer


		class NetSvrPolicyGameMatchServer : public ISvrPolicyGameMatchServer
		{
 			public:
			// Constructor
			NetSvrPolicyGameMatchServer ( Net::IConnection* pConn ) : ISvrPolicyGameMatchServer(pConn)
			{}

			// Cmd: Match
			HRESULT RegisterPartyMatchRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext );
			// Cmd: Match
			HRESULT RegisterPlayerMatchRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext );
			// S2C: Game is matched
			HRESULT GameMatchedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InDestPlayerID, const GameInsID &InGameInsID );

		}; // class NetSvrPolicyGameMatchServer : public ISvrPolicyGameMatchServer


	}; // namespace Policy
}; // namespace BR


