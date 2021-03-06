﻿////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2015 The Braves Co.
// 
// Author : Generated
// 
// Description : OrganicTbl implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "TablePCH.h"
#include "SFTypedefs.h"
#include "OrganicTbl.h"
#include "MemoryManager/SFMemory.h"
#include "Variable/SFVariableTable.h"


namespace SF {
	namespace conspiracy
	{
		using namespace SF;

		// EItemEffect structure definition
		OrganicTbl::EItemEffect::EnumMap OrganicTbl::EItemEffect::m_Map;

		OrganicTbl::EItemEffect::EnumMap::EnumMap()
		{
			m_Map.insert(std::make_pair("ResetRankNormal", EItemEffect::Enum::ResetRankNormal));
			m_Map.insert(std::make_pair("NickName", EItemEffect::Enum::NickName));
			m_Map.insert(std::make_pair("ReStart", EItemEffect::Enum::ReStart));
			m_Map.insert(std::make_pair("Revival", EItemEffect::Enum::Revival));
			m_Map.insert(std::make_pair("Check", EItemEffect::Enum::Check));
			m_Map.insert(std::make_pair("RoleChoice", EItemEffect::Enum::RoleChoice));
		}

		OrganicTbl::EItemEffect::Enum OrganicTbl::EItemEffect::operator =(int i)
		{
			m_Enum = AsEnum((Enum)i);
			return m_Enum;
		}

		OrganicTbl::EItemEffect::operator int() const
		{
			return (int)m_Enum;
		}

		OrganicTbl::EItemEffect::operator Enum () const
		{
			return m_Enum;
		}

		OrganicTbl::EItemEffect::Enum OrganicTbl::EItemEffect::operator =(const char* sz)
		{
			m_Enum = AsEnum(sz);
			return m_Enum;
		}

		bool OrganicTbl::EItemEffect::operator ==(const char* sz)
		{
			return m_Enum == AsEnum(sz);
		}

		OrganicTbl::EItemEffect::operator const char* () const
		{
			return AsName(m_Enum);
		}

		OrganicTbl::EItemEffect::Enum OrganicTbl::EItemEffect::AsEnum(const char* sz)
		{
			EnumMapItr itr = m_Map.find(sz);
			if (itr == m_Map.end())
				return EItemEffect::Enum::INVALID_ENUM;

			return itr->second;
		}

		OrganicTbl::EItemEffect::Enum OrganicTbl::EItemEffect::AsEnum(Enum e)
		{
			for (EnumMapItr itr = m_Map.begin(); itr != m_Map.end(); ++itr)
			{
				if (itr->second == e)
					return itr->second;
			}
			return EItemEffect::Enum::INVALID_ENUM;
		}

		const char* OrganicTbl::EItemEffect::AsName(Enum e)
		{
			for (EnumMapItr itr = m_Map.begin(); itr != m_Map.end(); ++itr)
			{
				if (itr->second == e)
					return itr->first.c_str();
			}
			return "INVALID_ENUM";
		}

		OrganicTbl::ItemEffectTable* OrganicTbl::m_ItemEffectTable = nullptr;
		OrganicTbl::ItemEffectTable* OrganicTbl::m_ItemEffectTablePrev = nullptr;

		Result OrganicTbl::LoadTable(const Array<VariableTable>& rowList)
		{
			auto pNewItemEffectTable = new(GetSystemHeap()) ItemEffectTable;

			for (auto rowItem : rowList)
			{
				auto* pOrganicItem = new(GetSystemHeap()) OrganicTbl::OrganicItem;
				pOrganicItem->OrganicItemID = rowItem.GetValue<int>("OrganicItemID"_crc);
				StrUtil::StringCopy(pOrganicItem->ItemInfo, rowItem.GetValue<const char*>("ItemInfo"_crc));
				pOrganicItem->RequiredGem = rowItem.GetValue<int>("RequiredGem"_crc);
				pOrganicItem->RequiredGameMoney = rowItem.GetValue<int>("RequiredGameMoney"_crc);

				pNewItemEffectTable->insert(std::make_pair(pOrganicItem->ItemEffect, pOrganicItem));
			}

			if (m_ItemEffectTablePrev != nullptr)
			{
				for (auto itItem : *m_ItemEffectTablePrev) { IHeap::Delete(itItem.second); };
				IHeap::Delete(m_ItemEffectTablePrev);
			}
			m_ItemEffectTablePrev = m_ItemEffectTable;
			m_ItemEffectTable = pNewItemEffectTable;
			return ResultCode::SUCCESS;
		}


		Result OrganicTbl::FindItem(const unsigned int& Key, OrganicItem*& pRow)
		{
			auto itr = m_ItemEffectTable->find(Key);
			if (itr == m_ItemEffectTable->end())
			{
				// write error log
				return ResultCode::FAIL;
			}
			pRow = itr->second;
			return ResultCode::SUCCESS;
		}

	} // namespace conspiracy
}

