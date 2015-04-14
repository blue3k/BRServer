////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Performance Counter library
//	
//
////////////////////////////////////////////////////////////////////////////////




////////////////////////////////////////////////////////////////////////////////
//
//	Counter Instance Map
//

bool CounterInstanceMap::IsOpened()
{
	return m_pCounters != NULL;
}




// Get name of instance
const WCHAR *CounterInstanceMap::GetInstanceName()
{
	return (const WCHAR*)((m_pCounters && m_pCounters->szName[0] != L'\0') ? m_pCounters->szName : NULL);
}

// Get Class name
const WCHAR *CounterInstanceMap::GetClass()
{
	return (const WCHAR*)((m_pCounters && m_pCounters->szClass[0] != L'\0') ? m_pCounters->szClass : NULL);
}






////////////////////////////////////////////////////////////////////////////////
//
//	Counter wrapper
//


const WCHAR* Counter::GetCounterName() const
{
	return m_wszName;
}

bool Counter::IsConnected()
{
	return m_pCounter != NULL;
}


Counter::operator CounterType()
{
	if( !IsConnected() )
		return 0;

	return *m_pCounter;
}

CounterType Counter::operator++()
{
	if( IsConnected() )
	{
		(*m_pCounter)++;
	}
	else
		return 0;

	return *m_pCounter;
}
CounterType Counter::operator++(int)
{
	if( IsConnected() )
	{
		(*m_pCounter)++;
	}
	else
		return 0;

	return *m_pCounter;
}

CounterType Counter::operator--()
{
	if( IsConnected() )
	{
		(*m_pCounter)--;
	}
	else
		return 0;

	return *m_pCounter;
}

CounterType Counter::operator--(int)
{
	if( IsConnected() )
	{
		(*m_pCounter)--;
	}
	else
		return 0;

	return *m_pCounter;
}

CounterType Counter::operator+=(int value)
{
	if( IsConnected() )
	{
		(*m_pCounter)+=value;
	}
	else
		return 0;

	return *m_pCounter;
}

CounterType Counter::operator-=(int value)
{
	if( IsConnected() )
	{
		(*m_pCounter)-=value;
	}
	else
		return 0;

	return *m_pCounter;
}


CounterType Counter::operator=(CounterType value)
{
	if( IsConnected() )
	{
		(*m_pCounter) = value;
	}
	else
		return 0;

	return *m_pCounter;
}



CounterType Counter::operator=(Counter& value)
{
	if( IsConnected() )
	{
		(*m_pCounter) = (UINT32)value;
	}
	else
		return 0;

	return *m_pCounter;
}


