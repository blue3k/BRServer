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
#include "Common/BrAssert.h"
#include "Common/StrUtil.h"
#include "Common/ToString.h"
#include "Common/ToStringSvr.h"



namespace BR
{


	template<>
	HRESULT ToString(char*& pBuff, INT& iBuffLen, const ClusterID& Data, int Option)
	{
		return ToString(pBuff, iBuffLen, (UINT)Data, Option);
	}


	template<>
	HRESULT ToString(char*& pBuff, INT& iBuffLen, const BR::RouteContext& Data, int Option)
	{
		Option;

		if (FAILED(StrUtil::StringCpyEx(pBuff, iBuffLen, "(")))
			return E_FAIL;

		if (FAILED(ToString(pBuff, iBuffLen, Data.From, Option)))
			return E_FAIL;

		if (FAILED(StrUtil::StringCpyEx(pBuff, iBuffLen, ":")))
			return E_FAIL;

		if (FAILED(ToString(pBuff, iBuffLen, Data.To, Option)))
			return E_FAIL;

		if (FAILED(StrUtil::StringCpyEx(pBuff, iBuffLen, ")")))
			return E_FAIL;

		return S_OK;
	}



	template<>
	HRESULT ToString(char*& pBuff, INT& iBuffLen, const ClusterType& Data, int Option)
	{
		return ToString(pBuff, iBuffLen, (UINT)Data, Option);
	}

	template<>
	HRESULT ToString(char*& pBuff, INT& iBuffLen, const ClusterMembership& Data, int Option)
	{
		return ToString(pBuff, iBuffLen, (UINT)Data, Option);
	}

	template<>
	HRESULT ToString(char*& pBuff, INT& iBuffLen, const ServiceStatus& Data, int Option)
	{
		return ToString(pBuff, iBuffLen, (UINT)Data, Option);
	}



	template<>
	HRESULT ToString(char*& pBuff, INT& iBuffLen, const LocalUID& Data, int Option)
	{
		Option;
		if (FAILED(_IToA((UINT32)Data.Time, pBuff, iBuffLen, 10, -1)))
			return E_FAIL;

		if (FAILED(StrUtil::StringCpyEx(pBuff, iBuffLen, ":")))
			return E_FAIL;

		if (FAILED(_IToA((UINT32)Data.ID, pBuff, iBuffLen, 10, -1)))
			return E_FAIL;

		return S_OK;
	}

	template<>
	HRESULT ToString(char*& pBuff, INT& iBuffLen, const BR::ServiceInformation& Data, int Option)
	{
		Option;

		if (FAILED(StrUtil::StringCpyEx(pBuff, iBuffLen, "(")))
			return E_FAIL;

		if (FAILED(ToString(pBuff, iBuffLen, Data.UID, Option)))
			return E_FAIL;

		if (FAILED(StrUtil::StringCpyEx(pBuff, iBuffLen, ":C")))
			return E_FAIL;

		if (FAILED(ToString(pBuff, iBuffLen, (UINT)Data.ServerClass, Option)))
			return E_FAIL;

		if (FAILED(StrUtil::StringCpyEx(pBuff, iBuffLen, ":M")))
			return E_FAIL;

		if (FAILED(ToString(pBuff, iBuffLen, Data.Membership, Option)))
			return E_FAIL;

		if (FAILED(StrUtil::StringCpyEx(pBuff, iBuffLen, ":S")))
			return E_FAIL;

		if (FAILED(ToString(pBuff, iBuffLen, Data.Status, Option)))
			return E_FAIL;

		if (FAILED(StrUtil::StringCpyEx(pBuff, iBuffLen, ":T")))
			return E_FAIL;

		if (FAILED(ToString(pBuff, iBuffLen, Data.ServerUpTime.time_since_epoch().count(), Option)))
			return E_FAIL;

		if (FAILED(StrUtil::StringCpyEx(pBuff, iBuffLen, ":W")))
			return E_FAIL;

		if (FAILED(ToString(pBuff, iBuffLen, Data.Workload, Option)))
			return E_FAIL;

		if (FAILED(StrUtil::StringCpyEx(pBuff, iBuffLen, ")")))
			return E_FAIL;

		return S_OK;
	}


	template<>
	HRESULT ToString(char*& pBuff, INT& iBuffLen, const MatchingQueueTicket& Data, int Option)
	{
		Option;

		if (FAILED(StrUtil::StringCpyEx(pBuff, iBuffLen, "(")))
			return E_FAIL;

		if (FAILED(ToString(pBuff, iBuffLen, Data.QueueUID, Option)))
			return E_FAIL;


		if (FAILED(StrUtil::StringCpyEx(pBuff, iBuffLen, ":")))
			return E_FAIL;

		if (FAILED(ToString(pBuff, iBuffLen, Data.QueueItemID, Option)))
			return E_FAIL;

		if (FAILED(StrUtil::StringCpyEx(pBuff, iBuffLen, ")")))
			return E_FAIL;


		return S_OK;
	}

