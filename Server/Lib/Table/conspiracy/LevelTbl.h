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

#include "Common/Typedefs.h"



namespace conspiracy
{
 	class LevelTbl
	{
 	public:

		// LevelItem structure defition
		struct LevelItem
		{
 			int	Level;
			int	RequiredExpTotal;

		}; // struct LevelItem

		typedef std::unordered_map<int, LevelItem*> TableMap;
		typedef TableMap::iterator TableMapItr;
		static TableMap m_TableMap;
		static LevelTbl m_Instance;



		// declare member function
		static HRESULT LoadTable( const std::list<LevelItem>& rowList );
		static HRESULT ClearTable();

		static HRESULT FindItem( const int& Key, LevelItem*& pRow);
	}; // class LevelTbl
}; // namespace conspiracy


