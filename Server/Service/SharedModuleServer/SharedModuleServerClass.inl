////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2014 The Braves
// 
// Author : KyungKun Ko
//
// Description : Game server inline
//	
////////////////////////////////////////////////////////////////////////////////




// Query Loopback entity
SharedModuleServerEntity* SharedModuleServer::GetLoopbackGameServerEntity()
{
	return (SharedModuleServerEntity*)GetLoopbackServerEntity();
}

// Query Game server instance
SharedModuleServer* GetMyServer()
{
	return BR_DYNAMIC_CAST(SharedModuleServer*,Svr::BrServer::GetInstance());
	//return dynamic_cast<SharedModuleServer*>(Svr::BrServer::GetInstance());
}


// Query Loopback entity
SharedModuleServerEntity* GetLoopbackGameServerEntity()
{
	SharedModuleServer* pSvr = GetMyServer() ;
	return pSvr ? pSvr->GetLoopbackGameServerEntity() : nullptr;
}

