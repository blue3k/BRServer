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





// set Exclusive option
inline void Transaction::SetExclusive( bool bIsExclusive )
{
	m_Flags.IsExclusive = bIsExclusive;
}


// Get Exclusive option
inline bool Transaction::IsExclusive() const
{
	return m_Flags.IsExclusive;
}


// set DeleteByEntity option
inline void Transaction::SetDeleteByEntity( bool bIsDeleteByEntity )
{
	m_Flags.IsDeleteByEntity = bIsDeleteByEntity;
}

// Get Delete by entity
inline bool Transaction::IsDeleteByEntity() const
{
	return m_Flags.IsDeleteByEntity;
}


// Set PrintTrace
inline void Transaction::SetPrintTrace( bool bisTrace )
{
	m_Flags.IsPrintTrace = bisTrace;
}

// Set Direct Process
inline void Transaction::SetDirectProcess( bool bisDirectProcess )
{
	m_Flags.IsDirectProcess = bisDirectProcess;
}

		
// Get Print Trace
inline bool Transaction::IsPrintTrace() const
{
	return m_Flags.IsPrintTrace;
}

// Get Direct process 
inline bool Transaction::IsDirectProcess() const
{
	return m_Flags.IsDirectProcess;
}

// Get parent Transaction ID
inline const TransactionID& Transaction::GetParentTransID() const
{
	return m_parentTransID;
}

inline void Transaction::SetParentTransID(const TransactionID& transID)
{
	m_parentTransID = transID;
}


// Get Owner Entity
inline Entity* Transaction::GetOwnerEntity()
{
	return m_pOwner;
}

// Set transaction ID
inline void Transaction::SetTransID( const TransactionID& transID )
{
	m_transID = transID;
}

// Get transaction ID
inline const TransactionID& Transaction::GetTransID() const
{
	return m_transID;
}

// Get transaction state
inline Transaction::State Transaction::GetState()
{
	return m_state;
}

//// Get heart bit time, ms
//ULONG Transaction::GetHeartBitTime()
//{
//	return m_tHeartBit;
//}

// Update heart bit time, ms
inline TimeStampMS Transaction::UpdateHeartBitTime()
{
	m_tHeartBitTimeout = Util::Time.GetTimeMs() + Const::TRANSACTION_TIMEOUT;
	return m_tHeartBitTimeout;
}

// Check timeout, ms
inline Result Transaction::CheckHeartBitTimeout()
{
	if ((INT)(GetHeartBitTimeout() - Util::Time.GetTimeMs()).count() < 0)
		return ResultCode::SVR_TRANSACTION_TIMEOUT;

	return ResultCode::SUCCESS;
}

inline TimeStampMS Transaction::GetHeartBitTimeout()
{
	return m_tHeartBitTimeout;
}

// Timer
inline void Transaction::SetTimer( DurationMS ms )
{
	m_Timer.SetTimer( ms );
	Assert(m_Timer.IsTimerWorking());
}

inline void Transaction::ClearTimer()
{
	m_Timer.ClearTimer();
}

inline bool Transaction::CheckTimer()
{
	return m_Timer.CheckTimer();
}

inline TimeStampMS Transaction::GetTimerExpireTime()
{
	return m_Timer.GetTimerExpireTime();
}

inline bool Transaction::IsTimerWorking()
{
	return m_Timer.IsTimerWorking();
}

// Get expected result ID
uint Transaction::GetExpectedResultID()
{
	return m_uiExpectedResultID;
}

void Transaction::SetExpectedResultID( uint uiExpectedResult )
{
	m_uiExpectedResultID = uiExpectedResult;
}

void Transaction::SetClosed()
{
	m_state = Transaction::STATE_CLOSED;
}

bool Transaction::IsClosed()
{
	return m_state == Transaction::STATE_CLOSED;
}




///////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Transaction result base class
//




void TransactionResult::SetTransaction( const TransactionID &transID, Message::MessageID msgID )
{
	m_transID = transID;
	m_msgID = msgID;
}

void TransactionResult::SetTransaction( const TransactionID &transID )
{
	m_transID = transID;
}

// Set Result
void TransactionResult::SetResult( Result hrRes )
{
	m_hrRes = hrRes;
}

//uint32_t TransactionResult::GetContext() const
//{
//	return m_transID.EntityID;
//}


// Transaction ID
const TransactionID& TransactionResult::GetTransID() const
{
	return m_transID;
}

// Get Message ID

Message::MessageID TransactionResult::GetMsgID() const
{
	return m_msgID;
}

Result TransactionResult::GetResult() const
{
	return m_hrRes;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Transaction network message result class
//



MessageDataPtr MessageResult::GetMessage()
{
	return *m_pIMsg;
}




///////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	SubTransaction + result
//

// Get transaction ID
const TransactionID& SubTransactionWitResult::GetTransID() const
{
	return Transaction::GetTransID();
}


