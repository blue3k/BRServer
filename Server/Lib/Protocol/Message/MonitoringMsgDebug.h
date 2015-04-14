////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : Monitoring Message debug definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Common/Typedefs.h"
#include "Common/PolicyID.h"
#include "Net/Netdef.h"
#include "Protocol/ProtocolMessageCPPInc.h"
#include "Protocol/Message/MonitoringMsgClass.h"



namespace BR
{
 	namespace Message
	{
 		namespace Debug
		{
 
			///////////////////////////////////////////////////////////////
			// Monitoring Debug output function 

			void RegisterDebugTraceMonitoring();
			HRESULT DebugOutMonitoring( const char*Prefix, MessageData*pMsg );





		}; // namespace Debug
	}; // namespace Message
}; // namespace BR


