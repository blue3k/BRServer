////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Memory allocation library
//	
//
////////////////////////////////////////////////////////////////////////////////



#pragma once


#include "Common/Typedefs.h"
#include "Common/BrAssert.h"




#ifdef X64
#define BR_ALIGN				16
#define BR_ALIGN_SHIFT			3
#define BR_ALIGN_DOUBLE			16
#define BR_ALIGN_DOUBLE_SHIFT	4
#else
#define BR_ALIGN				4
#define BR_ALIGN_SHIFT			2
#define BR_ALIGN_DOUBLE			8
#define BR_ALIGN_DOUBLE_SHIFT	3
#endif

#if __GNUC__
__attribute__((aligned(16)))
#define BR_DECLARE_ALIGN		__attribute__((aligned(BR_ALIGN)))
#define BR_DECLARE_ALIGN_DOUBLE __attribute__((aligned(BR_ALIGN_DOUBLE)))
#else
#define BR_DECLARE_ALIGN		__declspec(align(BR_ALIGN))
#define BR_DECLARE_ALIGN_DOUBLE __declspec(align(BR_ALIGN_DOUBLE))
#endif

#define BR_ALLIGNUP(x,allign)	( (((uintptr_t)(x) + allign-1) & (~(allign-1))) )


namespace MemLog
{
	enum Logging;
	class IMemLogger;
}

namespace BR
{

	////////////////////////////////////////////////////////////////////////////////
	//
	//	Memory log system 
	//

	// Initialize memory logger
	bool InitializeMemLogger( MemLog::Logging log, int logMask = 0 );

	// get memory logger
	MemLog::IMemLogger* GetMemLogger();


	// Scoped memory logging 
	class ScopedMemLog
	{
	private:
		UINT32 m_uiLogMaskOrg;

	public:
		ScopedMemLog( UINT32 uiNewMask = 0xFFFFFFFFL );
		~ScopedMemLog();
	};

	#define MEMCHECKSCOPE() BR::ScopedMemLog __MemLogScope;


	////////////////////////////////////////////////////////////////////////////////
	//
	//	Memory block descriptor
	//

#if WINDOWS
	// Check CRT heap memory status
	inline void CheckCtrMemory()
	{
		//Assert( _CrtCheckMemory( ) );
	}

#else
	inline void CheckCtrMemory()
	{
		//Assert(_CrtCheckMemory());
	}
#endif



	////////////////////////////////////////////////////////////////////////////////
	//
	//	Processor Heap
	//

	namespace ProcessorHeap
	{
		HRESULT Alloc( size_t uiSize, void* &pPtr );
		HRESULT Realloc( size_t uiSize, void* &pPtr );
		HRESULT Free( void* pPtr );
	};


	////////////////////////////////////////////////////////////////////////////////
	//
	//	Std Heap
	//

	namespace StdHeap
	{
		HRESULT Alloc( size_t uiSize, void* &pPtr );
		HRESULT Realloc( size_t uiSize, void* &pPtr );
		HRESULT Free( void* pPtr );
	};


	////////////////////////////////////////////////////////////////////////////////
	//
	//	Heap allocator interface
	//

	class MemoryAllocator
	{
	public:
		// Allocate 
		virtual HRESULT Alloc( size_t uiSize, void* &pPtr ) = 0;

		// Reallocate
		virtual HRESULT Realloc( size_t uiSize, void* &pPtr ) = 0;

		// Free
		virtual HRESULT Free( void* pPtr ) = 0;
	};
	

	////////////////////////////////////////////////////////////////////////////////
	//
	//	Std heap allocator interface
	//

	class STDAllocator : public MemoryAllocator
	{
	private:
		// Default allocator
		static STDAllocator Instance;
	public:

		static FORCEINLINE STDAllocator& GetInstance();

		// Allocate 
		virtual HRESULT Alloc( size_t uiSize, void* &pPtr );

		// Reallocate
		virtual HRESULT Realloc( size_t uiSize, void* &pPtr );

		// Free
		virtual HRESULT Free( void* pPtr );
	};


	////////////////////////////////////////////////////////////////////////////////
	//
	//	Processor heap allocator interface
	//

	class ProcessorAllocator : public MemoryAllocator
	{
	private:
		// Default allocator
		static ProcessorAllocator Instance;
	public:

		static FORCEINLINE ProcessorAllocator& GetInstance();

		// Allocate 
		virtual HRESULT Alloc( size_t uiSize, void* &pPtr );

		// Reallocate
		virtual HRESULT Realloc( size_t uiSize, void* &pPtr );

		// Free
		virtual HRESULT Free( void* pPtr );
	};
	

	////////////////////////////////////////////////////////////////////////////////
	//
	//	static allocator interface
	//   // allocate from the buffer first
	//

	template< size_t BufferSize >
	class StaticAllocator : public MemoryAllocator
	{
	private:
		// internal heap
		MemoryAllocator		&m_OverflowHeap;

		// Static buffer
		intptr_t			m_AllocatePosition;
		BYTE				m_AllocationBuffer[BufferSize];

	public:

		StaticAllocator(MemoryAllocator& overflowHeap);
		~StaticAllocator();

		FORCEINLINE bool GetIsInStaticBuffer( void* pPtr );

		// Allocate 
		virtual HRESULT Alloc( size_t uiSize, void* &pPtr );

		// Reallocate
		virtual HRESULT Realloc( size_t uiSize, void* &pPtr );

		// Free
		virtual HRESULT Free( void* pPtr );
	};

	

	////////////////////////////////////////////////////////////////////////////////
	//
	//	Circular buffer allocator interface
	//

	template< size_t BufferSize, size_t alignment >
	class CircularBufferAllocator : public MemoryAllocator
	{
	public:

		enum class ChunkTypes : UINT32
		{
			Free			= 0xc7c7c7c7,
			Dummy,
			Allocated,
		};

#pragma pack(push)
#pragma pack(4)
		struct MemoryChunkHeader
		{
			ChunkTypes	ChunkType;
			UINT32		ChunkSize;
		};
#pragma pack(pop)

		// header size
		static const size_t HeaderSize = BR_ALLIGNUP( sizeof(MemoryChunkHeader), alignment );


		// internal overflow heap
		MemoryAllocator		&m_OverflowHeap;

		intptr_t			m_FreeSize;

		intptr_t			m_AllocatePosition;
		intptr_t			m_FreePosition;

		// Static buffer
		BYTE				m_AllocationBuffer[BufferSize];

	public:

		CircularBufferAllocator(MemoryAllocator& overflowHeap);
		~CircularBufferAllocator();

		FORCEINLINE bool GetIsInStaticBuffer( void* pPtr );

		// Get free memory size in static buffer
		FORCEINLINE size_t GetFreeMemorySize();

		// Validate allocated chunks for debug
		HRESULT ValidateAllocatedChunks();

		// Allocate 
		virtual HRESULT Alloc( size_t uiSize, void* &pPtr );

		// Reallocate
		virtual HRESULT Realloc( size_t uiSize, void* &pPtr );

		// Free
		virtual HRESULT Free( void* pPtr );
	};



#include "Memory.inl"



}; // namespace BR



