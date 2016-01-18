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

#include "Common/Trace.h"
#include "Protocol/ProtocolToString.h"
#include "Net/NetToString.h"


namespace Trace
{
	namespace Protocol {
		enum TraceChannels {
			//TRC_		=	Trace::TRC_DBG1,
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

#define protocolErr(e)			TrcErrJmp(protocol,e,hr)
#define protocolChk(e)			{ do{ HRESULT hRes = e; if( FAILED(hRes) ) TrcErrJmp(protocol,hRes,hr); } while(0); }
#define protocolMem(a)			{ if( (a) == NULL ) TrcErrJmp(protocol,E_SYSTEM_OUTOFMEMORY,hr); }
#define protocolChkPtr(a)		{ if( (a) == NULL ) TrcErrJmp(protocol,E_SYSTEM_POINTER,hr); }

#define protocolAssert(e)			trcAssert(e)
#define protocolAssertExp(e,expr)	trcAssertExp(e,expr)
//#define protocolTrace(x)				protocolTraceOut x
#define protocolTrace(lModeMask,szfmt, ...)				TRACE_OUT(protocol,lModeMask,szfmt,__VA_ARGS__)

#define protocolErrSilent(e)		trcErrSilent(e)


