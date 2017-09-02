////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2015 The Braves Co.
// 
// Author : Generated
// 
// Description : LevelTbl implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "SFTypedefs.h"
#include "LevelTbl.h"



namespace conspiracy
{
	using namespace SF;
 
	LevelTbl::LevelTable *LevelTbl::m_LevelTable = nullptr;
	LevelTbl::LevelTable *LevelTbl::m_LevelTablePrev = nullptr;

	Result LevelTbl::LoadTable( const std::list<LevelItem>& rowList )
	{
 		auto pNewLevelTable = new LevelTable;

		for( auto rowItem : rowList )
		{
 			auto* pLevelItem = new LevelTbl::LevelItem;
			*pLevelItem = rowItem;
			pNewLevelTable->insert(std::make_pair(pLevelItem->Level, pLevelItem));
		}

		if (m_LevelTablePrev != nullptr)
		{
 			for( auto itItem : *m_LevelTablePrev) { delete itItem.second; } ;
			delete m_LevelTablePrev;
		}
		m_LevelTablePrev = m_LevelTable;
		m_LevelTable = pNewLevelTable;
		return ResultCode::SUCCESS;
	}


	Result LevelTbl::FindItem( const int& Key, LevelItem*& pRow)
	{
 		auto itr = m_LevelTable->find(Key);
		if (itr == m_LevelTable->end())
		{
 			// write error log
			return ResultCode::FAIL;
		}
		pRow = itr->second;
		return ResultCode::SUCCESS;
	}

}; // namespace conspiracy


