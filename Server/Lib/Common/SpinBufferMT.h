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

		enum ITEM_STATE
		{
			STATE_FREE,			// Free State, READ_LOCK->FREE
			STATE_WRITE_LOCK,	// locked state to write
			STATE_WRITE_UNLOCK,	// writing complete state
			STATE_READ_LOCK,	// locked state to read
		};

	public:
		// Block of Buffer
		struct BLOCK
		{
			// Buffer block State 
			std::atomic<ITEM_STATE> BlockMode;

			T Data;

			BLOCK()
			{
				// initializing Buffer_state as Free
				BlockMode = ITEM_STATE::STATE_FREE;
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

		void WaitPendingWork();
	};

	#include "SpinBufferMT.inl"



}; // namespace BR
