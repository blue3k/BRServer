////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 Madk
// 
// Author : KyungKun Ko
//
// Description : Game Party service implementation
//	
//
////////////////////////////////////////////////////////////////////////////////



#include "stdafx.h"
#include "String/StrUtil.h"
#include "ServerLog/SvrLog.h"
#include "Thread/Thread.h"
#include "Memory/SFMemory.h"
#include "Net/NetDef.h"
#include "Entity/Entity.h"
#include "Component/ServerComponent.h"
#include "ServerService/ServerServiceBase.h"
#include "ServerEntity/ServerEntity.h"
#include "Entity/EntityManager.h"
#include "ServerSystem/ServiceEntity/GamePartyEntity.h"
#include "ServerSystem/ServiceEntity/GamePartyManagerServiceEntity.h"
#include "ServerSystem/ServiceEntity/GamePartyManagerServiceTrans.h"
#include "ServerSystem/ServiceEntity/GamePartyEntityTrans.h"
#include "SvrTrace.h"
#include "SvrConst.h"

#include "PerformanceCounter/PerformanceCounterClient.h"

#include "Protocol/Message/GamePartyManagerMsgClass.h"
#include "Protocol/Message/GamePartyMsgClass.h"




namespace SF {
namespace Svr {


	//////////////////////////////////////////////////////////////////////////
	//
	//	Entity informations
	//

	GamePartyManagerServiceEntity::GamePartyManagerServiceEntity(ClusterMembership initialMembership)
		: LoadbalanceClusterServiceEntity(ClusterID::GamePartyManager, initialMembership )
		, IServerComponent(ComponentID)
		, m_PartyCount("PartyCount")
	{
		// Game party manager transactions
		BR_ENTITY_MESSAGE(Message::GamePartyManager::CreatePartyCmd) { svrMemReturn(pNewTrans = new PartyManagerTransCreateParty(pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::GamePartyManager::PartyDeletedC2SEvt) { svrMemReturn(pNewTrans = new PartyManagerTransPartyDeleted(pMsgData)); return ResultCode::SUCCESS; } );

	}

	GamePartyManagerServiceEntity::~GamePartyManagerServiceEntity()
	{
	}

	Result GamePartyManagerServiceEntity::InitializeEntity(EntityID newEntityID)
	{
		Result hr = ResultCode::SUCCESS;

		auto pInstance = PerformanceCounterClient::GetDefaultCounterInstance();
		if (pInstance != nullptr)
		{
			pInstance->AddCounter(&m_PartyCount);
		}

		svrChk(LoadbalanceClusterServiceEntity::InitializeEntity(newEntityID));

	Proc_End:

		return hr;
	}

	Result GamePartyManagerServiceEntity::RegisterServiceMessageHandler( ServerEntity *pServerEntity )
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(LoadbalanceClusterServiceEntity::RegisterServiceMessageHandler( pServerEntity ) );

		// Game party manager transactions
		pServerEntity->BR_ENTITY_MESSAGE(Message::GamePartyManager::CreatePartyCmd)						{ svrMemReturn(pNewTrans = new PartyManagerTransCreateParty(pMsgData)); return ResultCode::SUCCESS; } );
		pServerEntity->BR_ENTITY_MESSAGE(Message::GamePartyManager::PartyDeletedC2SEvt)					{ svrMemReturn(pNewTrans = new PartyManagerTransPartyDeleted(pMsgData)); return ResultCode::SUCCESS; } );

