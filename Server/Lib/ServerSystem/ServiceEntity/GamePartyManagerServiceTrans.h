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

#include "Common/Typedefs.h"
#include "ServerSystem/Transaction.h"
#include "Common/MemoryPool.h"
#include "Common/ArrayUtil.h"
#include "Common/BrBaseTypes.h"
#include "Common/GameConst.h"
#include "Common/Message.h"
#include "Protocol/Message/GamePartyManagerMsgClass.h"
#include "Protocol/Policy/GamePartyManagerIPolicy.h"
#include "ServerSystem/MessageRoute.h"
#include "ServerSystem/ServiceEntity/GamePartyManagerServiceEntity.h"
#include "ServerSystem/ServerTransaction.h"


namespace BR {
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
		virtual HRESULT StartTransaction();

		Policy::ISvrPolicyGamePartyManager* GetPolicy()	{ return ServerEntityMessageTransaction::GetPolicy<Policy::ISvrPolicyGamePartyManager>(); }

		BR_IMPLEMENT_MSGTRANS_CLOSE_ARGS(CreatePartyRes, RouteContext(m_PartyUID,GetRouteContext().GetFrom()));
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
		virtual HRESULT StartTransaction();
	};

	
} // namespace GameServer 
} // namespace BR 

