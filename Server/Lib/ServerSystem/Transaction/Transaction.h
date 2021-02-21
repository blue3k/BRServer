////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Server Transaction
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once


#include "SFTypedefs.h"
#include "Util/SFTimeUtil.h"
#include "ResultCode/SFResultCodeSvr.h"
#include "Object/SFSharedObject.h"
#include "Object/SFSharedPointer.h"
#include "Container/SFPageQueue.h"
#include "Util/SFTimeUtil.h"

#include "Net/SFNetDef.h"

#include "SvrTrace.h"
#include "Types/BrSvrTypes.h"
#include "MessageHandlerTable.h"
#include "Protocol/Message/ServerMsgClass.h"
#include "Task/SFTimerSchedulerAction.h"
#include "Net/SFConnection.h"
#include "Server/BrServerUtil.h"
#include "SvrConst.h"
#include "Service/ServerService.h"
#include "Entity/Entity.h"
#include "Transaction/TransactionSubAction.h"


namespace SF {
	class TimerAction;

	namespace Svr {

		class Entity;
		class TransactionResult;
		class ServerEntity;

		///////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Transaction ID area
		//
		// 0 : Reserved for server instance
		// 1~ PLUGIN_ENTITYID_BASE -1 : User or Instance entity
		// PLUGIN_ENTITYID_BASE + serverID : Plugin entities

		///////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Transaction
		//

		class Transaction : public SharedObject
		{
		public:
			typedef enum tag_State
			{
				STATE_WAITSTART,
				STATE_STARTED,
				STATE_CLOSED,
			} State;


			struct TransactionHistory
			{
				TimeStampMS				TimeStamp;

				// Transaction msg ID
				SF::Message::MessageID	MsgID;

				// transaction result
				Result				hrRes;
			};




			////////////////////////////////////////////////////////////////////////////////////////
			//
			//	TransactionSubActionManager class
			//

			template<typename TransactionClassType,
				typename = std::enable_if_t<std::is_base_of_v<Transaction, TransactionClassType>>>
			class TransactionSubActionMemberT : public TransactionSubAction
			{
				typedef Result(TransactionClassType::* RequestFunc)();

			private:
				TransactionClassType* m_pOwnerTransaction = nullptr;
				RequestFunc m_RequestFunc;
				uint m_ExpectedResultId = 0;

			public:
				TransactionSubActionMemberT(TransactionClassType* pOwnerTrans, RequestFunc InRequestFunc, uint InExpectedResultId)
					: m_pOwnerTransaction(pOwnerTrans)
					, m_RequestFunc(InRequestFunc)
					, m_ExpectedResultId(InExpectedResultId)
				{
				}

				virtual Result Process() override
				{
					return (m_pOwnerTransaction->*m_RequestFunc)();
				}

			};




		public:

			using TransactionMessageHandlerType = std::function<Result(TransactionResult* pRes)>;
			using MessageHandlerType = TransactionMessageHandlerType;

		private:

			IHeap& m_Heap;

			// Parent transaction ID
			TransactionID		m_parentTransID;

			RouteContext	m_MessageRouteContext;

			// Owner Entity pointer
			Entity* m_pOwner;

			// Transaction ID
			TransactionID		m_transID;

			// Transaction heartbeat time, ms
			TimeStampMS			m_tHeartbeatTimeout;

			// Transaction start time
			TimeStampMS			m_TransactionStartTime;

			// Timer
			Util::TimeStampTimer m_Timer;

			// Expected result ID
			uint				m_uiExpectedResultID;

			// State 
			State	m_state;

			// Flags
			struct FLAGS {
				uint	IsExclusive : 1;
				uint	IsDeleteByEntity : 1;
				uint	IsPrintTrace : 1;
				uint	IsDirectProcess : 1;
			} m_Flags;


			SharedPointerT<TimerAction>		m_TimerAction;

			// store last two results
			uint m_CurrentHistoryIdx;
			TransactionHistory m_History[2];

			// remote endpoint for return
			SharedPointerT<MessageEndpoint> m_RemoteEndpoint;

			// Dynamically created sub actions
			DynamicArray<TransactionSubAction*> m_DynamicAction;

			// sub action manager
			TransactionSubActionManager<6> m_SubActionManager;

			// Message handler table
			MessageHandlerTable<TransactionMessageHandlerType>		m_Handlers;


		protected:

			// set Exclusive option
			void SetExclusive(bool bIsExclusive);

