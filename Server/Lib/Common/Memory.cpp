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
#include "Common/Memory.h"
#include "Common/Memory_Impl.h"
#include "Common/MemLog.h"
#include "Common/TypeUtility.h"


#ifdef _DEBUG
#define MEMORY_LOG
#endif





namespace BR
{
	MemLog::IMemLogger *g_pLogger = nullptr;
	HMODULE g_hLogger = nullptr;
	bool	g_bFirst = true;


	bool InitializeMemLogger( MemLog::Logging log, int logMask )
	{
#ifdef MEMORY_LOG

		HRESULT hr = S_SYSTEM_OK;
		if( g_pLogger == nullptr && g_bFirst )
		{
			g_bFirst = false;

			g_hLogger = LoadLibraryW( L"MemLog.dll" );
			if( g_hLogger == nullptr )
				return false;

			T_QueryMemLogger fpQueryMemLogger = (T_QueryMemLogger)GetProcAddress( g_hLogger, "QueryMemLogger" );
			if( fpQueryMemLogger == nullptr )
			{
				FreeLibrary( g_hLogger );
				g_hLogger = nullptr;
				return false;
			}

			hr = fpQueryMemLogger( g_pLogger );
			if( FAILED( hr ) )
			{
				FreeLibrary( g_hLogger );
				g_hLogger = nullptr;
				return false;
			}
		}

		g_pLogger->Initialize( log, logMask );
		return true;

#endif

		return false;
	}

	MemLog::IMemLogger* GetMemLogger()
	{
		return g_pLogger;
	}


	

	ScopedMemLog::ScopedMemLog( UINT32 uiNewMask )
	{
		if( g_pLogger )
		{
			m_uiLogMaskOrg = BR::GetMemLogger()->GetLogMask();
			BR::GetMemLogger()->SetLogMask( uiNewMask );
			BR::GetMemLogger()->StartDifferenceCheck();
		}
	}

	ScopedMemLog::~ScopedMemLog()
	{
		if( g_pLogger )
		{
			BR::GetMemLogger()->ReportDifference();
			BR::GetMemLogger()->StopDifferenceCheck();
			BR::GetMemLogger()->SetLogMask( m_uiLogMaskOrg );
		}
	}




	////////////////////////////////////////////////////////////////////////////////
	//
	//	Processor Heap
	//

	namespace ProcessorHeap
	{
#if WINDOWS

		// Heap handle for allocation
		static HANDLE m_hHeap = nullptr;


		HRESULT Alloc( size_t uiSize, void* &pPtr )
		{
			HRESULT hr = S_SYSTEM_OK;

			if( m_hHeap == nullptr )
				m_hHeap = GetProcessHeap();

			BR::MemBlockHdr* pMemBlock = (BR::MemBlockHdr*)HeapAlloc( m_hHeap, 0, uiSize + sizeof(BR::MemBlockHdr) );

			TrcAssertRel( pMemBlock );

			if( pMemBlock == nullptr )
				trcErr( E_SYSTEM_OUTOFMEMORY );// Out of memory or borken heap

			pMemBlock->uiMagic = BR::MemBlockHdr::MEM_MAGIC;
			pMemBlock->uiSize = uiSize;

			pPtr = (pMemBlock+1);

			if( BR::g_pLogger ) BR::g_pLogger->AddToLog( 2, pMemBlock, uiSize+sizeof(BR::MemBlockHdr) );

		Proc_End:

			return hr;
		}

		HRESULT Realloc( size_t uiSize, void* &pPtr )
		{
			HRESULT hr = S_SYSTEM_OK;

			if( m_hHeap == nullptr )
				m_hHeap = GetProcessHeap();

			if( pPtr == nullptr )
				trcErr( E_SYSTEM_INVALIDARG );

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
				trcErr( E_SYSTEM_OUTOFMEMORY );// Out of memory or borken heap


			pNewMemBlock->uiMagic = BR::MemBlockHdr::MEM_MAGIC;
			pNewMemBlock->uiSize = uiSize;

			pPtr = (pNewMemBlock+1);

			if( BR::g_pLogger )
			{
				BR::g_pLogger->RemoveFromLog( pMemBlock );
				BR::g_pLogger->AddToLog( 2, pNewMemBlock, uiSize + sizeof(BR::MemBlockHdr) );
			}

		Proc_End:

			return S_SYSTEM_OK;
		}

