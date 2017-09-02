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

	//Result ToString(char*& pBuff, INT& iBuffLen, const EntityID& Data, int Option)
	//{

	//}

	Result ToString(char*& pBuff, INT& iBuffLen, const EntityUID& Data, int Option)
	{
		if (!(ToString(pBuff, iBuffLen, Data.GetServerID(), Option)))
			return ResultCode::FAIL;

		if (!(StrUtil::StringCpyEx(pBuff, iBuffLen, ":")))
			return ResultCode::FAIL;

		if (!(ToString(pBuff, iBuffLen, Data.GetEntityID(), Option)))
			return ResultCode::FAIL;

		return ResultCode::SUCCESS;
	}

	Result ToString(char*& pBuff, INT& iBuffLen, const TransactionID& Data, int Option)
	{
		if (!(ToString(pBuff, iBuffLen, Data.GetEntityID(), Option)))
			return ResultCode::FAIL;

		if (!(StrUtil::StringCpyEx(pBuff, iBuffLen, ":")))
			return ResultCode::FAIL;

		if (!(_IToA((UINT32)Data.GetTransactionIndex(), pBuff, iBuffLen, 10, -1)))
			return ResultCode::FAIL;

		return ResultCode::SUCCESS;
	}

	Result ToString(char*& pBuff, INT& iBuffLen, const ClusterID& Data, int Option)
	{
		return ToString(pBuff, iBuffLen, (uint)Data, Option);
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



	Result ToString(char*& pBuff, INT& iBuffLen, const ClusterType& Data, int Option)
	{
		return ToString(pBuff, iBuffLen, (uint)Data, Option);
	}

	Result ToString(char*& pBuff, INT& iBuffLen, const ClusterMembership& Data, int Option)
	{
		return ToString(pBuff, iBuffLen, (uint)Data, Option);
	}

	Result ToString(char*& pBuff, INT& iBuffLen, const ServiceStatus& Data, int Option)
	{
		return ToString(pBuff, iBuffLen, (uint)Data, Option);
	}



	Result ToString(char*& pBuff, INT& iBuffLen, const LocalUID& Data, int Option)
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

	Result ToString(char*& pBuff, INT& iBuffLen, const ServiceInformation& Data, int Option)
	{
		unused(Option);

		if (!(StrUtil::StringCpyEx(pBuff, iBuffLen, "(")))
			return ResultCode::FAIL;

		if (!(ToString(pBuff, iBuffLen, Data.UID, Option)))
			return ResultCode::FAIL;

		if (!(StrUtil::StringCpyEx(pBuff, iBuffLen, ":C")))
			return ResultCode::FAIL;

		if (!(ToString(pBuff, iBuffLen, (uint)Data.ServerClass, Option)))
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


	Result ToString(char*& pBuff, INT& iBuffLen, const MatchingQueueTicket& Data, int Option)
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

	Result ToString(char*& pBuff, INT& iBuffLen, const MatchingPlayerInformation& Data, int Option)
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


	Result ToString(char*& pBuff, INT& iBuffLen, const PerformanceCounterInfo& Data, int Option)
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


	Result ToString(char*& pBuff, INT& iBuffLen, const PerformanceCounterInstanceInfo& Data, int Option)
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



	Result ToString(char*& pBuff, INT& iBuffLen, const ServerFriendInformation& Data, int Option)
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


	template Result ToStringArray(char*& pBuff, INT& iBuffLen, const Array<ServiceStatus>& Data, int Option);
	template Result ToStringArray(char*& pBuff, INT& iBuffLen, const Array<MatchingQueueTicket>& Data, int Option);
	template Result ToStringArray(char*& pBuff, INT& iBuffLen, const Array<MatchingPlayerInformation>& Data, int Option);
	template Result ToStringArray(char*& pBuff, INT& iBuffLen, const Array<PerformanceCounterInfo>& Data, int Option);
	template Result ToStringArray(char*& pBuff, INT& iBuffLen, const Array<PerformanceCounterInstanceInfo>& Data, int Option);


	template class ArgumentWrapperT < TransactionID>;
	template class ArgumentWrapperT < ClusterID>;
	template class ArgumentWrapperT < RouteContext>;
	template class ArgumentWrapperT < ClusterType>;
	template class ArgumentWrapperT < ServiceStatus>;
	template class ArgumentWrapperT < LocalUID>;
	template class ArgumentWrapperT < ServiceInformation>;
	template class ArgumentWrapperT < MatchingQueueTicket>;
	template class ArgumentWrapperT < MatchingPlayerInformation>;
	template class ArgumentWrapperT < PerformanceCounterInfo>;
	template class ArgumentWrapperT < PerformanceCounterInstanceInfo>;
	template class ArgumentWrapperT < ServerFriendInformation>;


	template class ArgArray < ServiceStatus>;
	template class ArgArray < MatchingQueueTicket>;
	template class ArgArray < MatchingPlayerInformation>;
	template class ArgArray < PerformanceCounterInfo>;
	template class ArgArray < PerformanceCounterInstanceInfo>;


};	// namespace SF


