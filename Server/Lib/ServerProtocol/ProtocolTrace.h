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


namespace Trace
{
	namespace Protocol {
		enum TraceChannels {
			Debug		=	SF::Trace::TRC_DBG1,
			Debug1 = SF::Trace::TRC_DBG2,
			Debug2 = SF::Trace::TRC_DBG3,
			//TRC_		=	Trace::TRC_DBG2,
			//TRC_		=	Trace::TRC_DBG3,
			//TRC_		=	Trace::TRC_DBG4,
			//TRC_		=	Trace::TRC_DBG5,
			//TRC_		=	Trace::TRC_DBG6,
			//TRC_		=	Trace::TRC_DBG7,
			//TRC_		=	Trace::TRC_DBG8,
			//TRC_		=	Trace::TRC_DBG6,

		};
	};
};


///////////////////////////////////////////////////////////////////////////////////
//
// Default trace module
//

DEFINE_TRACE_MODULE(protocol)


#define protocolTrace(lModeMask,szfmt, ...)				TRACE_OUT(protocol,lModeMask,szfmt,__VA_ARGS__)




