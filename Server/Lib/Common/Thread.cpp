////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Thread library
//	
//
////////////////////////////////////////////////////////////////////////////////


#include "StdAfx.h"
#include "Common/TimeUtil.h"
#include "Common/Thread.h"
#include <process.h>



namespace BR
{


//////////////////////////////////////////////////////////////////////////////////
//
//	Basic multithread class
//


Thread::Thread()
:m_uiThread(0),
m_uiThreadID(0),
m_threadPriority(PRIORITY_NORMAL),
m_ulPreTime(0)
{
	m_hKillEvent = CreateEvent( NULL, FALSE, FALSE, NULL );
}

Thread::~Thread()
{
	Stop( true );

	if( m_hKillEvent )
		CloseHandle( m_hKillEvent );
}

// Get/Set Thread Priority
void Thread::SetPriority(PRIORITY priority)
{
	m_threadPriority = priority;

	static const UINT toWINPriority[] =
	{
		THREAD_PRIORITY_TIME_CRITICAL,		// PRIORITY_TIME_CRITICAL
		THREAD_PRIORITY_HIGHEST,			// PRIORITY_HIGHEST
		THREAD_PRIORITY_ABOVE_NORMAL,		// PRIORITY_ABOVE_NORMAL 
		THREAD_PRIORITY_NORMAL,				// PRIORITY_NORMAL
		THREAD_PRIORITY_BELOW_NORMAL,		// PRIORITY_BELOW_NORMAL
		THREAD_PRIORITY_LOWEST,				// PRIORITY_LOWEST
		THREAD_PRIORITY_IDLE,				// PRIORITY_IDLE
	};

	if (m_uiThread)
		::SetThreadPriority((HANDLE)m_uiThread, toWINPriority[priority]);
}

// thread function for run
unsigned int __stdcall Thread::ThreadFunc( void* arg )
{
	HRESULT hr = S_OK;
	Thread *pThread = (Thread*)arg;

	Assert( pThread != nullptr );
	pThread->m_uiThreadID = GetCurrentThreadId();

	if( !pThread->OnStart() )
	{
		goto Proc_End;
	}

	if( !pThread->Run() )
	{
		goto Proc_End;
	}

	if( !pThread->OnEnd() )
	{
		goto Proc_End;
	}

	pThread->m_uiThread = 0;
	pThread->m_uiThreadID = 0;

Proc_End:


	return 0;
}




// Calculate sleep interval from the expected interval
ULONG Thread::UpdateInterval( ULONG ulExpectedInterval )
{
	ULONG ulCurTime = Util::Time.GetTimeMs();
	if( m_ulPreTime == 0 )
	{
		m_ulPreTime = ulCurTime;
		return 0;
	}

	ULONG diffTime = ulCurTime - m_ulPreTime;
	ULONG sleepInterval = ulExpectedInterval;

	if( diffTime > ulExpectedInterval )
		sleepInterval = 0;
	else if( diffTime < ulExpectedInterval )
		sleepInterval = ulExpectedInterval - diffTime;

	m_ulPreTime = ulCurTime;

	return sleepInterval;
}


bool Thread::CheckKillEvent( DWORD dwWaitTime )
{
	DWORD dwWaitRes = WaitForSingleObject( GetKillEvent(), dwWaitTime );
	return dwWaitRes == WAIT_OBJECT_0;
}

// thread Controlling

void Thread::Start()
{
	m_ulPreTime = 0;

	if( m_uiThread == 0 )
	{
		ResetEvent( m_hKillEvent );

		m_uiThread = _beginthreadex( NULL, 0, ThreadFunc, this, 0, &m_uiThreadID );
		_ASSERTE( m_uiThread != 0 );
		SetThreadPriority( (HANDLE)m_uiThread, GetPriority() );
	}
}

void Thread::Stop( bool bSendKillEvt )
{
	HRESULT hr = S_OK;

	if( m_uiThread )
	{
		if( bSendKillEvt )
		{
			// Set close event
			SetEvent( m_hKillEvent );
		}

		long waitCount = 0;
		while (m_uiThread != 0 && bSendKillEvt )
		{
			DWORD dwRes = WaitForSingleObject((HANDLE)m_uiThread, 10 * 1000);

			if (dwRes == WAIT_TIMEOUT)
			{
				Assert(waitCount<3);
				if (waitCount >= 3)
				{
					hr = E_UNEXPECTED;
					goto Proc_End;
				}

				continue;
			}

			if (dwRes == WAIT_FAILED)
			{
				hr = HRESULT_FROM_WIN32(GetLastError());
				goto Proc_End;
			}
			else if (dwRes != WAIT_OBJECT_0)
			{
				hr = E_UNEXPECTED;
				goto Proc_End;
			}

			std::atomic_thread_fence(std::memory_order_release);

			break;
		}
	}

Proc_End:

	if( !m_uiThread )
	{
		CloseHandle( (HANDLE)m_uiThread );
		m_uiThread = NULL;
		m_uiThreadID = 0;
	}

}

void Thread::Resume()
{
	if( m_uiThread )
	{
		ResumeThread( (HANDLE)m_uiThread );
	}
}

void Thread::Pause()
{
	if( m_uiThread )
	{
		SuspendThread( (HANDLE)m_uiThread );
	}
}






}; // namespace BR



