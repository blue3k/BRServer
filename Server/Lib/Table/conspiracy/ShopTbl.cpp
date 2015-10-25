﻿////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2015 The Braves Co.
// 
// Author : Generated
// 
// Description : ShopTbl implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/Typedefs.h"
#include "ShopTbl.h"



namespace conspiracy
{
 
	// EItemEffect structure defition
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

	ShopTbl::EItemEffect::operator int () const
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
 		EnumMapItr itr =  m_Map.find(sz);
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

	ShopTbl ShopTbl::m_Instance;
	ShopTbl::TableMap ShopTbl::m_TableMap;

	HRESULT ShopTbl::LoadTable( const std::list<ShopItem>& rowList )
	{
 
		for( auto rowItem : rowList )
		{
 			auto* pShopItem = new ShopTbl::ShopItem;
			*pShopItem = rowItem;
			ShopTbl::m_TableMap.insert(std::make_pair(pShopItem->ShopItemID, pShopItem));
		}
		return S_OK;
	}

	HRESULT ShopTbl::ClearTable()
	{
 		for (TableMapItr itr = m_TableMap.begin(); itr != m_TableMap.end(); ++itr)
			delete itr->second;

		m_TableMap.clear();
		return S_OK;
	}

	HRESULT ShopTbl::FindItem( const int& Key, ShopItem*& pRow)
	{
 		TableMapItr itr = m_TableMap.find(Key);
		if (itr == m_TableMap.end())
		{
 			// write error log
			return E_FAIL;
		}
		pRow = itr->second;
		return S_OK;
	}

}; // namespace conspiracy

