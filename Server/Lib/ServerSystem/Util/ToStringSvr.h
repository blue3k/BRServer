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
#include "Util/SFToStringEngineTypes.h"
#include "Variable/SFVariableBoxing.h"
#include "Types/BrSvrTypeSerialization.h"

namespace SF
{

	Result _ToString(ToStringContext& context, const EntityUID& Data);
	inline Result _ToString(ToStringContext& context, const EntityState& Data) { return _IToA(context, (uint32_t)Data); }

	Result _ToString(ToStringContext& context, const TransactionID& Data);
	Result _ToString(ToStringContext& context, const ClusterID& Data);
	Result _ToString(ToStringContext& context, const ClusterType& Data);
	Result _ToString(ToStringContext& context, const ClusterMembership& Data);
	Result _ToString(ToStringContext& context, const ServiceStatus& Data);
	Result _ToString(ToStringContext& context, const LocalUID& Data);
	Result _ToString(ToStringContext& context, const ServiceInformation& Data);
	Result _ToString(ToStringContext& context, const MatchingPlayerInformation& Data);
	Result _ToString(ToStringContext& context, const PerformanceCounterInfo& Data);
	Result _ToString(ToStringContext& context, const PerformanceCounterInstanceInfo& Data);
	Result _ToString(ToStringContext& context, const ServerFriendInformation& Data);


	DEFINE_BOXING_TEMPLETE_BYVALUE(EntityState);
	DEFINE_BOXING_TEMPLETE_BYVALUE(EntityUID);
	DEFINE_BOXING_TEMPLETE_BYVALUE(TransactionID);
	DEFINE_BOXING_TEMPLETE_BYVALUE(ClusterID);
	DEFINE_BOXING_TEMPLETE_BYVALUE(ClusterType);
	DEFINE_BOXING_TEMPLETE_BYVALUE(ClusterMembership);
	DEFINE_BOXING_TEMPLETE_BYVALUE(ServiceStatus);
	DEFINE_BOXING_TEMPLETE_BYVALUE(LocalUID);
	DEFINE_BOXING_TEMPLETE_BYVALUE(ServiceInformation);
	DEFINE_BOXING_TEMPLETE_BYVALUE(MatchingPlayerInformation);
	DEFINE_BOXING_TEMPLETE_BYVALUE(PerformanceCounterInfo);
	DEFINE_BOXING_TEMPLETE_BYVALUE(PerformanceCounterInstanceInfo);
	DEFINE_BOXING_TEMPLETE_BYVALUE(ServerFriendInformation);



} // namespace SF


