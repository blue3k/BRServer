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
//Result GameInstanceEntity::ForeachPlayer(std::function<Result(GamePlayer* pPlayer)> func)
template< class Func >
Result GameInstanceEntity::ForeachPlayer(Func func)
{
	m_GamePlayerByUID.ForeachOrder(0, GameLogItem::LEGACY_MAX_GAMEPLAYER, [&](const PlayerID& playerID, Svr::GameInstancePlayer* pPlayer)-> bool
	{
		Result hrRes = func((GamePlayer*)pPlayer);
		if (!(hrRes))
			return false;
		return true;
	});
	return ResultCode::SUCCESS;
}


// foreach game player with Game policy
template< class Func >
inline Result GameInstanceEntity::ForeachPlayerGameServer(Func func)
{
	m_GamePlayerByUID.ForeachOrder(0, GameLogItem::LEGACY_MAX_GAMEPLAYER, [&](const PlayerID& playerID, Svr::GameInstancePlayer* pGameInsPlayer)-> bool
	{
		if (pGameInsPlayer == nullptr)
			return true;

		GamePlayer* pGamePlayer = (GamePlayer*)pGameInsPlayer;

		if (pGamePlayer->GetPlayerState() == PlayerState::None)
			return true;

		NetPolicyGameServer netPolicy(pGamePlayer->GetRemoteEndpoint());
		Result hrRes = func(pGamePlayer, netPolicy);
		if (!(hrRes))
			return false;

		return true;
	});
	return ResultCode::SUCCESS;
}

// foreach game player with Game policy
template< class Func >
inline Result GameInstanceEntity::ForeachPlayerSvrGameInstance(Func func)
{
	m_GamePlayerByUID.ForeachOrder(0, GameLogItem::LEGACY_MAX_GAMEPLAYER, [&](const PlayerID& playerID, Svr::GameInstancePlayer* pGameInsPlayer)-> bool
	{
		if (pGameInsPlayer == nullptr)
			return true;

		GamePlayer* pGamePlayer = (GamePlayer*)pGameInsPlayer;

		if (pGamePlayer->GetPlayerState() == PlayerState::None)
			return true;

		NetSvrPolicyGameInstance netPolicy(pGamePlayer->GetRemoteEndpoint());
		Result hrRes = func(pGamePlayer, netPolicy);
		if (!(hrRes))
			return false;

		return true;
	});

	return ResultCode::SUCCESS;
}

