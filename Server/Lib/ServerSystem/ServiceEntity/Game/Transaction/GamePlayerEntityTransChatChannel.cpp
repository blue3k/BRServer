////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : game player entity implementation
//
////////////////////////////////////////////////////////////////////////////////


#include "ServerSystemPCH.h"

#include "ResultCode/SFResultCodeLibrary.h"
#include "ResultCode/SFResultCodeGame.h"
#include "ResultCode/SFResultCodeLogin.h"
#include "Memory/SFMemoryPool.h"
#include "Types/SFEngineTypedefs.h"

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
#include "ServiceEntity/Game/GamePlayerEntity.h"

#include "ServiceEntity/Game/Subsystem/UserFriendSystem.h"
//#include "ServiceEntity/Game/Subsystem/UserGamePlayerInfoSystem.h"

#include "DB/AccountDB.h"
#include "DB/AccountQuery.h"


namespace SF {
namespace Svr {


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
		ScopeContext hr([this](Result hr)
			{
				CloseTransaction(hr);
			});

		Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;
		Message::ChatChannelManager::CreateChannelRes res;

		svrCheck(pRes->GetResult());
		svrCheck( res.ParseMessage( *pMsgRes->GetMessage() ) );

		GetMyOwner()->SetChatChannelUID( res.GetRouteContext().GetFrom());
		m_ChatChannelUID = res.GetRouteContext().GetFrom();

		return ResultCode::SUCCESS;
	}

	// Start Transaction
	Result PlayerTransCreateOrJoinChatChannel::StartTransaction()
	{
		ScopeContext hr([this](Result hr)
			{
				if (!hr)
					CloseTransaction(hr);
			});
		Svr::ServerServiceInformation *pService = nullptr;

		svrCheck( super::StartTransaction() );

		// Can't do any ChatChannel operations while you are in matching queue
		if( GetMyOwner()->GetMatchingTicket() != 0 )
			svrErrorClose(ResultCode::SVR_ALREADY_INQUEUE);

		if( GetMyOwner()->GetGameInsUID().UID != 0 )
			svrErrorClose(ResultCode::GAME_ALREADY_IN_GAME);

		if( GetMyOwner()->GetChatChannelUID().UID != 0 )
			svrErrorClose(ResultCode::GAME_ALREADY_IN_CHAT);

		svrCheck( Service::ClusterManager->GetRandomService(Svr::GetServerGameID(), ClusterID::ChatChannelManager, pService ) );

		svrCheck( pService->GetService<Svr::ChatChannelManagerService>()->CreateChannelCmd(GetTransID(), 0, GetChannelName(), GetPasscode(), GetMyOwner()->GetPlayerInformation() ) );

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
		ScopeContext hr([this](Result hr)
			{
				CloseTransaction(hr);
			});

		Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;
		Message::ChatChannel::JoinRes res;

		svrCheck(pRes->GetResult());
		svrCheck( res.ParseMessage( *pMsgRes->GetMessage() ) );


		GetMyOwner()->SetChatChannelUID(m_ChatChannelID);

		m_LeaderID = res.GetChatChannelLeaderID();
		m_ChatChannelID = res.GetRouteContext().GetFrom();

		return ResultCode::SUCCESS; 
	}

	// Start Transaction
	Result PlayerTransJoinChatChannel::StartTransaction()
	{
		ScopeContext hr([this](Result hr)
			{
				if (!hr)
					CloseTransaction(hr);
			});
		Svr::ServerEntity *pServerEntity = nullptr;
		//auto config = GetMyServer()->GetPresetGameConfig();

		m_LeaderID = 0;

		svrCheck( super::StartTransaction() );

		if( GetMyOwner()->GetGameInsUID().UID != 0 )
			svrError(ResultCode::GAME_ALREADY_IN_GAME);

		// Can't do any ChatChannel operations while you are in matching queue
		if( GetMyOwner()->GetMatchingTicket() != 0 )
			svrErrorClose(ResultCode::SVR_ALREADY_INQUEUE);

		if( GetMyOwner()->GetChatChannelUID().UID != 0 )
			svrError(ResultCode::GAME_ALREADY_IN_CHAT);

		// Need stamina to join a ChatChannel
		//if (config != nullptr)
		//{
		//	//svrError(ResultCode::INVALID_STATE);
		//	GetMyOwner()->UpdateGamePlayer();
		//	if (GetMyOwner()->GetComponent<UserGamePlayerInfoSystem>()->GetStamina() < config->StaminaForGame)
		//		svrErrorClose(ResultCode::GAME_NOTENOUGH_RESOURCE);
		//}

		svrCheck( Service::ServerEntityManager->GetServerEntity( EntityUID(GetChatUID()).GetServerID(), pServerEntity ) );

		svrError(ResultCode::NOT_IMPLEMENTED);
		//svrCheck(Policy::NetPolicyChatChannel(pServerEntity->GetConnection()).JoinChatChannelCmd(RouteContext(GetOwnerEntityUID(), GetChatUID()), GetTransID(), GetMyOwner()->GetPlayerInformation() ) );

		return hr;
	}
	

