////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : GameParty Service definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Common/Typedefs.h"
#include "Common/PolicyID.h"
#include "ServerSystem/ServerServiceBase.h"
#include "Net/Netdef.h"
#include "Net/NetPolicy.h"
#include "Protocol/Policy/GamePartyIPolicy.h"



namespace BR
{
 	namespace Svr
	{
 		class GamePartyService : public ServerServiceBase
		{
 			private:
				Policy::IPolicyGameParty* GetPolicyGameParty() { return GetPolicy<Policy::IPolicyGameParty>(); }
			public:
				enum { ID_SERVICEPOLICY = Policy::IPolicyGameParty::ID_POLICY };
			// Constructor
			GamePartyService ( ServerServiceInformation* pService );

			// Cmd: Join Party
			HRESULT JoinPartyCmd( const Context &InContext, const PlayerID &InInviterID, const PlayerInformation &InInvitedPlayer );
			// Cmd: Kick player
			HRESULT LeavePartyCmd( const Context &InContext, const PlayerID &InPlayerID );
			// Cmd: Kick player
			HRESULT KickPlayerCmd( const Context &InContext, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick );
			// C2S: Chatting message
			HRESULT ChatMessageC2SEvt( const EntityID &InSenderEntityID, const PlayerID &InPlayerID, const char* InChatMessage );
			// C2S: Quick Chatting message
			HRESULT QuickChatMessageC2SEvt( const EntityID &InSenderEntityID, const PlayerID &InPlayerID, const UINT32 &InQuickChatID );
			// Cmd: Match
			HRESULT StartGameMatchCmd( const Context &InContext, const PlayerID &InPlayerID, const UINT32 &InMaxGamePlayers );
			// Cmd: Match
			HRESULT CancelGameMatchCmd( const Context &InContext, const PlayerID &InPlayerID );

		}; // class GamePartyService : public ServerServiceBase


	}; // namespace Svr
}; // namespace BR


