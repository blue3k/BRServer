////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2019 The Braves
// 
// Author : KyungKun Ko
//
// Description : Relay Game Instance
//	
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "SFTypedefs.h"

#include "Util/SFTimeUtil.h"
#include "Math/SFMathUtil.h"
#include "Container/SFHashTable.h"
#include "MemoryManager/SFMemoryPool.h"
#include "Entity/Entity.h"
#include "Transaction/MessageRoute.h"
#include "GameConst.h"
#include "Types/BrGameTypes.h"
#include "Util/GlobalUIDGenerator.h"
#include "ServiceEntity/Relay/RelayPlayer.h"
#include "Container/SFStaticDoubleLinkedList.h"
#include "Protocol/PlayInstanceMsgClass.h"
#include "Protocol/PlayInstanceNetPolicy.h"


namespace SF {
	namespace Message
	{
		namespace Relay
		{
			class RelayPacketC2SEvt;
		}
	}

	namespace Net {
		class RawUDP;
	}
}


namespace SF {

	class RelayPlayer;

	//////////////////////////////////////////////////////////////////////////
	//
	//	Relay GameInstance class
	//

	class RelayInstance
	{
	public:

		static const int32_t RELAY_INSTANCE_TIMEOUT;


		enum class GameInstanceState : uint32_t
		{
			None,		// Just created
			Active,		// Active
			Closing,	// Closing
		};

	public:

		RelayInstance(IHeap& heap, Net::RawUDP* pNet, uint32_t instanceID);
		virtual ~RelayInstance();

		// clear
		Result Clear();

		IHeap& GetHeap() { return *m_Heap.get(); }

		GameID GetGameID() const { return m_GameID; }
		uint32_t GetInstanceID() const { return m_InstanceID; }

		const Util::TimeStampTimer& GetTimeToKill() const { return m_TimeToKill; }

		GameInstanceState GetGameInstanceState() const { return m_GameInstanceState; }
		void SetGameInstanceState(GameInstanceState newState);

		// Initialize entity to proceed new connection
		virtual Result InitializeGameInstance(GameID gameId, uint32_t maxUser);

		void Heartbeat();

		// Add player and assign relay player ID
		Result AddPlayer(const sockaddr_storage& remoteAddr, PlayerID playerID, const String playerIdentification, RelayPlayerID& relayPlayerID);
		Result RemovePlayer(PlayerID playerID);

		// Handle relay packet
		void OnRelayPacket(const sockaddr_storage& remoteAddr, const Message::PlayInstance::PlayPacketC2SEvt& message);

		template<typename Func>
		void ForeachPlayer(const Func& func)
		{
			for (uint32_t iUser = 0; iUser < m_Users.size(); iUser++)
			{
				func(iUser, m_Users[iUser]);
			}
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	
		//

		// register message handlers
		virtual Result RegisterMessageHandlers();

		// Run the task
		virtual Result TickUpdate();


		//////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Update
		//

		// Update Game Player 
		Result UpdateRelayInstances(TimeStampMS CurTime );


	private:


		// Heap
		Heap m_Heap;

		Net::RawUDP* m_pNet = nullptr;

		// Game ID
		GameID m_GameID = nullptr;

		uint32_t m_InstanceID = 0;

		// int32
		uint32_t m_MaxUser = 32;

		// Time for kill this entity
		Util::TimeStampTimer m_TimeToKill;

		// GameInstance State
		GameInstanceState m_GameInstanceState = GameInstanceState::None;

		StaticArray<RelayPlayer, 32> m_Users;

	};



}; // namespace SF

