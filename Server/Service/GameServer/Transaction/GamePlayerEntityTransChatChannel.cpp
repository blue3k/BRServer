////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : game player entity implementation
//
////////////////////////////////////////////////////////////////////////////////


#include "GameServerPCH.h"
#include "GameServer.h"
#include "GameServerClass.h"

#include "ResultCode/SFResultCodeLibrary.h"
#include "ResultCode/SFResultCodeGame.h"
#include "ResultCode/SFResultCodeLogin.h"
#include "Memory/SFMemoryPool.h"
#include "Types/SFEngineTypedefs.h"

#include "GameServerClass.h"
#include "Server/BrServerUtil.h"
#include "SvrTrace.h"
#include "Server/BrServer.h"
#include "ServerEntity/ServerEntityManager.h"

#include "ServiceEntity/Chat/ChatChannelManagerServiceEntity.h"
#include "Protocol/ServerService/ChatChannelManagerService.h"
#include "ServiceEntity/Game/PlayerManagerServiceEntity.h"

#include "Protocol/Message/GameInstanceMsgClass.h"
#include "Protocol/Policy/GameInstanceNetPolicy.h"

#include "Protocol/Message/ChatChannelManagerMsgClass.h"
#include "Protocol/Policy/ChatChannelManagerNetPolicy.h"

#include "Protocol/Message/ChatChannelMsgClass.h"
#include "Protocol/Policy/ChatChannelNetPolicy.h"

#include "GamePlayerEntityTransChatChannel.h"
#include "GameInstance/GamePlayerEntity.h"

#include "GameSystem/UserFriendSystem.h"
#include "GameSystem/UserGamePlayerInfoSystem.h"

#include "DB/AccountDB.h"
#include "DB/AccountQuery.h"
#include "DB/GameConspiracyDB.h"
#include "DB/GameConspiracyQuery.h"


SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransCreateOrJoinChatChannel);
SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransJoinChatChannel);
SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransChatChannelPlayerJoinedS2SEvt);
SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransChatChannelLeaderChangedS2SEvt);
SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransLeaveChatChannel);
SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransChatChannelPlayerLeftS2SEvt);
SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransChatChannelKickPlayer);
SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransChatChannelPlayerKickedS2SEvt);
SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransChatChannelChatMessage);
SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransChatChannelChatMessageS2CEvt);




namespace SF {
namespace GameServer {


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	ChatChannel
	//



	PlayerTransCreateOrJoinChatChannel::PlayerTransCreateOrJoinChatChannel(IHeap& heap, MessageDataPtr &pIMsg )
		:MessageTransaction(heap, std::forward<MessageDataPtr>(pIMsg) )
	{
		BR_TRANS_MESSAGE( Message::ChatChannelManager::CreateChannelCmd, { return OnCreateRes(pRes); } );
	}

	Result PlayerTransCreateOrJoinChatChannel::OnCreateRes( Svr::TransactionResult* &pRes )
	{
		Result hr = ResultCode::SUCCESS;

		Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;
		Message::ChatChannelManager::CreateChannelRes res;

		svrChk(pRes->GetResult());
		svrChk( res.ParseMessage( *pMsgRes->GetMessage() ) );

		GetMyOwner()->SetChatChannelUID( res.GetRouteContext().GetFrom());
		m_ChatChannelUID = res.GetRouteContext().GetFrom();

	Proc_End:

		CloseTransaction(hr);

		return ResultCode::SUCCESS;
	}

	// Start Transaction
	Result PlayerTransCreateOrJoinChatChannel::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		Svr::ServerServiceInformation *pService = nullptr;

		svrChk( super::StartTransaction() );

		// Can't do any ChatChannel operations while you are in matching queue
		if( GetMyOwner()->GetMatchingTicket() != 0 )
			svrErrClose(ResultCode::SVR_ALREADY_INQUEUE);

		if( GetMyOwner()->GetGameInsUID().UID != 0 )
			svrErrClose(ResultCode::GAME_ALREADY_IN_GAME);

		if( GetMyOwner()->GetChatChannelUID().UID != 0 )
			svrErrClose(ResultCode::GAME_ALREADY_IN_CHAT);

		svrChk( Service::ClusterManager->GetRandomService(Svr::GetServerGameID(), ClusterID::ChatChannelManager, pService ) );

		svrChk( pService->GetService<Svr::ChatChannelManagerService>()->CreateChannelCmd(GetTransID(), 0, GetChannelName(), GetPasscode(), GetMyOwner()->GetPlayerInformation() ) );

	Proc_End:

		if( !(hr) )
			CloseTransaction( hr );

		return hr;
	}




