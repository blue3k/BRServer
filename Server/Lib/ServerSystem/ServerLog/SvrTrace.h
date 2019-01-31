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

#include "ServerLog/SvrLog.h"
#include "Net/SFNetToString.h"
#include "String/SFSvrToString.h"


namespace SF {
namespace Svr {

	constexpr uint32_t TRC_INFO = Trace::TRC_USER1;
	constexpr uint32_t TRC_ENTITY = Trace::TRC_USER2;
	constexpr uint32_t TRC_TRANSACTION = Trace::TRC_USER3;
	constexpr uint32_t TRC_CLUSTER = Trace::TRC_USER4;
	constexpr uint32_t TRC_MATCHING = Trace::TRC_USER5;
	constexpr uint32_t TRC_GAME1 = Trace::TRC_USER6;

	// For debug
	constexpr uint32_t TRC_DBGFAIL = Debug1;
	constexpr uint32_t TRC_DBGSVR = Trace::TRC_DBG2;
	constexpr uint32_t TRC_DBGINFO = Trace::TRC_DBG3;
	constexpr uint32_t TRC_DBGTRANS = Trace::TRC_DBG4;
	constexpr uint32_t TRC_DBGMATCHING = Trace::TRC_DBG5;

};
};


///////////////////////////////////////////////////////////////////////////////////
//
// Default trace module
//


#define svrErr(e)					TrcErrJmp(Svr,e,hr)
#define svrChk(e)					{ do{ Result hRes = e; if( !(hRes) ) TrcErrJmp(Svr,hRes,hr); } while(0); }
#define svrChkSilent(e)				{ do{ Result hRes = e; hr = hRes; if( !(hRes) ) goto Proc_End; } while(0); }
#define svrChkErr(ErrCode,exp)		{ do{ Result hRes = exp; if( !(hRes) ) TrcErrJmp(Svr,ErrCode,hr); } while(0); }
#define svrMem(a)					{ while( (a) == nullptr ){ TrcErrJmp(Svr,ResultCode::OUT_OF_MEMORY,hr); } }
#define svrChkPtr(a)				{ while( (a) == nullptr ){ TrcErrJmp(Svr,ResultCode::INVALID_POINTER,hr); }  }

#define svrAssert(e)				trcAssert(e)
#define svrAssertExp(e,expr)		trcAssertExp(e,expr)

#define svrTrace(lModeMask, ...)				SFLog(Svr,lModeMask,__VA_ARGS__)

#define svrErrSilent(e)		trcErrSilent(e)


// Transaction operations
#define svrErrClose(e)				{ do{ CloseTransaction(e); goto Proc_End; } while(0); }
#define svrChkClose(e)				{ do{ Result hRes = e; if( !(hRes) ) {CloseTransaction(hRes); goto Proc_End;} } while(0); }
#define svrChkCloseErr(ErrCode,e)	{ do{ Result hRes = e; if( !(hRes) ) {CloseTransaction(ErrCode); goto Proc_End;} } while(0); }


// Transaction operations
#define svrErrReturn(e)					TrcErrReturn(Svr,e)
#define svrChkReturn(e)					{ do{ Result hRes = e; if( !(hRes) ) TrcErrReturn(Svr,hRes); } while(0); }
#define svrChkErrReturn(ErrCode,exp)	{ do{ Result hRes = exp; if( !(hRes) ) TrcErrReturn(Svr,ErrCode); } while(0); }
#define svrMemReturn(a)					{ while( (a) == nullptr ){ TrcErrReturn(Svr,ResultCode::OUT_OF_MEMORY); } }
#define svrChkPtrReturn(a)				{ while( (a) == nullptr ){ TrcErrReturn(Svr,ResultCode::INVALID_POINTER); }  }

#define svrAssertReturn(e)				TrcAssertReturn(e)
#define svrAssertExpReturn(e,expr)		TrcAssertReturnExp(e,expr)

