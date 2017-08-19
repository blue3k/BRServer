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
	return *m_pLoopbackServerEntity;
}

// Set server UID
void BrServer::SetServerUID( UINT uiUID )
{
	m_uiUID = uiUID;
}

// Get Server start up time, UTC
TimeStampSec BrServer::GetServerUpTime()
{
	return m_ServerUpUTCTIme;
}

// Set my config 
void BrServer::SetMyConfig( const Config::GenericServer* pMyConfig )
{
	m_pMyConfig = pMyConfig;
}


// Get net class
NetClass BrServer::GetNetClass()
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






///////////////////////////////////////////////////////////////////////////////
//
//	Static interfaces
//

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

template< class ComponentType >
Result AddServerComponent(ComponentType* &newComponent)
{
	if (BrServer::GetInstance() == nullptr) return ResultCode::UNEXPECTED;
	return BrServer::GetInstance()->AddComponent(newComponent);
}



// Get Remote Entity Manager
ServerEntity* GetLoopbackServerEntity()
{
	Assert( BrServer::GetInstance() );
	return BrServer::GetInstance()->GetLoopbackServerEntity();
}



template<class DBManagerType>
Result BrServer::AddDBCluster(Svr::Config::DBCluster *pDBClusterCfg)
{
	Result hr = ResultCode::SUCCESS;
	DB::DBClusterManager* pDBManager = nullptr;
	DBManagerType *pDB = nullptr;

	if (pDBClusterCfg == nullptr)
		return ResultCode::UNEXPECTED;

	// Just add it once
	if (GetComponent<DBManagerType>() != nullptr)
		return ResultCode::SUCCESS_FALSE;


	auto& DBinstances = Svr::Config::GetConfig().DBInstances;
	auto itInstnace = DBinstances.find(pDBClusterCfg->DBInstanceName);

	if (itInstnace == DBinstances.end())
		return ResultCode::E_DB_INVALID_CONFIG;

	auto instanceInfo = itInstnace->second;

	svrMem( pDB = new DBManagerType );
	pDBManager = reinterpret_cast<DB::DBClusterManager*>(pDB);
	svrChk( pDBManager->InitializeDBCluster( 1 ) );

	m_DBManagers.push_back(pDBManager);
	svrTrace(Trace::TRC_INFO, "Adding DB manager {0} clusterType:{1}", typeid(DBManagerType).name(), (UINT32)pDBClusterCfg->ClusterType);
	svrTrace(Trace::TRC_INFO, "	DB manager Instance:{0} DB:{1}, ConnectionString:{2}", instanceInfo->InstanceName, pDBClusterCfg->DBName, instanceInfo->ConnectionString);

	svrChk( pDBManager->AddDBSource( 
		0,
		instanceInfo->InstanceName,
		instanceInfo->ConnectionString, 
		pDBClusterCfg->DBName,
		instanceInfo->UserID, 
		instanceInfo->Password ) );

	svrChk( AddComponent( pDB ) );


Proc_End:


	return hr;
}
