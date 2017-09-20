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
#include "GameServer.h"
#include "GameServerClass.h"

#include "ResultCode/SFResultCodeLibrary.h"
#include "ResultCode/SFResultCodeGame.h"
#include "ResultCode/SFResultCodeLogin.h"
#include "Memory/MemoryPool.h"
#include "Types/SFEngineTypedefs.h"

#include "GameServerClass.h"
#include "Server/BrServerUtil.h"
#include "SvrTrace.h"
#include "ServerEntity/ServerEntityManager.h"

#include "Protocol/ServerService/PartyMatchingQueueService.h"
#include "ServiceEntity/MatchingQueueServiceEntity.h"
#include "ServiceEntity/Game/PlayerManagerServiceEntity.h"

#include "Protocol/Message/PartyMatchingQueueMsgClass.h"

#include "Protocol/Message/GamePartyMsgClass.h"
#include "Protocol/Policy/GamePartyNetPolicy.h"

#include "Protocol/Message/GameInstanceMsgClass.h"
#include "Protocol/Policy/GameInstanceNetPolicy.h"

#include "GamePlayerEntityTransClose.h"
#include "GameInstance/GamePlayerEntity.h"

//#include "PlayerInfoManager.h"
//#include "GameInstance/GamePlayer.h"
#include "Server/BrServer.h"


#include "GameSystem/UserFriendSystem.h"
#include "GameSystem/UserGamePlayerInfoSystem.h"


#include "DB/GameConspiracyDB.h"
#include "DB/GameConspiracyQuery.h"
#include "DB/LoginSessionDB.h"
#include "DB/LoginSessionQuery.h"


SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransCloseInstance);


	
namespace SF {
namespace GameServer {


	PlayerTransCloseInstance::PlayerTransCloseInstance(IHeap& heap)
		: TransactionT(heap, TransactionID() )
	{
		SetExclusive(true);
		BR_TRANS_MESSAGE(DB::QueryUpdateTickStatusCmd, { return UpdateDBRes(pRes); });
		BR_TRANS_MESSAGE(DB::QueryDeleteLoginSessionCmd, { return DeleteLoginSessionRes(pRes); });
		BR_TRANS_MESSAGE(Message::PartyMatchingQueue::UnregisterMatchingRes, { return UnregisterMatchingRes(pRes); });
		BR_TRANS_MESSAGE(Message::GameInstance::LeaveGameRes, { return LeaveGameRes(pRes); });
		BR_TRANS_MESSAGE(Message::GameParty::LeavePartyRes, { return LeavePartyRes(pRes); });
	}

	Result PlayerTransCloseInstance::UpdateDBRes(Svr::TransactionResult* &pRes)
	{
		Result hr = ResultCode::SUCCESS;

		m_WaitingTransactions--;

		svrChk(pRes->GetResult());

	Proc_End:

		if (m_WaitingTransactions <= 0)
			CloseTransaction(hr);

		return ResultCode::SUCCESS;
	}

	Result PlayerTransCloseInstance::DeleteLoginSessionRes(Svr::TransactionResult* &pRes)
	{
		Result hr = ResultCode::SUCCESS;

		m_WaitingTransactions--;

		svrChk(pRes->GetResult());

	Proc_End:

		if (m_WaitingTransactions <= 0)
			CloseTransaction(hr);

		return hr;
	}

	Result PlayerTransCloseInstance::UnregisterMatchingRes( Svr::TransactionResult* & pRes )
	{
		Result hr = ResultCode::SUCCESS;

		//Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;
		//Message::PartyMatchingQueue::UnregisterMatchingRes res;

		m_WaitingTransactions--;

		GetMyOwner()->SetMatchingTicket(0);

		//svrChk(pRes->GetResult());
		if (!(pRes->GetResult()))
			svrTrace(SVR_INFO, "Unregister Matching is failed hr:{0:X8}", pRes->GetResult());

	//Proc_End:

		if (m_WaitingTransactions <= 0)
			CloseTransaction(hr);

		return hr; 
	}

