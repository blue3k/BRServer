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
	return GameInsUID( BrServer::GetInstance()->GetServerUID(), GetEntityID() );
}



// Get player count at this zone
UINT GameInstanceEntity::GetNumPlayer()
{
	return (UINT)m_GamePlayerByUID.GetItemCount();
}


//////////////////////////////////////////////////////////////////////////////////////////
//
//	foreach implementations
//
//
//// foreach game player
////Result GameInstanceEntity::ForeachPlayer(std::function<Result(GamePlayer* pPlayer)> func)
//template< class Func >
//Result GameInstanceEntity::ForeachPlayer(Func func)
//{
//	m_GamePlayerByUID.ForeachOrder(0, GameConst::MAX_GAMEPLAYER, [&](const PlayerID& playerID, GamePlayer* pPlayer)-> bool
//	{
//		Result hrRes = func(pPlayer);
//		if (!(hrRes))
//			return false;
//		return true;
//	});
//	return ResultCode::SUCCESS;
//}

//
//// foreach game player with Game policy
//inline Result GameInstanceEntity::ForeachPlayerGameServer(std::function<Result(GamePlayer* pPlayer, Policy::IPolicyGameServer *pPolicy)> func)
//{
//	m_GamePlayerByUID.ForeachOrder(0, GameConst::MAX_GAMEPLAYER, [&](const PlayerID& playerID, GamePlayer* pGamePlayer)-> bool
//	{
//		if (pGamePlayer == nullptr)
//			return true;
//
//		if (pGamePlayer->GetPlayerState() == PlayerState::None)
//			return true;
//
//		Policy::IPolicyGameServer *pPolicy = pGamePlayer->GetPolicy<Policy::IPolicyGameServer>();
//		if (pPolicy)
//		{
//			Result hrRes = func(pGamePlayer, pPolicy);
//			if (!(hrRes))
//				return false;
//		}
//		return true;
//	});
//	return ResultCode::SUCCESS;
//}
//
//// foreach game player with Game policy
//inline Result GameInstanceEntity::ForeachPlayerSvrGameInstance(std::function<Result(GamePlayer* pPlayer, Policy::ISvrPolicyGameInstance *pPolicy)> func)
//{
//	m_GamePlayerByUID.ForeachOrder(0, GameConst::MAX_GAMEPLAYER, [&](const PlayerID& playerID, GamePlayer* pGamePlayer)-> bool
//	{
//		if (pGamePlayer == nullptr)
//			return true;
//
//		if (pGamePlayer->GetPlayerState() == PlayerState::None)
//			return true;
//
//		Policy::ISvrPolicyGameInstance *pPolicy = pGamePlayer->GetPolicy<Policy::ISvrPolicyGameInstance>();
//		if (pPolicy)
//		{
//			Result hrRes = func(pGamePlayer, pPolicy);
//			if (!(hrRes))
//				return false;
//		}
//
//		return true;
//	});
//
//	return ResultCode::SUCCESS;
//}
//
