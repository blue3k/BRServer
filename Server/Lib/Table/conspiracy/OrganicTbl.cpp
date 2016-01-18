////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2015 The Braves Co.
// 
// Author : Generated
// 
// Description : OrganicTbl implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/Typedefs.h"
#include "OrganicTbl.h"



namespace conspiracy
{
 
	// EItemEffect structure defition
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

	OrganicTbl::EItemEffect::operator int () const
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
 		EnumMapItr itr =  m_Map.find(sz);
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

	OrganicTbl OrganicTbl::m_Instance;
	OrganicTbl::TableMap OrganicTbl::m_TableMap;

	HRESULT OrganicTbl::LoadTable( const std::list<OrganicItem>& rowList )
	{
 
		for( auto rowItem : rowList )
		{
 			auto* pOrganicItem = new OrganicTbl::OrganicItem;
			*pOrganicItem = rowItem;
			OrganicTbl::m_TableMap.insert(std::make_pair(pOrganicItem->ItemEffect, pOrganicItem));
		}
		return S_OK;
	}

	HRESULT OrganicTbl::ClearTable()
	{
 		for (TableMapItr itr = m_TableMap.begin(); itr != m_TableMap.end(); ++itr)
			delete itr->second;

		m_TableMap.clear();
		return S_OK;
	}

	HRESULT OrganicTbl::FindItem( const unsigned int& Key, OrganicItem*& pRow)
	{
 		TableMapItr itr = m_TableMap.find(Key);
		if (itr == m_TableMap.end())
		{
 			// write error log
			return E_SYSTEM_FAIL;
		}
		pRow = itr->second;
		return S_OK;
	}

}; // namespace conspiracy


