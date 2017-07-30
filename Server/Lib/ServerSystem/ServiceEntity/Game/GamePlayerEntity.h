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

#include "Common/Typedefs.h"
#include "Common/ClassUtil.h"
#include "Common/TimeUtil.h"
#include "Common/HashTable.h"
#include "Common/MemoryPool.h"
#include "Common/GameConst.h"
#include "Common/ClassUtil.h"
#include "Common/BrGameTypes.h"
#include "ServerSystem/Entity.h"
#include "ServerSystem/GameSystem.h"
#include "ServerSystem/SimpleUserEntity.h"



namespace BR {
namespace Net {
	class Connection;
};

namespace Policy {
	class ISvrPolicyGame;
};
};



namespace BR {
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
		BRCLASS_ATTRIBUTE_READONLY(Policy::ISvrPolicyGame*,ISvrGamePolicy);

		// Player state
		BRCLASS_ATTRIBUTE(PlayerState,PlayerState);

		// Game instance UID
		BRCLASS_ATTRIBUTE_READONLY(GameInsUID,GameInsUID);

		mutable PlayerInformation m_PlayerInformation;

		BRCLASS_ATTRIBUTE(UINT, ShardID);

		BRCLASS_ATTRIBUTE(DurationMS, PlayerAutoLogout);

		// Time for kill this game
		BRCLASS_ATTRIBUTE_CONST(Util::TimeStampTimer, TimeToKill);

		// Latest update time in UTC time
		BRCLASS_ATTRIBUTE(TimeStampSec, LatestUpdateTime);

		// Latest active time in UTC time
		BRCLASS_ATTRIBUTE_READONLY(TimeStampSec, LatestActiveTime);
		void SetLatestActiveTime(TimeStampSec latestActiveTime);

		BRCLASS_ATTRIBUTE(TimeStampSec, LatestDBSyncTime);

		BRCLASS_ATTRIBUTE(PartyUID, PartyUID);



		//BRCLASS_ATTRIBUTE_READONLY(TimeStampMS, MatchingStartTime);
		//BRCLASS_ATTRIBUTE_READONLY(MatchingQueueTicket,MatchingTicket);
		//void SetMatchingTicket(MatchingQueueTicket ticket);


		//BRCLASS_ATTRIBUTE_STRING(GCMKeys,GameConst::MAX_GCMKEYS);
		



		//////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Player Info
		//

		PlayerID GetPlayerID() { return GetAccountID(); }



	protected:


	public:

		GamePlayerEntity();
		virtual ~GamePlayerEntity();

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
		void AddGameTransactionLogT(TransLogCategory LogCategory, INT consume, INT gain, UINT64 totalValue, const char* strFormat, ArgTypes... args)
		{
			char strBuffer[1024];
			StrUtil::Format(strBuffer, strFormat, args...);

			AddGameTransactionLog(LogCategory, consume, gain, totalValue, strBuffer);
		}

		void AddGameTransactionLog(TransLogCategory LogCategory, INT consume, INT gain, UINT64 totalValue, const char* logMessage);
		void AddGameTransactionLog(TransLogCategory LogCategory, INT consume, INT gain, UINT64 totalValue);

		// TODO: move to component
		// Send push notify
		//Result SendPushNotify( const char* strMessage, UINT64 param = 0 );

	};

#include "GamePlayerEntity.inl"

}; // namespace GameServer
}; // namespace BR
