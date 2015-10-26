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



TimeStampTimer::TimeStampTimer()
{
}

TimeStampTimer::~TimeStampTimer()
{
}

// Set timer delegate
void TimeStampTimer::SetTimerFunc( std::function<void()> funcOnExpired )
{
	m_delOnExpired = funcOnExpired;
}

// clear timer
void TimeStampTimer::ClearTimer()
{
	m_ulTimeToExpire = InvalidTime;
}

// check about timer is working
bool	TimeStampTimer::IsTimerWorking() const
{
	return m_ulTimeToExpire != InvalidTime;
}

// Timer check update
bool	TimeStampTimer::CheckTimer()
{
	bool bExpired = m_ulTimeToExpire != InvalidTime && ((LONG)(m_ulTimeToExpire - Time.GetTimeMs()).count() < 0);

	if( bExpired )
	{
		if( m_delOnExpired )
			m_delOnExpired();

		// Clear timer
		m_ulTimeToExpirePrev = m_ulTimeToExpire;
		m_ulTimeToExpire = InvalidTime;
	}

	return bExpired;
}
