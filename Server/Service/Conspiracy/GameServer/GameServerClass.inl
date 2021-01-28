////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Game server inline
//	
////////////////////////////////////////////////////////////////////////////////



// Query Loopback entity
Svr::ServerEntity* GameServer::GetLoopbackGameServerEntity()
{
	return GetLoopbackServerEntity();
}

// Query Game server instance
GameServer* GetMyServer()
{
	return dynamic_cast<GameServer*>(Svr::BrServer::GetInstance());
}


// Query Loopback entity
Svr::ServerEntity* GetLoopbackGameServerEntity()
{
	GameServer* pSvr = GetMyServer() ;
	return pSvr ? pSvr->GetLoopbackGameServerEntity() : nullptr;
}

