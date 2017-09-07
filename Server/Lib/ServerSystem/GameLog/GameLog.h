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

#include "String/StrUtil.h"

#include "Memory/SFMemory.h"
#include "Memory/SFCircularHeap.h"
#include "Types/BrGameTypes.h"
#include "GameConst.h"
#include "Container/CircularQueue.h"
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
		CircularHeap<MaxBufferSize,SF_ALIGN> m_LogAllocator;

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

			void* pPtr = nullptr;
			if( !(m_LogAllocator.Alloc( szExpectedSize, pPtr )) )
			{
				return nullptr;
			}

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