	// Start Transaction
	Result PlayerTransChatChannelPlayerJoinedS2SEvt::StartTransaction()
	{
		ScopeContext hr([this](Result hr)
			{
				CloseTransaction(hr);
			});

		svrCheck( super::StartTransaction() );

		if( GetMyOwner()->GetChatChannelUID() != GetRouteContext().GetFrom())
			svrErrorClose(ResultCode::INVALID_ENTITY);

		svrCheck(Policy::NetSvrPolicyGame(GetMyOwner()->GetConnection()).ChatChannelPlayerJoinedS2CEvt( GetRouteContext().GetFrom(), GetJoinedPlayer() ) );

		return hr;
	}


	// Start Transaction
	Result PlayerTransChatChannelLeaderChangedS2SEvt::StartTransaction()
	{
		ScopeContext hr([this](Result hr)
			{
				CloseTransaction(hr);
			});

		svrCheck( super::StartTransaction() );

		if( GetMyOwner()->GetChatChannelUID() != GetRouteContext().GetFrom())
			svrErrorClose(ResultCode::INVALID_ENTITY);

		svrCheck(Policy::NetSvrPolicyGame(GetMyOwner()->GetConnection()).ChatChannelLeaderChangedS2CEvt( GetRouteContext().GetFrom(), GetNewLeaderID() ) );
	
		return hr;
	}



	PlayerTransLeaveChatChannel::PlayerTransLeaveChatChannel(IHeap& heap, MessageDataPtr &pIMsg )
		:MessageTransaction( heap, std::forward<MessageDataPtr>(pIMsg) )
	{
		BR_TRANS_MESSAGE( Message::ChatChannel::LeaveRes, { return OnLeaveChatChannelRes(pRes); } );
	}

	Result PlayerTransLeaveChatChannel::OnLeaveChatChannelRes( Svr::TransactionResult* &pRes )
	{
		ScopeContext hr([this](Result hr)
			{
				CloseTransaction(hr);
			});

		Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;
		Message::ChatChannel::LeaveRes res;

		if (pRes->GetResult() == Result(ResultCode::SVR_INVALID_ENTITYUID))
		{
			GetMyOwner()->SetChatChannelUID(0);
			GetMyOwner()->UpdateDBSync();
		}
		else
		{
			svrCheck(pRes->GetResult());
			svrCheck(res.ParseMessage(*pMsgRes->GetMessage()));

			GetMyOwner()->SetChatChannelUID(0);
			GetMyOwner()->UpdateDBSync();
		}

		return ResultCode::SUCCESS; 
	}

	// Start Transaction
	Result PlayerTransLeaveChatChannel::StartTransaction()
	{
		ScopeContext hr([this](Result hr)
			{
				if (!hr)
					CloseTransaction(hr);
			});
		Svr::ServerEntity *pServerEntity = nullptr;

		svrCheck( super::StartTransaction() );

		// Can't do any ChatChannel operations while you are in matching queue
		if( GetMyOwner()->GetMatchingTicket() != 0 )
			svrErrorClose(ResultCode::SVR_ALREADY_INQUEUE);

		if( GetMyOwner()->GetChatChannelUID() == EntityUID(0) )
			svrError(ResultCode::INVALID_ENTITY);

		svrCheck( Service::ServerEntityManager->GetServerEntity( EntityUID(GetChatUID()).GetServerID(), pServerEntity ) );

		svrCheck(Policy::NetPolicyChatChannel(pServerEntity->GetConnection()).LeaveCmd( RouteContext(GetOwnerEntityUID(),GetMyOwner()->GetChatChannelUID()), GetTransID(),
			GetMyOwner()->GetPlayerID() ) );
	
		return hr;
	}
	
	
	
	// Start Transaction
	Result PlayerTransChatChannelPlayerLeftS2SEvt::StartTransaction()
	{
		ScopeContext hr([this](Result hr)
			{
				CloseTransaction(hr);
			});

		svrCheck( super::StartTransaction() );

		if( GetMyOwner()->GetChatChannelUID() != GetRouteContext().GetFrom())
			svrErrorClose(ResultCode::INVALID_ENTITY);

		svrCheck(Policy::NetSvrPolicyGame(GetMyOwner()->GetConnection()).ChatChannelPlayerLeftS2CEvt( GetRouteContext().GetFrom(), GetLeftPlayerID() ) );

		return hr;
	}
	


	
	

