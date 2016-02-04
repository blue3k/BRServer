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
#include "Common/StrUtil.h"
#include "Common/Trace.h"
#include "Common/Thread.h"
#include "Common/Memory.h"
#include "Net/NetDef.h"
#include "ServerSystem/Entity.h"
#include "ServerSystem/ServerComponent.h"
#include "ServerSystem/ServerServiceBase.h"
#include "ServerSystem/ServerEntity.h"
#include "ServerSystem/EntityManager.h"
#include "ServerSystem/ServiceEntity/GamePartyEntity.h"
#include "ServerSystem/ServiceEntity/GamePartyManagerServiceEntity.h"
#include "ServerSystem/ServiceEntity/GamePartyManagerServiceTrans.h"
#include "ServerSystem/ServiceEntity/GamePartyEntityTrans.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/SvrConst.h"

#include "ServerSystem/PerformanceCounter/PerformanceCounterClient.h"

#include "Protocol/Message/GamePartyManagerMsgClass.h"
#include "Protocol/Message/GamePartyMsgClass.h"




namespace BR {
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
		BR_ENTITY_MESSAGE(Message::GamePartyManager::CreatePartyCmd) { svrMemReturn(pNewTrans = new PartyManagerTransCreateParty(pMsgData)); return S_SYSTEM_OK; } );
		BR_ENTITY_MESSAGE(Message::GamePartyManager::PartyDeletedC2SEvt) { svrMemReturn(pNewTrans = new PartyManagerTransPartyDeleted(pMsgData)); return S_SYSTEM_OK; } );

		// Game party instance transactions
		BR_ENTITY_MESSAGE(Message::GameParty::JoinPartyCmd) { svrMemReturn(pNewTrans = new PartyTransJoinParty(pMsgData)); return S_SYSTEM_OK; } );
		BR_ENTITY_MESSAGE(Message::GameParty::LeavePartyCmd) { svrMemReturn(pNewTrans = new PartyTransLeaveParty(pMsgData)); return S_SYSTEM_OK; } );
		BR_ENTITY_MESSAGE(Message::GameParty::KickPlayerCmd) { svrMemReturn(pNewTrans = new PartyTransKickPlayer(pMsgData)); return S_SYSTEM_OK; } );
		BR_ENTITY_MESSAGE(Message::GameParty::ChatMessageC2SEvt) { svrMemReturn(pNewTrans = new PartyTransChatMessage(pMsgData)); return S_SYSTEM_OK; } );
		BR_ENTITY_MESSAGE(Message::GameParty::QuickChatMessageC2SEvt) { svrMemReturn(pNewTrans = new PartyTransQuickChatMessage(pMsgData)); return S_SYSTEM_OK; } );

		BR_ENTITY_MESSAGE(Message::GameParty::StartGameMatchCmd) { svrMemReturn(pNewTrans = new PartyTransStartGameMatchCmd(pMsgData)); return S_SYSTEM_OK; } );
		BR_ENTITY_MESSAGE(Message::GameParty::CancelGameMatchCmd) { svrMemReturn(pNewTrans = new PartyTransCancelGameMatchCmd(pMsgData)); return S_SYSTEM_OK; } );
		BR_ENTITY_MESSAGE(Message::PartyMatchingQueue::PartyMatchingCanceledS2CEvt) { svrMemReturn(pNewTrans = new PartyTransPartyMatchingCanceled(pMsgData)); return S_SYSTEM_OK; } );
		BR_ENTITY_MESSAGE(Message::PartyMatchingQueue::PartyMatchingItemDequeuedS2CEvt) { svrMemReturn(pNewTrans = new PartyTransMatchingItemDequeued(pMsgData)); return S_SYSTEM_OK; } );
		BR_ENTITY_MESSAGE(Message::PartyMatching::PartyGameMatchedS2CEvt) { svrMemReturn(pNewTrans = new PartyTransPartyGameMatchedS2CEvt(pMsgData)); return S_SYSTEM_OK; } );
	}

	GamePartyManagerServiceEntity::~GamePartyManagerServiceEntity()
	{
	}

	HRESULT GamePartyManagerServiceEntity::InitializeEntity(EntityID newEntityID)
	{
		HRESULT hr = S_SYSTEM_OK;

		auto pInstance = PerformanceCounterClient::GetDefaultCounterInstance();
		if (pInstance != nullptr)
		{
			pInstance->AddCounter(&m_PartyCount);
		}

		svrChk(LoadbalanceClusterServiceEntity::InitializeEntity(newEntityID));

	Proc_End:

		return hr;
	}

	HRESULT GamePartyManagerServiceEntity::RegisterServiceMessageHandler( ServerEntity *pServerEntity )
	{
		HRESULT hr = S_SYSTEM_OK;

		svrChk(LoadbalanceClusterServiceEntity::RegisterServiceMessageHandler( pServerEntity ) );

		// Game party manager transactions
		pServerEntity->BR_ENTITY_MESSAGE(Message::GamePartyManager::CreatePartyCmd)						{ svrMemReturn(pNewTrans = new PartyManagerTransCreateParty(pMsgData)); return S_SYSTEM_OK; } );
		pServerEntity->BR_ENTITY_MESSAGE(Message::GamePartyManager::PartyDeletedC2SEvt)					{ svrMemReturn(pNewTrans = new PartyManagerTransPartyDeleted(pMsgData)); return S_SYSTEM_OK; } );

		// Game party instance transactions
		pServerEntity->BR_ENTITY_MESSAGE(Message::GameParty::JoinPartyCmd)								{ svrMemReturn(pNewTrans = new PartyTransJoinParty(pMsgData)); return S_SYSTEM_OK; } );
		pServerEntity->BR_ENTITY_MESSAGE(Message::GameParty::LeavePartyCmd)								{ svrMemReturn(pNewTrans = new PartyTransLeaveParty(pMsgData)); return S_SYSTEM_OK; } );
		pServerEntity->BR_ENTITY_MESSAGE(Message::GameParty::KickPlayerCmd)								{ svrMemReturn(pNewTrans = new PartyTransKickPlayer(pMsgData)); return S_SYSTEM_OK; } );
		pServerEntity->BR_ENTITY_MESSAGE(Message::GameParty::ChatMessageC2SEvt)							{ svrMemReturn(pNewTrans = new PartyTransChatMessage(pMsgData)); return S_SYSTEM_OK; } );
		pServerEntity->BR_ENTITY_MESSAGE(Message::GameParty::QuickChatMessageC2SEvt)					{ svrMemReturn(pNewTrans = new PartyTransQuickChatMessage(pMsgData)); return S_SYSTEM_OK; } );

		pServerEntity->BR_ENTITY_MESSAGE(Message::GameParty::StartGameMatchCmd)							{ svrMemReturn(pNewTrans = new PartyTransStartGameMatchCmd(pMsgData)); return S_SYSTEM_OK; } );
		pServerEntity->BR_ENTITY_MESSAGE(Message::GameParty::CancelGameMatchCmd)						{ svrMemReturn(pNewTrans = new PartyTransCancelGameMatchCmd(pMsgData)); return S_SYSTEM_OK; } );
		pServerEntity->BR_ENTITY_MESSAGE(Message::PartyMatchingQueue::PartyMatchingCanceledS2CEvt)		{ svrMemReturn(pNewTrans = new PartyTransPartyMatchingCanceled(pMsgData)); return S_SYSTEM_OK; } );
		pServerEntity->BR_ENTITY_MESSAGE(Message::PartyMatchingQueue::PartyMatchingItemDequeuedS2CEvt)	{ svrMemReturn(pNewTrans = new PartyTransMatchingItemDequeued(pMsgData)); return S_SYSTEM_OK; } );
		pServerEntity->BR_ENTITY_MESSAGE(Message::PartyMatching::PartyGameMatchedS2CEvt)				{ svrMemReturn(pNewTrans = new PartyTransPartyGameMatchedS2CEvt(pMsgData)); return S_SYSTEM_OK; } );

	Proc_End:

		return hr;
	}


	//////////////////////////////////////////////////////////////////////////
	//
	//	Game party operations
	//

	// Add new Entity
	HRESULT GamePartyManagerServiceEntity::CreateGameParty( const PlayerInformation& creator, EntityUID playerUID, ServerEntity *pServerEntity, PartyUID &partyUID )
	{
		HRESULT hr = S_SYSTEM_OK;
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
	HRESULT GamePartyManagerServiceEntity::FreeGameParty( PartyUID partyUID )
	{
		HRESULT hr = S_SYSTEM_OK;

		if (FAILED(Svr::GetServerComponent<Svr::EntityManager>()->RemoveEntity(partyUID.GetEntityID())))
		{
			svrTrace(Trace::TRC_ERROR, "Failed to delete party info {0}", partyUID);
		}
		else
		{
			--m_PartyCount;
			svrTrace(Trace::TRC_TRACE, "Party deleted {0}", partyUID);
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
}; // namespace BR {



