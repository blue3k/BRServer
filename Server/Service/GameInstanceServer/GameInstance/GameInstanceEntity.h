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

#include "Common/TypeDefs.h"
#include "Common/ClassUtil.h"
#include "Common/BRBaseTypes.h"

#include "ServerSystem/MessageRoute.h"
#include "ServerSystem/Entity.h"
#include "ServerSystem/BRServer.h"
#include "ServerSystem/GameSystem.h"
#include "ServerSystem/Component.h"
#include "ServerSystem/GameLog/ChattingHistory.h"

#include "ConspiracyGameInstanceSvrConst.h"
#include "Common/Indexing.h"
#include "Common/HashTable.h"
#include "GameSystem/GameSystemComponentIDs.h"
#include "GamePlayer.h"

#include "table/conspiracy\GameConfigTbl.h"


namespace BR{ 
namespace Net {
	class Connection;
}}

namespace BR
{
	namespace Policy
	{
		//interface ISvrPolicyGame;
		class ISvrPolicyGameInstance;
		class IPolicyGameMasterServer;
	};

};

namespace BR {
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

	class GameInstanceEntity : public Svr::MasterEntity, public MemoryPoolObject<GameInstanceEntity>, public Svr::ComponentCarrier<GameSystemComponentID_Max>
	{
	public:

		typedef ::conspiracy::GameConfigTbl::GameConfigItem GameConfigType;

		typedef SortedMap<PlayerID,GamePlayer*>
				GamePlayerUIDMap;


	private:
		typedef Svr::MasterEntity super;

		// Player by PlayerID
		GamePlayerUIDMap		m_GamePlayerByUID;

		// Player by index
		GamePlayer*				m_PlayerByIndex[GameConst::MAX_GAMEPLAYER];

		// Player character table
		BYTE					m_PlayerCharacter[GameConst::MAX_GAMEPLAYER];

		// Release array
		PageQueue<PlayerID>		m_PendingReleasePlayer;

		// Game configuration
		UINT					m_PresetGameConfigID;
		BRCLASS_ATTRIBUTE_READONLY_PTR(GameConfigType*,PresetGameConfig);

		//////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Game info
		//


		// Time for kill this game
		BRCLASS_ATTRIBUTE_CONST(Util::TimeStampTimer,TimeToKill);

		// Is accept join?
		BRCLASS_ATTRIBUTE_READONLY(bool,AcceptJoin);

		// Team Leader UID
		BRCLASS_ATTRIBUTE(PlayerID,LeaderUID);

		// Max player
		BRCLASS_ATTRIBUTE_READONLY(UINT,MaxPlayer);

		// Total joined player since game instance is created
		BRCLASS_ATTRIBUTE(UINT, TotalJoinedPlayer);

		// Number of bot
		BRCLASS_ATTRIBUTE(UINT, NumBot);

		BRCLASS_ATTRIBUTE(UINT, RoleRequestSeer);
		BRCLASS_ATTRIBUTE(UINT, RoleRequestWerewolf);

		//////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Game state delegate
		//

	
		// Allocator for member
		StaticAllocator< sizeof(Svr::EntityMessageHandlerItem)*30 > m_Allocator;

	protected:

		virtual MemoryAllocator& GetAllocator()			{ return m_Allocator; }

	public:

		//////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Game Instance
		//

		GameInstanceEntity();
		~GameInstanceEntity();

		// Get server instance
		GameInstanceServer* GetMyOwner();

		// Instance ID query
		inline GameInsID GetInstanceID();
		inline GameInsUID GetInstanceUID();

		// Get player count at this game
		inline UINT GetNumPlayer();


		//////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Game Systems
		//

		// Initialize game system
		HRESULT InitializeSystem();

		// Update game config
		HRESULT UpdateGameConfig(UINT configPresetID);


	private:
		// Initialize entity to proceed new connection
		virtual HRESULT InitializeEntity( EntityID newEntityID ) override;

	public:
		// Close entity and clear transaction
		virtual HRESULT TerminateEntity() override;

		// Run entity
		virtual HRESULT TickUpdate(Svr::TimerAction *pAction = nullptr) override;



		//////////////////////////////////////////////////////////////////////////////////////////
		//
		//	foreach implementations
		//

		// foreach game player
		HRESULT ForeachPlayer( std::function<HRESULT(GamePlayer* pPlayer)> func );

		HRESULT ForeachPlayerGameServer( std::function<HRESULT(GamePlayer* pPlayer, Policy::IPolicyGameServer *pPolicy)> func );

		HRESULT ForeachPlayerSvrGameInstance( std::function<HRESULT(GamePlayer* pPlayer, Policy::ISvrPolicyGameInstance *pPolicy)> func );

		// foreach game player with world master policy
		HRESULT ForeachPlayerGameMaster( std::function<HRESULT(GamePlayer* pPlayer, Policy::IPolicyGameMasterServer *pPolicy)> func );


		////////////////////////////////////////////////////////////
		//
		//	Game entity
		//

		// Initialize entity to proceed new connection
		HRESULT InitializeGameEntity( UINT numBot, UINT maxPlayer );



		////////////////////////////////////////////////////////////
		//
		//	Update processes
		//


		// Update Game status
		HRESULT UpdateGameStatus(TimeStampMS ulCurTime );



		////////////////////////////////////////////////////////////
		//
		//	Game timers
		//


		// set game instance kill timer
		HRESULT SetGameKillTimer( DurationMS ulWaitTime );



		// On Game Kill timer
		void OnGameKillTimer();


		// Close Game Instance
		void CloseGameInstance();


		//////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Game Player
		//

		HRESULT GetPlayerIndex( PlayerID playerID, UINT &playerIndex );
		HRESULT GetPlayerByIndex( INT playerIndex, GamePlayer* &pGamePlayer );

		// Register new player to join
		HRESULT AddPlayerToJoin( GamePlayer* &pPlayer );

		// Player leave
		HRESULT LeavePlayer( GamePlayer* &pPlayer );

		//HRESULT LeavePlayer( Net::Connection *pCon );
		HRESULT LeavePlayer( PlayerID pltID );

		// Leave all player
		HRESULT LeaveAllPlayerForGameDelete();

		// Find Player pilotid
		HRESULT FindPlayer( PlayerID pltID, GamePlayer* &pGamePlayer );

		// Called when a player get out of game
		HRESULT OnPlayerGetOutOfGame( GamePlayer *pPlayer );
	};

#include "GameInstanceEntity.inl"

}; // namespace ConspiracyGameInstanceServer
}; // namespace BR
