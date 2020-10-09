
////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2015 The Braves
// 
// Author: Kyungkun Ko
//
// Description : Table DB queries
//
////////////////////////////////////////////////////////////////////////////////


#pragma once


#include "DB/Query.h"
#include "DB/DBConst.h"
#include "DB/QueryConst.h"

#include "conspiracy/GameConfigTbl.h"
#include "conspiracy/LevelTbl.h"
#include "conspiracy/OrganicTbl.h"
#include "conspiracy/ShopTbl.h"
#include "conspiracy/RewardTbl.h"
#include "conspiracy/BotTalkTbl.h"
#include "conspiracy/TableVersionTbl.h"


namespace SF {
namespace DB {
	
	
	///////////////////////////////////////////////////////////////////////////////////////
	//
	//	Query Message Code
	//

	enum MsgCode
	{
		MCODE_QueryNull,
		MCODE_QueryTableVersionTbl,
		MCODE_QueryGameConfigTbl,
		MCODE_QueryLevelTbl,
		MCODE_QueryOrganicTbl,
		MCODE_QueryShopTbl,
		MCODE_QueryRewardTbl,
		MCODE_QueryBotTalkTable,
	}; // enum MsgCode



	class QueryTableVersionTbl : public QueryBase
	{
	public:
		int32_t TableVersion;


	public:
		BRDB_BEGIN_PARAM_MAP(QueryTableVersionTbl, "spGetTableVersion")
			BRDB_PARAM_ENTRY(ParamIO::Output, TableVersion)
		BRDB_END_PARAM_MAP()
	};

	BRDB_DEFINE_QUERYCLASS(PROTOCOLID_TABLEDB, QueryTableVersionTbl);



	class QueryGameConfigTbl : public QueryBase
	{
	public:
		int32_t Dummy;


	public:
		BRDB_RAW_QUERY(QueryGameConfigTbl, "select * from gtbl_gameconfigtable;")

		//BRDB_BEGIN_RESULT_MAP(QueryGameConfigTbl)
		//BRDB_COLUMN_ENTRY(PresetID)
		//BRDB_COLUMN_ENTRY(DefaultMail)
		//BRDB_COLUMN_ENTRY(DefaultGem)
		//BRDB_COLUMN_ENTRY(MaxGem)
		//BRDB_COLUMN_ENTRY(DefaultMoney)
		//BRDB_COLUMN_ENTRY(MaxMoney)
		//BRDB_COLUMN_ENTRY(DefaultFriend)
		//BRDB_COLUMN_ENTRY(MaxFriend)
		//BRDB_COLUMN_ENTRY(DefaultStamina)
		//BRDB_COLUMN_ENTRY(MaxStamina)
		//BRDB_COLUMN_ENTRY(MaxAutoRefilStamina)
		//BRDB_COLUMN_ENTRY(StaminaRecoveryTime)
		//BRDB_COLUMN_ENTRY(StaminaForGame)
		//BRDB_COLUMN_ENTRY(FreeDiscussion)
		//BRDB_COLUMN_ENTRY(RolePlayTime)
		//BRDB_COLUMN_ENTRY(RolePlayAndKillingTime)
		//BRDB_COLUMN_ENTRY(FirstVote)
		//BRDB_COLUMN_ENTRY(DefenceTime)
		//BRDB_COLUMN_ENTRY(SecondVote)
		//BRDB_COLUMN_ENTRY(MorningDiscussion)
		//BRDB_COLUMN_ENTRY(ResultNoticeTime)
		//BRDB_COLUMN_ENTRY(PlayerDisplayOffline)
		//BRDB_COLUMN_ENTRY(MaxCharacterSet)
		//BRDB_COLUMN_ENTRY(ForceProgressCount)
		//BRDB_COLUMN_ENTRY(PlayerAutoLogout)
		//BRDB_END_RESULT_MAP()

	};

	BRDB_DEFINE_ROWSETQUERYCLASS(PROTOCOLID_TABLEDB, QueryGameConfigTbl);


	class QueryLevelTbl : public QueryBase
	{
	public:
		int32_t Dummy;


	public:
		BRDB_RAW_QUERY(QueryLevelTbl, "select * from gtbl_leveltable;")

		//BRDB_BEGIN_RESULT_MAP(QueryLevelTbl)
		//BRDB_COLUMN_ENTRY(Level)
		//BRDB_COLUMN_ENTRY(RequiredExpTotal)
		//BRDB_END_RESULT_MAP()

	};

	BRDB_DEFINE_ROWSETQUERYCLASS(PROTOCOLID_TABLEDB, QueryLevelTbl);




	class QueryOrganicTbl : public QueryBase
	{
	public:
		int32_t Dummy;


	public:
		BRDB_RAW_QUERY(QueryOrganicTbl, "select * from gtbl_organictable;")

