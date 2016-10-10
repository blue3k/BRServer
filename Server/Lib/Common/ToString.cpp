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



namespace BR
{
	

	template<>
	Result ToString(char*& pBuff, INT& iBuffLen, const EntityID& Data, int Option)
	{
		unused(Option);
		if (FAILED(ToStringEnum(pBuff, iBuffLen, (EntityFaculty)Data.Components.FacultyID)))
			return ResultCode::FAIL;

		if (FAILED(StrUtil::StringCpyEx(pBuff, iBuffLen, ":")))
			return ResultCode::FAIL;

		if (FAILED(_IToA((UINT32)Data.Components.EntityLID, pBuff, iBuffLen, 10, -1)))
			return ResultCode::FAIL;

		return ResultCode::SUCCESS;
	}



	template<>
	Result ToString(char*& pBuff, INT& iBuffLen, const EntityUID& Data, int Option)
	{
		unused(Option);
		if (FAILED(_IToA((UINT32)Data.Components.SvrID, pBuff, iBuffLen, 10, -1)))
			return ResultCode::FAIL;

		if (FAILED(StrUtil::StringCpyEx(pBuff, iBuffLen, ":")))
			return ResultCode::FAIL;

		if (FAILED(ToString(pBuff, iBuffLen, Data.Components.EntID, Option)))
			return ResultCode::FAIL;

		return ResultCode::SUCCESS;
	}



	template<>
	Result ToString(char*& pBuff, INT& iBuffLen, const Message::MessageID& Data, int Option)
	{
		unused(Option);
		if (FAILED(ToStringEnum(pBuff, iBuffLen, (BR::PolicyID)Data.IDs.Policy)))
			return ResultCode::FAIL;

		if (FAILED(StrUtil::StringCpyEx(pBuff, iBuffLen, ":")))
			return ResultCode::FAIL;

		if (FAILED(ToStringEnum(pBuff, iBuffLen, Data.IDs.MsgCode)))
			return ResultCode::FAIL;

		if (FAILED(StrUtil::StringCpyEx(pBuff, iBuffLen, ":Seq")))
			return ResultCode::FAIL;

		if (FAILED(ToStringEnum(pBuff, iBuffLen, Data.IDs.Sequence)))
			return ResultCode::FAIL;

		return ResultCode::SUCCESS;
	}

	template<>
	Result ToString(char*& pBuff, INT& iBuffLen, const NetAddress& Data, int Option)
	{
		unused(Option);
		if (FAILED(ToString(pBuff, iBuffLen, (LPCSTR)Data.strAddr, 0)))
			return ResultCode::FAIL;

		if (FAILED(StrUtil::StringCpyEx(pBuff, iBuffLen, ":")))
			return ResultCode::FAIL;

		if (FAILED(_IToA((UINT32)Data.usPort, pBuff, iBuffLen, 10, -1)))
			return ResultCode::FAIL;

		return ResultCode::SUCCESS;
	}


	template<>
	Result ToString(char*& pBuff, INT& iBuffLen, const GameID& Data, int Option)
	{
		unused(Option);

		if (FAILED(_IToA((UINT32)Data, pBuff, iBuffLen, 10, -1)))
			return ResultCode::FAIL;

		return ResultCode::SUCCESS;
	}


	template<>
	Result ToString(char*& pBuff, INT& iBuffLen, const PlayerInformation& Data, int Option)
	{
		unused(Option);

		if (FAILED(StrUtil::StringCpyEx(pBuff, iBuffLen, "(")))
			return ResultCode::FAIL;

		if (FAILED(_IToA((UINT64)Data.PlayerID, pBuff, iBuffLen, 10, -1)))
			return ResultCode::FAIL;

		if (FAILED(StrUtil::StringCpyEx(pBuff, iBuffLen, ":")))
			return ResultCode::FAIL;

		if (FAILED(_IToA((UINT64)Data.FBUID, pBuff, iBuffLen, 10, -1)))
			return ResultCode::FAIL;

		if (FAILED(StrUtil::StringCpyEx(pBuff, iBuffLen, ")")))
			return ResultCode::FAIL;

		return ResultCode::SUCCESS;
	}


