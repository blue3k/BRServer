////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves 
// 
// Author : KyungKun Ko
//
// Description : Net base type definitions. 
//	
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Net/NetSystem.h"
#include "Net/NetSystem_impl.h"
#include "Common/ResultCode/BRResultCodeNet.h"
#include "Net/NetTrace.h"
#include "Common/TimeUtil.h"
#include "Common/Utility.h"
#include "Common/SpinSharedBuffer.h"
#include "Common/MemoryPool.h"



namespace BR {
namespace Net {



	////////////////////////////////////////////////////////////////////////////////
	//
	// NetIO interface
	//


	Result INetIOCallBack::ProcessSendQueue()
	{
		Result hr = ResultCode::SUCCESS;
		IOBUFFER_WRITE* pSendBuffer = nullptr;

		auto writeQueue = m_pWriteQueues;
		netChkPtr(writeQueue);

		while (1)
		{
			hr = writeQueue->GetFront(pSendBuffer);
			if (!(hr))
			{
				hr = ResultCode::SUCCESS;
				break;
			}

			hr = SendBuffer(pSendBuffer);
			switch ((int32_t)hr)
			{
			case ResultCode::SUCCESS:
				writeQueue->Dequeue(pSendBuffer);
				break;
			case ResultCode::E_NET_IO_PENDING:
				break;
			case ResultCode::E_NET_WOULDBLOCK:  // WOULDBLOCK in linux can be try again
			case ResultCode::E_NET_TRY_AGAIN:
			default:
				goto Proc_End;
				break;
			}
		}

	Proc_End:

		return hr;
	}



	Result INetIOCallBack::EnqueueBuffer(IOBUFFER_WRITE *pSendBuffer)
	{
		Result hr = ResultCode::SUCCESS;

		auto writeQueue = m_pWriteQueues;
		netChkPtr(writeQueue);

		netChk(writeQueue->Enqueue(pSendBuffer));

	Proc_End:

		return hr;

	}


	namespace NetSystem
	{
		static UINT g_GatheringSize = 1024;
		static MemoryPool* g_pGatheringBufferPool = nullptr;


		Result AllocBuffer(IOBUFFER_WRITE* &pIOBuffer)
		{
			pIOBuffer = new IOBUFFER_WRITE;

			return pIOBuffer == nullptr ? ResultCode::FAIL : ResultCode::SUCCESS;
		}

		Result FreeBuffer(IOBUFFER_WRITE *pIOBuffer)
		{
			Util::SafeDelete(pIOBuffer);

			return ResultCode::SUCCESS;
		}

		Result SetGatheringBufferSize(UINT bufferSize)
		{
			g_GatheringSize = bufferSize;
			MemoryPoolManager::GetMemoryPoolBySize(g_GatheringSize, g_pGatheringBufferPool);
			if (g_pGatheringBufferPool == nullptr)
				return ResultCode::FAIL;

			return ResultCode::SUCCESS;
		}

		Result AllocGatheringBuffer(BYTE* &pBuffer, UINT& bufferSize)
		{
			if (g_pGatheringBufferPool == nullptr)
				return ResultCode::UNEXPECTED;

			void* pPtr = nullptr;
			bufferSize = g_GatheringSize;
			if(!(g_pGatheringBufferPool->Alloc(pPtr, "AllocGatheringBuffer")))
				return ResultCode::OUT_OF_MEMORY;

			pBuffer = (BYTE*)pPtr;

			return ResultCode::SUCCESS;
		}

		Result FreeGatheringBuffer(BYTE *pBuffer)
		{
			if (g_pGatheringBufferPool == nullptr)
				return ResultCode::FAIL;

			return g_pGatheringBufferPool->Free(pBuffer, "AllocGatheringBuffer");
		}
	}


} // namespace Net
} // namespace BR

