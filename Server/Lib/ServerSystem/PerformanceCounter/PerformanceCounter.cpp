////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 Madk
// 
// Author : KyungKun Ko
//
// Description : Performance counter
//
////////////////////////////////////////////////////////////////////////////////


#include "StdAfx.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/PerformanceCounter/PerformanceCounter.h"





namespace BR {
namespace Svr {




	PerformanceCounter::PerformanceCounter(const char* counterName, DataTypes dataType, CountingTypes countingType)
		: m_DataType(dataType)
		, m_CountingType(countingType)
		, m_SyncSerial(0)
	{
		StrUtil::StringCpy(m_CounterName, counterName);
	}

	PerformanceCounter::PerformanceCounter(DataTypes dataType, CountingTypes countingType)
		: m_DataType(dataType)
		, m_CountingType(countingType)
	{
		StrUtil::StringCpy(m_CounterName, "");
	}

	PerformanceCounter::~PerformanceCounter()
	{

	}

	void PerformanceCounter::SetCounterName(const char* strName)
	{
		StrUtil::StringCpy(m_CounterName, strName);
	}


	




	template PerformanceCounterRaw < INT32 >;
	template PerformanceCounterRaw < UINT32 >;
	template PerformanceCounterRaw < INT64 >;
	template PerformanceCounterRaw < UINT64 >;

	template PerformanceCounterAveragePerSec < INT32 >;
	template PerformanceCounterAveragePerSec < UINT32 >;
	template PerformanceCounterAveragePerSec < INT64 >;
	template PerformanceCounterAveragePerSec < UINT64 >;

	template PerformanceCounterTickPerSec < INT32 >;
	template PerformanceCounterTickPerSec < UINT32 >;
	template PerformanceCounterTickPerSec < INT64 >;
	template PerformanceCounterTickPerSec < UINT64 >;


};	// Svr
};	// namespace BR


