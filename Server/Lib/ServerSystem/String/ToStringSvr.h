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

#include "SFTypedefs.h"
#include "Types/BrSvrTypes.h"
#include "Types/BrGameTypes.h"
#include "String/ToStringBase.h"
#include "String/Argument.h"


namespace SF {

	//Result ToString(char*& pBuff, INT& iBuffLen, const EntityID& Data, int Option);
	Result ToString(char*& pBuff, INT& iBuffLen, const EntityUID& Data, int Option);

	Result ToString(char*& pBuff, INT& iBuffLen, const TransactionID& Data, int Option);
	Result ToString(char*& pBuff, INT& iBuffLen, const ClusterID& Data, int Option);
	Result ToString(char*& pBuff, INT& iBuffLen, const RouteContext& Data, int Option);
	Result ToString(char*& pBuff, INT& iBuffLen, const ClusterType& Data, int Option);
	Result ToString(char*& pBuff, INT& iBuffLen, const ClusterMembership& Data, int Option);
	Result ToString(char*& pBuff, INT& iBuffLen, const ServiceStatus& Data, int Option);
	Result ToString(char*& pBuff, INT& iBuffLen, const LocalUID& Data, int Option);
	Result ToString(char*& pBuff, INT& iBuffLen, const ServiceInformation& Data, int Option);
	Result ToString(char*& pBuff, INT& iBuffLen, const MatchingQueueTicket& Data, int Option);
	Result ToString(char*& pBuff, INT& iBuffLen, const MatchingPlayerInformation& Data, int Option);
	Result ToString(char*& pBuff, INT& iBuffLen, const PerformanceCounterInfo& Data, int Option);
	Result ToString(char*& pBuff, INT& iBuffLen, const PerformanceCounterInstanceInfo& Data, int Option);
	Result ToString(char*& pBuff, INT& iBuffLen, const ServerFriendInformation& Data, int Option);

	extern template Result ToStringArray(char*& pBuff, INT& iBuffLen, const Array<ServiceStatus>& Data, int Option);
	extern template Result ToStringArray(char*& pBuff, INT& iBuffLen, const Array<MatchingQueueTicket>& Data, int Option);
	extern template Result ToStringArray(char*& pBuff, INT& iBuffLen, const Array<MatchingPlayerInformation>& Data, int Option);
	extern template Result ToStringArray(char*& pBuff, INT& iBuffLen, const Array<PerformanceCounterInfo>& Data, int Option);
	extern template Result ToStringArray(char*& pBuff, INT& iBuffLen, const Array<PerformanceCounterInstanceInfo>& Data, int Option);


	inline Result ToString(char*& pBuff, INT& iBuffLen, const LinkedArray<MatchingQueueTicket>& Data, int Option)
	{
		return ToStringArray(pBuff, iBuffLen, Data, Option);
	}

	inline Result ToString(char*& pBuff, INT& iBuffLen, const LinkedArray<EntityUID>& Data, int Option)
	{
		return ToStringArray(pBuff, iBuffLen, Data, Option);
	}

	inline Result ToString(char*& pBuff, INT& iBuffLen, const LinkedArray<PerformanceCounterInfo>& Data, int Option)
	{
		return ToStringArray(pBuff, iBuffLen, Data, Option);
	}

	inline Result ToString(char*& pBuff, INT& iBuffLen, const LinkedArray<PerformanceCounterInstanceInfo>& Data, int Option)
	{
		return ToStringArray(pBuff, iBuffLen, Data, Option);
	}

	inline Result ToString(char*& pBuff, INT& iBuffLen, const LinkedArray<ServiceStatus>& Data, int Option)
	{
		return ToStringArray(pBuff, iBuffLen, Data, Option);
	}

	inline Result ToString(char*& pBuff, INT& iBuffLen, const LinkedArray<ServiceInformation>& Data, int Option)
	{
		return ToStringArray(pBuff, iBuffLen, Data, Option);
	}

	inline Result ToString(char*& pBuff, INT& iBuffLen, const LinkedArray<MatchingPlayerInformation>& Data, int Option)
	{
		return ToStringArray(pBuff, iBuffLen, Data, Option);
	}


	extern template class ArgumentWrapperT < TransactionID>;
	extern template class ArgumentWrapperT < ClusterID>;
	extern template class ArgumentWrapperT < RouteContext>;
	extern template class ArgumentWrapperT < ClusterType>;
	extern template class ArgumentWrapperT < ServiceStatus>;
	extern template class ArgumentWrapperT < LocalUID>;
	extern template class ArgumentWrapperT < ServiceInformation>;
	extern template class ArgumentWrapperT < MatchingQueueTicket>;
	extern template class ArgumentWrapperT < MatchingPlayerInformation>;
	extern template class ArgumentWrapperT < PerformanceCounterInfo>;
	extern template class ArgumentWrapperT < PerformanceCounterInstanceInfo>;
	extern template class ArgumentWrapperT < ServerFriendInformation>;


	extern template class ArgArray < ServiceStatus>;
	extern template class ArgArray < MatchingQueueTicket>;
	extern template class ArgArray < MatchingPlayerInformation>;
	extern template class ArgArray < PerformanceCounterInfo>;
	extern template class ArgArray < PerformanceCounterInstanceInfo>;

}; // namespace StrUtil



