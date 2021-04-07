////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) The Braves
// 
// Author : Generated
// 
// Description : GameInstance Message debug definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "SFTypedefs.h"
#include "Protocol/SFProtocol.h"
#include "Net/SFNetDef.h"



namespace SF
{
    class MessageEndpoint;
   class VariableTable;
   struct ActorMovement;


	class NetPolicyGameInstance 
	{
 		private:
			SharedPointerT<MessageEndpoint> m_Endpoint;
		public:
		// Constructor
		NetPolicyGameInstance (const SharedPointerT<MessageEndpoint>& pEndpoint ) : m_Endpoint(pEndpoint)
		{}

		// Cmd: Join to a game instance. You can call multiple times, but it would be a waste
		Result JoinGameInstanceCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerInformation &InPlayer, const VariableTable &InCharacterVisual, const VariableTable &InCharacterAttribute );
		// Cmd: Leave game instance.
		Result LeaveGameInstanceCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID );
		// Cmd: Player. revive himself
		Result GamePlayerReviveCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID );

	}; // class NetPolicyGameInstance 


	class NetSvrPolicyGameInstance
	{
 		private:
			SharedPointerT<MessageEndpoint> m_Endpoint;
		public:
		// Constructor
		NetSvrPolicyGameInstance ( const SharedPointerT<MessageEndpoint>& pEndpoint ) : m_Endpoint(pEndpoint)
		{}

		// Cmd: Join to a game instance. You can call multiple times, but it would be a waste
		Result JoinGameInstanceRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const NetAddress &InGameInsSvr4, const NetAddress &InGameInsSvr6, const uint32_t &InTimeStamp );
		// Cmd: Leave game instance.
		Result LeaveGameInstanceRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult );
		// S2C: Player left event.
		Result PlayerLeftS2CEvt( const RouteContext &InRouteContext, const PlayerID &InLeftPlayerID );
		// Cmd: Player. revive himself
		Result GamePlayerReviveRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult );
		// S2C: Player is revived
		Result GamePlayerRevivedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InRevivedPlayerID );

	}; // class NetSvrPolicyGameInstance


}; // namespace SF


