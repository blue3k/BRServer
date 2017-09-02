////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Server TickTask manager
//	
//
////////////////////////////////////////////////////////////////////////////////



#include "stdafx.h"
#include "Thread/Thread.h"
#include "String/StrUtil.h"
#include "Entity/EntityTimerActions.h"
#include "Entity/Entity.h"
#include "Transaction/Transaction.h"


SF_MEMORYPOOL_IMPLEMENT(SF::Svr::TimerActionTransaction);


namespace SF {
namespace Svr {
	
	////////////////////////////////////////////////////////////////////////////////////////
	//
	//	
	//

	TimerActionTransaction::TimerActionTransaction(SharedPointerT<Transaction>& pTrans)
		: m_pTrans(pTrans)
	{
		TimeData.Components.ObjectID = pTrans->GetTransID().GetTransactionIndex();
	}

	TimerActionTransaction::~TimerActionTransaction()
	{
	}

	void TimerActionTransaction::SetNextTickTime(SharedPointerT<Transaction>& pTrans)
	{
		auto nextTick = Util::TimeMinNonZero(pTrans->GetTimerExpireTime(), pTrans->GetHeartBitTimeout());
		Assert(nextTick != TimeStampMS::max() && nextTick != TimeStampMS::min());
		TimerAction::SetNextTickTime(nextTick);
	}

	bool TimerActionTransaction::OnTimerTick()
	{
		SharedPointerT<Transaction> pTransPtr;
		Transaction* pTrans;
		m_pTrans.GetSharedPointer(pTransPtr);

		ClearTime();

		Assert(GetInQueueKey().Components.NextTickTime == TimeStampMS::max()); // Tick updater should updated this

		if (pTransPtr == nullptr)
		{
			Assert(false);
			return false;
		}
		pTrans = (Transaction*)pTransPtr;
		Entity *pOwnerEntity = pTrans->GetOwnerEntity();

		pOwnerEntity->ProcessTransaction(pTrans);
		if (pTrans == nullptr)
		{
			return false;
		}

		SetNextTickTime(pTransPtr);
		return true;
	}

	const char* TimerActionTransaction::GetDebugString()
	{
		SharedPointerT<Transaction> pTransPtr;
		Transaction* pTrans;
		m_pTrans.GetSharedPointer(pTransPtr);
		pTrans = (Transaction*)pTransPtr;
		if (pTrans != nullptr) return typeid(*pTrans).name();

		return "TimerActionTransaction";
	}


}; // namespace Svr
}; // namespace SF

