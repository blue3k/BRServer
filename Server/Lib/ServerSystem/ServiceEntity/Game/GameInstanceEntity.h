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
#include "Component/BrComponent.h"

#include "Transaction/MessageRoute.h"
#include "Entity/Entity.h"
#include "ServiceEntity/ServiceEntity.h"
#include "Server/BrServer.h"

#include "ServerSystem/GameLog/ChattingHistory.h"

#include "ServiceEntity/Game/GameSystem.h"
#include "ServiceEntity/Game/GameInstancePlayer.h"

#include "Protocol/Policy/GameServerNetPolicy.h"
#include "Protocol/Policy/GameInstanceNetPolicy.h"



namespace SF {
namespace Svr
{
	class GameInstancePlayer;


	//////////////////////////////////////////////////////////////////////////
	//
	//	Game Instance entity class
	//

	class GameInstanceEntity : public ServiceEntity
	{
	public:

		using GamePlayerUIDMap = SortedMap<PlayerID,GameInstancePlayer*>;
		using super = ServiceEntity;

	protected:

		// Player by PlayerID
		GamePlayerUIDMap		m_GamePlayerByUID;

		// Release array
		PageQueue<PlayerID>		m_PendingReleasePlayer;

		// Instance type
		StringCrc32 m_InstanceType;

		// data id
		uint32_t m_ZoneTableID{};


		//////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Game info
		//


		// Time for kill this game
		Util::TimeStampTimer m_TimeToKill;

		// Is accept join?
		bool m_AcceptJoin = false;


		// Max player
		uint m_MaxPlayer;

		// Total joined player since game instance is created
		uint m_TotalJoinedPlayer;

		// 
		DurationMS m_EmptyInstanceKillTimeOut;

		// Component manager
		ComponentManager m_ComponentManger;



	public:
		//////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Game Instance
		//

		GameInstanceEntity();
		~GameInstanceEntity();


		StringCrc32 GetInstanceType() const { return m_InstanceType; }
		uint32_t GetZoneTableID() const { return m_ZoneTableID; }


		const Util::TimeStampTimer& GetTimeToKill() { return m_TimeToKill; }
		bool GetAcceptJoin() { return m_AcceptJoin; }

		uint GetMaxPlayer() { return m_MaxPlayer; }
		uint GetTotalJoinedPlayer() { return m_TotalJoinedPlayer; }

		DurationMS GetEmptyInstanceKillTimeOut() { return m_EmptyInstanceKillTimeOut; }
		void SetEmptyInstanceKillTimeOut(DurationMS value) { m_EmptyInstanceKillTimeOut = value; }

		// Instance ID query
		inline GameInsID GetInstanceID();
		inline GameInsUID GetInstanceUID();

		// Get player count at this game
		inline uint GetNumPlayer();


	protected:
		// Initialize entity to proceed new connection
		virtual Result InitializeEntity( EntityID newEntityID ) override;

	public:
		// Close entity and clear transaction
		virtual Result TerminateEntity() override;

		// Run entity
		virtual Result TickUpdate(TimerAction *pAction = nullptr) override;




		//////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Component manager
		//

		ComponentManager& GetComponentManager() { return m_ComponentManger; }

		template< class ComponentType >
		ComponentType* GetComponent() { return m_ComponentManger.GetComponent<ComponentType>(); }

		// Get component with its type
		template< class ComponentType >
		const ComponentType* GetComponent() const { return m_ComponentManger.GetComponent<ComponentType>(); }



		////////////////////////////////////////////////////////////
		//
		//	Game entity
		//

		// Initialize entity to proceed new connection
		virtual Result InitializeGameEntity(const VariableTable& attributes);


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

		virtual Result CreatePlayerInstance(const PlayerInformation& playerInfo, SFUniquePtr<Svr::GameInstancePlayer> &pPlayer);


		// Register new player to join
		virtual Result AddPlayerToJoin(SFUniquePtr<Svr::GameInstancePlayer> &pPlayer );

		// Player leave
		virtual Result LeavePlayer(GameInstancePlayer* &pPlayer );

		virtual Result LeavePlayer( PlayerID pltID );

		// Leave all player
		Result LeaveAllPlayerForGameDelete();

		// Find Player with player id
		Result FindPlayer( PlayerID pltID, GameInstancePlayer* &pGamePlayer );

		// Called when a player get out of game
		virtual Result OnPlayerGetOutOfGame(GameInstancePlayer *pPlayer );
	};

#include "GameInstanceEntity.inl"

}; // namespace Svr
}; // namespace SF
