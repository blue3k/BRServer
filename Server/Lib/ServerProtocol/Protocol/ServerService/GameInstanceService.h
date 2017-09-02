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
#include "Protocol/Protocol.h"
#include "ServerService/ServerServiceBase.h"
#include "Net/NetDef.h"
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

			// C2S: Game instance deletion
			Result DeleteGameC2SEvt( const EntityID &InSenderEntityID );
			// Cmd: Join Game
			Result JoinGameCmd( const TransactionID &InTransactionID, const PlayerInformation &InPlayer, const AuthTicket &InTicket, const uint8_t &InRequestedRole );
			// C2S: Change configue preset
			Result SetConfigPresetC2SEvt( const EntityID &InSenderEntityID, const uint32_t &InPresetID );
			// Cmd: Leave Game
			Result LeaveGameCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID );
			// Cmd: Kick player
			Result KickPlayerCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick );
			// Cmd: Assign role
			Result AssignRoleCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID );
			// C2S: Chatting message
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


