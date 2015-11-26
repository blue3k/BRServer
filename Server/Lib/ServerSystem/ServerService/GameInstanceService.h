////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : GameInstance Service definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Common/Typedefs.h"
#include "Common/PolicyID.h"
#include "ServerSystem/ServerServiceBase.h"
#include "Net/NetDef.h"
#include "Net/NetPolicy.h"
#include "Protocol/Policy/GameInstanceIPolicy.h"



namespace BR
{
 	namespace Svr
	{
 		class GameInstanceService : public ServerServiceBase
		{
 			private:
				Policy::IPolicyGameInstance* GetPolicyGameInstance() { return GetPolicy<Policy::IPolicyGameInstance>(); }
			public:
				enum { ID_SERVICEPOLICY = Policy::IPolicyGameInstance::ID_POLICY };
			// Constructor
			GameInstanceService ( ServerServiceInformation* pService );

			// C2S: Game instance deletion
			HRESULT DeleteGameC2SEvt( const EntityID &InSenderEntityID );
			// Cmd: Join Game
			HRESULT JoinGameCmd( const Context &InContext, const PlayerInformation &InPlayer, const AuthTicket &InTicket, const PlayerRole &InRequestedRole );
			// C2S: Change configue preset
			HRESULT SetConfigPresetC2SEvt( const EntityID &InSenderEntityID, const UINT32 &InPresetID );
			// Cmd: Leave Game
			HRESULT LeaveGameCmd( const Context &InContext, const PlayerID &InPlayerID );
			// Cmd: Kick player
			HRESULT KickPlayerCmd( const Context &InContext, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick );
			// Cmd: Assign role
			HRESULT AssignRoleCmd( const Context &InContext, const PlayerID &InPlayerID );
			// C2S: Chatting message
			HRESULT ChatMessageC2SEvt( const EntityID &InSenderEntityID, const PlayerID &InPlayerID, const PlayerRole &InRole, const char* InChatMessage );
			// Cmd: Advance game
			HRESULT AdvanceGameCmd( const Context &InContext, const PlayerID &InPlayerID );
			// Cmd: *Vote game advance
			HRESULT VoteGameAdvanceCmd( const Context &InContext, const PlayerID &InPlayerID );
			// Cmd: Assign role
			HRESULT VoteCmd( const Context &InContext, const PlayerID &InPlayerID, const PlayerID &InVoteTarget, const UINT32 &InActionSerial );
			// Cmd: Play again with the current players
			HRESULT GamePlayAgainCmd( const Context &InContext, const PlayerID &InLeadPlayer, const PartyUID &InPartyUID );
			// Cmd: Player. revive himself
			HRESULT GameRevealPlayerCmd( const Context &InContext, const PlayerID &InPlayerID, const Array<PlayerID>& InTargetPlayerID );
			// Cmd: Player. revive himself
			HRESULT GamePlayerReviveCmd( const Context &InContext, const PlayerID &InPlayerID );

		}; // class GameInstanceService : public ServerServiceBase


	}; // namespace Svr
}; // namespace BR