			// set DeleteByEntity option
			void SetDeleteByEntity(bool bIsDeleteByEntity);

			// Set PrintTrace
			void SetPrintTrace(bool bisTrace);

			// Set Direct Process
			void SetDirectProcess(bool bisDirectProcess);

			virtual ~Transaction();

			// Generic error handler
			virtual Result OnGenericError(Svr::TransactionResult* pRes);

		public:
			// Constructor/Destructor
			Transaction(IHeap& heap, TransactionID parentTransID);

			virtual void Dispose() override;

			IHeap& GetHeap() { return m_Heap; }

			inline void SetRemoteEndpoint(const SharedPointerT<MessageEndpoint>& remoteEndpoint)
			{
				m_RemoteEndpoint = remoteEndpoint;
			}

			const SharedPointerT<MessageEndpoint>& GetRemoteEndpoint() { return m_RemoteEndpoint; }

			const RouteContext& GetMessageRouteContext() { return m_MessageRouteContext; }
			void SetMessageRouteContext(const RouteContext& src) { m_MessageRouteContext = src; }

			TimerAction* GetTimerAction() { return (TimerAction*)m_TimerAction; }
			void SetTimerAction(TimerAction* src);

			TimeStampMS GetTransactionStartTime() { return m_TransactionStartTime; }

			TimeStampMS GetNextTickTime() { return Util::TimeMinNonZero(GetHeartbeatTimeout(), GetTimerExpireTime()); }

			// Get Exclusive option
			bool	IsExclusive() const;

			// Get Delete by entity
			bool	IsDeleteByEntity() const;

			// Get Print Trace
			bool	IsPrintTrace() const;

			// Get Direct process 
			bool	IsDirectProcess() const;

			// Get parent Transaction ID
			const TransactionID& GetParentTransID() const;
			void SetParentTransID(const TransactionID& transID);

			// Set Owner Entity
			void SetOwnerEntity(Entity* pOwner);

			// Get Owner Entity
			Entity* GetOwnerEntity();

			EntityUID GetOwnerUID();

			// Set transaction ID
			void SetTransID(const TransactionID& transID);

			// Get transaction ID
			const TransactionID& GetTransID() const;

			// Get transaction state
			Transaction::State GetState();

			// Update heartbeat time, with timestamp
			TimeStampMS UpdateHeartbeatTime();

			// Check timeout with timestamp
			Result CheckHeartbeatTimeout();
			TimeStampMS GetHeartbeatTimeout();

			// Timer
			void SetTimer(DurationMS ms);
			void ClearTimer();
			bool CheckTimer();
			TimeStampMS GetTimerExpireTime();
			bool IsTimerWorking();

			// Get expected result ID
			inline uint GetExpectedResultID();
			inline void SetExpectedResultID(uint uiExpectedResult);

			inline void SetClosed();
			inline bool IsClosed();

			// Initialize Transaction
			virtual Result InitializeTransaction(Entity* pOwner);

			// Start Transaction
			virtual Result StartTransaction();

			// Process Transaction
			virtual Result ProcessTransaction(SFUniquePtr<TransactionResult>& pRes);

			void RecordTransactionHistory(const SFUniquePtr<TransactionResult>& pRes);

			virtual Result OnCloseTransaction(Result hrRes) { return ResultCode::SUCCESS; }

			// Close transaction and notify to parent
			// process abnormal termination of transaction
			virtual Result CloseTransaction(Result hrRes);

			// flush transaction result
			virtual Result FlushTransaction();

		protected:

			template<typename TransactionClassType
				, typename = std::enable_if_t<std::is_base_of_v<Transaction, TransactionClassType>>
			>
			SF_FORCEINLINE Result AddSubAction(Result(TransactionClassType::* requestFunc)())
			{
				auto newAction = new(GetHeap()) TransactionSubActionMemberT<TransactionClassType>(static_cast<TransactionClassType*>(this), requestFunc, 0);
				m_DynamicAction.push_back(newAction);

				return m_SubActionManager.AddAction(newAction);
			}

			template<typename MessageClassType, typename TransactionClassType
				, typename = std::enable_if_t<std::is_base_of_v<Transaction, TransactionClassType>>
			>
			SF_FORCEINLINE Result AddSubAction(Result(TransactionClassType::* requestFunc)(), Result(TransactionClassType::* handlerFunc)(TransactionResult*))
			{
				ScopeContext hr;
				auto newAction = new(GetHeap()) TransactionSubActionMemberT<TransactionClassType>(static_cast<TransactionClassType*>(this), requestFunc, MessageClassType::MID.IDSeq.MsgID);
				m_DynamicAction.push_back(newAction);

				svrCheck(m_SubActionManager.AddAction(newAction));

				if (handlerFunc != nullptr)
					svrCheck(RegisterMessageHandler<MessageClassType>(handlerFunc));

				return hr;
			}

