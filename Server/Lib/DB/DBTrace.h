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

#include "ServerLog/SvrLog.h"
#include "DB/DBToString.h"


namespace BR {
namespace DB {

	enum TraceChannels {
		TRC_QUERYSTR		=	Trace::TRC_DBG1,
		TRC_QUERY = Trace::TRC_DBG2,
		TRC_ROUTING = Trace::TRC_DBG3,

		TRC_INFO			=	Trace::TRC_USER1,
	};

};
};


///////////////////////////////////////////////////////////////////////////////////
//
// Default trace module
//

DEFINE_TRACE_MODULE(db)

#define dbErr(e)			TrcErrJmp(db,e,hr)
#define dbChk(e)			{ do{ Result hRes = e; if( !(hRes) ) TrcErrJmp(db,hRes,hr); } while(0); }
#define dbMem(a)			{ if( (a) == nullptr ) TrcErrJmp(db,ResultCode::OUT_OF_MEMORY,hr); }
#define dbChkPtr(a)			{ if( (a) == nullptr ) TrcErrJmp(db,ResultCode::INVALID_POINTER,hr); }

#define dbAssert(e)			trcAssert(e)
#define dbAssertExp(e,expr)	trcAssertExp(e,expr)
#define dbTrace(lModeMask, ...)				TRACE_OUT(db,lModeMask,__VA_ARGS__)

#define dbErrSilent(e)		trcErrSilent(e)


