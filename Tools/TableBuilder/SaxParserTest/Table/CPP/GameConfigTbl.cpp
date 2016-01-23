////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2015 The Braves Co.
// 
// Author : Generated
// 
// Description : GameConfigTbl implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/Typedefs.h"
#include "GameConfigTbl.h"



namespace conspiracy
{
 
	GameConfigTbl GameConfigTbl::m_Instance;
	GameConfigTbl::TableMap GameConfigTbl::m_TableMap;

	HRESULT GameConfigTbl::LoadTable( const std::list<GameConfigItem>& rowList )
	{
 
		for( auto rowItem : rowList )
		{
 			auto* pGameConfigItem = new GameConfigTbl::GameConfigItem;
			*pGameConfigItem = rowItem;
			GameConfigTbl::m_TableMap.insert(std::make_pair(pGameConfigItem->PresetID, pGameConfigItem));
		}
		return S_SYSTEM_OK;
	}

	HRESULT GameConfigTbl::ClearTable()
	{
 		for (TableMapItr itr = m_TableMap.begin(); itr != m_TableMap.end(); ++itr)
			delete itr->second;

		m_TableMap.clear();
		return S_SYSTEM_OK;
	}

	HRESULT GameConfigTbl::FindItem( const int& Key, GameConfigItem*& pRow)
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


