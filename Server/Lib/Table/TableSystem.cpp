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
#include "Common/Typedefs.h"
#include "Common/BrXML.h"
#include "Common/Trace.h"
#include "Common/ResultCode/BRResultCodeSvr.h"
#include "Common/ResultCode/BRResultCodeDB.h"
#include "Common/TimeUtil.h"

#include "TableSystem.h"
#include "DB/DataSource.h"
#include "DB/Factory.h"
#include "DB/Session.h"
#include "conspiracy/RewardTbl.h"
#include "conspiracy/LevelTbl.h"
#include "conspiracy/ShopTbl.h"
#include "conspiracy/GameConfigTbl.h"
#include "conspiracy/OrganicTbl.h"
#include "ServerSystem/SvrConfig.h"
#include "TableQueries.h"



namespace BR {
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

		HRESULT Initialize(const std::string& strConnectionString, const std::string& strDBName, const std::string& strUserID, const std::string& strPassword)
		{
			HRESULT	hr = S_SYSTEM_OK;

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

		HRESULT Terminate()
		{
			HRESULT	hr = S_SYSTEM_OK;

			if (m_pDataSource != nullptr)
			{
				m_pDataSource->CloseDBSource();
				delete m_pDataSource;
			}

		//Proc_End:

			return hr;
		}

		template<class TableQueryType, class TableClass>
		HRESULT QueryTable()
		{
			HRESULT	hr = S_SYSTEM_OK;
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

		HRESULT QueryTableVersion()
		{
			HRESULT	hr = S_SYSTEM_OK;
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


		HRESULT Update()
		{
			HRESULT hr = S_SYSTEM_OK;

			if (!IsInitialized())
				return S_SYSTEM_OK;

			if (!m_Timer.CheckTimer())
				return S_SYSTEM_OK;

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
	HRESULT InitializeTable()
	{
		HRESULT hr = S_SYSTEM_OK;
		auto found = Svr::Config::GetConfig().DBInstances.begin();
		auto tableDB = Svr::Config::GetConfig().TableDB;
		defChkPtr(tableDB);

		found = Svr::Config::GetConfig().DBInstances.find(tableDB->DBInstanceName);
		if (found == Svr::Config::GetConfig().DBInstances.end())
		{
			defErr(E_DB_INVALID_CONFIG);
		}

		{
			auto tableDBInstance = found->second;
			defTrace(Trace::TRC_TRACE, "Loading tables from {0}/{1}", tableDBInstance->ConnectionString, tableDB->DBName);
			defChk(TableSystem::stm_Instance.Initialize(tableDBInstance->ConnectionString, tableDB->DBName, tableDBInstance->UserID, tableDBInstance->Password));
		}

		defChk(TableSystem::stm_Instance.QueryTableVersion());

		defChk(LoadTables());

	Proc_End:

		return hr;
	}

	// Release all tables
	HRESULT TerminateTable()
	{
		TableSystem::stm_Instance.Terminate();
		return S_SYSTEM_OK;
	}


	HRESULT LoadTables()
	{
		HRESULT hr = S_SYSTEM_OK;

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

	HRESULT Update()
	{
		return TableSystem::stm_Instance.Update();
	}


}; // namespace conspiracy
}



