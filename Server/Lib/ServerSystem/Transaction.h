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


#include "Common/Typedefs.h"
#include "Common/TimeUtil.h"
#include "Common/HRESSvrSys.h"
#include "Common/MemoryPool.h"
#include "Common/SharedObject.h"
#include "Common/SharedPointer.h"
#include "Common/PageQueue.h"
#include "Common/TimeUtil.h"

#include "Net/NetDef.h"

#include "ServerSystem/SvrConst.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/SvrTypes.h"
#include "ServerSystem/MessageHandlerTable.h"
#include "Protocol/Message/ServerMsgClass.h"
#include "ServerSystem/TimeSchedulerAction.h"
#include "Net/Connection.h"
#include "ServerSystem/BrServerUtil.h"

namespace BR {
namespace Svr {

	class Entity;
	class TransactionResult;
	class ServerEntity;
	class TimerAction;
	
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
			Message::MessageID	MsgID;

			// transaction result
			HRESULT				hrRes;
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
		UINT				m_uiExpectedResultID;

		// State 
		State	m_state;

		// Flags
		struct FLAGS {
			UINT	IsExclusive			: 1;
			UINT	IsDeleteByEntity	: 1;
			UINT	IsPrintTrace		: 1;
			UINT	IsDirectProcess		: 1;
		} m_Flags;


		SharedPointerT<TimerAction>		m_TimerAction;

		// store last two results
		UINT m_CurrentHistoryIdx;
		TransactionHistory m_History[2];


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
		HRESULT CheckHeartBitTimeout();
		TimeStampMS GetHeartBitTimeout();

		// Timer
		void SetTimer( DurationMS ms );
		void ClearTimer();
		bool CheckTimer();
		TimeStampMS GetTimerExpireTime();
		bool IsTimerWorking();

		// Get expected result ID
		inline UINT GetExpectedResultID();
		inline void SetExpectedResultID( UINT uiExpectedResult );

		inline void SetClosed();
		inline bool IsClosed();

		// Initialize Transaction
		virtual HRESULT InitializeTransaction( Entity* pOwner );

		// Start Transaction
		virtual HRESULT StartTransaction();

		// Process Transaction
		virtual HRESULT ProcessTransaction( TransactionResult* &pRes );

		void	RecordTransactionHistory(TransactionResult* pRes);

		virtual HRESULT OnCloseTransaction( HRESULT hrRes ){return S_OK;}

		// Close transaction and notify to parent
		// process abnormal termination of transaction
		virtual HRESULT CloseTransaction( HRESULT hrRes );

		// flush transaction result
		virtual HRESULT FlushTransaction();

		///////////////////////////////////////////////////////////
		// Helper functions

		Net::IConnection* GetServerEntityConnection(ServerEntity* pServerEntity);
	};



	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	SubTransaction base class
	//

	class SubTransaction : public Transaction
	{
		private:
			Message::MessageID m_MsgID;

		public:
			SubTransaction( TransactionID parentTransID , Message::MessageID MsgID );
			virtual ~SubTransaction();

			//virtual HRESULT CloseTransaction( HRESULT hrRes );

			Message::MessageID GetMsgID() { return m_MsgID; }

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
		Message::MessageID	m_msgID;

		// transaction result
		HRESULT				m_hrRes;

	public:
		TransactionResult();
		TransactionResult( Message::MessageID msgID );
		virtual ~TransactionResult();

		// Set transaction info
		inline void SetTransaction( const TransactionID &transID, Message::MessageID msgID );

		// 
		inline void SetTransaction( const TransactionID &transID );

		// Set Result
		inline void SetResult( HRESULT hrRes );

		// Transaction ID
		inline const TransactionID& GetTransID() const;

		// Get Message ID
		inline Message::MessageID GetMsgID() const;

		// Get result value
		inline HRESULT GetHRESULT() const;

		// Virtual release operation
		virtual void Release();
	};



	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Timer resultclass
	//

	class TimerResult : public TransactionResult, public MemoryPoolObject<TimerResult>
	{
	public:
		const static Message::MessageID MID;

	private:

	public:

		TimerResult();
		virtual ~TimerResult();

		// Release method ovride
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
		Message::MessageData *m_pIMsg;

	public:

		MessageResult();
		virtual ~MessageResult();

		// Release Object called by ObjectPool
		virtual void ReleaseObjectByPool();

		// Setup message result
		HRESULT SetMessage( Message::MessageData* &pIMsg );

		// Get message 
		inline Message::MessageData* GetMessage();

		// Release method ovride
		virtual void Release();
	};






	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	SubTransaction + result
	//

	class SubTransactionWithResult : public SubTransaction, public TransactionResult
	{
	private:
		bool	m_bFlushRes;

	public:
		SubTransactionWithResult( TransactionID parentTransID , Message::MessageID MsgID );
		virtual ~SubTransactionWithResult();

		// Get transaction ID
		inline const TransactionID& GetTransID() const;

