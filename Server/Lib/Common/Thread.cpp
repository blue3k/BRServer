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


#include "stdafx.h"
#include "Common/TimeUtil.h"
#include "Common/Thread.h"




namespace BR
{



	//////////////////////////////////////////////////////////////////////////////////
	//
	//	Basic multithread class
	//

#if WINDOWS

	static const INT ThreadSchedulingTable[] =
	{
		THREAD_PRIORITY_TIME_CRITICAL,		// PRIORITY_TIME_CRITICAL
		THREAD_PRIORITY_HIGHEST,			// PRIORITY_HIGHEST
		THREAD_PRIORITY_ABOVE_NORMAL,		// PRIORITY_ABOVE_NORMAL 
		THREAD_PRIORITY_NORMAL,				// PRIORITY_NORMAL
		THREAD_PRIORITY_BELOW_NORMAL,		// PRIORITY_BELOW_NORMAL
		THREAD_PRIORITY_LOWEST,				// PRIORITY_LOWEST
		THREAD_PRIORITY_IDLE,				// PRIORITY_IDLE
	};

#elif LINUX || ANDROID

	// schedulingPolicies
	//
	//	// Normal threads, No Scheduling decision. priority must be 0
	//	SCHED_OTHER,
	//	SCHED_IDLE,
	//	SCHED_BATCH,

	//	// Priority value from 1 to 99
	//	// Varies by system, use from sched_get_priority_min(2) to sched_get_priority_max(2)
	//	SCHED_FIFO,
	//	SCHED_RR,
	//

	static const struct
	{
		int Policy;
		int Priority;

	} ThreadSchedulingTable[] =
	{
#if ANDROID
		{ SCHED_FIFO,	10 },																				// PRIORITY_TIME_CRITICAL
		{ SCHED_RR,	5, },	// PRIORITY_HIGHEST
		{ SCHED_RR,	0, },																				// PRIORITY_ABOVE_NORMAL 
		{ SCHED_OTHER,	0, },																									// PRIORITY_NORMAL
		{ SCHED_OTHER,	0, },																									// PRIORITY_BELOW_NORMAL
		{ SCHED_OTHER,	0, },																									// PRIORITY_LOWEST
		{ SCHED_OTHER,	0, },																									// PRIORITY_IDLE
#else
		{ SCHED_FIFO,	sched_get_priority_max(SCHED_FIFO) },																				// PRIORITY_TIME_CRITICAL
		{ SCHED_RR,	sched_get_priority_min(SCHED_RR) + ((sched_get_priority_max(SCHED_RR) - sched_get_priority_min(SCHED_RR)) >> 1), },	// PRIORITY_HIGHEST
		{ SCHED_RR,	sched_get_priority_min(SCHED_RR), },																				// PRIORITY_ABOVE_NORMAL 
		{ SCHED_OTHER,	0, },																									// PRIORITY_NORMAL
		{ SCHED_BATCH,	0, },																									// PRIORITY_BELOW_NORMAL
		{ SCHED_BATCH,	0, },																									// PRIORITY_LOWEST
		{ SCHED_IDLE,	0, },																									// PRIORITY_IDLE
#endif
	};

	static_assert(countof(ThreadSchedulingTable) == ((int)Thread::PRIORITY::IDLE + 1), "Invalid Thread scheduling table count");


