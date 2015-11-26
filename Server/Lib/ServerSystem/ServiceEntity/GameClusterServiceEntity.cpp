////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 Madk
// 
// Author : KyungKun Ko
//
// Description : Server service Component entity implementation
//	
//
////////////////////////////////////////////////////////////////////////////////



#include "stdafx.h"
#include "Common/StrUtil.h"
#include "Common/Trace.h"
#include "Common/Thread.h"
#include "Net/NetDef.h"
#include "ServerSystem/Entity.h"
#include "ServerSystem/ServerComponent.h"
#include "ServerSystem/ServerServicebase.h"
#include "ServerSystem/ServerEntity.h"
#include "ServerSystem/ServiceEntity/GameClusterServiceEntity.h"
#include "ServerSystem/ServerService/ClusterServerService.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/BrServerUtil.h"

#include "Protocol/Message/LoginServerMsgClass.h"
#include "ServerSystem/ServiceEntity/GameClusterServiceTrans.h"


BR_MEMORYPOOL_IMPLEMENT(Svr::GameClusterServiceEntity::PlayerTableItem);


namespace BR {
namespace Svr {


	//////////////////////////////////////////////////////////////////////////
	//
	//	Entity informations
	//

	GameClusterServiceEntity::GameClusterServiceEntity(GameID gameID, ClusterMembership initialMembership)
		:IServerComponent(ComponentID)
		,FreeReplicaClusterServiceEntity(GetGameClusterID(gameID), initialMembership )
	{
	}

	GameClusterServiceEntity::~GameClusterServiceEntity()
	{
	}



	//////////////////////////////////////////////////////////////////////////
	//
	//	Player operations
	//

	void GameClusterServiceEntity::PlayerTableItem::UpdateEntityInfo( EntityUID entityUID, ServerEntity* pServerEntity )
	{
		m_EntityUID = entityUID;
		m_ServerUpTime = pServerEntity->GetServerUpTime();
		m_ServerEntity = pServerEntity;
	}

	HRESULT GameClusterServiceEntity::RegisterServiceMessageHandler( ServerEntity *pServerEntity )
	{
		HRESULT hr = S_OK;

		svrChk( __super::RegisterServiceMessageHandler( pServerEntity ) );

		pServerEntity->BR_ENTITY_MESSAGE(Message::ClusterServer::GamePlayerEntityCreatedC2SEvt)				{ svrMemReturn(pNewTrans = new GameServerTransGamePlayerEntityCreatedS2CEvt(pMsgData)); return S_OK; } );
		pServerEntity->BR_ENTITY_MESSAGE(Message::ClusterServer::GamePlayerEntityDeletedC2SEvt)				{ svrMemReturn(pNewTrans = new GameServerTransGamePlayerEntityDeletedS2CEvt(pMsgData)); return S_OK; } );


	Proc_End:

