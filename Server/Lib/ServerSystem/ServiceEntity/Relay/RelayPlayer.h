////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2019 The Braves
// 
// Author : KyungKun Ko
//
// Description : Relay player
//	
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "SFTypedefs.h"

#include "Util/SFTimeUtil.h"
#include "Math/SFMathUtil.h"
#include "Container/SFHashTable.h"
#include "Memory/SFMemoryPool.h"
#include "GameConst.h"
#include "Types/BrGameTypes.h"




namespace SF {
namespace Net {
	class Connection;
};

namespace Policy {
	class ISvrPolicyRelay;
};
};



namespace SF {


	class RelayInstance;


	class RelayPlayerID
	{
	public:

		RelayPlayerID() = default;
		RelayPlayerID(uint32_t relayPlayerID) { SetRelayPlayerID(relayPlayerID); }

		void SetRelayPlayerID(uint32_t relayPlayerID) { m_RelayPlayerIndex = Math::FindLSBIndex(relayPlayerID); }
		uint32_t GetRelayPlayerID() const { return m_RelayPlayerIndex < 0 ? 0 : uint32_t(1) << m_RelayPlayerIndex; }

		void SetPlayerIndex(uint32_t userIndex) { m_RelayPlayerIndex = userIndex; }
		uint32_t GetPlayerIndex() const { return m_RelayPlayerIndex; };

		operator uint32_t() const { return GetRelayPlayerID(); }

	private:

		uint32_t m_RelayPlayerIndex = -1;
	};



	//////////////////////////////////////////////////////////////////////////
	//
	//	Relay Player class
	//

	class RelayPlayer
	{
	public:

		enum class PlayerState : uint32_t
		{
			None,		// Just created
			Joining,	// Joining
			Active,		// Active
			Closing,	// Closing
		};

	private:

		// Relay instance
		RelayInstance* m_RelayInstance = nullptr;

		// Socket address
		sockaddr_storage m_Address{};

		// player Id
		PlayerID m_PlayerID = 0;

		// Relay player id. unique in relay instance
		RelayPlayerID m_RelayPlayerID;

		// String player Identifier. 
		String m_PlayerIdentifier;

		// Player display name
		char m_PlayerDisplayName[GameConst::MAX_NAME]{};

		// Time for kill this entity
		Util::TimeStampTimer m_TimeToKill;

		// Player State
		PlayerState m_PlayerState = PlayerState::None;

	protected:


	public:

		RelayPlayer() = default;
		~RelayPlayer();

		// clear 
		void Clear();

		const sockaddr_storage& GetSocketAddress() const { return m_Address; }

		PlayerID GetPlayerID() const { return m_PlayerID; }
		const String& GetPlayerIdentifier() const { return m_PlayerIdentifier; }

		RelayPlayerID GetRelayPlayerID() const { return m_RelayPlayerID; }

		const Util::TimeStampTimer& GetTimeToKill() { return m_TimeToKill; }

		PlayerState GetPlayerState() const { return m_PlayerState; }
		void SetPlayerState(PlayerState newState);

		// Initialize entity to proceed new connection
		virtual Result InitializePlayer(IHeap& heap, RelayInstance* relayInstance, const sockaddr_storage& remoteAddr, const RelayPlayerID& relayPlayerID, PlayerID playerID, const String& playerIdentifier, const char* PlayerDisplayName);

		const char* GetPlayerDisplayName() { return m_PlayerDisplayName; }
		Result SetPlayerDisplayName(const char* playerName);



		//////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	
		//

		void Heartbeat();

		// Run the task
		Result TickUpdate();


		//////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Update
		//

		// Update timeout
		Result UpdateTimeout(TimeStampMS CurTime );

	};



}; // namespace SF