		//BRDB_BEGIN_RESULT_MAP(QueryOrganicTbl)
		//	BRDB_COLUMN_ENTRY(OrganicItemID)
		//	BRDB_COLUMN_ENTRY(ItemInfo)
		//	BRDB_COLUMN_ENTRY(RequiredGem)
		//	BRDB_COLUMN_ENTRY(RequiredGameMoney)
		//	BRDB_COLUMN_ENTRY(ItemEffect.m_iEnum)
		//BRDB_END_RESULT_MAP()

	};

	BRDB_DEFINE_ROWSETQUERYCLASS(PROTOCOLID_TABLEDB, QueryOrganicTbl);




	class QueryShopTbl : public QueryBase
	{
	public:
		int32_t Dummy;


	public:
		BRDB_RAW_QUERY(QueryShopTbl, "select * from gtbl_shoptable;")

		//BRDB_BEGIN_RESULT_MAP(QueryShopTbl)
		//	BRDB_COLUMN_ENTRY(ShopItemID)
		//	BRDB_COLUMN_ENTRY(ItemInfo)
		//	BRDB_COLUMN_ENTRY(RequiredCash)
		//	BRDB_COLUMN_ENTRY(RequiredGem)
		//	BRDB_COLUMN_ENTRY(RequiredGameMoney)
		//	BRDB_COLUMN_ENTRY(ItemEffect.m_iEnum)
		//	BRDB_COLUMN_ENTRY(Quantity)
		//	BRDB_COLUMN_ENTRY(AndroidItemID)
		//	BRDB_COLUMN_ENTRY(iOSItemID)
		//BRDB_END_RESULT_MAP()

	};

	BRDB_DEFINE_ROWSETQUERYCLASS(PROTOCOLID_TABLEDB, QueryShopTbl);




	class QueryRewardTbl : public conspiracy::RewardTbl::RewardItem, public QueryBase
	{
	public:
		int32_t Dummy;


	public:
		BRDB_RAW_QUERY(QueryRewardTbl, "select * from gtbl_rewardtable;")

		//BRDB_BEGIN_RESULT_MAP(QueryRewardTbl)
		//BRDB_COLUMN_ENTRY(Role.m_iEnum)
		//BRDB_COLUMN_ENTRY(WinExp)
		//BRDB_COLUMN_ENTRY(LoseExp)
		//BRDB_COLUMN_ENTRY(WinMoney)
		//BRDB_COLUMN_ENTRY(LoseMoney)
		//BRDB_END_RESULT_MAP()

	};

	BRDB_DEFINE_ROWSETQUERYCLASS(PROTOCOLID_TABLEDB, QueryRewardTbl);



	class QueryBotTalkTable : public QueryBase
	{
	public:
		int32_t Dummy;


	public:
		BRDB_RAW_QUERY(QueryBotTalkTable, "select * from gtbl_bottalktable;")

		//BRDB_BEGIN_RESULT_MAP(QueryBotTalkTable)
		//	BRDB_COLUMN_ENTRY(ItemID)
		//	BRDB_COLUMN_ENTRY(BotName_Begin)
		//	BRDB_COLUMN_ENTRY(BotName_End)
		//	BRDB_COLUMN_ENTRY(FirstDay_Begin)
		//	BRDB_COLUMN_ENTRY(FirstDay_End)
		//	BRDB_COLUMN_ENTRY(FirstDay_PlayDelay)
		//	BRDB_COLUMN_ENTRY(FirstDay_BotDelay1)
		//	BRDB_COLUMN_ENTRY(FirstDay_BotDelay2)
		//	BRDB_COLUMN_ENTRY(FreeTalk_Begin)
		//	BRDB_COLUMN_ENTRY(FreeTalk_End)
		//	BRDB_COLUMN_ENTRY(FreeTalk_PlayDelay)
		//	BRDB_COLUMN_ENTRY(FreeTalk_BotDelay1)
		//	BRDB_COLUMN_ENTRY(FreeTalk_BotDelay2)
		//	BRDB_COLUMN_ENTRY(Defense_Begin)
		//	BRDB_COLUMN_ENTRY(Defense_End)
		//	BRDB_COLUMN_ENTRY(Defense_PlayDelay)
		//	BRDB_COLUMN_ENTRY(Defense_BotDelay1)
		//	BRDB_COLUMN_ENTRY(Defense_BotDelay2)
		//	BRDB_COLUMN_ENTRY(GameResultWinner_Begin)
		//	BRDB_COLUMN_ENTRY(GameResultWinner_End)
		//	BRDB_COLUMN_ENTRY(GameResultLoser_Begin)
		//	BRDB_COLUMN_ENTRY(GameResultLoser_End)
		//BRDB_END_RESULT_MAP()

	};

	BRDB_DEFINE_ROWSETQUERYCLASS(PROTOCOLID_TABLEDB, QueryBotTalkTable);




}  // namespace DB
}  // namespace SF
