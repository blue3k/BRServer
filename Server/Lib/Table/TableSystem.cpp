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
#include "Common/HRESSvrSys.h"
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

	private:
		DB::DataSource* m_pDataSource;


	public:

		HRESULT Initialize(const std::string& strConnectionString, const std::string& strDBName, const std::string& strUserID, const std::string& strPassword)
		{
			HRESULT	hr = S_OK;

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
			HRESULT	hr = S_OK;

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
			HRESULT	hr = S_OK;
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
	};

	TableSystem TableSystem::stm_Instance;


	// Initialize tables
	HRESULT InitializeTable()
	{
		HRESULT hr = S_OK;
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

			defChk(TableSystem::stm_Instance.Initialize(tableDBInstance->ConnectionString, tableDB->DBName, tableDBInstance->UserID, tableDBInstance->Password));
		}

		defChk(LoadTables());

	Proc_End:

		return hr;
	}

	// Release all tables
	HRESULT TerminateTable()
	{
		TableSystem::stm_Instance.Terminate();
		return S_OK;
	}


	HRESULT LoadTables()
	{
		HRESULT hr = S_OK;

		defChk(::conspiracy::OrganicTbl::ClearTable());
		hr = TableSystem::stm_Instance.QueryTable<DB::QueryOrganicTblCmd, conspiracy::OrganicTbl>();
		defChk(hr);

		defChk(::conspiracy::RewardTbl::ClearTable());
		hr = TableSystem::stm_Instance.QueryTable<DB::QueryRewardTblCmd, conspiracy::RewardTbl>();
		defChk(hr);

		defChk(::conspiracy::LevelTbl::ClearTable());
		hr = TableSystem::stm_Instance.QueryTable<DB::QueryLevelTblCmd, conspiracy::LevelTbl>();
		defChk(hr);

		defChk(::conspiracy::ShopTbl::ClearTable());
		hr = TableSystem::stm_Instance.QueryTable<DB::QueryShopTblCmd, conspiracy::ShopTbl>();
		defChk(hr);


		defChk(::conspiracy::GameConfigTbl::ClearTable());
		hr = TableSystem::stm_Instance.QueryTable<DB::QueryGameConfigTblCmd, conspiracy::GameConfigTbl>();
		defChk(hr);


		defChk(::conspiracy::BotTalkTbl::ClearTable());
		hr = TableSystem::stm_Instance.QueryTable<DB::QueryBotTalkTableCmd, conspiracy::BotTalkTbl>();
		defChk(hr);

	Proc_End:

		return hr;
	}



}; // namespace conspiracy
}