		return hr;
	}

	HRESULT GameClusterServiceEntity::TickUpdate(Svr::TimerAction *pAction)
	{
		HRESULT hr = S_OK;

		svrChk( __super::TickUpdate(pAction) );

		if( (SignedCounterType)m_NumberOfPlayerOnThisServer > 0 )
		{
			SetWorkload((UINT)m_NumberOfPlayerOnThisServer);
		}

	Proc_End:

		return hr;
	}

	// Create PlayerInfo
	HRESULT GameClusterServiceEntity::CreatePlayer( PlayerID playerID, EntityUID entityUID, ServerEntity* pGameServerEntity )
	{
		HRESULT hr = S_OK;
		PlayerTableItem *pNewPlayerInfo = nullptr;
		PlayerIDMap::iterator itPlayer;

		if( playerID == 0 || entityUID == 0 ) svrErr(E_INVALID_PLAYERID);
		svrChkPtr( pGameServerEntity );
		if( entityUID.GetServerID() != pGameServerEntity->GetServerID() )
		{
			svrErr(E_SVR_INVALID_SERVERID);
		}

		if( SUCCEEDED(m_PlayerIDMap.find( playerID, itPlayer )) )
		{
			Assert(itPlayer->m_PlayerID == playerID);
			itPlayer->UpdateEntityInfo( entityUID, pGameServerEntity );
			goto Proc_End;
		}

		svrMem( pNewPlayerInfo = new PlayerTableItem( playerID, entityUID, pGameServerEntity ) );

		svrChk( m_PlayerIDMap.insert(pNewPlayerInfo->GetPlayerID(), pNewPlayerInfo ) );

		pNewPlayerInfo = nullptr;

	Proc_End:

		Util::SafeDelete( pNewPlayerInfo );

		return hr;
	}

	HRESULT GameClusterServiceEntity::CreatePlayer( PlayerID playerID, EntityUID entityUID )
	{
		HRESULT hr = S_OK;

		if( playerID == 0 || entityUID == 0 ) svrErr(E_INVALID_PLAYERID);

		svrChk( CreatePlayer( playerID, entityUID, GetLoopbackServerEntity() ) );

		m_NumberOfPlayerOnThisServer.fetch_add(1, std::memory_order_relaxed);

		ForEachNonWatcher([&](ServerServiceInformation* pServerService){
			if( pServerService->GetEntityUID() == GetEntityUID() )
				return;

			ClusterServerService *pService = pServerService->GetService<ClusterServerService>();
			if( pService == nullptr )
				return;

			// I don't want to let it be replicated, so the route hop is 1 even though it's the first hop
			pService->GamePlayerEntityCreatedC2SEvt( GetEntityID(), 1, playerID, entityUID );
			
		});

	Proc_End:

		return hr;
	}

	// Create PlayerInfo
	HRESULT GameClusterServiceEntity::DeletePlayer( PlayerID playerID, EntityUID playerEntityUID )
	{
		HRESULT hr = S_OK;
		PlayerTableItem *pPlayerInfo = nullptr;
		PlayerIDMap::iterator itPlayer;

		if( FAILED(m_PlayerIDMap.find( playerID, itPlayer )) )
		{
			svrErr( E_INVALID_PLAYERID );
		}

		pPlayerInfo = *itPlayer;

		svrChk( m_PlayerIDMap.erase( itPlayer ) );

		if (pPlayerInfo->GetEntityUID() != playerEntityUID)
			svrErr(E_INVALID_ENTITY);

		// replicate delete from my server
		if( pPlayerInfo && pPlayerInfo->GetEntityUID().GetServerID() == GetMyServerID() )
		{
			m_NumberOfPlayerOnThisServer.fetch_sub(1, std::memory_order_relaxed);

			ForEachNonWatcher([&](ServerServiceInformation* pServerService){
				if( pServerService->GetEntityUID() == GetEntityUID() )
					return;

				ClusterServerService *pService = pServerService->GetService<ClusterServerService>();
				if( pService == nullptr )
					return;

				// I don't want to let it be replicated, so the route hop is 1 even though it's the first hop
				pService->GamePlayerEntityDeletedC2SEvt( GetEntityID(), 1, playerID, pPlayerInfo->GetEntityUID() );
			
			});
		}

	Proc_End:

		Util::SafeDelete( pPlayerInfo );

		return hr;

	}

	// Get Player info
	HRESULT GameClusterServiceEntity::FindPlayer( PlayerID playerID, EntityUID &playerUID )
	{
		HRESULT hr = S_OK;
		PlayerIDMap::iterator itPlayer;

		if( FAILED(m_PlayerIDMap.find( playerID, itPlayer )) )
		{
			return E_PLAYER_NOT_FOUND;
		}

		// Check validity
		if( itPlayer->GetServerUpTime() != itPlayer->GetServerEntity()->GetServerUpTime() )
		{
			// invalid player information
			PlayerTableItem *pPlayerInfo = nullptr;

			pPlayerInfo = *itPlayer;

			svrChk( m_PlayerIDMap.erase( itPlayer ) );

			Util::SafeDelete( pPlayerInfo );

			return E_PLAYER_NOT_FOUND;
		}

		playerUID = itPlayer->GetEntityUID();


	Proc_End:

		return hr;
	}


}; // namespace Svr {
}; // namespace BR {



