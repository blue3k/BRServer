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

#include "SFTypedefs.h"
#include "ResultCode/SFResultCodeSystem.h"



namespace conspiracy
{
	using namespace SF;

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
		static Result LoadTable( const std::list<TableVersionItem>& rowList );

		static Result FindItem( const int& Key, TableVersionItem*& pRow);
	}; // class TableVersionTbl
}; // namespace conspiracy