	template<>
	HRESULT ToString(char*& pBuff, INT& iBuffLen, const MatchingPlayerInformation& Data, int Option)
	{
		Option;
		if (FAILED(StrUtil::StringCpyEx(pBuff, iBuffLen, "(")))
			return E_FAIL;

		if (FAILED(ToString(pBuff, iBuffLen, Data.PlayerUID, Option)))
			return E_FAIL;


		if (FAILED(StrUtil::StringCpyEx(pBuff, iBuffLen, ":")))
			return E_FAIL;

		if (FAILED(ToString(pBuff, iBuffLen, Data.PlayerID, Option)))
			return E_FAIL;

		if (FAILED(StrUtil::StringCpyEx(pBuff, iBuffLen, ")")))
			return E_FAIL;

		return S_OK;
	}


	template<>
	HRESULT ToString(char*& pBuff, INT& iBuffLen, const PerformanceCounterInfo& Data, int Option)
	{
		Option;
		if (FAILED(StrUtil::StringCpyEx(pBuff, iBuffLen, "(")))
			return E_FAIL;

		if (FAILED(ToString(pBuff, iBuffLen, Data.DateType, Option)))
			return E_FAIL;


		if (FAILED(StrUtil::StringCpyEx(pBuff, iBuffLen, ":")))
			return E_FAIL;

		if (FAILED(StrUtil::StringCpyEx(pBuff, iBuffLen, Data.CounterName)))
			return E_FAIL;

		if (FAILED(StrUtil::StringCpyEx(pBuff, iBuffLen, ")")))
			return E_FAIL;

		return S_OK;
	}


	template<>
	HRESULT ToString(char*& pBuff, INT& iBuffLen, const PerformanceCounterInstanceInfo& Data, int Option)
	{
		Option;
		if (FAILED(StrUtil::StringCpyEx(pBuff, iBuffLen, "(")))
			return E_FAIL;

		if (FAILED(StrUtil::StringCpyEx(pBuff, iBuffLen, Data.InstanceName)))
			return E_FAIL;


		if (FAILED(StrUtil::StringCpyEx(pBuff, iBuffLen, ":")))
			return E_FAIL;

		if (FAILED(ToString(pBuff, iBuffLen, Data.InstanceUID, Option)))
			return E_FAIL;

		if (FAILED(StrUtil::StringCpyEx(pBuff, iBuffLen, ")")))
			return E_FAIL;

		return S_OK;
	}



	template<>
	HRESULT ToString(char*& pBuff, INT& iBuffLen, const ServerFriendInformation& Data, int Option)
	{
		Option;

		if (FAILED(ToString(pBuff, iBuffLen, reinterpret_cast<const FriendInformation&>(Data), Option)))
			return E_FAIL;

		if (FAILED(StrUtil::StringCpyEx(pBuff, iBuffLen, ":S")))
			return E_FAIL;

		if (FAILED(ToString(pBuff, iBuffLen, Data.ShardID, Option)))
			return E_FAIL;

		return S_OK;
	}


	template HRESULT ToStringArray(char*& pBuff, INT& iBuffLen, const Array<ServiceStatus>& Data, int Option);
	template HRESULT ToStringArray(char*& pBuff, INT& iBuffLen, const Array<MatchingQueueTicket>& Data, int Option);
	template HRESULT ToStringArray(char*& pBuff, INT& iBuffLen, const Array<MatchingPlayerInformation>& Data, int Option);
	template HRESULT ToStringArray(char*& pBuff, INT& iBuffLen, const Array<PerformanceCounterInfo>& Data, int Option);
	template HRESULT ToStringArray(char*& pBuff, INT& iBuffLen, const Array<PerformanceCounterInstanceInfo>& Data, int Option);


	template class Arg < ClusterID>;
	template class Arg < RouteContext>;
	template class Arg < ClusterType>;
	template class Arg < ServiceStatus>;
	template class Arg < LocalUID>;
	template class Arg < ServiceInformation>;
	template class Arg < MatchingQueueTicket>;
	template class Arg < MatchingPlayerInformation>;
	template class Arg < PerformanceCounterInfo>;
	template class Arg < PerformanceCounterInstanceInfo>;
	template class Arg < ServerFriendInformation>;


	template class ArgArray < ServiceStatus>;
	template class ArgArray < MatchingQueueTicket>;
	template class ArgArray < MatchingPlayerInformation>;
	template class ArgArray < PerformanceCounterInfo>;
	template class ArgArray < PerformanceCounterInstanceInfo>;


};	// namespace BR


