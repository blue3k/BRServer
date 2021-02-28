////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2015 The Braves Co.
// 
// Author : Generated
// 
// Description : GameConfigTbl implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "TablePCH.h"
#include "SFTypedefs.h"
#include "GameConfigTbl.h"
#include "MemoryManager/SFMemory.h"
#include "Variable/SFVariableTable.h"


namespace SF {
	namespace conspiracy
	{

		using namespace SF;

		GameConfigTbl::PresetIDTable* GameConfigTbl::m_PresetIDTable = nullptr;
		GameConfigTbl::PresetIDTable* GameConfigTbl::m_PresetIDTablePrev = nullptr;

		Result GameConfigTbl::LoadTable(const Array<VariableTable>& rowList)
		{
			auto pNewPresetIDTable = new(GetSystemHeap()) PresetIDTable;

			for (auto rowItem : rowList)
			{
				auto* pGameConfigItem = new(GetSystemHeap()) GameConfigTbl::GameConfigItem;
				pGameConfigItem->PresetID = rowItem.GetValue<int>("PresetID"_crc);
				pGameConfigItem->DefaultMail = rowItem.GetValue<int>("DefaultMail"_crc);
				pGameConfigItem->DefaultGem = rowItem.GetValue<int>("DefaultGem"_crc);
				pGameConfigItem->MaxGem = rowItem.GetValue<int>("MaxGem"_crc);
				pGameConfigItem->DefaultMoney = rowItem.GetValue<int>("DefaultMoney"_crc);
				pGameConfigItem->MaxMoney = rowItem.GetValue<int>("MaxMoney"_crc);
				pGameConfigItem->DefaultFriend = rowItem.GetValue<int>("DefaultFriend"_crc);
				pGameConfigItem->MaxFriend = rowItem.GetValue<int>("MaxFriend"_crc);
				pGameConfigItem->DefaultStamina = rowItem.GetValue<int>("DefaultStamina"_crc);
				pGameConfigItem->MaxStamina = rowItem.GetValue<int>("MaxStamina"_crc);
				pGameConfigItem->MaxAutoRefilStamina = rowItem.GetValue<int>("MaxAutoRefilStamina"_crc);
				pGameConfigItem->StaminaRecoveryTime = rowItem.GetValue<int>("StaminaRecoveryTime"_crc);
				pGameConfigItem->StaminaForGame = rowItem.GetValue<int>("StaminaForGame"_crc);
				pGameConfigItem->FreeDiscussion = rowItem.GetValue<int>("FreeDiscussion"_crc);
				pGameConfigItem->RolePlayTime = rowItem.GetValue<int>("RolePlayTime"_crc);
				pGameConfigItem->RolePlayAndKillingTime = rowItem.GetValue<int>("RolePlayAndKillingTime"_crc);
				pGameConfigItem->FirstVote = rowItem.GetValue<int>("FirstVote"_crc);
				pGameConfigItem->DefenceTime = rowItem.GetValue<int>("DefenceTime"_crc);
				pGameConfigItem->SecondVote = rowItem.GetValue<int>("SecondVote"_crc);
				pGameConfigItem->MorningDiscussion = rowItem.GetValue<int>("MorningDiscussion"_crc);
				pGameConfigItem->ResultNoticeTime = rowItem.GetValue<int>("ResultNoticeTime"_crc);
				pGameConfigItem->PlayerDisplayOffline = rowItem.GetValue<int>("PlayerDisplayOffline"_crc);
				pGameConfigItem->MaxCharacterSet = rowItem.GetValue<int>("MaxCharacterSet"_crc);
				pGameConfigItem->ForceProgressCount = rowItem.GetValue<int>("ForceProgressCount"_crc);
				pGameConfigItem->PlayerAutoLogout = rowItem.GetValue<int>("PlayerAutoLogout"_crc);

				pNewPresetIDTable->insert(std::make_pair(pGameConfigItem->PresetID, pGameConfigItem));
			}

			if (m_PresetIDTablePrev != nullptr)
			{
				for (auto itItem : *m_PresetIDTablePrev) { IHeap::Delete(itItem.second); };
				IHeap::Delete(m_PresetIDTablePrev);
			}
			m_PresetIDTablePrev = m_PresetIDTable;
			m_PresetIDTable = pNewPresetIDTable;
			return ResultCode::SUCCESS;
		}


		Result GameConfigTbl::FindItem(const int& Key, GameConfigItem*& pRow)
		{
			auto itr = m_PresetIDTable->find(Key);
			if (itr == m_PresetIDTable->end())
			{
				// write error log
				return ResultCode::FAIL;
			}
			pRow = itr->second;
			return ResultCode::SUCCESS;
		}

	} // namespace conspiracy
}

