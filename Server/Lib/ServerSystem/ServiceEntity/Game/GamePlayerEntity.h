////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 Blue3k
// 
// Author : KyungKun Ko
//
// Description : Game player Entity
//	
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "SFTypedefs.h"

#include "Util/TimeUtil.h"
#include "Container/HashTable.h"
#include "Memory/MemoryPool.h"
#include "GameConst.h"

#include "Types/BrGameTypes.h"
#include "Entity/Entity.h"
#include "ServiceEntity/Game/GameSystem.h"
#include "Entity/SimpleUserEntity.h"



namespace SF {
namespace Net {
	class Connection;
};

namespace Policy {
	class ISvrPolicyGame;
};
};



namespace SF {
namespace Svr {


	class GameServer;
	class InvenSystem;
	class GameInstanceEntity;
	class GameMasterServerEntity;

	// GCM predefined messages
	#define BRPUSHMSG_DISCONNECTED		"BRDisconnected"
	#define BRPUSHMSG_SYNC				"BRSync"


	//////////////////////////////////////////////////////////////////////////
	//
	//	Game Instance entity class
	//

	class GamePlayerEntity : public SimpleUserEntity, public MemoryPoolObject<GamePlayerEntity>
	{
	public:

		// Player state
		enum PlayerState
		{
			PlayerState_None,		// Nothing just created
			PlayerState_LogedIn,	// Just Loged in
		};

	private:

		// Player connection
		//Policy::NetSvrPolicyGame* m_ISvrGamePolicy;

		// Player state
		PlayerState m_PlayerState;

		// Game instance UID
		GameInsUID m_GameInsUID;

		mutable PlayerInformation m_PlayerInformation;

		uint m_ShardID;

		DurationMS m_PlayerAutoLogout;

		// Time for kill this game
		Util::TimeStampTimer m_KillTimer;

		// Latest update time in UTC time
		TimeStampSec m_LatestUpdateTime;

		// Latest active time in UTC time
		TimeStampSec m_LatestActiveTime;
		void SetLatestActiveTime(TimeStampSec latestActiveTime);

		TimeStampSec m_LatestDBSyncTime;

		PartyUID m_PartyUID;



		//////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Player Info
		//

		PlayerID GetPlayerID() { return GetAccountID(); }



	protected:


	public:

		GamePlayerEntity();
		virtual ~GamePlayerEntity();

		PlayerState GetPlayerState() { return m_PlayerState; }
		void SetPlayerState(PlayerState value) { m_PlayerState = value; }


		GameInsUID GetGameInsUID() { return m_GameInsUID; }
		uint GetShardID() { return m_ShardID; }

		DurationMS GetPlayerAutoLogoutTime() { return m_PlayerAutoLogout; }

		const Util::TimeStampTimer& GetKillTimer() { return m_KillTimer; }

		TimeStampSec GetLatestUpdateTime() { return m_LatestUpdateTime; }
		TimeStampSec GetLatestActiveTime() { return m_LatestActiveTime; }
		TimeStampSec GetLatestDBSyncTime() { return m_LatestDBSyncTime; }

		PartyUID GetPartyUID() { return m_PartyUID; }
		void SetPartyUID(PartyUID value) { m_PartyUID = value; }

		// Initialize entity to proceed new connection
		virtual Result InitializeEntity( EntityID newEntityID ) override;

		// Set connection for pilot
		virtual Result SetConnection( SharedPointerT<Net::Connection>&& pCon ) override;

		// Release connection if has
		virtual void ReleaseConnection() override;




		//////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Entity process
		//

		virtual Result OnNewUserTranscation();
		virtual Result UpdateDBSync(TransactionID transID = TransactionID());

		// register message handlers
		virtual Result RegisterMessageHandlers() override;

		// clear transaction
		virtual Result ClearEntity() override;

		// Run the task
		virtual Result TickUpdate(TimerAction *pAction = nullptr) override;


		virtual Transaction* CreateCloseTransaction() { return nullptr; }

		//////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Update
		//

		// Update Game Player 
		virtual Result UpdateGamePlayer();


		//////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	
		//

		bool GetIsInGame() const;

		void SetGameInsUID( const GameInsUID& gameInsID );

		virtual const PlayerInformation& GetPlayerInformation() const;
		//const ServerFriendInformation& GetFriendInformation() const;

		FORCEINLINE const char* GetNickName()							{ return m_PlayerInformation.NickName; }
		FORCEINLINE Result SetNickName( const char* newName )			{ return StrUtil::StringCpy( m_PlayerInformation.NickName, newName ); }

		virtual Result SetAccountID( AccountID accID ) override { m_PlayerInformation.PlayerID = accID; return Svr::SimpleUserEntity::SetAccountID(accID); }

		template< class ...ArgTypes >
		void AddGameTransactionLogT(TransLogCategory LogCategory, INT consume, INT gain, uint64_t totalValue, const char* strFormat, ArgTypes... args)
		{
			char strBuffer[1024];
			StrUtil::Format(strBuffer, strFormat, args...);

			AddGameTransactionLog(LogCategory, consume, gain, totalValue, strBuffer);
		}

		void AddGameTransactionLog(TransLogCategory LogCategory, INT consume, INT gain, uint64_t totalValue, const char* logMessage);
		void AddGameTransactionLog(TransLogCategory LogCategory, INT consume, INT gain, uint64_t totalValue);

		// TODO: move to component
		// Send push notify
		//Result SendPushNotify( const char* strMessage, uint64_t param = 0 );

	};

#include "GamePlayerEntity.inl"

}; // namespace GameServer
}; // namespace SF
