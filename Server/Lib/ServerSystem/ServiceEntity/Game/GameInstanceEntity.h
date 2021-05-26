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
#include "ServiceEntity/ServiceEntity.h"
#include "Server/BrServer.h"

#include "ServerSystem/GameLog/ChattingHistory.h"

#include "ServiceEntity/Game/GameSystem.h"
#include "ServiceEntity/Game/GameInstancePlayer.h"

#include "Protocol/GameServerNetPolicy.h"
#include "Protocol/GameInstanceNetPolicy.h"
#include "Protocol/PlayInstanceNetPolicy.h"



namespace SF {
namespace Svr
{
	class GameInstancePlayer;
	class GameInstanceMapObject;

	//////////////////////////////////////////////////////////////////////////
	//
	//	Game Instance entity class
	//

	class GameInstanceEntity : public MasterEntity
	{
	public:

		using GamePlayerByPlayerIDMap = SortedMap<PlayerID,GameInstancePlayer*>;
		using super = MasterEntity;

	protected:


		NetAddress m_AddressIPV4;
		NetAddress m_AddressIPV6;

		CriticalSection m_UpdateLock;

		// Player by PlayerID
		GamePlayerByPlayerIDMap		m_GamePlayerByPlayerID;

		// Release array
		PageQueue<PlayerID>		m_PendingReleasePlayer;
		PageQueue<PlayerID>		m_PendingJoinedPlayer;

		// Instance type
		StringCrc32 m_InstanceType;

		// data id
		uint32_t m_ZoneTableID{};

		// map objects
		SortedMap<StringCrc32, GameInstanceMapObject*> m_MapObjects;

		//////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Game info
		//

		// Time for kill this game
		Util::TimeStampTimer m_TimeToKill;


		// Is accept join?
		bool m_AcceptJoin = false;
		bool m_InstanceClosed = false;

		// Max player
		uint m_MaxPlayer;

		// Total joined player since game instance is created
		uint m_TotalJoinedPlayer;

		// 
		DurationMS m_EmptyInstanceKillTimeOut;

		// Component manager
		ComponentManager m_ComponentManger;

		// movement frame
		TimeStampMS m_LatestTickTime;
		uint32_t m_MovementFrame = 0;

		// Player start movement
		mutable ActorMovement m_StartMove{};

	public:
		//////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Game Instance
		//

		GameInstanceEntity();
		~GameInstanceEntity();


		const NetAddress& GetAddressIPV4() const { return m_AddressIPV4; }
		const NetAddress& GetAddressIPV6() const { return m_AddressIPV6; }

		StringCrc32 GetInstanceType() const { return m_InstanceType; }
		uint32_t GetZoneTableID() const { return m_ZoneTableID; }


		const Util::TimeStampTimer& GetTimeToKill() { return m_TimeToKill; }
		bool GetAcceptJoin() { return m_AcceptJoin; }

		uint GetMaxPlayer() { return m_MaxPlayer; }
		uint GetTotalJoinedPlayer() { return m_TotalJoinedPlayer; }

		DurationMS GetEmptyInstanceKillTimeOut() { return m_EmptyInstanceKillTimeOut; }
		void SetEmptyInstanceKillTimeOut(DurationMS value) { m_EmptyInstanceKillTimeOut = value; }

		virtual void InitStartMovement();
		uint32_t GetMovementFrame() const { return m_MovementFrame; }

		// Instance ID query
		inline GameInsID GetInstanceID() {
			return GameInsID(GetEntityID());
		}

		inline GameInsUID GetInstanceUID() {
			return GameInsUID(Service::ServerConfig->UID, GetEntityID());
		}

		// Get player count at this game
		inline uint GetNumPlayer() {
			return static_cast<uint>(m_GamePlayerByPlayerID.size());
		}

		// map object
		Result GetMapObject(StringCrc32 mapObjectId, GameInstanceMapObject*& pMapObject);

	protected:
		// Initialize entity to proceed new connection
		virtual Result InitializeEntity( EntityID newEntityID ) override;

		// create player instance
		virtual GameInstancePlayer* CreatePlayer(EntityUID playerEntityUID, const PlayerInformation& player);
		virtual Svr::GameInstanceMapObject* CreateMapObject(StringCrc32 mapObjectId);

	public:
		// Close entity and clear transaction
		virtual Result TerminateEntity() override;

		// Run entity
		virtual Result TickUpdate(TimerAction *pAction = nullptr) override;

		uint32_t UpdateMovementTick(TimeStampMS newUpdateTimeStamp);

		void UpdateReleasedPlayers();
		void UpdateJoinedPlayers();


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
		virtual Result InitializeGameEntity(const ServerConfig::NetPublic& netPublic, const VariableTable& attributes);


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

		// called by instance manager when the player is connected
		Result PlayerConnected(PlayerID playerId, const SharedPointerT<Net::Connection>& connection, ActorMovement& outMovement);

		// Register new player to join
		virtual Result AddPlayerToJoin(EntityUID playerEntityUID, const PlayerInformation& playerInfo, const VariableTable& characterVisual, const VariableTable& characterAttribute);

		// Player leave
		virtual Result LeavePlayer(GameInstancePlayer* pPlayer);

		virtual Result PendingLeavePlayer(PlayerID pltID);

		// Leave all player
		Result LeaveAllPlayerForGameDelete();

		// Find Player with player id
		Result FindPlayer(PlayerID pltID, GameInstancePlayer* &pGamePlayer);

		// Called when a player get out of game
		virtual Result OnPlayerGetOutOfGame(GameInstancePlayer *pPlayer);

		Result NewPlayerMovement(PlayerID playerId, const ActorMovement& newMovement);

		// Broadcast raw message data
		Result Broadcast(PlayerID fromPlayerId, const MessageDataPtr& messageData);
	};


} // namespace Svr
} // namespace SF
