////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Thread utilities
//
////////////////////////////////////////////////////////////////////////////////


#pragma once


#include "Common/BrAssert.h"


namespace BR
{
	typedef UINT ThreadID;

	//////////////////////////////////////////////////////////////////////////////////
	//
	//	Basic multithread class
	//
	class Thread
	{
	public:
		typedef enum tag_PRIORITY
		{
			PRIORITY_TIME_CRITICAL,		// Indicates 3 points above normal priority. 
			PRIORITY_HIGHEST,			// Indicates 2 points above normal priority. 
			PRIORITY_ABOVE_NORMAL,		// Indicates 1 point above normal priority. 
			PRIORITY_NORMAL,			// Indicates normal priority. 
			PRIORITY_BELOW_NORMAL,		// Indicates 1 point below normal priority. 
			PRIORITY_LOWEST,			// Indicates 2 points below normal priority. 
			PRIORITY_IDLE,				// Indicates 4 points below normal priority. 
		} PRIORITY;


	private:
		// Thread handle
		volatile uintptr_t m_uiThread;

		// Thread ID
		ThreadID m_uiThreadID;

		// working thread priority
		PRIORITY m_threadPriority;

		// Event handles for thread control
		HANDLE	m_hKillEvent;

		// Calculate sleep interval
		ULONG	m_ulPreTime;

		// thread function for run
		static unsigned int __stdcall ThreadFunc( void* arg );

	protected:


	public:
		Thread();
		virtual ~Thread();

		// Get thread
		inline uintptr_t GetThread();
		inline ThreadID GetThreadID();

		// Get/Set Thread Priority
		inline virtual void SetPriority( PRIORITY priority );
		inline PRIORITY GetPriority();

		// Get end event handle
		inline HANDLE GetKillEvent();


		// Calculate sleep interval
		ULONG UpdateInterval( ULONG ulSleepInterval );

		// Check kill event for give wait time
		bool CheckKillEvent( DWORD dwWaitTime );


		// thread Controlling
		virtual void Start();
		virtual void Stop( bool bSendKillEvt = false );
		virtual void Resume();
		virtual void Pause();

		// run process, if return value is false then thread will stop
		virtual bool OnStart() {  return true;}
		virtual bool Run() = 0;
		virtual bool OnEnd() {return true;}
	};


	//////////////////////////////////////////////////////////////////////////////////
	//
	//	Functor execution thread
	//

	class FunctorThread : public Thread
	{
	public: 

	private:
		std::function<void(Thread *pThread)> m_Functor;

	public:
		FunctorThread(std::function<void(Thread *pThread)> Functor)
			:m_Functor(Functor)
		{
		}
		virtual bool Run()
		{
			m_Functor(this);
			return true;
		}
	};







#include "Thread.inl"


}; // namespace BR

