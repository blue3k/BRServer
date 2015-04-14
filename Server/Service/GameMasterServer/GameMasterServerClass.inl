////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Game master server inline implementation
//	
////////////////////////////////////////////////////////////////////////////////



// Query Zone server instance
GameMasterServer* GetMyServer()
{
	return (GameMasterServer*)BR::Svr::BrServer::GetInstance();
}


// Player info manager
PlayerInfoManager* GetPlayerInfoMgr()
{
	return GetMyServer()->GetPlayerInfoMgr();
}