	Result PlayerTransCloseInstance::LeaveGameRes(Svr::TransactionResult* & pRes)
	{
		Result hr = ResultCode::SUCCESS;

		//Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;
		//Message::GameInstance::LeaveGameRes res;

		m_WaitingTransactions--;

		GetMyOwner()->SetGameInsUID(0);

		svrChk(pRes->GetResult());

	Proc_End:

		if (m_WaitingTransactions <= 0)
			CloseTransaction(hr);

		return hr;
	}

	Result PlayerTransCloseInstance::LeavePartyRes(Svr::TransactionResult* & pRes)
	{
		Result hr = ResultCode::SUCCESS;

		//Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;
		//Message::GameParty::LeavePartyRes res;

		m_WaitingTransactions--;

		GetMyOwner()->SetPartyUID(0);

		svrChk(pRes->GetResult());

	Proc_End:

		if (m_WaitingTransactions <= 0)
			CloseTransaction(hr);

		return hr;
	}

	Result PlayerTransCloseInstance::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		GamePlayerEntity* pOwner = (GamePlayerEntity*)GetOwnerEntity();

		m_WaitingTransactions = 0;

		svrChk( super::StartTransaction() );

		GetMyOwner()->ReleaseConnection("Closing Game Player entity");

		if((pOwner->UpdateDBSync(GetTransID())))
			m_WaitingTransactions++;

		if( pOwner->GetMatchingTicket() != 0 )
		{
			if((Policy::NetPolicyPartyMatchingQueue(Service::ServerEntityManager->GetServerConnection(pOwner->GetMatchingTicket().QueueUID.GetServerID())).UnregisterMatchingCmd(
				RouteContext(GetOwnerEntityUID(), pOwner->GetMatchingTicket().QueueUID), GetTransID(), 
				0, pOwner->GetMatchingTicket())))
				m_WaitingTransactions++;
		}

		if (pOwner->GetPartyUID().UID != 0)
		{
			if ((Policy::NetPolicyGameParty(Service::ServerEntityManager->GetServerConnection(pOwner->GetPartyUID().GetServerID())).LeavePartyCmd(
				RouteContext(GetOwnerEntityUID(), pOwner->GetPartyUID()), GetTransID(), pOwner->GetPlayerID())))
				m_WaitingTransactions++;
		}

		if (pOwner->GetGameInsUID().UID != 0)
		{
			if ((Policy::NetPolicyGameInstance(Service::ServerEntityManager->GetServerConnection(pOwner->GetGameInsUID().GetServerID())).LeaveGameCmd(
				RouteContext(GetOwnerEntityUID(), pOwner->GetGameInsUID()), GetTransID(), pOwner->GetPlayerID())))
				m_WaitingTransactions++;
		}

		if ((Svr::GetServerComponent<DB::LoginSessionDB>()->DeleteLoginSession(GetTransID(), GetMyOwner()->GetPlayerID(), GetMyOwner()->GetAuthTicket())))
			m_WaitingTransactions++;

		svrChk(Service::PlayerManager->DeletePlayer(GetMyOwner()->GetPlayerID()));


	Proc_End:

		if (!(hr) && hr != Result(ResultCode::INVALID_PLAYERID))
		{
			svrTrace(Error, "Failed to close entity:{0}, hr:{1:X8}", GetOwnerEntityUID(), hr);
		}

		if (m_WaitingTransactions <= 0)
			CloseTransaction(hr);

		return hr;
	}

	Result PlayerTransCloseInstance::OnCloseTransaction( Result hrRes )
	{
		GetMyOwner()->AddGameTransactionLog(TransLogCategory::Account, -2, 0, 0, "EntityClose");

		Service::EntityManager->RemoveEntity(GetMyOwner());

		return ResultCode::SUCCESS;
	}






};// namespace GameServer 
};// namespace SF 

