////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Game system
//	
////////////////////////////////////////////////////////////////////////////////



// Get suspect list
inline PlayerID GamePlaySystem::GetSuspect( int index )
{
	AssertRel(index>=0 && index<(int)countof(m_Suspects));
	return m_Suspects[index];
}

inline uint GamePlaySystem::GetNumberOfSuspects()
{
	uint numSuspect = 0;
	for( uint iSuspect = 0; iSuspect < countof(m_Suspects); iSuspect++ )
	{
		if( m_Suspects[iSuspect] != 0 ) 
			numSuspect++;
	}
	return numSuspect;
}

inline bool GamePlaySystem::IsSuspect( PlayerID playerID )
{
	return m_Suspects[0] == playerID || m_Suspects[1] == playerID;
}


inline const Array<GamePlayer*>& GamePlaySystem::GetWerewolves()
{
	return m_Werewolves;
}
