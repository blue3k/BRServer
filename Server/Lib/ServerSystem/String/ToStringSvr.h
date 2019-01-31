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
#include "String/SFToString.h"
#include "String/SFToStringEngineTypes.h"


namespace SF {

	inline Result _ToString(ToStringContext& context, const ServerID& Data) { return _IToA(context, (uint32_t)Data); }
	inline Result _ToString(ToStringContext& context, const AccountID& Data) { return _IToA(context, (uint64_t)Data); }
	//Result _ToString(ToStringContext& context, const EntityID& Data, int Option);
	Result _ToString(ToStringContext& context, const EntityUID& Data);
	inline Result _ToString(ToStringContext& context, const EntityState& Data) { return _IToA(context, (uint32_t)Data); }

	Result _ToString(ToStringContext& context, const TransactionID& Data);
	Result _ToString(ToStringContext& context, const ClusterID& Data);
	Result _ToString(ToStringContext& context, const RouteContext& Data);
	Result _ToString(ToStringContext& context, const ClusterType& Data);
	Result _ToString(ToStringContext& context, const ClusterMembership& Data);
	Result _ToString(ToStringContext& context, const ServiceStatus& Data);
	Result _ToString(ToStringContext& context, const LocalUID& Data);
	Result _ToString(ToStringContext& context, const ServiceInformation& Data);
	//Result _ToString(ToStringContext& context, const MatchingQueueTicket& Data);
	Result _ToString(ToStringContext& context, const MatchingPlayerInformation& Data);
	Result _ToString(ToStringContext& context, const PerformanceCounterInfo& Data);
	Result _ToString(ToStringContext& context, const PerformanceCounterInstanceInfo& Data);
	Result _ToString(ToStringContext& context, const ServerFriendInformation& Data);

	DEFINE_BOXING_TEMPLETE_BYVALUE(EntityState);
	DEFINE_BOXING_TEMPLETE_BYVALUE(EntityUID);
	DEFINE_BOXING_TEMPLETE_BYVALUE(TransactionID);
	DEFINE_BOXING_TEMPLETE_BYVALUE(ClusterID);
	DEFINE_BOXING_TEMPLETE_BYVALUE(RouteContext);
	DEFINE_BOXING_TEMPLETE_BYVALUE(ClusterType);
	DEFINE_BOXING_TEMPLETE_BYVALUE(ClusterMembership);
	DEFINE_BOXING_TEMPLETE_BYVALUE(ServiceStatus);
	DEFINE_BOXING_TEMPLETE_BYVALUE(LocalUID);
	DEFINE_BOXING_TEMPLETE_BYVALUE(ServiceInformation);
	DEFINE_BOXING_TEMPLETE_BYVALUE(MatchingPlayerInformation);
	DEFINE_BOXING_TEMPLETE_BYVALUE(PerformanceCounterInfo);
	DEFINE_BOXING_TEMPLETE_BYVALUE(PerformanceCounterInstanceInfo);
	DEFINE_BOXING_TEMPLETE_BYVALUE(ServerFriendInformation);



/*
	inline Result _ToString(ToStringContext& context, const LinkedArray<MatchingQueueTicket>& Data, int Option)
	{
		return ToStringArray(pBuff, iBuffLen, Data, Option);
	}

	inline Result _ToString(ToStringContext& context, const LinkedArray<EntityUID>& Data, int Option)
	{
		return ToStringArray(pBuff, iBuffLen, Data, Option);
	}

	inline Result _ToString(ToStringContext& context, const LinkedArray<PerformanceCounterInfo>& Data, int Option)
	{
		return ToStringArray(pBuff, iBuffLen, Data, Option);
	}

	inline Result _ToString(ToStringContext& context, const LinkedArray<PerformanceCounterInstanceInfo>& Data, int Option)
	{
		return ToStringArray(pBuff, iBuffLen, Data, Option);
	}

	inline Result _ToString(ToStringContext& context, const LinkedArray<ServiceStatus>& Data, int Option)
	{
		return ToStringArray(pBuff, iBuffLen, Data, Option);
	}

	inline Result _ToString(ToStringContext& context, const LinkedArray<ServiceInformation>& Data, int Option)
	{
		return ToStringArray(pBuff, iBuffLen, Data, Option);
	}

	inline Result _ToString(ToStringContext& context, const LinkedArray<MatchingPlayerInformation>& Data, int Option)
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
*/
}; // namespace StrUtil



