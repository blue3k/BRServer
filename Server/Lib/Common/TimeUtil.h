////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Time Utility library
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Common/Typedefs.h"
#include "Common/ClassUtil.h"

namespace BR {
namespace Util {

	enum {
		// UTC timer reference year, This is DB time reference
		UTC_REFERENCE_YEAR = 2014,
	};

	////////////////////////////////////////////////////////////////////////////////
	//
	//	Win32 timer class
	//

	class Time_WIN32
	{
	public:

	private:
		// cached values
		ULONGLONG		m_ullPerfTickMS;
		LARGE_INTEGER	m_ullPerfFreq;

		// Time stamp
		std::atomic<LARGE_INTEGER>	m_ullTimeStamp;
		LARGE_INTEGER	m_ullTimeStampPrevious;
		std::atomic<ULONG>			m_ulTimeStampMs;
		std::atomic<ULONGLONG>		m_ullTimeStampUTC;

		// Default UTC offset
		ULONGLONG		m_ullUTCOffset;

	protected:
		// Update Time stamp
		void		UpdateTimer();

		friend class TimerThread;

	public:
		Time_WIN32();
		~Time_WIN32();



		/////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// Initialize/Terminate
		//

		HRESULT InitializeTimer();
		HRESULT TerminateTimer();

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// Time query
		//

		// Get time stamp in ms
		ULONG		GetTimeMs();

		// Get UTC time stamp
		ULONGLONG	GetTimeUTCSec();
		ULONG		GetTimeUTCSec32();


		/////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// Global Time query
		//

		// Get time tick in ms
		ULONG		GetRawTimeMs();

		// Get current sec
		ULONG		GetRawTimeSec();

		// Get current UTC sec
		ULONGLONG	GetRawUTCSec();

	};


	// Global main timer
	extern Time_WIN32 Time;



	////////////////////////////////////////////////////////////////////////////////
	//
	//	Timer
	//

	class TimeStampTimer
	{
	public:

	private:
		ULONG	m_ulTimeToExpire;
		ULONG	m_ulTimeToExpirePrev;

		// Timer expire caller
		std::function<void()> m_delOnExpired;

	public:
		inline TimeStampTimer();
		inline ~TimeStampTimer();


		// Set timer delegate
		inline  void	SetTimerFunc( std::function<void()> funcOnExpired );

		// set timer
		HRESULT	SetTimer( ULONG TimerDuration );

		// clear timer
		inline void	ClearTimer();

		ULONG	GetTimerExpireTime()							{ return m_ulTimeToExpire; }

		// check about timer is working
		inline bool	IsTimerWorking() const;

		// Timer check update
		inline bool	CheckTimer();
	};


	////////////////////////////////////////////////////////////////////////////////
	//
	//	Utility
	//

	LONG TimeMin(ULONG timeMs, ULONG timeMs2);

	inline LONG TimeSince(ULONG timeMs)					{ return (LONG)(Time.GetTimeMs() - timeMs); }
	inline LONGLONG TimeSinceUTC(ULONGLONG timeUTC)		{ return (LONGLONG)(Time.GetTimeUTCSec() - timeUTC); }
	inline LONG TimeSinceUTC(ULONG timeUTC)				{ return (LONG)(Time.GetTimeUTCSec32() - timeUTC); }

#include "TimeUtil.inl"


}; // namespace Util
} // namespace BR


