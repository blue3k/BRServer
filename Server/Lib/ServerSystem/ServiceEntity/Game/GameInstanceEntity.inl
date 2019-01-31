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
uint GameInstanceEntity::GetNumPlayer()
{
	return (uint)m_GamePlayerByUID.size();
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
//inline Result GameInstanceEntity::ForeachPlayerGameServer(std::function<Result(GamePlayer* pPlayer, Policy::NetPolicyGameServer *pPolicy)> func)
//{
//	m_GamePlayerByUID.ForeachOrder(0, GameConst::MAX_GAMEPLAYER, [&](const PlayerID& playerID, GamePlayer* pGamePlayer)-> bool
//	{
//		if (pGamePlayer == nullptr)
//			return true;
//
//		if (pGamePlayer->GetPlayerState() == PlayerState::None)
//			return true;
//
//		Policy::NetPolicyGameServer *pPolicy = pGamePlayer->GetInterface<Policy::NetPolicyGameServer>();
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
//inline Result GameInstanceEntity::ForeachPlayerSvrGameInstance(std::function<Result(GamePlayer* pPlayer, Policy::NetSvrPolicyGameInstance *pPolicy)> func)
//{
//	m_GamePlayerByUID.ForeachOrder(0, GameConst::MAX_GAMEPLAYER, [&](const PlayerID& playerID, GamePlayer* pGamePlayer)-> bool
//	{
//		if (pGamePlayer == nullptr)
//			return true;
//
//		if (pGamePlayer->GetPlayerState() == PlayerState::None)
//			return true;
//
//		Policy::NetSvrPolicyGameInstance *pPolicy = pGamePlayer->GetInterface<Policy::NetSvrPolicyGameInstance>();
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
