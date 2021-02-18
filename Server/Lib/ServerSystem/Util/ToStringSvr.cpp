////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 MadK
// 
// Author : KyungKun Ko
//
// Description : Base type ToString implementation
//	
//
////////////////////////////////////////////////////////////////////////////////


#include "ServerSystemPCH.h"
#include "SFAssert.h"
#include "Util/SFStrUtil.h"
#include "Util/SFToString.h"
#include "Util/ToStringSvr.h"



namespace SF
{
	IMPLEMENT_BOXING_TEMPLETE_BYVALUE(EntityState);
	IMPLEMENT_BOXING_TEMPLETE_BYVALUE(EntityUID);
	IMPLEMENT_BOXING_TEMPLETE_BYVALUE(TransactionID);
	IMPLEMENT_BOXING_TEMPLETE_BYVALUE(ClusterID);
	IMPLEMENT_BOXING_TEMPLETE_BYVALUE(ClusterType);
	IMPLEMENT_BOXING_TEMPLETE_BYVALUE(ClusterMembership);
	IMPLEMENT_BOXING_TEMPLETE_BYVALUE(ServiceStatus);
	IMPLEMENT_BOXING_TEMPLETE_BYVALUE(LocalUID);
	IMPLEMENT_BOXING_TEMPLETE_BYVALUE(ServiceInformation);
	IMPLEMENT_BOXING_TEMPLETE_BYVALUE(MatchingPlayerInformation);
	IMPLEMENT_BOXING_TEMPLETE_BYVALUE(PerformanceCounterInfo);
	IMPLEMENT_BOXING_TEMPLETE_BYVALUE(PerformanceCounterInstanceInfo);
	IMPLEMENT_BOXING_TEMPLETE_BYVALUE(ServerFriendInformation);





	Result _ToString(ToStringContext& context, const EntityUID& Data)
	{
		if (!(_ToString(context, Data.GetServerID())))
			return ResultCode::FAIL;

		if (!(StrUtil::StringCopyEx(context.StringBuffer, context.StringBufferLength, ":")))
			return ResultCode::FAIL;

		if (!(_ToString(context, Data.GetEntityID())))
			return ResultCode::FAIL;

		return ResultCode::SUCCESS;
	}

	Result _ToString(ToStringContext& context, const TransactionID& Data)
	{
		if (!(_ToString(context, Data.GetEntityID())))
			return ResultCode::FAIL;

		if (!(StrUtil::StringCopyEx(context.StringBuffer, context.StringBufferLength, ":")))
			return ResultCode::FAIL;

		auto oldRadix = context.Radix;
		context.Radix = 16;
		if (!(_IToA(context, (uint32_t)Data.GetTransactionIndex())))
			return ResultCode::FAIL;
		context.Radix = oldRadix;

		return ResultCode::SUCCESS;
	}

	Result _ToString(ToStringContext& context, const ClusterID& Data)
	{
		return _ToString(context, (uint)Data);
	}



	Result _ToString(ToStringContext& context, const ClusterType& Data)
	{
		return _ToString(context, (uint)Data);
	}

	Result _ToString(ToStringContext& context, const ClusterMembership& Data)
	{
		return _ToString(context, (uint)Data);
	}

	Result _ToString(ToStringContext& context, const ServiceStatus& Data)
	{
		return _ToString(context, (uint)Data);
	}



	Result _ToString(ToStringContext& context, const LocalUID& Data)
	{
		if (!(_IToA(context, (uint32_t)Data.Time)))
			return ResultCode::FAIL;

		if (!(StrUtil::StringCopyEx(context.StringBuffer, context.StringBufferLength, ":")))
			return ResultCode::FAIL;

		if (!(_IToA(context, (uint32_t)Data.ID)))
			return ResultCode::FAIL;

		return ResultCode::SUCCESS;
	}

