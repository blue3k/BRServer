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
#include "Common/ClassUtil.h"
#include "Types/SFEngineTypedefs.h"
#include "Container/Indexing.h"
#include "Common/HashTable.h"
#include "Component/BrComponent.h"

#include "Transaction/MessageRoute.h"
#include "Entity/Entity.h"
#include "Server/BrServer.h"
#include "ServiceEntity/Game/GameSystem.h"
#include "ServerSystem/ServiceEntity/Game/GameInstanceEntity.h"
#include "ServerSystem/GameLog/ChattingHistory.h"
#include "GameSystem/GameSystemComponentIDs.h"

#include "ConspiracyGameInstanceSvrConst.h"
#include "GamePlayer.h"

#include "Protocol/Policy/GameServerNetPolicy.h"
#include "Protocol/Policy/GameInstanceNetPolicy.h"

#include "Table/conspiracy/GameConfigTbl.h"
#include "Table/conspiracy/BotTalkTbl.h"


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
		class IPolicyGameMasterServer;
	};

};

namespace SF {
namespace ConspiracyGameInstanceServer
{
	class GameInstanceServer;
	class GamePlaySystem;
	class GameStateSystem;
	class GameLogSystem;
	class GameMasterServerEntity;
	class GamePlayer;


	//////////////////////////////////////////////////////////////////////////
	//
	//	Game Instance entity class
	//

	class GameInstanceEntity : public Svr::GameInstanceEntity, public MemoryPoolObject<GameInstanceEntity>, public ComponentCarrier<GameSystemComponentID_Max>
	{
	public:

		typedef ::conspiracy::GameConfigTbl::GameConfigItem GameConfigType;

	private:
		typedef Svr::GameInstanceEntity super;


		// Player by index
		GamePlayer*				m_PlayerByIndex[GameConst::MAX_GAMEPLAYER];

		// Player character table
		uint8_t					m_PlayerCharacter[GameConst::MAX_GAMEPLAYER];

		// Game configuration
		int m_TableVersion;
		uint					m_PresetGameConfigID;
		BRCLASS_ATTRIBUTE_READONLY_PTR(GameConfigType*,PresetGameConfig);

		//////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Game info
		//


		BRCLASS_ATTRIBUTE(uint, RoleRequestSeer);
		BRCLASS_ATTRIBUTE(uint, RoleRequestWerewolf);

		conspiracy::BotTalkTbl::BotTalkTblItem *m_pBotTalk;


	public:

		//////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Game Instance
		//

		GameInstanceEntity();
		~GameInstanceEntity();

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



		////////////////////////////////////////////////////////////
		//
		//	Game entity
		//

		// Initialize entity to proceed new connection
		virtual Result InitializeGameEntity( uint numBot, uint maxPlayer) override;



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

		virtual Result CreatePlayerInstance(const PlayerInformation& playerInfo, Svr::GameInstancePlayer* &pPlayer) override;

		// Register new player to join
		virtual Result AddPlayerToJoin(Svr::GameInstancePlayer* &pPlayer ) override;


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
