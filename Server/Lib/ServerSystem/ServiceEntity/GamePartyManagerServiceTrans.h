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
#include "Memory/MemoryPool.h"
#include "Container/SFArray.h"
#include "Types/SFEngineTypedefs.h"
#include "GameConst.h"
#include "Net/Message.h"
#include "Protocol/Message/GamePartyManagerMsgClass.h"
#include "Protocol/Policy/GamePartyManagerNetPolicy.h"
#include "Transaction/MessageRoute.h"
#include "ServerSystem/ServiceEntity/GamePartyManagerServiceEntity.h"
#include "ServerSystem/ServerTransaction.h"


namespace SF {
namespace Svr {


	class PartyManagerTransCreateParty : public ClusterEntityMessageTransaction< GamePartyManagerServiceEntity, Message::GamePartyManager::CreatePartyCmd, PartyManagerTransCreateParty, 1>
	{
	public:
		typedef ClusterEntityMessageTransaction< GamePartyManagerServiceEntity, Message::GamePartyManager::CreatePartyCmd, PartyManagerTransCreateParty, 1> super;

	private:
		PartyUID	m_PartyUID;

	public:
		PartyManagerTransCreateParty(Message::MessageData* &pIMsg) : ClusterEntityMessageTransaction(pIMsg) {}
		virtual ~PartyManagerTransCreateParty() {}

		// Start Transaction
		virtual Result StartTransaction();

		Policy::NetSvrPolicyGamePartyManager* GetInterface()	{ return ServerEntityMessageTransaction::GetInterface<Policy::NetSvrPolicyGamePartyManager>(); }

		BR_SVR_MSGTRANS_CLOSE(CreatePartyRes, RouteContext(m_PartyUID,GetRouteContext().GetFrom()));
	};




	class PartyManagerTransPartyDeleted : public ClusterEntityMessageTransaction< GamePartyManagerServiceEntity, Message::GamePartyManager::PartyDeletedC2SEvt, PartyManagerTransPartyDeleted, 1>
	{
	public:
		typedef ClusterEntityMessageTransaction< GamePartyManagerServiceEntity, Message::GamePartyManager::PartyDeletedC2SEvt, PartyManagerTransPartyDeleted, 1> super;

	private:

	public:
		PartyManagerTransPartyDeleted(Message::MessageData* &pIMsg) : ClusterEntityMessageTransaction(pIMsg) {}
		virtual ~PartyManagerTransPartyDeleted() {}

		// Start Transaction
		virtual Result StartTransaction();
	};

	
} // namespace GameServer 
} // namespace SF 

