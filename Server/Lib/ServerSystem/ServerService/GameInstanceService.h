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
			Result DeleteGameC2SEvt( const EntityID &InSenderEntityID );
			// Cmd: Join Game
			Result JoinGameCmd( const TransactionID &InTransactionID, const PlayerInformation &InPlayer, const AuthTicket &InTicket, const PlayerRole &InRequestedRole );
			// C2S: Change configue preset
			Result SetConfigPresetC2SEvt( const EntityID &InSenderEntityID, const UINT32 &InPresetID );
			// Cmd: Leave Game
			Result LeaveGameCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID );
			// Cmd: Kick player
			Result KickPlayerCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick );
			// Cmd: Assign role
			Result AssignRoleCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID );
			// C2S: Chatting message
			Result ChatMessageC2SEvt( const EntityID &InSenderEntityID, const PlayerID &InPlayerID, const PlayerRole &InRole, const char* InChatMessage );
			// Cmd: Advance game
			Result AdvanceGameCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID );
			// Cmd: *Vote game advance
			Result VoteGameAdvanceCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID );
			// Cmd: Assign role
			Result VoteCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InVoteTarget, const UINT32 &InActionSerial );
			// Cmd: Play again with the current players
			Result GamePlayAgainCmd( const TransactionID &InTransactionID, const PlayerID &InLeadPlayer, const PartyUID &InPartyUID );
			// Cmd: Player. revive himself
			Result GameRevealPlayerCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID, const Array<PlayerID>& InTargetPlayerID );
			// Cmd: Player. revive himself
			Result GamePlayerReviveCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID );

		}; // class GameInstanceService : public ServerServiceBase


	}; // namespace Svr
}; // namespace BR


