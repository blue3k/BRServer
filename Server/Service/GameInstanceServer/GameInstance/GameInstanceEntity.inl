////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Game Instance entity
//	
////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////////
//
//	foreach implementations
//

// foreach game player
//HRESULT GameInstanceEntity::ForeachPlayer(std::function<HRESULT(GamePlayer* pPlayer)> func)
template< class Func >
HRESULT GameInstanceEntity::ForeachPlayer(Func func)
{
	m_GamePlayerByUID.ForeachOrder(0, GameConst::MAX_GAMEPLAYER, [&](const PlayerID& playerID, Svr::GameInstancePlayer* pPlayer)-> bool
	{
		HRESULT hrRes = func((GamePlayer*)pPlayer);
		if (FAILED(hrRes))
			return false;
		return true;
	});
	return S_SYSTEM_OK;
}


// foreach game player with Game policy
template< class Func >
inline HRESULT GameInstanceEntity::ForeachPlayerGameServer(Func func)
{
	m_GamePlayerByUID.ForeachOrder(0, GameConst::MAX_GAMEPLAYER, [&](const PlayerID& playerID, Svr::GameInstancePlayer* pGameInsPlayer)-> bool
	{
		if (pGameInsPlayer == nullptr)
			return true;

		GamePlayer* pGamePlayer = (GamePlayer*)pGameInsPlayer;

		if (pGamePlayer->GetPlayerState() == PlayerState::None)
			return true;

		Policy::IPolicyGameServer *pPolicy = pGamePlayer->GetPolicy<Policy::IPolicyGameServer>();
		if (pPolicy)
		{
			HRESULT hrRes = func(pGamePlayer, pPolicy);
			if (FAILED(hrRes))
				return false;
		}
		return true;
	});
	return S_SYSTEM_OK;
}

// foreach game player with Game policy
template< class Func >
inline HRESULT GameInstanceEntity::ForeachPlayerSvrGameInstance(Func func)
{
	m_GamePlayerByUID.ForeachOrder(0, GameConst::MAX_GAMEPLAYER, [&](const PlayerID& playerID, Svr::GameInstancePlayer* pGameInsPlayer)-> bool
	{
		if (pGameInsPlayer == nullptr)
			return true;

		GamePlayer* pGamePlayer = (GamePlayer*)pGameInsPlayer;

		if (pGamePlayer->GetPlayerState() == PlayerState::None)
			return true;

		Policy::ISvrPolicyGameInstance *pPolicy = pGamePlayer->GetPolicy<Policy::ISvrPolicyGameInstance>();
		if (pPolicy)
		{
			HRESULT hrRes = func(pGamePlayer, pPolicy);
			if (FAILED(hrRes))
				return false;
		}

		return true;
	});

	return S_SYSTEM_OK;
}

