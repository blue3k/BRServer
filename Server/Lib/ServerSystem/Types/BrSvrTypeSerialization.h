////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author: Kyungkun Ko
//
// Description : Brave base types
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "SFTypedefs.h"
#include "SFAssert.h"
#include "String/SFStrUtil.h"
#include "Types/SFEngineTypedefs.h"
#include "Types/BRSvrTypes.h"


namespace SF {


	inline size_t SerializedSizeOf(const ServiceStatus& Value) { return sizeof(Value); }
	inline Result operator >> (IInputStream& input, ServiceStatus& data) { return input.Read(&data, sizeof(data)); }
	inline Result operator << (IOutputStream& output, const ServiceStatus& data) { return output.Write(&data, sizeof(data)); }

	inline size_t SerializedSizeOf(const EntityState& Value) { return sizeof(Value); }
	inline Result operator >> (IInputStream& input, EntityState& data) { return input.Read(&data, sizeof(data)); }
	inline Result operator << (IOutputStream& output, const EntityState& data) { return output.Write(&data, sizeof(data)); }

	inline size_t SerializedSizeOf(const MatchingPlayerInformation& Value) { return sizeof(Value); }
	inline Result operator >> (IInputStream& input, MatchingPlayerInformation& data) { return input.Read(&data, sizeof(data)); }
	inline Result operator << (IOutputStream& output, const MatchingPlayerInformation& data) { return output.Write(&data, sizeof(data)); }

	inline size_t SerializedSizeOf(const ServiceInformation& Value) { return sizeof(Value); }
	inline Result operator >> (IInputStream& input, ServiceInformation& data) { return input.Read(&data, sizeof(data)); }
	inline Result operator << (IOutputStream& output, const ServiceInformation& data) { return output.Write(&data, sizeof(data)); }

	inline size_t SerializedSizeOf(const PerformanceCounterInfo& Value) { return sizeof(Value); }
	inline Result operator >> (IInputStream& input, PerformanceCounterInfo& data) { return input.Read(&data, sizeof(data)); }
	inline Result operator << (IOutputStream& output, const PerformanceCounterInfo& data) { return output.Write(&data, sizeof(data)); }

	inline size_t SerializedSizeOf(const PerformanceCounterInstanceInfo& Value) { return sizeof(Value); }
	inline Result operator >> (IInputStream& input, PerformanceCounterInstanceInfo& data) { return input.Read(&data, sizeof(data)); }
	inline Result operator << (IOutputStream& output, const PerformanceCounterInstanceInfo& data) { return output.Write(&data, sizeof(data)); }


};


