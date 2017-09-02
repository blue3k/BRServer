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
#include "Types/BrGameTypes.h"
#include "Memory/MemoryPool.h"
#include "Common/Memento.h"

#include "ServiceEntity/Game/GameSystem.h"
#include "Common/MemoryBufferUtil.h"
#include "GameConst.h"
#include "UserSystemComponentIDs.h"
#include "Table/conspiracy/LevelTbl.h"
#include "Table/conspiracy/ShopTbl.h"
#include "Table/conspiracy/OrganicTbl.h"


namespace SF {
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
		uint8_t m_Grade;
		int16_t m_Level;
		int64_t m_Exp;
		int64_t m_GameMoney;
		int64_t m_Gem;
		int16_t m_Stamina;
		int16_t m_AddedFriendSlot;
		int32_t m_TotalPlayed;
		int32_t m_WinPlaySCitizen;
		int32_t m_WinPlaySMonster;
		int32_t m_WinPlaySSeer;
		int32_t m_LosePlaySCitizen;
		int32_t m_LosePlaySMonster;
		int32_t m_LosePlaySSeer;
		int32_t m_WinPlayNCitizen;
		int32_t m_WinPlayNMonster;
		int32_t m_WinPlayNSeer;
		int32_t m_LosePlayNCitizen;
		int32_t m_LosePlayNMonster;
		int32_t m_LosePlayNSeer;
		int32_t m_WeeklyWin;
		int32_t m_WeeklyLose;
		// -------------- End data from DB

		// Max stat by level
		int m_MaxAutoRefillStamina;
		int m_MaxStamina;
		int m_MaxFriend;
		int64_t m_MaxGameMoney;
		int64_t m_MaxGem;

		int16_t m_DefaultFriendSlot;

	public:

		// Constructor 
		UserGamePlayerInfoSystem( GamePlayerEntity* pEntity );
		~UserGamePlayerInfoSystem();


		uint8_t GetGrade() { return m_Grade; }
		void SetGrade(uint8_t value) { m_Grade = value; }

		int16_t GetLevel() { return m_Level; }
		void SetLevel(int16_t value) { m_Level = value; }

		int64_t GetExp() { return m_Exp; }
		void SetExp(int64_t value) { m_Exp = value; }

		int64_t GetGameMoney() { return m_GameMoney; }
		void SetGameMoney(int64_t value) { m_GameMoney = value; }

		int64_t GetGem() { return m_Gem; }
		void SetGem(int64_t value) { m_Gem = value; }

		int16_t GetStamina() { return m_Stamina; }
		void SetStamina(int16_t value) { m_Stamina = value; }

		int16_t GetAddedFriendSlot() { return m_AddedFriendSlot; }
		void SetAddedFriendSlot(int16_t value) { m_AddedFriendSlot = value; }

		int32_t GetTotalPlayed() { return m_TotalPlayed; }
		void SetTotalPlayed(int32_t value) { m_TotalPlayed = value; }

		int32_t GetWinPlaySCitizen() { return m_WinPlaySCitizen; }
		void SetWinPlaySCitizen(int32_t value) { m_WinPlaySCitizen = value; }

		int32_t GetWinPlaySMonster() { return m_WinPlaySMonster; }
		void SetWinPlaySMonster(int32_t value) { m_WinPlaySMonster = value; }

		int32_t GetWinPlaySSeer() { return m_WinPlaySSeer; }
		void SetWinPlaySSeer(int32_t value) { m_WinPlaySSeer = value; }

		int32_t GetLosePlaySCitizen() { return m_LosePlaySCitizen; }
		void SetLosePlaySCitizen(int32_t value) { m_LosePlaySCitizen = value; }

		int32_t GetLosePlaySMonster() { return m_LosePlaySMonster; }
		void SetLosePlaySMonster(int32_t value) { m_LosePlaySMonster = value; }

		int32_t GetLosePlaySSeer() { return m_LosePlaySSeer; }
		void SetLosePlaySSeer(int32_t value) { m_LosePlaySSeer = value; }

		int32_t GetWinPlayNCitizen() { return m_WinPlayNCitizen; }
		void SetWinPlayNCitizen(int32_t value) { m_WinPlayNCitizen = value; }

		int32_t GetWinPlayNMonster() { return m_WinPlayNMonster; }
		void SetWinPlayNMonster(int32_t value) { m_WinPlayNMonster = value; }

		int32_t GetWinPlayNSeer() { return m_WinPlayNSeer; }
		void SetWinPlayNSeer(int32_t value) { m_WinPlayNSeer = value; }

		int32_t GetLosePlayNCitizen() { return m_LosePlayNCitizen; }
		void SetLosePlayNCitizen(int32_t value) { m_LosePlayNCitizen = value; }

		int32_t GetLosePlayNMonster() { return m_LosePlayNMonster; }
		void SetLosePlayNMonster(int32_t value) { m_LosePlayNMonster = value; }

		int32_t GetLosePlayNSeer() { return m_LosePlayNSeer; }
		void SetLosePlayNSeer(int32_t value) { m_LosePlayNSeer = value; }

		int32_t GetWeeklyWin() { return m_WeeklyWin; }
		void SetWeeklyWin(int32_t value) { m_WeeklyWin = value; }

		int32_t GetWeeklyLose() { return m_WeeklyLose; }
		void SetWeeklyLose(int32_t value) { m_WeeklyLose = value; }

		int GetMaxAutoRefillStamina() { return m_MaxAutoRefillStamina; }

		int GetMaxStamina() { return m_MaxStamina; }
		void SetMaxStamina(int value) { m_MaxStamina = value; }

		int GetMaxFriend() { return m_MaxFriend; }
		void SetMaxFriend(int value) { m_MaxFriend = value; }

		int64_t GetMaxGameMoney() { return m_MaxGameMoney; }
		void SetMaxGameMoney(int64_t value) { m_MaxGameMoney = value; }

		int64_t GetMaxGem() { return m_MaxGem; }
		void SetMaxGem(int64_t value) { m_MaxGem = value; }

		int16_t GetDefaultFriendSlot() { return m_DefaultFriendSlot; }
		void SetDefaultFriendSlot(int16_t value) { m_DefaultFriendSlot = value; }


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

		Result SetLevel( uint newLevel );

		Result GainExp( uint64_t expGain );

		Result AchivedWin( PlayerRole playedRole, bool isWon );

		Result ResetRankNormal(conspiracy::OrganicTbl::OrganicItem *pCostItem);

		// Add stamina, negative will reduce the stamina
		Result GainStamina( INT stamina );

		// Add Friend slot, negative will reduce the friend slot
		Result GainFriendSlot( INT numSlot );
		uint GetFriendSlot();

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
}; // namespace SF







