////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2015 The Braves Co.
// 
// Author : Generated
// 
// Description : BotTalkTbl  definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Common/Typedefs.h"



namespace conspiracy
{
 	class BotTalkTbl
	{
 	public:

		// BotTalkTblItem structure defition
		struct BotTalkTblItem
		{
 			int	ItemID;
			int	BotName_Begin;
			int	BotName_End;
			int	FirstDay_Begin;
			int	FirstDay_End;
			int	FreeTalk_Begin;
			int	FreeTalk_End;
			int	Defense_Begin;
			int	Defense_End;
			int	GameResultWinner_Begin;
			int	GameResultWinner_End;
			int	GameResultLoser_Begin;
			int	GameResultLoser_End;

		}; // struct BotTalkTblItem

		typedef std::unordered_map<int, BotTalkTblItem*> TableMap;
		typedef TableMap::iterator TableMapItr;
		static TableMap m_TableMap;
		static BotTalkTbl m_Instance;



		// declare member function
		static HRESULT LoadTable( const std::list<BotTalkTblItem>& rowList );
		static HRESULT ClearTable();

		static HRESULT FindItem( const int& Key, BotTalkTblItem*& pRow);
	}; // class BotTalkTbl
}; // namespace conspiracy


