////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Game entity manager
//	
////////////////////////////////////////////////////////////////////////////////



#include "stdafx.h"
#include "ServerSystem/SvrConst.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/EntityManager.h"
#include "Common/TimeUtil.h"
#include "Common/BrBaseTypes.h"


#include "ConspiracyGameInstanceSvrConst.h"
#include "GameInstanceEntity.h"
#include "GameInstanceManager.h"

#include "Protocol/Message/GameInstanceMsgClass.h"

#include "Transaction/GameInstanceTransPlayer.h"


#include "ConspiracyGameInstanceServerClass.h"



namespace BR {
namespace ConspiracyGameInstanceServer {



	//////////////////////////////////////////////////////////////////////////
	//
	//	Game User entity manager
	//


	// Constructor/Destructor
	GameInstanceManagerServiceEntity::GameInstanceManagerServiceEntity( ClusterID clusterID, ClusterMembership initialMembership )
		:Svr::GameInstanceManagerServiceEntity(clusterID, initialMembership)
	{
	}


	GameInstanceManagerServiceEntity::~GameInstanceManagerServiceEntity()
	{
	}



	// Create new game instance
	HRESULT GameInstanceManagerServiceEntity::CreateGameInstance( GameInsUID &gameUID, UINT numBot, UINT maxPlayer )
	{
		HRESULT hr = S_SYSTEM_OK;
		 GameInstanceEntity* pGameInstance = nullptr;

		svrChkPtr( pGameInstance = new GameInstanceEntity );

		svrChk( pGameInstance->InitializeGameEntity( numBot, maxPlayer ) );

		svrChk(Svr::GetServerComponent<Svr::EntityManager>()->AddEntity(EntityFaculty::GameInstance, pGameInstance));

		gameUID = pGameInstance->GetEntityUID();
		svrTrace(Trace::TRC_TRACE, "CreateGameInstance:{0}, numBot:{1}, maxPlayer:{2}", GetEntityUID(), numBot, maxPlayer);

		++m_NumberOfInstance;
		m_LocalWorkload.fetch_add(1,std::memory_order_relaxed);

	Proc_End:

		if( FAILED(hr) )
			Util::SafeDelete(pGameInstance);

		return hr;
	}



	
	HRESULT GameInstanceManagerServiceEntity::RegisterServiceMessageHandler( Svr::ServerEntity *pServerEntity )
	{
		HRESULT hr = S_SYSTEM_OK;

		svrChk( super::RegisterServiceMessageHandler( pServerEntity ) );


		////////////////////////////////////////////////////////////////////////////////////////////////////
		// To game instance
		pServerEntity->BR_ENTITY_MESSAGE(Message::GameInstance::DeleteGameC2SEvt)				{ svrMemReturn(pNewTrans = new GameEntityTransDeleteGame(pMsgData)); return S_SYSTEM_OK; } );
		pServerEntity->BR_ENTITY_MESSAGE(Message::GameInstance::JoinGameCmd)					{ svrMemReturn(pNewTrans = new GameEntityTransJoinGame(pMsgData)); return S_SYSTEM_OK; } );
		pServerEntity->BR_ENTITY_MESSAGE(Message::GameInstance::LeaveGameCmd)					{ svrMemReturn(pNewTrans = new GameEntityTransLeaveGame(pMsgData)); return S_SYSTEM_OK; } );
		pServerEntity->BR_ENTITY_MESSAGE(Message::GameInstance::KickPlayerCmd)					{ svrMemReturn(pNewTrans = new GameEntityTransKickPlayer(pMsgData)); return S_SYSTEM_OK; } );
		pServerEntity->BR_ENTITY_MESSAGE(Message::GameInstance::AssignRoleCmd)					{ svrMemReturn(pNewTrans = new GameEntityTransAssignRole(pMsgData)); return S_SYSTEM_OK; } );
		pServerEntity->BR_ENTITY_MESSAGE(Message::GameInstance::ChatMessageC2SEvt)				{ svrMemReturn(pNewTrans = new GameEntityTransChatMessage(pMsgData)); return S_SYSTEM_OK; } );
		pServerEntity->BR_ENTITY_MESSAGE(Message::GameInstance::VoteGameAdvanceCmd)				{ svrMemReturn(pNewTrans = new GameEntityTransVoteGameAdvance(pMsgData)); return S_SYSTEM_OK; } );
		pServerEntity->BR_ENTITY_MESSAGE(Message::GameInstance::VoteCmd)						{ svrMemReturn(pNewTrans = new GameEntityTransVote(pMsgData)); return S_SYSTEM_OK; } );
		pServerEntity->BR_ENTITY_MESSAGE(Message::GameInstance::AdvanceGameCmd)					{ svrMemReturn(pNewTrans = new GameEntityTransAdvanceGame(pMsgData)); return S_SYSTEM_OK; } );
		pServerEntity->BR_ENTITY_MESSAGE(Message::GameInstance::GamePlayAgainCmd)				{ svrMemReturn(pNewTrans = new GameEntityTransGamePlayAgain(pMsgData)); return S_SYSTEM_OK; } );
		pServerEntity->BR_ENTITY_MESSAGE(Message::GameInstance::GameRevealPlayerCmd)			{ svrMemReturn(pNewTrans = new GameEntityTransGameRevealPlayer(pMsgData)); return S_SYSTEM_OK; } );
		pServerEntity->BR_ENTITY_MESSAGE(Message::GameInstance::GamePlayerReviveCmd)			{ svrMemReturn(pNewTrans = new GameEntityTransGamePlayerRevive(pMsgData)); return S_SYSTEM_OK; } );


	Proc_End:

		return hr;
	}

}; // namespace ConspiracyGameInstanceServer
}; // namespace BR







