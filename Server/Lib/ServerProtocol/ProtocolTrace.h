////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Network trace module 
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "ServerLog/SvrLog.h"
#include "Net/NetToString.h"


namespace SF
{

	//namespace LogSubChannels
	//{
	//	constexpr LogSubChannelType Debug = SF::LogSubChannels::Debug1;
	//	constexpr LogSubChannelType TRC_QUERY = SF::LogSubChannels::Debug2;
	//	constexpr LogSubChannelType TRC_ROUTING = SF::LogSubChannels::Debug3;
	//	constexpr LogSubChannelType TRC_DBINFO = SF::LogSubChannels::Custom1;
	//}

};


///////////////////////////////////////////////////////////////////////////////////
//
// Default trace module
//


#define protocolTrace(lModeMask, ...)				SFLog(Protocol,lModeMask,__VA_ARGS__)