		HRESULT Free( void* pPtr )
		{
			HRESULT hr = S_SYSTEM_OK;

			if( m_hHeap == nullptr )
				m_hHeap = GetProcessHeap();

			if( pPtr == nullptr )
			{
				goto Proc_End;
				//trcErr( E_SYSTEM_INVALIDARG );
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
				return GetLastHRESULT();// Out of memory or borken heap

			if( BR::g_pLogger ) BR::g_pLogger->RemoveFromLog( pMemBlock );

		Proc_End:

			return S_SYSTEM_OK;
		}

#else


	HRESULT Alloc(size_t uiSize, void* &pPtr)
	{
		return StdHeap::Alloc(uiSize, pPtr);
	}

	HRESULT Realloc(size_t uiSize, void* &pPtr)
	{
		return StdHeap::Realloc(uiSize, pPtr);
	}

	HRESULT Free(void* pPtr)
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

		HRESULT Alloc( size_t uiSize, void* &pPtr )
		{
			HRESULT hr = S_SYSTEM_OK;

			BR::MemBlockHdr* pMemBlock = (BR::MemBlockHdr*)malloc( uiSize + sizeof(BR::MemBlockHdr) );

			TrcAssertRel( pMemBlock );

			if( pMemBlock == nullptr )
				trcErr( E_SYSTEM_OUTOFMEMORY );// Out of memory or borken heap

			pMemBlock->uiMagic = BR::MemBlockHdr::MEM_MAGIC;
			pMemBlock->uiSize = uiSize;

			pPtr = (pMemBlock+1);

			if( BR::g_pLogger ) BR::g_pLogger->AddToLog( 2, pMemBlock, uiSize + sizeof(BR::MemBlockHdr) );

		Proc_End:

			return hr;
		}

		HRESULT Realloc( size_t uiSize, void* &pPtr )
		{
			HRESULT hr = S_SYSTEM_OK;
			BR::MemBlockHdr* pNewMemBlock = nullptr;
			BR::MemBlockHdr* pMemBlock = nullptr;

			if( pPtr == nullptr )
				trcErr( E_SYSTEM_INVALIDARG );

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
				trcErr( E_SYSTEM_OUTOFMEMORY );// Out of memory or borken heap


			pNewMemBlock->uiMagic = BR::MemBlockHdr::MEM_MAGIC;
			pNewMemBlock->uiSize = uiSize;

			pPtr = (pNewMemBlock+1);

			if( BR::g_pLogger )
			{
				BR::g_pLogger->RemoveFromLog( pMemBlock );
				BR::g_pLogger->AddToLog( 2, pNewMemBlock, uiSize + sizeof(BR::MemBlockHdr) );
			}

		Proc_End:

			return S_SYSTEM_OK;
		}

		HRESULT Free( void* pPtr )
		{
			HRESULT hr = S_SYSTEM_OK;
			BR::MemBlockHdr* pMemBlock = nullptr;

			if( pPtr == nullptr )
			{
				goto Proc_End;
				//trcErr( E_SYSTEM_INVALIDARG );
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

			if( BR::g_pLogger ) BR::g_pLogger->RemoveFromLog( pMemBlock );

		Proc_End:

			return S_SYSTEM_OK;
		}
	}; // namespace StdHeap

	
	////////////////////////////////////////////////////////////////////////////////
	//
	//	Std heap allocator interface
	//

	// Default allocator
	STDAllocator STDAllocator::Instance;

	// Allocate 
	HRESULT STDAllocator::Alloc( size_t uiSize, void* &pPtr )
	{
		return StdHeap::Alloc( uiSize, pPtr );
	}

	// Reallocate
	HRESULT STDAllocator::Realloc( size_t uiSize, void* &pPtr )
	{
		return StdHeap::Realloc( uiSize, pPtr );
	}

	// Free
	HRESULT STDAllocator::Free( void* pPtr )
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
	HRESULT ProcessorAllocator::Alloc( size_t uiSize, void* &pPtr )
	{
		return StdHeap::Alloc( uiSize, pPtr );
	}

	// Reallocate
	HRESULT ProcessorAllocator::Realloc( size_t uiSize, void* &pPtr )
	{
		return StdHeap::Realloc( uiSize, pPtr );
	}

	// Free
	HRESULT ProcessorAllocator::Free( void* pPtr )
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

void operator delete( void* pBuff )
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

void operator delete[]( void* pBuff )
{
	BR::StdHeap::Free( pBuff );
}

