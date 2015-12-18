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
#include "Net/NetToString.h"


namespace BR {
namespace Net {

	enum TraceChannels {
		TRC_MSG			=	Trace::TRC_DBG1,
		TRC_SENDRAW		=	Trace::TRC_DBG2,
		TRC_RECVRAW		=	Trace::TRC_DBG3,
		TRC_SEND		=	Trace::TRC_DBG4,
		TRC_RECV		=	Trace::TRC_DBG5,
		TRC_NETCTRL		=	Trace::TRC_DBG6,
		TRC_GUARREANTEDCTRL		=	Trace::TRC_DBG7,
		TRC_TCPSENDRAW = Trace::TRC_DBG8,
		TRC_TCPRECVRAW = Trace::TRC_DBG9,
		TRC_TCPNETCTRL = Trace::TRC_DBG10,
		TRC_RAW			= Trace::TRC_DBG11,

		TRC_CONNECTION	=	Trace::TRC_USER1,
		TRC_NET			=	Trace::TRC_USER2,
		TRC_INFO		=	Trace::TRC_USER3,
		TRC_NETSYS = Trace::TRC_USER4,
	};

};
};


///////////////////////////////////////////////////////////////////////////////////
//
// Default trace module
//

DEFINE_TRACE_MODULE(net)

#define netErr(e)				TrcErrJmp(net,e,hr)
#define netChk(e)				{ do{ HRESULT hRes = e; if( FAILED(hRes) ) TrcErrJmp(net,hRes,hr); } while(0); }
#define netChkErr(ErrCode,exp)	{ do{ HRESULT hRes = exp; if( FAILED(hRes) ) TrcErrJmp(net,ErrCode,hr); } while(0); }
#define netMem(a)				{ if( (a) == NULL ) TrcErrJmp(net,E_OUTOFMEMORY,hr); }
#define netChkPtr(a)			{ if( (a) == NULL ) TrcErrJmp(net,E_POINTER,hr); }

#define netAssert(e)			trcAssert(e)
#define netAssertExp(e,expr)	trcAssertExp(e,expr)
//#define netTrace(x)				netTraceOut x
#define netTrace(lModeMask, ...)				TRACE_OUT(net,lModeMask,__VA_ARGS__)

#define netErrSilent(e)		trcErrSilent(e)