	PlayerTransJoinChatChannel::PlayerTransJoinChatChannel(IHeap& heap, MessageDataPtr &pIMsg )
		: MessageTransaction( heap, std::forward<MessageDataPtr>(pIMsg) )
	{
		BR_TRANS_MESSAGE(Message::ChatChannel::JoinRes, { return OnJoinChatChannelRes(pRes); });
		BR_TRANS_MESSAGE(Message::ChatChannelManager::CreateChannelRes, { return OnJoinChatChannelRes(pRes); });
	}

	Result PlayerTransJoinChatChannel::OnJoinChatChannelRes( Svr::TransactionResult* &pRes )
	{
		Result hr = ResultCode::SUCCESS;

		Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;
		Message::ChatChannel::JoinRes res;

		svrChk(pRes->GetResult());
		svrChk( res.ParseMessage( *pMsgRes->GetMessage() ) );


		GetMyOwner()->SetChatChannelUID(m_ChatChannelID);

		m_LeaderID = res.GetChatChannelLeaderID();
		m_ChatChannelID = res.GetRouteContext().GetFrom();


	Proc_End:

		CloseTransaction(hr);

		return ResultCode::SUCCESS; 
	}

	// Start Transaction
	Result PlayerTransJoinChatChannel::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		Svr::ServerEntity *pServerEntity = nullptr;
		auto config = GetMyServer()->GetPresetGameConfig();

		m_LeaderID = 0;

		svrChk( super::StartTransaction() );

		if( GetMyOwner()->GetGameInsUID().UID != 0 )
			svrErr(ResultCode::GAME_ALREADY_IN_GAME);

		// Can't do any ChatChannel operations while you are in matching queue
		if( GetMyOwner()->GetMatchingTicket() != 0 )
			svrErrClose(ResultCode::SVR_ALREADY_INQUEUE);

		if( GetMyOwner()->GetChatChannelUID().UID != 0 )
			svrErr(ResultCode::GAME_ALREADY_IN_CHAT);

		// Need stamina to join a ChatChannel
		if (config != nullptr)
		{
			//svrErr(ResultCode::INVALID_STATE);
			GetMyOwner()->UpdateGamePlayer();
			if (GetMyOwner()->GetComponent<UserGamePlayerInfoSystem>()->GetStamina() < config->StaminaForGame)
				svrErrClose(ResultCode::GAME_NOTENOUGH_RESOURCE);
		}

		svrChk( Service::ServerEntityManager->GetServerEntity( EntityUID(GetChatUID()).GetServerID(), pServerEntity ) );

		svrErr(ResultCode::NOT_IMPLEMENTED);
		//svrChk(Policy::NetPolicyChatChannel(pServerEntity->GetConnection()).JoinChatChannelCmd(RouteContext(GetOwnerEntityUID(), GetChatUID()), GetTransID(), GetMyOwner()->GetPlayerInformation() ) );

	Proc_End:

		if( !(hr) )
			CloseTransaction( hr );

