﻿////////////////////////////////////////////////////////////////////////////////
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
#include "MemoryManager/SFMemory.h"
#include "Variable/SFVariableTable.h"


namespace SF {
	namespace conspiracy
	{

		LevelTbl::LevelTable* LevelTbl::m_LevelTable = nullptr;
		LevelTbl::LevelTable* LevelTbl::m_LevelTablePrev = nullptr;

		Result LevelTbl::LoadTable(const Array<VariableTable>& rowList)
		{
			auto pNewLevelTable = new(GetSystemHeap()) LevelTable;

			for (auto rowItem : rowList)
			{
				auto* pLevelItem = new(GetSystemHeap()) LevelTbl::LevelItem;
				pLevelItem->Level = rowItem.GetValue<int>("Level"_crc);
				pLevelItem->RequiredExpTotal = rowItem.GetValue<int>("RequiredExpTotal"_crc);

				pNewLevelTable->insert(std::make_pair(pLevelItem->Level, pLevelItem));
			}

			if (m_LevelTablePrev != nullptr)
			{
				for (auto itItem : *m_LevelTablePrev) { IHeap::Delete(itItem.second); };
				IHeap::Delete(m_LevelTablePrev);
			}
			m_LevelTablePrev = m_LevelTable;
			m_LevelTable = pNewLevelTable;
			return ResultCode::SUCCESS;
		}


		Result LevelTbl::FindItem(const int& Key, LevelItem*& pRow)
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

	} // namespace conspiracy
}

