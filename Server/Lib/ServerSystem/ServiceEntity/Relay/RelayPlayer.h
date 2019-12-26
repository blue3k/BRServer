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
namespace Svr {


	class RelayInstance;

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

		// Account Id
		AccountID m_AccountID = 0;

		// String Account Identifier. 
		String m_AccountIdentifier;

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

		AccountID GetAccountID() const { return m_AccountID; }
		const String& GetAccountIdentifier() const { return m_AccountIdentifier; }

		const Util::TimeStampTimer& GetTimeToKill() { return m_TimeToKill; }

		PlayerState GetPlayerState() const { return m_PlayerState; }
		void SetPlayerState(PlayerState newState);

		// Initialize entity to proceed new connection
		virtual Result InitializePlayer(IHeap& heap, RelayInstance* relayInstance, AccountID accountId, const String& accountIdentifier, const char* PlayerDisplayName);

		const char* GetPlayerDisplayName() { return m_PlayerDisplayName; }
		void SetPlayerDisplayName(const char* PlayerName);



		//////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	
		//

		void HeartBit();

		// Run the task
		Result TickUpdate();


		//////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Update
		//

		// Update timeout
		Result UpdateTimeout(TimeStampMS CurTime );

	};



}; // namespace Svr
}; // namespace SF

