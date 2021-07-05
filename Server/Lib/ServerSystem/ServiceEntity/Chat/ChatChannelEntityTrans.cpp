////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) The Braves
// 
// Author : KyungKun Ko
//
// Description : game player entity implementation
//
////////////////////////////////////////////////////////////////////////////////


#include "ServerSystemPCH.h"

#include "ResultCode/SFResultCodeLibrary.h"
#include "ResultCode/SFResultCodeGame.h"
#include "ResultCode/SFResultCodeSvr.h"

#include "MemoryManager/SFMemoryPool.h"
#include "Types/BrSvrTypes.h"
#include "GameConst.h"

#include "Server/BrServerUtil.h"
#include "SvrTrace.h"
#include "Protocol/GameServerMsgClass.h"
#include "Protocol/GameServerNetPolicy.h"
#include "Protocol/ChatChannelMsgClass.h"
#include "Protocol/ChatChannelNetPolicy.h"
#include "Protocol/ChatChannelManagerService.h"

#include "ServiceEntity/Chat/ChatChannelManagerServiceEntity.h"



#include "Server/BrServer.h"

#include "Entity/EntityManager.h"
#include "ServiceEntity/Chat/ChatChannelEntity.h"
#include "ServiceEntity/Chat/ChatChannelEntityTrans.h"


#include "DB/AccountDB.h"
#include "DB/AccountQuery.h"






namespace SF {
namespace Svr {


	Result ChatChannelTransCloseInstance::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		auto* pOwner = (ChatChannelEntity*)GetOwnerEntity();
		SharedPointerT<ServerServiceInformation> pService = nullptr;

		svrCheck(Transaction::StartTransaction());

		// TODO: move to server instance 
		//svrCheck(Service::ChatChannelManager->RemoveChannel(pOwner));
		//svrCheck(Service::ServiceDirectory->RemoveLocalService(pOwner));
		//svrCheck(Service::ServiceDirectory->GetRandomService(nullptr, ClusterID::ChatChannel, pService));
		//svrCheck(pService->GetService<ChatChannelManagerService>()->ChatChannelDeletedC2SEvt(pOwner->GetEntityUID(), 0));

		Service::EntityManager->RemoveEntity( pOwner );

		return hr;
	}


	

	// Start Transaction
	Result ChatChannelTransJoin::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		UniquePtr<ChatChannelPlayer> pPlayer;

		m_LeaderID = 0;

		svrCheck( super::StartTransaction() );

		if (StrUtil::StringCompair(GetMyOwner()->GetPasscode(), -1, GetPasscode(), -1))
			svrCheckClose(ResultCode::INVALID_PASSWORD);

		ChatChannelPlayer* pFound{};
		if(GetMyOwner()->FindPlayer(GetJoiningPlayer().PlayerID, pFound))
		{
			// This should be rejoin by relogging
			svrCheck(pFound->SetRemoteEndpoint(GetRouteContext().GetFrom(), GetRemoteEndpoint()));

			NetSvrPolicyChatChannel policy(pFound->GetRemoteEndpoint());

			// Send others to joined
			GetMyOwner()->ForeachPlayer( [&]( ChatChannelPlayer* pOtherPlayer )->Result {
				if(pFound != pOtherPlayer )
				{
					policy.PlayerJoinedS2CEvt(pFound->GetRouteContext(GetOwnerEntityUID()), pOtherPlayer->GetPlayerInformation());
				}
				return ResultCode::SUCCESS;
			});
		}
		else
		{
			pPlayer.reset(new(GetMyOwner()->GetHeap()) ChatChannelPlayer(GetMyOwner()->GetHeap(), GetRouteContext().GetFrom(), GetJoiningPlayer()));
			svrCheckMem(pPlayer);
			svrCheck( pPlayer->SetRemoteEndpoint(GetRouteContext().GetFrom(), GetRemoteEndpoint()) );
			svrCheck( GetMyOwner()->JoinPlayer( pPlayer.get(), false ) );
			pPlayer.release();
		}

		m_LeaderID = GetMyOwner()->GetLeaderID();

		CloseTransaction( hr );

		return hr;
	}


	// Start Transaction
	Result ChatChannelTransLeave::StartTransaction()
	{
		ChatChannelPlayer* pPlayer = nullptr;
		ScopeContext hr([&pPlayer](Result hr)
			{
				if (pPlayer)
					IHeap::Delete(pPlayer);
			});

		svrCheck( super::StartTransaction() );

		svrCheck( GetMyOwner()->FindPlayer( GetPlayerID(), pPlayer ) );
		svrCheck( GetMyOwner()->LeavePlayer( pPlayer, false ) );
		pPlayer = nullptr;

		return hr;
	}


	// Start Transaction
	Result ChatChannelTransKickPlayer::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		ChatChannelPlayer *pPlayer = nullptr;

		svrChk( super::StartTransaction() );


		svrChkClose( GetMyOwner()->FindPlayer( GetPlayerToKick(), pPlayer ) );

		GetMyOwner()->ForeachPlayerSvrChatChannel( [&]( ChatChannelPlayer* pOtherPlayer, NetSvrPolicyChatChannel &pPolicy )->Result
		{
			pPolicy.PlayerKickedS2CEvt( pOtherPlayer->GetRouteContext(GetOwnerEntityUID()), GetPlayerToKick() );
			return ResultCode::SUCCESS;
		});

		svrChk( GetMyOwner()->LeavePlayer( pPlayer, true ) );
		pPlayer = nullptr;

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


		GetMyOwner()->ForeachPlayerSvrChatChannel( [&]( ChatChannelPlayer* pOtherPlayer, NetSvrPolicyChatChannel &pPolicy )->Result {
				pPolicy.ChatMessageS2CEvt( pOtherPlayer->GetRouteContext(GetOwnerEntityUID()), GetPlayerID(), pPlayer->GetPlayerName(), GetChatMessage() );
				return ResultCode::SUCCESS;
			});

	Proc_End:

		CloseTransaction( hr );

		return hr;
	}
	


}// namespace Svr 
}// namespace SF 

