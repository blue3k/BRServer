////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : utility
//	
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/StrUtil.h"
#include "Common/Trace.h"
#include "Common/Thread.h"
#include "Common/BrMemory.h"
#include "Common/Memory_Impl.h"
#include "Common/TypeUtility.h"






namespace BR
{


	////////////////////////////////////////////////////////////////////////////////
	//
	//	Processor Heap
	//

	namespace ProcessorHeap
	{
#if WINDOWS

		// Heap handle for allocation
		static HANDLE m_hHeap = nullptr;


		Result Alloc( size_t uiSize, void* &pPtr )
		{
			Result hr = ResultCode::SUCCESS;

			if( m_hHeap == nullptr )
				m_hHeap = GetProcessHeap();

			BR::MemBlockHdr* pMemBlock = (BR::MemBlockHdr*)HeapAlloc( m_hHeap, 0, uiSize + sizeof(BR::MemBlockHdr) );

			TrcAssertRel( pMemBlock );

			if( pMemBlock == nullptr )
				trcErr( ResultCode::OUT_OF_MEMORY );// Out of memory or borken heap

			pMemBlock->uiMagic = BR::MemBlockHdr::MEM_MAGIC;
			pMemBlock->uiSize = uiSize;

			pPtr = (pMemBlock+1);


		Proc_End:

			return hr;
		}

		Result Realloc( size_t uiSize, void* &pPtr )
		{
			Result hr = ResultCode::SUCCESS;

			if( m_hHeap == nullptr )
				m_hHeap = GetProcessHeap();

			if( pPtr == nullptr )
				trcErr( ResultCode::INVALID_ARG );

			BR::MemBlockHdr* pMemBlock = (BR::MemBlockHdr*)pPtr - 1;

			if( pMemBlock->uiMagic != BR::MemBlockHdr::MEM_MAGIC )
			{
				if( pMemBlock->uiMagic == BR::MemBlockHdr::MEM_MAGIC_FREE )
				{
					TrcAssertRel( 0 ); // Double Delete
				}
				TrcAssertRel( 0 ); // Broken memory
			}

			BR::MemBlockHdr* pNewMemBlock = (BR::MemBlockHdr*)HeapReAlloc( m_hHeap, 0, pMemBlock, uiSize + sizeof(BR::MemBlockHdr) );
			if( pNewMemBlock == nullptr )
				trcErr( ResultCode::OUT_OF_MEMORY );// Out of memory or borken heap


			pNewMemBlock->uiMagic = BR::MemBlockHdr::MEM_MAGIC;
			pNewMemBlock->uiSize = uiSize;

			pPtr = (pNewMemBlock+1);


		Proc_End:

			return ResultCode::SUCCESS;
		}

		Result Free( void* pPtr )
		{
			Result hr = ResultCode::SUCCESS;

			if( m_hHeap == nullptr )
				m_hHeap = GetProcessHeap();

			if( pPtr == nullptr )
			{
				goto Proc_End;
				//trcErr( ResultCode::INVALID_ARG );
			}

			BR::MemBlockHdr* pMemBlock = (BR::MemBlockHdr*)pPtr - 1;

			if( pMemBlock->uiMagic != BR::MemBlockHdr::MEM_MAGIC )
			{
				if( pMemBlock->uiMagic == BR::MemBlockHdr::MEM_MAGIC_FREE )
				{
					TrcAssertRel( !"Double Delete" ); // Double Delete
				}
				TrcAssertRel( !"Broken memory" ); // Broken memory
			}

			if( !HeapFree( m_hHeap, 0, pMemBlock ) )
				return GetLastResult();// Out of memory or borken heap

		Proc_End:

			return ResultCode::SUCCESS;
		}

#else


	Result Alloc(size_t uiSize, void* &pPtr)
	{
		return StdHeap::Alloc(uiSize, pPtr);
	}

	Result Realloc(size_t uiSize, void* &pPtr)
	{
		return StdHeap::Realloc(uiSize, pPtr);
	}

	Result Free(void* pPtr)
	{
		return StdHeap::Free(pPtr);
	}

#endif

	};




	////////////////////////////////////////////////////////////////////////////////
	//
	//	Std Heap
	//

	namespace StdHeap
	{

		Result Alloc( size_t uiSize, void* &pPtr )
		{
			Result hr = ResultCode::SUCCESS;

			BR::MemBlockHdr* pMemBlock = (BR::MemBlockHdr*)malloc( uiSize + sizeof(BR::MemBlockHdr) );

			TrcAssertRel( pMemBlock );

			if( pMemBlock == nullptr )
				trcErr( ResultCode::OUT_OF_MEMORY );// Out of memory or borken heap

			pMemBlock->uiMagic = BR::MemBlockHdr::MEM_MAGIC;
			pMemBlock->uiSize = uiSize;

			pPtr = (pMemBlock+1);


		Proc_End:

			return hr;
		}

