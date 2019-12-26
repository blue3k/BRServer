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
#include "Container/SFHashTable.h"
#include "Memory/SFMemoryPool.h"
#include "Entity/Entity.h"
#include "Transaction/MessageRoute.h"
#include "GameConst.h"
#include "Types/BrGameTypes.h"
#include "Util/GlobalUIDGenerator.h"




namespace SF {
namespace Net {
	class Connection;
};

namespace Policy {
	class ISvrPolicyRelay;
};
};



namespace SF {
namespace Svr {

	class RelayPlayer;

	//////////////////////////////////////////////////////////////////////////
	//
	//	Relay GameInstance class
	//

	class RelayGameInstance
	{
	public:

		enum class GameInstanceState : uint32_t
		{
			None,		// Just created
			Active,		// Active
			Closing,	// Closing
		};

	private:

		// Game ID
		GameID m_GameID = nullptr;

		// int32
		uint32_t m_MaxUser = 32;

		// Time for kill this entity
		Util::TimeStampTimer m_TimeToKill;

		// GameInstance State
		GameInstanceState m_GameInstanceState = GameInstanceState::None;

		StaticArray<RelayPlayer*, 16> m_Users;

	public:

		RelayGameInstance(IHeap& heap);
		virtual ~RelayGameInstance();

		// clear
		Result Clear();

		GameID GetGameID() { return m_GameID; }

		const Util::TimeStampTimer& GetTimeToKill() { return m_TimeToKill; }

		GameInstanceState GetGameInstanceState() const { return m_GameInstanceState; }
		void SetGameInstanceState(GameInstanceState newState);

		// Initialize entity to proceed new connection
		virtual Result InitializeGameInstance(GameID gameId, uint32_t maxUser);

		void HeartBit();




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
		Result UpdateRelayGameInstances(TimeStampMS CurTime );

	};



}; // namespace Svr
}; // namespace SF

