////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Game Instance entity
//	
////////////////////////////////////////////////////////////////////////////////




// Instance ID query
GameInsID GameInstanceEntity::GetInstanceID()
{
	return GameInsID(GetEntityID());
}

GameInsUID GameInstanceEntity::GetInstanceUID()
{
	return GameInsUID( Svr::BrServer::GetInstance()->GetServerUID(), GetEntityID() );
}



// Get player count at this zone
UINT GameInstanceEntity::GetNumPlayer()
{
	return (UINT)m_GamePlayerByUID.GetItemCount();
}


