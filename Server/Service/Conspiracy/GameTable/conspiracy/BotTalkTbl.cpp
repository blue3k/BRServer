////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2015 The Braves Co.
// 
// Author : Generated
// 
// Description : BotTalkTbl implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "TablePCH.h"
#include "SFTypedefs.h"
#include "BotTalkTbl.h"
#include "Memory/SFMemory.h"
#include "Variable/SFVariableTable.h"


namespace SF {
	namespace conspiracy
	{
		using namespace SF;

		BotTalkTbl::ItemIDTable* BotTalkTbl::m_ItemIDTable = nullptr;
		BotTalkTbl::ItemIDTable* BotTalkTbl::m_ItemIDTablePrev = nullptr;

		Result BotTalkTbl::LoadTable(const Array<VariableTable>& rowList)
		{
			auto pNewItemIDTable = new(GetSystemHeap()) ItemIDTable;

			for (auto rowItem : rowList)
			{
				auto* pBotTalkTblItem = new(GetSystemHeap()) BotTalkTbl::BotTalkTblItem;

				pBotTalkTblItem->ItemID = rowItem.GetValue<int>("ItemID"_crc);
				pBotTalkTblItem->BotName_Begin = rowItem.GetValue<int>("BotName_Begin"_crc);
				pBotTalkTblItem->BotName_End = rowItem.GetValue<int>("BotName_End"_crc);
				pBotTalkTblItem->FirstDay_Begin = rowItem.GetValue<int>("FirstDay_Begin"_crc);
				pBotTalkTblItem->FirstDay_End = rowItem.GetValue<int>("FirstDay_End"_crc);
				pBotTalkTblItem->FirstDay_PlayDelay = rowItem.GetValue<float>("FirstDay_PlayDelay"_crc);
				pBotTalkTblItem->FirstDay_BotDelay1 = rowItem.GetValue<float>("FirstDay_BotDelay1"_crc);
				pBotTalkTblItem->FirstDay_BotDelay2 = rowItem.GetValue<float>("FirstDay_BotDelay2"_crc);
				pBotTalkTblItem->FreeTalk_Begin = rowItem.GetValue<int>("FreeTalk_Begin"_crc);
				pBotTalkTblItem->FreeTalk_End = rowItem.GetValue<int>("FreeTalk_End"_crc);
				pBotTalkTblItem->FreeTalk_PlayDelay = rowItem.GetValue<float>("FreeTalk_PlayDelay"_crc);
				pBotTalkTblItem->FreeTalk_BotDelay1 = rowItem.GetValue<float>("FreeTalk_BotDelay1"_crc);
				pBotTalkTblItem->FreeTalk_BotDelay2 = rowItem.GetValue<float>("FreeTalk_BotDelay2"_crc);
				pBotTalkTblItem->Defense_Begin = rowItem.GetValue<int>("Defense_Begin"_crc);
				pBotTalkTblItem->Defense_End = rowItem.GetValue<int>("Defense_End"_crc);
				pBotTalkTblItem->Defense_PlayDelay = rowItem.GetValue<float>("Defense_PlayDelay"_crc);
				pBotTalkTblItem->Defense_BotDelay1 = rowItem.GetValue<float>("Defense_BotDelay1"_crc);
				pBotTalkTblItem->Defense_BotDelay2 = rowItem.GetValue<float>("Defense_BotDelay2"_crc);
				pBotTalkTblItem->GameResultWinner_Begin = rowItem.GetValue<int>("GameResultWinner_Begin"_crc);
				pBotTalkTblItem->GameResultWinner_End = rowItem.GetValue<int>("GameResultWinner_End"_crc);
				pBotTalkTblItem->GameResultLoser_Begin = rowItem.GetValue<int>("GameResultLoser_Begin"_crc);
				pBotTalkTblItem->GameResultLoser_End = rowItem.GetValue<int>("GameResultLoser_End"_crc);

				pNewItemIDTable->insert(std::make_pair(pBotTalkTblItem->ItemID, pBotTalkTblItem));
			}

			if (m_ItemIDTablePrev != nullptr)
			{
				for (auto itItem : *m_ItemIDTablePrev) { IHeap::Delete(itItem.second); };
				IHeap::Delete(m_ItemIDTablePrev);
			}
			m_ItemIDTablePrev = m_ItemIDTable;
			m_ItemIDTable = pNewItemIDTable;
			return ResultCode::SUCCESS;
		}


		Result BotTalkTbl::FindItem(const int& Key, BotTalkTblItem*& pRow)
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
}

