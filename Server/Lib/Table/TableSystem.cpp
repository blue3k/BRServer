////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves Co.
// 
// Author : Madk
// 
// Description : Table system definitions
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "SFTypedefs.h"
#include "ServerLog/SvrLog.h"
#include "ResultCode/SFResultCodeSvr.h"
#include "ResultCode/SFResultCodeDB.h"
#include "Util/TimeUtil.h"

#include "TableSystem.h"
#include "DB/DataSource.h"
#include "DB/Factory.h"
#include "DB/Session.h"
#include "conspiracy/RewardTbl.h"
#include "conspiracy/LevelTbl.h"
#include "conspiracy/ShopTbl.h"
#include "conspiracy/GameConfigTbl.h"
#include "conspiracy/OrganicTbl.h"
#include "ServerConfig/SFServerConfig.h"
#include "TableQueries.h"



namespace SF {
namespace GameTable {


	class TableSystem
	{
	public:
		static TableSystem stm_Instance;

		// Update table every 30sec
		const int UPDATE_TIME_SEC = 10;

	private:
		DB::DataSource* m_pDataSource;

		int m_TableVersion;

		DurationMS m_UpdateTimeDuration;
		Util::TimeStampTimer m_Timer;


	public:

		int GetTableVersion() { return m_TableVersion; }

		bool IsInitialized() { return m_pDataSource != nullptr; }

		Result Initialize(const String& strConnectionString, const String& strDBName, const String& strUserID, const String& strPassword)
		{
			Result	hr = ResultCode::SUCCESS;

			// Update table every ...
			m_UpdateTimeDuration = DurationMS(UPDATE_TIME_SEC*1000);
			m_Timer.SetTimer(m_UpdateTimeDuration);

			m_TableVersion = -1;

			if (m_pDataSource == nullptr)
			{
				defChk(DB::Factory::GetInstance().CreateDataSource(m_pDataSource));
			}

			defChk(m_pDataSource->InitializeDBSource(strConnectionString, strDBName, strUserID, strPassword));

		Proc_End:

			return hr;
		}

		Result Terminate()
		{
			Result	hr = ResultCode::SUCCESS;

			if (m_pDataSource != nullptr)
			{
				m_pDataSource->CloseDBSource();
				delete m_pDataSource;
			}

		//Proc_End:

			return hr;
		}

		template<class TableQueryType, class TableClass>
		Result QueryTable()
		{
			Result	hr = ResultCode::SUCCESS;
			DB::Session* pSession = nullptr;
			TableQueryType* pTblQuery = nullptr;

			defChkPtr(m_pDataSource);

			defChk(m_pDataSource->AssignSession(pSession));

			defChk(pSession->OpenSession());

			defChkPtr(pTblQuery = new TableQueryType());
			pTblQuery->Dummy = 0;
			defChk(pSession->SendQuery(pTblQuery));

			TableClass::LoadTable(pTblQuery->m_RowsetResult);

		Proc_End:

			if (pSession)
				pSession->ReleaseSession();

			return hr;
		}

		Result QueryTableVersion()
		{
			Result	hr = ResultCode::SUCCESS;
			DB::Session* pSession = nullptr;
			DB::QueryTableVersionTblCmd* pTblQuery = nullptr;

			defChkPtr(m_pDataSource);

			defChk(m_pDataSource->AssignSession(pSession));

			defChk(pSession->OpenSession());

			defChkPtr(pTblQuery = new DB::QueryTableVersionTblCmd);
			pTblQuery->TableVersion = 0;
			defChk(pSession->SendQuery(pTblQuery));

			m_TableVersion = pTblQuery->TableVersion;

		Proc_End:

			if (pSession)
				pSession->ReleaseSession();

			return hr;
		}


		Result Update()
		{
			Result hr = ResultCode::SUCCESS;

			if (!IsInitialized())
				return ResultCode::SUCCESS;

			if (!m_Timer.CheckTimer())
				return ResultCode::SUCCESS;

			int oldTableVersion = GetTableVersion();
			int tableVersion = -1;

			defChk(QueryTableVersion());

			tableVersion = GetTableVersion();
			if (tableVersion != oldTableVersion)
			{
				LoadTables();
			}

			m_Timer.SetTimer(m_UpdateTimeDuration);

		Proc_End:

			return hr;
		}

	};

	TableSystem TableSystem::stm_Instance;


	// Initialize tables
	Result InitializeTable(const ServerConfig* config)
	{
		Result hr = ResultCode::SUCCESS;
		auto tableDB = config->GetDBCluster("TableDB");
		if (tableDB == nullptr)
			return ResultCode::DB_INVALID_CONFIG;

		auto tableDBInstance = config->FindDBInstance(tableDB->DBInstanceName);
		if (tableDBInstance == nullptr)
			return ResultCode::DB_INVALID_CONFIG;

		defTrace(Trace::TRC_INFO, "Loading tables from {0}/{1}", tableDBInstance->ConnectionString, tableDB->DBName);
		defChk(TableSystem::stm_Instance.Initialize(tableDBInstance->ConnectionString, tableDB->DBName, tableDBInstance->UserID, tableDBInstance->Password));

		defChk(TableSystem::stm_Instance.QueryTableVersion());

		defChk(LoadTables());

	Proc_End:

		return hr;
	}

	// Release all tables
	Result TerminateTable()
	{
		TableSystem::stm_Instance.Terminate();
		return ResultCode::SUCCESS;
	}


	Result LoadTables()
	{
		Result hr = ResultCode::SUCCESS;

		hr = TableSystem::stm_Instance.QueryTable<DB::QueryOrganicTblCmd, conspiracy::OrganicTbl>();
		defChk(hr);

		hr = TableSystem::stm_Instance.QueryTable<DB::QueryRewardTblCmd, conspiracy::RewardTbl>();
		defChk(hr);

		hr = TableSystem::stm_Instance.QueryTable<DB::QueryLevelTblCmd, conspiracy::LevelTbl>();
		defChk(hr);

		hr = TableSystem::stm_Instance.QueryTable<DB::QueryShopTblCmd, conspiracy::ShopTbl>();
		defChk(hr);

		hr = TableSystem::stm_Instance.QueryTable<DB::QueryGameConfigTblCmd, conspiracy::GameConfigTbl>();
		defChk(hr);

		hr = TableSystem::stm_Instance.QueryTable<DB::QueryBotTalkTableCmd, conspiracy::BotTalkTbl>();
		defChk(hr);

	Proc_End:

		return hr;
	}

	int GetTableVersion()
	{
		return TableSystem::stm_Instance.GetTableVersion();
	}

	Result Update()
	{
		return TableSystem::stm_Instance.Update();
	}


}; // namespace conspiracy
}



