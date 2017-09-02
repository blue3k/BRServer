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





namespace SF {
namespace Svr {

	//template<> PerformanceCounter::DataTypes PerformanceCounterRaw<int32_t>::GetDataType() { return DataTypes::Int32; }
	//template<> PerformanceCounter::DataTypes PerformanceCounterRaw<uint32_t>::GetDataType() { return DataTypes::UInt32; }
	//template<> PerformanceCounter::DataTypes PerformanceCounterRaw<int64_t>::GetDataType() { return DataTypes::Int64; }
	//template<> PerformanceCounter::DataTypes PerformanceCounterRaw<uint64_t>::GetDataType() { return DataTypes::UInt64; }



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


	




	template class PerformanceCounterRaw < int32_t >;
	template class PerformanceCounterRaw < uint32_t >;
	template class PerformanceCounterRaw < int64_t >;
	template class PerformanceCounterRaw < uint64_t >;

	template class PerformanceCounterAveragePerSec < int32_t >;
	template class PerformanceCounterAveragePerSec < uint32_t >;
	template class PerformanceCounterAveragePerSec < int64_t >;
	template class PerformanceCounterAveragePerSec < uint64_t >;

	template class PerformanceCounterTickPerSec < int32_t >;
	template class PerformanceCounterTickPerSec < uint32_t >;
	template class PerformanceCounterTickPerSec < int64_t >;
	template class PerformanceCounterTickPerSec < uint64_t >;


};	// Svr
};	// namespace SF


