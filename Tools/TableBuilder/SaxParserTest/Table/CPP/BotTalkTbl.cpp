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
#include "Common/Typedefs.h"
#include "BotTalkTbl.h"



namespace conspiracy
{
 
	BotTalkTbl::ItemIDTable *BotTalkTbl::m_ItemIDTable = nullptr;
	BotTalkTbl::ItemIDTable *BotTalkTbl::m_ItemIDTablePrev = nullptr;

	HRESULT BotTalkTbl::LoadTable( const std::list<BotTalkTblItem>& rowList )
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
		return S_SYSTEM_OK;
	}


	HRESULT BotTalkTbl::FindItem( const int& Key, BotTalkTblItem*& pRow)
	{
 		auto itr = m_ItemIDTable->find(Key);
		if (itr == m_ItemIDTable->end())
		{
 			// write error log
			return E_SYSTEM_FAIL;
		}
		pRow = itr->second;
		return S_SYSTEM_OK;
	}

}; // namespace conspiracy


