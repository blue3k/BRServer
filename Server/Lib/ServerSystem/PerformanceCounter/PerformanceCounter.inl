////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 Kyungkun Ko
// 
// Author : KyungKun Ko
//
// Description : Performance counter
//
////////////////////////////////////////////////////////////////////////////////



extern template class PerformanceCounterRaw < int32_t >;
extern template class PerformanceCounterRaw < uint32_t >;
extern template class PerformanceCounterRaw < int64_t >;
extern template class PerformanceCounterRaw < uint64_t >;

extern template class PerformanceCounterAveragePerSec < int32_t >;
extern template class PerformanceCounterAveragePerSec < uint32_t >;
extern template class PerformanceCounterAveragePerSec < int64_t >;
extern template class PerformanceCounterAveragePerSec < uint64_t >;

extern template class PerformanceCounterTickPerSec < int32_t >;
extern template class PerformanceCounterTickPerSec < uint32_t >;
extern template class PerformanceCounterTickPerSec < int64_t >;
extern template class PerformanceCounterTickPerSec < uint64_t >;