		virtual HRESULT CloseTransaction( HRESULT hrRes );


		// flush transaction result
		virtual HRESULT FlushTransaction();

		virtual void Release() override;
	};
	
	
	template< class TransactionType >
	class SubTransactionWithResultMemoryPooled : public SubTransaction, public TransactionResult, public MemoryPoolObject<TransactionType>
	{
	public:
		SubTransactionWithResultMemoryPooled( TransactionID parentTransID , Message::MessageID MsgID )
			:SubTransactionWithResult( parentTransID, MsgID )
		{}

		virtual void Release() { delete this; }
	};
	



	typedef std::function<HRESULT(TransactionResult* pRes)> TransactionMessageHandlerType;

	// Message transaction template
	template< class OwnerType, class MemoryPoolClass, size_t MessageHandlerBufferSize = sizeof(TransactionMessageHandlerType)*2 >
	class TransactionT : public Transaction, public MemoryPoolObject<MemoryPoolClass>
	{
	public:
		typedef TransactionMessageHandlerType MessageHandlerType;

	private:

		// static allocation buffer
		StaticAllocator<MessageHandlerBufferSize>	m_StaticAllocator;

		// Message handler table
		MessageHandlerTable<TransactionMessageHandlerType>		m_Handlers;

	public:
		TransactionT( TransactionID transID )
			:Transaction( transID )
			,m_StaticAllocator(STDAllocator::GetInstance())
			,m_Handlers(m_StaticAllocator)
		{
			BR_TRANS_MESSAGE(Message::Server::GenericFailureRes, { return OnGenericError(pRes); });
		}

		virtual ~TransactionT()
		{
		}

		HRESULT OnGenericError(Svr::TransactionResult* &pRes)
		{
			HRESULT hr = S_OK;

			CloseTransaction(pRes->GetHRESULT());

			return hr;
		}


		// Register message handler
		template< class MessageClassType >
		FORCEINLINE HRESULT RegisterMessageHandler( MessageHandlerType newHandler )
		{
			return m_Handlers.Register<MessageClassType>( newHandler );
		}

		// Caller handler 
		virtual HRESULT ProcessTransaction( TransactionResult* &pRes )
		{
			HRESULT hr = S_OK;

			svrChk(m_Handlers.HandleMessage(pRes->GetMsgID(), pRes));

		Proc_End:

			if( FAILED(hr) )
			{
				if( hr == E_SVR_NO_RESULT_HANDLER )
				{
					svrTrace( Trace::TRC_ERROR, "Transaction has no result handler : Result MessageID:%0%, %1%", pRes->GetMsgID(), typeid(*this).name() );
				}
				else
				{
					svrTrace( Trace::TRC_ERROR, "Transaction result process failed: Result MessageID:%0%, %1%", pRes->GetMsgID(), typeid(*this).name() );
				}
			}

			Util::SafeRelease( pRes );

			return hr;
		}

