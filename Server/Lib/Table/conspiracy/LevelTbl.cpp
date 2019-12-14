////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2015 The Braves Co.
// 
// Author : Generated
// 
// Description : LevelTbl implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "TablePCH.h"
#include "SFTypedefs.h"
#include "LevelTbl.h"
#include "Memory/SFMemory.h"



namespace conspiracy
{
	using namespace SF;
 
	LevelTbl::LevelTable *LevelTbl::m_LevelTable = nullptr;
	LevelTbl::LevelTable *LevelTbl::m_LevelTablePrev = nullptr;

	Result LevelTbl::LoadTable( const Array<LevelItem>& rowList )
	{
 		auto pNewLevelTable = new(GetSystemHeap()) LevelTable;

		for( auto rowItem : rowList )
		{
 			auto* pLevelItem = new(GetSystemHeap()) LevelTbl::LevelItem;
			*pLevelItem = rowItem;
			pNewLevelTable->insert(std::make_pair(pLevelItem->Level, pLevelItem));
		}

		if (m_LevelTablePrev != nullptr)
		{
 			for( auto itItem : *m_LevelTablePrev) { IHeap::Delete(itItem.second); } ;
			IHeap::Delete(m_LevelTablePrev);
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