			SF_FORCEINLINE Result AddSubAction(TransactionSubAction* pAction)
			{
				return m_SubActionManager.AddAction(pAction);
			}
			
			template<typename TransactionClassType
				, typename = std::enable_if_t<std::is_base_of_v<Transaction, TransactionClassType>>
			>
			SF_FORCEINLINE Result AddSubActionFront(Result(TransactionClassType::* requestFunc)())
			{
				auto newAction = new(GetHeap()) TransactionSubActionMemberT<TransactionClassType>(static_cast<TransactionClassType*>(this), requestFunc, 0);
				m_DynamicAction.push_back(newAction);

				return m_SubActionManager.AddActionFront(newAction);
			}

			template<typename MessageClassType, typename TransactionClassType
				, typename = std::enable_if_t<std::is_base_of_v<Transaction, TransactionClassType>>
			>
			SF_FORCEINLINE Result AddSubActionFront(Result(TransactionClassType::* requestFunc)(), Result(TransactionClassType::* handlerFunc)(TransactionResult*))
			{
				ScopeContext hr;
				auto newAction = new(GetHeap()) TransactionSubActionMemberT<TransactionClassType>(static_cast<TransactionClassType*>(this), requestFunc, MessageClassType::MID.IDSeq.MsgID);
				m_DynamicAction.push_back(newAction);

				svrCheck(m_SubActionManager.AddActionFront(newAction));

				if (handlerFunc != nullptr)
					svrCheck(RegisterMessageHandler<MessageClassType>(handlerFunc));

				return hr;
			}

			SF_FORCEINLINE Result AddSubActionFront(TransactionSubAction* pAction)
			{
				return m_SubActionManager.AddActionFront(pAction);
			}

			// Default finalize success implementation
			virtual Result FinalizeSuccess()
			{
				return CloseTransaction(ResultCode::SUCCESS);
			}

			// Register message handler
			template< typename MessageClassType >
			SF_FORCEINLINE Result RegisterMessageHandler(MessageHandlerType&& newHandler)
			{
				return m_Handlers.Register<MessageClassType>(std::forward<MessageHandlerType>(newHandler));
			}

			template<typename MessageClassType, typename TransactionClassType
				, typename = std::enable_if_t<std::is_base_of_v<Transaction, TransactionClassType>>
			>
			SF_FORCEINLINE Result RegisterMessageHandler(Result(TransactionClassType::* handlerFunc)(TransactionResult*))
			{
				return m_Handlers.Register<MessageClassType>([this, handlerFunc](TransactionResult* pRes) { return (static_cast<std::decay_t<TransactionClassType>*>(this)->*handlerFunc)(pRes); });
			}

		};



		///////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	SubTransaction base class
		//

		class SubTransaction : public Transaction
		{
		private:
			SF::Message::MessageID m_MsgID;

		public:
			SubTransaction(IHeap& memoryManager, TransactionID parentTransID, SF::Message::MessageID MsgID);
			virtual ~SubTransaction();

			//virtual Result CloseTransaction( Result hrRes );

			SF::Message::MessageID GetMsgID() { return m_MsgID; }

		};



		////////////////////////////////////////////////////////////////////////////////
		//
		//	Network Packet Message macros
		//

		class TransactionResult
		{
		public:

			enum class ResultType : uint8_t
			{
				Generic,		// Generic c++ type
				MessageData,	// Message data
				DBQuery,		// DB query data
			};

		private:
			// transaction ID
			TransactionID	m_transID;

			// Transaction msg ID
			SF::Message::MessageID	m_msgID;

			// Transaction result data type
			ResultType m_ResultDataType = ResultType::Generic;

			// transaction result
			Result				m_hrRes;

		public:
			TransactionResult();
			TransactionResult(SF::Message::MessageID msgID, ResultType resultDataType = ResultType::Generic);
			virtual ~TransactionResult();

			// Set transaction info
			inline void SetTransaction(const TransactionID& transID, SF::Message::MessageID msgID);

			// 
			inline void SetTransaction(const TransactionID& transID);

