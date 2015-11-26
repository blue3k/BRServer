////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : DB trace module 
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Common/Trace.h"
#include "DB/DBToString.h"


namespace BR {
namespace DB {

	enum TraceChannels {
		TRC_QUERYSTR		=	Trace::TRC_DBG1,

		TRC_INFO			=	Trace::TRC_USER1,
		TRC_QUERY			=	Trace::TRC_USER2,
	};

};
};


///////////////////////////////////////////////////////////////////////////////////
//
// Default trace module
//

DEFINE_TRACE_MODULE(db)

#define dbErr(e)			TrcErrJmp(db,e,hr)
#define dbChk(e)			{ do{ HRESULT hRes = e; if( FAILED(hRes) ) TrcErrJmp(db,hRes,hr); } while(0); }
#define dbMem(a)			{ if( (a) == NULL ) TrcErrJmp(db,E_OUTOFMEMORY,hr); }
#define dbChkPtr(a)			{ if( (a) == NULL ) TrcErrJmp(db,E_POINTER,hr); }

#define dbAssert(e)			trcAssert(e)
#define dbAssertExp(e,expr)	trcAssertExp(e,expr)
#define dbTrace(lModeMask, ...)				TRACE_OUT(db,lModeMask,__VA_ARGS__)

#define dbErrSilent(e)		trcErrSilent(e)


