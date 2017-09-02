////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2015 The Braves
// 
// Author : KyungKun Ko
//
// Description : Entity class implementation
//	
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "String/StrUtil.h"
#include "Util/TimeUtil.h"
#include "ServerLog/SvrLog.h"
#include "Thread/Thread.h"
#include "SvrConst.h"
#include "ServerSystem/SimpleEntity.h"
#include "ServerSystem/MessageRoute.h"
#include "ServerSystem/Transaction.h"
//#include "ServerSystem/PlugIn.h"
#include "ServerSystem/SvrTrace.h"
#include "Net/Message.h"



namespace SF {
namespace Svr {



	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Simple Entity class
	//		: Entity with low transaction load
	//

	SimpleEntity::SimpleEntity( uint uiTransQueueSize, uint TransResQueueSize )
		: Entity( uiTransQueueSize, TransResQueueSize )
		, m_pCurTran(nullptr)
	{
	}

	SimpleEntity::~SimpleEntity()
	{
		m_pCurTran = SharedPointerT<Transaction>();
	}


	// clear transaction
	Result SimpleEntity::ClearEntity()
	{
		ReleaseTransaction((Transaction*)m_pCurTran);
		m_pCurTran = SharedPointerT<Transaction>();

		return Entity::ClearEntity();
	}

	Result SimpleEntity::FindActiveTransaction(const TransactionID& transID, Transaction* &pTransaction)
	{
		if (m_pCurTran != nullptr && m_pCurTran->GetTransID() == transID)
		{
			pTransaction = (Transaction*)m_pCurTran;
			return ResultCode::SUCCESS;
		}

		return ResultCode::FAIL;
	}


	// Update entity process
	//  - Make new transaction from connection queue
	//  - status update for game
	//  - Process transaction
	Result SimpleEntity::TickUpdate(TimerAction *pAction)
	{
		Result hr = ResultCode::SUCCESS;
		CounterType loopCount;
		Transaction* pCurTrans = nullptr;

		if (GetEntityState() == EntityState::FREE)
			return hr;

		if( GetEntityState() == EntityState::CLOSING )
		{
			svrChk( TerminateEntity() );
			return hr;
		}

		// Tick current transaction
		pCurTrans = (Transaction*)m_pCurTran;
		if (pCurTrans != nullptr)
		{
			svrChk(ProcessTransaction(pCurTrans));
		}

		// If no active transaction, pop one and try it
		loopCount = GetTransactionQueue().GetEnqueCount();
		for (decltype(loopCount) iTrans = 0; pCurTrans == nullptr && iTrans < loopCount; iTrans++)
		{
			if (!(GetTransactionQueue().Dequeue(pCurTrans)))
				break;

			m_pCurTran = SharedPointerT<Transaction>(pCurTrans);
			//svrTrace(Svr::TRC_TRANSACTION, "Trans NewActive TID:{0}, ParentTID:{1} {2}, Entity:{3}:%4%", pCurTrans->GetTransID(), pCurTrans->GetParentTransID(), typeid(*pCurTrans).name(), GetEntityUID(), typeid(*this).name());

			svrChk(ProcessTransaction(pCurTrans));
		}

		m_pCurTran = SharedPointerT<Transaction>(pCurTrans);

		if( GetEntityState() == EntityState::CLOSING )
		{
			svrChk( TerminateEntity() );
			return hr;
		}

		// We need to reschedule us
		if (pAction != nullptr)
		{
			SetBestScehdulingTime((Transaction*)m_pCurTran);
		}


	Proc_End:

		return hr;
	}


	Result SimpleEntity::ProcessTransactionResult(Transaction *pCurTran, TransactionResult *pTranRes)
	{
		Result hr = ResultCode::SUCCESS;
		Result hrTem = ResultCode::SUCCESS;

		if (pCurTran->GetState() != Transaction::STATE_STARTED)
			goto Proc_End;

		if (pTranRes->GetTransID() == pCurTran->GetTransID())
		{
			if (pCurTran->IsPrintTrace())
			{
				svrTrace(Svr::TRC_DBGTRANS, "Trans result, TID:{0}, Entity:{1}, Msg:{2}",
					pCurTran->GetTransID(), GetEntityUID(),
					pTranRes->GetMsgID());
			}
			pCurTran->UpdateHeartBitTime();
			if (pCurTran->IsTimerWorking()) pCurTran->ClearTimer();
			pCurTran->RecordTransactionHistory(pTranRes);
			hrTem = pCurTran->ProcessTransaction(pTranRes);
		}
		else
		{
			if (pCurTran->IsPrintTrace())
			{
				svrTrace(Svr::TRC_DBGTRANS, "Trans Missing result drop, TID:{0}",
					pCurTran->GetTransID());
			}
		}

		if (!(hrTem))// Transaction failed
		{
			if (pCurTran->IsPrintTrace())
			{
				svrTrace(Svr::TRC_TRANSACTION, "Trans Proc failed hr={0:X8}, TID:{1}:{2}, Entity:{3}",
					hrTem,
					pCurTran->GetTransID(), typeid(*pCurTran).name(),
					GetEntityUID());
			}
			if (!pCurTran->IsClosed())
				pCurTran->CloseTransaction(ResultCode::FAIL);
		}

		if (pCurTran->IsClosed())
		{
			TickUpdate();
		}

	Proc_End:

		Util::SafeRelease(pTranRes);

		return hr;
	}

	uint SimpleEntity::GetActiveTransactionCount()
	{
		return m_pCurTran != nullptr ? 1 : 0;
	}

}; // namespace Svr
}; // namespace SF



