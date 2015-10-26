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
#include "Common/ToStringGame.h"
#include "Common/Argument.h"



namespace BR
{
	template<>
	HRESULT ToString(char*& pBuff, INT& iBuffLen, const PlayerRole& Data, int Option)
	{
		return ToString(pBuff, iBuffLen, (UINT)Data, Option);
	}
	template class Arg < PlayerRole>;

	template<>
	HRESULT ToString(char*& pBuff, INT& iBuffLen, const GameStateID& Data, int Option)
	{
		return ToString(pBuff, iBuffLen, (UINT)Data, Option);
	}
	template class Arg < GameStateID>;

	template<>
	HRESULT ToString(char*& pBuff, INT& iBuffLen, const GameWinner& Data, int Option)
	{
		return ToString(pBuff, iBuffLen, (UINT)Data, Option);
	}
	template class Arg < GameWinner>;

	template<>
	HRESULT ToString(char*& pBuff, INT& iBuffLen, const PlayerKilledReason& Data, int Option)
	{
		return ToString(pBuff, iBuffLen, (UINT)Data, Option);
	}
	template class Arg < PlayerKilledReason>;

	template<>
	HRESULT ToString(char*& pBuff, INT& iBuffLen, const PlayerRevealedReason& Data, int Option)
	{
		return ToString(pBuff, iBuffLen, (UINT)Data, Option);
	}
	template class Arg < PlayerRevealedReason>;

	template<>
	HRESULT ToString(char*& pBuff, INT& iBuffLen, const RankingType& Data, int Option)
	{
		return ToString(pBuff, iBuffLen, (UINT)Data, Option);
	}
	template class Arg < RankingType>;


};	// namespace BR


