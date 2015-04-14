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
ConspiracyGameInstanceServerEntity* GameInstanceServer::GetLoopbackGameServerEntity()
{
	return (ConspiracyGameInstanceServerEntity*)GetLoopbackServerEntity();
}

// Query Game server instance
GameInstanceServer* GetMyServer()
{
	return BR_DYNAMIC_CAST(GameInstanceServer*,Svr::BrServer::GetInstance());
	//return dynamic_cast<GameInstanceServer*>(Svr::BrServer::GetInstance());
}


// Query Loopback entity
ConspiracyGameInstanceServerEntity* GetLoopbackGameServerEntity()
{
	GameInstanceServer* pSvr = GetMyServer() ;
	return pSvr ? pSvr->GetLoopbackGameServerEntity() : nullptr;
}

