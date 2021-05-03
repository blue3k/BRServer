////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author: Kyungkun Ko
//
// Description : Brave base types
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "SFTypedefs.h"
#include "SFAssert.h"
#include "Util/SFStrUtil.h"
#include "Types/SFEngineTypedefs.h"
#include "Types/BrSvrTypes.h"


namespace SF {


	inline size_t SerializedSizeOf(const ServerFriendInformation& Value) { return sizeof(Value); }
	inline Result operator >> (IInputStream& input, ServerFriendInformation& data) { return input.Read(&data, sizeof(data)); }
	inline Result operator << (IOutputStream& output, const ServerFriendInformation& data) { return output.Write(&data, sizeof(data)); }

	inline size_t SerializedSizeOf(const GameLogItem& Value) { return sizeof(Value); }
	inline Result operator >> (IInputStream& input, GameLogItem& data) { return input.Read(&data, sizeof(data)); }
	inline Result operator << (IOutputStream& output, const GameLogItem& data) { return output.Write(&data, sizeof(data)); }

	inline size_t SerializedSizeOf(const GameLogChatMessage& Value) { return sizeof(Value); }
	inline Result operator >> (IInputStream& input, GameLogChatMessage& data) { return input.Read(&data, sizeof(data)); }
	inline Result operator << (IOutputStream& output, const GameLogChatMessage& data) { return output.Write(&data, sizeof(data)); }

	inline size_t SerializedSizeOf(const GameLogGameStateChange& Value) { return sizeof(Value); }
	inline Result operator >> (IInputStream& input, GameLogGameStateChange& data) { return input.Read(&data, sizeof(data)); }
	inline Result operator << (IOutputStream& output, const GameLogGameStateChange& data) { return output.Write(&data, sizeof(data)); }


};


