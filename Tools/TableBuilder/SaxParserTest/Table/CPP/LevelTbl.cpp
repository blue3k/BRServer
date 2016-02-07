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
#include "Common/Typedefs.h"
#include "LevelTbl.h"



namespace conspiracy
{
 
	LevelTbl::LevelTable *LevelTbl::m_LevelTable = nullptr;
	LevelTbl::LevelTable *LevelTbl::m_LevelTablePrev = nullptr;

	HRESULT LevelTbl::LoadTable( const std::list<LevelItem>& rowList )
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
		return S_SYSTEM_OK;
	}


	HRESULT LevelTbl::FindItem( const int& Key, LevelItem*& pRow)
	{
 		auto itr = m_LevelTable->find(Key);
		if (itr == m_LevelTable->end())
		{
 			// write error log
			return E_SYSTEM_FAIL;
		}
		pRow = itr->second;
		return S_SYSTEM_OK;
	}

}; // namespace conspiracy


