////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Game server inline
//	
////////////////////////////////////////////////////////////////////////////////



const ServerConfig::GameServer* GameServer::GetGameConfig()
{
	return dynamic_cast<const ServerConfig::GameServer*>(GetMyConfig());
}


// Get net public
SharedPointerT<Net::ServerMUDP>& GameServer::GetNetPublic()
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
	return dynamic_cast<GameServer*>(Svr::BrServer::GetInstance());
}


// Query Loopback entity
Svr::ServerEntity* GetLoopbackGameServerEntity()
{
	GameServer* pSvr = GetMyServer() ;
	return pSvr ? pSvr->GetLoopbackGameServerEntity() : nullptr;
}

