////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Base type ToString implementation
//	
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Common/Typedefs.h"
#include "Common/BrSvrTypes.h"
#include "Common/ToStringBase.h"
#include "Common/BrCommonTypes.h"
#include "Common/Message.h"
#include "Common/Argument.h"


namespace BR {

	template<> HRESULT ToString(char*& pBuff, INT& iBuffLen, const ClusterID& Data, int Option);
	template<> HRESULT ToString(char*& pBuff, INT& iBuffLen, const BR::RouteContext& Data, int Option);
	template<> HRESULT ToString(char*& pBuff, INT& iBuffLen, const ClusterType& Data, int Option);
	template<> HRESULT ToString(char*& pBuff, INT& iBuffLen, const ClusterMembership& Data, int Option);
	template<> HRESULT ToString(char*& pBuff, INT& iBuffLen, const ServiceStatus& Data, int Option);
	template<> HRESULT ToString(char*& pBuff, INT& iBuffLen, const LocalUID& Data, int Option);
	template<> HRESULT ToString(char*& pBuff, INT& iBuffLen, const ServiceInformation& Data, int Option);
	template<> HRESULT ToString(char*& pBuff, INT& iBuffLen, const MatchingQueueTicket& Data, int Option);
	template<> HRESULT ToString(char*& pBuff, INT& iBuffLen, const MatchingPlayerInformation& Data, int Option);
	template<> HRESULT ToString(char*& pBuff, INT& iBuffLen, const PerformanceCounterInfo& Data, int Option);
	template<> HRESULT ToString(char*& pBuff, INT& iBuffLen, const PerformanceCounterInstanceInfo& Data, int Option);
	template<> HRESULT ToString(char*& pBuff, INT& iBuffLen, const ServerFriendInformation& Data, int Option);

	extern template HRESULT ToStringArray(char*& pBuff, INT& iBuffLen, const Array<ServiceStatus>& Data, int Option);
	extern template HRESULT ToStringArray(char*& pBuff, INT& iBuffLen, const Array<MatchingQueueTicket>& Data, int Option);
	extern template HRESULT ToStringArray(char*& pBuff, INT& iBuffLen, const Array<MatchingPlayerInformation>& Data, int Option);
	extern template HRESULT ToStringArray(char*& pBuff, INT& iBuffLen, const Array<PerformanceCounterInfo>& Data, int Option);
	extern template HRESULT ToStringArray(char*& pBuff, INT& iBuffLen, const Array<PerformanceCounterInstanceInfo>& Data, int Option);


	template<>
	inline HRESULT ToString(char*& pBuff, INT& iBuffLen, const LinkedArray<MatchingQueueTicket>& Data, int Option)
	{
		return ToStringArray(pBuff, iBuffLen, Data, Option);
	}

	template<>
	inline HRESULT ToString(char*& pBuff, INT& iBuffLen, const LinkedArray<EntityUID>& Data, int Option)
	{
		return ToStringArray(pBuff, iBuffLen, Data, Option);
	}

	template<>
	inline HRESULT ToString(char*& pBuff, INT& iBuffLen, const LinkedArray<PerformanceCounterInfo>& Data, int Option)
	{
		return ToStringArray(pBuff, iBuffLen, Data, Option);
	}

	template<>
	inline HRESULT ToString(char*& pBuff, INT& iBuffLen, const LinkedArray<PerformanceCounterInstanceInfo>& Data, int Option)
	{
		return ToStringArray(pBuff, iBuffLen, Data, Option);
	}

	template<>
	inline HRESULT ToString(char*& pBuff, INT& iBuffLen, const LinkedArray<ServiceStatus>& Data, int Option)
	{
		return ToStringArray(pBuff, iBuffLen, Data, Option);
	}

	template<>
	inline HRESULT ToString(char*& pBuff, INT& iBuffLen, const LinkedArray<ServiceInformation>& Data, int Option)
	{
		return ToStringArray(pBuff, iBuffLen, Data, Option);
	}

	template<>
	inline HRESULT ToString(char*& pBuff, INT& iBuffLen, const LinkedArray<MatchingPlayerInformation>& Data, int Option)
	{
		return ToStringArray(pBuff, iBuffLen, Data, Option);
	}


	extern template class Arg < ClusterID>;
	extern template class Arg < RouteContext>;
	extern template class Arg < ClusterType>;
	extern template class Arg < ServiceStatus>;
	extern template class Arg < LocalUID>;
	extern template class Arg < ServiceInformation>;
	extern template class Arg < MatchingQueueTicket>;
	extern template class Arg < MatchingPlayerInformation>;
	extern template class Arg < PerformanceCounterInfo>;
	extern template class Arg < PerformanceCounterInstanceInfo>;
	extern template class Arg < ServerFriendInformation>;


	extern template class ArgArray < ServiceStatus>;
	extern template class ArgArray < MatchingQueueTicket>;
	extern template class ArgArray < MatchingPlayerInformation>;
	extern template class ArgArray < PerformanceCounterInfo>;
	extern template class ArgArray < PerformanceCounterInstanceInfo>;

}; // namespace StrUtil



