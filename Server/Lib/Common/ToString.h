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
#include "Common/BrAssert.h"
#include "Common/BrBaseTypes.h"
#include "Common/ToStringBase.h"
#include "Common/BrCommonTypes.h"
#include "Common/Message.h"


namespace BR {


	template<> HRESULT ToString(char*& pBuff, INT& iBuffLen, const EntityID& Data, int Option);
	template<> HRESULT ToString(char*& pBuff, INT& iBuffLen, const EntityUID& Data, int Option);
	template<> HRESULT ToString(char*& pBuff, INT& iBuffLen, const Message::MessageID& Data, int Option);
	template<> HRESULT ToString(char*& pBuff, INT& iBuffLen, const NetAddress& Data, int Option);
	template<> HRESULT ToString(char*& pBuff, INT& iBuffLen, const GameID& Data, int Option);
	template<> HRESULT ToString(char*& pBuff, INT& iBuffLen, const PlayerInformation& Data, int Option);
	template<> HRESULT ToString(char*& pBuff, INT& iBuffLen, const FriendInformation& Data, int Option);
	template<> HRESULT ToString(char*& pBuff, INT& iBuffLen, const TotalRankingPlayerInformation& Data, int Option);
	template<> HRESULT ToString(char*& pBuff, INT& iBuffLen, const NotificationType& Data, int Option);
	template<> HRESULT ToString(char*& pBuff, INT& iBuffLen, const LinkedArray<PlayerID>& Data, int Option);
	template<> HRESULT ToString(char*& pBuff, INT& iBuffLen, const LinkedArray<TotalRankingPlayerInformation>& Data, int Option);
	template<> HRESULT ToString(char*& pBuff, INT& iBuffLen, const LinkedArray<FriendInformation>& Data, int Option);
	template<> HRESULT ToString(char*& pBuff, INT& iBuffLen, const TimeStampMS& Data, int Option);
	template<> HRESULT ToString(char*& pBuff, INT& iBuffLen, const DurationMS& Data, int Option);
	template<> HRESULT ToString(char*& pBuff, INT& iBuffLen, const TimeStampSec& Data, int Option);
	template<> HRESULT ToString(char*& pBuff, INT& iBuffLen, const DurationSec& Data, int Option);


}; // namespace StrUtil



