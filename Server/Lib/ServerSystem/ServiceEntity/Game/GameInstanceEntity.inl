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
////HRESULT GameInstanceEntity::ForeachPlayer(std::function<HRESULT(GamePlayer* pPlayer)> func)
//template< class Func >
//HRESULT GameInstanceEntity::ForeachPlayer(Func func)
//{
//	m_GamePlayerByUID.ForeachOrder(0, GameConst::MAX_GAMEPLAYER, [&](const PlayerID& playerID, GamePlayer* pPlayer)-> bool
//	{
//		HRESULT hrRes = func(pPlayer);
//		if (FAILED(hrRes))
//			return false;
//		return true;
//	});
//	return S_SYSTEM_OK;
//}

//
//// foreach game player with Game policy
//inline HRESULT GameInstanceEntity::ForeachPlayerGameServer(std::function<HRESULT(GamePlayer* pPlayer, Policy::IPolicyGameServer *pPolicy)> func)
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
//			HRESULT hrRes = func(pGamePlayer, pPolicy);
//			if (FAILED(hrRes))
//				return false;
//		}
//		return true;
//	});
//	return S_SYSTEM_OK;
//}
//
//// foreach game player with Game policy
//inline HRESULT GameInstanceEntity::ForeachPlayerSvrGameInstance(std::function<HRESULT(GamePlayer* pPlayer, Policy::ISvrPolicyGameInstance *pPolicy)> func)
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
//			HRESULT hrRes = func(pGamePlayer, pPolicy);
//			if (FAILED(hrRes))
//				return false;
//		}
//
//		return true;
//	});
//
//	return S_SYSTEM_OK;
//}
//
