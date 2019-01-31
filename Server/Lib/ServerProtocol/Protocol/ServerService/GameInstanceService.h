////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 StormForge
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
 	namespace Svr
	{
 		class GameInstanceService : public ServerServiceBase
		{
 			public:
			// Constructor
			GameInstanceService ( ServerServiceInformation* pService );

			// C2S: Nitify that a game instance is deleted. Game instance send this message to manager before it destroy itself.
			Result DeleteGameC2SEvt( const EntityID &InSenderEntityID );
			// Cmd: Join to a game instance. You can call multiple times, but it would be waste
			Result JoinGameCmd( const TransactionID &InTransactionID, const PlayerInformation &InPlayer, const uint8_t &InRequestedRole );
			// C2S: For debug purpose, change configue preset. There is a game setting table. you can switch between those setting value.
			Result SetConfigPresetC2SEvt( const EntityID &InSenderEntityID, const uint32_t &InPresetID );
			// Cmd: Leave game instance.
			Result LeaveGameCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID );
			// Cmd: Kick player with given ID
			Result KickPlayerCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick );
			// Cmd: Assign new roles to all players.
			Result AssignRoleCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID );
			// C2S: in-game chatting message.
			Result ChatMessageC2SEvt( const EntityID &InSenderEntityID, const PlayerID &InPlayerID, const uint8_t &InRole, const char* InChatMessage );
			// Cmd: Advance game
			Result AdvanceGameCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID );
			// Cmd: *Vote game advance
			Result VoteGameAdvanceCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID );
			// Cmd: Assign role
			Result VoteCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InVoteTarget, const uint32_t &InActionSerial );
			// Cmd: Play again with the current players
			Result GamePlayAgainCmd( const TransactionID &InTransactionID, const PlayerID &InLeadPlayer, const uint64_t &InPartyUID );
			// Cmd: Player. revive himself
			Result GameRevealPlayerCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID, const Array<PlayerID>& InTargetPlayerID );
			// Cmd: Player. revive himself
			Result GamePlayerReviveCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID );

		}; // class GameInstanceService : public ServerServiceBase


	}; // namespace Svr
}; // namespace SF