		// Message processor. When get result, just check result and close
		HRESULT OnMessageClose( Svr::TransactionResult* &pRes )
		{
			HRESULT hr = S_OK;

			svrChk(pRes->GetHRESULT());

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
	template< class OwnerType, class PolicyClass, class MessageClass, class MemoryPoolClass, size_t MessageHandlerBufferSize = sizeof(TransactionMessageHandlerType)*2 >
	class MessageTransaction : public TransactionT<OwnerType,MemoryPoolClass,MessageHandlerBufferSize>, public MessageClass
	{
	protected:
		// Policy
		SharedPointerT<Net::Connection> m_pConn;

	public:
		MessageTransaction( Message::MessageData* &pIMsg )
			: TransactionT<OwnerType, MemoryPoolClass, MessageHandlerBufferSize>( TransactionID() )
			, MessageClass( pIMsg )
		{
		}

		HRESULT ParseMessage()
		{
			HRESULT hr = MessageClass::ParseMsg();
			if (SUCCEEDED(hr))
			{
				if (MessageClass::GetMessage()->GetMessageHeader()->msgID.IDs.Type == Message::MSGTYPE_COMMAND)
				{
					if(MessageClass::HasContext)
					{
						TransactionT<OwnerType, MemoryPoolClass, MessageHandlerBufferSize>::SetParentTransID(MessageClass::GetContext());
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
		virtual HRESULT InitializeTransaction( Entity *pOwner )
		{
			HRESULT hr = S_OK;
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
		
		virtual PolicyClass* GetPolicy()
		{
			if (TransactionT<OwnerType, MemoryPoolClass, MessageHandlerBufferSize>::GetOwnerEntity() == nullptr)
				return nullptr;

			TransactionT<OwnerType, MemoryPoolClass, MessageHandlerBufferSize>::GetMyOwner()->GetConnectionShared(m_pConn);

			if (m_pConn != nullptr)
			{
				return m_pConn->GetPolicy<PolicyClass>();
			}
			return nullptr;
		}

		template< class PolicyType >
		PolicyType* GetPolicy()
		{
			if (TransactionT<OwnerType, MemoryPoolClass, MessageHandlerBufferSize>::GetOwnerEntity() == nullptr)
				return nullptr;

			TransactionT<OwnerType, MemoryPoolClass, MessageHandlerBufferSize>::GetMyOwner()->GetConnectionShared(m_pConn);

			if (m_pConn != nullptr)
			{
				return m_pConn->GetPolicy<PolicyType>();
			}
			return nullptr;
		}
	};


	// User event routing transaction, result policy will be routed to user
	template< class OwnerEntityType, class PolicyType, class MessageClass, class TransactionType, size_t MessageHandlerBufferSize = sizeof(TransactionMessageHandlerType)*2 >
	class UserTransactionS2SEvt : public MessageTransaction<OwnerEntityType,PolicyType, MessageClass, TransactionType, MessageHandlerBufferSize>
	{
	protected:
		UserTransactionS2SEvt( Message::MessageData* &pIMsg )
			:MessageTransaction<OwnerEntityType, PolicyType, MessageClass, TransactionType, MessageHandlerBufferSize>(pIMsg )
		{
		}

		// Initialize Transaction
		virtual HRESULT InitializeTransaction( Entity *pOwner )
		{
			HRESULT hr = S_OK;

			OwnerEntityType *pOwnerEntity = nullptr;
			SharedPointerT<Entity> pFound;

			// We ignore input owner
			pOwner = nullptr;

			hr = MessageTransaction<OwnerEntityType, PolicyType, MessageClass, TransactionType, MessageHandlerBufferSize>::ParseMessage();
			svrChk(hr);

			if(MessageClass::GetRouteContext().GetTo().GetServerID() != ::BR::Svr::GetMyServerID() )
			{
				svrTrace( Trace::TRC_ERROR, "Invalid ServerID %0% MsgID:%0%", typeid(*this).name(), MessageClass::GetMessage()->GetMessageHeader()->msgID );
				svrErr(E_SVR_INVALID_SERVERID);
			}

			svrChk(FindEntity(MessageClass::GetRouteContext().GetTo().GetEntityID(), pFound));

			svrChkPtr(pOwnerEntity = static_cast<OwnerEntityType*>((Entity*)pFound));

			//if( pOwnerEntity->GetConnection() != nullptr )
			//{
			//	svrChkPtr( pOwnerEntity->GetConnection()->GetPolicy<PolicyType>() );
			//}


			hr = MessageTransaction<OwnerEntityType, PolicyType, MessageClass, TransactionType, MessageHandlerBufferSize>::InitializeTransaction(pOwnerEntity);
			svrChk(hr);

		Proc_End:

			return hr;
		}
	};



	// User S2S command routing transaction, result policy will be routed to server
	template< class OwnerEntityType, class PolicyType, class MessageClass, class TransactionType, size_t MessageHandlerBufferSize = sizeof(TransactionMessageHandlerType)*2 >
	class UserTransactionS2SCmd : public MessageTransaction<OwnerEntityType,PolicyType, MessageClass, TransactionType, MessageHandlerBufferSize>
	{
	private:

		ServerEntity* m_ServerEntity;
	protected:
		UserTransactionS2SCmd( Message::MessageData* &pIMsg )
			: MessageTransaction<OwnerEntityType, PolicyType, MessageClass, TransactionType, MessageHandlerBufferSize>(pIMsg )
			, m_ServerEntity(nullptr)
		{
		}

		ServerEntity* GetServerEntity() { return m_ServerEntity; }

		// Initialize Transaction
		virtual HRESULT InitializeTransaction( Entity *pOwner )
		{
			HRESULT hr = S_OK;
			SharedPointerT<Entity> entity;

			OwnerEntityType *pOwnerEntity = nullptr;

			hr = MessageTransaction<OwnerEntityType, PolicyType, MessageClass, TransactionType, MessageHandlerBufferSize>::ParseMessage();
			svrChk(hr);

			if (FAILED(FindEntity(MessageClass::GetRouteContext().GetTo().GetEntityID(), entity)))
			{
				// Can't find target player entity, maybe logged out?
				hr = E_SVR_INVALID_ENTITYUID;
				goto Proc_End;
			}

			svrChkPtr(pOwnerEntity = static_cast<OwnerEntityType*>((Entity*)entity));

			// S2S Communication so return policy owner is server entity
			svrChkPtr( m_ServerEntity = BR_DYNAMIC_CAST(ServerEntity*, pOwner) );

			svrChk( Transaction::InitializeTransaction( pOwnerEntity ) );

		Proc_End:

			return hr;
		}

		virtual PolicyType* GetPolicy() override
		{
			auto pConn = Transaction::GetServerEntityConnection(m_ServerEntity);
			if (pConn != nullptr)
			{
				return pConn->template GetPolicy<PolicyType>();
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

}; // namespace BR





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Transaction helper utility
//


#include "TransactionUtil.h"
