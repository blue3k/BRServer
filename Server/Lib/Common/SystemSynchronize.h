////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 MadK
// 
// Author : KyungKun Ko
//
// Description : Thread Synchronize
//
////////////////////////////////////////////////////////////////////////////////



#pragma once

#include "Common/Typedefs.h"
#include "Common/BrAssert.h"


namespace BR
{


	/////


	///////////////////////////////////////////////////////////////////////////////
	//
	//	Critical Section
	//

	class MutexBase
	{
	public:
		virtual void Lock() = 0;
		virtual void UnLock() = 0;
	};


	// Scope locker
	class MutexScopeLock
	{
	public:
		MutexScopeLock(MutexBase& lock) : m_Mutex(lock)
		{
			m_Mutex.Lock();
		}

		~MutexScopeLock()
		{
			m_Mutex.UnLock();
		}

	private:
		MutexBase &m_Mutex;
	};



#if defined(_WIN32) || defined(_WIN64)
#include "SystemSynchronize_win.inl"
#else
#include "SystemSynchronize_linux.inl"
#endif


}; // namespace BR

