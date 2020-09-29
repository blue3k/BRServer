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
#include "String/ToStringSvr.h"


namespace SF {

	namespace LogSubChannels
	{
		constexpr LogSubChannelType SVR_INFO = LogSubChannelType::Custom1;
		constexpr LogSubChannelType SVR_ENTITY = LogSubChannelType::Custom2;
		constexpr LogSubChannelType SVR_TRANSACTION = LogSubChannelType::Custom3;
		constexpr LogSubChannelType SVR_CLUSTER = LogSubChannelType::Custom4;
		constexpr LogSubChannelType SVR_MATCHING = LogSubChannelType::Custom5;
		constexpr LogSubChannelType SVR_GAME1 = LogSubChannelType::Custom6;

		// For debug
		constexpr LogSubChannelType SVR_DBGFAIL = LogSubChannelType::Debug1;
		constexpr LogSubChannelType SVR_DBGSVR = LogSubChannelType::Debug2;
		constexpr LogSubChannelType SVR_DBGINFO = LogSubChannelType::Debug3;
		constexpr LogSubChannelType SVR_DBGTRANS = LogSubChannelType::Debug4;
		constexpr LogSubChannelType SVR_DBGMATCHING = LogSubChannelType::Debug5;
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


// Legacy - gradually deprecated

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

#define svrErrorClose(e)				{ do{ CloseTransaction(e); return hr; } while(0); }
#define svrCheckClose(e)				{ do{ Result hRes = e; if( !(hRes) ) {CloseTransaction(hRes); return hr;} } while(0); }
#define svrCheckCloseErr(ErrCode,e)		{ do{ Result hRes = e; if( !(hRes) ) {CloseTransaction(ErrCode); return hr;} } while(0); }


// Transaction operations
#define svrErrReturn(e)					TrcErrReturn(Svr,e)
#define svrChkReturn(e)					{ do{ Result hRes = e; if( !(hRes) ) TrcErrReturn(Svr,hRes); } while(0); }
#define svrChkErrReturn(ErrCode,exp)	{ do{ Result hRes = exp; if( !(hRes) ) TrcErrReturn(Svr,ErrCode); } while(0); }
#define svrMemReturn(a)					{ while( (a) == nullptr ){ TrcErrReturn(Svr,ResultCode::OUT_OF_MEMORY); } }
#define svrChkPtrReturn(a)				{ while( (a) == nullptr ){ TrcErrReturn(Svr,ResultCode::INVALID_POINTER); }  }

#define svrAssertReturn(e)				TrcAssertReturn(e)
#define svrAssertExpReturn(e,expr)		TrcAssertReturnExp(e,expr)

