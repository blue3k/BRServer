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
#include "Common/HRESNet.h"
#include "Net/NetTrace.h"
#include "Common/TimeUtil.h"
#include "Common/Utility.h"
#include "Common/SpinSharedBuffer.h"
#include "Common/MemoryPool.h"



namespace BR {
namespace Net {





	namespace NetSystem
	{
		static UINT g_GatheringSize = 1024;
		static MemoryPool* g_pGatheringBufferPool = nullptr;


		HRESULT AllocBuffer(IOBUFFER_WRITE* &pIOBuffer)
		{
			pIOBuffer = new IOBUFFER_WRITE;

			return pIOBuffer == nullptr ? E_FAIL : S_OK;
		}

		HRESULT FreeBuffer(IOBUFFER_WRITE *pIOBuffer)
		{
			Util::SafeDelete(pIOBuffer);

			return S_OK;
		}

		HRESULT SetGatheringBufferSize(UINT bufferSize)
		{
			g_GatheringSize = bufferSize;
			MemoryPoolManager::GetMemoryPoolBySize(g_GatheringSize, g_pGatheringBufferPool);
			if (g_pGatheringBufferPool == nullptr)
				return E_FAIL;

			return S_OK;
		}

		HRESULT AllocGatheringBuffer(BYTE* &pBuffer, UINT& bufferSize)
		{
			if (g_pGatheringBufferPool == nullptr)
				return E_UNEXPECTED;

			void* pPtr = nullptr;
			bufferSize = g_GatheringSize;
			if(FAILED(g_pGatheringBufferPool->Alloc(pPtr, "AllocGatheringBuffer")))
				return E_OUTOFMEMORY;

			pBuffer = (BYTE*)pPtr;

			return S_OK;
		}

		HRESULT FreeGatheringBuffer(BYTE *pBuffer)
		{
			if (g_pGatheringBufferPool == nullptr)
				return E_FAIL;

			return g_pGatheringBufferPool->Free(pBuffer, "AllocGatheringBuffer");
		}
	}


} // namespace Net
} // namespace BR

