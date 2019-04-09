////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2015 The Braves Co.
// 
// Author : Generated
// 
// Description : TableVersionTbl implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "TablePCH.h"
#include "SFTypedefs.h"
#include "TableVersionTbl.h"
#include "Memory/SFMemory.h"



namespace conspiracy
{
	using namespace SF;
 
	TableVersionTbl::TableVersionTable *TableVersionTbl::m_TableVersionTable = nullptr;
	TableVersionTbl::TableVersionTable *TableVersionTbl::m_TableVersionTablePrev = nullptr;

	Result TableVersionTbl::LoadTable( const std::list<TableVersionItem>& rowList )
	{
 		auto pNewTableVersionTable = new(GetSystemHeap()) TableVersionTable;

		for( auto rowItem : rowList )
		{
 			auto* pTableVersionItem = new(GetSystemHeap()) TableVersionTbl::TableVersionItem;
			*pTableVersionItem = rowItem;
			pNewTableVersionTable->insert(std::make_pair(pTableVersionItem->TableVersion, pTableVersionItem));
		}

		if (m_TableVersionTablePrev != nullptr)
		{
 			for( auto itItem : *m_TableVersionTablePrev) { IHeap::Delete(itItem.second); } ;
			IHeap::Delete(m_TableVersionTablePrev);
		}
		m_TableVersionTablePrev = m_TableVersionTable;
		m_TableVersionTable = pNewTableVersionTable;
		return ResultCode::SUCCESS;
	}


	Result TableVersionTbl::FindItem( const int& Key, TableVersionItem*& pRow)
	{
 		auto itr = m_TableVersionTable->find(Key);
		if (itr == m_TableVersionTable->end())
		{
 			// write error log
			return ResultCode::FAIL;
		}
		pRow = itr->second;
		return ResultCode::SUCCESS;
	}

}; // namespace conspiracy