	PlayerTransChatChannelKickPlayer::PlayerTransChatChannelKickPlayer(IHeap& heap, MessageDataPtr &pIMsg )
		: MessageTransaction( heap, std::forward<MessageDataPtr>(pIMsg) )
	{
		BR_TRANS_MESSAGE( Message::ChatChannel::KickPlayerRes, { return OnPlayerKickRes(pRes); } );
	}

	Result PlayerTransChatChannelKickPlayer::OnPlayerKickRes( Svr::TransactionResult* &pRes )
	{
		ScopeContext hr([this](Result hr)
			{
				CloseTransaction(hr);
			});

		//Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;
		//Message::ChatChannel::KickPlayerRes res;

		svrCheckClose(pRes->GetResult());
		//svrCheck( res.ParseMessage( pMsgRes->GetMessage() ) );

		return ResultCode::SUCCESS; 
	}

	// Start Transaction
	Result PlayerTransChatChannelKickPlayer::StartTransaction()
	{
		ScopeContext hr([this](Result hr)
			{
				if (!hr)
					CloseTransaction(hr);
			});
		Svr::ServerEntity *pServerEntity = nullptr;

		svrCheck( super::StartTransaction() );

		// Can't do any ChatChannel operations while you are in matching queue
		if( GetMyOwner()->GetMatchingTicket() != 0 )
			svrErrorClose(ResultCode::SVR_ALREADY_INQUEUE);

		if( GetMyOwner()->GetChatChannelUID().UID != EntityUID() )
			svrError(ResultCode::INVALID_ENTITY);

		svrCheck( Service::ServerEntityManager->GetServerEntity( EntityUID(GetChatUID()).GetServerID(), pServerEntity ) );

		svrCheck(Policy::NetPolicyChatChannel(pServerEntity->GetConnection()).KickPlayerCmd( RouteContext(GetOwnerEntityUID(),GetChatUID()), GetTransID(),
			GetMyOwner()->GetPlayerID(), GetPlayerToKick() ) );

		return hr;
	}
	
	
	
	// Start Transaction
	Result PlayerTransChatChannelPlayerKickedS2SEvt::StartTransaction()
	{
		ScopeContext hr([this](Result hr)
			{
				CloseTransaction(hr);
			});

		svrCheck( super::StartTransaction() );

		if( GetMyOwner()->GetChatChannelUID() != GetRouteContext().GetFrom())
			svrErrorClose(ResultCode::INVALID_ENTITY);

		svrCheck( Policy::NetSvrPolicyGame(GetMyOwner()->GetConnection()).ChatChannelPlayerKickedS2CEvt( GetRouteContext().GetFrom(), GetKickedPlayerID() ) );

		if( GetKickedPlayerID() == GetMyOwner()->GetPlayerID() )
		{
			GetMyOwner()->SetChatChannelUID(0);
			GetMyOwner()->UpdateDBSync();
		}
	
		return ResultCode::SUCCESS;
	}
	

	// Start Transaction
	Result PlayerTransChatChannelChatMessage::StartTransaction()
	{
		ScopeContext hr([this](Result hr)
			{
				if (!hr)
					CloseTransaction(hr);
			});

		Svr::ServerEntity* pServerEntity = nullptr;

		svrCheck(super::StartTransaction());

		// Can't do any ChatChannel operations while you are in matching queue
		if (GetMyOwner()->GetChatChannelUID().UID != EntityUID())
			svrError(ResultCode::INVALID_ENTITY);

		svrCheck(Service::ServerEntityManager->GetServerEntity(EntityUID(GetChatUID()).GetServerID(), pServerEntity));

		svrCheck(Policy::NetPolicyChatChannel(pServerEntity->GetConnection()).ChatMessageC2SEvt(RouteContext(GetOwnerEntityUID(), GetChatUID()), GetMyOwner()->GetPlayerID(),
			GetChatMessage()));

		return hr;
	}

	// Start Transaction
	Result PlayerTransChatChannelChatMessageS2CEvt::StartTransaction()
	{
		ScopeContext hr([this](Result hr)
			{
				CloseTransaction(hr);
			});

		svrCheck(super::StartTransaction());

		if (GetMyOwner()->GetChatChannelUID() != GetRouteContext().GetFrom())
			svrErrorClose(ResultCode::INVALID_ENTITY);

		svrCheck(Policy::NetSvrPolicyGame(GetMyOwner()->GetConnection()).ChatChannelChatMessageS2CEvt(GetSenderID(), GetSenderName(), GetChatMessage()));

		return ResultCode::SUCCESS;
	}


}// namespace Svr 
}// namespace SF 

