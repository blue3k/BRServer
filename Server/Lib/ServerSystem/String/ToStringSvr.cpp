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


#include "stdafx.h"
#include "SFAssert.h"
#include "String/StrUtil.h"
#include "String/ToString.h"
#include "String/ToStringSvr.h"



namespace SF
{

	//Result ToString(char*& pBuff, INT& iBuffLen, const BR::EntityID& Data, int Option)
	//{

	//}

	Result ToString(char*& pBuff, INT& iBuffLen, const BR::EntityUID& Data, int Option)
	{
		if (!(ToString(pBuff, iBuffLen, Data.GetServerID(), Option)))
			return ResultCode::FAIL;

		if (!(StrUtil::StringCpyEx(pBuff, iBuffLen, ":")))
			return ResultCode::FAIL;

		if (!(ToString(pBuff, iBuffLen, Data.GetEntityID(), Option)))
			return ResultCode::FAIL;

		return ResultCode::SUCCESS;
	}

	Result ToString(char*& pBuff, INT& iBuffLen, const BR::TransactionID& Data, int Option)
	{
		if (!(ToString(pBuff, iBuffLen, Data.GetEntityID(), Option)))
			return ResultCode::FAIL;

		if (!(StrUtil::StringCpyEx(pBuff, iBuffLen, ":")))
			return ResultCode::FAIL;

		if (!(_IToA((UINT32)Data.GetTransactionIndex(), pBuff, iBuffLen, 10, -1)))
			return ResultCode::FAIL;

		return ResultCode::SUCCESS;
	}

	Result ToString(char*& pBuff, INT& iBuffLen, const BR::ClusterID& Data, int Option)
	{
		return ToString(pBuff, iBuffLen, (UINT)Data, Option);
	}


	Result ToString(char*& pBuff, INT& iBuffLen, const RouteContext& Data, int Option)
	{
		unused(Option);

		if (!(StrUtil::StringCpyEx(pBuff, iBuffLen, "(")))
			return ResultCode::FAIL;

		if (!(ToString(pBuff, iBuffLen, Data.Components.From, Option)))
			return ResultCode::FAIL;

		if (!(StrUtil::StringCpyEx(pBuff, iBuffLen, ":")))
			return ResultCode::FAIL;

		if (!(ToString(pBuff, iBuffLen, Data.Components.To, Option)))
			return ResultCode::FAIL;

		if (!(StrUtil::StringCpyEx(pBuff, iBuffLen, ")")))
			return ResultCode::FAIL;

		return ResultCode::SUCCESS;
	}



	Result ToString(char*& pBuff, INT& iBuffLen, const BR::ClusterType& Data, int Option)
	{
		return ToString(pBuff, iBuffLen, (UINT)Data, Option);
	}

	Result ToString(char*& pBuff, INT& iBuffLen, const BR::ClusterMembership& Data, int Option)
	{
		return ToString(pBuff, iBuffLen, (UINT)Data, Option);
	}

	Result ToString(char*& pBuff, INT& iBuffLen, const BR::ServiceStatus& Data, int Option)
	{
		return ToString(pBuff, iBuffLen, (UINT)Data, Option);
	}



	Result ToString(char*& pBuff, INT& iBuffLen, const BR::LocalUID& Data, int Option)
	{
		unused(Option);
		if (!(_IToA((UINT32)Data.Time, pBuff, iBuffLen, 10, -1)))
			return ResultCode::FAIL;

		if (!(StrUtil::StringCpyEx(pBuff, iBuffLen, ":")))
			return ResultCode::FAIL;

		if (!(_IToA((UINT32)Data.ID, pBuff, iBuffLen, 10, -1)))
			return ResultCode::FAIL;

		return ResultCode::SUCCESS;
	}

	Result ToString(char*& pBuff, INT& iBuffLen, const BR::ServiceInformation& Data, int Option)
	{
		unused(Option);

		if (!(StrUtil::StringCpyEx(pBuff, iBuffLen, "(")))
			return ResultCode::FAIL;

		if (!(ToString(pBuff, iBuffLen, Data.UID, Option)))
			return ResultCode::FAIL;

		if (!(StrUtil::StringCpyEx(pBuff, iBuffLen, ":C")))
			return ResultCode::FAIL;

		if (!(ToString(pBuff, iBuffLen, (UINT)Data.ServerClass, Option)))
			return ResultCode::FAIL;

		if (!(StrUtil::StringCpyEx(pBuff, iBuffLen, ":M")))
			return ResultCode::FAIL;

		if (!(ToString(pBuff, iBuffLen, Data.Membership, Option)))
			return ResultCode::FAIL;

		if (!(StrUtil::StringCpyEx(pBuff, iBuffLen, ":S")))
			return ResultCode::FAIL;

		if (!(ToString(pBuff, iBuffLen, Data.Status, Option)))
			return ResultCode::FAIL;

		if (!(StrUtil::StringCpyEx(pBuff, iBuffLen, ":T")))
			return ResultCode::FAIL;

		if (!(ToString(pBuff, iBuffLen, Data.ServerUpTime.time_since_epoch().count(), Option)))
			return ResultCode::FAIL;

		if (!(StrUtil::StringCpyEx(pBuff, iBuffLen, ":W")))
			return ResultCode::FAIL;

		if (!(ToString(pBuff, iBuffLen, Data.Workload, Option)))
			return ResultCode::FAIL;

		if (!(StrUtil::StringCpyEx(pBuff, iBuffLen, ")")))
			return ResultCode::FAIL;

		return ResultCode::SUCCESS;
	}


