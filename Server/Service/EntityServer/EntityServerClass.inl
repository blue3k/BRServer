////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Entity
//	
////////////////////////////////////////////////////////////////////////////////



//// Entity information table
//EntityInfoTable* EntityServer::GetInfoTable()
//{
//	return m_pEntityInfoTable;
//}

// Query entity server instance
EntityServer* GetMyServer()
{
	return dynamic_cast<EntityServer*>(BR::Svr::BrServer::GetInstance());
}


	
//// Query login server instance
//EntityInfoTable* GetEntityInfoTable()
//{
//	EntityServer *pEntSvr = GetMyServer();
//	return pEntSvr ? pEntSvr->GetInfoTable() : nullptr;
//}
//