		// Game party instance transactions
		pServerEntity->BR_ENTITY_MESSAGE(Message::GameParty::JoinPartyCmd)								{ svrMemReturn(pNewTrans = new PartyTransJoinParty(pMsgData)); return ResultCode::SUCCESS; } );
		pServerEntity->BR_ENTITY_MESSAGE(Message::GameParty::LeavePartyCmd)								{ svrMemReturn(pNewTrans = new PartyTransLeaveParty(pMsgData)); return ResultCode::SUCCESS; } );
		pServerEntity->BR_ENTITY_MESSAGE(Message::GameParty::KickPlayerCmd)								{ svrMemReturn(pNewTrans = new PartyTransKickPlayer(pMsgData)); return ResultCode::SUCCESS; } );
		pServerEntity->BR_ENTITY_MESSAGE(Message::GameParty::ChatMessageC2SEvt)							{ svrMemReturn(pNewTrans = new PartyTransChatMessage(pMsgData)); return ResultCode::SUCCESS; } );
		pServerEntity->BR_ENTITY_MESSAGE(Message::GameParty::QuickChatMessageC2SEvt)					{ svrMemReturn(pNewTrans = new PartyTransQuickChatMessage(pMsgData)); return ResultCode::SUCCESS; } );

		pServerEntity->BR_ENTITY_MESSAGE(Message::GameParty::StartGameMatchCmd)							{ svrMemReturn(pNewTrans = new PartyTransStartGameMatchCmd(pMsgData)); return ResultCode::SUCCESS; } );
		pServerEntity->BR_ENTITY_MESSAGE(Message::GameParty::CancelGameMatchCmd)						{ svrMemReturn(pNewTrans = new PartyTransCancelGameMatchCmd(pMsgData)); return ResultCode::SUCCESS; } );
		pServerEntity->BR_ENTITY_MESSAGE(Message::PartyMatchingQueue::PartyMatchingCanceledS2CEvt)		{ svrMemReturn(pNewTrans = new PartyTransPartyMatchingCanceled(pMsgData)); return ResultCode::SUCCESS; } );
		pServerEntity->BR_ENTITY_MESSAGE(Message::PartyMatchingQueue::PartyMatchingItemDequeuedS2CEvt)	{ svrMemReturn(pNewTrans = new PartyTransMatchingItemDequeued(pMsgData)); return ResultCode::SUCCESS; } );
		pServerEntity->BR_ENTITY_MESSAGE(Message::PartyMatching::PartyGameMatchedS2CEvt)				{ svrMemReturn(pNewTrans = new PartyTransPartyGameMatchedS2CEvt(pMsgData)); return ResultCode::SUCCESS; } );

	Proc_End:

		return hr;
	}


	//////////////////////////////////////////////////////////////////////////
	//
	//	Game party operations
	//

	// Add new Entity
	Result GamePartyManagerServiceEntity::CreateGameParty( const PlayerInformation& creator, EntityUID playerUID, ServerEntity *pServerEntity, PartyUID &partyUID )
	{
		Result hr = ResultCode::SUCCESS;
		GamePartyEntity *pGameParty = nullptr;
		PartyPlayer *pPlayer = nullptr;

		svrChkPtr( pGameParty = new GamePartyEntity );

		svrChk( GetServerComponent<EntityManager>()->AddEntity( EntityFaculty::Party, pGameParty ) );

		svrMem( pPlayer = new PartyPlayer( creator ) );
		svrChk( pPlayer->SetServerEntity( pServerEntity, playerUID ) );
		svrChk( pGameParty->JoinPlayer( pPlayer ) );

		partyUID = pGameParty->GetEntityUID();
		pGameParty = nullptr;

		++m_PartyCount;
		m_LocalWorkload.fetch_add(1, std::memory_order_relaxed);

	Proc_End:

		// close party instance if it failed to initialize
		if( pGameParty )
			pGameParty->PendingCloseTransaction();

		return hr;
	}

	// Called when a game party is deleted
	Result GamePartyManagerServiceEntity::FreeGameParty( PartyUID partyUID )
	{
		Result hr = ResultCode::SUCCESS;

		if (!(Svr::GetServerComponent<Svr::EntityManager>()->RemoveEntity(partyUID.GetEntityID())))
		{
			svrTrace(Trace::TRC_ERROR, "Failed to delete party info {0}", partyUID);
		}
		else
		{
			--m_PartyCount;
			svrTrace(TRC_INFO, "Party deleted {0}", partyUID);
			m_LocalWorkload.fetch_sub(1, std::memory_order_relaxed);
		}

	//Proc_End:

		return hr;
	}




	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	GamePartyManagerWatcherServiceEntity class
	//


	GamePartyManagerWatcherServiceEntity::GamePartyManagerWatcherServiceEntity()
		:LoadbalanceClusterServiceEntity(ClusterID::GamePartyManager, ClusterMembership::StatusWatcher)
		,IServerComponent(ComponentID)
	{
	}


	GamePartyManagerWatcherServiceEntity::~GamePartyManagerWatcherServiceEntity()
	{
	}

	





}; // namespace Svr {
}; // namespace SF {



