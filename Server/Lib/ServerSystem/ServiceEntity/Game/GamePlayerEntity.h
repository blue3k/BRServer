////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) The Braves
// 
// Author : KyungKun Ko
//
// Description : Game player Entity
//	
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "SFTypedefs.h"

#include "Util/SFTimeUtil.h"
#include "Container/SFHashTable.h"
#include "MemoryManager/SFMemoryPool.h"
#include "GameConst.h"

#include "Types/BrGameTypes.h"
#include "Entity/Entity.h"
#include "Entity/SimpleUserEntity.h"
#include "ServiceEntity/Game/GameSystem.h"
#include "Variable/SFVariableTable.h"


namespace SF {
namespace Net {
	class Connection;
};

	class ISvrPolicyGame;
};




namespace SF {
namespace Svr {


	class GameServer;
	class InvenSystem;
	class GameInstanceEntity;


	// GCM predefined messages
	#define BRPUSHMSG_DISCONNECTED		"BRDisconnected"
	#define BRPUSHMSG_SYNC				"BRSync"


	//////////////////////////////////////////////////////////////////////////
	//
	//	Game Instance entity class
	//

	class GamePlayerEntity : public SimpleUserEntity
	{
	public:

		using super = SimpleUserEntity;

		// Player state
		enum PlayerState
		{
			PlayerState_None,		// Nothing just created
			PlayerState_LogedIn,	// Just Logged in
		};

	private:

		// Player state
		PlayerState m_PlayerState = PlayerState_None;

		// Game instance UID
		GameInsUID m_GameInsUID = 0;

		PartyUID m_PartyUID = 0;
		uint m_ShardID = 0;

		EntityUID m_ChatChannelUID = 0;

		TimeStampMS m_MatchingStartTime = TimeStampSec(DurationSec(0));

		MatchingQueueTicket m_MatchingTicket;

		char m_GCMKeys[GameConst::MAX_GCMKEYS];

		// Latest update time in UTC time
		UTCTimeStampSec m_LatestUpdateTime{};

		// Latest active time in UTC time
		UTCTimeStampSec m_LatestActiveTime{};

		UTCTimeStampSec m_LatestDBSyncTime{};



		//////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Player Info
		//

		// Player Name
		char m_UserName[GameConst::MAX_NAME];

		// Player data
		VariableTable m_PlayerData;

		// Character data
		uint32_t m_CharacterId{};
		VariableTable m_CharacterData;
		VariableTable m_CharacterVisualData;

	protected:

		// Cached player information
		mutable ServerFriendInformation m_PlayerInformation;

		bool m_ClosingPended = false;

	public:

		GamePlayerEntity();
		virtual ~GamePlayerEntity();

		PlayerID GetPlayerID() { return GetAccountID(); }

		PlayerState GetPlayerState() const { return m_PlayerState; }
		void SetPlayerState(PlayerState value) { m_PlayerState = value; }

		const GameInsUID& GetGameInsUID() const { return m_GameInsUID; }

		const PartyUID& GetPartyUID() const { return m_PartyUID; }
		void SetPartyUID(const PartyUID& value) { m_PartyUID = value; }

		const EntityUID& GetChatChannelUID() const { return m_ChatChannelUID; }
		void SetChatChannelUID(const EntityUID& value) { m_ChatChannelUID = value; }

		uint GetShardID() const { return m_ShardID; }
		void SetShardID(uint value) { m_ShardID = value; }

		TimeStampMS GetMatchingStartTime() const { return m_MatchingStartTime; }

		const MatchingQueueTicket& GetMatchingTicket() const { return m_MatchingTicket; }
		void SetMatchingTicket(MatchingQueueTicket ticket);

		const char* GetGCMKeys() const { return m_GCMKeys; }
		void SetGCMKeys(const char* value) { StrUtil::StringCopy(m_GCMKeys, value); }

		UTCTimeStampSec GetLatestUpdateTime() const { return m_LatestUpdateTime; }
		void SetLatestUpdateTime(UTCTimeStampSec value) { m_LatestUpdateTime = value; }


		UTCTimeStampSec GetLatestActiveTime() const { return m_LatestActiveTime; }
		virtual void SetLatestActiveTime(UTCTimeStampSec latestActiveTime) { m_LatestActiveTime = latestActiveTime; }

		UTCTimeStampSec GetLatestDBSyncTime() const { return m_LatestDBSyncTime; }
		void SetLatestDBSyncTime(UTCTimeStampSec value) { m_LatestDBSyncTime = value; }


		const char* GetUserName() const { return m_UserName; }
		void SetUserName(const char* value) { StrUtil::StringCopy(m_UserName, value); }


		// Initialize entity to proceed new connection
		virtual Result InitializeEntity(EntityID newEntityID) override;

		Result RegisterMessageHandlers() override;

		// Set connection for pilot
		virtual Result SetConnection(SharedPointerT<Net::Connection>&& pCon) override;

		VariableTable& GetPlayerData() { return m_PlayerData; }

		uint32_t GetCharacterID() const { return m_CharacterId; }
		void SetCharacterID(uint32_t characterId) { m_CharacterId = characterId; }

		VariableTable& GetCharacterVisualData() { return m_CharacterVisualData; }
		Result SetCharacterVisualData(const VariableTable& characterVisualData);

		VariableTable& GetCharacterData() { return m_CharacterData; }
		Result SetCharacterData(const VariableTable& characterData);
		void ClearCharacterData();


		//////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Entity process
		//

		Result OnJoinGameServerInitialize(AuthTicket authTicket, FacebookUID fbUID);
		virtual void Heartbeat() override;
		Result OnNewUserTranscation();
		virtual Result UpdateDBSync(TransactionID transID = TransactionID()) { return ResultCode::SUCCESS; }

		Result PendingCloseTransaction(const char* reason) override;

		// register message handlers
		//virtual Result RegisterMessageHandlers();

		// clear transaction
		virtual Result ClearEntity() override;

		// Run the task
		virtual Result TickUpdate(TimerAction *pAction = nullptr) override;


		//////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Update
		//

		// Update game configuration
		virtual Result UpdateGameConfig() { return ResultCode::SUCCESS; }

		// Update Game Player 
		virtual Result UpdateGamePlayer() { return ResultCode::SUCCESS; }




		//////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	
		//

		bool GetIsInGame() const;

		void SetGameInsUID(const GameInsUID& gameInsID);

		virtual const PlayerInformation& GetPlayerInformation() const;
		virtual const ServerFriendInformation& GetFriendInformation() const;

		SF_FORCEINLINE const char* GetNickName() { return m_PlayerInformation.NickName; }
		SF_FORCEINLINE Result SetNickName(const char* newName) { return StrUtil::StringCopy(m_PlayerInformation.NickName, newName); }

		SF_FORCEINLINE FacebookUID GetFacebookUID() { return m_PlayerInformation.FBUID; }
		SF_FORCEINLINE void SetFacebookUID(FacebookUID newUID) { m_PlayerInformation.FBUID = newUID; }

		virtual Result SetAccountID(AccountID accID) override
		{
			m_PlayerInformation.PlayerID = accID;
			return Svr::SimpleUserEntity::SetAccountID(accID);
		}

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
		Result SendPushNotify(const char* strMessage, uint64_t param = 0);

	};

} // namespace Svr
} // namespace SF
