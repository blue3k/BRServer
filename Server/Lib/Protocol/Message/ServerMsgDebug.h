////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : Server Message debug definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Common/Typedefs.h"
#include "Common/PolicyID.h"
#include "Net/Netdef.h"
#include "Protocol/ProtocolMessageCPPInc.h"
#include "Protocol/Message/ServerMsgClass.h"



namespace BR
{
 	namespace Message
	{
 		namespace Debug
		{
 
			///////////////////////////////////////////////////////////////
			// Server Debug output function 

			void RegisterDebugTraceServer();
			HRESULT DebugOutServer( const char*Prefix, MessageData*pMsg );





		}; // namespace Debug
	}; // namespace Message
}; // namespace BR


