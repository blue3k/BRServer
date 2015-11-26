////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : MadK
//
// Description : Server Transaction implementation
//	
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"

#include "Common/StrUtil.h"
#include "Common/Trace.h"
#include "Common/Thread.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/Entity.h"
#include "ServerSystem/MessageRoute.h"
#include "ServerSystem/Transaction.h"
#include "ServerSystem/BrServer.h"
#include "Common/SvrPolicyID.h"


BR_MEMORYPOOL_IMPLEMENT(Svr::MessageResult);
BR_MEMORYPOOL_IMPLEMENT(Svr::TimerResult);


namespace BR {

	template class PageQueue<Svr::Transaction*>;
	template class PageQueue<Svr::TransactionResult*>;

	template class SharedPointerT < Svr::Transaction >;
	template class WeakPointerT < Svr::Transaction >;

	namespace Svr {


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Transaction base class
	//

	// Constructor/Destructor
	Transaction::Transaction( TransactionID parentTransID )
		: m_parentTransID(parentTransID)
		, m_MessageRouteContext(0)
		, m_pOwner(nullptr)
		, m_transID(0,0)
		, m_tHeartBitTimeout(DurationMS(5*60*1000))
		, m_uiExpectedResultID(0)
		, m_state(STATE_WAITSTART)
		, m_bIsExclusive(false)
		, m_bIsDeleteByEntity(true)
		, m_bIsPrintTrace(true)
		, m_bIsDirectProcess(false)
		, m_TimerAction(nullptr)
		, m_CurrentHistoryIdx(0)
		, m_TransactionStartTime(DurationMS(0))
	{
		memset(m_History, 0, sizeof(m_History));
	}

	Transaction::~Transaction()
	{
		Assert(GetReferenceCount() == 0);
		Assert(GetWeakReferenceCount() == 0);
	}


	void Transaction::Dispose()
	{
//#ifdef DEBUG
//		auto pEntity = dynamic_cast<MasterEntity*>(GetOwnerEntity());
//		if (pEntity != nullptr)
//		{
//			pEntity->ValidateTransactionCount();
//		}
//#endif
	}

	void Transaction::SetTimerAction(TimerAction* src)
	{
		m_TimerAction = SharedPointerT<TimerAction>(src);
	}

	// Set Owner Entity
	void Transaction::SetOwnerEntity( Entity* pOwner )
	{
		m_pOwner = pOwner;
	}

	HRESULT Transaction::InitializeTransaction( Entity* pOwner )
	{
		SetTransID( TransactionID( pOwner->GetEntityID(), GetTransID().GetTransactionIndex() ) );
		SetOwnerEntity(pOwner);
		return S_OK;
	}

	// Start Transaction
	HRESULT Transaction::StartTransaction()
	{
		HRESULT hr = S_OK;

		if( GetState() != Transaction::STATE_WAITSTART )
			svrErr( E_SVR_TRANSACTION_INVALID_STATE );

		m_TransactionStartTime = Util::Time.GetTimeMs();

		m_state = STATE_STARTED;

		UpdateHeartBitTime();

	Proc_End:

		return hr;
	}

	void Transaction::RecordTransactionHistory(TransactionResult* pRes)
	{
		auto historyIndex = m_CurrentHistoryIdx++ % countof(m_History);
		m_History[historyIndex].TimeStamp = Util::Time.GetTimeMs();
		m_History[historyIndex].MsgID = pRes->GetMsgID();
		m_History[historyIndex].hrRes = pRes->GetHRESULT();
	}

	// Process Transaction
	HRESULT Transaction::ProcessTransaction( TransactionResult* &pRes )
	{
		HRESULT hr = S_OK;

		svrChkPtr( pRes );

		svrTrace( Trace::TRC_ERROR, "Trans Proc Invalid call, %0% TID:%1%, Msg:%2%", 
								typeid(*this).name(),
								GetTransID(),
								pRes->GetMsgID() );
		svrErr( E_NOTIMPL );

	Proc_End:

		return hr;
	}

	// Close transaction and notify to parent
	// process abnormal termination of transaction
	HRESULT Transaction::CloseTransaction( HRESULT hrRes )
	{
		HRESULT hr = S_OK;

		if( IsClosed() )
			return S_OK;

		OnCloseTransaction(hrRes);

	Proc_End:

		SetClosed();

		return hr;
	}

