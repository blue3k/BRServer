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

#include "SFTypedefs.h"
#include "ResultCode/SFResultCodeSystem.h"
#include "Container/SFArray.h"

namespace SF {

	class VariableTable;

	namespace conspiracy
	{
		using namespace SF;

		class BotTalkTbl
		{
		public:

			// BotTalkTblItem structure definition
			struct BotTalkTblItem
			{
				int	ItemID;
				int	BotName_Begin;
				int	BotName_End;
				int	FirstDay_Begin;
				int	FirstDay_End;
				float	FirstDay_PlayDelay;
				float	FirstDay_BotDelay1;
				float	FirstDay_BotDelay2;
				int	FreeTalk_Begin;
				int	FreeTalk_End;
				float	FreeTalk_PlayDelay;
				float	FreeTalk_BotDelay1;
				float	FreeTalk_BotDelay2;
				int	Defense_Begin;
				int	Defense_End;
				float	Defense_PlayDelay;
				float	Defense_BotDelay1;
				float	Defense_BotDelay2;
				int	GameResultWinner_Begin;
				int	GameResultWinner_End;
				int	GameResultLoser_Begin;
				int	GameResultLoser_End;

			}; // struct BotTalkTblItem

			typedef std::unordered_map<int, BotTalkTblItem*> ItemIDTable;
			typedef ItemIDTable::iterator ItemIDTableItr;
			static ItemIDTable* m_ItemIDTable;
			static ItemIDTable* m_ItemIDTablePrev;



			// declare member function
			static Result LoadTable(const Array<VariableTable>& rowList);

			static Result FindItem(const int& Key, BotTalkTblItem*& pRow);
		}; // class BotTalkTbl
	} // namespace conspiracy
}

