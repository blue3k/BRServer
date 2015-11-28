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
#include "Protocol/Message/ClusterServerMsgClass.h"
#include "Protocol/Policy/ClusterServerIPolicy.h"
#include "ServerSystem/MessageRoute.h"
#include "ServerSystem/ServiceEntity/RankingServiceEntity.h"
#include "ServerSystem/ServerTransaction.h"


namespace BR {
namespace Svr {

	
	
	class RankingPartyTrans : public TransactionT<RankingServiceEntity, RankingPartyTrans, sizeof(TransactionMessageHandlerType)*7>
	{
	public:
		typedef TransactionT<RankingServiceEntity, RankingPartyTrans, sizeof(TransactionMessageHandlerType) * 7> super;

	private:

		enum {
			MAX_GRAB_TRY = 12,
			MAX_NUM_PLAYER = 20,
		};

		enum class Step {
			Grabbing,
			Dequeuing,
			Canceling,
			Matched,
			CreateGame,
		} m_Step;

		BRCLASS_ATTRIBUTE(UINT,QueryMemberCount);
		// Target matching member count
		BRCLASS_ATTRIBUTE(UINT,TargetMemberCount);
		// Current total member count
		BRCLASS_ATTRIBUTE(UINT,CurrentMemberCount);

		BRCLASS_ATTRIBUTE(INT,CurrentGrabbing);
		BRCLASS_ATTRIBUTE(UINT,GrabTryCount);

		BRCLASS_ATTRIBUTE(INT,ProcessingIndex);

		struct RankingPlayerInfo {
			RankingPlayerInformation	Players[MAX_NUM_PLAYER];

			RankingPlayerInfo()
			{
			}
		};
		StaticArray<RankingPlayerInfo, MAX_NUM_PLAYER> m_ReservedMember;

	public:
		RankingPartyTrans(UINT startMemberCount, UINT targetMemberCount);
		virtual ~RankingPartyTrans() {}

		// Start Transaction
		virtual HRESULT StartTransaction();

	};





} // namespace GameServer 
} // namespace BR 

