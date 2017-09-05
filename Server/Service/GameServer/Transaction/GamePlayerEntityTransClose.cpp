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

#include "ServerSystem/ServerService/PartyMatchingQueueService.h"
#include "ServerSystem/ServiceEntity/MatchingQueueServiceEntity.h"
#include "ServerSystem/ServiceEntity/Game/GameClusterServiceEntity.h"

#include "Protocol/Message/PartyMatchingQueueMsgClass.h"

#include "Protocol/Message/GamePartyMsgClass.h"
#include "Protocol/Policy/GamePartyNetPolicy.h"

#include "Protocol/Message/GameServerMsgClass.h"
#include "Protocol/Policy/GameServerNetPolicy.h"
#include "Protocol/Message/GameInstanceMsgClass.h"
#include "Protocol/Policy/GameInstanceNetPolicy.h"

#include "GamePlayerEntityTransClose.h"
#include "GameInstance/GamePlayerEntity.h"

//#include "PlayerInfoManager.h"
//#include "GameInstance/GamePlayer.h"
#include "Server/BrServer.h"

#include "GameInstance/GameEntityManager.h"

#include "GameSystem/UserFriendSystem.h"
#include "GameSystem/UserGamePlayerInfoSystem.h"


#include "DB/GameConspiracyDB.h"
#include "DB/GameConspiracyQuery.h"
#include "DB/LoginSessionDB.h"
#include "DB/LoginSessionQuery.h"


SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::PlayerTransCloseInstance);


	
namespace SF {
namespace GameServer {


	PlayerTransCloseInstance::PlayerTransCloseInstance()
		: TransactionT( TransactionID() )
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
		Svr::GameClusterServiceEntity *pGameService = nullptr;

		m_WaitingTransactions = 0;

		svrChk( super::StartTransaction() );

		GetMyOwner()->ReleaseConnection();

		if((pOwner->UpdateDBSync(GetTransID())))
			m_WaitingTransactions++;

		if( pOwner->GetMatchingTicket() != 0 )
		{
			Policy::IPolicyPartyMatchingQueue *pPolicy = Svr::GetServerComponent<Svr::ServerEntityManager>()->GetServerPolicy<Policy::IPolicyPartyMatchingQueue>(pOwner->GetMatchingTicket().QueueUID.GetServerID());
			if (pPolicy != nullptr)
			{
				if((pPolicy->UnregisterMatchingCmd(RouteContext(GetOwnerEntityUID(), pOwner->GetMatchingTicket().QueueUID), GetTransID(), 0, pOwner->GetMatchingTicket())))
					m_WaitingTransactions++;
			}
		}

		if (pOwner->GetPartyUID() != 0)
		{
			Policy::IPolicyGameParty *pPolicy = Svr::GetServerComponent<Svr::ServerEntityManager>()->GetServerPolicy<Policy::IPolicyGameParty>(pOwner->GetPartyUID().GetServerID());
			if (pPolicy != nullptr)
			{
				if ((pPolicy->LeavePartyCmd(RouteContext(GetOwnerEntityUID(), pOwner->GetPartyUID()), GetTransID(), pOwner->GetPlayerID())))
					m_WaitingTransactions++;
			}
		}

		if (pOwner->GetGameInsUID() != 0)
		{
			Policy::IPolicyGameInstance *pPolicy = Svr::GetServerComponent<Svr::ServerEntityManager>()->GetServerPolicy<Policy::IPolicyGameInstance>(pOwner->GetGameInsUID().GetServerID());
			if (pPolicy != nullptr)
			{
				if ((pPolicy->LeaveGameCmd(RouteContext(GetOwnerEntityUID(), pOwner->GetGameInsUID()), GetTransID(), pOwner->GetPlayerID())))
					m_WaitingTransactions++;
			}
		}

		if ((Svr::GetServerComponent<DB::LoginSessionDB>()->DeleteLoginSession(GetTransID(), GetMyOwner()->GetPlayerID(), GetMyOwner()->GetAuthTicket())))
			m_WaitingTransactions++;

		svrChkPtr(pGameService = Svr::GetServerComponent<Svr::GameClusterServiceEntity>());
		svrChk(pGameService->DeletePlayer(GetMyOwner()->GetPlayerID(), GetOwnerEntityUID()));


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

		Svr::GetServerComponent<GameEntityManager>()->RemoveEntity(GetMyOwner());

		return ResultCode::SUCCESS;
	}






};// namespace GameServer 
};// namespace SF 

