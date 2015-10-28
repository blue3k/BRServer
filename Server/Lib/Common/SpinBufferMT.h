////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : MadK
//
// Description : Spin buffer for Multi thread
//
////////////////////////////////////////////////////////////////////////////////


#pragma once


#include "Common/BrAssert.h"
#include "Common/Synchronization.h"

namespace BR
{


	//////////////////////////////////////////////////////////////////////////////////
	//
	//	Spin Buffer Class (N-by-M)
	//
	


	template <typename T, int SIZE_BUFFER = 8>
	class SpinBufferMT
	{

		enum 
		{
			MAX_COUNTER	= SIZE_BUFFER - 1,
		};

	public:
		// Block of Buffer
		struct BLOCK
		{
			// Buffer block State 
			typedef enum 
			{
				STATE_WRITE_LOCK,	// locked state to write
				STATE_WRITE_UNLOCK,	// writing complete state
				STATE_READ_LOCK,	// locked state to read
				STATE_FREE,			// Free State, READ_LOCK->FREE
			}STATE;

			volatile union {
				LONG64 BlockMode; 
				STATE eBlockMode;
			};

			T Data;

			BLOCK::BLOCK()
			{
				// initializing Buffer_state as Free
				BlockMode = STATE_FREE;
				memset(&Data, 0, sizeof(T));
			}
		};

	private:
		// Spin Buffer
		BLOCK m_SpinBuffer[SIZE_BUFFER];	

		// ticket to the entering write section
		Ticketing	m_writeTicket; 

		// ticket to the entering read section
		Ticketing	m_readTicket;

	public:
		// Constructor / Destructor
		SpinBufferMT();
		~SpinBufferMT();

	public:
	// Public Interface
		// locking on write mode 
		BLOCK* Write_Lock();
		void Write_Unlock(BLOCK* pItem);

		// locking on read mode 
		BLOCK* Read_Lock();
		void Read_Unlock(BLOCK* pItem);

		// writing complet and get ready for reading
		CounterType GetReadableCount();
	};

	#include "SpinBufferMT.inl"



}; // namespace BR
