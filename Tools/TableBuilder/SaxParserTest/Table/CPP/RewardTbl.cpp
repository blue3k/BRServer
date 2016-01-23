////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2015 The Braves Co.
// 
// Author : Generated
// 
// Description : RewardTbl implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/Typedefs.h"
#include "RewardTbl.h"



namespace conspiracy
{
 
	// ERole structure defition
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

	RewardTbl::ERole::operator int () const
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
 		EnumMapItr itr =  m_Map.find(sz);
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

	RewardTbl RewardTbl::m_Instance;
	RewardTbl::TableMap RewardTbl::m_TableMap;

	HRESULT RewardTbl::LoadTable( const std::list<RewardItem>& rowList )
	{
 
		for( auto rowItem : rowList )
		{
 			auto* pRewardItem = new RewardTbl::RewardItem;
			*pRewardItem = rowItem;
			RewardTbl::m_TableMap.insert(std::make_pair(pRewardItem->Role, pRewardItem));
		}
		return S_SYSTEM_OK;
	}

	HRESULT RewardTbl::ClearTable()
	{
 		for (TableMapItr itr = m_TableMap.begin(); itr != m_TableMap.end(); ++itr)
			delete itr->second;

		m_TableMap.clear();
		return S_SYSTEM_OK;
	}

	HRESULT RewardTbl::FindItem( const unsigned int& Key, RewardItem*& pRow)
	{
 		TableMapItr itr = m_TableMap.find(Key);
		if (itr == m_TableMap.end())
		{
 			// write error log
			return E_SYSTEM_FAIL;
		}
		pRow = itr->second;
		return S_SYSTEM_OK;
	}

}; // namespace conspiracy


