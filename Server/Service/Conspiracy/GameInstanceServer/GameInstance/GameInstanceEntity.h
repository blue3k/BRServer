////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Game Instance entity
//	
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "SFTypedefs.h"

#include "Types/SFEngineTypedefs.h"
#include "Container/SFIndexing.h"
#include "Container/SFHashTable.h"
#include "Component/SFComponent.h"

#include "Transaction/MessageRoute.h"
#include "Entity/Entity.h"
#include "Server/BrServer.h"
#include "ServiceEntity/Game/GameSystem.h"
#include "ServiceEntity/Game/GameInstanceEntity.h"
#include "ServerSystem/GameLog/ChattingHistory.h"
#include "GameSystem/GameSystemComponentIDs.h"

#include "ConspiracyGameInstanceSvrConst.h"
#include "GamePlayer.h"

#include "Protocol/Policy/GameServerNetPolicy.h"
#include "Protocol/Policy/GameInstanceNetPolicy.h"

#include "conspiracy/GameConfigTbl.h"
#include "conspiracy/BotTalkTbl.h"


namespace SF{ 
namespace Net {
	class Connection;
}}

namespace SF
{
	namespace Policy
	{
		//interface ISvrPolicyGame;
		class ISvrPolicyGameInstance;
	};

};

namespace SF {
namespace ConspiracyGameInstanceServer
{
	class GameInstanceServer;
	class GamePlaySystem;
	class GameStateSystem;
	class GameLogSystem;
	class GamePlayer;


	//////////////////////////////////////////////////////////////////////////
	//
	//	Game Instance entity class
	//

	class GameInstanceEntity : public Svr::GameInstanceEntity
	{
	public:

		typedef conspiracy::GameConfigTbl::GameConfigItem GameConfigType;

	private:
		typedef Svr::GameInstanceEntity super;

		// Player by index
		GamePlayer* m_PlayerByIndex[GameLogItem::LEGACY_MAX_GAMEPLAYER]{};

		// Player character table
		uint8_t m_PlayerCharacter[GameLogItem::LEGACY_MAX_GAMEPLAYER]{};

		// Game configuration
		int m_TableVersion = -1;
		uint m_PresetGameConfigID = 1;
		GameConfigType* m_PresetGameConfig{};


		// Team Leader UID
		PlayerID m_LeaderUID{};


		//////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Game info
		//

		// Number of bot
		uint m_NumBot = 0;


		uint m_RoleRequestSeer = 0;
		uint m_RoleRequestWerewolf = 0;

		conspiracy::BotTalkTbl::BotTalkTblItem* m_pBotTalk{};


	public:

		//////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Game Instance
		//

		GameInstanceEntity();
		~GameInstanceEntity();


		GameConfigType* GetPresetGameConfig() { return m_PresetGameConfig; }

		PlayerID GetLeaderUID() { return m_LeaderUID; }
		void SetLeaderUID(PlayerID value) { m_LeaderUID = value; }

		uint GetRoleRequestSeer() { return m_RoleRequestSeer; }
		void SetRoleRequestSeer(uint value) { m_RoleRequestSeer = value; }

		uint GetRoleRequestWerewolf() { return m_RoleRequestWerewolf; }
		void SetRoleRequestWerewolf(uint value) { m_RoleRequestWerewolf = value; }

		uint GetNumBot() { return m_NumBot; }

		//// Get server instance
		//GameInstanceServer* GetMyOwner();

		// Get player count at this game
		//inline uint GetNumPlayer();

		conspiracy::BotTalkTbl::BotTalkTblItem *GetBotTalkTbl() { return m_pBotTalk; }


		//////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Game Systems
		//

		// Initialize game system
		Result InitializeSystem();

		// Update game config
		Result UpdateGameTable();


	private:
		// Initialize entity to proceed new connection
		virtual Result InitializeEntity( EntityID newEntityID ) override;

	public:
		// Close entity and clear transaction
		virtual Result TerminateEntity() override;

		// Run entity
		//virtual Result TickUpdate(TimerAction *pAction = nullptr) override;



		//////////////////////////////////////////////////////////////////////////////////////////
		//
		//	foreach implementations
		//

		// foreach game player
		template< class Func >
		Result ForeachPlayer(Func func);

		template< class Func >
		Result ForeachPlayerGameServer(Func func );

		template< class Func >
		Result ForeachPlayerSvrGameInstance(Func func );

		template< class Func >
		Result ForeachPlayerSvrPlayInstance(Func func);



		////////////////////////////////////////////////////////////
		//
		//	Game entity
		//

		// Initialize entity to proceed new connection
		virtual Result InitializeGameEntity(const ServerConfig::NetPublic& netPublic, const VariableTable& attributes) override;



		////////////////////////////////////////////////////////////
		//
		//	Update processes
		//


		// Update Game status
		virtual Result UpdateGameStatus(TimeStampMS ulCurTime ) override;





		//////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Game Player
		//

		Result GetPlayerIndex( PlayerID playerID, uint &playerIndex );
		Result GetPlayerByIndex( INT playerIndex, GamePlayer* &pGamePlayer );

		virtual Result CreatePlayerInstance(const PlayerInformation& playerInfo, SFUniquePtr<Svr::GameInstancePlayer>& pPlayer) override;

		// Register new player to join
		virtual Result AddPlayerToJoin(SFUniquePtr<Svr::GameInstancePlayer>& pPlayer ) override;


		// Leave all player
		Result LeaveAllPlayerForGameDelete();

		// Find Player pilotid
		Result FindPlayer( PlayerID pltID, GamePlayer* &pGamePlayer );

		// Called when a player get out of game
		virtual Result OnPlayerGetOutOfGame(Svr::GameInstancePlayer *pPlayer ) override;
	};

#include "GameInstanceEntity.inl"

}; // namespace ConspiracyGameInstanceServer
}; // namespace SF
