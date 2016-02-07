////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2015 The Braves Co.
// 
// Author : Generated
// 
// Description : TableVersionTbl implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/Typedefs.h"
#include "TableVersionTbl.h"



namespace conspiracy
{
 
	TableVersionTbl::TableVersionTable *TableVersionTbl::m_TableVersionTable = nullptr;
	TableVersionTbl::TableVersionTable *TableVersionTbl::m_TableVersionTablePrev = nullptr;

	HRESULT TableVersionTbl::LoadTable( const std::list<TableVersionItem>& rowList )
	{
 		auto pNewTableVersionTable = new TableVersionTable;

		for( auto rowItem : rowList )
		{
 			auto* pTableVersionItem = new TableVersionTbl::TableVersionItem;
			*pTableVersionItem = rowItem;
			pNewTableVersionTable->insert(std::make_pair(pTableVersionItem->TableVersion, pTableVersionItem));
		}

		if (m_TableVersionTablePrev != nullptr)
		{
 			for( auto itItem : *m_TableVersionTablePrev) { delete itItem.second; } ;
			delete m_TableVersionTablePrev;
		}
		m_TableVersionTablePrev = m_TableVersionTable;
		m_TableVersionTable = pNewTableVersionTable;
		return S_SYSTEM_OK;
	}


	HRESULT TableVersionTbl::FindItem( const int& Key, TableVersionItem*& pRow)
	{
 		auto itr = m_TableVersionTable->find(Key);
		if (itr == m_TableVersionTable->end())
		{
 			// write error log
			return E_SYSTEM_FAIL;
		}
		pRow = itr->second;
		return S_SYSTEM_OK;
	}

}; // namespace conspiracy


