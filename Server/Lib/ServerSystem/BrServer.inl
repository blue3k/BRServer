////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Server interface inline implementation
//
////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////
//
//	Server definition
//


// Set server state
void BrServer::SetServerState( ServerState state )
{
	m_State = state;
}

// Get Server state
ServerState BrServer::GetServerState()
{
	return m_State;
}

// Set Loopback entity
void BrServer::SetLoopbackServerEntity( ServerEntity* pLoopback )
{
	m_pLoopbackServerEntity = pLoopback;
}
// Get Loopback entity
ServerEntity* BrServer::GetLoopbackServerEntity()
{
	return m_pLoopbackServerEntity;
}

// Set server UID
void BrServer::SetServerUID( UINT uiUID )
{
	m_uiUID = uiUID;
}

// Get Server start up time, UTC
ULONGLONG BrServer::GetServerUpTime()
{
	return m_ServerUpUTCTIme;
}

// Set my config 
void BrServer::SetMyConfig( const Config::GenericServer* pMyConfig )
{
	m_pMyConfig = pMyConfig;
}


// Get net class
BR::NetClass BrServer::GetNetClass()
{
	return m_NetClass;
}

// Get config
const Config::GenericServer* BrServer::GetMyConfig()
{
	return m_pMyConfig;
}

// Get/Set Public network enable status
bool BrServer::IsNetPublicEnabled()
{
	return m_bIsNetPublicEnabled;
}

bool BrServer::EnableNetPublic( bool bIsEnable )
{
	m_bIsNetPublicEnabled = bIsEnable;
	return m_bIsNetPublicEnabled;
}

// Get Server UID
UINT BrServer::GetServerUID()
{
	return m_uiUID;
}

// Get server instance
BrServer* BrServer::GetInstance()
{
	return BrServer::stm_pServerInstance;
}




// Get entity table
EntityTable& BrServer::GetEntityTable()
{
	return m_EntityTable;
}



///////////////////////////////////////////////////////////////////////////////
//
//	Static interfaces
//

// Get entity table
EntityTable& GetEntityTable()
{
	Assert( BrServer::GetInstance() );
	return BrServer::GetInstance()->GetEntityTable();
}

// Get Remote Entity Manager
template<class componentType>
componentType* GetServerComponent()
{
	if (BrServer::GetInstance() == nullptr) return nullptr;
	return BrServer::GetInstance()->GetComponent<componentType>();
}

template<class componentType>
componentType* GetServerComponent(UINT componentID)
{
	Assert( BrServer::GetInstance() );
	return BrServer::GetInstance()->GetComponent<componentType>(componentID);
}


// Get Remote Entity Manager
ServerEntity* GetLoopbackServerEntity()
{
	Assert( BrServer::GetInstance() );
	return BrServer::GetInstance()->GetLoopbackServerEntity();
}

// Get server ID
ServerID GetMyServerID()
{
	Assert( BrServer::GetInstance() );
	return BrServer::GetInstance()->GetServerUID();
}


template<class DBManagerType>
HRESULT BrServer::InitializeDBCluster(Svr::Config::DBCluster *pDBClusterCfg)
{
	HRESULT hr = S_OK;
	Svr::ServerEntity *pEntity = nullptr;
	DB::QueryManager* pDBManager = nullptr;
	DBManagerType *pDB = nullptr;

	svrChkPtr(pDBClusterCfg);

	auto& DBinstances = Svr::Config::GetConfig().DBInstances;
	auto& DBMembers = pDBClusterCfg->DBMembers;

	svrMem( pDB = new DBManagerType );
	pDBManager = reinterpret_cast<DB::QueryManager*>(pDB);
	svrChk( pDBManager->InitializeDB( pDBClusterCfg->PartitioningCount ) );

	m_DBManagers.push_back(pDBManager);

	for( auto dbInstanceInfo = DBMembers.begin(); dbInstanceInfo != DBMembers.end(); ++dbInstanceInfo )
	{
		Svr::Config::DBClusterInstance* pClusterInstanceCfg = *dbInstanceInfo;
		auto itInstnace = DBinstances.find( pClusterInstanceCfg->DBInstanceName );
		if( itInstnace == DBinstances.end() )
		{
			svrErr(E_DB_INVALID_CONFIG);
		}

		auto instanceInfo = itInstnace->second;

		svrChk( pDBManager->AddDBSource( 
			pClusterInstanceCfg->PartitioningID,
			instanceInfo->InstanceName,
			instanceInfo->ConnectionString, 
			pClusterInstanceCfg->DBName, 
			instanceInfo->UserID, 
			instanceInfo->Password ) );
	}

	svrChk( AddComponent( pDB ) );


Proc_End:


	return hr;
}
