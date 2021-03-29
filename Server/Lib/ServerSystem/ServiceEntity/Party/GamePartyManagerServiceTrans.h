////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : game player entity implementation
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "SFTypedefs.h"
#include "Transaction/Transaction.h"
#include "MemoryManager/SFMemoryPool.h"
#include "Container/SFArray.h"
#include "Types/SFEngineTypedefs.h"
#include "GameConst.h"
#include "Net/SFMessage.h"
#include "Protocol/Message/GamePartyManagerMsgClass.h"
#include "Protocol/Policy/GamePartyManagerNetPolicy.h"
#include "Transaction/MessageRoute.h"
#include "ServiceEntity/Party/GamePartyManagerServiceEntity.h"
#include "Transaction/ServerTransaction.h"


namespace SF {
namespace Svr {


	class PartyManagerTransCreateParty : public ClusterEntityMessageTransaction< GamePartyManagerServiceEntity, Message::GamePartyManager::CreatePartyCmd>
	{
	public:
		typedef ClusterEntityMessageTransaction< GamePartyManagerServiceEntity, Message::GamePartyManager::CreatePartyCmd> super;

	private:
		PartyUID	m_PartyUID;

	public:
		PartyManagerTransCreateParty(IHeap& heap, const MessageDataPtr &pIMsg) : ClusterEntityMessageTransaction(heap, pIMsg) {}
		virtual ~PartyManagerTransCreateParty() {}

		// Start Transaction
		virtual Result StartTransaction() override;


		BR_SVR_MSGTRANS_CLOSE(NetSvrPolicyGamePartyManager, CreatePartyRes, RouteContext(m_PartyUID,GetRouteContext().GetFrom()));
	};




	class PartyManagerTransPartyDeleted : public ClusterEntityMessageTransaction< GamePartyManagerServiceEntity, Message::GamePartyManager::PartyDeletedC2SEvt>
	{
	public:
		typedef ClusterEntityMessageTransaction< GamePartyManagerServiceEntity, Message::GamePartyManager::PartyDeletedC2SEvt> super;

	private:

	public:
		PartyManagerTransPartyDeleted(IHeap& heap, const MessageDataPtr &pIMsg) : ClusterEntityMessageTransaction(heap, pIMsg) {}
		virtual ~PartyManagerTransPartyDeleted() {}

		// Start Transaction
		virtual Result StartTransaction();
	};

	
} // namespace GameServer 
} // namespace SF 

