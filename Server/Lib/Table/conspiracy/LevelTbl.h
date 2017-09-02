////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2015 The Braves Co.
// 
// Author : Generated
// 
// Description : LevelTbl  definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "SFTypedefs.h"
#include "ResultCode/SFResultCodeSystem.h"



namespace conspiracy
{
	using namespace SF;

 	class LevelTbl
	{
 	public:

		// LevelItem structure defition
		struct LevelItem
		{
 			int	Level;
			int	RequiredExpTotal;

		}; // struct LevelItem

		typedef std::unordered_map<int, LevelItem*> LevelTable;
		typedef LevelTable::iterator LevelTableItr;
		static LevelTable *m_LevelTable;
		static LevelTable *m_LevelTablePrev;



		// declare member function
		static Result LoadTable( const std::list<LevelItem>& rowList );

		static Result FindItem( const int& Key, LevelItem*& pRow);
	}; // class LevelTbl
}; // namespace conspiracy


