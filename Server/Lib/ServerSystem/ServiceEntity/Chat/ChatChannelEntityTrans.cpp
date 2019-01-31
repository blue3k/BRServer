////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : game player entity implementation
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"

#include "ResultCode/SFResultCodeLibrary.h"
#include "ResultCode/SFResultCodeGame.h"
#include "ResultCode/SFResultCodeSvr.h"

#include "Memory/SFMemoryPool.h"
#include "Types/BrSvrTypes.h"
#include "GameConst.h"

#include "Server/BrServerUtil.h"
#include "SvrTrace.h"
#include "Protocol/Message/GameServerMsgClass.h"
#include "Protocol/Policy/GameServerNetPolicy.h"
#include "Protocol/Message/ChatChannelMsgClass.h"
#include "Protocol/Policy/ChatChannelNetPolicy.h"
#include "Protocol/ServerService/ChatChannelManagerService.h"

#include "ServiceEntity/Chat/ChatChannelManagerServiceEntity.h"



#include "Server/BrServer.h"

#include "Entity/EntityManager.h"
#include "ServerEntity/ServerEntityManager.h"
#include "ServiceEntity/Chat/ChatChannelEntity.h"
#include "ServiceEntity/Chat/ChatChannelEntityTrans.h"


#include "DB/AccountDB.h"
#include "DB/AccountQuery.h"



SF_MEMORYPOOL_IMPLEMENT(SF::Svr::ChatChannelTransCloseInstance);
SF_MEMORYPOOL_IMPLEMENT(SF::Svr::ChatChannelTransJoin);
SF_MEMORYPOOL_IMPLEMENT(SF::Svr::ChatChannelTransLeave);
SF_MEMORYPOOL_IMPLEMENT(SF::Svr::ChatChannelTransKickPlayer);
SF_MEMORYPOOL_IMPLEMENT(SF::Svr::ChatChannelTransChatMessage);



namespace SF {
namespace Svr {


	Result ChatChannelTransCloseInstance::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		auto* pOwner = (ChatChannelEntity*)GetOwnerEntity();
		ServerServiceInformation *pService = nullptr;

		svrChk(Transaction::StartTransaction() );

		svrChk(Service::ClusterManager->GetRandomService(nullptr, ClusterID::ChatChannelManager, pService));
		svrChk(pService->GetService<Svr::ChatChannelManagerService>()->ChatChannelDeletedC2SEvt(pOwner->GetEntityUID(), 0));

		Service::EntityManager->RemoveEntity( pOwner );

	Proc_End:

		return hr;
	}


	

	// Start Transaction
	Result ChatChannelTransJoin::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		ChatChannelPlayer *pPlayer = nullptr;

		m_LeaderID = 0;

		svrChk( super::StartTransaction() );

		if (StrUtil::StringCompair(GetMyOwner()->GetPasscode(), -1, GetPasscode(), -1))
			svrChkClose(ResultCode::INVALID_PASSWORD);

		if(GetMyOwner()->FindPlayer(GetJoiningPlayer().PlayerID, pPlayer))
		{
			// This should be rejoin by relogging
			Policy::NetSvrPolicyChatChannel *pPolicy = nullptr;

			svrChk( pPlayer->SetServerEntity( GetServerEntity<Svr::ServerEntity>(), GetRouteContext().GetFrom()) );

			Policy::NetSvrPolicyChatChannel policy(pPlayer->GetConnection());

			// Send others to joined
			GetMyOwner()->ForeachPlayer( [&]( ChatChannelPlayer* pOtherPlayer )->Result {
				if( pPlayer != pOtherPlayer )
				{
					policy.PlayerJoinedS2CEvt( pPlayer->GetRouteContext(GetOwnerEntityUID()), pOtherPlayer->GetPlayerInformation() );
				}
				return ResultCode::SUCCESS;
			});


			pPlayer = nullptr;
		}
		else
		{
			svrMem( pPlayer = new(GetHeap()) ChatChannelPlayer( GetJoiningPlayer() ) );
			svrChk( pPlayer->SetServerEntity( GetServerEntity<ServerEntity>(), GetRouteContext().GetFrom()) );
			svrChk( GetMyOwner()->JoinPlayer( pPlayer, false ) );
		}

		m_LeaderID = GetMyOwner()->GetLeaderID();

	Proc_End:

		Util::SafeDelete( pPlayer );
		CloseTransaction( hr );

		return hr;
	}


	// Start Transaction
	Result ChatChannelTransLeave::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		ChatChannelPlayer *pPlayer = nullptr;

		svrChk( super::StartTransaction() );

		svrChk( GetMyOwner()->FindPlayer( GetPlayerID(), pPlayer ) );
		svrChk( GetMyOwner()->LeavePlayer( pPlayer, false ) );

	Proc_End:

		Util::SafeDelete( pPlayer );
		CloseTransaction( hr );

		return hr;
	}


	// Start Transaction
	Result ChatChannelTransKickPlayer::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		ChatChannelPlayer *pPlayer = nullptr;

		svrChk( super::StartTransaction() );


		svrChkClose( GetMyOwner()->FindPlayer( GetPlayerToKick(), pPlayer ) );

		GetMyOwner()->ForeachPlayerSvrChatChannel( [&]( ChatChannelPlayer* pOtherPlayer, Policy::NetSvrPolicyChatChannel &pPolicy )->Result
		{
			pPolicy.PlayerKickedS2CEvt( pOtherPlayer->GetRouteContext(GetOwnerEntityUID()), GetPlayerToKick() );
			return ResultCode::SUCCESS;
		});

		svrChk( GetMyOwner()->LeavePlayer( pPlayer, true ) );

	Proc_End:

		Util::SafeDelete( pPlayer );
		CloseTransaction( hr );

		return hr;
	}
	

	// Start Transaction
	Result ChatChannelTransChatMessage::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		ChatChannelPlayer *pPlayer = nullptr;

		svrChk( super::StartTransaction() );

		svrChk( GetMyOwner()->FindPlayer( GetPlayerID(), pPlayer ) );


		GetMyOwner()->ForeachPlayerSvrChatChannel( [&]( ChatChannelPlayer* pOtherPlayer, Policy::NetSvrPolicyChatChannel &pPolicy )->Result {
				pPolicy.ChatMessageS2CEvt( pOtherPlayer->GetRouteContext(GetOwnerEntityUID()), GetPlayerID(), pPlayer->GetPlayerName(), GetChatMessage() );
				return ResultCode::SUCCESS;
			});

	Proc_End:

		CloseTransaction( hr );

		return hr;
	}
	


};// namespace Svr 
};// namespace SF 

