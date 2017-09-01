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
#include "ServerSystem/Transaction.h"
#include "Memory/MemoryPool.h"
#include "Common/ArrayUtil.h"
#include "Types/BrBaseTypes.h"
#include "Common/GameConst.h"
#include "Net/Message.h"
#include "Protocol/Message/ClusterServerMsgClass.h"
#include "Protocol/Policy/ClusterServerIPolicy.h"
#include "Protocol/Message/RankingServerMsgClass.h"
#include "Protocol/Policy/RankingServerIPolicy.h"
#include "ServerSystem/MessageRoute.h"
#include "ServerSystem/ServiceEntity/RankingServiceEntity.h"
#include "ServerSystem/ServerTransaction.h"


namespace BR {
namespace Svr {

	
	
	class RankingServerAddPlayerTrans : public ServerEntityMessageTransaction<RankingServiceEntity, Message::RankingServer::AddPlayerCmd, RankingServerAddPlayerTrans, 1>
	{
	public:
		typedef ServerEntityMessageTransaction<RankingServiceEntity, Message::RankingServer::AddPlayerCmd, RankingServerAddPlayerTrans, 1> super;

	private:



	public:
		RankingServerAddPlayerTrans(Message::MessageData* &pIMsg);
		virtual ~RankingServerAddPlayerTrans() {}

		// Start Transaction
		virtual Result StartTransaction();

	};


	class RankingServerUpdatePlayerScoreTrans : public ServerEntityMessageTransaction<RankingServiceEntity, Message::RankingServer::UpdatePlayerScoreCmd, RankingServerUpdatePlayerScoreTrans, 1>
	{
	public:
		typedef ServerEntityMessageTransaction<RankingServiceEntity, Message::RankingServer::UpdatePlayerScoreCmd, RankingServerUpdatePlayerScoreTrans, 1> super;

	private:

		int64_t m_PlayerRanking;
		StaticArray<TotalRankingPlayerInformation, 32> m_RankingList;

	public:
		RankingServerUpdatePlayerScoreTrans(Message::MessageData* &pIMsg);
		virtual ~RankingServerUpdatePlayerScoreTrans() {}

		// Start Transaction
		virtual Result StartTransaction();

		Policy::ISvrPolicyRankingServer* GetPolicy() { return ServerEntityMessageTransaction::GetPolicy<Policy::ISvrPolicyRankingServer>(); }
		BR_SVR_MSGTRANS_CLOSE_ARGS(UpdatePlayerScoreRes, GetRouteContext().GetSwaped(), m_RankingList);
	};


	class RankingServerDebugPrintALLRankingTrans : public ServerEntityMessageTransaction<RankingServiceEntity, Message::RankingServer::DebugPrintALLRankingCmd, RankingServerDebugPrintALLRankingTrans, 1>
	{
	public:
		typedef ServerEntityMessageTransaction<RankingServiceEntity, Message::RankingServer::DebugPrintALLRankingCmd, RankingServerDebugPrintALLRankingTrans, 1> super;

	private:

		StaticArray<TotalRankingPlayerInformation, 10000> m_RankingList;
	public:
		RankingServerDebugPrintALLRankingTrans(Message::MessageData* &pIMsg);
		virtual ~RankingServerDebugPrintALLRankingTrans() {}

		// Start Transaction
		virtual Result StartTransaction();

		Policy::ISvrPolicyRankingServer* GetPolicy() { return ServerEntityMessageTransaction::GetPolicy<Policy::ISvrPolicyRankingServer>(); }
		BR_SVR_MSGTRANS_CLOSE(DebugPrintALLRankingRes, GetRouteContext().GetSwaped());
	};





} // namespace Svr 
} // namespace BR 

