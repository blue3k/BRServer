////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : GameServer Message debug definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Common/Typedefs.h"
#include "Common/PolicyID.h"
#include "Net/Netdef.h"
#include "Protocol/ProtocolMessageCPPInc.h"
#include "Protocol/Message/GameServerMsgClass.h"



namespace BR
{
 	namespace Message
	{
 		namespace Debug
		{
 
			///////////////////////////////////////////////////////////////
			// GameServer Debug output function 

			void RegisterDebugTraceGameServer();
			HRESULT DebugOutGameServer( const char*Prefix, MessageData*pMsg );





		}; // namespace Debug
	}; // namespace Message
}; // namespace BR


