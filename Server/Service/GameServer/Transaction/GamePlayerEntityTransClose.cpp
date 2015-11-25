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

#include "Common/HRESCommon.h"
#include "Common/HRESGame.h"
#include "Common/HRESLogin.h"
#include "Common/MemoryPool.h"
#include "Common/BRBaseTypes.h"

#include "GameServerClass.h"
#include "ServerSystem/BRServerUtil.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/ServerEntityManager.h"

#include "ServerSystem/ServerService/PartyMatchingQueueService.h"
#include "ServerSystem/ServiceEntity/MatchingQueueServiceEntity.h"
#include "ServerSystem/ServiceEntity/GameClusterServiceEntity.h"

#include "Protocol/Message/PartyMatchingQueueMsgClass.h"

#include "Protocol/Message/GamePartyMsgClass.h"
#include "Protocol/Policy/GamePartyIPolicy.h"

#include "Protocol/Message/GameServerMsgClass.h"
#include "Protocol/Policy/GameServerIPolicy.h"
#include "Protocol/Message/GameInstanceMsgClass.h"
#include "Protocol/Policy/GameInstanceIPolicy.h"

#include "GamePlayerEntityTransClose.h"
#include "GameInstance/GamePlayerEntity.h"

//#include "PlayerInfoManager.h"
//#include "GameInstance/GamePlayer.h"
#include "ServerSystem/BrServer.h"

#include "GameInstance/GameEntityManager.h"

#include "GameSystem/UserFriendSystem.h"
#include "GameSystem/UserGamePlayerInfoSystem.h"


#include "DB/GameConspiracyDB.h"
#include "DB/GameConspiracyQuery.h"
#include "DB/LoginSessionDB.h"
#include "DB/LoginSessionQuery.h"


BR_MEMORYPOOL_IMPLEMENT(GameServer::PlayerTransCloseInstance);


	
namespace BR {
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

	HRESULT PlayerTransCloseInstance::UpdateDBRes(Svr::TransactionResult* &pRes)
	{
		HRESULT hr = S_OK;

		m_WaitingTransactions--;

		svrChk(pRes->GetHRESULT());

	Proc_End:

		if (m_WaitingTransactions <= 0)
			CloseTransaction(hr);

		return hr;
	}

	HRESULT PlayerTransCloseInstance::DeleteLoginSessionRes(Svr::TransactionResult* &pRes)
	{
		HRESULT hr = S_OK;

		m_WaitingTransactions--;

		svrChk(pRes->GetHRESULT());

	Proc_End:

		if (m_WaitingTransactions <= 0)
			CloseTransaction(hr);

		return hr;
	}

	HRESULT PlayerTransCloseInstance::UnregisterMatchingRes( Svr::TransactionResult* & pRes )
	{
		HRESULT hr = S_OK;

		Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;
		Message::PartyMatchingQueue::UnregisterMatchingRes res;

		m_WaitingTransactions--;

		GetMyOwner()->SetMatchingTicket(0);

		svrChk(pRes->GetHRESULT());

	Proc_End:

		if (m_WaitingTransactions <= 0)
			CloseTransaction(hr);

		return hr; 
	}

	HRESULT PlayerTransCloseInstance::LeaveGameRes(Svr::TransactionResult* & pRes)
	{
		HRESULT hr = S_OK;

		Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;
		Message::GameInstance::LeaveGameRes res;

		m_WaitingTransactions--;

		GetMyOwner()->SetGameInsUID(0);

		svrChk(pRes->GetHRESULT());

	Proc_End:

		if (m_WaitingTransactions <= 0)
			CloseTransaction(hr);

		return hr;
	}

	HRESULT PlayerTransCloseInstance::LeavePartyRes(Svr::TransactionResult* & pRes)
	{
		HRESULT hr = S_OK;

		Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;
		Message::GameParty::LeavePartyRes res;

		m_WaitingTransactions--;

		GetMyOwner()->SetPartyUID(0);

		svrChk(pRes->GetHRESULT());

	Proc_End:

		if (m_WaitingTransactions <= 0)
			CloseTransaction(hr);

		return hr;
	}

