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
void Transaction::SetExclusive( bool bIsExclusive )
{
	m_bIsExclusive = bIsExclusive;
}


// Get Exclusive option
bool Transaction::IsExclusive() const
{
	return m_bIsExclusive;
}


// set DeleteByEntity option
void Transaction::SetDeleteByEntity( bool bIsDeleteByEntity )
{
	m_bIsDeleteByEntity = bIsDeleteByEntity;
}

// Get Delete by entity
bool Transaction::IsDeleteByEntity() const
{
	return m_bIsDeleteByEntity;
}


// Set PrintTrace
void Transaction::SetPrintTrace( bool bisTrace )
{
	m_bIsPrintTrace = bisTrace;
}

// Set Direct Process
void Transaction::SetDirectProcess( bool bisDirectProcess )
{
	m_bIsDirectProcess = bisDirectProcess;
}

		
// Get Print Trace
bool Transaction::IsPrintTrace() const
{
	return m_bIsPrintTrace;
}

// Get Direct process 
bool Transaction::IsDirectProcess() const
{
	return m_bIsDirectProcess;
}

// Get parent Transaction ID
const TransactionID& Transaction::GetParentTransID() const
{
	return m_parentTransID;
}

void Transaction::SetParentTransID(const TransactionID& transID)
{
	m_parentTransID = transID;
}


// Get Owner Entity
Entity* Transaction::GetOwnerEntity()
{
	return m_pOwner;
}

// Set transaction ID
void Transaction::SetTransID( const TransactionID& transID )
{
	m_transID = transID;
}

// Get transaction ID
const TransactionID& Transaction::GetTransID() const
{
	return m_transID;
}

// Get transaction state
Transaction::State Transaction::GetState()
{
	return m_state;
}

//// Get heart bit time, ms
//ULONG Transaction::GetHeartBitTime()
//{
//	return m_tHeartBit;
//}

// Update heart bit time, ms
ULONG Transaction::UpdateHeartBitTime()
{
	m_tHeartBitTimeout = Util::Time.GetTimeMs() + Const::TRANSACTION_TIMEOUT;
	return m_tHeartBitTimeout;
}

// Check timeout, ms
HRESULT Transaction::CheckHeartBitTimeout()
{
	if ((LONG)(GetHeartBitTimeout() - Util::Time.GetTimeMs()) < 0)
		return E_SVR_TRANSACTION_TIMEOUT;

	return S_OK;
}

ULONG Transaction::GetHeartBitTimeout()
{
	return m_tHeartBitTimeout;
}

// Timer
void Transaction::SetTimer( ULONG ms )
{
	m_Timer.SetTimer( ms );
}

void Transaction::ClearTimer()
{
	m_Timer.ClearTimer();
}

bool Transaction::CheckTimer()
{
	return m_Timer.CheckTimer();
}

inline ULONG Transaction::GetTimerExpireTime()
{
	return m_Timer.GetTimerExpireTime();
}

bool Transaction::IsTimerWorking()
{
	return m_Timer.IsTimerWorking();
}

// Get expected result ID
UINT Transaction::GetExpectedResultID()
{
	return m_uiExpectedResultID;
}

void Transaction::SetExpectedResultID( UINT uiExpectedResult )
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
void TransactionResult::SetResult( HRESULT hrRes )
{
	m_hrRes = hrRes;
}

//UINT32 TransactionResult::GetContext() const
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

HRESULT TransactionResult::GetHRESULT() const
{
	return m_hrRes;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Transaction network message result class
//



Message::MessageData* MessageResult::GetMessage()
{
	return m_pIMsg;
}




///////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	SubTransaction + result
//

// Get transaction ID
const TransactionID& SubTransactionWithResult::GetTransID() const
{
	return Transaction::GetTransID();
}