	class ModuleThread_impl
	{
	public:
		ModuleThread_impl()
		{
#if !ANDROID
			rlimit limit;
			memset(&limit, 0, sizeof limit);
			if (getrlimit(RLIMIT_RTPRIO, &limit) != 0)
			{
				std::cout << "Failed to get rtpio limits:" << errno << std::endl;
				assert(!"Failed to get rtpio limits:");
				return;
			}

			unsigned int rrMax = (unsigned)sched_get_priority_max(SCHED_RR);
			unsigned int fifoMax = (unsigned)sched_get_priority_max(SCHED_FIFO);
			if (rrMax > limit.rlim_cur || rrMax > limit.rlim_max)
			{
				std::cout << "rtpio RR limit: min:" << (int)sched_get_priority_min(SCHED_RR) << " max:" << (int)sched_get_priority_max(SCHED_RR) << std::endl;
				assert(!"Invalid rtpio RR limits:");
			}
			if (fifoMax > limit.rlim_cur || fifoMax > limit.rlim_max)
			{
				std::cout << "rtpio FIFO limit: min:" << (int)sched_get_priority_min(SCHED_FIFO) << " max:" << (int)sched_get_priority_max(SCHED_FIFO) << std::endl;
				assert(!"Invalid rtpio FIFO limits:");
			}
			std::cout << "TestThreadLimits - OK" << std::endl;
#endif
		}
	};
	static ModuleThread_impl CheckThreadLimits;



#endif

	Thread::Thread()
		: m_threadPriority(PRIORITY::NORMAL)
		, m_IsRunning(false)
	{
	}

	Thread::~Thread()
	{
		Stop(true);
	}

	// Get/Set Thread Priority
	void Thread::SetPriority(PRIORITY priority)
	{
		m_threadPriority = priority;
		auto threadHandle = GetThread();
		if (threadHandle == ThreadHandle(0)) return;

#if WINDOWS

		::SetThreadPriority((HANDLE)threadHandle, ThreadSchedulingTable[(int)priority]);

#elif LINUX || ANDROID

		sched_param sch_params;
		sch_params.sched_priority = ThreadSchedulingTable[(int)priority].Priority;
		if (pthread_setschedparam(native_handle(), ThreadSchedulingTable[(int)priority].Policy, &sch_params))
		{
			std::cerr << "Failed to set Thread scheduling : " << errno << std::endl;
			assert(false);
		}

#endif
	}

	// thread function for run
	void Thread::ThreadFunc(Thread* pThread)
	{
		if (pThread == nullptr)
			return;

		pThread->SetPriority(pThread->GetPriority());

		pThread->Run();

		pThread->m_IsRunning.store(false, std::memory_order_release);

		return;
	}




	// Calculate sleep interval from the expected interval
	DurationMS Thread::UpdateInterval(const DurationMS& ulExpectedInterval)
	{
		auto ulCurTime = Util::Time.GetTimeMs();
		if (m_ulPreTime.time_since_epoch().count() == 0)
		{
			m_ulPreTime = ulCurTime;
			return DurationMS(0);
		}

		LONG diffTime = (LONG)(ulCurTime - m_ulPreTime).count();
		DurationMS sleepInterval;

		if (diffTime < 0) diffTime = 0;

		if (diffTime > (LONG)ulExpectedInterval.count())
			sleepInterval = DurationMS(0);
		else if (diffTime < (LONG)ulExpectedInterval.count())
			sleepInterval = DurationMS((LONG)ulExpectedInterval.count() - diffTime);
		else
			sleepInterval = ulExpectedInterval;
		m_ulPreTime = ulCurTime;

		return sleepInterval;
	}


	bool Thread::CheckKillEvent(const DurationMS& waitTime)
	{
		bool bRet = m_KillFlag.load(std::memory_order_acquire);
		if (!bRet)
			ThisThread::SleepFor(waitTime);

		return bRet;
	}

	// thread Controlling

	void Thread::Start()
	{
		m_ulPreTime = Util::Time.GetTimeMs();

		m_KillFlag.store(false, std::memory_order_release);

		m_IsRunning.store(true, std::memory_order_release);

		std::thread localThread(ThreadFunc, this);
		swap(localThread);
	}

	void Thread::Stop(bool bSendKillEvt)
	{
		// if already finished or not started
		if (!joinable()) return;

		if (bSendKillEvt)
		{
			// Set close event
			SetKillEvent();
		}

		join();
	}











}; // namespace BR