		return hr;
	}
	

	// Start Transaction
	Result PlayerTransChatChannelPlayerJoinedS2SEvt::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk( super::StartTransaction() );

		if( GetMyOwner()->GetChatChannelUID() != GetRouteContext().GetFrom())
			svrErrClose(ResultCode::INVALID_ENTITY);

		svrChk(Policy::NetSvrPolicyGame(GetMyOwner()->GetConnection()).ChatChannelPlayerJoinedS2CEvt( GetRouteContext().GetFrom(), GetJoinedPlayer() ) );
	
	Proc_End:

		CloseTransaction( hr );

		return hr;
	}


	// Start Transaction
	Result PlayerTransChatChannelLeaderChangedS2SEvt::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk( super::StartTransaction() );

		if( GetMyOwner()->GetChatChannelUID() != GetRouteContext().GetFrom())
			svrErrClose(ResultCode::INVALID_ENTITY);

		svrChk(Policy::NetSvrPolicyGame(GetMyOwner()->GetConnection()).ChatChannelLeaderChangedS2CEvt( GetRouteContext().GetFrom(), GetNewLeaderID() ) );
	
	Proc_End:

		CloseTransaction( hr );

		return hr;
	}



	PlayerTransLeaveChatChannel::PlayerTransLeaveChatChannel(IHeap& heap, MessageDataPtr &pIMsg )
		:MessageTransaction( heap, std::forward<MessageDataPtr>(pIMsg) )
	{
		BR_TRANS_MESSAGE( Message::ChatChannel::LeaveRes, { return OnLeaveChatChannelRes(pRes); } );
	}

	Result PlayerTransLeaveChatChannel::OnLeaveChatChannelRes( Svr::TransactionResult* &pRes )
	{
		Result hr = ResultCode::SUCCESS;

		Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;
		Message::ChatChannel::LeaveRes res;

		if (pRes->GetResult() == Result(ResultCode::SVR_INVALID_ENTITYUID))
		{
			GetMyOwner()->SetChatChannelUID(0);
			GetMyOwner()->UpdateDBSync();
		}
		else
		{
			svrChk(pRes->GetResult());
			svrChk(res.ParseMessage(*pMsgRes->GetMessage()));

			GetMyOwner()->SetChatChannelUID(0);
			GetMyOwner()->UpdateDBSync();
		}

	Proc_End:

		CloseTransaction(hr);

		return ResultCode::SUCCESS; 
	}

	// Start Transaction
	Result PlayerTransLeaveChatChannel::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		Svr::ServerEntity *pServerEntity = nullptr;

		svrChk( super::StartTransaction() );

		// Can't do any ChatChannel operations while you are in matching queue
		if( GetMyOwner()->GetMatchingTicket() != 0 )
			svrErrClose(ResultCode::SVR_ALREADY_INQUEUE);

		if( GetMyOwner()->GetChatChannelUID() == EntityUID(0) )
			svrErr(ResultCode::INVALID_ENTITY);

		svrChk( Service::ServerEntityManager->GetServerEntity( EntityUID(GetChatUID()).GetServerID(), pServerEntity ) );

		svrChk(Policy::NetPolicyChatChannel(pServerEntity->GetConnection()).LeaveCmd( RouteContext(GetOwnerEntityUID(),GetMyOwner()->GetChatChannelUID()), GetTransID(),
			GetMyOwner()->GetPlayerID() ) );
	
	Proc_End:

		if( !(hr) )
			CloseTransaction( hr );

		return hr;
	}
	
	
	
	// Start Transaction
	Result PlayerTransChatChannelPlayerLeftS2SEvt::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk( super::StartTransaction() );

		if( GetMyOwner()->GetChatChannelUID() != GetRouteContext().GetFrom())
			svrErrClose(ResultCode::INVALID_ENTITY);

		svrChk(Policy::NetSvrPolicyGame(GetMyOwner()->GetConnection()).ChatChannelPlayerLeftS2CEvt( GetRouteContext().GetFrom(), GetLeftPlayerID() ) );
	
	Proc_End:

		CloseTransaction( hr );

		return hr;
	}
	


	
	

	PlayerTransChatChannelKickPlayer::PlayerTransChatChannelKickPlayer(IHeap& heap, MessageDataPtr &pIMsg )
		: MessageTransaction( heap, std::forward<MessageDataPtr>(pIMsg) )
	{
		BR_TRANS_MESSAGE( Message::ChatChannel::KickPlayerRes, { return OnPlayerKickRes(pRes); } );
	}

	Result PlayerTransChatChannelKickPlayer::OnPlayerKickRes( Svr::TransactionResult* &pRes )
	{
		Result hr = ResultCode::SUCCESS;

		//Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;
		//Message::ChatChannel::KickPlayerRes res;

		svrChkClose(pRes->GetResult());
		//svrChk( res.ParseMessage( pMsgRes->GetMessage() ) );

	Proc_End:

		CloseTransaction(hr);

		return ResultCode::SUCCESS; 
	}

	// Start Transaction
	Result PlayerTransChatChannelKickPlayer::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		Svr::ServerEntity *pServerEntity = nullptr;

		svrChk( super::StartTransaction() );

		// Can't do any ChatChannel operations while you are in matching queue
		if( GetMyOwner()->GetMatchingTicket() != 0 )
			svrErrClose(ResultCode::SVR_ALREADY_INQUEUE);

		if( GetMyOwner()->GetChatChannelUID().UID != EntityUID() )
			svrErr(ResultCode::INVALID_ENTITY);

		svrChk( Service::ServerEntityManager->GetServerEntity( EntityUID(GetChatUID()).GetServerID(), pServerEntity ) );

		svrChk(Policy::NetPolicyChatChannel(pServerEntity->GetConnection()).KickPlayerCmd( RouteContext(GetOwnerEntityUID(),GetChatUID()), GetTransID(),
			GetMyOwner()->GetPlayerID(), GetPlayerToKick() ) );
	
	Proc_End:

		if( !(hr) )
			CloseTransaction( hr );

		return hr;
	}
	
	
	
	// Start Transaction
	Result PlayerTransChatChannelPlayerKickedS2SEvt::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk( super::StartTransaction() );

		if( GetMyOwner()->GetChatChannelUID() != GetRouteContext().GetFrom())
			svrErrClose(ResultCode::INVALID_ENTITY);

		svrChk( Policy::NetSvrPolicyGame(GetMyOwner()->GetConnection()).ChatChannelPlayerKickedS2CEvt( GetRouteContext().GetFrom(), GetKickedPlayerID() ) );

		if( GetKickedPlayerID() == GetMyOwner()->GetPlayerID() )
		{
			GetMyOwner()->SetChatChannelUID(0);
			GetMyOwner()->UpdateDBSync();
		}
	
	Proc_End:

		CloseTransaction( hr );

		return ResultCode::SUCCESS;
	}
	


};// namespace GameServer 
};// namespace SF 

