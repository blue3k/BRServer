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
#include "Common/HRESCommon.h"
#include "Common/HRESGame.h"
#include "Common/HRESLogin.h"
#include "Common/MemoryPool.h"
#include "Common/BRSvrTypes.h"

#include "Protocol/Message/PartyMatchingQueueMsgClass.h"
#include "Protocol/Policy/PartyMatchingQueueIPolicy.h"

#include "ServerSystem/BRServerUtil.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/BrServer.h"
#include "ServerSystem/ServerEntityManager.h"
#include "ServerSystem/EntityManager.h"
#include "ServerSystem/ServiceEntity/MatchingQueueServiceTrans.h"
#include "ServerSystem/ServiceEntity/MatchingServiceEntity.h"
#include "ServerSystem/ServiceEntity/MatchingQueueServiceEntity.h"

#include "ServerSystem/ServerService/PartyMatchingQueueService.h"


BR_MEMORYPOOL_IMPLEMENT(Svr::RegisterPartyMatchingTrans);
BR_MEMORYPOOL_IMPLEMENT(Svr::RegisterPlayerMatchingTrans);
BR_MEMORYPOOL_IMPLEMENT(Svr::UpdateMatchingEntityUIDTrans);
BR_MEMORYPOOL_IMPLEMENT(Svr::UnregisterMatchingTrans);

BR_MEMORYPOOL_IMPLEMENT(Svr::ReserveItemTrans);
BR_MEMORYPOOL_IMPLEMENT(Svr::MatchingQueueReserveItemsTrans);
BR_MEMORYPOOL_IMPLEMENT(Svr::CancelReservationTrans);
BR_MEMORYPOOL_IMPLEMENT(Svr::DequeueItemTrans);
BR_MEMORYPOOL_IMPLEMENT(Svr::MatchingQueueTransMatchingItemError);


namespace BR {
namespace Svr {




	// Start Transaction
	HRESULT RegisterPartyMatchingTrans::StartTransaction()
	{
		HRESULT hr = S_OK;

		m_MatchingTicket = 0;

		svrChk( __super::StartTransaction() );

		svrChk(GetMyOwner()->Enqueue(GetRouteContext().From, 0, (UINT)GetPlayers().GetSize(), GetPlayers().data(), m_MatchingTicket));

	Proc_End:

		CloseTransaction(hr);

		return hr;
	}


	// Start Transaction
	HRESULT RegisterPlayerMatchingTrans::StartTransaction()
	{
		HRESULT hr = S_OK;

		MatchingPlayerInformation playerInfo(GetRouteContext().From, GetPlayerID());
		m_MatchingTicket = 0;

		svrChk( __super::StartTransaction() );

		svrChk( GetMyOwner()->Enqueue( GetRouteContext().From, GetPlayerID(), 1, &playerInfo, m_MatchingTicket ) );

	Proc_End:

		CloseTransaction(hr);

		return hr;
	}
	

	// Start Transaction
	HRESULT UpdateMatchingEntityUIDTrans::StartTransaction()
	{
		HRESULT hr = S_OK;

		svrChk( __super::StartTransaction() );

		svrChk( GetMyOwner()->UpdateTicket( GetMatchingTicket(), GetPreviousUID(), GetRouteContext().From ) );

	Proc_End:

		CloseTransaction(hr);

		return hr;
	}
	

	// Start Transaction
	HRESULT UnregisterMatchingTrans::StartTransaction()
	{
		HRESULT hr = S_OK;

		svrChk( __super::StartTransaction() );

		svrChk( GetMyOwner()->CancelTicket( GetMatchingTicket() ) );

	Proc_End:

		CloseTransaction(hr);

		return hr;
	}


	// Start Transaction
	HRESULT ReserveItemTrans::StartTransaction()
	{
		HRESULT hr = S_OK;

		svrChk( __super::StartTransaction() );

		hr = GetMyOwner()->ReserveItem( GetRouteContext().From, m_NumPlayersInTheTicket, m_MatchingTicket );

	Proc_End:

		CloseTransaction(hr);

		return hr;
	}


