////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Game play system
//	
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "SFTypedefs.h"
#include "Common/BrGameTypes.h"
#include "Memory/MemoryPool.h"
#include "Common/Memento.h"
#include "Common/ClassUtil.h"
#include "ServerSystem/GameSystem.h"
#include "Common/MemoryBufferUtil.h"
#include "Common/GameConst.h"
#include "UserSystemComponentIDs.h"
#include "Table/conspiracy/LevelTbl.h"
#include "Table/conspiracy/ShopTbl.h"
#include "Table/conspiracy/OrganicTbl.h"


namespace BR {
namespace GameServer {

	class GamePlayerEntity;

	//////////////////////////////////////////////////////////////////////////
	//
	//	GamePlaySystem class
	//

	class UserGamePlayerInfoSystem : public Svr::GameSystem<GamePlayerEntity,UserGamePlayerInfoSystem>
	{
	public:

		enum {
			ComponentID		= UserSystemComponentID_GamePlayerInfo,

			MEMENTO_SIZE	= 512
		};

	private:

		// -------------- Start data from DB
		BRCLASS_ATTRIBUTE(uint8_t, Grade);
		BRCLASS_ATTRIBUTE_READONLY(SHORT, Level);
		BRCLASS_ATTRIBUTE(int64_t, Exp);
		BRCLASS_ATTRIBUTE(int64_t, GameMoney);
		BRCLASS_ATTRIBUTE(int64_t, Gem);
		BRCLASS_ATTRIBUTE(SHORT, Stamina);
		BRCLASS_ATTRIBUTE(SHORT, AddedFriendSlot);
		BRCLASS_ATTRIBUTE(int32_t, TotalPlayed);
		BRCLASS_ATTRIBUTE(int32_t, WinPlaySCitizen);
		BRCLASS_ATTRIBUTE(int32_t, WinPlaySMonster);
		BRCLASS_ATTRIBUTE(int32_t, WinPlaySSeer);
		BRCLASS_ATTRIBUTE(int32_t, LosePlaySCitizen);
		BRCLASS_ATTRIBUTE(int32_t, LosePlaySMonster);
		BRCLASS_ATTRIBUTE(int32_t, LosePlaySSeer);
		BRCLASS_ATTRIBUTE(int32_t, WinPlayNCitizen);
		BRCLASS_ATTRIBUTE(int32_t, WinPlayNMonster);
		BRCLASS_ATTRIBUTE(int32_t, WinPlayNSeer);
		BRCLASS_ATTRIBUTE(int32_t, LosePlayNCitizen);
		BRCLASS_ATTRIBUTE(int32_t, LosePlayNMonster);
		BRCLASS_ATTRIBUTE(int32_t, LosePlayNSeer);
		BRCLASS_ATTRIBUTE(int32_t, WeeklyWin);
		BRCLASS_ATTRIBUTE(int32_t, WeeklyLose);
		// -------------- End data from DB

		// Max stat by level
		BRCLASS_ATTRIBUTE_READONLY(INT, MaxAutoRefillStamina);
		BRCLASS_ATTRIBUTE_READONLY(INT, MaxStamina);
		BRCLASS_ATTRIBUTE_READONLY(INT, MaxFriend);
		BRCLASS_ATTRIBUTE_READONLY(int64_t, MaxGameMoney);
		BRCLASS_ATTRIBUTE_READONLY(int64_t, MaxGem);

		BRCLASS_ATTRIBUTE(SHORT, DefaultFriendSlot);

	public:

		// Constructor 
		UserGamePlayerInfoSystem( GamePlayerEntity* pEntity );
		~UserGamePlayerInfoSystem();

		// Initialize server component
		virtual Result InitializeComponent();

		// Terminate server component
		virtual void TerminateComponent();


		////////////////////////////////////////////////////////////////////
		//
		//	Operations
		//

		// Save to memento
		Result SaveStatToMemento( Memento<MEMENTO_SIZE> &memento );
		Result SaveWinLoseToMemento( Memento<MEMENTO_SIZE> &memento );

		// Apply shop item
		Result ApplyItem( conspiracy::ShopTbl::ShopItem *pShopItem );

		// Apply Cost
		Result CheckCost(conspiracy::OrganicTbl::OrganicItem *pCostItem);
		Result ApplyCost(conspiracy::OrganicTbl::OrganicItem *pCostItem, TransLogCategory logCategory, const char* message);

		Result SetLevel( UINT newLevel );

		Result GainExp( uint64_t expGain );

		Result AchivedWin( PlayerRole playedRole, bool isWon );

		Result ResetRankNormal(conspiracy::OrganicTbl::OrganicItem *pCostItem);

		// Add stamina, negative will reduce the stamina
		Result GainStamina( INT stamina );

		// Add Friend slot, negative will reduce the friend slot
		Result GainFriendSlot( INT numSlot );
		UINT GetFriendSlot();

		// gain game money
		Result GainGameMoney( int64_t numValue );

		// gain gem
		Result GainGem( int64_t numValue );

		// Called when the stat maiximum need to be calculated again.
		Result UpdateStatMaximum();

		// Setup default stat for new player
		Result SetupDefaultStat();

		// Called when level is changed
		Result UpdateStatByLevel();
		Result UpdateStatByLevel(conspiracy::LevelTbl::LevelItem *pLevelInfo);

		Result SavePurchaseInfoToDB(TransactionID transID, const Array<uint8_t>& purchaseID, const char* purchasePlatform, const char* purchaseToken);
		Result SavePlayerInfoToDB(TransactionID transID);
	};


}; // namespace GameServer
}; // namespace BR







