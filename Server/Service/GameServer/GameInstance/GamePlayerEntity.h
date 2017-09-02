////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
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
#include "Entity/Entity.h"
#include "ServiceEntity/Game/GameSystem.h"
#include "GameConst.h"
#include "Types/BrGameTypes.h"
#include "GameSystem/UserSystemComponentIDs.h"
#include "GameSvrConst.h"



namespace SF {
namespace Net {
	class Connection;
};

namespace Policy {
	class ISvrPolicyGame;
};
};



namespace SF {
namespace GameServer {


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

	class GamePlayerEntity : public Svr::SimpleUserEntity, public ComponentCarrier<UserSystemComponentID_Max>, public MemoryPoolObject<GamePlayerEntity>
	{
	public:

		// Player state
		enum PlayerState
		{
			PlayerState_None,		// Nothing just created
			PlayerState_LogedIn,	// Just Loged in
		};

	private:

		// Player state
		PlayerState m_PlayerState;


		// Game instance UID
		GameInsUID m_GameInsUID;


		mutable ServerFriendInformation m_PlayerInformation;

		PartyUID m_PartyUID;
		uint m_ShardID;

		TimeStampMS m_MatchingStartTime;

		MatchingQueueTicket m_MatchingTicket;

		// Time for kill this game
		Util::TimeStampTimer m_TimeToKill;

		char m_GCMKeys[GameConst::MAX_GCMKEYS];
		
		// Latest update time in UTC time
		TimeStampSec m_LatestUpdateTime;

		// Latest active time in UTC time
		TimeStampSec m_LatestActiveTime;

		TimeStampSec m_LatestDBSyncTime;



		//////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Player Info
		//

		// Player Name
		char m_UserName[GameConst::MAX_NAME];

		PlayerID GetPlayerID() { return GetAccountID(); }


	public:

		GamePlayerEntity();
		virtual ~GamePlayerEntity();

		PlayerState GetPlayerState() { return m_PlayerState; }
		void SetPlayerState(PlayerState value) { m_PlayerState = value; }

		const GameInsUID& GetGameInsUID() { return m_GameInsUID; }

		const PartyUID& GetPartyUID() { return m_PartyUID; }
		void SetPartyUID(const PartyUID& value) { m_PartyUID = value; }

		uint GetShardID() { return m_ShardID; }
		void SetShardID(uint value) { m_ShardID = value; }

		TimeStampMS GetMatchingStartTime() { return m_MatchingStartTime; }

		const MatchingQueueTicket& GetMatchingTicket() { return m_MatchingTicket; }
		void SetMatchingTicket(MatchingQueueTicket ticket);

		const Util::TimeStampTimer& GetTimeToKill() { return m_TimeToKill; }


		const char* GetGCMKeys() { return m_GCMKeys; }
		void SetGCMKeys(const char* value) { StrUtil::StringCpy(m_GCMKeys, value); }

		TimeStampSec GetLatestUpdateTime() { return m_LatestUpdateTime; }
		void SetLatestUpdateTime(TimeStampSec value) { m_LatestUpdateTime = value; }


		TimeStampSec GetLatestActiveTime() { return m_LatestActiveTime; }
		void SetLatestActiveTime(TimeStampSec latestActiveTime);

		TimeStampSec GetLatestDBSyncTime() { return m_LatestDBSyncTime; }
		void SetLatestDBSyncTime(TimeStampSec value) { m_LatestDBSyncTime = value; }


		const char* GetUserName() { return m_UserName; }
		void SetUserName(const char* value) { StrUtil::StringCpy(m_UserName, value); }


		// Initialize entity to proceed new connection
		virtual Result InitializeEntity( EntityID newEntityID );

		// Set connection for pilot
		virtual Result SetConnection( SharedPointerT<Net::Connection>&& pCon ) override;

		// Release connection if has
		virtual void ReleaseConnection() override;




		//////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Entity process
		//

		Result OnJoinGameServerInitialize(AuthTicket authTicket, FacebookUID fbUID);

		Result OnNewUserTranscation();
		Result UpdateDBSync(TransactionID transID = TransactionID());

		// register message handlers
		virtual Result RegisterMessageHandlers();

		// clear transaction
		virtual Result ClearEntity() override;

		// Run the task
		virtual Result TickUpdate(TimerAction *pAction = nullptr) override;


		//////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Update
		//

		// Update game configuration
		Result UpdateGameConfig();

		// Update Game Player 
		Result UpdateGamePlayer();


		//////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	
		//

		bool GetIsInGame() const;

		void SetGameInsUID( const GameInsUID& gameInsID );

		const PlayerInformation& GetPlayerInformation() const;
		const ServerFriendInformation& GetFriendInformation() const;

		FORCEINLINE const char* GetNickName()							{ return m_PlayerInformation.NickName; }
		FORCEINLINE Result SetNickName( const char* newName )			{ return StrUtil::StringCpy( m_PlayerInformation.NickName, newName ); }

		FORCEINLINE FacebookUID GetFacebookUID()						{ return m_PlayerInformation.FBUID; }
		FORCEINLINE void SetFacebookUID( FacebookUID newUID )			{ m_PlayerInformation.FBUID = newUID; }

		virtual Result SetAccountID( AccountID accID )					{ m_PlayerInformation.PlayerID = accID; return Svr::SimpleUserEntity::SetAccountID(accID); }

		template< class ...ArgTypes >
		void AddGameTransactionLogT(TransLogCategory LogCategory, INT consume, INT gain, uint64_t totalValue, const char* strFormat, ArgTypes... args)
		{
			char strBuffer[1024];
			StrUtil::Format(strBuffer, strFormat, args...);

			AddGameTransactionLog(LogCategory, consume, gain, totalValue, strBuffer);
		}

		void AddGameTransactionLog(TransLogCategory LogCategory, INT consume, INT gain, uint64_t totalValue, const char* logMessage);
		void AddGameTransactionLog(TransLogCategory LogCategory, INT consume, INT gain, uint64_t totalValue);

		// Send push notify
		Result SendPushNotify( const char* strMessage, uint64_t param = 0 );

	};

#include "GamePlayerEntity.inl"

}; // namespace GameServer
}; // namespace SF