	Result ToString(char*& pBuff, INT& iBuffLen, const BR::MatchingQueueTicket& Data, int Option)
	{
		unused(Option);

		if (!(StrUtil::StringCpyEx(pBuff, iBuffLen, "(")))
			return ResultCode::FAIL;

		if (!(ToString(pBuff, iBuffLen, Data.QueueUID, Option)))
			return ResultCode::FAIL;


		if (!(StrUtil::StringCpyEx(pBuff, iBuffLen, ":")))
			return ResultCode::FAIL;

		if (!(ToString(pBuff, iBuffLen, Data.QueueItemID, Option)))
			return ResultCode::FAIL;

		if (!(StrUtil::StringCpyEx(pBuff, iBuffLen, ")")))
			return ResultCode::FAIL;


		return ResultCode::SUCCESS;
	}

	Result ToString(char*& pBuff, INT& iBuffLen, const BR::MatchingPlayerInformation& Data, int Option)
	{
		unused(Option);
		if (!(StrUtil::StringCpyEx(pBuff, iBuffLen, "(")))
			return ResultCode::FAIL;

		if (!(ToString(pBuff, iBuffLen, Data.PlayerUID, Option)))
			return ResultCode::FAIL;


		if (!(StrUtil::StringCpyEx(pBuff, iBuffLen, ":")))
			return ResultCode::FAIL;

		if (!(ToString(pBuff, iBuffLen, Data.PlayerID, Option)))
			return ResultCode::FAIL;

		if (!(StrUtil::StringCpyEx(pBuff, iBuffLen, ")")))
			return ResultCode::FAIL;

		return ResultCode::SUCCESS;
	}


	Result ToString(char*& pBuff, INT& iBuffLen, const BR::PerformanceCounterInfo& Data, int Option)
	{
		unused(Option);
		if (!(StrUtil::StringCpyEx(pBuff, iBuffLen, "(")))
			return ResultCode::FAIL;

		if (!(ToString(pBuff, iBuffLen, Data.DateType, Option)))
			return ResultCode::FAIL;


		if (!(StrUtil::StringCpyEx(pBuff, iBuffLen, ":")))
			return ResultCode::FAIL;

		if (!(StrUtil::StringCpyEx(pBuff, iBuffLen, Data.CounterName)))
			return ResultCode::FAIL;

		if (!(StrUtil::StringCpyEx(pBuff, iBuffLen, ")")))
			return ResultCode::FAIL;

		return ResultCode::SUCCESS;
	}


	Result ToString(char*& pBuff, INT& iBuffLen, const BR::PerformanceCounterInstanceInfo& Data, int Option)
	{
		unused(Option);
		if (!(StrUtil::StringCpyEx(pBuff, iBuffLen, "(")))
			return ResultCode::FAIL;

		if (!(StrUtil::StringCpyEx(pBuff, iBuffLen, Data.InstanceName)))
			return ResultCode::FAIL;


		if (!(StrUtil::StringCpyEx(pBuff, iBuffLen, ":")))
			return ResultCode::FAIL;

		if (!(ToString(pBuff, iBuffLen, Data.InstanceUID, Option)))
			return ResultCode::FAIL;

		if (!(StrUtil::StringCpyEx(pBuff, iBuffLen, ")")))
			return ResultCode::FAIL;

		return ResultCode::SUCCESS;
	}



	Result ToString(char*& pBuff, INT& iBuffLen, const BR::ServerFriendInformation& Data, int Option)
	{
		unused(Option);

		if (!(ToString(pBuff, iBuffLen, reinterpret_cast<const FriendInformation&>(Data), Option)))
			return ResultCode::FAIL;

		if (!(StrUtil::StringCpyEx(pBuff, iBuffLen, ":S")))
			return ResultCode::FAIL;

		if (!(ToString(pBuff, iBuffLen, Data.ShardID, Option)))
			return ResultCode::FAIL;

		return ResultCode::SUCCESS;
	}


	template Result ToStringArray(char*& pBuff, INT& iBuffLen, const Array<BR::ServiceStatus>& Data, int Option);
	template Result ToStringArray(char*& pBuff, INT& iBuffLen, const Array<BR::MatchingQueueTicket>& Data, int Option);
	template Result ToStringArray(char*& pBuff, INT& iBuffLen, const Array<BR::MatchingPlayerInformation>& Data, int Option);
	template Result ToStringArray(char*& pBuff, INT& iBuffLen, const Array<BR::PerformanceCounterInfo>& Data, int Option);
	template Result ToStringArray(char*& pBuff, INT& iBuffLen, const Array<BR::PerformanceCounterInstanceInfo>& Data, int Option);


	template class ArgumentWrapperT < BR::TransactionID>;
	template class ArgumentWrapperT < BR::ClusterID>;
	template class ArgumentWrapperT < BR::RouteContext>;
	template class ArgumentWrapperT < BR::ClusterType>;
	template class ArgumentWrapperT < BR::ServiceStatus>;
	template class ArgumentWrapperT < BR::LocalUID>;
	template class ArgumentWrapperT < BR::ServiceInformation>;
	template class ArgumentWrapperT < BR::MatchingQueueTicket>;
	template class ArgumentWrapperT < BR::MatchingPlayerInformation>;
	template class ArgumentWrapperT < BR::PerformanceCounterInfo>;
	template class ArgumentWrapperT < BR::PerformanceCounterInstanceInfo>;
	template class ArgumentWrapperT < BR::ServerFriendInformation>;


	template class ArgArray < BR::ServiceStatus>;
	template class ArgArray < BR::MatchingQueueTicket>;
	template class ArgArray < BR::MatchingPlayerInformation>;
	template class ArgArray < BR::PerformanceCounterInfo>;
	template class ArgArray < BR::PerformanceCounterInstanceInfo>;


};	// namespace BR