	// Start Transaction
	HRESULT MatchingQueueReserveItemsTrans::StartTransaction()
	{
		HRESULT hr = S_OK;

		svrChk(__super::StartTransaction());

		for (UINT iItem = 0; iItem < GetNumberOfItemsToReserve(); iItem++)
		{
			UINT numPlayersInTheTicket;
			MatchingQueueTicket matchingTicket;

			hr = GetMyOwner()->ReserveItem(GetRouteContext().From, numPlayersInTheTicket, matchingTicket);
			if (FAILED(hr))
				break;

			m_NumPlayersInTheTicket.push_back(numPlayersInTheTicket);
			m_MatchingTicket.push_back(matchingTicket);
		}

		hr = m_NumPlayersInTheTicket.GetSize() == 0 ? E_SVR_NOITEM_INQUEUE : S_OK;

	Proc_End:

		CloseTransaction(hr);

		return hr;
	}



	// Start Transaction
	HRESULT CancelReservationTrans::StartTransaction()
	{
		HRESULT hr = S_OK;

		svrChk( __super::StartTransaction() );

		svrChk( GetMyOwner()->CancelReservation( GetTicketToCancel() ) );

	Proc_End:

		CloseTransaction(hr);

		return hr;
	}



	// Start Transaction
	HRESULT DequeueItemTrans::StartTransaction()
	{
		HRESULT hr = S_OK;
		ServerEntity *pServerEntity = nullptr;

		svrChk( __super::StartTransaction() );

		svrChk(GetMyOwner()->DequeueItem(GetMatchingTicket(), m_matchingQueueItem));


		svrChk(GetServerComponent<ServerEntityManager>()->GetServerEntity( m_matchingQueueItem.RegisterUID.SvrID, pServerEntity ));

		if( m_matchingQueueItem.NumPlayers == 0 )
			svrErrClose(E_SVR_INVALID_QUEUEITEM);

		if( m_matchingQueueItem.NumPlayers > 1 || m_matchingQueueItem.Players[0].PlayerUID != m_matchingQueueItem.RegisterUID ) // This should be a party
		{
			pServerEntity->GetPolicy<Policy::ISvrPolicyPartyMatchingQueue>()->PartyMatchingItemDequeuedS2CEvt( 
				RouteContext(GetOwnerEntityUID(),m_matchingQueueItem.RegisterUID), 0,
				GetMatchingTicket() );
		}
		else
		{
			// This should be the player case
			Assert( m_matchingQueueItem.Players[0].PlayerUID == m_matchingQueueItem.RegisterUID );

			pServerEntity->GetPolicy<Policy::ISvrPolicyPartyMatchingQueue>()->PlayerMatchingItemDequeuedS2CEvt( 
				RouteContext(GetOwnerEntityUID(),m_matchingQueueItem.RegisterUID), 0,
				GetMatchingTicket() );
		}


	Proc_End:

		CloseTransaction(hr);

		return hr;
	}



	// Start Transaction
	HRESULT MatchingQueueTransMatchingItemError::StartTransaction()
	{
		HRESULT hr = S_OK;
		ServerEntity *pServerEntity = nullptr;
		MatchingQueueItem matchingQueueItem;

		svrChk(__super::StartTransaction());

		svrChk(GetMyOwner()->DeleteItem(GetMatchingTicket(), matchingQueueItem));


		svrChk(GetServerComponent<ServerEntityManager>()->GetServerEntity(matchingQueueItem.RegisterUID.SvrID, pServerEntity));

		if (matchingQueueItem.NumPlayers == 0)
			svrErrClose(E_SVR_INVALID_QUEUEITEM);

		if (matchingQueueItem.NumPlayers > 1 || matchingQueueItem.Players[0].PlayerUID != matchingQueueItem.RegisterUID) // This should be a party
		{
			pServerEntity->GetPolicy<Policy::ISvrPolicyPartyMatchingQueue>()->PartyMatchingItemDequeuedS2CEvt(
				RouteContext(GetOwnerEntityUID(), matchingQueueItem.RegisterUID), 0,
				GetMatchingTicket());
		}
		else
		{
			// This should be the player case
			Assert(matchingQueueItem.Players[0].PlayerUID == matchingQueueItem.RegisterUID);

			pServerEntity->GetPolicy<Policy::ISvrPolicyPartyMatchingQueue>()->PlayerMatchingItemDequeuedS2CEvt(
				RouteContext(GetOwnerEntityUID(), matchingQueueItem.RegisterUID), 0,
				GetMatchingTicket());
		}


	Proc_End:

		CloseTransaction(hr);

		return hr;
	}


	

};// namespace Svr 
};// namespace BR 