			// Set Result
			inline void SetResult(Result hrRes);

			// Transaction ID
			inline const TransactionID& GetTransID() const;

			// Get Message ID
			inline SF::Message::MessageID GetMsgID() const;

			// Get result value
			inline Result GetResult() const;

			inline ResultType GetResultDataType() const { return m_ResultDataType; }


			// Get result data
			//	Return type will be different based on result data type
			template<typename ResultDataType>
			inline const ResultDataType* GetResultData() const
			{
				if (m_ResultDataType == ResultType::MessageData)
				{
					return nullptr;
				}
				else
				{
					return static_cast<const ResultDataType*>(this);
				}
			}

		};



		///////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Timer result class
		//

		class TimerResult : public TransactionResult
		{
		public:
			const static SF::Message::MessageID MID;

		private:

		public:

			TimerResult();
			virtual ~TimerResult();
		};



		///////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Message result base class
		//

		class MessageResult : public TransactionResult
		{
		private:
			// Message that result require
			MessageDataPtr m_pIMsg;

		public:

			MessageResult();
			virtual ~MessageResult();

			// Setup message result
			Result SetMessage(MessageDataPtr& pIMsg);

			// Get message 
			inline SF::MessageDataPtr GetMessage() const { return *m_pIMsg; }

		};






		///////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	SubTransaction + result
		//

		class SubTransactionWitResult : public SubTransaction
		{
		private:
			bool	m_bFlushRes;

			SFUniquePtr<TransactionResult> m_Result;

		public:
			SubTransactionWitResult(IHeap& memoryManager, TransactionID parentTransID, Message::MessageID MsgID);
			virtual ~SubTransactionWitResult();

			SFUniquePtr<TransactionResult>& GetResult() { return m_Result; }

			// Get transaction ID
			inline const TransactionID& GetTransID() const;

			virtual Result CloseTransaction(Result hrRes) override;


			// flush transaction result
			virtual Result FlushTransaction() override;
		};




		// Transaction template, handling transaction result
		template< class OwnerType >
		class TransactionT : public Transaction
		{

		public:
			TransactionT(IHeap& memMgr, TransactionID transID)
				: Transaction(memMgr, transID)
			{
			}

			virtual ~TransactionT()
			{
			}

			// Message processor. When get result, just check result and close
			Result OnMessageClose(Svr::TransactionResult*& pRes)
			{
				ScopeContext hr([this](Result hr)
					{
						CloseTransaction(hr);
					});

				svrCheck(pRes->GetResult());

				return hr;
			}


			// Get owner instance
			OwnerType* GetMyOwner() { return static_cast<OwnerType*>(GetOwnerEntity()); }

			EntityUID GetOwnerEntityUID() { Assert(GetOwnerEntity()); return GetOwnerEntity()->GetEntityUID(); }
		};


		// Message transaction template
		template< class OwnerType, class MessageClass >
		class MessageTransaction : public TransactionT<OwnerType>, public MessageClass
		{
		public:

			using MessageClassType = MessageClass;

		protected:
			// Policy
			SharedPointerT<Net::Connection> m_pConn;

		public:
			MessageTransaction(IHeap& memoryManager, MessageDataPtr&& pIMsg)
				: TransactionT<OwnerType>(memoryManager, TransactionID())
				, MessageClass(std::forward<MessageDataPtr>(pIMsg))
			{
			}

			Result ParseMessage()
			{
				Result hr = MessageClass::ParseMsg();
				if (hr)
				{
					if (MessageClass::GetMessage()->GetMessageHeader()->msgID.IDs.Type == Message::MSGTYPE_COMMAND)
					{
						if (MessageClass::HasTransactionID)
						{
							TransactionT<OwnerType>::SetParentTransID(MessageClass::GetTransactionID());
						}
						else if (MessageClass::HasRouteContext)
						{
							TransactionT<OwnerType>::SetMessageRouteContext(MessageClass::GetRouteContext());
						}
					}
				}
				return hr;
			}

			// Initialize Transaction
			virtual Result InitializeTransaction(Entity* pOwner)
			{
				Result hr = ResultCode::SUCCESS;

				svrCheckPtr(pOwner);

				svrCheck(ParseMessage());

				if (Transaction::GetRemoteEndpoint() == nullptr)
				{
					if (MessageClass::GetRouteContext().GetFrom().UID != 0)
					{
						Transaction::SetRemoteEndpoint(Service::MessageEndpointManager->GetEndpoint(MessageClass::GetRouteContext().GetFrom()));
					}
					else
					{
						
					}
				}

				hr = TransactionT<OwnerType>::InitializeTransaction(pOwner);
				svrCheck(hr);

				return hr;
			}

