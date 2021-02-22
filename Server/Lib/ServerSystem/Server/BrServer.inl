////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) The Braves
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
void BrServer::SetServerState(ServerState state)
{
	m_State = state;
}

// Get Server state
ServerState BrServer::GetServerState()
{
	return m_State;
}

// Set Loopback entity
void BrServer::SetLoopbackServerEntity(ServerEntity* pLoopback)
{
	m_pLoopbackServerEntity = pLoopback;
}

// Get Loopback entity
ServerEntity* BrServer::GetLoopbackServerEntity()
{
	return *m_pLoopbackServerEntity;
}

// Get Server start up time, UTC
UTCTimeStampSec BrServer::GetServerUpTime()
{
	return m_ServerUpUTCTIme;
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
	return BrServer::GetInstance()->GetComponentCarrier().GetComponent<componentType>();
}

template<class componentType>
componentType* GetServerComponent(uint componentID)
{
	Assert( BrServer::GetInstance() );
	return BrServer::GetInstance()->GetComponentCarrier().GetComponent<componentType>(componentID);
}

// Get Remote Entity Manager
ServerEntity* GetLoopbackServerEntity()
{
	Assert( BrServer::GetInstance() );
	return BrServer::GetInstance()->GetLoopbackServerEntity();
}

template<class DBManagerType>
Result BrServer::AddDBCluster(const ServerConfig::DBCluster *pDBClusterCfg)
{
	Result hr = ResultCode::SUCCESS;
	DB::DBClusterManager* pDBManager = nullptr;
	DBManagerType *pDB = nullptr;

	if (pDBClusterCfg == nullptr)
		return ResultCode::INVALID_ARG;

	// Just add it once
	if (m_Components.GetComponent<DBManagerType>() != nullptr)
		return ResultCode::SUCCESS_FALSE;


	auto instanceInfo = Service::ServerConfig->FindDBInstance(pDBClusterCfg->DBInstanceName);
	if (instanceInfo == nullptr)
		return ResultCode::DB_INVALID_CONFIG;

	svrMem( pDB = new(GetHeap()) DBManagerType );
	pDBManager = reinterpret_cast<DB::DBClusterManager*>(pDB);
	svrChk( pDBManager->InitializeDBCluster( 1 ) );

	m_DBManagers.push_back(pDBManager);
	svrTrace(Info, "Adding DB manager {0} clusterType:{1}", typeid(DBManagerType).name(), (uint32_t)pDBClusterCfg->ClusterType);
	svrTrace(Info, "	DB manager Instance:{0} DB:{1}, ConnectionString:{2}", instanceInfo->InstanceName, pDBClusterCfg->DBName, instanceInfo->ConnectionString);

	svrChk( pDBManager->AddDBSource( 
		0,
		instanceInfo->InstanceName,
		instanceInfo->ConnectionString, 
		pDBClusterCfg->DBName,
		instanceInfo->UserID, 
		instanceInfo->Password ) );

	svrChk(m_Components.AddComponent( pDB ) );

Proc_End:

	return hr;
}
