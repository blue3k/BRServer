////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Time utility 
//	
//
////////////////////////////////////////////////////////////////////////////////


#include "StdAfx.h"
#include "Common/TimeUtil.h"
#include "Common/Trace.h"
#include <Mmsystem.h>


#pragma comment(lib, "Winmm.lib")


namespace BR {
namespace Util {

	////////////////////////////////////////////////////////////////////////////////
	//
	//	Timer
	//

	class TimerThread : public Thread
	{
	public:
		virtual bool Run()
		{
			SetPriority(PRIORITY_TIME_CRITICAL);

			SYSTEM_INFO sysInfo;
			memset( &sysInfo, 0, sizeof(sysInfo) );
			GetSystemInfo( &sysInfo );

			// fix this thread to the second if exists
			UINT_PTR core = sysInfo.dwNumberOfProcessors > 1 ? 1 : 0;
			SetThreadAffinityMask( (HANDLE)GetThread(), 1<<core );

			while( 1 )
			{
				// 50ms will be the precision of our timer
				ULONG loopInterval = UpdateInterval( 10 );

				if (CheckKillEvent(loopInterval))
				{
					// Kill Event signaled
					break;
				}

				Time.UpdateTimer();
			}

			return true;
		}
	};


	TimerThread TimerUpdateThread;

	Time_WIN32 Time;

	Time_WIN32::Time_WIN32()
	{
		QueryPerformanceFrequency(&m_ullPerfFreq);

		LARGE_INTEGER ulTimeStamp;
		QueryPerformanceCounter(&ulTimeStamp);
		m_ullTimeStamp.store(ulTimeStamp, std::memory_order_relaxed);
		//QueryPerformanceCounter( &m_ullTimeStamp );
		m_ullTimeStampPrevious = ulTimeStamp;
		m_ulTimeStampMs = 0;

		m_ullPerfTickMS = m_ullPerfFreq.QuadPart / 1000;

		// Calculate shift of UTC
		tm timeStruct;
		memset(&timeStruct, 0, sizeof(timeStruct));
		timeStruct.tm_year = UTC_REFERENCE_YEAR - 1900;
		timeStruct.tm_mday = 1;
		
		m_ullUTCOffset = (ULONGLONG)_mkgmtime64( &timeStruct );
		//m_ullUTCOffset = 0;

		UpdateTimer();

		//__time64_t now;
		//tm nowStruct;
		//memset(&nowStruct, 0, sizeof(struct tm));

		//now = _time64( &now );
		//_gmtime64_s( &nowStruct, &now );

		//Assert( nowStruct.tm_yday == 144);

	}

	Time_WIN32::~Time_WIN32()
	{
		timeEndPeriod(1);
	}

	
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// Initialize/Terminate
	//

	HRESULT Time_WIN32::InitializeTimer()
	{
		TimerUpdateThread.Start();
		return S_OK;
	}

	HRESULT Time_WIN32::TerminateTimer()
	{
		TimerUpdateThread.Stop();
		return S_OK;
	}

	// Update Timer
	void Time_WIN32::UpdateTimer()
	{
		LARGE_INTEGER ulTimeStamp;
		QueryPerformanceCounter(&ulTimeStamp);
		m_ullTimeStamp.store(ulTimeStamp, std::memory_order_relaxed);

		ULONGLONG diffTime = (ULONGLONG)(ulTimeStamp.QuadPart - m_ullTimeStampPrevious.QuadPart);
		m_ulTimeStampMs += (ULONG)(diffTime / m_ullPerfTickMS);

		m_ullTimeStampPrevious.QuadPart = ulTimeStamp.QuadPart - (diffTime % m_ullPerfTickMS);

		m_ullTimeStampUTC = GetRawUTCSec();
	}

	// Get time stamp in MS
	ULONG Time_WIN32::GetTimeMs()
	{
		return m_ulTimeStampMs;
	}

	// Get UTC time stamp
	ULONGLONG Time_WIN32::GetTimeUTCSec()
	{
		return m_ullTimeStampUTC;
	}
	ULONG Time_WIN32::GetTimeUTCSec32()
	{
		return (ULONG)m_ullTimeStampUTC;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// Timer query
	//

	// Get time tick in ms
	ULONG Time_WIN32::GetRawTimeMs()
	{
		LARGE_INTEGER counter;
		QueryPerformanceCounter( &counter );
		return (ULONG)((ULONGLONG)counter.QuadPart / m_ullPerfTickMS);
	}

	// Get current sec
	ULONG Time_WIN32::GetRawTimeSec()
	{
		LARGE_INTEGER counter;
		QueryPerformanceCounter( &counter );
		return (ULONG)((ULONGLONG)counter.QuadPart / m_ullPerfFreq.QuadPart);
	}


	// Get current UTC sec
	ULONGLONG Time_WIN32::GetRawUTCSec()
	{
		__time64_t counter;
		counter = _time64( &counter );
		return (ULONGLONG)counter - m_ullUTCOffset;
	}


	// set timer
	HRESULT TimeStampTimer::SetTimer( ULONG TimerDuration )
	{
		ULONG ulNewTime = Time.GetTimeMs() + TimerDuration;

		m_ulTimeToExpire = ulNewTime;
		if( m_ulTimeToExpire == 0 ) m_ulTimeToExpire = 1;

		return S_OK;
	}


	LONG TimeMin(ULONG timeMs, ULONG timeMs2)
	{
		if (timeMs == -1) return timeMs2;
		if (timeMs2 == -1) return timeMs;

		return (LONG)(timeMs - timeMs2) >= 0 ? timeMs2 : timeMs;
	}


};	// namespace Util
} // namespace BR

