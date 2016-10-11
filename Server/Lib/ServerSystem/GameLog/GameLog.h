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

#include "Common/StrUtil.h"
#include "Common/ClassUtil.h"
#include "Common/Memory.h"
#include "Common/BrGameTypes.h"
#include "Common/GameConst.h"
#include "Common/CircularQueue.h"
#include "Common/MemoryStream.h"
#include "ServerSystem/SvrTypes.h"





namespace BR {
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
		CircularBufferAllocator<MaxBufferSize,BR_ALIGN> m_LogAllocator;

		// Game log queue
		CircularQueue<GameLogItem*,MaxLog> m_gameLogQueue;

	protected:
		// Create new log item
		template< class LogItemType >
		BYTE* NewLogItemBuffer(size_t szExpectedSize = 0)
		{
			if( szExpectedSize == 0 )
				szExpectedSize = sizeof(LogItemType);

			Assert(szExpectedSize >= sizeof(LogItemType));

			void* pPtr = nullptr;
			if( !(m_LogAllocator.Alloc( szExpectedSize, pPtr )) )
			{
				return nullptr;
			}

			return (BYTE*)pPtr;
		}

		Result AddLogItem( GameLogItem* logItem );

	public:

		GameLog();
		virtual ~GameLog();

		// Initialize 
		virtual Result InitializeLog();

		// Terminate
		virtual void TerminateLog();

		// Get log binary
		Result GetGameLogBinary( OutputMemoryStream& stream, UINT startIndex, UINT count );

	};


#include "GameLog.inl"

}; // namespace BR
}; // namespace Svr





