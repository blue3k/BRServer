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


	extern template HRESULT ToString(char*& pBuff, INT& iBuffLen, const EntityID& Data, int Option);
	extern template HRESULT ToString(char*& pBuff, INT& iBuffLen, const EntityUID& Data, int Option);
	extern template HRESULT ToString(char*& pBuff, INT& iBuffLen, const Message::MessageID& Data, int Option);
	extern template HRESULT ToString(char*& pBuff, INT& iBuffLen, const NetAddress& Data, int Option);
	extern template HRESULT ToString(char*& pBuff, INT& iBuffLen, const BR::GameID& Data, int Option);
	extern template HRESULT ToString(char*& pBuff, INT& iBuffLen, const BR::PlayerInformation& Data, int Option);
	extern template HRESULT ToString(char*& pBuff, INT& iBuffLen, const BR::FriendInformation& Data, int Option);
	extern template HRESULT ToString(char*& pBuff, INT& iBuffLen, const BR::TotalRankingPlayerInformation& Data, int Option);
	extern template HRESULT ToString(char*& pBuff, INT& iBuffLen, const BR::NotificationType& Data, int Option);



}; // namespace StrUtil



