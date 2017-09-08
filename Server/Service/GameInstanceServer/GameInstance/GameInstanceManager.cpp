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
#include "SvrConst.h"
#include "SvrTrace.h"
#include "Entity/EntityManager.h"
#include "Util/TimeUtil.h"
#include "Types/SFEngineTypedefs.h"


#include "ConspiracyGameInstanceSvrConst.h"
#include "GameInstanceEntity.h"
#include "GameInstanceManager.h"

#include "Protocol/Message/GameInstanceMsgClass.h"

#include "Transaction/GameInstanceTransPlayer.h"


#include "ConspiracyGameInstanceServerClass.h"



namespace SF {
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



	//// Create new game instance
	//Result GameInstanceManagerServiceEntity::CreateGameInstance( GameInsUID &gameUID, uint numBot, uint maxPlayer )
	//{
	//	Result hr = ResultCode::SUCCESS;
	//	 GameInstanceEntity* pGameInstance = nullptr;

	//	svrChkPtr( pGameInstance = new(GetMemoryManager()) GameInstanceEntity );

	//	svrChk( pGameInstance->InitializeGameEntity( numBot, maxPlayer ) );

	//	svrChk(Svr::GetServerComponent<Svr::EntityManager>()->AddEntity(EntityFaculty::GameInstance, pGameInstance));

	//	gameUID = pGameInstance->GetEntityUID();
	//	svrTrace(Info, "CreateGameInstance:{0}, numBot:{1}, maxPlayer:{2}", GetEntityUID(), numBot, maxPlayer);

	//	++m_NumberOfInstance;
	//	m_LocalWorkload.fetch_add(1,std::memory_order_relaxed);

	//Proc_End:

	//	if( !(hr) )
	//		Util::SafeDelete(pGameInstance);

	//	return hr;
	//}



	
	Result GameInstanceManagerServiceEntity::RegisterServiceMessageHandler( Svr::ServerEntity *pServerEntity )
	{
		Result hr = ResultCode::SUCCESS;

		svrChk( super::RegisterServiceMessageHandler( pServerEntity ) );


		////////////////////////////////////////////////////////////////////////////////////////////////////
		// To game instance
		pServerEntity->BR_ENTITY_MESSAGE(Message::GameInstance::DeleteGameC2SEvt)				{ svrMemReturn(pNewTrans = new(GetHeap()) GameEntityTransDeleteGame(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		pServerEntity->BR_ENTITY_MESSAGE(Message::GameInstance::JoinGameCmd)					{ svrMemReturn(pNewTrans = new(GetHeap()) GameEntityTransJoinGame(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		pServerEntity->BR_ENTITY_MESSAGE(Message::GameInstance::LeaveGameCmd)					{ svrMemReturn(pNewTrans = new(GetHeap()) GameEntityTransLeaveGame(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		pServerEntity->BR_ENTITY_MESSAGE(Message::GameInstance::KickPlayerCmd)					{ svrMemReturn(pNewTrans = new(GetHeap()) GameEntityTransKickPlayer(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		pServerEntity->BR_ENTITY_MESSAGE(Message::GameInstance::AssignRoleCmd)					{ svrMemReturn(pNewTrans = new(GetHeap()) GameEntityTransAssignRole(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		pServerEntity->BR_ENTITY_MESSAGE(Message::GameInstance::ChatMessageC2SEvt)				{ svrMemReturn(pNewTrans = new(GetHeap()) GameEntityTransChatMessage(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		pServerEntity->BR_ENTITY_MESSAGE(Message::GameInstance::VoteGameAdvanceCmd)				{ svrMemReturn(pNewTrans = new(GetHeap()) GameEntityTransVoteGameAdvance(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		pServerEntity->BR_ENTITY_MESSAGE(Message::GameInstance::VoteCmd)						{ svrMemReturn(pNewTrans = new(GetHeap()) GameEntityTransVote(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		pServerEntity->BR_ENTITY_MESSAGE(Message::GameInstance::AdvanceGameCmd)					{ svrMemReturn(pNewTrans = new(GetHeap()) GameEntityTransAdvanceGame(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		pServerEntity->BR_ENTITY_MESSAGE(Message::GameInstance::GamePlayAgainCmd)				{ svrMemReturn(pNewTrans = new(GetHeap()) GameEntityTransGamePlayAgain(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		pServerEntity->BR_ENTITY_MESSAGE(Message::GameInstance::GameRevealPlayerCmd)			{ svrMemReturn(pNewTrans = new(GetHeap()) GameEntityTransGameRevealPlayer(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		pServerEntity->BR_ENTITY_MESSAGE(Message::GameInstance::GamePlayerReviveCmd)			{ svrMemReturn(pNewTrans = new(GetHeap()) GameEntityTransGamePlayerRevive(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );


	Proc_End:

		return hr;
	}

}; // namespace ConspiracyGameInstanceServer
}; // namespace SF







