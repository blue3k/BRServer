////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Game Party entity
//	
////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////
//
//	Game Party entity class
//

// Party ID query
BR::PartyID GamePartyEntity::GetPartyID()
{
	return GetEntityID();
}

BR::PartyUID GamePartyEntity::GetPartyUID()
{
	return GetEntityUID();
}



// Get player count in this party
UINT GamePartyEntity::GetNumPlayer()
{
	return (UINT)m_PartyPlayerByUID.size();
}


