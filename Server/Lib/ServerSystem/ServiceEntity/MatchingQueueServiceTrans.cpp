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
#include "Common/ResultCode/BRResultCodeCommon.h"
#include "Common/ResultCode/BRResultCodeGame.h"
#include "Common/MemoryPool.h"
#include "Common/BrSvrTypes.h"

#include "Protocol/Message/PartyMatchingQueueMsgClass.h"
#include "Protocol/Policy/PartyMatchingQueueIPolicy.h"

#include "ServerSystem/BrServerUtil.h"
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
	Result RegisterPartyMatchingTrans::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		m_MatchingTicket = 0;

		svrChk( super::StartTransaction() );

		svrChk(GetMyOwner()->Enqueue(GetRouteContext().GetFrom(), 0, (UINT)GetPlayers().GetSize(), GetPlayers().data(), m_MatchingTicket));

	Proc_End:

		CloseTransaction(hr);

		return hr;
	}


	// Start Transaction
	Result RegisterPlayerMatchingTrans::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		MatchingPlayerInformation playerInfo(GetRouteContext().GetFrom(), GetPlayerID(), PlayerRole::None);
		m_MatchingTicket = 0;

		svrChk( super::StartTransaction() );

		svrChk( GetMyOwner()->Enqueue( GetRouteContext().GetFrom(), GetPlayerID(), 1, &playerInfo, m_MatchingTicket ) );

	Proc_End:

		CloseTransaction(hr);

		return hr;
	}
	

	// Start Transaction
	Result UpdateMatchingEntityUIDTrans::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk( super::StartTransaction() );

		svrChk( GetMyOwner()->UpdateTicket( GetMatchingTicket(), GetPreviousUID(), GetRouteContext().GetFrom()) );

	Proc_End:

		CloseTransaction(hr);

		return hr;
	}
	

	// Start Transaction
	Result UnregisterMatchingTrans::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk( super::StartTransaction() );

		svrChk( GetMyOwner()->CancelTicket( GetMatchingTicket() ) );

	Proc_End:

		CloseTransaction(hr);

		return hr;
	}


	// Start Transaction
	Result ReserveItemTrans::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk( super::StartTransaction() );

		hr = GetMyOwner()->ReserveItem( GetRouteContext().GetFrom(), m_NumPlayersInTheTicket, m_MatchingTicket );

	Proc_End:

		CloseTransaction(hr);

		return hr;
	}


	// Start Transaction
	Result MatchingQueueReserveItemsTrans::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(super::StartTransaction());

		for (UINT iItem = 0; iItem < GetNumberOfItemsToReserve(); iItem++)
		{
			UINT numPlayersInTheTicket;
			MatchingQueueTicket matchingTicket;

			hr = GetMyOwner()->ReserveItem(GetRouteContext().GetFrom(), numPlayersInTheTicket, matchingTicket);
			if (!(hr))
				break;

			m_NumPlayersInTheTicket.push_back(numPlayersInTheTicket);
			m_MatchingTicket.push_back(matchingTicket);
		}

		hr = m_NumPlayersInTheTicket.GetSize() == 0 ? Result(ResultCode::E_SVR_NOITEM_INQUEUE) : Result(ResultCode::SUCCESS);

	Proc_End:

		CloseTransaction(hr);

		return hr;
	}



	// Start Transaction
	Result CancelReservationTrans::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk( super::StartTransaction() );

		svrChk( GetMyOwner()->CancelReservation( GetTicketToCancel() ) );

	Proc_End:

		CloseTransaction(hr);

		return hr;
	}



	// Start Transaction
	Result DequeueItemTrans::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		ServerEntity *pServerEntity = nullptr;

		svrChk( super::StartTransaction() );

		svrChk(GetMyOwner()->DequeueItem(GetMatchingTicket(), m_matchingQueueItem));


		svrChk(GetServerComponent<ServerEntityManager>()->GetServerEntity( m_matchingQueueItem.RegisterUID.GetServerID(), pServerEntity ));

		if( m_matchingQueueItem.NumPlayers == 0 )
			svrErrClose(ResultCode::E_SVR_INVALID_QUEUEITEM);

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
	Result MatchingQueueTransMatchingItemError::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		ServerEntity *pServerEntity = nullptr;
		MatchingQueueItem matchingQueueItem;

		svrChk(super::StartTransaction());

		svrChk(GetMyOwner()->DeleteItem(GetMatchingTicket(), matchingQueueItem));


		svrChk(GetServerComponent<ServerEntityManager>()->GetServerEntity(matchingQueueItem.RegisterUID.GetServerID(), pServerEntity));

		if (matchingQueueItem.NumPlayers == 0)
			svrErrClose(ResultCode::E_SVR_INVALID_QUEUEITEM);

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

