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
#include "Common/Thread.h"
#include "Common/StrUtil.h"
#include "ServerSystem/EntityTimerActions.h"
#include "ServerSystem/Entity.h"
#include "ServerSystem/Transaction.h"


BR_MEMORYPOOL_IMPLEMENT(BR::Svr::TimerActionTransaction);


namespace BR {
namespace Svr {
	
	////////////////////////////////////////////////////////////////////////////////////////
	//
	//	
	//

	TimerActionTransaction::TimerActionTransaction(SharedPointerT<Transaction>& pTrans)
		: m_pTrans(pTrans)
	{
		TimeData.ObjectID = pTrans->GetTransID().TransID;
	}

	TimerActionTransaction::~TimerActionTransaction()
	{
	}

	void TimerActionTransaction::SetNextTickTime(SharedPointerT<Transaction>& pTrans)
	{
		auto nextTick = std::min(pTrans->GetTimerExpireTime(), pTrans->GetHeartBitTimeout());
		Assert(nextTick != -1);
		__super::SetNextTickTime(nextTick);
	}

	bool TimerActionTransaction::UpdateTick()
	{
		SharedPointerT<Transaction> pTransPtr;
		Transaction* pTrans;
		m_pTrans.GetSharedPointer(pTransPtr);

		ClearTime();

		Assert(GetInQueueKey().NextTickTime == -1); // Tick updater should updated this

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
}; // namespace BR

