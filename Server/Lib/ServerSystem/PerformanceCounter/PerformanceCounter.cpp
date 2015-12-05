////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 Madk
// 
// Author : KyungKun Ko
//
// Description : Performance counter
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/PerformanceCounter/PerformanceCounter.h"





namespace BR {
namespace Svr {

	template<> PerformanceCounter::DataTypes PerformanceCounterRaw<INT32>::GetDataType() { return DataTypes::Int32; }
	template<> PerformanceCounter::DataTypes PerformanceCounterRaw<UINT32>::GetDataType() { return DataTypes::UInt32; }
	template<> PerformanceCounter::DataTypes PerformanceCounterRaw<INT64>::GetDataType() { return DataTypes::Int64; }
	template<> PerformanceCounter::DataTypes PerformanceCounterRaw<UINT64>::GetDataType() { return DataTypes::UInt64; }



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


	




	template class PerformanceCounterRaw < INT32 >;
	template class PerformanceCounterRaw < UINT32 >;
	template class PerformanceCounterRaw < INT64 >;
	template class PerformanceCounterRaw < UINT64 >;

	template class PerformanceCounterAveragePerSec < INT32 >;
	template class PerformanceCounterAveragePerSec < UINT32 >;
	template class PerformanceCounterAveragePerSec < INT64 >;
	template class PerformanceCounterAveragePerSec < UINT64 >;

	template class PerformanceCounterTickPerSec < INT32 >;
	template class PerformanceCounterTickPerSec < UINT32 >;
	template class PerformanceCounterTickPerSec < INT64 >;
	template class PerformanceCounterTickPerSec < UINT64 >;


};	// Svr
};	// namespace BR


