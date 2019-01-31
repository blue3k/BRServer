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
PartyID GamePartyEntity::GetPartyID()
{
	return GetEntityID();
}

PartyUID GamePartyEntity::GetPartyUID()
{
	return GetEntityUID();
}



// Get player count in this party
uint GamePartyEntity::GetNumPlayer()
{
	return (uint)m_PartyPlayerByUID.size();
}


