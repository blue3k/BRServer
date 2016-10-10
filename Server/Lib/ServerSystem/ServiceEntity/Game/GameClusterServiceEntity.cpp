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
#include "ServerSystem/ServerServiceBase.h"
#include "ServerSystem/ServerEntity.h"
#include "ServerSystem/ServiceEntity/Game/GameClusterServiceEntity.h"
#include "ServerSystem/ServerService/ClusterServerService.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/BrServerUtil.h"

#include "Protocol/Message/LoginServerMsgClass.h"
#include "ServerSystem/ServiceEntity/Game/GameClusterServiceTrans.h"


BR_MEMORYPOOL_IMPLEMENT(Svr::GameClusterServiceEntity::PlayerTableItem);


namespace BR {
namespace Svr {


	//////////////////////////////////////////////////////////////////////////
	//
	//	Entity informations
	//

	GameClusterServiceEntity::GameClusterServiceEntity(Config::PublicNetSocket *publicNetSocket, GameID gameID, ClusterMembership initialMembership)
		: FreeReplicaClusterServiceEntity(GetGameClusterID(gameID), initialMembership)
		, IServerComponent(ComponentID)
		, m_PublicNetSocket(publicNetSocket)
	{
		//BR_ENTITY_MESSAGE(Message::GameServer::RegisterPlayerToJoinGameServerCmd) { svrMemReturn(pNewTrans = new GameServerTransRegisterPlayerToJoinGameServer(pMsgData)); return ResultCode::SUCCESS; } );

		BR_ENTITY_MESSAGE(Message::ClusterServer::GamePlayerEntityCreatedC2SEvt) { svrMemReturn(pNewTrans = new GameServerTransGamePlayerEntityCreatedS2CEvt(pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::ClusterServer::GamePlayerEntityDeletedC2SEvt) { svrMemReturn(pNewTrans = new GameServerTransGamePlayerEntityDeletedS2CEvt(pMsgData)); return ResultCode::SUCCESS; } );
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

	Result GameClusterServiceEntity::RegisterServiceMessageHandler( ServerEntity *pServerEntity )
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(FreeReplicaClusterServiceEntity::RegisterServiceMessageHandler( pServerEntity ) );

		pServerEntity->BR_ENTITY_MESSAGE(Message::ClusterServer::GamePlayerEntityCreatedC2SEvt)				{ svrMemReturn(pNewTrans = new GameServerTransGamePlayerEntityCreatedS2CEvt(pMsgData)); return ResultCode::SUCCESS; } );
		pServerEntity->BR_ENTITY_MESSAGE(Message::ClusterServer::GamePlayerEntityDeletedC2SEvt)				{ svrMemReturn(pNewTrans = new GameServerTransGamePlayerEntityDeletedS2CEvt(pMsgData)); return ResultCode::SUCCESS; } );


	Proc_End:

		return hr;
	}

	Result GameClusterServiceEntity::TickUpdate(Svr::TimerAction *pAction)
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(FreeReplicaClusterServiceEntity::TickUpdate(pAction) );

		if( (SignedCounterType)m_NumberOfPlayerOnThisServer > 0 )
		{
			SetWorkload((UINT)m_NumberOfPlayerOnThisServer);
		}

	Proc_End:

		return hr;
	}

	// Create PlayerInfo
	Result GameClusterServiceEntity::CreatePlayer( PlayerID playerID, EntityUID entityUID, ServerEntity* pGameServerEntity )
	{
		Result hr = ResultCode::SUCCESS;
		PlayerTableItem *pNewPlayerInfo = nullptr;
		PlayerIDMap::iterator itPlayer;

		if( playerID == 0 || entityUID == 0 ) svrErr(ResultCode::E_INVALID_PLAYERID);
		svrChkPtr( pGameServerEntity );
		if( entityUID.GetServerID() != pGameServerEntity->GetServerID() )
		{
			svrErr(ResultCode::E_SVR_INVALID_SERVERID);
		}

		if( SUCCEEDED(m_PlayerIDMap.find( playerID, itPlayer )) )
		{
			Assert(itPlayer->GetPlayerID() == playerID);
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

	Result GameClusterServiceEntity::CreatePlayer( PlayerID playerID, EntityUID entityUID )
	{
		Result hr = ResultCode::SUCCESS;

		if( playerID == 0 || entityUID == 0 ) svrErr(ResultCode::E_INVALID_PLAYERID);

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
	Result GameClusterServiceEntity::DeletePlayer( PlayerID playerID, EntityUID playerEntityUID )
	{
		Result hr = ResultCode::SUCCESS;
		PlayerTableItem *pPlayerInfo = nullptr;
		PlayerIDMap::iterator itPlayer;

		if( FAILED(m_PlayerIDMap.find( playerID, itPlayer )) )
		{
			svrErr( ResultCode::E_INVALID_PLAYERID );
		}

		pPlayerInfo = *itPlayer;

		svrChk( m_PlayerIDMap.erase( itPlayer ) );

		if (pPlayerInfo->GetEntityUID() != playerEntityUID)
			svrErr(ResultCode::E_INVALID_ENTITY);

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
	Result GameClusterServiceEntity::FindPlayer( PlayerID playerID, EntityUID &playerUID )
	{
		Result hr = ResultCode::SUCCESS;
		PlayerIDMap::iterator itPlayer;

		if( FAILED(m_PlayerIDMap.find( playerID, itPlayer )) )
		{
			return ResultCode::E_SVR_PLAYER_NOT_FOUND;
		}

		// Check validity
		if( itPlayer->GetServerUpTime() != itPlayer->GetServerEntity()->GetServerUpTime() )
		{
			// invalid player information
			PlayerTableItem *pPlayerInfo = nullptr;

			pPlayerInfo = *itPlayer;

			svrChk( m_PlayerIDMap.erase( itPlayer ) );

			Util::SafeDelete( pPlayerInfo );

			return ResultCode::E_SVR_PLAYER_NOT_FOUND;
		}

		playerUID = itPlayer->GetEntityUID();


	Proc_End:

		return hr;
	}


}; // namespace Svr {
}; // namespace BR {



