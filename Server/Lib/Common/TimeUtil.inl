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
	: m_ulTimeToExpire(UINT_MAX)
	, m_ulTimeToExpirePrev(UINT_MAX)
{
}

TimeStampTimer::~TimeStampTimer()
{
}

// Set timer delegate
void	TimeStampTimer::SetTimerFunc( std::function<void()> funcOnExpired )
{
	m_delOnExpired = funcOnExpired;
}

// clear timer
void	TimeStampTimer::ClearTimer()
{
	m_ulTimeToExpire = UINT_MAX;
}

// check about timer is working
bool	TimeStampTimer::IsTimerWorking() const
{
	return m_ulTimeToExpire != UINT_MAX;
}

// Timer check update
bool	TimeStampTimer::CheckTimer()
{
	bool bExpired = m_ulTimeToExpire != UINT_MAX && (LONG)(m_ulTimeToExpire - Time.GetTimeMs()) < 0;

	if( bExpired )
	{
		if( m_delOnExpired )
			m_delOnExpired();

		// Clear timer
		m_ulTimeToExpirePrev = m_ulTimeToExpire;
		m_ulTimeToExpire = UINT_MAX;
	}

	return bExpired;
}
