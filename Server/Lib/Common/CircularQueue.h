////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : MadK
//
// Description : Circular Buffer for multithread
//
////////////////////////////////////////////////////////////////////////////////

#pragma once 

#include "Common/Typedefs.h"

namespace BR {

////////////////////////////////////////////////////////////////////////////////
// Circular Buffer 

template <typename T, int SIZE_BUFFER = 3>
class CircularQueue
{
	typedef enum 
	{
		WRITE = 0,	
		READ,
		FREE,
	}BUFFER_ORDER;

private:
	// Buffer
	T m_Buffer[SIZE_BUFFER];

	// Read position
	INT m_nReadPtr; 

	// Write position
	INT m_nWritePtr;

	// Items in queue
	UINT m_nItemCount; 

public:

	// Constructor/Destructor
	CircularQueue();
	~CircularQueue(); 

	// Full/Empty check
	inline bool IsFull(); 
	inline bool IsEmpty();

	// Enqueue an element
	inline Result Enqueue( T&& item );

	// Dequeue an element
	inline Result Dequeue( T& item );

	// Get item count
	inline UINT	GetSize(); 

	// Clear all items
	inline void ClearQueue();

	// foreach items in queue
	void ReverseForeach( UINT from, UINT Count, std::function<bool(const T& item)> func );
};

#include "CircularQueue.inl"


}

