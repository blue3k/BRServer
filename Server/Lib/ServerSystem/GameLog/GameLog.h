////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : GameLog definition
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "String/SFStrUtil.h"

#include "MemoryManager/SFMemory.h"
#include "MemoryManager/SFCircularHeap.h"
#include "Types/BrGameTypes.h"
#include "GameConst.h"
#include "Container/SFCircularQueue.h"
#include "Stream/SFMemoryStream.h"
#include "Types/BrSvrTypes.h"





namespace SF {
namespace Svr {


	

	//////////////////////////////////////////////////////////////////////////
	//
	//	Game log class
	//

	template<size_t MaxBufferSize, size_t MaxLog>
	class GameLog
	{
	public:

	private:
		// Memory log allocator
		CircularHeapT<MaxBufferSize> m_LogAllocator;

		// Game log queue
		CircularQueue<GameLogItem*,MaxLog> m_gameLogQueue;

	protected:
		// Create new log item
		template< class LogItemType >
		uint8_t* NewLogItemBuffer(size_t szExpectedSize = 0)
		{
			if( szExpectedSize == 0 )
				szExpectedSize = sizeof(LogItemType);

			Assert(szExpectedSize >= sizeof(LogItemType));

			void* pPtr = m_LogAllocator.Alloc(szExpectedSize);
			if(pPtr == nullptr)
				return nullptr;

			return (uint8_t*)pPtr;
		}

		Result AddLogItem( GameLogItem* logItem );

	public:

		GameLog(IHeap& heap);
		virtual ~GameLog();

		// Initialize 
		virtual Result InitializeLog();

		// Terminate
		virtual void TerminateLog();

		// Get log binary
		Result GetGameLogBinary( OutputMemoryStream& stream, uint startIndex, uint count ) const;

	};


#include "GameLog.inl"

}; // namespace SF
}; // namespace Svr





