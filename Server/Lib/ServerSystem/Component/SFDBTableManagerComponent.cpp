////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) StormForge
// 
// Author : KyungKun Ko
//
// Description : Server message endpoint
//	
//
////////////////////////////////////////////////////////////////////////////////


#include "ServerSystemPCH.h"
#include "Util/SFStringCrc64.h"
#include "Component/SFDBTableManagerComponent.h"
#include "EngineObject/SFEngineObject.h"
#include "DB/Query.h"
#include "SvrTrace.h"
#include "Util/SFStringFormat.h"
#include "Service/ServerService.h"
#include "ResultCode/SFResultCodeDB.h"

namespace SF
{

	/////////////////////////////////////////////////////////////////////////////////////////
	//
	//	DBTableManagerComponent class
	//

	DBTableManagerComponent::DBTableManagerComponent()
		: LibraryComponent("DBTableManagerComponent")
		, m_Heap(GetSystemHeap())
	{
		Service::DataTableManager = this;
	}

	DBTableManagerComponent::~DBTableManagerComponent()
	{
		Service::DataTableManager = nullptr;
	}

	const SF::Message::MessageID QueryWholeTableCmd_MID = SF::Message::MessageID(Message::MSGTYPE_COMMAND, Message::MSGTYPE_RELIABLE, false, PROTOCOLID_TABLEDB, 1);


	Result DBTableManagerComponent::LoadDataTable(const char* tableName, StringCrc32 keyColumnName, const char* tableSrcPath)
	{
		DB::Session* pSession = nullptr;
		ScopeContext hr([&pSession](Result hr)
			{
				if (pSession)
					pSession->ReleaseSession();
			});

		if (m_pDataSource == nullptr)
			return ResultCode::NOT_INITIALIZED;

		// skip if already loaded
		if (GetDataTable(StringCrc32(tableName)) != nullptr)
			return ResultCode::SUCCESS;

		SFUniquePtr<DB::QueryWholeTable> pQuery;

		svrCheck(m_pDataSource->AssignSession(pSession));

		svrCheck(pSession->OpenSession());

		pQuery.reset(new(GetHeap()) DB::QueryWholeTable(GetHeap(), QueryWholeTableCmd_MID, tableName));
		svrCheckPtr(pQuery.get());

		svrCheck(pSession->ProcessQuery(pQuery.get()));

		DataTable* pTableData = nullptr;
		svrCheckPtr(pTableData = new(GetHeap()) DataTable);

		svrCheck(pTableData->LoadTable(keyColumnName, pQuery->RowsetResults));

		m_DataTableByName.insert(std::make_pair(StringCrc32(tableName), pTableData));

		return hr;
	}

	// Initialize component
	Result DBTableManagerComponent::InitializeComponent()
	{
		Result hr;

		svrCheck(LibraryComponent::InitializeComponent());

		auto tableDBConfig = Service::ServerConfig->FindDBCluster("TableDB");
		if (tableDBConfig == nullptr)
			return ResultCode::DB_INVALID_CONFIG;

		auto tableDBInstance = Service::ServerConfig->FindDBInstance(tableDBConfig->DBInstanceName);
		if (tableDBInstance == nullptr)
			return ResultCode::DB_INVALID_CONFIG;

		svrTrace(Info, "Initializing table DB: {0}/{1}", tableDBInstance->ConnectionString, tableDBConfig->DBName);

		if (m_pDataSource == nullptr)
		{
			svrCheck(DB::Factory::GetInstance().CreateDataSource(GetSystemHeap(), m_pDataSource));
		}

		svrCheck(m_pDataSource->InitializeDBSource(tableDBInstance->ConnectionString, tableDBConfig->DBName, tableDBInstance->UserID, tableDBInstance->Password));


		return hr;
	}

	// Terminate component
	void DBTableManagerComponent::DeinitializeComponent()
	{
		if (m_pDataSource)
		{
			IHeap::Delete(m_pDataSource);
			m_pDataSource = nullptr;
		}

		LibraryComponent::DeinitializeComponent();
	}

}