	HRESULT PlayerTransCloseInstance::StartTransaction()
	{
		HRESULT hr = S_OK;
		GamePlayerEntity* pOwner = (GamePlayerEntity*)GetOwnerEntity();
		Svr::GameClusterServiceEntity *pGameService = nullptr;

		m_WaitingTransactions = 0;

		svrChk( __super::StartTransaction() );

		GetMyOwner()->ReleaseConnection();

		if(SUCCEEDED(pOwner->UpdateDBSync(GetTransID())))
			m_WaitingTransactions++;

		if( pOwner->GetMatchingTicket() != 0 )
		{
			Policy::IPolicyPartyMatchingQueue *pPolicy = Svr::GetServerComponent<Svr::ServerEntityManager>()->GetServerPolicy<Policy::IPolicyPartyMatchingQueue>(pOwner->GetMatchingTicket().QueueUID.GetServerID());
			if (pPolicy != nullptr)
			{
				if(SUCCEEDED(pPolicy->UnregisterMatchingCmd(GetTransID(), RouteContext(GetOwnerEntityUID(), pOwner->GetMatchingTicket().QueueUID), 0, pOwner->GetMatchingTicket())))
					m_WaitingTransactions++;
			}
		}

		if (pOwner->GetPartyUID() != 0)
		{
			Policy::IPolicyGameParty *pPolicy = Svr::GetServerComponent<Svr::ServerEntityManager>()->GetServerPolicy<Policy::IPolicyGameParty>(pOwner->GetPartyUID().GetServerID());
			if (pPolicy != nullptr)
			{
				if (SUCCEEDED(pPolicy->LeavePartyCmd(GetTransID(), RouteContext(GetOwnerEntityUID(), pOwner->GetPartyUID()), pOwner->GetPlayerID())))
					m_WaitingTransactions++;
			}
		}

		if (pOwner->GetGameInsUID() != 0)
		{
			Policy::IPolicyGameInstance *pPolicy = Svr::GetServerComponent<Svr::ServerEntityManager>()->GetServerPolicy<Policy::IPolicyGameInstance>(pOwner->GetGameInsUID().GetServerID());
			if (pPolicy != nullptr)
			{
				if (SUCCEEDED(pPolicy->LeaveGameCmd(GetTransID(), RouteContext(GetOwnerEntityUID(), pOwner->GetGameInsUID()), pOwner->GetPlayerID())))
					m_WaitingTransactions++;
			}
		}

		if (SUCCEEDED(Svr::GetServerComponent<DB::LoginSessionDB>()->DeleteLoginSession(GetTransID(), GetMyOwner()->GetPlayerID(), GetMyOwner()->GetAuthTicket())))
			m_WaitingTransactions++;

		svrChkPtr(pGameService = Svr::GetServerComponent<Svr::GameClusterServiceEntity>());
		svrChk(pGameService->DeletePlayer(GetMyOwner()->GetPlayerID(), GetOwnerEntityUID()));


	Proc_End:

		if (FAILED(hr))
		{
			svrTrace(Trace::TRC_ERROR, "Failed to close entity:%0%, hr:{1:X8}", GetOwnerEntityUID(), hr);
		}

		if (m_WaitingTransactions <= 0)
			CloseTransaction(hr);

		return hr;
	}

	HRESULT PlayerTransCloseInstance::OnCloseTransaction( HRESULT hrRes )
	{
		GetMyOwner()->AddGameTransactionLog(TransLogCategory::Account, -2, 0, 0, "EntityClose");

		Svr::GetServerComponent<GameEntityManager>()->RemoveEntity(GetMyOwner());

		return S_OK;
	}






};// namespace GameServer 
};// namespace BR 

