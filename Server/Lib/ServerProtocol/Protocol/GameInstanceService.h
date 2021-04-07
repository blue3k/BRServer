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
#include "Protocol/GameInstanceNetPolicy.h"



namespace SF
{
 	class GameInstanceService : public ServerServiceBase
	{
 		public:
		// Constructor
		GameInstanceService ( ServerServiceInformation* pService );

		// Cmd: Join to a game instance. You can call multiple times, but it would be a waste
		Result JoinGameInstanceCmd( const uint64_t &InTransactionID, const PlayerInformation &InPlayer, const VariableTable &InCharacterVisual, const VariableTable &InCharacterAttribute );
		// Cmd: Leave game instance.
		Result LeaveGameInstanceCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID );
		// Cmd: Player. revive himself
		Result GamePlayerReviveCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID );

	}; // class GameInstanceService : public ServerServiceBase


}; // namespace SF


