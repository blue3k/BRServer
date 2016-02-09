////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Game server inline
//	
////////////////////////////////////////////////////////////////////////////////



const Svr::Config::PublicServer* GameServer::GetGameConfig()
{
	return dynamic_cast<const Svr::Config::PublicServer*>(GetMyConfig());
}


// Get net public
BR::Net::ServerMUDP* GameServer::GetNetPublic()
{
	return m_pNetPublic;
}


// Query Loopback entity
Svr::ServerEntity* GameServer::GetLoopbackGameServerEntity()
{
	return GetLoopbackServerEntity();
}

// Query Game server instance
GameServer* GetMyServer()
{
	return BR_DYNAMIC_CAST(GameServer*,Svr::BrServer::GetInstance());
	//return dynamic_cast<GameServer*>(Svr::BrServer::GetInstance());
}


// Query Loopback entity
Svr::ServerEntity* GetLoopbackGameServerEntity()
{
	GameServer* pSvr = GetMyServer() ;
	return pSvr ? pSvr->GetLoopbackGameServerEntity() : nullptr;
}

