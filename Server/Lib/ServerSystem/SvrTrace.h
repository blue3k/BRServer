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

#include "Service/SFLogService.h"
#include "ServerLog/SvrLog.h"
#include "Net/SFNetToString.h"
#include "String/ToStringSvr.h"


namespace SF {

	namespace Log
	{
		extern LogChannel Svr;

		constexpr LogOutputType SVR_INFO = LogOutputType::Custom1;
		constexpr LogOutputType SVR_ENTITY = LogOutputType::Custom2;
		constexpr LogOutputType SVR_TRANSACTION = LogOutputType::Custom3;
		constexpr LogOutputType SVR_CLUSTER = LogOutputType::Custom4;
		constexpr LogOutputType SVR_MATCHING = LogOutputType::Custom5;
		constexpr LogOutputType SVR_GAME1 = LogOutputType::Custom6;

		// For debug
		constexpr LogOutputType SVR_DBGFAIL = LogOutputType::Debug1;
		constexpr LogOutputType SVR_DBGSVR = LogOutputType::Debug2;
		constexpr LogOutputType SVR_DBGINFO = LogOutputType::Debug3;
		constexpr LogOutputType SVR_DBGTRANS = LogOutputType::Debug4;
		constexpr LogOutputType SVR_DBGMATCHING = LogOutputType::Debug5;
	}
};


///////////////////////////////////////////////////////////////////////////////////
//
// Default trace module
//

#define svrCheck(Op) SFCheckResult(Svr,Op)
#define svrCheckMem(Op) SFCheckMem(Svr,Op)
#define svrCheckPtr(Op) SFCheckPtr(Svr,Op)
#define svrError(e)		SFCheckResult(Svr,e)
#define svrCheckCloseError(ErrCode,e)	{ do{ Result hRes = (e); if( !hRes ) { CloseTransaction(ErrCode); return hr;} } while(0); }

#define svrCheckCondition(e)				trcCheckCondition(e)
#define svrCheckConditionExp(e,expr)		trcCheckConditionExp(e,expr)

#define svrErrorClose(e)				{ do{ CloseTransaction(e); return hr = e; } while(0); }
#define svrCheckClose(e)				{ do{ hr = e; if(!hr) { CloseTransaction(hr); return hr; } } while(0); }
#define svrCheckCloseErr(ErrCode,e)		{ do{ hr = e; if(!hr) { CloseTransaction(ErrCode); return hr; } } while(0); }


// Legacy - gradually deprecated

#define svrErr(e)					TrcErrJmp(Svr,e,hr)
#define svrChk(e)					{ do{ Result hRes = e; if( !(hRes) ) TrcErrJmp(Svr,hRes,hr); } while(0); }
#define svrChkSilent(e)				{ do{ Result hRes = e; hr = hRes; if( !(hRes) ) goto Proc_End; } while(0); }
#define svrChkErr(ErrCode,exp)		{ do{ Result hRes = exp; if( !(hRes) ) TrcErrJmp(Svr,ErrCode,hr); } while(0); }
#define svrMem(a)					{ while( (a) == nullptr ){ TrcErrJmp(Svr,ResultCode::OUT_OF_MEMORY,hr); } }
#define svrChkPtr(a)				{ while( (a) == nullptr ){ TrcErrJmp(Svr,ResultCode::INVALID_POINTER,hr); }  }

#define svrAssert(e)				trcAssert(e)
#define svrAssertExp(e,expr)		trcAssertExp(e,expr)

#define svrTrace(lModeMask, ...)				SFLog2(Log::Svr,Log::lModeMask,__VA_ARGS__)

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

