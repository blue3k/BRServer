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
 
	BotTalkTbl BotTalkTbl::m_Instance;
	BotTalkTbl::TableMap BotTalkTbl::m_TableMap;

	HRESULT BotTalkTbl::LoadTable( const std::list<BotTalkTblItem>& rowList )
	{
 
		for( auto rowItem : rowList )
		{
 			auto* pBotTalkTblItem = new BotTalkTbl::BotTalkTblItem;
			*pBotTalkTblItem = rowItem;
			BotTalkTbl::m_TableMap.insert(std::make_pair(pBotTalkTblItem->ItemID, pBotTalkTblItem));
		}
		return S_OK;
	}

	HRESULT BotTalkTbl::ClearTable()
	{
 		for (TableMapItr itr = m_TableMap.begin(); itr != m_TableMap.end(); ++itr)
			delete itr->second;

		m_TableMap.clear();
		return S_OK;
	}

	HRESULT BotTalkTbl::FindItem( const int& Key, BotTalkTblItem*& pRow)
	{
 		TableMapItr itr = m_TableMap.find(Key);
		if (itr == m_TableMap.end())
		{
 			// write error log
			return E_FAIL;
		}
		pRow = itr->second;
		return S_OK;
	}

}; // namespace conspiracy


