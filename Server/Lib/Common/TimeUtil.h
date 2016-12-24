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
//#include "Common/ClassUtil.h"
#include "Common/BrLibComponents.h"

namespace BR {
namespace Util {

	////////////////////////////////////////////////////////////////////////////////
	//
	//	Win32 timer class
	//

	class Time_Chrono
	{
	public:
		const int UTC_REFERENCE_YEAR = 2014;

	private:

		std::chrono::time_point<ClockType> m_TickStamp;

		// Time stamp
		std::atomic<UINT64>			m_ullTimeStamp;

		ClockType::duration			m_ullTimeStampPrevious;
		std::atomic<ULONG>			m_ulTimeStampMs;
		std::atomic<ULONGLONG>		m_ullTimeStampUTC;

		// Default UTC offset
		ULONGLONG					m_ullUTCOffset;

	protected:
		// Update Time stamp
		void		UpdateTimer();

		friend class TimerThread;

	public:
		Time_Chrono();
		~Time_Chrono();


		/////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// Time query
		//

		// Get time stamp in ms
		TimeStampMS		GetTimeMs();

		// Get UTC time stamp
		TimeStampSec	GetTimeUTCSec();



		/////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// Global Time query
		//

		// Get time tick in ms
		TimeStampMS		GetRawTimeMs();

		// Get current UTC sec
		TimeStampSec	GetRawUTCSec();

		DurationSec		GetUTCSecOffset() { return DurationSec(m_ullUTCOffset); }

	};


	// Global main timer
	extern Time_Chrono Time;


	////////////////////////////////////////////////////////////////////////////////
	//
	//	Timer
	//

	class TimeStampTimer
	{
	public:
		const static TimeStampMS	InvalidTime;

	private:
		TimeStampMS	m_ulTimeToExpire;
		TimeStampMS	m_ulTimeToExpirePrev;

		// Timer expire caller
		std::function<void()> m_delOnExpired;

	public:
		inline TimeStampTimer();
		inline ~TimeStampTimer();


		// Set timer delegate
		inline  void	SetTimerFunc( std::function<void()> funcOnExpired );

		// set timer
		Result	SetTimer(DurationMS TimerDuration );

		// clear timer
		inline void	ClearTimer();

		TimeStampMS	GetTimerExpireTime()							{ return m_ulTimeToExpire; }

		// check about timer is working
		inline bool	IsTimerWorking() const;

		// Timer check update
		bool	CheckTimer();
	};


	////////////////////////////////////////////////////////////////////////////////
	//
	//	Module
	//

	class LibComponentTime : public Component
	{
	public:

		enum {
			ComponentID = (UINT)LibCompoentIDs::Timer
		};

	public:

		LibComponentTime();
		~LibComponentTime();

		// Initialize server component
		virtual Result InitializeComponent() override;
		// Terminate server component
		virtual void TerminateComponent() override;
	};




	////////////////////////////////////////////////////////////////////////////////
	//
	//	Utility
	//



	DurationMS TimeMinNonZero(DurationMS timeMs, DurationMS timeMs2);
	TimeStampMS TimeMinNonZero(TimeStampMS timeMs, TimeStampMS timeMs2);


	inline DurationMS TimeSinceRaw(TimeStampMS timeMs)				{ auto timeCur = Time.GetRawTimeMs(); return (timeCur > timeMs) ? (timeCur - timeMs) : DurationMS(0); }

	inline DurationMS TimeSince(TimeStampMS timeMs)					{ auto timeCur = Time.GetTimeMs(); return (timeCur > timeMs) ? (timeCur - timeMs) : DurationMS(0); }
	inline DurationSec TimeSinceUTC(TimeStampSec timeUTC)			{ auto timeCur = Time.GetTimeUTCSec(); return (timeCur > timeUTC) ? (timeCur - timeUTC) : DurationSec(0); }

#include "TimeUtil.inl"


}; // namespace Util
} // namespace BR


