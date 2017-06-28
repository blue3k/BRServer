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
#define BR_ALIGN_SHIFT			4
#define BR_ALIGN_DOUBLE			16
#define BR_ALIGN_DOUBLE_SHIFT	4
#else
#define BR_ALIGN				8
#define BR_ALIGN_SHIFT			4
#define BR_ALIGN_DOUBLE			8
#define BR_ALIGN_DOUBLE_SHIFT	4
#endif

#if __GNUC__
#define BR_DECLARE_ALIGN		__attribute__((aligned(BR_ALIGN)))
#define BR_DECLARE_ALIGN_DOUBLE __attribute__((aligned(BR_ALIGN_DOUBLE)))
#else
#define BR_DECLARE_ALIGN		__declspec(align(BR_ALIGN))
#define BR_DECLARE_ALIGN_DOUBLE __declspec(align(BR_ALIGN_DOUBLE))
#endif

#define BR_ALLIGNUP(x,allign)	( (((uintptr_t)(x) + allign-1) & (~(allign-1))) )


namespace BR {

	////////////////////////////////////////////////////////////////////////////////
	//
	//	Memory log system 
	//


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
		Result Alloc( size_t uiSize, void* &pPtr );
		Result Realloc( size_t uiSize, void* &pPtr );
		Result Free( void* pPtr );
	};


	////////////////////////////////////////////////////////////////////////////////
	//
	//	Std Heap
	//

	namespace StdHeap
	{
		Result Alloc( size_t uiSize, void* &pPtr );
		Result Realloc( size_t uiSize, void* &pPtr );
		Result Free( void* pPtr );
	};


	////////////////////////////////////////////////////////////////////////////////
	//
	//	Heap allocator interface
	//

	class MemoryAllocator
	{
	public:
		// Allocate 
		virtual Result Alloc( size_t uiSize, void* &pPtr ) = 0;

		// Reallocate
		virtual Result Realloc( size_t uiSize, void* &pPtr ) = 0;

		// Free
		virtual Result Free( void* pPtr ) = 0;
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

		static STDAllocator& GetInstance();

		// Allocate 
		virtual Result Alloc( size_t uiSize, void* &pPtr );

		// Reallocate
		virtual Result Realloc( size_t uiSize, void* &pPtr );

		// Free
		virtual Result Free( void* pPtr );
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

		static ProcessorAllocator& GetInstance();

		// Allocate 
		virtual Result Alloc( size_t uiSize, void* &pPtr );

		// Reallocate
		virtual Result Realloc( size_t uiSize, void* &pPtr );

		// Free
		virtual Result Free( void* pPtr );
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

		bool GetIsInStaticBuffer( void* pPtr );

		// Allocate 
		virtual Result Alloc( size_t uiSize, void* &pPtr );

		// Reallocate
		virtual Result Realloc( size_t uiSize, void* &pPtr );

		// Free
		virtual Result Free( void* pPtr );
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

		bool GetIsInStaticBuffer( void* pPtr );

		// Get free memory size in static buffer
		size_t GetFreeMemorySize();

		// Validate allocated chunks for debug
		Result ValidateAllocatedChunks();

		// Allocate 
		virtual Result Alloc( size_t uiSize, void* &pPtr );

		// Reallocate
		virtual Result Realloc( size_t uiSize, void* &pPtr );

		// Free
		virtual Result Free( void* pPtr );
	};



#include "BrMemory.inl"



} // namespace BR