		Result Realloc( size_t uiSize, void* &pPtr )
		{
			Result hr = ResultCode::SUCCESS;
			BR::MemBlockHdr* pNewMemBlock = nullptr;
			BR::MemBlockHdr* pMemBlock = nullptr;

			if( pPtr == nullptr )
				trcErr( ResultCode::INVALID_ARG );

			pMemBlock = (BR::MemBlockHdr*)pPtr - 1;

			if( pMemBlock->uiMagic != BR::MemBlockHdr::MEM_MAGIC )
			{
				if( pMemBlock->uiMagic == BR::MemBlockHdr::MEM_MAGIC_FREE )
				{
					TrcAssertRel( 0 ); // Double Delete
				}
				TrcAssertRel( 0 ); // Broken memory
			}

			pNewMemBlock = (BR::MemBlockHdr*)realloc( pMemBlock, uiSize + sizeof(BR::MemBlockHdr) );
			if( pNewMemBlock == nullptr )
				trcErr( ResultCode::OUT_OF_MEMORY );// Out of memory or borken heap


			pNewMemBlock->uiMagic = BR::MemBlockHdr::MEM_MAGIC;
			pNewMemBlock->uiSize = uiSize;

			pPtr = (pNewMemBlock+1);


		Proc_End:

			return ResultCode::SUCCESS;
		}

		Result Free( void* pPtr )
		{
			Result hr = ResultCode::SUCCESS;
			BR::MemBlockHdr* pMemBlock = nullptr;

			if( pPtr == nullptr )
			{
				goto Proc_End;
				//trcErr( ResultCode::INVALID_ARG );
			}

			pMemBlock = (BR::MemBlockHdr*)pPtr - 1;

			if( pMemBlock->uiMagic != BR::MemBlockHdr::MEM_MAGIC )
			{
				if( pMemBlock->uiMagic == BR::MemBlockHdr::MEM_MAGIC_FREE )
				{
					TrcAssertRel( 0 ); // Double Delete
				}
				TrcAssertRel( 0 ); // Broken memory
			}

			pMemBlock->uiMagic = 0;

			free( pMemBlock );

		Proc_End:

			return ResultCode::SUCCESS;
		}
	}; // namespace StdHeap

	
	////////////////////////////////////////////////////////////////////////////////
	//
	//	Std heap allocator interface
	//

	// Default allocator
	STDAllocator STDAllocator::Instance;

	// Allocate 
	Result STDAllocator::Alloc( size_t uiSize, void* &pPtr )
	{
		return StdHeap::Alloc( uiSize, pPtr );
	}

	// Reallocate
	Result STDAllocator::Realloc( size_t uiSize, void* &pPtr )
	{
		return StdHeap::Realloc( uiSize, pPtr );
	}

	// Free
	Result STDAllocator::Free( void* pPtr )
	{
		return StdHeap::Free( pPtr );
	}


	////////////////////////////////////////////////////////////////////////////////
	//
	//	Processor heap allocator interface
	//


	// Default allocator
	ProcessorAllocator ProcessorAllocator::Instance;

	// Allocate 
	Result ProcessorAllocator::Alloc( size_t uiSize, void* &pPtr )
	{
		return StdHeap::Alloc( uiSize, pPtr );
	}

	// Reallocate
	Result ProcessorAllocator::Realloc( size_t uiSize, void* &pPtr )
	{
		return StdHeap::Realloc( uiSize, pPtr );
	}

	// Free
	Result ProcessorAllocator::Free( void* pPtr )
	{
		return StdHeap::Free( pPtr );
	}

	
};	// namespace BR





////////////////////////////////////////////////////////////////////////////////
//
//	Overriding of 4 std memory operator
//

void* operator new( size_t size )
{
	void *pPtr = nullptr;

	BR::StdHeap::Alloc( size, pPtr );

	AssertRel( ((intptr_t)pPtr & (BR_ALIGN_DOUBLE-1)) == 0 );

	return pPtr;
}

void operator delete( void* pBuff ) noexcept
{
	BR::StdHeap::Free( pBuff );
}


void* operator new[]( size_t size )
{
	void *pPtr = nullptr;

	BR::StdHeap::Alloc( size, pPtr );

	AssertRel( ((intptr_t)pPtr & (BR_ALIGN_DOUBLE-1)) == 0 );

	return pPtr;
}

void operator delete[]( void* pBuff ) noexcept
{
	BR::StdHeap::Free( pBuff );
}

