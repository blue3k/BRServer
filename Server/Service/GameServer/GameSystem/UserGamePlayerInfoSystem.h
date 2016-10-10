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

#include "Common/Typedefs.h"
#include "Common/BrGameTypes.h"
#include "Common/MemoryPool.h"
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
		BRCLASS_ATTRIBUTE(BYTE, Grade);
		BRCLASS_ATTRIBUTE_READONLY(SHORT, Level);
		BRCLASS_ATTRIBUTE(INT64, Exp);
		BRCLASS_ATTRIBUTE(INT64, GameMoney);
		BRCLASS_ATTRIBUTE(INT64, Gem);
		BRCLASS_ATTRIBUTE(SHORT, Stamina);
		BRCLASS_ATTRIBUTE(SHORT, AddedFriendSlot);
		BRCLASS_ATTRIBUTE(INT32, TotalPlayed);
		BRCLASS_ATTRIBUTE(INT32, WinPlaySCitizen);
		BRCLASS_ATTRIBUTE(INT32, WinPlaySMonster);
		BRCLASS_ATTRIBUTE(INT32, WinPlaySSeer);
		BRCLASS_ATTRIBUTE(INT32, LosePlaySCitizen);
		BRCLASS_ATTRIBUTE(INT32, LosePlaySMonster);
		BRCLASS_ATTRIBUTE(INT32, LosePlaySSeer);
		BRCLASS_ATTRIBUTE(INT32, WinPlayNCitizen);
		BRCLASS_ATTRIBUTE(INT32, WinPlayNMonster);
		BRCLASS_ATTRIBUTE(INT32, WinPlayNSeer);
		BRCLASS_ATTRIBUTE(INT32, LosePlayNCitizen);
		BRCLASS_ATTRIBUTE(INT32, LosePlayNMonster);
		BRCLASS_ATTRIBUTE(INT32, LosePlayNSeer);
		BRCLASS_ATTRIBUTE(INT32, WeeklyWin);
		BRCLASS_ATTRIBUTE(INT32, WeeklyLose);
		// -------------- End data from DB

		// Max stat by level
		BRCLASS_ATTRIBUTE_READONLY(INT, MaxAutoRefillStamina);
		BRCLASS_ATTRIBUTE_READONLY(INT, MaxStamina);
		BRCLASS_ATTRIBUTE_READONLY(INT, MaxFriend);
		BRCLASS_ATTRIBUTE_READONLY(INT64, MaxGameMoney);
		BRCLASS_ATTRIBUTE_READONLY(INT64, MaxGem);

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

		Result GainExp( UINT64 expGain );

		Result AchivedWin( PlayerRole playedRole, bool isWon );

		Result ResetRankNormal(conspiracy::OrganicTbl::OrganicItem *pCostItem);

		// Add stamina, negative will reduce the stamina
		Result GainStamina( INT stamina );

		// Add Friend slot, negative will reduce the friend slot
		Result GainFriendSlot( INT numSlot );
		UINT GetFriendSlot();

		// gain game money
		Result GainGameMoney( INT64 numValue );

		// gain gem
		Result GainGem( INT64 numValue );

		// Called when the stat maiximum need to be calculated again.
		Result UpdateStatMaximum();

		// Setup default stat for new player
		Result SetupDefaultStat();

		// Called when level is changed
		Result UpdateStatByLevel();
		Result UpdateStatByLevel(conspiracy::LevelTbl::LevelItem *pLevelInfo);

		Result SavePurchaseInfoToDB(TransactionID transID, const Array<BYTE>& purchaseID, const char* purchasePlatform, const char* purchaseToken);
		Result SavePlayerInfoToDB(TransactionID transID);
	};


}; // namespace GameServer
}; // namespace BR







