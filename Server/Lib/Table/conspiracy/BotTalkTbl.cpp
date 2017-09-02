////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2015 The Braves Co.
// 
// Author : Generated
// 
// Description : BotTalkTbl implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "SFTypedefs.h"
#include "BotTalkTbl.h"



namespace conspiracy
{
	using namespace SF;
 
	BotTalkTbl::ItemIDTable *BotTalkTbl::m_ItemIDTable = nullptr;
	BotTalkTbl::ItemIDTable *BotTalkTbl::m_ItemIDTablePrev = nullptr;

	Result BotTalkTbl::LoadTable( const std::list<BotTalkTblItem>& rowList )
	{
 		auto pNewItemIDTable = new ItemIDTable;

		for( auto rowItem : rowList )
		{
 			auto* pBotTalkTblItem = new BotTalkTbl::BotTalkTblItem;
			*pBotTalkTblItem = rowItem;
			pNewItemIDTable->insert(std::make_pair(pBotTalkTblItem->ItemID, pBotTalkTblItem));
		}

		if (m_ItemIDTablePrev != nullptr)
		{
 			for( auto itItem : *m_ItemIDTablePrev) { delete itItem.second; } ;
			delete m_ItemIDTablePrev;
		}
		m_ItemIDTablePrev = m_ItemIDTable;
		m_ItemIDTable = pNewItemIDTable;
		return ResultCode::SUCCESS;
	}


	Result BotTalkTbl::FindItem( const int& Key, BotTalkTblItem*& pRow)
	{
 		auto itr = m_ItemIDTable->find(Key);
		if (itr == m_ItemIDTable->end())
		{
 			// write error log
			return ResultCode::FAIL;
		}
		pRow = itr->second;
		return ResultCode::SUCCESS;
	}

}; // namespace conspiracy


