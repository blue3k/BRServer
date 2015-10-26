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
	HRESULT ToString(char*& pBuff, INT& iBuffLen, const EntityID& Data, int Option)
	{
		Option;
		if (FAILED(ToStringEnum(pBuff, iBuffLen, (EntityFaculty)Data.FacultyID)))
			return E_FAIL;

		if (FAILED(StrUtil::StringCpyEx(pBuff, iBuffLen, ":")))
			return E_FAIL;

		if (FAILED(_IToA((UINT32)Data.EntityLID, pBuff, iBuffLen, 10, -1)))
			return E_FAIL;

		return S_OK;
	}



	template<>
	HRESULT ToString(char*& pBuff, INT& iBuffLen, const EntityUID& Data, int Option)
	{
		Option;
		if (FAILED(_IToA((UINT32)Data.SvrID, pBuff, iBuffLen, 10, -1)))
			return E_FAIL;

		if (FAILED(StrUtil::StringCpyEx(pBuff, iBuffLen, ":")))
			return E_FAIL;

		if (FAILED(ToString(pBuff, iBuffLen, Data.EntityID, Option)))
			return E_FAIL;

		return S_OK;
	}



	template<>
	HRESULT ToString(char*& pBuff, INT& iBuffLen, const Message::MessageID& Data, int Option)
	{
		Option;
		if (FAILED(ToStringEnum(pBuff, iBuffLen, (BR::PolicyID)Data.IDs.Policy)))
			return E_FAIL;

		if (FAILED(StrUtil::StringCpyEx(pBuff, iBuffLen, ":")))
			return E_FAIL;

		if (FAILED(ToStringEnum(pBuff, iBuffLen, Data.IDs.MsgCode)))
			return E_FAIL;

		if (FAILED(StrUtil::StringCpyEx(pBuff, iBuffLen, ":Seq")))
			return E_FAIL;

		if (FAILED(ToStringEnum(pBuff, iBuffLen, Data.IDs.Sequence)))
			return E_FAIL;

		return S_OK;
	}

	template<>
	HRESULT ToString(char*& pBuff, INT& iBuffLen, const NetAddress& Data, int Option)
	{
		Option;
		if (FAILED(ToString(pBuff, iBuffLen, (LPCSTR)Data.strAddr, 0)))
			return E_FAIL;

		if (FAILED(StrUtil::StringCpyEx(pBuff, iBuffLen, ":")))
			return E_FAIL;

		if (FAILED(_IToA((UINT32)Data.usPort, pBuff, iBuffLen, 10, -1)))
			return E_FAIL;

		return S_OK;
	}


	template<>
	HRESULT ToString(char*& pBuff, INT& iBuffLen, const BR::GameID& Data, int Option)
	{
		Option;

		if (FAILED(_IToA((UINT32)Data, pBuff, iBuffLen, 10, -1)))
			return E_FAIL;

		return S_OK;
	}


	template<>
	HRESULT ToString(char*& pBuff, INT& iBuffLen, const BR::PlayerInformation& Data, int Option)
	{
		Option;

		if (FAILED(StrUtil::StringCpyEx(pBuff, iBuffLen, "(")))
			return E_FAIL;

		if (FAILED(_IToA((UINT64)Data.PlayerID, pBuff, iBuffLen, 10, -1)))
			return E_FAIL;

		if (FAILED(StrUtil::StringCpyEx(pBuff, iBuffLen, ":")))
			return E_FAIL;

		if (FAILED(_IToA((UINT64)Data.FacebookUID, pBuff, iBuffLen, 10, -1)))
			return E_FAIL;

		if (FAILED(StrUtil::StringCpyEx(pBuff, iBuffLen, ")")))
			return E_FAIL;

		return S_OK;
	}


	template<>
	HRESULT ToString(char*& pBuff, INT& iBuffLen, const BR::FriendInformation& Data, int Option)
	{
		Option;

		if (FAILED(StrUtil::StringCpyEx(pBuff, iBuffLen, "(")))
			return E_FAIL;

		if (FAILED(_IToA((UINT64)Data.PlayerID, pBuff, iBuffLen, 10, -1)))
			return E_FAIL;

		if (FAILED(StrUtil::StringCpyEx(pBuff, iBuffLen, ":")))
			return E_FAIL;

		if (FAILED(_IToA((UINT64)Data.FacebookUID, pBuff, iBuffLen, 10, -1)))
			return E_FAIL;

		if (FAILED(StrUtil::StringCpyEx(pBuff, iBuffLen, ")")))
			return E_FAIL;

		return S_OK;
	}

	template<>
	HRESULT ToString(char*& pBuff, INT& iBuffLen, const BR::TotalRankingPlayerInformation& Data, int Option)
	{
		Option;

		if (FAILED(StrUtil::StringCpyEx(pBuff, iBuffLen, "(")))
			return E_FAIL;

		if (FAILED(_IToA((UINT64)Data.PlayerID, pBuff, iBuffLen, 10, -1)))
			return E_FAIL;

		if (FAILED(StrUtil::StringCpyEx(pBuff, iBuffLen, ":")))
			return E_FAIL;

		if (FAILED(_IToA((UINT64)Data.FacebookUID, pBuff, iBuffLen, 10, -1)))
			return E_FAIL;

		if (FAILED(StrUtil::StringCpyEx(pBuff, iBuffLen, ")")))
			return E_FAIL;

		return S_OK;
	}


	template<>
	HRESULT ToString(char*& pBuff, INT& iBuffLen, const NotificationType& Data, int Option)
	{
		return ToString(pBuff, iBuffLen, (UINT)Data, Option);
	}



};	// namespace BR


