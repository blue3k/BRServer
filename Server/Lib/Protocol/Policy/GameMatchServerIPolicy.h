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
 
		interface ISvrPolicyGameMatchServer : Net::INetPolicy
		{
 			enum { ID_POLICY = POLICY_SVR_GAMEMATCHSERVER };

			// Constructor
			ISvrPolicyGameMatchServer( Net::IConnection* pConn ) : Net::INetPolicy(ID_POLICY, pConn)
			{}

			// Cmd: Match
			virtual HRESULT RegisterPartyMatchRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext ) = 0;
			// Cmd: Match
			virtual HRESULT RegisterPlayerMatchRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext ) = 0;
			// S2C: Game is matched
			virtual HRESULT GameMatchedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InDestPlayerID, const GameInsID &InGameInsID ) = 0;

		}; // interface ISvrPolicyGameMatchServer : Net::INetPolicy


		interface IPolicyGameMatchServer : Net::INetPolicy
		{
 			enum { ID_POLICY = POLICY_GAMEMATCHSERVER };

			// Constructor
			IPolicyGameMatchServer( Net::IConnection* pConn ) : Net::INetPolicy(ID_POLICY, pConn)
			{}

			// Cmd: Match
			virtual HRESULT RegisterPartyMatchCmd( const Context &InContext, const RouteContext &InRouteContext, const PartyUID &InPartyUID, const UINT32 &InNumGamePlayer ) = 0;
			// Cmd: Match
			virtual HRESULT RegisterPlayerMatchCmd( const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID, const UINT32 &InNumGamePlayer ) = 0;
			// S2C: Game is matched

		}; // interface IPolicyGameMatchServer : Net::INetPolicy


	}; // namespace Policy
}; // namespace BR


