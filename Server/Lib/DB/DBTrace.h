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

#include "Service/SFLogService.h"
#include "ServerLog/SvrLog.h"
#include "DB/DBToString.h"


namespace SF {

	namespace Log
	{
		extern LogChannel DB;


		constexpr LogOutputType TRC_QUERYSTR = LogOutputType::Debug1;
		constexpr LogOutputType TRC_QUERY = LogOutputType::Debug2;
		constexpr LogOutputType TRC_ROUTING = LogOutputType::Debug3;
		constexpr LogOutputType TRC_DBINFO = LogOutputType::Custom1;
	}


};


///////////////////////////////////////////////////////////////////////////////////
//
// Default trace module
//

#define dbCheck(Op) SFCheckResult(DB,Op)
#define dbCheckMem(Op) SFCheckMem(DB,Op)
#define dbCheckPtr(Op) SFCheckPtr(DB,Op)
#define dbError(e)		SFCheckResult(DB,e)
#define dbCheckCloseError(ErrCode,e)	{ do{ Result hRes = (e); if( !hRes ) { CloseTransaction(ErrCode); return hr;} } while(0); }

#define dbCheckCondition(e)				trcCheckCondition(e)
#define dbCheckConditionExp(e,expr)		trcCheckConditionExp(e,expr)


#define dbErr(e)			TrcErrJmp(DB,e,hr)
#define dbChk(e)			{ do{ Result hRes = e; if( !(hRes) ) TrcErrJmp(DB,hRes,hr); } while(0); }
#define dbMem(a)			{ if( (a) == nullptr ) TrcErrJmp(DB,ResultCode::OUT_OF_MEMORY,hr); }
#define dbChkPtr(a)			{ if( (a) == nullptr ) TrcErrJmp(DB,ResultCode::INVALID_POINTER,hr); }

#define dbAssert(e)			trcAssert(e)
#define dbAssertExp(e,expr)	trcAssertExp(e,expr)
#define dbTrace(lModeMask, ...)				SFLog2(Log::DB, Log::lModeMask,__VA_ARGS__)

#define dbErrSilent(e)		trcErrSilent(e)


