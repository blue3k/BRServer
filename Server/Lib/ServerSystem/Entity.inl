////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Entity class implementation
//	
//
////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Entity base class
//


// Set Entity State
void Entity::SetEntityState( EntityState state )
{
	m_State = state;
}

// Get Entity State
EntityState Entity::GetEntityState()
{
	return m_State;
}


// Get Entity UID
EntityUID Entity::GetEntityUID() const
{
	return m_EntityUID;
}

// Set Entity UID
void Entity::SetEntityUID( EntityUID entityUID )
{
	m_EntityUID = entityUID;
}


// Get Entity ID
EntityID Entity::GetEntityID() const
{
	return EntityID((UINT32)GetTaskID());
}

// Set/Get Entity Create TIme
TimeStampMS Entity::GetEntityCreateTime()
{
	return m_ulCreateTime;
}

void Entity::SetEntityCreateTime(TimeStampMS ulCreateTime)
{
	m_ulCreateTime = ulCreateTime;
}

// Generate new transaction index
ULONG Entity::GenTransIndex()
{
	auto result = m_lTransIdx.fetch_add(1, std::memory_order_relaxed);

	// not return 0
	if (result == 0) result = m_lTransIdx.fetch_add(1, std::memory_order_relaxed);

	return result;
}


// Get transaction queue
PageQueue<Transaction*>& Entity::GetTransactionQueue()
{
	return m_transactionQueue;
}
//
//// Get transaction result queue
//BR::PageQueue<TransactionResult*>& Entity::GetTransactionResQueue()
//{
//	return m_transactionResQueue;
//}
//






