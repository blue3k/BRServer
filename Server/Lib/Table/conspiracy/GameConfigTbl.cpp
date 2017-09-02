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
#include "SFTypedefs.h"
#include "GameConfigTbl.h"



namespace conspiracy
{
 
	using namespace SF;

	GameConfigTbl::PresetIDTable *GameConfigTbl::m_PresetIDTable = nullptr;
	GameConfigTbl::PresetIDTable *GameConfigTbl::m_PresetIDTablePrev = nullptr;

	Result GameConfigTbl::LoadTable( const std::list<GameConfigItem>& rowList )
	{
 		auto pNewPresetIDTable = new PresetIDTable;

		for( auto rowItem : rowList )
		{
 			auto* pGameConfigItem = new GameConfigTbl::GameConfigItem;
			*pGameConfigItem = rowItem;
			pNewPresetIDTable->insert(std::make_pair(pGameConfigItem->PresetID, pGameConfigItem));
		}

		if (m_PresetIDTablePrev != nullptr)
		{
 			for( auto itItem : *m_PresetIDTablePrev) { delete itItem.second; } ;
			delete m_PresetIDTablePrev;
		}
		m_PresetIDTablePrev = m_PresetIDTable;
		m_PresetIDTable = pNewPresetIDTable;
		return ResultCode::SUCCESS;
	}


	Result GameConfigTbl::FindItem( const int& Key, GameConfigItem*& pRow)
	{
 		auto itr = m_PresetIDTable->find(Key);
		if (itr == m_PresetIDTable->end())
		{
 			// write error log
			return ResultCode::FAIL;
		}
		pRow = itr->second;
		return ResultCode::SUCCESS;
	}

}; // namespace conspiracy


