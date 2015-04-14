////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : PartyMatchingQueue Message debug definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Common/Typedefs.h"
#include "Common/PolicyID.h"
#include "Net/Netdef.h"
#include "Protocol/ProtocolMessageCPPInc.h"
#include "Protocol/Message/PartyMatchingQueueMsgClass.h"



namespace BR
{
 	namespace Message
	{
 		namespace Debug
		{
 
			///////////////////////////////////////////////////////////////
			// PartyMatchingQueue Debug output function 

			void RegisterDebugTracePartyMatchingQueue();
			HRESULT DebugOutPartyMatchingQueue( const char*Prefix, MessageData*pMsg );





		}; // namespace Debug
	}; // namespace Message
}; // namespace BR


