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
#include "Memory/SFMemoryPool.h"
#include "Types/BrSvrTypes.h"

#include "Protocol/Message/PartyMatchingQueueMsgClass.h"
#include "Protocol/Policy/PartyMatchingQueueNetPolicy.h"

#include "Server/BrServerUtil.h"
#include "SvrTrace.h"
#include "Server/BrServer.h"
#include "ServerEntity/ServerEntityManager.h"

#include "Entity/EntityManager.h"
#include "ServiceEntity/MatchingQueueServiceTrans.h"
#include "ServiceEntity/MatchingServiceEntity.h"
#include "ServiceEntity/MatchingQueueServiceEntity.h"

#include "Protocol/ServerService/PartyMatchingQueueService.h"


SF_MEMORYPOOL_IMPLEMENT(SF::Svr::RegisterPartyMatchingTrans);
SF_MEMORYPOOL_IMPLEMENT(SF::Svr::RegisterPlayerMatchingTrans);
SF_MEMORYPOOL_IMPLEMENT(SF::Svr::UpdateMatchingEntityUIDTrans);
SF_MEMORYPOOL_IMPLEMENT(SF::Svr::UnregisterMatchingTrans);

SF_MEMORYPOOL_IMPLEMENT(SF::Svr::ReserveItemTrans);
SF_MEMORYPOOL_IMPLEMENT(SF::Svr::MatchingQueueReserveItemsTrans);
SF_MEMORYPOOL_IMPLEMENT(SF::Svr::CancelReservationTrans);
SF_MEMORYPOOL_IMPLEMENT(SF::Svr::DequeueItemTrans);
SF_MEMORYPOOL_IMPLEMENT(SF::Svr::MatchingQueueTransMatchingItemError);


namespace SF {
namespace Svr {




	// Start Transaction
	Result RegisterPartyMatchingTrans::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		m_MatchingTicket = 0;

		svrChk( super::StartTransaction() );

		svrChk(GetMyOwner()->Enqueue(GetRouteContext().GetFrom(), 0, (uint)GetPlayers().size(), GetPlayers().data(), m_MatchingTicket));

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

		for (uint iItem = 0; iItem < GetNumberOfItemsToReserve(); iItem++)
		{
			uint numPlayersInTheTicket;
			MatchingQueueTicket matchingTicket;

			hr = GetMyOwner()->ReserveItem(GetRouteContext().GetFrom(), numPlayersInTheTicket, matchingTicket);
			if (!(hr))
				break;

			m_NumPlayersInTheTicket.push_back(numPlayersInTheTicket);
			m_MatchingTicket.push_back(matchingTicket);
		}

		hr = m_NumPlayersInTheTicket.size() == 0 ? Result(ResultCode::SVR_NOITEM_INQUEUE) : Result(ResultCode::SUCCESS);

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


		svrChk(Service::ServerEntityManager->GetServerEntity( m_matchingQueueItem.RegisterUID.GetServerID(), pServerEntity ));

		if( m_matchingQueueItem.NumPlayers == 0 )
			svrErrClose(ResultCode::SVR_INVALID_QUEUEITEM);

		if( m_matchingQueueItem.NumPlayers > 1 || m_matchingQueueItem.Players[0].PlayerUID != m_matchingQueueItem.RegisterUID ) // This should be a party
		{
			
			Policy::NetSvrPolicyPartyMatchingQueue(pServerEntity->GetConnection()).PartyMatchingItemDequeuedS2CEvt(
				RouteContext(GetOwnerEntityUID(),m_matchingQueueItem.RegisterUID), 0,
				GetMatchingTicket() );
		}
		else
		{
			// This should be the player case
			Assert( m_matchingQueueItem.Players[0].PlayerUID == m_matchingQueueItem.RegisterUID );

			Policy::NetSvrPolicyPartyMatchingQueue(pServerEntity->GetConnection()).PlayerMatchingItemDequeuedS2CEvt(
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


		svrChk(Service::ServerEntityManager->GetServerEntity(matchingQueueItem.RegisterUID.GetServerID(), pServerEntity));

		if (matchingQueueItem.NumPlayers == 0)
			svrErrClose(ResultCode::SVR_INVALID_QUEUEITEM);

		if (matchingQueueItem.NumPlayers > 1 || matchingQueueItem.Players[0].PlayerUID != matchingQueueItem.RegisterUID) // This should be a party
		{
			Policy::NetSvrPolicyPartyMatchingQueue(pServerEntity->GetConnection()).PartyMatchingItemDequeuedS2CEvt(
				RouteContext(GetOwnerEntityUID(), matchingQueueItem.RegisterUID), 0,
				GetMatchingTicket());
		}
		else
		{
			// This should be the player case
			Assert(matchingQueueItem.Players[0].PlayerUID == matchingQueueItem.RegisterUID);

			Policy::NetSvrPolicyPartyMatchingQueue(pServerEntity->GetConnection()).PlayerMatchingItemDequeuedS2CEvt(
				RouteContext(GetOwnerEntityUID(), matchingQueueItem.RegisterUID), 0,
				GetMatchingTicket());
		}


	Proc_End:

		CloseTransaction(hr);

		return hr;
	}


	

};// namespace Svr 
};// namespace SF 

