
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

#include "Table/conspiracy/GameConfigTbl.h"
#include "Table/conspiracy/LevelTbl.h"
#include "Table/conspiracy/OrganicTbl.h"
#include "Table/conspiracy/ShopTbl.h"
#include "Table/conspiracy/RewardTbl.h"
#include "Table/conspiracy/BotTalkTbl.h"
#include "Table/conspiracy/TableVersionTbl.h"


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
		BRDB_BEGIN_PARAM_MAP(QueryTableVersionTbl,1)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_OUTPUT)
			BRDB_COLUMN_ENTRY(TableVersion)
		BRDB_END_PARAM_MAP()

		BRDB_QUERYSTRING("spGetTableVersion", BRDB_PARAM_1)
	};

	BRDB_DEFINE_QUERYCLASS(PROTOCOLID_TABLEDB, QueryTableVersionTbl);



	class QueryGameConfigTbl : public conspiracy::GameConfigTbl::GameConfigItem, public QueryBase
	{
	public:
		int32_t Dummy;


	public:
		BRDB_BEGIN_EMPTY_PARAM_MAP(QueryGameConfigTbl)
		BRDB_END_EMPTY_PARAM_MAP()

		BRDB_BEGIN_RESULT_MAP(QueryGameConfigTbl, 25)
		BRDB_COLUMN_ENTRY(PresetID)
		BRDB_COLUMN_ENTRY(DefaultMail)
		BRDB_COLUMN_ENTRY(DefaultGem)
		BRDB_COLUMN_ENTRY(MaxGem)
		BRDB_COLUMN_ENTRY(DefaultMoney)
		BRDB_COLUMN_ENTRY(MaxMoney)
		BRDB_COLUMN_ENTRY(DefaultFriend)
		BRDB_COLUMN_ENTRY(MaxFriend)
		BRDB_COLUMN_ENTRY(DefaultStamina)
		BRDB_COLUMN_ENTRY(MaxStamina)
		BRDB_COLUMN_ENTRY(MaxAutoRefilStamina)
		BRDB_COLUMN_ENTRY(StaminaRecoveryTime)
		BRDB_COLUMN_ENTRY(StaminaForGame)
		BRDB_COLUMN_ENTRY(FreeDiscussion)
		BRDB_COLUMN_ENTRY(RolePlayTime)
		BRDB_COLUMN_ENTRY(RolePlayAndKillingTime)
		BRDB_COLUMN_ENTRY(FirstVote)
		BRDB_COLUMN_ENTRY(DefenceTime)
		BRDB_COLUMN_ENTRY(SecondVote)
		BRDB_COLUMN_ENTRY(MorningDiscussion)
		BRDB_COLUMN_ENTRY(ResultNoticeTime)
		BRDB_COLUMN_ENTRY(PlayerDisplayOffline)
		BRDB_COLUMN_ENTRY(MaxCharacterSet)
		BRDB_COLUMN_ENTRY(ForceProgressCount)
		BRDB_COLUMN_ENTRY(PlayerAutoLogout)
		BRDB_END_RESULT_MAP()


		BRDB_RAW_QUERY_STRING("select * from gtbl_gameconfigtable;")
	};

	BRDB_DEFINE_ROWSETQUERYCLASS(PROTOCOLID_TABLEDB, QueryGameConfigTbl, conspiracy::GameConfigTbl::GameConfigItem);


	class QueryLevelTbl : public conspiracy::LevelTbl::LevelItem, public QueryBase
	{
	public:
		int32_t Dummy;


	public:
		BRDB_BEGIN_EMPTY_PARAM_MAP(QueryLevelTbl)
		BRDB_END_EMPTY_PARAM_MAP()

		BRDB_BEGIN_RESULT_MAP(QueryLevelTbl, 2)
		BRDB_COLUMN_ENTRY(Level)
		BRDB_COLUMN_ENTRY(RequiredExpTotal)
		BRDB_END_RESULT_MAP()


		BRDB_RAW_QUERY_STRING("select * from gtbl_leveltable;")
	};

	BRDB_DEFINE_ROWSETQUERYCLASS(PROTOCOLID_TABLEDB, QueryLevelTbl, conspiracy::LevelTbl::LevelItem);




	class QueryOrganicTbl : public conspiracy::OrganicTbl::OrganicItem, public QueryBase
	{
	public:
		int32_t Dummy;


	public:
		BRDB_BEGIN_EMPTY_PARAM_MAP(QueryOrganicTbl)
		BRDB_END_EMPTY_PARAM_MAP()

		BRDB_BEGIN_RESULT_MAP(QueryOrganicTbl, 5)
		BRDB_COLUMN_ENTRY(OrganicItemID)
		BRDB_COLUMN_ENTRY(ItemInfo)
		BRDB_COLUMN_ENTRY(RequiredGem)
		BRDB_COLUMN_ENTRY(RequiredGameMoney)
		BRDB_COLUMN_ENTRY(ItemEffect.m_iEnum)
		BRDB_END_RESULT_MAP()


		BRDB_RAW_QUERY_STRING("select * from gtbl_organictable;")
	};

	BRDB_DEFINE_ROWSETQUERYCLASS(PROTOCOLID_TABLEDB, QueryOrganicTbl, conspiracy::OrganicTbl::OrganicItem);




	class QueryShopTbl : public conspiracy::ShopTbl::ShopItem, public QueryBase
	{
	public:
		int32_t Dummy;


	public:
		BRDB_BEGIN_EMPTY_PARAM_MAP(QueryShopTbl)
		BRDB_END_EMPTY_PARAM_MAP()

		BRDB_BEGIN_RESULT_MAP(QueryShopTbl, 9)
		BRDB_COLUMN_ENTRY(ShopItemID)
		BRDB_COLUMN_ENTRY(ItemInfo)
		BRDB_COLUMN_ENTRY(RequiredCash)
		BRDB_COLUMN_ENTRY(RequiredGem)
		BRDB_COLUMN_ENTRY(RequiredGameMoney)
		BRDB_COLUMN_ENTRY(ItemEffect.m_iEnum)
		BRDB_COLUMN_ENTRY(Quantity)
		BRDB_COLUMN_ENTRY(AndroidItemID)
		BRDB_COLUMN_ENTRY(iOSItemID)
		BRDB_END_RESULT_MAP()


		BRDB_RAW_QUERY_STRING("select * from gtbl_shoptable;")
	};

	BRDB_DEFINE_ROWSETQUERYCLASS(PROTOCOLID_TABLEDB, QueryShopTbl, conspiracy::ShopTbl::ShopItem);




	class QueryRewardTbl : public conspiracy::RewardTbl::RewardItem, public QueryBase
	{
	public:
		int32_t Dummy;


	public:
		BRDB_BEGIN_EMPTY_PARAM_MAP(QueryRewardTbl)
		BRDB_END_EMPTY_PARAM_MAP()

		BRDB_BEGIN_RESULT_MAP(QueryRewardTbl, 5)
		BRDB_COLUMN_ENTRY(Role.m_iEnum)
		BRDB_COLUMN_ENTRY(WinExp)
		BRDB_COLUMN_ENTRY(LoseExp)
		BRDB_COLUMN_ENTRY(WinMoney)
		BRDB_COLUMN_ENTRY(LoseMoney)
		BRDB_END_RESULT_MAP()


		BRDB_RAW_QUERY_STRING("select * from gtbl_rewardtable;")
	};

	BRDB_DEFINE_ROWSETQUERYCLASS(PROTOCOLID_TABLEDB, QueryRewardTbl, conspiracy::RewardTbl::RewardItem);



	class QueryBotTalkTable : public conspiracy::BotTalkTbl::BotTalkTblItem, public QueryBase
	{
	public:
		int32_t Dummy;


	public:
		BRDB_BEGIN_EMPTY_PARAM_MAP(QueryBotTalkTable)
			BRDB_END_EMPTY_PARAM_MAP()

			BRDB_BEGIN_RESULT_MAP(QueryBotTalkTable, 22)
				BRDB_COLUMN_ENTRY(ItemID)
				BRDB_COLUMN_ENTRY(BotName_Begin)
				BRDB_COLUMN_ENTRY(BotName_End)
				BRDB_COLUMN_ENTRY(FirstDay_Begin)
				BRDB_COLUMN_ENTRY(FirstDay_End)
				BRDB_COLUMN_ENTRY(FirstDay_PlayDelay)
				BRDB_COLUMN_ENTRY(FirstDay_BotDelay1)
				BRDB_COLUMN_ENTRY(FirstDay_BotDelay2)
				BRDB_COLUMN_ENTRY(FreeTalk_Begin)
				BRDB_COLUMN_ENTRY(FreeTalk_End)
				BRDB_COLUMN_ENTRY(FreeTalk_PlayDelay)
				BRDB_COLUMN_ENTRY(FreeTalk_BotDelay1)
				BRDB_COLUMN_ENTRY(FreeTalk_BotDelay2)
				BRDB_COLUMN_ENTRY(Defense_Begin)
				BRDB_COLUMN_ENTRY(Defense_End)
				BRDB_COLUMN_ENTRY(Defense_PlayDelay)
				BRDB_COLUMN_ENTRY(Defense_BotDelay1)
				BRDB_COLUMN_ENTRY(Defense_BotDelay2)
				BRDB_COLUMN_ENTRY(GameResultWinner_Begin)
				BRDB_COLUMN_ENTRY(GameResultWinner_End)
				BRDB_COLUMN_ENTRY(GameResultLoser_Begin)
				BRDB_COLUMN_ENTRY(GameResultLoser_End)
			BRDB_END_RESULT_MAP()

			BRDB_RAW_QUERY_STRING("select * from gtbl_bottalktable;")
	};

	BRDB_DEFINE_ROWSETQUERYCLASS(PROTOCOLID_TABLEDB, QueryBotTalkTable, conspiracy::BotTalkTbl::BotTalkTblItem);




}  // namespace DB
}  // namespace SF
