////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Game play system
//	
////////////////////////////////////////////////////////////////////////////////




#include "stdafx.h"

#include "Common/TimeUtil.h"
#include "Common/BrRandom.h"
#include "Common/ResultCode/BRResultCodeSystem.h"
#include "Common/ResultCode/BRResultCodeSvr.h"
#include "Common/ResultCode/BRResultCodeGame.h"

#include "ServerSystem/SvrConst.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/BrServerUtil.h"
#include "GameSvrConst.h"

#include "Protocol/Policy/GameInstanceIPolicy.h"
#include "GameServerClass.h"
#include "Protocol/Policy/GameIPolicy.h"

#include "GameSystem/UserGamePlayerInfoSystem.h"
#include "GameInstance/GamePlayerEntity.h"

#include "DB/GameConspiracyDB.h"


#include "Table/conspiracy/RewardTbl.h"
#include "Table/conspiracy/LevelTbl.h"
#include "Table/conspiracy/ShopTbl.h"


BR_MEMORYPOOL_IMPLEMENT(GameServer::UserGamePlayerInfoSystem);


namespace BR {
namespace GameServer {


	
	//////////////////////////////////////////////////////////////////////////
	//
	//	GamePlay state calss
	//

	// Constructor 
	UserGamePlayerInfoSystem::UserGamePlayerInfoSystem( GamePlayerEntity* pEntity )
		:GameSystem(pEntity)
	{
	}

	UserGamePlayerInfoSystem::~UserGamePlayerInfoSystem()
	{
	}

	// Initialzie system
	HRESULT UserGamePlayerInfoSystem::InitializeComponent()
	{
		HRESULT hr = S_OK;

		m_Grade = 0;
		m_Level = 0;
		m_Exp = 0;
		m_GameMoney = 0;
		m_Gem = 0;
		m_Stamina = 0;
		m_AddedFriendSlot = 0;
		m_TotalPlayed = 0;
		m_WinPlaySCitizen = 0;
		m_WinPlaySMonster = 0;
		m_WinPlaySSeer = 0;
		m_LosePlaySCitizen = 0;
		m_LosePlaySMonster = 0;
		m_LosePlaySSeer = 0;
		m_WinPlayNCitizen = 0;
		m_WinPlayNMonster = 0;
		m_WinPlayNSeer = 0;
		m_LosePlayNCitizen = 0;
		m_LosePlayNMonster = 0;
		m_LosePlayNSeer = 0;
		m_WeeklyWin = 0;
		m_WeeklyLose = 0;

		//m_MaxStamina = 30;
		//m_MaxFriend = 30;
		//m_MaxGameMoney = 30;
		//m_MaxGem = 30;
		svrChk( UpdateStatMaximum() );

	Proc_End:

		return hr;
	}

	// Terminate component
	void UserGamePlayerInfoSystem::TerminateComponent()
	{
	}

	////////////////////////////////////////////////////////////////////
	//
	//	Operations
	//

	// Save to memento
	HRESULT UserGamePlayerInfoSystem::SaveStatToMemento( Memento<MEMENTO_SIZE> &memento )
	{
		HRESULT hr = S_OK;

		svrChk( memento.Add(m_Level) );
		svrChk( memento.Add(m_Exp) );
		svrChk( memento.Add(m_GameMoney) );
		svrChk( memento.Add(m_Gem) );
		svrChk( memento.Add(m_Stamina) );
		svrChk(memento.Add(m_AddedFriendSlot));
		svrChk( memento.Add(m_TotalPlayed) );

	Proc_End:


		return hr;
	}
	
	// Save to memento
	HRESULT UserGamePlayerInfoSystem::SaveWinLoseToMemento( Memento<MEMENTO_SIZE> &memento )
	{
		HRESULT hr = S_OK;

		svrChk( memento.Add(m_WinPlaySCitizen) );
		svrChk( memento.Add(m_WinPlaySMonster) );
		svrChk( memento.Add(m_WinPlaySSeer) );
		svrChk( memento.Add(m_LosePlaySCitizen) );
		svrChk( memento.Add(m_LosePlaySMonster) );
		svrChk( memento.Add(m_LosePlaySSeer));
		svrChk( memento.Add(m_WinPlayNCitizen));
		svrChk( memento.Add(m_WinPlayNMonster) );
		svrChk( memento.Add(m_WinPlayNSeer));
		svrChk( memento.Add(m_LosePlayNCitizen));
		svrChk( memento.Add(m_LosePlayNMonster) );
		svrChk( memento.Add(m_LosePlayNSeer));

	Proc_End:


		return hr;
	}

