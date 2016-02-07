////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2015 The Braves Co.
// 
// Author : Generated
// 
// Description : TableVersionTbl  definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Common/Typedefs.h"
#include "Common/ResultCode/BRResultCodeSystem.h"



namespace conspiracy
{
 	class TableVersionTbl
	{
 	public:

		// TableVersionItem structure defition
		struct TableVersionItem
		{
 			int	TableVersion;

		}; // struct TableVersionItem

		typedef std::unordered_map<int, TableVersionItem*> TableVersionTable;
		typedef TableVersionTable::iterator TableVersionTableItr;
		static TableVersionTable *m_TableVersionTable;
		static TableVersionTable *m_TableVersionTablePrev;



		// declare member function
		static HRESULT LoadTable( const std::list<TableVersionItem>& rowList );

		static HRESULT FindItem( const int& Key, TableVersionItem*& pRow);
	}; // class TableVersionTbl
}; // namespace conspiracy


