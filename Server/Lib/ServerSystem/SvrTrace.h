////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Server trace module 
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Common/Trace.h"
#include "Net/NetToString.h"
#include "ServerSystem/SvrToString.h"


namespace BR {
namespace Svr {

	enum {

		TRC_INFO		=	Trace::TRC_USER1,
		TRC_ENTITY		=	Trace::TRC_USER2,
		TRC_TRANSACTION	=	Trace::TRC_USER3,
		TRC_CLUSTER		=	Trace::TRC_USER4,
		TRC_MATCHING	=	Trace::TRC_USER5,

		// For debug
		TRC_DBGFAIL		=	Trace::TRC_DBG1,
		TRC_DBGSVR		=	Trace::TRC_DBG2,
		TRC_DBGINFO		=	Trace::TRC_DBG3,
	};

};
};


///////////////////////////////////////////////////////////////////////////////////
//
// Default trace module
//

DEFINE_TRACE_MODULE(svr)

#define svrErr(e)					TrcErrJmp(svr,e,hr)
#define svrChk(e)					{ do{ HRESULT hRes = e; if( FAILED(hRes) ) TrcErrJmp(svr,hRes,hr); } while(0); }
#define svrChkSilent(e)				{ do{ HRESULT hRes = e; hr = hRes; if( FAILED(hRes) ) goto Proc_End; } while(0); }
#define svrChkErr(ErrCode,exp)		{ do{ HRESULT hRes = exp; if( FAILED(hRes) ) TrcErrJmp(svr,ErrCode,hr); } while(0); }
#define svrMem(a)					{ while( (a) == nullptr ){ TrcErrJmp(svr,E_SYSTEM_OUTOFMEMORY,hr); } }
#define svrChkPtr(a)				{ while( (a) == nullptr ){ TrcErrJmp(svr,E_SYSTEM_POINTER,hr); }  }

#define svrAssert(e)				trcAssert(e)
#define svrAssertExp(e,expr)		trcAssertExp(e,expr)

#define svrTrace(lModeMask, ...)				TRACE_OUT(svr,lModeMask,__VA_ARGS__)

#define svrErrSilent(e)		trcErrSilent(e)


// Transaction operations
#define svrErrClose(e)				{ do{ CloseTransaction(e); goto Proc_End; } while(0); }
#define svrChkClose(e)				{ do{ HRESULT hRes = e; if( FAILED(hRes) ) {CloseTransaction(hRes); goto Proc_End;} } while(0); }
#define svrChkCloseErr(ErrCode,e)	{ do{ HRESULT hRes = e; if( FAILED(hRes) ) {CloseTransaction(ErrCode); goto Proc_End;} } while(0); }


// Transaction operations
#define svrErrReturn(e)					TrcErrReturn(svr,e)
#define svrChkReturn(e)					{ do{ HRESULT hRes = e; if( FAILED(hRes) ) TrcErrReturn(svr,hRes); } while(0); }
#define svrChkErrReturn(ErrCode,exp)	{ do{ HRESULT hRes = exp; if( FAILED(hRes) ) TrcErrReturn(svr,ErrCode); } while(0); }
#define svrMemReturn(a)					{ while( (a) == nullptr ){ TrcErrReturn(svr,E_SYSTEM_OUTOFMEMORY); } }
#define svrChkPtrReturn(a)				{ while( (a) == nullptr ){ TrcErrReturn(svr,E_SYSTEM_POINTER); }  }

#define svrAssertReturn(e)				TrcAssertReturn(e)
#define svrAssertExpReturn(e,expr)		TrcAssertReturnExp(e,expr)

