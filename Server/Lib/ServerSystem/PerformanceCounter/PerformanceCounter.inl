////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 Madk
// 
// Author : KyungKun Ko
//
// Description : Performance counter
//
////////////////////////////////////////////////////////////////////////////////



extern template class PerformanceCounterRaw < INT32 >;
extern template class PerformanceCounterRaw < UINT32 >;
extern template class PerformanceCounterRaw < INT64 >;
extern template class PerformanceCounterRaw < UINT64 >;

extern template class PerformanceCounterAveragePerSec < INT32 >;
extern template class PerformanceCounterAveragePerSec < UINT32 >;
extern template class PerformanceCounterAveragePerSec < INT64 >;
extern template class PerformanceCounterAveragePerSec < UINT64 >;

extern template class PerformanceCounterTickPerSec < INT32 >;
extern template class PerformanceCounterTickPerSec < UINT32 >;
extern template class PerformanceCounterTickPerSec < INT64 >;
extern template class PerformanceCounterTickPerSec < UINT64 >;
