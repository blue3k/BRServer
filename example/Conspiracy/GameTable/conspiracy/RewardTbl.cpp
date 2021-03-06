﻿////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2015 The Braves Co.
// 
// Author : Generated
// 
// Description : RewardTbl implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "TablePCH.h"
#include "SFTypedefs.h"
#include "RewardTbl.h"
#include "MemoryManager/SFMemory.h"
#include "Variable/SFVariableTable.h"


namespace SF {
	namespace conspiracy
	{
		using namespace SF;

		// ERole structure definition
		RewardTbl::ERole::EnumMap RewardTbl::ERole::m_Map;

		RewardTbl::ERole::EnumMap::EnumMap()
		{
			m_Map.insert(std::make_pair("Villager", ERole::Enum::Villager));
			m_Map.insert(std::make_pair("Monster", ERole::Enum::Monster));
			m_Map.insert(std::make_pair("Seer", ERole::Enum::Seer));
		}

		RewardTbl::ERole::Enum RewardTbl::ERole::operator =(int i)
		{
			m_Enum = AsEnum((Enum)i);
			return m_Enum;
		}

		RewardTbl::ERole::operator int() const
		{
			return (int)m_Enum;
		}

		RewardTbl::ERole::operator Enum () const
		{
			return m_Enum;
		}

		RewardTbl::ERole::Enum RewardTbl::ERole::operator =(const char* sz)
		{
			m_Enum = AsEnum(sz);
			return m_Enum;
		}

		bool RewardTbl::ERole::operator ==(const char* sz)
		{
			return m_Enum == AsEnum(sz);
		}

		RewardTbl::ERole::operator const char* () const
		{
			return AsName(m_Enum);
		}

		RewardTbl::ERole::Enum RewardTbl::ERole::AsEnum(const char* sz)
		{
			EnumMapItr itr = m_Map.find(sz);
			if (itr == m_Map.end())
				return ERole::Enum::INVALID_ENUM;

			return itr->second;
		}

		RewardTbl::ERole::Enum RewardTbl::ERole::AsEnum(Enum e)
		{
			for (EnumMapItr itr = m_Map.begin(); itr != m_Map.end(); ++itr)
			{
				if (itr->second == e)
					return itr->second;
			}
			return ERole::Enum::INVALID_ENUM;
		}

		const char* RewardTbl::ERole::AsName(Enum e)
		{
			for (EnumMapItr itr = m_Map.begin(); itr != m_Map.end(); ++itr)
			{
				if (itr->second == e)
					return itr->first.c_str();
			}
			return "INVALID_ENUM";
		}

		RewardTbl::RoleTable* RewardTbl::m_RoleTable = nullptr;
		RewardTbl::RoleTable* RewardTbl::m_RoleTablePrev = nullptr;

		Result RewardTbl::LoadTable(const Array<VariableTable>& rowList)
		{
			auto pNewRoleTable = new(GetSystemHeap()) RoleTable;

			for (auto rowItem : rowList)
			{
				auto* pRewardItem = new(GetSystemHeap()) RewardTbl::RewardItem;
				pRewardItem->Role = rowItem.GetValue<int>("Role"_crc);
				pRewardItem->WinExp = rowItem.GetValue<int>("WinExp"_crc);
				pRewardItem->LoseExp = rowItem.GetValue<int>("LoseExp"_crc);
				pRewardItem->WinMoney = rowItem.GetValue<int>("WinMoney"_crc);
				pRewardItem->LoseMoney = rowItem.GetValue<int>("LoseMoney"_crc);

				pNewRoleTable->insert(std::make_pair(pRewardItem->Role, pRewardItem));
			}

			if (m_RoleTablePrev != nullptr)
			{
				for (auto itItem : *m_RoleTablePrev) { IHeap::Delete(itItem.second); };
				IHeap::Delete(m_RoleTablePrev);
			}
			m_RoleTablePrev = m_RoleTable;
			m_RoleTable = pNewRoleTable;
			return ResultCode::SUCCESS;
		}


		Result RewardTbl::FindItem(const unsigned int& Key, RewardItem*& pRow)
		{
			auto itr = m_RoleTable->find(Key);
			if (itr == m_RoleTable->end())
			{
				// write error log
				return ResultCode::FAIL;
			}
			pRow = itr->second;
			return ResultCode::SUCCESS;
		}

	} // namespace conspiracy
}

