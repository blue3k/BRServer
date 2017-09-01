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

	//Result ToString(char*& pBuff, INT& iBuffLen, const BR::EntityID& Data, int Option);
	Result ToString(char*& pBuff, INT& iBuffLen, const BR::EntityUID& Data, int Option);

	Result ToString(char*& pBuff, INT& iBuffLen, const BR::TransactionID& Data, int Option);
	Result ToString(char*& pBuff, INT& iBuffLen, const BR::ClusterID& Data, int Option);
	Result ToString(char*& pBuff, INT& iBuffLen, const BR::RouteContext& Data, int Option);
	Result ToString(char*& pBuff, INT& iBuffLen, const BR::ClusterType& Data, int Option);
	Result ToString(char*& pBuff, INT& iBuffLen, const BR::ClusterMembership& Data, int Option);
	Result ToString(char*& pBuff, INT& iBuffLen, const BR::ServiceStatus& Data, int Option);
	Result ToString(char*& pBuff, INT& iBuffLen, const BR::LocalUID& Data, int Option);
	Result ToString(char*& pBuff, INT& iBuffLen, const BR::ServiceInformation& Data, int Option);
	Result ToString(char*& pBuff, INT& iBuffLen, const BR::MatchingQueueTicket& Data, int Option);
	Result ToString(char*& pBuff, INT& iBuffLen, const BR::MatchingPlayerInformation& Data, int Option);
	Result ToString(char*& pBuff, INT& iBuffLen, const BR::PerformanceCounterInfo& Data, int Option);
	Result ToString(char*& pBuff, INT& iBuffLen, const BR::PerformanceCounterInstanceInfo& Data, int Option);
	Result ToString(char*& pBuff, INT& iBuffLen, const BR::ServerFriendInformation& Data, int Option);

	extern template Result ToStringArray(char*& pBuff, INT& iBuffLen, const Array<BR::ServiceStatus>& Data, int Option);
	extern template Result ToStringArray(char*& pBuff, INT& iBuffLen, const Array<BR::MatchingQueueTicket>& Data, int Option);
	extern template Result ToStringArray(char*& pBuff, INT& iBuffLen, const Array<BR::MatchingPlayerInformation>& Data, int Option);
	extern template Result ToStringArray(char*& pBuff, INT& iBuffLen, const Array<BR::PerformanceCounterInfo>& Data, int Option);
	extern template Result ToStringArray(char*& pBuff, INT& iBuffLen, const Array<BR::PerformanceCounterInstanceInfo>& Data, int Option);


	inline Result ToString(char*& pBuff, INT& iBuffLen, const LinkedArray<MatchingQueueTicket>& Data, int Option)
	{
		return ToStringArray(pBuff, iBuffLen, Data, Option);
	}

	inline Result ToString(char*& pBuff, INT& iBuffLen, const LinkedArray<EntityUID>& Data, int Option)
	{
		return ToStringArray(pBuff, iBuffLen, Data, Option);
	}

	inline Result ToString(char*& pBuff, INT& iBuffLen, const LinkedArray<BR::PerformanceCounterInfo>& Data, int Option)
	{
		return ToStringArray(pBuff, iBuffLen, Data, Option);
	}

	inline Result ToString(char*& pBuff, INT& iBuffLen, const LinkedArray<BR::PerformanceCounterInstanceInfo>& Data, int Option)
	{
		return ToStringArray(pBuff, iBuffLen, Data, Option);
	}

	inline Result ToString(char*& pBuff, INT& iBuffLen, const LinkedArray<BR::ServiceStatus>& Data, int Option)
	{
		return ToStringArray(pBuff, iBuffLen, Data, Option);
	}

	inline Result ToString(char*& pBuff, INT& iBuffLen, const LinkedArray<BR::ServiceInformation>& Data, int Option)
	{
		return ToStringArray(pBuff, iBuffLen, Data, Option);
	}

	inline Result ToString(char*& pBuff, INT& iBuffLen, const LinkedArray<BR::MatchingPlayerInformation>& Data, int Option)
	{
		return ToStringArray(pBuff, iBuffLen, Data, Option);
	}


	extern template class ArgumentWrapperT < BR::TransactionID>;
	extern template class ArgumentWrapperT < BR::ClusterID>;
	extern template class ArgumentWrapperT < BR::RouteContext>;
	extern template class ArgumentWrapperT < BR::ClusterType>;
	extern template class ArgumentWrapperT < BR::ServiceStatus>;
	extern template class ArgumentWrapperT < BR::LocalUID>;
	extern template class ArgumentWrapperT < BR::ServiceInformation>;
	extern template class ArgumentWrapperT < BR::MatchingQueueTicket>;
	extern template class ArgumentWrapperT < BR::MatchingPlayerInformation>;
	extern template class ArgumentWrapperT < BR::PerformanceCounterInfo>;
	extern template class ArgumentWrapperT < BR::PerformanceCounterInstanceInfo>;
	extern template class ArgumentWrapperT < BR::ServerFriendInformation>;


	extern template class ArgArray < BR::ServiceStatus>;
	extern template class ArgArray < BR::MatchingQueueTicket>;
	extern template class ArgArray < BR::MatchingPlayerInformation>;
	extern template class ArgArray < BR::PerformanceCounterInfo>;
	extern template class ArgArray < BR::PerformanceCounterInstanceInfo>;

}; // namespace StrUtil



