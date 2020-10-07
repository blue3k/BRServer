////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author: Kyungkun Ko
//
// Description : Server Transaction implementation
//	
//
////////////////////////////////////////////////////////////////////////////////


#include "ServerSystemPCH.h"

#include "String/SFStrUtil.h"
#include "ServerLog/SvrLog.h"
#include "Thread/SFThread.h"
#include "SvrTrace.h"
#include "Entity/Entity.h"
#include "Transaction/MessageRoute.h"
#include "Transaction/Transaction.h"
#include "Transaction/TransactionSubAction.h"
#include "Server/BrServer.h"
#include "SvrPolicyID.h"





namespace SF {

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
		Transaction::Transaction(IHeap& heap, TransactionID parentTransID)
			: m_Heap(heap)
			, m_parentTransID(parentTransID)
			, m_MessageRouteContext(0)
			, m_pOwner(nullptr)
			, m_transID(0, 0)
			, m_tHeartBitTimeout(DurationMS(5 * 60 * 1000))
			, m_TransactionStartTime(DurationMS(0))
			, m_uiExpectedResultID(0)
			, m_state(STATE_WAITSTART)
			//, m_TimerAction(nullptr)
			, m_CurrentHistoryIdx(0)
			, m_ServerEntity(nullptr)
			, m_DynamicAction(m_Heap)
			, m_SubActionManager(m_Heap)
			, m_Handlers(m_Heap)
		{
			m_Flags.IsExclusive = false;
			m_Flags.IsDeleteByEntity = true;
			m_Flags.IsPrintTrace = true;
			m_Flags.IsDirectProcess = false;

			memset(m_History, 0, sizeof(m_History));

			RegisterMessageHandler<Message::Server::GenericFailureRes>(&Transaction::OnGenericError);
		}

		Transaction::~Transaction()
		{
			Assert(GetReferenceCount() == 0);
			Assert(GetWeakReferenceCount() == 0);
		}


		void Transaction::Dispose()
		{
			assert(GetState() != State::STATE_STARTED);
			//#ifdef DEBUG
			//		auto pEntity = dynamic_cast<MasterEntity*>(GetOwnerEntity());
			//		if (pEntity != nullptr)
			//		{
			//			pEntity->ValidateTransactionCount();
			//		}
			//#endif
		}

		EntityUID Transaction::GetOwnerUID()
		{
			return GetOwnerEntity() != nullptr ? GetOwnerEntity()->GetEntityUID() : EntityUID();
		}


		void Transaction::SetTimerAction(TimerAction* src)
		{
			m_TimerAction = SharedPointerT<TimerAction>(src);
		}

		// Set Owner Entity
		void Transaction::SetOwnerEntity(Entity* pOwner)
		{
			m_pOwner = pOwner;
		}

		Result Transaction::InitializeTransaction(Entity* pOwner)
		{
			SetTransID(TransactionID(pOwner->GetEntityID(), GetTransID().GetTransactionIndex()));
			SetOwnerEntity(pOwner);
			return ResultCode::SUCCESS;
		}

		// Start Transaction
		Result Transaction::StartTransaction()
		{
			Result hr = ResultCode::SUCCESS;

			if (GetState() != Transaction::STATE_WAITSTART)
				svrErr(ResultCode::SVR_TRANSACTION_INVALID_STATE);

			m_TransactionStartTime = Util::Time.GetTimeMs();

			m_state = STATE_STARTED;

			UpdateHeartBitTime();

			m_SubActionManager.Process();

		Proc_End:

			return hr;
		}

		void Transaction::RecordTransactionHistory(const UniquePtr<TransactionResult>& pRes)
		{
			auto historyIndex = m_CurrentHistoryIdx++ % countof(m_History);
			m_History[historyIndex].TimeStamp = Util::Time.GetTimeMs();
			m_History[historyIndex].MsgID = pRes->GetMsgID();
			m_History[historyIndex].hrRes = pRes->GetResult();
		}

		// Process Transaction
		Result Transaction::ProcessTransaction(UniquePtr<TransactionResult>& pRes)
		{
			FunctionContext hr;

			svrCheckPtr(pRes);

			hr = m_Handlers.HandleMessage(pRes->GetMsgID(), pRes.get());
			if (!hr)
			{
				if (hr == Result(ResultCode::SVR_NO_RESULT_HANDLER))
				{
					svrTrace(Error, "Transaction has no result handler : Result MessageID:{0}, {1}", pRes->GetMsgID(), typeid(*this).name());
				}
				else
				{
					svrTrace(Error, "Transaction result process failed: Result MessageID:{0}, {1}", pRes->GetMsgID(), typeid(*this).name());
				}
			}
			else
			{
				hr = m_SubActionManager.Process();
			}

			return hr;
		}

