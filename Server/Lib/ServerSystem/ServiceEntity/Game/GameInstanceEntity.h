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
#include "Container/Indexing.h"
#include "Container/HashTable.h"
#include "Component/BrComponent.h"

#include "Transaction/MessageRoute.h"
#include "Entity/Entity.h"
#include "Server/BrServer.h"
#include "ServiceEntity/Game/GameSystem.h"
#include "ServerSystem/GameLog/ChattingHistory.h"
//#include "GameSystem/GameSystemComponentIDs.h"
//
//#include "ConspiracyGameInstanceSvrConst.h"
#include "ServiceEntity/Game/GameInstancePlayer.h"

#include "Protocol/Policy/GameServerNetPolicy.h"
#include "Protocol/Policy/GameInstanceNetPolicy.h"

//#include "Table/conspiracy/GameConfigTbl.h"
//#include "Table/conspiracy/BotTalkTbl.h"


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
namespace Svr
{
	class GameInstancePlayer;


	//////////////////////////////////////////////////////////////////////////
	//
	//	Game Instance entity class
	//

	class GameInstanceEntity : public MasterEntity
	{
	public:

		//typedef ::conspiracy::GameConfigTbl::GameConfigItem GameConfigType;

		typedef SortedMap<PlayerID,GameInstancePlayer*>
				GamePlayerUIDMap;


	protected:
		typedef Svr::MasterEntity super;

		// Player by PlayerID
		GamePlayerUIDMap		m_GamePlayerByUID;

		// Release array
		PageQueue<PlayerID>		m_PendingReleasePlayer;


		//////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Game info
		//


		// Time for kill this game
		Util::TimeStampTimer m_TimeToKill;

		// Is accept join?
		bool m_AcceptJoin = false;

		// Team Leader UID
		PlayerID m_LeaderUID;

		// Max player
		uint m_MaxPlayer;

		// Total joined player since game instance is created
		uint m_TotalJoinedPlayer;

		// Number of bot
		uint m_NumBot;

		DurationMS m_EmptyInstanceKillTimeOut;


	public:
		//////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Game Instance
		//

		GameInstanceEntity();
		~GameInstanceEntity();

		const Util::TimeStampTimer& GetTimeToKill() { return m_TimeToKill; }
		bool GetAcceptJoin() { return m_AcceptJoin; }

		PlayerID GetLeaderUID() { return m_LeaderUID; }
		void SetLeaderUID(PlayerID value) { m_LeaderUID = value; }

		uint GetMaxPlayer() { return m_MaxPlayer; }
		uint GetTotalJoinedPlayer() { return m_TotalJoinedPlayer; }
		uint GetNumBot() { return m_NumBot; }

		DurationMS GetEmptyInstanceKillTimeOut() { return m_EmptyInstanceKillTimeOut; }
		void SetEmptyInstanceKillTimeOut(DurationMS value) { m_EmptyInstanceKillTimeOut = value; }

		// Instance ID query
		inline GameInsID GetInstanceID();
		inline GameInsUID GetInstanceUID();

		// Get player count at this game
		inline uint GetNumPlayer();

		//conspiracy::BotTalkTbl::BotTalkTblItem *GetBotTalkTbl() { return m_pBotTalk; }


		//////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Game Systems
		//

		//// Initialize game system
		//Result InitializeSystem();

		//// Update game config
		//Result UpdateGameConfig(uint configPresetID);


	protected:
		// Initialize entity to proceed new connection
		virtual Result InitializeEntity( EntityID newEntityID ) override;

	public:
		// Close entity and clear transaction
		virtual Result TerminateEntity() override;

		// Run entity
		virtual Result TickUpdate(TimerAction *pAction = nullptr) override;



		////////////////////////////////////////////////////////////////////////////////////////////
		////
		////	foreach implementations
		////

		//// foreach game player
		//template< class Func >
		//Result ForeachPlayer(Func func);
		////Result ForeachPlayer( std::function<Result(GamePlayer* pPlayer)> func );

		//Result ForeachPlayerGameServer( std::function<Result(GamePlayer* pPlayer, Policy::NetPolicyGameServer *pPolicy)> func );

		//Result ForeachPlayerSvrGameInstance( std::function<Result(GamePlayer* pPlayer, Policy::NetSvrPolicyGameInstance *pPolicy)> func );



		////////////////////////////////////////////////////////////
		//
		//	Game entity
		//

		// Initialize entity to proceed new connection
		virtual Result InitializeGameEntity(uint numBot, uint maxPlayer);


		////////////////////////////////////////////////////////////
		//
		//	Update processes
		//


		// Update Game status
		virtual Result UpdateGameStatus(TimeStampMS ulCurTime );



		////////////////////////////////////////////////////////////
		//
		//	Game timers
		//


		// set game instance kill timer
		Result SetGameKillTimer( DurationMS ulWaitTime );


		// On Game Kill timer
		void OnGameKillTimer();


		// Close Game Instance
		void CloseGameInstance();


		//////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Game Player
		//

		virtual Result CreatePlayerInstance(const PlayerInformation& playerInfo, GameInstancePlayer* &pPlayer);

		//Result GetPlayerIndex( PlayerID playerID, uint &playerIndex );
		//Result GetPlayerByIndex( INT playerIndex, GamePlayer* &pGamePlayer );

		// Register new player to join
		virtual Result AddPlayerToJoin( GameInstancePlayer* &pPlayer );

		// Player leave
		virtual Result LeavePlayer(GameInstancePlayer* &pPlayer );

		virtual Result LeavePlayer( PlayerID pltID );

		// Leave all player
		Result LeaveAllPlayerForGameDelete();

		// Find Player pilotid
		Result FindPlayer( PlayerID pltID, GameInstancePlayer* &pGamePlayer );

		// Called when a player get out of game
		virtual Result OnPlayerGetOutOfGame(GameInstancePlayer *pPlayer );
	};

#include "GameInstanceEntity.inl"

}; // namespace Svr
}; // namespace SF
