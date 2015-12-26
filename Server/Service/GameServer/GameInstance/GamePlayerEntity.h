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

#include "Common/Typedefs.h"
#include "Common/ClassUtil.h"
#include "Common/TimeUtil.h"
#include "Common/HashTable.h"
#include "Common/MemoryPool.h"
#include "ServerSystem/Entity.h"
#include "ServerSystem/GameSystem.h"
#include "Common/GameConst.h"
#include "Common/BrGameTypes.h"
#include "GameSystem/UserSystemComponentIDs.h"
#include "GameSvrConst.h"



namespace BR {
namespace Net {
	class Connection;
};

namespace Policy {
	class ISvrPolicyGame;
};
};



namespace BR {
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

		// Player connection
		BRCLASS_ATTRIBUTE_READONLY(Policy::ISvrPolicyGame*,ISvrGamePolicy);

		// Player state
		BRCLASS_ATTRIBUTE(PlayerState,PlayerState);

		// Game instance UID
		BRCLASS_ATTRIBUTE_READONLY(GameInsUID,GameInsUID);

		mutable ServerFriendInformation m_PlayerInformation;

		BRCLASS_ATTRIBUTE(PartyUID,PartyUID);
		BRCLASS_ATTRIBUTE(UINT, ShardID);

		BRCLASS_ATTRIBUTE(MatchingQueueTicket,MatchingTicket);

		// Time for kill this game
		BRCLASS_ATTRIBUTE_CONST(Util::TimeStampTimer,TimeToKill);

		BRCLASS_ATTRIBUTE_STRING(GCMKeys,GameConst::MAX_GCMKEYS);
		
		// Latest update time in UTC time
		BRCLASS_ATTRIBUTE(TimeStampSec,LatestUpdateTime);

		// Latest active time in UTC time
		BRCLASS_ATTRIBUTE_READONLY(TimeStampSec,LatestActiveTime);
		void SetLatestActiveTime(TimeStampSec latestActiveTime);

		BRCLASS_ATTRIBUTE(TimeStampSec,LatestDBSyncTime);



		//////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Player Info
		//

		// Player Name
		BRCLASS_ATTRIBUTE_STRING(UserName,GameConst::MAX_NAME);

		PlayerID GetPlayerID() { return GetAccountID(); }


		StaticAllocator< sizeof(Svr::EntityMessageHandlerItem)*4 >		m_Allocator;

	protected:

		virtual MemoryAllocator& GetAllocator()							{ return m_Allocator; }

	public:

		GamePlayerEntity();
		virtual ~GamePlayerEntity();

		// Initialize entity to proceed new connection
		virtual HRESULT InitializeEntity( EntityID newEntityID );

		// Set connection for pilot
		virtual HRESULT SetConnection( SharedPointerT<Net::Connection>&& pCon ) override;

		// Release connection if has
		virtual void ReleaseConnection() override;




		//////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Entity process
		//

		HRESULT OnNewUserTranscation();
		HRESULT UpdateDBSync(TransactionID transID);

		// register message handlers
		virtual HRESULT RegisterMessageHandlers();

		// clear transaction
		virtual HRESULT ClearEntity() override;

		// Run the task
		virtual HRESULT TickUpdate(Svr::TimerAction *pAction = nullptr) override;


		//////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Update
		//

		// Update game configuration
		HRESULT UpdateGameConfig();

		// Update Game Player 
		HRESULT UpdateGamePlayer();


		//////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	
		//

		bool GetIsInGame() const;

		void SetGameInsUID( const GameInsUID& gameInsID );

		const PlayerInformation& GetPlayerInformation() const;
		const ServerFriendInformation& GetFriendInformation() const;

		FORCEINLINE const char* GetNickName()							{ return m_PlayerInformation.NickName; }
		FORCEINLINE HRESULT SetNickName( const char* newName )			{ return StrUtil::StringCpy( m_PlayerInformation.NickName, newName ); }

		FORCEINLINE FacebookUID GetFacebookUID()						{ return m_PlayerInformation.FBUID; }
		FORCEINLINE void SetFacebookUID( FacebookUID newUID )			{ m_PlayerInformation.FBUID = newUID; }

		virtual HRESULT SetAccountID( AccountID accID )					{ m_PlayerInformation.PlayerID = accID; return Svr::SimpleUserEntity::SetAccountID(accID); }

		template< class ...ArgTypes >
		void AddGameTransactionLogT(TransLogCategory LogCategory, INT consume, INT gain, UINT64 totalValue, const char* strFormat, ArgTypes... args)
		{
			char strBuffer[1024];
			StrUtil::Format(strBuffer, strFormat, args...);

			AddGameTransactionLog(LogCategory, consume, gain, totalValue, strBuffer);
		}

		void AddGameTransactionLog(TransLogCategory LogCategory, INT consume, INT gain, UINT64 totalValue, const char* logMessage);
		void AddGameTransactionLog(TransLogCategory LogCategory, INT consume, INT gain, UINT64 totalValue);

		// Send push notify
		HRESULT SendPushNotify( const char* strMessage, UINT64 param = 0 );

	};

#include "GamePlayerEntity.inl"

}; // namespace GameServer
}; // namespace BR
