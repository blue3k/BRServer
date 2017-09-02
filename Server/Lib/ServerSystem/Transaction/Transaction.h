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
#include "Util/TimeUtil.h"
#include "ResultCode/SFResultCodeSvr.h"
#include "Memory/MemoryPool.h"
#include "Object/SharedObject.h"
#include "Object/SharedPointer.h"
#include "Container/PageQueue.h"
#include "Util/TimeUtil.h"

#include "Net/NetDef.h"

#include "SvrTrace.h"
#include "Types/BrSvrTypes.h"
#include "MessageHandlerTable.h"
#include "Protocol/Message/ServerMsgClass.h"
#include "Task/TimerSchedulerAction.h"
#include "Net/Connection.h"
#include "Server/BrServerUtil.h"
#include "SvrConst.h"

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

	private:
		// Parent transaction ID
		TransactionID		m_parentTransID;

		RouteContext	m_MessageRouteContext;

		// Owner Entity pointer
		Entity*				m_pOwner;

		// Transaction ID
		TransactionID		m_transID;

		// Transaction heart bit time, ms
		TimeStampMS			m_tHeartBitTimeout;

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
			uint	IsExclusive			: 1;
			uint	IsDeleteByEntity	: 1;
			uint	IsPrintTrace		: 1;
			uint	IsDirectProcess		: 1;
		} m_Flags;


		SharedPointerT<TimerAction>		m_TimerAction;

		// store last two results
		uint m_CurrentHistoryIdx;
		TransactionHistory m_History[2];


		// Server entity who issued this transaction
		// This pointer will be valid for inter server messages
		ServerEntity* m_ServerEntity;


	protected:


		// set Exclusive option
		void SetExclusive( bool bIsExclusive );

		// set DeleteByEntity option
		void SetDeleteByEntity( bool bIsDeleteByEntity );

		// Set PrintTrace
		void SetPrintTrace( bool bisTrace );

		// Set Direct Process
		void SetDirectProcess( bool bisDirectProcess );

		virtual ~Transaction();

	public:
		// Constructor/Destructor
		Transaction( TransactionID parentTransID );

		virtual void Dispose() override;


		inline void SetServerEntity(ServerEntity* pServerEntity)
		{
			assert(pServerEntity != nullptr);
			m_ServerEntity = pServerEntity;
		}

		ServerEntity* GetServerEntity() { return m_ServerEntity; }

		const RouteContext& GetMessageRouteContext()					{ return m_MessageRouteContext; }
		void SetMessageRouteContext(const RouteContext& src)			{ m_MessageRouteContext = src; }

		TimerAction* GetTimerAction()									{ return (TimerAction*)m_TimerAction; }
		void SetTimerAction(TimerAction* src);

		TimeStampMS GetTransactionStartTime()							{ return m_TransactionStartTime;  }

		TimeStampMS GetNextTickTime()									{ return Util::TimeMinNonZero(GetHeartBitTimeout(), GetTimerExpireTime()); }

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
		void SetOwnerEntity( Entity* pOwner );

		// Get Owner Entity
		Entity* GetOwnerEntity();

		// Set transaction ID
		void SetTransID( const TransactionID& transID );

		// Get transaction ID
		const TransactionID& GetTransID() const;

		// Get transaction state
		Transaction::State GetState();

		// Get heart bit time, ms
		//FORCEINLINE ULONG GetHeartBitTime();

		// Update heart bit time, with timestamp
		TimeStampMS UpdateHeartBitTime();

		// Check timeout with timestamp
		Result CheckHeartBitTimeout();
		TimeStampMS GetHeartBitTimeout();

		// Timer
		void SetTimer( DurationMS ms );
		void ClearTimer();
		bool CheckTimer();
		TimeStampMS GetTimerExpireTime();
		bool IsTimerWorking();

		// Get expected result ID
		inline uint GetExpectedResultID();
		inline void SetExpectedResultID( uint uiExpectedResult );

		inline void SetClosed();
		inline bool IsClosed();

		// Initialize Transaction
		virtual Result InitializeTransaction( Entity* pOwner );

		// Start Transaction
		virtual Result StartTransaction();

		// Process Transaction
		virtual Result ProcessTransaction( TransactionResult* &pRes );

		void	RecordTransactionHistory(TransactionResult* pRes);

		virtual Result OnCloseTransaction( Result hrRes ){return ResultCode::SUCCESS;}

		// Close transaction and notify to parent
		// process abnormal termination of transaction
		virtual Result CloseTransaction( Result hrRes );

		// flush transaction result
		virtual Result FlushTransaction();

		///////////////////////////////////////////////////////////
		// Helper functions

		SF::Net::Connection* GetServerEntityConnection(ServerEntity* pServerEntity);
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
			SubTransaction( TransactionID parentTransID , SF::Message::MessageID MsgID );
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
	private:
		// transaction ID
		TransactionID	m_transID;

		// Transaction msg ID
		SF::Message::MessageID	m_msgID;

		// transaction result
		Result				m_hrRes;

	public:
		TransactionResult();
		TransactionResult(SF::Message::MessageID msgID );
		virtual ~TransactionResult();

		// Set transaction info
		inline void SetTransaction( const TransactionID &transID, SF::Message::MessageID msgID );

		// 
		inline void SetTransaction( const TransactionID &transID );

		// Set Result
		inline void SetResult( Result hrRes );

		// Transaction ID
		inline const TransactionID& GetTransID() const;

		// Get Message ID
		inline SF::Message::MessageID GetMsgID() const;

		// Get result value
		inline Result GetResult() const;

		// Virtual release operation
		virtual void Release();
	};



	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Timer result class
	//

	class TimerResult : public TransactionResult, public MemoryPoolObject<TimerResult>
	{
	public:
		const static SF::Message::MessageID MID;

	private:

	public:

		TimerResult();
		virtual ~TimerResult();

		// Release method ovrride
		virtual void Release();
	};


	
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Message result base class
	//

	class MessageResult : public TransactionResult, public MemoryPoolObject<MessageResult>
	{
	private:
		// Message that result require
		SF::Message::MessageData *m_pIMsg;

	public:

		MessageResult();
		virtual ~MessageResult();

		// Release Object called by ObjectPool
		virtual void ReleaseObjectByPool();

		// Setup message result
		Result SetMessage(SF::Message::MessageData* &pIMsg );

		// Get message 
		inline SF::Message::MessageData* GetMessage();

		// Release method override
		virtual void Release();
	};






	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	SubTransaction + result
	//

	class SubTransactionWitResult : public SubTransaction, public TransactionResult
	{
	private:
		bool	m_bFlushRes;

	public:
		SubTransactionWitResult( TransactionID parentTransID , Message::MessageID MsgID );
		virtual ~SubTransactionWitResult();

		// Get transaction ID
		inline const TransactionID& GetTransID() const;

		virtual Result CloseTransaction( Result hrRes ) override;


		// flush transaction result
		virtual Result FlushTransaction() override;

		virtual void Release() override;
	};
	
	
	template< class TransactionType >
	class SubTransactionWitResultMemoryPooled : public SubTransaction, public TransactionResult, public MemoryPoolObject<TransactionType>
	{
	public:
		SubTransactionWitResultMemoryPooled( TransactionID parentTransID , SF::Message::MessageID MsgID )
			:SubTransactionWitResult( parentTransID, MsgID )
		{}

		virtual void Release() { delete this; }
	};
	



	typedef std::function<Result(TransactionResult* pRes)> TransactionMessageHandlerType;

	// Message transaction template
	template< class OwnerType, class MemoryPoolClass >
	class TransactionT : public Transaction, public MemoryPoolObject<MemoryPoolClass>
	{
	public:
		typedef TransactionMessageHandlerType MessageHandlerType;

	private:

		// Message handler table
		MessageHandlerTable<TransactionMessageHandlerType>		m_Handlers;

	public:
		TransactionT(IMemoryManager& memMgr, TransactionID transID )
			:Transaction(memMgr, transID )
			,m_Handlers(memMgr)
		{
			BR_TRANS_MESSAGE(Message::Server::GenericFailureRes, { return OnGenericError(pRes); });
		}

		virtual ~TransactionT()
		{
		}

		virtual Result OnGenericError(Svr::TransactionResult* &pRes)
		{
			Result hr = ResultCode::SUCCESS;

			CloseTransaction(pRes->GetResult());

			return hr;
		}


		// Register message handler
		template< class MessageClassType >
		FORCEINLINE Result RegisterMessageHandler(const char* fileName, uint lineNumber, MessageHandlerType newHandler )
		{
			return m_Handlers.Register<MessageClassType>(fileName, lineNumber, newHandler);
		}

		// Caller handler 
		virtual Result ProcessTransaction( TransactionResult* &pRes )
		{
			Result hr = ResultCode::SUCCESS;

			hr = m_Handlers.HandleMessage(pRes->GetMsgID(), pRes);

		//Proc_End:

			if( !(hr) )
			{
				if( hr == Result(ResultCode::SVR_NO_RESULT_HANDLER))
				{
					svrTrace( Trace::TRC_ERROR, "Transaction has no result handler : Result MessageID:{0}, {1}", pRes->GetMsgID(), typeid(*this).name() );
				}
				else
				{
					svrTrace( Trace::TRC_ERROR, "Transaction result process failed: Result MessageID:{0}, {1}", pRes->GetMsgID(), typeid(*this).name() );
				}
			}

			Util::SafeRelease( pRes );

			return hr;
		}

		// Message processor. When get result, just check result and close
		Result OnMessageClose( Svr::TransactionResult* &pRes )
		{
			Result hr = ResultCode::SUCCESS;

			svrChk(pRes->GetResult());

		Proc_End:

			CloseTransaction(hr);

			return hr; 
		}

		//// implementation of release
		//virtual void Release()
		//{
		//	delete this;
		//}

		// Get owner instance
		OwnerType* GetMyOwner()
		{
			Assert( GetOwnerEntity() );
			return (OwnerType*)GetOwnerEntity();
		}

		EntityUID GetOwnerEntityUID()
		{
			Assert( GetOwnerEntity() );
			return GetOwnerEntity()->GetEntityUID();
		}
	};


	// Message transaction template
	template< class OwnerType, class PolicyClass, class MessageClass, class MemoryPoolClass >
	class MessageTransaction : public TransactionT<OwnerType,MemoryPoolClass>, public MessageClass
	{
	protected:
		// Policy
		SharedPointerT<Net::Connection> m_pConn;

	public:
		MessageTransaction(IMemoryManager& memoryManager, Message::MessageData* &pIMsg )
			: TransactionT<OwnerType, MemoryPoolClass>(memoryManager, TransactionID() )
			, MessageClass( pIMsg )
		{
		}

		Result ParseMessage()
		{
			Result hr = MessageClass::ParseMsg();
			if ((hr))
			{
				if (MessageClass::GetMessage()->GetMessageHeader()->msgID.IDs.Type == Message::MSGTYPE_COMMAND)
				{
					if(MessageClass::HasTransactionID)
					{
						TransactionT<OwnerType, MemoryPoolClass, MessageHandlerBufferSize>::SetParentTransID(MessageClass::GetTransactionID());
					}
					else if(MessageClass::HasRouteContext)
					{
						TransactionT<OwnerType, MemoryPoolClass, MessageHandlerBufferSize>::SetMessageRouteContext(MessageClass::GetRouteContext());
					}
				}
			}
			return hr;
		}

		// Initialize Transaction
		virtual Result InitializeTransaction( Entity *pOwner )
		{
			Result hr = ResultCode::SUCCESS;
			//OwnerType *pOwnerEntity = nullptr;

			svrChkPtr( pOwner );

			svrChk(ParseMessage());

			//pOwnerEntity = dynamic_cast<OwnerType*>(pOwner);


			hr = TransactionT<OwnerType, MemoryPoolClass, MessageHandlerBufferSize>::InitializeTransaction(pOwner);
			svrChk(hr);

		Proc_End:
			return hr;
		}

		virtual ~MessageTransaction()
		{
		}
		
		virtual PolicyClass* GetInterface()
		{
			if (TransactionT<OwnerType, MemoryPoolClass, MessageHandlerBufferSize>::GetOwnerEntity() == nullptr)
				return nullptr;

			TransactionT<OwnerType, MemoryPoolClass, MessageHandlerBufferSize>::GetMyOwner()->GetConnectionShared(m_pConn);

			if (m_pConn != nullptr)
			{
				return m_pConn->GetInterface<PolicyClass>();
			}
			return nullptr;
		}

		template< class PolicyType >
		PolicyType* GetInterface()
		{
			if (TransactionT<OwnerType, MemoryPoolClass, MessageHandlerBufferSize>::GetOwnerEntity() == nullptr)
				return nullptr;

			TransactionT<OwnerType, MemoryPoolClass, MessageHandlerBufferSize>::GetMyOwner()->GetConnectionShared(m_pConn);

			if (m_pConn != nullptr)
			{
				return m_pConn->GetInterface<PolicyType>();
			}
			return nullptr;
		}
	};


	// User event routing transaction, result policy will be routed to user
	template< class OwnerEntityType, class PolicyType, class MessageClass, class TransactionType >
	class UserTransactionS2SEvt : public MessageTransaction<OwnerEntityType,PolicyType, MessageClass, TransactionType>
	{
	protected:
		UserTransactionS2SEvt(IMemoryManager& memMgr, Message::MessageData* &pIMsg )
			:MessageTransaction<OwnerEntityType, PolicyType, MessageClass, TransactionType>(memMgr, pIMsg )
		{
		}

		// Initialize Transaction
		virtual Result InitializeTransaction( Entity *pOwner )
		{
			Result hr = ResultCode::SUCCESS;

			OwnerEntityType *pOwnerEntity = nullptr;
			SharedPointerT<Entity> pFound;

			// We ignore input owner
			pOwner = nullptr;

			hr = MessageTransaction<OwnerEntityType, PolicyType, MessageClass, TransactionType, MessageHandlerBufferSize>::ParseMessage();
			svrChk(hr);

			if(MessageClass::GetRouteContext().GetTo().GetServerID() != Svr::GetMyServerID() )
			{
				svrTrace( Trace::TRC_ERROR, "Invalid ServerID {0} MsgID:{0}", typeid(*this).name(), MessageClass::GetMessage()->GetMessageHeader()->msgID );
				svrErr(ResultCode::SVR_INVALID_SERVERID);
			}

			svrChk(FindEntity(MessageClass::GetRouteContext().GetTo().GetEntityID(), pFound));

			svrChkPtr(pOwnerEntity = static_cast<OwnerEntityType*>((Entity*)pFound));

			//if( pOwnerEntity->GetConnection() != nullptr )
			//{
			//	svrChkPtr( pOwnerEntity->GetConnection()->GetInterface<PolicyType>() );
			//}


			hr = MessageTransaction<OwnerEntityType, PolicyType, MessageClass, TransactionType, MessageHandlerBufferSize>::InitializeTransaction(pOwnerEntity);
			svrChk(hr);

		Proc_End:

			return hr;
		}
	};



	// User S2S command routing transaction, result policy will be routed to server
	template< class OwnerEntityType, class PolicyType, class MessageClass, class TransactionType >
	class UserTransactionS2SCmd : public MessageTransaction<OwnerEntityType,PolicyType, MessageClass, TransactionType>
	{
	private:
		typedef MessageTransaction<OwnerEntityType, PolicyType, MessageClass, TransactionType> superTrans;

	protected:
		UserTransactionS2SCmd(IMemoryManager& memMgr, Message::MessageData* &pIMsg )
			: MessageTransaction<OwnerEntityType, PolicyType, MessageClass, TransactionType>(memMgr, pIMsg )
		{
		}


		// Initialize Transaction
		virtual Result InitializeTransaction( Entity *pOwner )
		{
			Result hr = ResultCode::SUCCESS;
			SharedPointerT<Entity> entity;

			OwnerEntityType *pOwnerEntity = nullptr;

			hr = MessageTransaction<OwnerEntityType, PolicyType, MessageClass, TransactionType, MessageHandlerBufferSize>::ParseMessage();
			svrChk(hr);

			if (!(FindEntity(MessageClass::GetRouteContext().GetTo().GetEntityID(), entity)))
			{
				// Can't find target player entity, maybe logged out?
				hr = ResultCode::SVR_INVALID_ENTITYUID;
				goto Proc_End;
			}

			svrChkPtr(pOwnerEntity = static_cast<OwnerEntityType*>((Entity*)entity));

			assert(superTrans::GetServerEntity() != nullptr);
			svrChkPtr(superTrans::GetServerEntity());
			// S2S Communication so return policy owner is server entity
			//svrChkPtr( m_ServerEntity = BR_DYNAMIC_CAST(ServerEntity*, pOwner) );

			svrChk( Transaction::InitializeTransaction( pOwnerEntity ) );

		Proc_End:

			return hr;
		}

		virtual PolicyType* GetInterface() override
		{
			auto pConn = Transaction::GetServerEntityConnection(superTrans::GetServerEntity());
			if (pConn != nullptr)
			{
				return pConn->template GetInterface<PolicyType>();
			}
			return nullptr;
		}

	};





#include "Transaction.inl"

}; // namespace Svr

extern template class PageQueue<Svr::Transaction*>;
extern template class PageQueue<Svr::TransactionResult*>;
extern template class SharedPointerT < Svr::Transaction >;
extern template class WeakPointerT < Svr::Transaction >;

}; // namespace SF





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Transaction helper utility
//


#include "TransactionUtil.h"
