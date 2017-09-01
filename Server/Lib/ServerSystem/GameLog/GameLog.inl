////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : 
//	
//
////////////////////////////////////////////////////////////////////////////////



template<size_t MaxBufferSize, size_t MaxLog>
GameLog<MaxBufferSize,MaxLog>::GameLog()
	:m_LogAllocator(STDAllocator::GetInstance())
{
}

template<size_t MaxBufferSize, size_t MaxLog>
GameLog<MaxBufferSize,MaxLog>::~GameLog()
{
}
	
// Initialize 
template<size_t MaxBufferSize, size_t MaxLog>
Result GameLog<MaxBufferSize,MaxLog>::InitializeLog()
{
	TerminateLog();

	return ResultCode::SUCCESS;
}

// Terminate
template<size_t MaxBufferSize, size_t MaxLog>
void GameLog<MaxBufferSize,MaxLog>::TerminateLog()
{
	GameLogItem *pItem = nullptr;

	while( m_gameLogQueue.GetSize() > 0 && (m_gameLogQueue.Dequeue( pItem )) )
	{
		m_LogAllocator.Free( pItem );
	}
	Assert( m_gameLogQueue.GetSize() == 0 );
	m_gameLogQueue.ClearQueue();
	Assert( m_LogAllocator.GetFreeMemorySize() == MaxBufferSize );
}

template<size_t MaxBufferSize, size_t MaxLog>
Result GameLog<MaxBufferSize,MaxLog>::AddLogItem( GameLogItem* logItem )
{
	Result hr = ResultCode::SUCCESS;

	// If full, remove old one
	if( m_gameLogQueue.GetSize() == MaxLog )
	{
		GameLogItem *pOldItem = nullptr;
		svrChk( m_gameLogQueue.Dequeue( pOldItem ) );
	}

	svrChk( m_gameLogQueue.Enqueue(std::move(logItem)) );
	logItem = nullptr;

Proc_End:

	if( logItem != nullptr )
		m_LogAllocator.Free(logItem);

	return hr;
}

// Get log binary
template<size_t MaxBufferSize, size_t MaxLog>
Result GameLog<MaxBufferSize,MaxLog>::GetGameLogBinary( OutputMemoryStream& stream, UINT startIndex, UINT count )
{
	Result hr = ResultCode::SUCCESS;

	m_gameLogQueue.ReverseForeach( startIndex, count, [&]( GameLogItem* pLogItem ) 
	{
		return (stream.Write( pLogItem->LogItemSize, (uint8_t*)pLogItem ));
	});


//Proc_End:


	return hr;
}



