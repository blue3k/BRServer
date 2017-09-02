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
#include "Types/BrBaseTypes.h"
#include "Container/Indexing.h"
#include "Common/HashTable.h"
#include "Component/BrComponent.h"

#include "ServerSystem/MessageRoute.h"
#include "ServerSystem/Entity.h"
#include "ServerSystem/BrServer.h"
#include "ServerSystem/GameSystem.h"
#include "ServerSystem/GameLog/ChattingHistory.h"
//#include "GameSystem/GameSystemComponentIDs.h"
//
//#include "ConspiracyGameInstanceSvrConst.h"
#include "ServerSystem/ServiceEntity/Game/GameInstancePlayer.h"

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
		BRCLASS_ATTRIBUTE_CONST(Util::TimeStampTimer,TimeToKill);

		// Is accept join?
		BRCLASS_ATTRIBUTE_READONLY(bool,AcceptJoin);

		// Team Leader UID
		BRCLASS_ATTRIBUTE(PlayerID,LeaderUID);

		// Max player
		BRCLASS_ATTRIBUTE_READONLY(uint,MaxPlayer);

		// Total joined player since game instance is created
		BRCLASS_ATTRIBUTE(uint, TotalJoinedPlayer);

		// Number of bot
		BRCLASS_ATTRIBUTE(uint, NumBot);

		BRCLASS_ATTRIBUTE(DurationMS, EmptyInstanceKillTimeOut);


		//////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Game state delegate
		//

	
		// Allocator for member
		StaticAllocator< sizeof(Svr::EntityMessageHandlerItem)*100 > m_Allocator;

	protected:

		virtual MemoryAllocator& GetAllocator()			{ return m_Allocator; }

	public:

		//////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Game Instance
		//

		GameInstanceEntity();
		~GameInstanceEntity();

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

		//Result ForeachPlayerGameServer( std::function<Result(GamePlayer* pPlayer, Policy::IPolicyGameServer *pPolicy)> func );

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
