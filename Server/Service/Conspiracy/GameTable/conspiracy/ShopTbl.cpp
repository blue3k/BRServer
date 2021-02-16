////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2015 The Braves Co.
// 
// Author : Generated
// 
// Description : ShopTbl implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "TablePCH.h"
#include "SFTypedefs.h"
#include "ShopTbl.h"
#include "MemoryManager/SFMemory.h"
#include "Variable/SFVariableTable.h"


namespace SF {
	namespace conspiracy
	{

		// EItemEffect structure definition
		ShopTbl::EItemEffect::EnumMap ShopTbl::EItemEffect::m_Map;

		ShopTbl::EItemEffect::EnumMap::EnumMap()
		{
			m_Map.insert(std::make_pair("Gem", EItemEffect::Enum::Gem));
			m_Map.insert(std::make_pair("Stamina", EItemEffect::Enum::Stamina));
			m_Map.insert(std::make_pair("FriendSlot", EItemEffect::Enum::FriendSlot));
		}

		ShopTbl::EItemEffect::Enum ShopTbl::EItemEffect::operator =(int i)
		{
			m_Enum = AsEnum((Enum)i);
			return m_Enum;
		}

		ShopTbl::EItemEffect::operator int() const
		{
			return (int)m_Enum;
		}

		ShopTbl::EItemEffect::operator Enum () const
		{
			return m_Enum;
		}

		ShopTbl::EItemEffect::Enum ShopTbl::EItemEffect::operator =(const char* sz)
		{
			m_Enum = AsEnum(sz);
			return m_Enum;
		}

		bool ShopTbl::EItemEffect::operator ==(const char* sz)
		{
			return m_Enum == AsEnum(sz);
		}

		ShopTbl::EItemEffect::operator const char* () const
		{
			return AsName(m_Enum);
		}

		ShopTbl::EItemEffect::Enum ShopTbl::EItemEffect::AsEnum(const char* sz)
		{
			EnumMapItr itr = m_Map.find(sz);
			if (itr == m_Map.end())
				return EItemEffect::Enum::INVALID_ENUM;

			return itr->second;
		}

		ShopTbl::EItemEffect::Enum ShopTbl::EItemEffect::AsEnum(Enum e)
		{
			for (EnumMapItr itr = m_Map.begin(); itr != m_Map.end(); ++itr)
			{
				if (itr->second == e)
					return itr->second;
			}
			return EItemEffect::Enum::INVALID_ENUM;
		}

		const char* ShopTbl::EItemEffect::AsName(Enum e)
		{
			for (EnumMapItr itr = m_Map.begin(); itr != m_Map.end(); ++itr)
			{
				if (itr->second == e)
					return itr->first.c_str();
			}
			return "INVALID_ENUM";
		}

		ShopTbl::ShopItemIDTable* ShopTbl::m_ShopItemIDTable = nullptr;
		ShopTbl::ShopItemIDTable* ShopTbl::m_ShopItemIDTablePrev = nullptr;

		Result ShopTbl::LoadTable(const Array<VariableTable>& rowList)
		{
			auto pNewShopItemIDTable = new(GetSystemHeap()) ShopItemIDTable;

			for (auto rowItem : rowList)
			{
				auto* pShopItem = new(GetSystemHeap()) ShopTbl::ShopItem;
				pShopItem->ShopItemID = rowItem.GetValue<int>("ShopItemID"_crc);
				StrUtil::StringCopy(pShopItem->ItemInfo, rowItem.GetValue<const char*>("ItemInfo"_crc));
				StrUtil::StringCopy(pShopItem->RequiredCash, rowItem.GetValue<const char*>("RequiredCash"_crc));
				pShopItem->RequiredGem = rowItem.GetValue<int>("RequiredGem"_crc);
				pShopItem->RequiredGameMoney = rowItem.GetValue<int>("RequiredGameMoney"_crc);
				pShopItem->ItemEffect = rowItem.GetValue<int>("ItemEffect"_crc);
				pShopItem->Quantity = rowItem.GetValue<int>("Quantity"_crc);
				StrUtil::StringCopy(pShopItem->AndroidItemID, rowItem.GetValue<const char*>("AndroidItemID"_crc));
				StrUtil::StringCopy(pShopItem->iOSItemID, rowItem.GetValue<const char*>("iOSItemID"_crc));

				pNewShopItemIDTable->insert(std::make_pair(pShopItem->ShopItemID, pShopItem));
			}

			if (m_ShopItemIDTablePrev != nullptr)
			{
				for (auto itItem : *m_ShopItemIDTablePrev) { IHeap::Delete(itItem.second); };
				IHeap::Delete(m_ShopItemIDTablePrev);
			}
			m_ShopItemIDTablePrev = m_ShopItemIDTable;
			m_ShopItemIDTable = pNewShopItemIDTable;
			return ResultCode::SUCCESS;
		}


		Result ShopTbl::FindItem(const int& Key, ShopItem*& pRow)
		{
			auto itr = m_ShopItemIDTable->find(Key);
			if (itr == m_ShopItemIDTable->end())
			{
				// write error log
				return ResultCode::FAIL;
			}
			pRow = itr->second;
			return ResultCode::SUCCESS;
		}

	} // namespace conspiracy
}

