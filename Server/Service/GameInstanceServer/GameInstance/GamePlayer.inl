////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Game player 
//	
////////////////////////////////////////////////////////////////////////////////

inline bool GamePlayer::IsInGame()
{
	return GetPlayerState() == PlayerState::Playing && GetRole() != PlayerRole::None;
}