	// Apply shop item
	HRESULT UserGamePlayerInfoSystem::ApplyItem( conspiracy::ShopTbl::ShopItem *pShopItem )
	{
		HRESULT hr = S_OK;
		conspiracy::LevelTbl::LevelItem *pLevelInfo = nullptr;

		if( pShopItem == nullptr )
			return E_SYSTEM_POINTER;

		if( FAILED(conspiracy::LevelTbl::FindItem( m_Level, pLevelInfo )) )
			return E_INVALID_PLAYER_LEVEL;

		if( pShopItem->RequiredGem > GetGem()
			|| pShopItem->RequiredGameMoney > GetGameMoney()
			//|| pShopItem->RequiredCash > GetCash()
			)
		{
			return E_GAME_NOTENOUGH_RESOURCE;
		}

		GetOwner().AddGameTransactionLog(TransLogCategory::Buy, pShopItem->RequiredGem, pShopItem->RequiredGameMoney, pShopItem->Quantity);

		switch( (int)pShopItem->ItemEffect )
		{
		case (int)conspiracy::ShopTbl::EItemEffect::Enum::Gem:
			svrChk( GainGem(pShopItem->Quantity) );
			break;
		case (int)conspiracy::ShopTbl::EItemEffect::Enum::Stamina:
			svrChk( GainStamina(pShopItem->Quantity) );
			break;
		case (int)conspiracy::ShopTbl::EItemEffect::Enum::FriendSlot:
			svrChk( GainFriendSlot(pShopItem->Quantity) );
			break;
		default:
			return E_GAME_INVALID_SHOPITEM;
			break;
		}

		m_Gem -= pShopItem->RequiredGem;
		m_GameMoney -= pShopItem->RequiredGameMoney;
		//m_Cash -= pShopItem->RequiredCash;

		UpdateStatByLevel(pLevelInfo);

	Proc_End:


		return hr;
	}


	// Apply Cost
	HRESULT UserGamePlayerInfoSystem::CheckCost(conspiracy::OrganicTbl::OrganicItem *pCostItem)
	{
		HRESULT hr = S_OK;

		if (pCostItem == nullptr)
			return E_SYSTEM_POINTER;

		if (GetGem() < pCostItem->RequiredGem || GetGameMoney() < pCostItem->RequiredGameMoney)
			return E_GAME_NOTENOUGH_RESOURCE;

	//Proc_End:

		return hr;
	}

	HRESULT UserGamePlayerInfoSystem::ApplyCost(conspiracy::OrganicTbl::OrganicItem *pCostItem, TransLogCategory logCategory, const char* message)
	{
		HRESULT hr = S_OK;

		if (pCostItem == nullptr)
			return E_SYSTEM_POINTER;

		hr = CheckCost(pCostItem);
		if (FAILED(hr))
			return hr;

		m_Gem -= pCostItem->RequiredGem;
		m_GameMoney -= pCostItem->RequiredGameMoney;
		//m_Cash -= pShopItem->RequiredCash;

		GetOwner().AddGameTransactionLogT(TransLogCategory::Buy, pCostItem->RequiredGem, pCostItem->RequiredGameMoney, 0, "Remain, Gem:{0}, GameMoney:{1}, message:{2}", GetGem(), GetGameMoney(), message != nullptr ? message : "");


	//Proc_End:

		return hr;
	}


	HRESULT UserGamePlayerInfoSystem::SetLevel( UINT newLevel )
	{
		HRESULT hr = S_OK;

		conspiracy::LevelTbl::LevelItem *pLevelInfo = nullptr;

		// modify
		if( newLevel == 0 )
			newLevel = 1;

		svrChk( conspiracy::LevelTbl::FindItem( newLevel, pLevelInfo ) );

		m_Level = newLevel;
		UpdateStatByLevel(pLevelInfo);

	Proc_End:

		if( FAILED(hr) )
			svrTrace( Trace::TRC_ERROR, "Player Setlevel is failed to set {0}", newLevel );

		return hr;
	}

