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


#include "Common/Typedefs.h"
#include "Common/TimeUtil.h"
#include "Common/SystemSynchronization.h"


namespace BR
{

	typedef std::thread::id ThreadID;
	typedef std::thread::native_handle_type			ThreadHandle;

	//////////////////////////////////////////////////////////////////////////////////
	//
	//	Basic multithread class
	//
	class Thread : public std::thread
	{
	public:
		enum class PRIORITY
		{
			TIME_CRITICAL,		// Indicates 3 points above normal priority. 
			HIGHEST,			// Indicates 2 points above normal priority. 
			ABOVE_NORMAL,		// Indicates 1 point above normal priority. 
			NORMAL,			// Indicates normal priority. 
			BELOW_NORMAL,		// Indicates 1 point below normal priority. 
			LOWEST,			// Indicates 2 points below normal priority. 
			IDLE,				// Indicates 4 points below normal priority. 
		};


	private:
		// working thread priority
		PRIORITY m_threadPriority;

		// Event handles for thread control
		std::atomic<bool>	m_KillFlag;

		// thread running status
		std::atomic<bool> m_IsRunning;

		// Calculate sleep interval
		TimeStampMS	m_ulPreTime;

		// thread function for run
		static void ThreadFunc(Thread* pThread);

	protected:


	public:
		Thread();
		virtual ~Thread();

		// Get thread
		inline ThreadHandle GetThread();
		inline ThreadID GetThreadID();

#if WINDOWS
		intptr_t GetNativeThreadID() { return  GetThreadId(GetThread()); }
#elif ANDROID
		intptr_t GetNativeThreadID() { return  gettid(); }
#else
		intptr_t GetNativeThreadID() { return  syscall(SYS_gettid); }
#endif

		bool GetIsRunning() { return m_IsRunning.load(std::memory_order_relaxed); }

		// Get/Set Thread Priority
		virtual void SetPriority(PRIORITY priority);
		inline PRIORITY GetPriority();

		//void SetAffinity(PRIORITY priority);

		// Get end event handle
		bool GetKillEvent();

		void SetKillEvent();

		// Calculate sleep interval
		DurationMS UpdateInterval(const DurationMS& ulSleepInterval);

		// Check kill event for give wait time
		bool CheckKillEvent(const DurationMS& dwWaitTime);


		// thread Controlling
		virtual void Start();
		virtual void Stop(bool bSendKillEvt = false);


		// run process, if return value is false then thread will stop
		virtual void Run() = 0;
	};


	// This thread helper
	class ThisThread
	{
	public:
		//static void Yield()									{ std::this_thread::yield(); }
		static std::thread::id GetThreadID()				{ return std::this_thread::get_id(); }
		static void SleepFor(DurationMS duration)			{ std::this_thread::sleep_for(duration); }
	};


	class FunctorThread : public Thread
	{
	private:
		std::function<void(Thread* pThread)> m_Func;

	public:

		FunctorThread(std::function<void(Thread* pThread)> func)
			: m_Func(func)
		{}

		virtual void Run() override
		{
			m_Func((Thread*)this);
		}
	};


#include "Thread.inl"


} // namespace BR