	template<>
	Result ToString(char*& pBuff, INT& iBuffLen, const FriendInformation& Data, int Option)
	{
		unused(Option);

		if (FAILED(StrUtil::StringCpyEx(pBuff, iBuffLen, "(")))
			return ResultCode::FAIL;

		if (FAILED(_IToA((UINT64)Data.PlayerID, pBuff, iBuffLen, 10, -1)))
			return ResultCode::FAIL;

		if (FAILED(StrUtil::StringCpyEx(pBuff, iBuffLen, ":")))
			return ResultCode::FAIL;

		if (FAILED(_IToA((UINT64)Data.FBUID, pBuff, iBuffLen, 10, -1)))
			return ResultCode::FAIL;

		if (FAILED(StrUtil::StringCpyEx(pBuff, iBuffLen, ")")))
			return ResultCode::FAIL;

		return ResultCode::SUCCESS;
	}

	template<>
	Result ToString(char*& pBuff, INT& iBuffLen, const TotalRankingPlayerInformation& Data, int Option)
	{
		unused(Option);

		if (FAILED(StrUtil::StringCpyEx(pBuff, iBuffLen, "(")))
			return ResultCode::FAIL;

		if (FAILED(_IToA((UINT64)Data.PlayerID, pBuff, iBuffLen, 10, -1)))
			return ResultCode::FAIL;

		if (FAILED(StrUtil::StringCpyEx(pBuff, iBuffLen, ":")))
			return ResultCode::FAIL;

		if (FAILED(_IToA((UINT64)Data.FBUID, pBuff, iBuffLen, 10, -1)))
			return ResultCode::FAIL;

		if (FAILED(StrUtil::StringCpyEx(pBuff, iBuffLen, ")")))
			return ResultCode::FAIL;

		return ResultCode::SUCCESS;
	}


	template<>
	Result ToString(char*& pBuff, INT& iBuffLen, const NotificationType& Data, int Option)
	{
		return ToString(pBuff, iBuffLen, (UINT)Data, Option);
	}


	template<>
	Result ToString(char*& pBuff, INT& iBuffLen, const LinkedArray<PlayerID>& Data, int Option)
	{
		return ToStringArray(pBuff, iBuffLen, Data, Option);
	}

	template<>
	Result ToString(char*& pBuff, INT& iBuffLen, const LinkedArray<TotalRankingPlayerInformation>& Data, int Option)
	{
		return ResultCode::SUCCESS;// TODO: ToStringArray(pBuff, iBuffLen, Data, Option);
	}

	template<>
	Result ToString(char*& pBuff, INT& iBuffLen, const LinkedArray<FriendInformation>& Data, int Option)
	{
		return ToStringArray(pBuff, iBuffLen, Data, Option);
	}

	template<>
	Result ToString(char*& pBuff, INT& iBuffLen, const TimeStampMS& Data, int Option)
	{
		return ToString(pBuff, iBuffLen, Data.time_since_epoch().count(), Option);
	}

	template<>
	Result ToString(char*& pBuff, INT& iBuffLen, const DurationMS& Data, int Option)
	{
		return ToString(pBuff, iBuffLen, Data.count(), Option);
	}

	template<>
	Result ToString(char*& pBuff, INT& iBuffLen, const TimeStampSec& Data, int Option)
	{
		return ToString(pBuff, iBuffLen, Data.time_since_epoch().count(), Option);
	}

	template<>
	Result ToString(char*& pBuff, INT& iBuffLen, const DurationSec& Data, int Option)
	{
		return ToString(pBuff, iBuffLen, Data.count(), Option);
	}


};	// namespace BR


