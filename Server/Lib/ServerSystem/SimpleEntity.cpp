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
#include "Common/StrUtil.h"
#include "Common/TimeUtil.h"
#include "Common/Trace.h"
#include "Common/Thread.h"
#include "ServerSystem/SvrConstDefault.h"
#include "ServerSystem/SimpleEntity.h"
#include "ServerSystem/MessageRoute.h"
#include "ServerSystem/Transaction.h"
//#include "ServerSystem/PlugIn.h"
#include "ServerSystem/SvrTrace.h"
#include "Common/Message.h"



namespace BR {
namespace Svr {



	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Simple Entity class
	//		: Entity with low transaction load
	//

	SimpleEntity::SimpleEntity( UINT uiTransQueueSize, UINT TransResQueueSize )
		: Entity( uiTransQueueSize, TransResQueueSize )
		, m_pCurTran(nullptr)
	{
	}

	SimpleEntity::~SimpleEntity()
	{
		m_pCurTran = SharedPointerT<Transaction>();
	}


	// clear transaction
	HRESULT SimpleEntity::ClearEntity()
	{
		ReleaseTransaction((Transaction*)m_pCurTran);
		m_pCurTran = SharedPointerT<Transaction>();

		return Entity::ClearEntity();
	}

	HRESULT SimpleEntity::FindActiveTransaction(const TransactionID& transID, Transaction* &pTransaction)
	{
		if (m_pCurTran != nullptr && m_pCurTran->GetTransID() == transID)
		{
			pTransaction = (Transaction*)m_pCurTran;
			return S_OK;
		}

		return E_SYSTEM_FAIL;
	}


	// Update entity process
	//  - Make new transaction from connection queue
	//  - status update for game
	//  - Process transaction
	HRESULT SimpleEntity::TickUpdate(Svr::TimerAction *pAction)
	{
		HRESULT hr = S_OK;
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
			if (FAILED(GetTransactionQueue().Dequeue(pCurTrans)))
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


	HRESULT SimpleEntity::ProcessTransactionResult(Transaction *pCurTran, TransactionResult *pTranRes)
	{
		HRESULT hr = S_OK;
		HRESULT hrTem = S_OK;

		if (pCurTran->GetState() != Transaction::STATE_STARTED)
			goto Proc_End;

		if (pTranRes->GetTransID() == pCurTran->GetTransID())
		{
			if (pCurTran->IsPrintTrace())
			{
				svrTrace(Svr::TRC_TRANSACTION, "Trans result, TID:{0}, Entity:{1}, Msg:{2}",
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
				svrTrace(Svr::TRC_TRANSACTION, "Trans Missing result drop, TID:{0}",
					pCurTran->GetTransID());
			}
		}

		if (FAILED(hrTem))// Transaction failed
		{
			if (pCurTran->IsPrintTrace())
			{
				svrTrace(Svr::TRC_TRANSACTION, "Trans Proc failed hr={0:X8}, TID:{1}:{2}, Entity:{3}",
					hrTem,
					pCurTran->GetTransID(), typeid(*pCurTran).name(),
					GetEntityUID());
			}
			if (!pCurTran->IsClosed())
				pCurTran->CloseTransaction(E_SYSTEM_FAIL);
		}

		if (pCurTran->IsClosed())
		{
			TickUpdate();
		}

	Proc_End:

		Util::SafeRelease(pTranRes);

		return hr;
	}

	UINT SimpleEntity::GetActiveTransactionCount()
	{
		return m_pCurTran != nullptr ? 1 : 0;
	}

}; // namespace Svr
}; // namespace BR