	HRESULT UserGamePlayerInfoSystem::GainExp( UINT64 expGain )
	{
		HRESULT hr = S_OK;
		conspiracy::LevelTbl::LevelItem *pNextLevelInfo = nullptr;

		m_Exp += expGain;

		int nextLevel = m_Level + 1;
		svrChk( conspiracy::LevelTbl::FindItem( nextLevel, pNextLevelInfo ) );

		while( m_Exp >= pNextLevelInfo->RequiredExpTotal )
		{
			m_Level++;

			// Send level up to client
			if( GetOwner().GetISvrGamePolicy() != nullptr )
			{
				GetOwner().GetISvrGamePolicy()->LevelUpS2CEvt( m_Exp, m_Level );
			}

			UpdateStatByLevel(pNextLevelInfo);

			nextLevel = m_Level + 1;
			svrChk( conspiracy::LevelTbl::FindItem( nextLevel, pNextLevelInfo ) );
		}

	Proc_End:

		return hr;
	}

	HRESULT UserGamePlayerInfoSystem::AchivedWin( PlayerRole playedRole, bool isWon )
	{
		HRESULT hr = S_OK;

		switch( playedRole )
		{
		case PlayerRole::Seer:
			if (isWon)
			{
				m_WinPlayNSeer++;
			}
			else
			{
				m_LosePlayNSeer++;
			}
			break;
		case PlayerRole::Villager:
		case PlayerRole::Medium:
		//case PlayerRole::Bodyguard:
		//case PlayerRole::Owlman:
			if( isWon )
			{
				m_WinPlayNCitizen++;
			}
			else
			{
				m_LosePlayNCitizen++;
			}
			break;
		case PlayerRole::Werewolf:
			if( isWon )
			{
				m_WinPlayNMonster++;
			}
			else
			{
				m_LosePlayNMonster++;
			}
			break;
		default:
			svrErr(E_GAME_INVALID_ROLE);
		}

		m_TotalPlayed++;


	Proc_End:

		return hr;
	}

	HRESULT UserGamePlayerInfoSystem::ResetRankNormal(conspiracy::OrganicTbl::OrganicItem *pCostItem)
	{
		HRESULT hr = ApplyCost(pCostItem, TransLogCategory::Buy, "ResetRank");
		if (FAILED(hr))
			return hr;

		m_WinPlayNCitizen = 0;
		m_WinPlayNMonster = 0;
		m_WinPlayNSeer = 0;
		m_LosePlayNCitizen = 0;
		m_LosePlayNMonster = 0;
		m_LosePlayNSeer = 0;

		return hr;
	}
	

	// Add stamina, negative will reduce the stamina
	HRESULT UserGamePlayerInfoSystem::GainStamina( INT stamina )
	{
		HRESULT hr = S_OK;

		svrChkPtr(GetMyServer()->GetPresetGameConfig());

		Assert(m_MaxStamina > 10);

		m_Stamina = std::min( m_Stamina + stamina, m_MaxStamina );
		m_Stamina = std::max( m_Stamina, (SHORT)0 );

	Proc_End:

		return hr;
	}


	// Add Friend slot, negative will reduce the friend slot
	HRESULT UserGamePlayerInfoSystem::GainFriendSlot( INT numSlot )
	{
		HRESULT hr = S_OK;
		auto maxFriend = m_MaxFriend >= m_DefaultFriendSlot ? m_MaxFriend - m_DefaultFriendSlot : 0;

		svrChkPtr(GetMyServer()->GetPresetGameConfig());

		Assert(m_MaxFriend > 10);

		m_AddedFriendSlot = std::min(m_AddedFriendSlot + numSlot, maxFriend);
		m_AddedFriendSlot = std::max(m_AddedFriendSlot, (SHORT)0);

	Proc_End:

		return hr;
	}

	UINT UserGamePlayerInfoSystem::GetFriendSlot()
	{
		return m_DefaultFriendSlot + m_AddedFriendSlot;
	}
	
	// gain game money
	HRESULT UserGamePlayerInfoSystem::GainGameMoney( INT64 numValue )
	{
		HRESULT hr = S_OK;

		svrChkPtr(GetMyServer()->GetPresetGameConfig());

		Assert(m_MaxGameMoney > 10);

		m_GameMoney = std::min( m_GameMoney + numValue, m_MaxGameMoney );
		m_GameMoney = std::max( m_GameMoney, (INT64)0 );

	Proc_End:

		return hr;
	}

	// gain gem
	HRESULT UserGamePlayerInfoSystem::GainGem( INT64 numValue )
	{
		HRESULT hr = S_OK;

		svrChkPtr(GetMyServer()->GetPresetGameConfig());

		Assert(m_MaxGem > 10);

		m_Gem = std::min( m_Gem + numValue, m_MaxGem );
		m_Gem = std::max( m_Gem, (INT64)0 );

	Proc_End:

		return hr;
	}


