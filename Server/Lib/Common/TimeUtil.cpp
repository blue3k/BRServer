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


#include "stdafx.h"
#include "Common/TimeUtil.h"
#include "Common/Trace.h"
#include <Mmsystem.h>


#pragma comment(lib, "Winmm.lib")


namespace BR {
namespace Util {

	Time_Chrono Time;

	class TimerThread : public Thread
	{
	public:

		TimerThread()
		{
			Start();
		}

		virtual bool Run()
		{
			DurationMS expectedTickInterval(10);
			SetPriority(PRIORITY::TIME_CRITICAL);

			while (1)
			{
				// 50ms will be the precision of our timer
				auto loopInterval = UpdateInterval(expectedTickInterval);

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


	Time_Chrono::Time_Chrono()
	{
		auto timeStamp = ClockType::now().time_since_epoch();
		m_ullTimeStamp.store(timeStamp.count(), std::memory_order_relaxed);
		m_ullTimeStampPrevious = timeStamp;
		m_ulTimeStampMs = std::chrono::duration_cast<DurationMS>(timeStamp).count();

		// Calculate shift of UTC
		tm timeStruct;
		memset(&timeStruct, 0, sizeof(timeStruct));
		timeStruct.tm_year = UTC_REFERENCE_YEAR - 1900;
		timeStruct.tm_mday = 1;

		m_ullUTCOffset = (ULONGLONG)timegm(&timeStruct);

		UpdateTimer();
	}

	Time_Chrono::~Time_Chrono()
	{
	}


	/////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// Initialize/Terminate
	//


	// Update Timer
	void Time_Chrono::UpdateTimer()
	{
		m_ullTimeStampPrevious = ClockType::duration(m_ullTimeStamp);

		auto timeStamp = ClockType::now().time_since_epoch();
		m_ullTimeStamp.store(timeStamp.count(), std::memory_order_relaxed);

		m_ulTimeStampMs.store(std::chrono::duration_cast<DurationMS>(timeStamp).count(), std::memory_order_relaxed);

		m_ullTimeStampUTC.store(GetRawUTCSec().time_since_epoch().count(), std::memory_order_relaxed);
	}

	// Get time stamp in MS
	TimeStampMS Time_Chrono::GetTimeMs()
	{
		return TimeStampMS(DurationMS(m_ulTimeStampMs));
	}

	// Get UTC time stamp
	TimeStampSec Time_Chrono::GetTimeUTCSec()
	{
		return TimeStampSec(DurationSec(m_ullTimeStampUTC));
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// Timer query
	//

	// Get time tick in ms
	TimeStampMS Time_Chrono::GetRawTimeMs()
	{
		auto timeStamp = ClockType::now().time_since_epoch();
		return TimeStampMS(std::chrono::duration_cast<DurationMS>(timeStamp));
	}


	// Get current UTC sec
	TimeStampSec Time_Chrono::GetRawUTCSec()
	{
		time_t counter;
		counter = time(&counter);
		return TimeStampSec(DurationSec((ULONGLONG)counter - m_ullUTCOffset));
	}


	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// TimeStampTimer
	//


	const TimeStampMS TimeStampTimer::InvalidTime;


	// set timer
	HRESULT TimeStampTimer::SetTimer(DurationMS TimerDuration )
	{
		TimeStampMS ulNewTime = Time.GetTimeMs() + TimerDuration;

		m_ulTimeToExpire = ulNewTime;
		if( m_ulTimeToExpire.time_since_epoch().count() == 0 ) m_ulTimeToExpire = Time.GetTimeMs();

		return S_OK;
	}


	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// Utility
	//

	DurationMS TimeDurationMin(DurationMS timeMs, DurationMS timeMs2)
	{
		if (timeMs.count() <= 0) return timeMs2;
		if (timeMs2.count() <= 0) return timeMs;

		return (timeMs - timeMs2).count() >= 0 ? timeMs2 : timeMs;
	}


};	// namespace Util
} // namespace BR

