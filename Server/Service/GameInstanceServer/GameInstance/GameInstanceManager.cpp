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
#include "Util/SFTimeUtil.h"
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
	GameInstanceManagerServiceEntity::GameInstanceManagerServiceEntity( GameID gameID, ClusterID clusterID, ClusterMembership initialMembership )
		: Svr::GameInstanceManagerServiceEntity(gameID, clusterID, initialMembership)
	{
	}


	GameInstanceManagerServiceEntity::~GameInstanceManagerServiceEntity()
	{
	}




	
	Result GameInstanceManagerServiceEntity::RegisterServiceMessageHandler( Svr::ServerEntity *pServerEntity )
	{
		Result hr = ResultCode::SUCCESS;

		svrChk( super::RegisterServiceMessageHandler( pServerEntity ) );


		////////////////////////////////////////////////////////////////////////////////////////////////////
		// We register message handler to server entity. the message transaction will be add to the game instance entity
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







