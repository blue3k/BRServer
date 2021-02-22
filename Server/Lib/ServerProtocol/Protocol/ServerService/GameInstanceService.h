////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) Kyungkun Ko
// 
// Author : Generated
// 
// Description : GameInstance Service definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "SFTypedefs.h"
#include "Protocol/SFProtocol.h"
#include "ServerService/ServerServiceBase.h"
#include "Net/SFNetDef.h"
#include "Protocol/Policy/GameInstanceNetPolicy.h"



namespace SF
{
 	class GameInstanceService : public ServerServiceBase
	{
 		public:
		// Constructor
		GameInstanceService ( ServerServiceInformation* pService );

		// Cmd: Join to a game instance. You can call multiple times, but it would be a waste
		Result JoinGameInstanceCmd( const uint64_t &InTransactionID, const PlayerInformation &InPlayer );
		// Cmd: Leave game instance.
		Result LeaveGameInstanceCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID );
		// C2S: Player Movement
		Result PlayerMovementC2SEvt( const EntityID &InSenderEntityID, const uint64_t &InGameInsUID, const PlayerID &InPlayerID, const VariableTable &InAttributes );
		// Cmd: Kick player with given ID
		Result KickPlayerCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick );
		// Cmd: Join to a game instance. You can call multiple times, but it would be waste
		Result JoinGameCmd( const uint64_t &InTransactionID, const PlayerInformation &InPlayer, const uint8_t &InRequestedRole );
		// Cmd: Assign new roles to all players.
		Result AssignRoleCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID );
		// C2S: in-game chatting message.
		Result ChatMessageC2SEvt( const EntityID &InSenderEntityID, const PlayerID &InPlayerID, const uint8_t &InRole, const char* InChatMessage );
		// Cmd: Advance game
		Result AdvanceGameCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID );
		// Cmd: *Vote game advance
		Result VoteGameAdvanceCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID );
		// Cmd: Assign role
		Result VoteCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InVoteTarget, const uint32_t &InActionSerial );
		// Cmd: Play again with the current players
		Result GamePlayAgainCmd( const uint64_t &InTransactionID, const PlayerID &InLeadPlayer, const uint64_t &InPartyUID );
		// Cmd: Player. revive himself
		Result GameRevealPlayerCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID, const Array<PlayerID>& InTargetPlayerID );
		// Cmd: Player. revive himself
		Result GamePlayerReviveCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID );

	}; // class GameInstanceService : public ServerServiceBase


}; // namespace SF