		// Close transaction and notify to parent
		// process abnormal termination of transaction
		Result Transaction::CloseTransaction(Result hrRes)
		{
			Result hr = ResultCode::SUCCESS;

			if (IsClosed())
				return ResultCode::SUCCESS;

			OnCloseTransaction(hrRes);

			//Proc_End:

			SetClosed();

			return hr;
		}

		// flush transaction result
		Result Transaction::FlushTransaction()
		{
			return ResultCode::SUCCESS;
		}


		Result Transaction::OnGenericError(Svr::TransactionResult* pRes)
		{
			Result hr = ResultCode::SUCCESS;

			CloseTransaction(pRes->GetResult());

			return hr;
		}


		///////////////////////////////////////////////////////////
		// Helper functions

		const SharedPointerAtomicT<Net::Connection>& Transaction::GetServerEntityConnection(ServerEntity* pServerEntity)
		{
			static const SharedPointerAtomicT<Net::Connection> Dummy;
			if (pServerEntity == nullptr)
				return Dummy;

			return pServerEntity->GetConnection();
		}


		///////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	SubTransaction base class
		//
		SubTransaction::SubTransaction(IHeap& memoryManager, TransactionID parentTransID, Message::MessageID MsgID)
			:Transaction(memoryManager, parentTransID),
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

		SubTransactionWitResult::SubTransactionWitResult(IHeap& memoryManager, TransactionID parentTransID, Message::MessageID MsgID)
			:SubTransaction(memoryManager, parentTransID, MsgID)
			, m_bFlushRes(false)
			, m_Result(new TransactionResult(MsgID))
		{
			//SetDeleteByEntity(false);
		}

		SubTransactionWitResult::~SubTransactionWitResult()
		{
		}

		Result SubTransactionWitResult::CloseTransaction(Result hrRes)
		{
			FunctionContext hr([this](Result hr)
				{
					SubTransaction::CloseTransaction(hr);
				});

			if (m_Result)
				m_Result->SetResult(hrRes);


			if (IsClosed())
			{
				defError(ResultCode::UNEXPECTED);
			}

			if (!GetParentTransID().IsValid())
				return hr;

			m_bFlushRes = true;

			return hr;
		}



		// flush transaction result
		Result SubTransactionWitResult::FlushTransaction()
		{
			FunctionContext hr;

			SetClosed();

			if (!m_bFlushRes)
				return hr;

			m_bFlushRes = false;

			return hr;
		}



		///////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Transaction result base class
		//
		TransactionResult::TransactionResult()
			://m_uiContext(-1),
			m_transID(-1, 0),
			m_msgID(0),
			m_hrRes(ResultCode::SUCCESS)
		{
		}

		TransactionResult::TransactionResult(Message::MessageID msgID, ResultType resultDataType)
			: m_transID(-1, 0)
			, m_msgID(msgID)
			, m_ResultDataType(resultDataType)
			, m_hrRes(ResultCode::SUCCESS)
		{
		}

		TransactionResult::~TransactionResult()
		{
		}





		///////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Transaction network message result class
		//

		const Message::MessageID TimerResult::MID = Message::MessageID(Message::MSGTYPE_RESULT, Message::MSGTYPE_RELIABLE, Message::MSGTYPE_NONE, PROTOCOLID_SVR_SYSTEM, 1);

		TimerResult::TimerResult()
		{
			SetTransaction(TransactionID(), MID);
			SetResult(ResultCode::SUCCESS_FALSE);
		}

		TimerResult::~TimerResult()
		{
		}



		///////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Transaction network message result class
		//


		MessageResult::MessageResult()
			: TransactionResult(Message::MessageID(0), Svr::TransactionResult::ResultType::MessageData)
		{
		}

		MessageResult::~MessageResult()
		{
		}


		// Setup message result
		Result MessageResult::SetMessage(MessageDataPtr& pIMsg)
		{
			if (pIMsg == nullptr)
				return ResultCode::FAIL;

			Message::MessageHeader* pMsgRes = pIMsg->GetMessageHeader();
			// This assumed that Message result has context
			AssertRel(pIMsg->GetMessageSize() >= (sizeof(Message::MessageHeader) + sizeof(RouteContext) + sizeof(Result) + sizeof(Context)));
			RouteContext* pRouteContext = (RouteContext*)(pMsgRes + 1);
			Context* pContext = (Context*)(pRouteContext + 1);
			Result* phrRes = (Result*)(pContext + 1);
			TransactionID transID;
			transID.ID = *pContext;

			SetTransaction(transID, pMsgRes->msgID);
			SetResult(*phrRes);

			m_pIMsg = std::forward<MessageDataPtr>(pIMsg);
			pIMsg = nullptr;

			return ResultCode::SUCCESS;
		}





	} // namespace Svr
} // namespace SF



