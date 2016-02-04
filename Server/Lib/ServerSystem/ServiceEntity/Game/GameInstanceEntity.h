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

#include "Common/Typedefs.h"
#include "Common/ClassUtil.h"
#include "Common/BrBaseTypes.h"
#include "Common/Indexing.h"
#include "Common/HashTable.h"
#include "Common/BrComponent.h"

#include "ServerSystem/MessageRoute.h"
#include "ServerSystem/Entity.h"
#include "ServerSystem/BrServer.h"
#include "ServerSystem/GameSystem.h"
#include "ServerSystem/GameLog/ChattingHistory.h"
//#include "GameSystem/GameSystemComponentIDs.h"
//
//#include "ConspiracyGameInstanceSvrConst.h"
#include "ServerSystem/ServiceEntity/Game/GameInstancePlayer.h"

#include "Protocol/Policy/GameServerIPolicy.h"
#include "Protocol/Policy/GameInstanceIPolicy.h"

//#include "Table/conspiracy/GameConfigTbl.h"
//#include "Table/conspiracy/BotTalkTbl.h"


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
		BRCLASS_ATTRIBUTE_READONLY(UINT,MaxPlayer);

		// Total joined player since game instance is created
		BRCLASS_ATTRIBUTE(UINT, TotalJoinedPlayer);

		// Number of bot
		BRCLASS_ATTRIBUTE(UINT, NumBot);

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
		inline UINT GetNumPlayer();

		//conspiracy::BotTalkTbl::BotTalkTblItem *GetBotTalkTbl() { return m_pBotTalk; }


		//////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Game Systems
		//

		//// Initialize game system
		//HRESULT InitializeSystem();

		//// Update game config
		//HRESULT UpdateGameConfig(UINT configPresetID);


	protected:
		// Initialize entity to proceed new connection
		virtual HRESULT InitializeEntity( EntityID newEntityID ) override;

	public:
		// Close entity and clear transaction
		virtual HRESULT TerminateEntity() override;

		// Run entity
		virtual HRESULT TickUpdate(Svr::TimerAction *pAction = nullptr) override;



		////////////////////////////////////////////////////////////////////////////////////////////
		////
		////	foreach implementations
		////

		//// foreach game player
		//template< class Func >
		//HRESULT ForeachPlayer(Func func);
		////HRESULT ForeachPlayer( std::function<HRESULT(GamePlayer* pPlayer)> func );

		//HRESULT ForeachPlayerGameServer( std::function<HRESULT(GamePlayer* pPlayer, Policy::IPolicyGameServer *pPolicy)> func );

		//HRESULT ForeachPlayerSvrGameInstance( std::function<HRESULT(GamePlayer* pPlayer, Policy::ISvrPolicyGameInstance *pPolicy)> func );



		////////////////////////////////////////////////////////////
		//
		//	Game entity
		//

		// Initialize entity to proceed new connection
		virtual HRESULT InitializeGameEntity( UINT numBot, UINT maxPlayer );



		////////////////////////////////////////////////////////////
		//
		//	Update processes
		//


		// Update Game status
		virtual HRESULT UpdateGameStatus(TimeStampMS ulCurTime );



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

		virtual HRESULT CreatePlayerInstance(const PlayerInformation& playerInfo, GameInstancePlayer* &pPlayer);

		//HRESULT GetPlayerIndex( PlayerID playerID, UINT &playerIndex );
		//HRESULT GetPlayerByIndex( INT playerIndex, GamePlayer* &pGamePlayer );

		// Register new player to join
		virtual HRESULT AddPlayerToJoin( GameInstancePlayer* &pPlayer );

		// Player leave
		virtual HRESULT LeavePlayer(GameInstancePlayer* &pPlayer );

		virtual HRESULT LeavePlayer( PlayerID pltID );

		// Leave all player
		HRESULT LeaveAllPlayerForGameDelete();

		// Find Player pilotid
		HRESULT FindPlayer( PlayerID pltID, GameInstancePlayer* &pGamePlayer );

		// Called when a player get out of game
		virtual HRESULT OnPlayerGetOutOfGame(GameInstancePlayer *pPlayer );
	};

#include "GameInstanceEntity.inl"

}; // namespace Svr
}; // namespace BR
