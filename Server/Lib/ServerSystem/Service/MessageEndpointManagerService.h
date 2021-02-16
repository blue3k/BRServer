////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) StormForge
// 
// Author : KyungKun Ko
//
// Description : Message endpoint manager
//
////////////////////////////////////////////////////////////////////////////////


#pragma once 

#include "SFTypedefs.h"
#include "Object/SFSharedPointer.h"
#include "Types/SFEngineTypedefs.h"
#include "Util/SFString.h"

namespace SF {

	namespace ServerConfig
	{
		struct MessageEndpoint;
	}


	/////////////////////////////////////////////////////////////////////////////////////////
	//
	//	MessageEndpoint Manager service
	//

	class MessageEndpointManagerService
	{
	public:

		virtual Result AddOrGetRemoteEndpoint(const EntityUID& entityUID, const EndpointAddress& messageEndpoint, MessageEndpoint*& pEndpoint) { pEndpoint = nullptr;  return ResultCode::NOT_IMPLEMENTED; }

		virtual MessageEndpoint* GetEndpoint(const EntityUID& entityUID) { return nullptr; }
	};

} // namespace SF
