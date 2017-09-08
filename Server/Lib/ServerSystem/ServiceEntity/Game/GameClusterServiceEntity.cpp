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
#include "String/StrUtil.h"
#include "ServerLog/SvrLog.h"
#include "Thread/Thread.h"
#include "Net/NetDef.h"
#include "Entity/Entity.h"
#include "Component/ServerComponent.h"
#include "ServerService/ServerServiceBase.h"
#include "ServerEntity/ServerEntity.h"
#include "ServiceEntity/Game/GameClusterServiceEntity.h"
#include "Protocol/ServerService/ClusterServerService.h"
#include "SvrTrace.h"
#include "Server/BrServerUtil.h"

#include "Protocol/Message/LoginServerMsgClass.h"
#include "ServiceEntity/Game/GameClusterServiceTrans.h"


SF_MEMORYPOOL_IMPLEMENT(SF::Svr::GameClusterServiceEntity::PlayerTableItem);


namespace SF {
namespace Svr {


	//////////////////////////////////////////////////////////////////////////
	//
	//	Entity informations
	//

	GameClusterServiceEntity::GameClusterServiceEntity(GameID gameID, ServerConfig::NetPublic *publicNetSocket, ClusterMembership initialMembership)
		: FreeReplicaClusterServiceEntity(GetGameClusterID(gameID), initialMembership)
		, IServerComponent(ComponentID)
		, m_PlayerIDMap(GetMemoryManager())
		, m_PublicNetSocket(publicNetSocket)
	{
		//BR_ENTITY_MESSAGE(Message::GameServer::RegisterPlayerToJoinGameServerCmd) { svrMemReturn(pNewTrans = new(GetMemoryManager()) GameServerTransRegisterPlayerToJoinGameServer(pMsgData)); return ResultCode::SUCCESS; } );

		BR_ENTITY_MESSAGE(Message::ClusterServer::GamePlayerEntityCreatedC2SEvt) { svrMemReturn(pNewTrans = new(GetHeap()) GameServerTransGamePlayerEntityCreatedS2CEvt(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::ClusterServer::GamePlayerEntityDeletedC2SEvt) { svrMemReturn(pNewTrans = new(GetHeap()) GameServerTransGamePlayerEntityDeletedS2CEvt(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
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

		pServerEntity->BR_ENTITY_MESSAGE(Message::ClusterServer::GamePlayerEntityCreatedC2SEvt)				{ svrMemReturn(pNewTrans = new(GetHeap()) GameServerTransGamePlayerEntityCreatedS2CEvt(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		pServerEntity->BR_ENTITY_MESSAGE(Message::ClusterServer::GamePlayerEntityDeletedC2SEvt)				{ svrMemReturn(pNewTrans = new(GetHeap()) GameServerTransGamePlayerEntityDeletedS2CEvt(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );


	Proc_End:

		return hr;
	}

	Result GameClusterServiceEntity::TickUpdate(TimerAction *pAction)
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(FreeReplicaClusterServiceEntity::TickUpdate(pAction) );

		if( (SignedCounterType)m_NumberOfPlayerOnThisServer > 0 )
		{
			SetWorkload((uint)m_NumberOfPlayerOnThisServer);
		}

	Proc_End:

		return hr;
	}

	// Create PlayerInfo
	Result GameClusterServiceEntity::CreatePlayer( PlayerID playerID, EntityUID entityUID, ServerEntity* pGameServerEntity )
	{
		Result hr = ResultCode::SUCCESS;
		PlayerTableItem *pNewPlayerInfo = nullptr;
		PlayerIDMapIterator itPlayer;

		if( playerID == 0 || entityUID.UID == 0 ) svrErr(ResultCode::INVALID_PLAYERID);
		svrChkPtr( pGameServerEntity );
		if( entityUID.GetServerID() != pGameServerEntity->GetServerID() )
		{
			svrErr(ResultCode::SVR_INVALID_SERVERID);
		}

		if( (m_PlayerIDMap.find( playerID, itPlayer )) )
		{
			Assert(itPlayer->GetPlayerID() == playerID);
			itPlayer->UpdateEntityInfo( entityUID, pGameServerEntity );
			goto Proc_End;
		}

		svrMem( pNewPlayerInfo = new(GetMemoryManager()) PlayerTableItem( playerID, entityUID, pGameServerEntity ) );

		svrChk( m_PlayerIDMap.insert(pNewPlayerInfo->GetPlayerID(), pNewPlayerInfo ) );

		pNewPlayerInfo = nullptr;

	Proc_End:

		Util::SafeDelete( pNewPlayerInfo );

		return hr;
	}

	Result GameClusterServiceEntity::CreatePlayer( PlayerID playerID, EntityUID entityUID )
	{
		Result hr = ResultCode::SUCCESS;

		if( playerID == 0 || entityUID.UID == 0 ) svrErr(ResultCode::INVALID_PLAYERID);

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
		PlayerIDMapIterator itPlayer;

		if( !(m_PlayerIDMap.find( playerID, itPlayer )) )
		{
			return ResultCode::INVALID_PLAYERID;
		}

		pPlayerInfo = itPlayer;

		svrChk( m_PlayerIDMap.erase(playerID) );

		if (pPlayerInfo->GetEntityUID() != playerEntityUID)
			svrErr(ResultCode::INVALID_ENTITY);

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
		PlayerIDMapIterator itPlayer;

		if( !(m_PlayerIDMap.find( playerID, itPlayer )) )
		{
			return ResultCode::SVR_PLAYER_NOT_FOUND;
		}

		// Check validity
		if( itPlayer->GetServerUpTime() != itPlayer->GetServerEntity()->GetServerUpTime() )
		{
			// invalid player information
			PlayerTableItem *pPlayerInfo = nullptr;

			pPlayerInfo = itPlayer;

			svrChk( m_PlayerIDMap.erase(pPlayerInfo->GetPlayerID()) );

			Util::SafeDelete( pPlayerInfo );

			return ResultCode::SVR_PLAYER_NOT_FOUND;
		}

		playerUID = itPlayer->GetEntityUID();


	Proc_End:

		return hr;
	}


}; // namespace Svr {
}; // namespace SF {



