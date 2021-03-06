﻿////////////////////////////////////////////////////////////////////////////////
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

	RewardTbl::RoleTable *RewardTbl::m_RoleTable = nullptr;
	RewardTbl::RoleTable *RewardTbl::m_RoleTablePrev = nullptr;

	BR::Result RewardTbl::LoadTable( const std::list<RewardItem>& rowList )
	{
 		auto pNewRoleTable = new RoleTable;

		for( auto rowItem : rowList )
		{
 			auto* pRewardItem = new RewardTbl::RewardItem;
			*pRewardItem = rowItem;
			pNewRoleTable->insert(std::make_pair(pRewardItem->Role, pRewardItem));
		}

		if (m_RoleTablePrev != nullptr)
		{
 			for( auto itItem : *m_RoleTablePrev) { delete itItem.second; } ;
			delete m_RoleTablePrev;
		}
		m_RoleTablePrev = m_RoleTable;
		m_RoleTable = pNewRoleTable;
		return BR::ResultCode::SUCCESS;
	}


	BR::Result RewardTbl::FindItem( const unsigned int& Key, RewardItem*& pRow)
	{
 		auto itr = m_RoleTable->find(Key);
		if (itr == m_RoleTable->end())
		{
 			// write error log
			return BR::ResultCode::FAIL;
		}
		pRow = itr->second;
		return BR::ResultCode::SUCCESS;
	}

}; // namespace conspiracy