			virtual ~MessageTransaction() = default;

			TransactionID GetMessageContext() { return MessageClass::GetTransactionID(); }
		};


		// User event routing transaction, result policy will be routed to user
		template< class OwnerEntityType, class MessageClass >
		class UserTransactionS2SEvt : public MessageTransaction<OwnerEntityType, MessageClass>
		{
		protected:
			UserTransactionS2SEvt(IHeap& memMgr, MessageDataPtr& pIMsg)
				:MessageTransaction<OwnerEntityType, MessageClass>(memMgr, std::forward<MessageDataPtr>(pIMsg))
			{
			}


			// Initialize Transaction
			virtual Result InitializeTransaction(Entity* pOwner)
			{
				Result hr = ResultCode::SUCCESS;

				OwnerEntityType* pOwnerEntity = nullptr;
				SharedPointerT<Entity> pFound;

				// We ignore input owner
				pOwner = nullptr;

				hr = MessageTransaction<OwnerEntityType, MessageClass>::ParseMessage();
				svrChk(hr);

				if (MessageClass::GetRouteContext().GetTo().GetServerID() != GetMyServerID())
				{
					svrTrace(Error, "Invalid ServerID {0} MsgID:{0}", typeid(*this).name(), MessageClass::GetMessage()->GetMessageHeader()->msgID);
					svrErr(ResultCode::SVR_INVALID_SERVERID);
				}

				svrChk(Service::EntityTable->find(MessageClass::GetRouteContext().GetTo().GetEntityID(), pFound));

				svrChkPtr(pOwnerEntity = static_cast<OwnerEntityType*>((Entity*)pFound));

				//if( pOwnerEntity->GetConnection() != nullptr )
				//{
				//	svrChkPtr( pOwnerEntity->GetConnection()->GetInterface<PolicyType>() );
				//}


				hr = MessageTransaction<OwnerEntityType, MessageClass>::InitializeTransaction(pOwnerEntity);
				svrChk(hr);

			Proc_End:

				return hr;
			}
		};



		// User S2S command routing transaction, result policy will be routed to server
		template< class OwnerEntityType, class MessageClass>
		class UserTransactionS2SCmd : public MessageTransaction<OwnerEntityType, MessageClass>
		{
		private:
			typedef MessageTransaction<OwnerEntityType, MessageClass> superTrans;

		protected:
			UserTransactionS2SCmd(IHeap& memMgr, MessageDataPtr& pIMsg)
				: MessageTransaction<OwnerEntityType, MessageClass>(memMgr, Forward<MessageDataPtr>(pIMsg))
			{
			}

			// Initialize Transaction
			virtual Result InitializeTransaction(Entity* pOwner)
			{
				Result hr = ResultCode::SUCCESS;
				SharedPointerT<Entity> entity;

				OwnerEntityType* pOwnerEntity = nullptr;

				hr = MessageTransaction<OwnerEntityType, MessageClass>::ParseMessage();
				svrCheck(hr);

				if (!(Service::EntityTable->find(MessageClass::GetRouteContext().GetTo().GetEntityID(), entity)))
				{
					// Can't find target player entity, maybe logged out?
					return hr = ResultCode::SVR_INVALID_ENTITYUID;
				}

				svrCheckPtr(pOwnerEntity = static_cast<OwnerEntityType*>((Entity*)entity));

				svrCheck(Transaction::InitializeTransaction(pOwnerEntity));

				return hr;
			}


		};



		template<>
		inline const Message::MessageData* TransactionResult::GetResultData() const
		{
			if (m_ResultDataType == ResultType::MessageData)
			{
				return *static_cast<const MessageResult*>(this)->GetMessage();
			}
			else
			{
				return nullptr;
			}
		}


		#include "Transaction.inl"

	} // namespace Svr

	typedef SharedPointerT < Svr::Transaction > TransactionPtr;

	extern template class PageQueue<Svr::Transaction*>;
	extern template class PageQueue<Svr::TransactionResult*>;
	extern template class SharedPointerT < Svr::Transaction >;
	extern template class WeakPointerT < Svr::Transaction >;




} // namespace SF





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Transaction helper utility
//


#include "TransactionUtil.h"
