////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2020 Kyungkun Ko
// 
// Author : KyungKun Ko
//
// Description : CharacterData service implementation
//	
//
////////////////////////////////////////////////////////////////////////////////



#include "ServerSystemPCH.h"
#include "SvrTrace.h"
#include "SvrConst.h"
#include "String/SFStrUtil.h"
#include "ServerLog/SvrLog.h"
#include "Thread/SFThread.h"
#include "Memory/SFMemory.h"
#include "Net/SFNetDef.h"
#include "Entity/Entity.h"
#include "Component/ServerComponent.h"
#include "ServerService/ServerServiceBase.h"
#include "ServerEntity/ServerEntity.h"
#include "Entity/EntityManager.h"
#include "ServiceEntity/Character/CharacterData.h"
#include "ServiceEntity/Character/CharacterDataManagerServiceEntity.h"
#include "ServiceEntity/Character/CharacterDataManagerServiceTrans.h"


#include "PerformanceCounter/PerformanceCounterClient.h"

#include "Protocol/Message/CharacterDataServerMsgClass.h"




namespace SF {
namespace Svr {


	//////////////////////////////////////////////////////////////////////////
	//
	//	Entity informations
	//

	CharacterDataManagerServiceEntity::CharacterDataManagerServiceEntity(GameID gameID, ClusterMembership initialMembership)
		: super(gameID, ClusterID::CharacterData, initialMembership )
		, m_CharacterDataCount("CharacterDataCount")
	{
		// Game CharacterData manager transactions
		BR_ENTITY_MESSAGE(Message::CharacterDataServer::AddCharacterDataCmd)		{ svrMemReturn(pNewTrans = new(GetHeap()) CharacterDataManagerTransAddCharacterData(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::CharacterDataServer::RemoveCharacterDataCmd)		{ svrMemReturn(pNewTrans = new(GetHeap()) CharacterDataManagerTransRemoveCharacterData(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );

	}

	CharacterDataManagerServiceEntity::~CharacterDataManagerServiceEntity()
	{
	}

	Result CharacterDataManagerServiceEntity::InitializeEntity(EntityID newEntityID)
	{
		Result hr = ResultCode::SUCCESS;

		auto pInstance = PerformanceCounterClient::GetDefaultCounterInstance();
		if (pInstance != nullptr)
		{
			pInstance->AddCounter(&m_CharacterDataCount);
		}

		svrChk(super::InitializeEntity(newEntityID));

		svrChk(Service::ClusterManager->RegisterClustereWatchers(GetGameID(), ClusterID::MatchingQueue_Game_4x1, ClusterID::MatchingQueue_Game_4x1W));
		svrChk(Service::ClusterManager->RegisterClustereWatchers(GetGameID(), ClusterID::MatchingQueue_Game_8x1, ClusterID::MatchingQueue_Game_8x1W));


	Proc_End:

		return hr;
	}

	Result CharacterDataManagerServiceEntity::RegisterServiceMessageHandler( ServerEntity *pServerEntity )
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(super::RegisterServiceMessageHandler( pServerEntity ) );

		// Game CharacterData manager transactions
		pServerEntity->BR_ENTITY_MESSAGE(Message::CharacterDataServer::CreateCharacterDataCmd)						{ svrMemReturn(pNewTrans = new(GetHeap()) CharacterDataManagerTransCreateCharacterData(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		pServerEntity->BR_ENTITY_MESSAGE(Message::CharacterDataServer::CharacterDataDeletedC2SEvt)					{ svrMemReturn(pNewTrans = new(GetHeap()) CharacterDataManagerTransCharacterDataDeleted(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );

		// Game CharacterData instance transactions
		pServerEntity->BR_ENTITY_MESSAGE(Message::GameCharacterData::JoinCharacterDataCmd)								{ svrMemReturn(pNewTrans = new(GetHeap()) CharacterDataTransJoinCharacterData(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		pServerEntity->BR_ENTITY_MESSAGE(Message::GameCharacterData::LeaveCharacterDataCmd)								{ svrMemReturn(pNewTrans = new(GetHeap()) CharacterDataTransLeaveCharacterData(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		pServerEntity->BR_ENTITY_MESSAGE(Message::GameCharacterData::KickPlayerCmd)										{ svrMemReturn(pNewTrans = new(GetHeap()) CharacterDataTransKickPlayer(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		pServerEntity->BR_ENTITY_MESSAGE(Message::GameCharacterData::ChatMessageC2SEvt)									{ svrMemReturn(pNewTrans = new(GetHeap()) CharacterDataTransChatMessage(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		pServerEntity->BR_ENTITY_MESSAGE(Message::GameCharacterData::QuickChatMessageC2SEvt)							{ svrMemReturn(pNewTrans = new(GetHeap()) CharacterDataTransQuickChatMessage(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );

		pServerEntity->BR_ENTITY_MESSAGE(Message::GameCharacterData::StartGameMatchCmd)									{ svrMemReturn(pNewTrans = new(GetHeap()) CharacterDataTransStartGameMatchCmd(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		pServerEntity->BR_ENTITY_MESSAGE(Message::GameCharacterData::CancelGameMatchCmd)								{ svrMemReturn(pNewTrans = new(GetHeap()) CharacterDataTransCancelGameMatchCmd(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		pServerEntity->BR_ENTITY_MESSAGE(Message::CharacterDataMatchingQueue::CharacterDataMatchingCanceledS2CEvt)		{ svrMemReturn(pNewTrans = new(GetHeap()) CharacterDataTransCharacterDataMatchingCanceled(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		pServerEntity->BR_ENTITY_MESSAGE(Message::CharacterDataMatchingQueue::CharacterDataMatchingItemDequeuedS2CEvt)	{ svrMemReturn(pNewTrans = new(GetHeap()) CharacterDataTransMatchingItemDequeued(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		pServerEntity->BR_ENTITY_MESSAGE(Message::CharacterDataMatching::CharacterDataGameMatchedS2CEvt)				{ svrMemReturn(pNewTrans = new(GetHeap()) CharacterDataTransCharacterDataGameMatchedS2CEvt(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );

	Proc_End:

		return hr;
	}


	//////////////////////////////////////////////////////////////////////////
	//
	//	Game CharacterData operations
	//

	// Add new Entity
	Result CharacterDataManagerServiceEntity::CreateGameCharacterData( GameID gameID, const PlayerInformation& creator, EntityUID playerUID, ServerEntity *pServerEntity, CharacterDataUID &CharacterDataUID )
	{
		Result hr = ResultCode::SUCCESS;
		GameCharacterDataEntity *pGameCharacterData = nullptr;
		CharacterDataPlayer *pPlayer = nullptr;

		svrChkPtr( pGameCharacterData = new(GetHeap()) GameCharacterDataEntity(gameID) );

		svrChk(Service::EntityManager->AddEntity( EntityFaculty::CharacterData, pGameCharacterData ) );

		svrMem( pPlayer = new(GetHeap()) CharacterDataPlayer( creator ) );
		svrChk( pPlayer->SetServerEntity( pServerEntity, playerUID ) );
		svrChk( pGameCharacterData->JoinPlayer( pPlayer ) );

		CharacterDataUID = pGameCharacterData->GetEntityUID();
		pGameCharacterData = nullptr;

		++m_CharacterDataCount;
		m_LocalWorkload.fetch_add(1, std::memory_order_relaxed);

	Proc_End:

		// close CharacterData instance if it failed to initialize
		if( pGameCharacterData )
			pGameCharacterData->PendingCloseTransaction("Game CharacterData creation is failed");

		return hr;
	}

	// Called when a game CharacterData is deleted
	Result CharacterDataManagerServiceEntity::FreeGameCharacterData( CharacterDataUID CharacterDataUID )
	{
		Result hr = ResultCode::SUCCESS;

		if (!(Service::EntityManager->RemoveEntity(CharacterDataUID.GetEntityID())))
		{
			svrTrace(Error, "Failed to delete CharacterData info {0}", CharacterDataUID);
		}
		else
		{
			--m_CharacterDataCount;
			svrTrace(SVR_INFO, "CharacterData deleted {0}", CharacterDataUID);
			m_LocalWorkload.fetch_sub(1, std::memory_order_relaxed);
		}

	//Proc_End:

		return hr;
	}







}; // namespace Svr {
}; // namespace SF {



