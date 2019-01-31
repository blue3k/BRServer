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

		IHeap& m_Heap;

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
		Transaction( IHeap& memoryManager, TransactionID parentTransID );

		virtual void Dispose() override;

		IHeap& GetHeap() { return m_Heap; }

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

		EntityUID GetOwnerUID();

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

		const SharedPointerAtomicT<Net::Connection>& GetServerEntityConnection(ServerEntity* pServerEntity);
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
			SubTransaction(IHeap& memoryManager, TransactionID parentTransID , SF::Message::MessageID MsgID );
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
		Result SetMessage(MessageDataPtr &pIMsg );

		// Get message 
		inline SF::MessageDataPtr GetMessage();
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
		SubTransactionWitResult(IHeap& memoryManager, TransactionID parentTransID , Message::MessageID MsgID );
		virtual ~SubTransactionWitResult();

		// Get transaction ID
		inline const TransactionID& GetTransID() const;

		virtual Result CloseTransaction( Result hrRes ) override;


		// flush transaction result
		virtual Result FlushTransaction() override;
	};

	



	typedef std::function<Result(TransactionResult* pRes)> TransactionMessageHandlerType;

	// Message transaction template
	template< class OwnerType >
	class TransactionT : public Transaction
	{
	public:
		typedef TransactionMessageHandlerType MessageHandlerType;

	private:

		// Message handler table
		MessageHandlerTable<TransactionMessageHandlerType>		m_Handlers;

	public:
		TransactionT(IHeap& memMgr, TransactionID transID )
			: Transaction(memMgr, transID )
			, m_Handlers(memMgr)
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
					svrTrace( Error, "Transaction has no result handler : Result MessageID:{0}, {1}", pRes->GetMsgID(), typeid(*this).name() );
				}
				else
				{
					svrTrace( Error, "Transaction result process failed: Result MessageID:{0}, {1}", pRes->GetMsgID(), typeid(*this).name() );
				}
			}

			Util::SafeDelete( pRes );

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


		// Get owner instance
		OwnerType* GetMyOwner() { return static_cast<OwnerType*>(GetOwnerEntity()); }

		EntityUID GetOwnerEntityUID()		{ Assert( GetOwnerEntity() ); return GetOwnerEntity()->GetEntityUID(); }
	};


	// Message transaction template
	template< class OwnerType, class MessageClass >
	class MessageTransaction : public TransactionT<OwnerType>, public MessageClass
	{
	protected:
		// Policy
		SharedPointerT<Net::Connection> m_pConn;

	public:
		MessageTransaction(IHeap& memoryManager, MessageDataPtr &pIMsg )
			: TransactionT<OwnerType>(memoryManager, TransactionID() )
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
						TransactionT<OwnerType>::SetParentTransID(MessageClass::GetTransactionID());
					}
					else if(MessageClass::HasRouteContext)
					{
						TransactionT<OwnerType>::SetMessageRouteContext(MessageClass::GetRouteContext());
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


			hr = TransactionT<OwnerType>::InitializeTransaction(pOwner);
			svrChk(hr);

		Proc_End:
			return hr;
		}

		virtual ~MessageTransaction()
		{
		}
		
		virtual const SharedPointerAtomicT<Net::Connection>& GetConnection()
		{
			const static SharedPointerAtomicT<Net::Connection> Dummy;
			auto pOwnerEntity = TransactionT<OwnerType>::GetMyOwner();
			if (pOwnerEntity == nullptr)
				return Dummy;

			return pOwnerEntity->GetConnection();
		}

	};


	// User event routing transaction, result policy will be routed to user
	template< class OwnerEntityType, class MessageClass >
	class UserTransactionS2SEvt : public MessageTransaction<OwnerEntityType, MessageClass>
	{
	protected:
		UserTransactionS2SEvt(IHeap& memMgr, MessageDataPtr &pIMsg )
			:MessageTransaction<OwnerEntityType, MessageClass>(memMgr, pIMsg )
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

			hr = MessageTransaction<OwnerEntityType, MessageClass>::ParseMessage();
			svrChk(hr);

			if(MessageClass::GetRouteContext().GetTo().GetServerID() != Svr::GetMyServerID() )
			{
				svrTrace( Error, "Invalid ServerID {0} MsgID:{0}", typeid(*this).name(), MessageClass::GetMessage()->GetMessageHeader()->msgID );
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
		UserTransactionS2SCmd(IHeap& memMgr, MessageDataPtr &pIMsg )
			: MessageTransaction<OwnerEntityType, MessageClass>(memMgr, pIMsg )
		{
		}


		// Initialize Transaction
		virtual Result InitializeTransaction( Entity *pOwner )
		{
			Result hr = ResultCode::SUCCESS;
			SharedPointerT<Entity> entity;

			OwnerEntityType *pOwnerEntity = nullptr;

			hr = MessageTransaction<OwnerEntityType, MessageClass>::ParseMessage();
			svrChk(hr);

			if (!(Service::EntityTable->find(MessageClass::GetRouteContext().GetTo().GetEntityID(), entity)))
			{
				// Can't find target player entity, maybe logged out?
				hr = ResultCode::SVR_INVALID_ENTITYUID;
				goto Proc_End;
			}

			svrChkPtr(pOwnerEntity = static_cast<OwnerEntityType*>((Entity*)entity));

			assert(superTrans::GetServerEntity() != nullptr);
			svrChkPtr(superTrans::GetServerEntity());
			// S2S Communication so return policy owner is server entity
			//svrChkPtr( m_ServerEntity = dynamic_cast<ServerEntity>( pOwner) );

			svrChk( Transaction::InitializeTransaction( pOwnerEntity ) );

		Proc_End:

			return hr;
		}

		const SharedPointerAtomicT<Net::Connection>& GetConnection()
		{
			return Transaction::GetServerEntityConnection(superTrans::GetServerEntity());
		}


	};





#include "Transaction.inl"

}; // namespace Svr

typedef SharedPointerT < Svr::Transaction > TransactionPtr;

extern template class PageQueue<Svr::Transaction*>;
extern template class PageQueue<Svr::TransactionResult*>;
extern template class SharedPointerT < Svr::Transaction >;
extern template class WeakPointerT < Svr::Transaction >;

template<> inline TransactionPtr DefaultValue<TransactionPtr>() { return TransactionPtr(); }



}; // namespace SF





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Transaction helper utility
//


#include "TransactionUtil.h"