	// Called when the stat maiximum need to be calculated again.
	HRESULT UserGamePlayerInfoSystem::UpdateStatMaximum()
	{
		HRESULT hr = S_OK;

		svrChkPtr(GetMyServer()->GetPresetGameConfig());

		m_MaxAutoRefillStamina = GetMyServer()->GetPresetGameConfig()->MaxAutoRefilStamina;
		m_MaxStamina = GetMyServer()->GetPresetGameConfig()->MaxStamina;
		m_MaxFriend = GetMyServer()->GetPresetGameConfig()->MaxFriend;
		m_MaxGameMoney = GetMyServer()->GetPresetGameConfig()->MaxMoney;
		m_MaxGem = GetMyServer()->GetPresetGameConfig()->MaxGem;

		m_DefaultFriendSlot = GetMyServer()->GetPresetGameConfig()->DefaultFriend;

	Proc_End:

		return hr;
	}


	
	// Setup default stat for new player
	HRESULT UserGamePlayerInfoSystem::SetupDefaultStat()
	{
		HRESULT hr = S_OK;

		svrChkPtr(GetMyServer()->GetPresetGameConfig());

		m_AddedFriendSlot = 0;

		m_Stamina = GetMyServer()->GetPresetGameConfig()->DefaultStamina;
		m_GameMoney = GetMyServer()->GetPresetGameConfig()->DefaultMoney;
		m_Gem = GetMyServer()->GetPresetGameConfig()->DefaultGem;

	Proc_End:

		return hr;
	}
	HRESULT UserGamePlayerInfoSystem::UpdateStatByLevel()
	{
		HRESULT hr = S_OK;
		conspiracy::LevelTbl::LevelItem *pLevelInfo = nullptr;

		svrChk( conspiracy::LevelTbl::FindItem( GetLevel(), pLevelInfo ) );

		svrChk( UpdateStatByLevel( pLevelInfo ) );

	Proc_End:

		return hr;
	}

	HRESULT UserGamePlayerInfoSystem::UpdateStatByLevel(conspiracy::LevelTbl::LevelItem *pLevelInfo)
	{
		HRESULT hr = S_OK;

		svrAssert(pLevelInfo->Level == m_Level);

		//// Update maximum stamina
		//m_MaxStamina = pLevelInfo->MaxStamina;

		//// Update maximum friend slot
		//m_MaxFriend = pLevelInfo->MaxFriend + m_FriendSlot;

	Proc_End:

		return hr;
	}


	HRESULT UserGamePlayerInfoSystem::SavePurchaseInfoToDB(TransactionID transID, const Array<BYTE>& purchaseID, const char* purchasePlatform, const char* purchaseToken)
	{
		if (purchasePlatform == nullptr || purchaseToken == nullptr)
			return E_SYSTEM_INVALIDARG;

		return Svr::GetServerComponent<DB::GameConspiracyDB>()->SavePurchaseInfoToDB(transID, GetOwner().GetShardID(), GetOwner().GetPlayerID(),
			GetLevel(), GetExp(),
			GetGameMoney(), GetGem(), GetStamina(), GetAddedFriendSlot(),
			purchaseID,
			purchasePlatform, purchaseToken, 
			GetOwner().GetLatestActiveTime(),
			GetOwner().GetLatestUpdateTime());
	}

	HRESULT UserGamePlayerInfoSystem::SavePlayerInfoToDB(TransactionID transID)
	{
		return Svr::GetServerComponent<DB::GameConspiracyDB>()->SetPlayerInfoCmd(transID, GetOwner().GetShardID(), GetOwner().GetPlayerID(),
			GetLevel(), GetExp(),
			GetGameMoney(), GetGem(), GetStamina(), GetAddedFriendSlot(),
			GetTotalPlayed(),
			GetWinPlaySCitizen(), GetWinPlaySMonster(), GetWinPlaySSeer(),
			GetLosePlaySCitizen(), GetLosePlaySMonster(), GetLosePlaySSeer(),
			GetWinPlayNCitizen(), GetWinPlayNMonster(), GetWinPlayNSeer(),
			GetLosePlayNCitizen(), GetLosePlayNMonster(), GetLosePlayNSeer(),
			GetOwner().GetLatestActiveTime(),
			GetOwner().GetLatestUpdateTime());
	}

}; // namespace GameServer
}; // namespace BR