	// flush transaction result
	HRESULT Transaction::FlushTransaction()
	{
		return S_OK;
	}

	///////////////////////////////////////////////////////////
	// Helper functions

	Net::IConnection* Transaction::GetServerEntityConnection(ServerEntity* pServerEntity)
	{
		if (pServerEntity == nullptr)
			return nullptr;

		return pServerEntity->GetConnection();
	}


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	SubTransaction base class
	//
	SubTransaction::SubTransaction( TransactionID parentTransID ,Message::MessageID MsgID )
			:Transaction( parentTransID ),
			//m_uiContext(0),
			m_MsgID(MsgID)
	{
	}

	SubTransaction::~SubTransaction()
	{
	}


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Sub Transaction with result base class
	//
	
	SubTransactionWithResult::SubTransactionWithResult( TransactionID parentTransID , Message::MessageID MsgID )
		:SubTransaction( parentTransID, MsgID ),
		m_bFlushRes(false)
	{
		//SetDeleteByEntity(false);
		TransactionResult::SetTransaction( parentTransID, MsgID );
	}

	SubTransactionWithResult::~SubTransactionWithResult()
	{
	}

	HRESULT SubTransactionWithResult::CloseTransaction( HRESULT hrRes )
	{
		HRESULT hr = S_OK;

		SetResult( hrRes );


		if( IsClosed() )
		{
			defErr( E_UNEXPECTED );
		}

		if( !GetParentTransID().IsValid() ) goto Proc_End;

		m_bFlushRes = true;

	Proc_End:

		__super::CloseTransaction(hrRes);

		return hr;
	}



	// flush transaction result
	HRESULT SubTransactionWithResult::FlushTransaction()
	{
		HRESULT hr = S_OK;
		TransactionResult *pRes = this;

		SetClosed();

		if (!m_bFlushRes)
			goto Proc_End;

		m_bFlushRes = false;

	Proc_End:


		return hr;
	}

	void SubTransactionWithResult::Release()
	{
		SharedPointer(this);
	}


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Transaction result base class
	//
	TransactionResult::TransactionResult()
		://m_uiContext(-1),
		m_transID(-1,0),
		m_msgID(0),
		m_hrRes(S_OK)
	{
	}

	TransactionResult::TransactionResult( Message::MessageID msgID )
		:
		m_transID(-1,0),
		m_msgID(msgID),
		m_hrRes(S_OK)
	{
	}

	TransactionResult::~TransactionResult()
	{
	}

	void TransactionResult::Release()
	{
		delete this;
	}


	

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Transaction network message result class
	//

	const Message::MessageID TimerResult::MID = Message::MessageID(Message::MSGTYPE_RESULT, Message::MSGTYPE_RELIABLE, Message::MSGTYPE_NONE, POLICY_SVR_SYSTEM, 1);

	TimerResult::TimerResult()
	{
		SetTransaction( TransactionID(), MID );
		SetResult( S_FALSE );
	}

	TimerResult::~TimerResult()
	{
	}

	// Release method ovride
	void TimerResult::Release()
	{
		delete this;
	}


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Transaction network message result class
	//


	MessageResult::MessageResult()
		:m_pIMsg(nullptr)
	{
	}

	MessageResult::~MessageResult()
	{
	}

	// Release Object
	void MessageResult::ReleaseObjectByPool()
	{
		Util::SafeRelease( m_pIMsg );
	}

	// Setup message result
	HRESULT MessageResult::SetMessage( Message::MessageData* &pIMsg )
	{
		if( pIMsg == nullptr )
			return E_FAIL;

		Message::MessageHeader *pMsgRes = pIMsg->GetMessageHeader();
		// This assumed that Message result has context
		AssertRel( pIMsg->GetMessageSize() >= (sizeof(Message::MessageHeader) + sizeof(HRESULT) + sizeof(Context)) );
		Context *pContext = (Context*)(pMsgRes+1);
		HRESULT *phrRes = (HRESULT*)(pContext+1);
		TransactionID transID;
		transID.ID = *pContext;

		SetTransaction( transID, pMsgRes->msgID );
		SetResult( *phrRes );

		m_pIMsg = pIMsg;
		pIMsg = nullptr;

		return TRUE;
	}

	
	// Release method ovride
	void MessageResult::Release()
	{
		delete this;
	}



}; // namespace Svr
}; // namespace BR



