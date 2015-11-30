////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun, Ko
//
// Description : Spin shared buffer for multithired thread 
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Common/Typedefs.h"
#include "Common/Synchronization.h"
#include "Common/Utility.h"



namespace BR
{


	//////////////////////////////////////////////////////////////////////////////////
	//
	//	Spin Shared Buffer Class
	//

	template <class ItemType>
	class SpinSharedBuffer
	{
		struct Buffer
		{
			// Buffer State 
			typedef enum 
			{
				STATE_FREE,	// Free Buffer State
				STATE_USE,	// Read Buffer State
			} BufferState;


			// Buffer State
			std::atomic<BufferState> State;
			//union {
			//	volatile LONG State; 
			//	BufferState eState; // for easy to see state
			//};

			// Data storage
			ItemType Data;


			// constructor
			Buffer()
			{
				// initializing Buffer state as Free
				State = STATE_FREE; 
			}
		};

	private:
		// Number of buffer to maintain
		UINT m_BufferCount;

		// Spin shared Buffer
		Buffer *m_SpinBuffer;	

		// Spin Index
		SyncCounter m_AccessPosition;

		// Used buffer count
		SyncCounter m_UsedBufferCount;

	public:
		// Constructor / Destructor
		SpinSharedBuffer( UINT BufferCount );
		~SpinSharedBuffer();

		// Set buffer count, all pervious buffer data will be lost
		HRESULT SetBufferCount(  UINT BufferCount  );

		// Get buffer count
		UINT GetBufferCount();

		// Get used buffer count
		CounterType GetUsedBufferCount();

		// Try to get one buffer while given times
		HRESULT TryAllocBuffer( INT iTryCount, ItemType* &pBuffer );

		// Get free buffer
		HRESULT AllocBuffer( ItemType* &pBuffer );

		// Free given buffer
		HRESULT FreeBuffer( ItemType* pBuffer );
	};

	#include "SpinSharedBuffer.inl"



}; // namespace BR