	Result _ToString(ToStringContext& context, const ServiceInformation& Data)
	{
		if (!(StrUtil::StringCopyEx(context.StringBuffer, context.StringBufferLength, "(")))
			return ResultCode::FAIL;

		if (!(_ToString(context, Data.UID)))
			return ResultCode::FAIL;

		if (!(StrUtil::StringCopyEx(context.StringBuffer, context.StringBufferLength, ":C")))
			return ResultCode::FAIL;

		if (!(StrUtil::StringCopyEx(context.StringBuffer, context.StringBufferLength, ":M")))
			return ResultCode::FAIL;

		if (!(_ToString(context, Data.Membership)))
			return ResultCode::FAIL;

		if (!(StrUtil::StringCopyEx(context.StringBuffer, context.StringBufferLength, ":S")))
			return ResultCode::FAIL;

		if (!(_ToString(context, Data.Status)))
			return ResultCode::FAIL;

		if (!(StrUtil::StringCopyEx(context.StringBuffer, context.StringBufferLength, ":T")))
			return ResultCode::FAIL;

		if (!(_ToString(context, Data.ServerUpTime.time_since_epoch().count())))
			return ResultCode::FAIL;

		if (!(StrUtil::StringCopyEx(context.StringBuffer, context.StringBufferLength, ":W")))
			return ResultCode::FAIL;

		if (!(_ToString(context, Data.Workload)))
			return ResultCode::FAIL;

		if (!(StrUtil::StringCopyEx(context.StringBuffer, context.StringBufferLength, ")")))
			return ResultCode::FAIL;

		return ResultCode::SUCCESS;
	}

	
	/*

	Result _ToString(ToStringContext& context, const MatchingQueueTicket& Data)
	{
		unused(Option);

		if (!(StrUtil::StringCopyEx(context.StringBuffer, context.StringBufferLength, "(")))
			return ResultCode::FAIL;

		if (!(_ToString(context, Data.QueueUID)))
			return ResultCode::FAIL;


		if (!(StrUtil::StringCopyEx(context.StringBuffer, context.StringBufferLength, ":")))
			return ResultCode::FAIL;

		if (!(_ToString(context, Data.QueueItemID)))
			return ResultCode::FAIL;

		if (!(StrUtil::StringCopyEx(context.StringBuffer, context.StringBufferLength, ")")))
			return ResultCode::FAIL;


		return ResultCode::SUCCESS;
	}
*/
	Result _ToString(ToStringContext& context, const MatchingPlayerInformation& Data)
	{
		if (!(StrUtil::StringCopyEx(context.StringBuffer, context.StringBufferLength, "(")))
			return ResultCode::FAIL;

		if (!(_ToString(context, Data.PlayerUID)))
			return ResultCode::FAIL;


		if (!(StrUtil::StringCopyEx(context.StringBuffer, context.StringBufferLength, ":")))
			return ResultCode::FAIL;

		if (!(_ToString(context, Data.PlayerID)))
			return ResultCode::FAIL;

		if (!(StrUtil::StringCopyEx(context.StringBuffer, context.StringBufferLength, ")")))
			return ResultCode::FAIL;

		return ResultCode::SUCCESS;
	}


	Result _ToString(ToStringContext& context, const PerformanceCounterInfo& Data)
	{
		if (!(StrUtil::StringCopyEx(context.StringBuffer, context.StringBufferLength, "(")))
			return ResultCode::FAIL;

		if (!(_ToString(context, Data.DateType)))
			return ResultCode::FAIL;


		if (!(StrUtil::StringCopyEx(context.StringBuffer, context.StringBufferLength, ":")))
			return ResultCode::FAIL;

		if (!(StrUtil::StringCopyEx(context.StringBuffer, context.StringBufferLength, Data.CounterName)))
			return ResultCode::FAIL;

		if (!(StrUtil::StringCopyEx(context.StringBuffer, context.StringBufferLength, ")")))
			return ResultCode::FAIL;

		return ResultCode::SUCCESS;
	}


	Result _ToString(ToStringContext& context, const PerformanceCounterInstanceInfo& Data)
	{
		if (!(StrUtil::StringCopyEx(context.StringBuffer, context.StringBufferLength, "(")))
			return ResultCode::FAIL;

		if (!(StrUtil::StringCopyEx(context.StringBuffer, context.StringBufferLength, Data.InstanceName)))
			return ResultCode::FAIL;


		if (!(StrUtil::StringCopyEx(context.StringBuffer, context.StringBufferLength, ":")))
			return ResultCode::FAIL;

		if (!(_ToString(context, Data.InstanceUID)))
			return ResultCode::FAIL;

		if (!(StrUtil::StringCopyEx(context.StringBuffer, context.StringBufferLength, ")")))
			return ResultCode::FAIL;

		return ResultCode::SUCCESS;
	}



	Result _ToString(ToStringContext& context, const ServerFriendInformation& Data)
	{
		if (!(_ToString(context, reinterpret_cast<const FriendInformation&>(Data))))
			return ResultCode::FAIL;

		if (!(StrUtil::StringCopyEx(context.StringBuffer, context.StringBufferLength, ":S")))
			return ResultCode::FAIL;

		if (!(_IToA(context, Data.ShardID)))
			return ResultCode::FAIL;

		return ResultCode::SUCCESS;
	}



};	// namespace SF


