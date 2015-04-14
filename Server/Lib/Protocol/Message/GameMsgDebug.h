////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : Game Message debug definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Common/Typedefs.h"
#include "Common/PolicyID.h"
#include "Net/Netdef.h"
#include "Protocol/ProtocolMessageCPPInc.h"
#include "Protocol/Message/GameMsgClass.h"



namespace BR
{
 	namespace Message
	{
 		namespace Debug
		{
 
			///////////////////////////////////////////////////////////////
			// Game Debug output function 

			void RegisterDebugTraceGame();
			HRESULT DebugOutGame( const char*Prefix, MessageData*pMsg );





		}; // namespace Debug
	}; // namespace Message
}; // namespace BR


