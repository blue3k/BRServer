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
 
	LevelTbl LevelTbl::m_Instance;
	LevelTbl::TableMap LevelTbl::m_TableMap;

	HRESULT LevelTbl::LoadTable( const std::list<LevelItem>& rowList )
	{
 
		for( auto rowItem : rowList )
		{
 			auto* pLevelItem = new LevelTbl::LevelItem;
			*pLevelItem = rowItem;
			LevelTbl::m_TableMap.insert(std::make_pair(pLevelItem->Level, pLevelItem));
		}
		return S_OK;
	}

	HRESULT LevelTbl::ClearTable()
	{
 		for (TableMapItr itr = m_TableMap.begin(); itr != m_TableMap.end(); ++itr)
			delete itr->second;

		m_TableMap.clear();
		return S_OK;
	}

	HRESULT LevelTbl::FindItem( const int& Key, LevelItem*& pRow)
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


