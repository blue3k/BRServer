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

#include "Common/Typedefs.h"
#include "Common/BrGameTypes.h"
#include "Common/ToStringBase.h"
#include "Common/BrCommonTypes.h"
#include "Common/Message.h"
#include "Common/Argument.h"


namespace BR {

	extern template HRESULT ToString(char*& pBuff, INT& iBuffLen, const PlayerRole& Data, int Option);
	extern template HRESULT ToString(char*& pBuff, INT& iBuffLen, const GameStateID& Data, int Option);
	extern template HRESULT ToString(char*& pBuff, INT& iBuffLen, const GameWinner& Data, int Option);
	extern template HRESULT ToString(char*& pBuff, INT& iBuffLen, const PlayerKilledReason& Data, int Option);
	extern template HRESULT ToString(char*& pBuff, INT& iBuffLen, const PlayerRevealedReason& Data, int Option);
	extern template HRESULT ToString(char*& pBuff, INT& iBuffLen, const RankingType& Data, int Option);



	extern template class Arg < PlayerRole>;
	extern template class Arg < GameStateID>;
	extern template class Arg < GameWinner>;
	extern template class Arg < PlayerKilledReason>;
	extern template class Arg < PlayerRevealedReason>;
	extern template class Arg < RankingType>;



}; // namespace StrUtil


