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
#include "Protocol/ClusterServerMsgClass.h"
#include "Protocol/ClusterServerNetPolicy.h"
#include "Protocol/RankingServerMsgClass.h"
#include "Protocol/RankingServerNetPolicy.h"
#include "Transaction/MessageRoute.h"
#include "ServiceEntity/RankingServiceEntity.h"
#include "Transaction/ServerTransaction.h"


namespace SF {
	namespace Svr {



		class RankingServerAddPlayerTrans : public ServerEntityMessageTransaction<RankingServiceEntity, Message::RankingServer::AddPlayerCmd>
		{
		public:
			typedef ServerEntityMessageTransaction<RankingServiceEntity, Message::RankingServer::AddPlayerCmd> super;

		private:



		public:
			RankingServerAddPlayerTrans(IHeap& memMgr, const MessageDataPtr& pIMsg);
			virtual ~RankingServerAddPlayerTrans() {}

			// Start Transaction
			virtual Result StartTransaction() override;

		};


		class RankingServerUpdatePlayerScoreTrans : public ServerEntityMessageTransaction<RankingServiceEntity, Message::RankingServer::UpdatePlayerScoreCmd>
		{
		public:
			typedef ServerEntityMessageTransaction<RankingServiceEntity, Message::RankingServer::UpdatePlayerScoreCmd> super;

		private:

			int64_t m_PlayerRanking;
			StaticArray<TotalRankingPlayerInformation, 32> m_RankingList;

		public:
			RankingServerUpdatePlayerScoreTrans(IHeap& memMgr, const MessageDataPtr& pIMsg);
			virtual ~RankingServerUpdatePlayerScoreTrans() {}

			// Start Transaction
			virtual Result StartTransaction() override;


			BR_SVR_MSGTRANS_CLOSE_ARGS(NetSvrPolicyRankingServer, UpdatePlayerScoreRes, GetRouteContext().GetSwaped(), m_RankingList);
		};


		class RankingServerDebugPrintALLRankingTrans : public ServerEntityMessageTransaction<RankingServiceEntity, Message::RankingServer::DebugPrintALLRankingCmd>
		{
		public:
			typedef ServerEntityMessageTransaction<RankingServiceEntity, Message::RankingServer::DebugPrintALLRankingCmd> super;

		private:

			StaticArray<TotalRankingPlayerInformation, 10000> m_RankingList;
		public:
			RankingServerDebugPrintALLRankingTrans(IHeap& memMgr, const MessageDataPtr& pIMsg);
			virtual ~RankingServerDebugPrintALLRankingTrans() {}

			// Start Transaction
			virtual Result StartTransaction() override;

			BR_SVR_MSGTRANS_CLOSE(NetSvrPolicyRankingServer, DebugPrintALLRankingRes, GetRouteContext().GetSwaped());
		};


	} // namespace Svr 
} // namespace SF 

