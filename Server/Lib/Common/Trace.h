////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 Madk
// 
// Author : KyungKun Ko
//
// Description : Server constants definitions. 
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Common/Typedefs.h"
#include "Common/StrUtil.h"
#include "Common/TimeUtil.h"
#include "Common/StrFormat.h"
#include "Common/ResultCode/BRResultCodeSystem.h"


#ifndef SHIPPING
#define SVRTRC_DBG
#endif

#define TRCCALL 

namespace BR {
namespace Trace {


	class TraceOutModule;
	class TraceModule;

	//////////////////////////////////////////////////////////////////////////////////////
	//
	//  Trace module
	//

	enum TraceChannels {

		// Generic trace
		TRC_ERROR		= 0x00000001,
		TRC_WARN		= 0x00000002,
		TRC_ASSERT		= 0x00000004,
		TRC_TRACE		= 0x00000008,
		
		// Moudule additional traces
		TRC_USER1		= 0x00000020,
		TRC_USER2		= 0x00000040,
		TRC_USER3		= 0x00000080,
		TRC_USER4		= 0x00000100,
		TRC_USER5		= 0x00000200,
		TRC_USER6		= 0x00000400,
		TRC_USER7		= 0x00000800,
		TRC_USER8		= 0x00001000,
		TRC_USER9		= 0x00002000,
		TRC_USER10		= 0x00004000,
		TRC_USER11		= 0x00008000,
		TRC_USER12		= 0x00010000,
		TRC_USER13		= 0x00020000,
		TRC_USER14		= 0x00040000,
		TRC_USER15		= 0x00080000,
		TRC_USERMASK	= (TRC_USER1 | TRC_USER2 | TRC_USER3 | TRC_USER4 | TRC_USER5 | TRC_USER6 | TRC_USER7 | TRC_USER8 
							| TRC_USER9 | TRC_USER10 | TRC_USER11 | TRC_USER12 | TRC_USER13 | TRC_USER14 | TRC_USER15),

		// Debugging traces
		TRC_DBG1		= 0x00100000,
		TRC_DBG2		= 0x00200000,
		TRC_DBG3		= 0x00400000,
		TRC_DBG4		= 0x00800000,
		TRC_DBG5		= 0x01000000,
		TRC_DBG6		= 0x02000000,
		TRC_DBG7		= 0x04000000,
		TRC_DBG8		= 0x08000000,
		TRC_DBG9		= 0x10000000,
		TRC_DBG10		= 0x20000000,
		TRC_DBG11		= 0x40000000,
		TRC_DBG12		= 0x80000000,
		TRC_DBGMASK		= (TRC_DBG1 | TRC_DBG2 | TRC_DBG3 | TRC_DBG4 | TRC_DBG5 | TRC_DBG6 | TRC_DBG7 | TRC_DBG8 | TRC_DBG9 | TRC_DBG10 | TRC_DBG11 | TRC_DBG12),

		// Trace level default value
		TRC_DEFAULT		= (TRC_ERROR | TRC_WARN | TRC_ASSERT | TRC_TRACE | TRC_USERMASK),

		TRC_GENERIC_ALL	= (TRC_ERROR | TRC_WARN | TRC_ASSERT | TRC_TRACE | TRC_USERMASK ),
	};


	// Trace output option
	enum TraceOutput {

		// Generic trace
		TRCOUT_DEBUG	= 0x00000001,	// VS debug console
		TRCOUT_CONSOLE	= 0x00000002,	// Console write
		TRCOUT_EVENT	= 0x00000004,	// Event log
		TRCOUT_FILE1	= 0x00000100,	// Release Log file
		TRCOUT_FILE2	= 0x00000200,	// Debug log file( maybe include all )

		TRCOUT_FILE_ALL	= TRCOUT_FILE1|TRCOUT_FILE2,

		// Trace level default value
		TRCOUT_DEFAULT			= (TRCOUT_FILE1 | TRCOUT_CONSOLE),
		TRCOUT_DEFAULT_DEBUG	= (TRCOUT_FILE2 | TRCOUT_CONSOLE),
		TRCOUT_NONE		= 0,

		TRCOUT_FILE_LOG			= 0,	// Release Log file
		TRCOUT_FILE_DBGLOG		= 1,	// Debug log file( maybe include all )
		TRCOUT_NUMFILE = 2,
	};



	////////////////////////////////////////////////////////////////////////////////
	//
	// Trace Log Module
	//
	class TraceModule
	{
	public:
		enum { MAX_TRACEMODULE = 30 };

		static const char* CONFIG_FILENAME;

	private:
		// Trace mode Mask 
		UINT			m_uiTraceMask;

		// Name of trace module
		char*			m_szName;
		char*			m_szNameTag;

		static TimeStampMS     m_MaskUpdated;
		static std::unordered_map<std::string, UINT32> stm_Masks;
		
	private:

		// Module state registry key
		//static HKEY		stm_hRegKey;
		//static 

		// Trace modules
		static TraceModule* stm_ModuleList[MAX_TRACEMODULE];

		static bool LoadTraceConfig();

		// Update trace
		void UpdateTrace();

		static HRESULT CheckAndUpdate();

		friend class TraceOutModule;

	public:

		TraceModule( const char *szName, const char *szNameTag );
		~TraceModule();

		static TraceModule** GetModules();

		inline bool CheckTrace( UINT trcMask );
		inline UINT GetTraceMask();
		inline const char *GetName();
		inline const char *GetNameTag();

		// Trace print out
		void TraceOut( UINT trcMask, const char *strString );

		void* TraceReserveWriteBuffer( UINT trcInputMask, char*& stringBuffer, INT &buffLen );
		void TraceSendWriteBuffer( void* pWriteBuffer, INT buffLen );
	};




	//////////////////////////////////////////////////////////////////////////////////////
	//
	// Trace system initialize
	//

	void Flush();

	// Allocate console if not exist
	void AllocScreenConsole();



	//////////////////////////////////////////////////////////////////////////////////////
	//
	//  System Exception handler
	//

	enum ExceptionMode 
	{
		Exception_Crash,
		Exception_SnapShot,
	};

	// initialize exception Handler
	void InitExceptionHandler();

	
} // namespace Trace
} // namespace BR


//////////////////////////////////////////////////////////////////////////////////////
//
//  Declare trace module
//
#define DECLARE_TRACE_MODULE(trcMod) \
	BR::Trace::TraceModule trcMod##TraceModule(#trcMod,#trcMod":");  \
														\


#define TRACE_OUT(trcMod,lModeMask, ...) \
	do{if( trcMod##TraceModule.CheckTrace(lModeMask) ) {\
		INT buffLen = 0;\
		char *strTraceBuff = nullptr;\
		void* bufferContext = trcMod##TraceModule.TraceReserveWriteBuffer(lModeMask,strTraceBuff,buffLen );\
		if( bufferContext == nullptr || strTraceBuff == nullptr ) break;\
		StrUtil::Format( strTraceBuff, buffLen,  __VA_ARGS__ );\
		trcMod##TraceModule.TraceSendWriteBuffer(bufferContext,buffLen );\
	}}while(0);\
	\



#define DEFINE_TRACE_MODULE(trcMod) \
	extern BR::Trace::TraceModule trcMod##TraceModule;\
	\





/////////////////////////////////////////////////
//
// Error Jump Macros.
//
/////////////////////////////////////////////////


#ifdef SVRTRC_DBG

	#define TrcErrJmp(trcMod, errval, __var) \
	do {\
		__var = errval;\
		TRACE_OUT(trcMod, Trace::TRC_ERROR, "{0}({1}): 0x{2:X8}",     \
			__FILE__, __LINE__, hr ); \
		goto Proc_End;\
	} while(0);

	#define TrcErrReturn(trcMod, errval) \
	do {\
		TRACE_OUT(trcMod, Trace::TRC_ERROR, "{0}({1}): 0x{2:X8}",     \
			__FILE__, __LINE__, errval ); \
		return errval;\
	} while(0);

// Basic check&branch routine
#define trcChk(checkState) \
	do{\
		hr = checkState;\
		if( FAILED(hr) )\
		{\
			defTrace( Trace::TRC_ERROR, "{0}({1}): 0x{2:X8}", __FILE__, __LINE__, hr ); \
			goto Proc_End;\
		}\
	}while(0)\

#define trcErr(errVal) \
	do{\
		defTrace( Trace::TRC_ERROR, "{0}({1}): 0x{2:X8}", __FILE__, __LINE__, hr ); \
		hr = errVal;\
		goto Proc_End;\
	}while(0)\


#define trcMem(checkPointer) \
	do {\
		if( (checkPointer) == NULL )\
		{\
			defTrace( Trace::TRC_ERROR, "{0}({1}): Null Exception",     \
				__FILE__, __LINE__ ); \
			hr = E_SYSTEM_OUTOFMEMORY;\
			goto Proc_End;\
		}\
	} while(0);\


#define trcChkPtr(checkPointer) \
	do{\
		if( (checkPointer) == NULL )\
		{\
			defTrace( Trace::TRC_ERROR, "{0}({1}): Invalid Pointer", __FILE__, __LINE__ ); \
			hr = E_SYSTEM_POINTER;\
			goto Proc_End;\
		}\
	}while(0);\



// Assert
#define trcAssert(condi) \
				do{ \
					if( !(condi) ) {\
					defTrace( Trace::TRC_ASSERT, "{0}({1}): Assert occure : {2}", __FILE__, __LINE__, #condi );  Trace::Flush();\
						Assert(condi);\
						trcErr(E_SYSTEM_UNEXPECTED);\
					}\
				}while(0) \


// Assert with expression
#define trcAssertExp(condi,expr) \
				do{ \
					if( !(condi) ) {\
						defTrace( Trace::TRC_ASSERT, "{0}({1}): Assert occure : {2} : {3}", __FILE__, __LINE__, #condi, expr ); Trace::Flush();\
						Assert(condi);\
						trcErr(E_SYSTEM_UNEXPECTED);\
					}\
				}while(0) \


// Assert
#define TrcAssertReturn(condi) \
				do{ \
					if( !(condi) ) {\
					defTrace( Trace::TRC_ASSERT, "{0}({1}): Assert occure : {2}", __FILE__, __LINE__, #condi );  Trace::Flush();\
						Assert(condi);\
						return E_SYSTEM_UNEXPECTED;\
					}\
				}while(0) \


// Assert with expression
#define TrcAssertReturnExp(condi,expr) \
				do{ \
					if( !(condi) ) {\
						defTrace( Trace::TRC_ASSERT, "{0}({1}): Assert occure : {2} : {3}", __FILE__, __LINE__, #condi, expr ); Trace::Flush();\
						Assert(condi);\
						return E_SYSTEM_UNEXPECTED;\
					}\
				}while(0) \



#else
	#define TrcErrJmp(trcMod, errval, var) \
	do {\
		var = errval;\
		goto Proc_End;\
	} while(0);

	#define TrcErrReturn(trcMod, errval) \
	do {\
		return errval;\
	} while(0);

// Basic check&branch routine
#define trcChk(checkState) \
	do{\
		hr = checkState;\
		if( FAILED(hr) )\
		{\
			goto Proc_End;\
		}\
	}while(0)\

#define trcErr(errVal) \
	do {\
		hr = errVal;\
		goto Proc_End;\
	} while(0);\


#define trcMem(checkPointer) \
	do{\
		if( (checkPointer) == NULL )\
		{\
			hr = E_SYSTEM_OUTOFMEMORY;\
			goto Proc_End;\
		}\
	}while(0)\


#define trcChkPtr(checkPointer) \
	do{\
		if( (checkPointer) == NULL )\
		{\
			hr = E_SYSTEM_POINTER;\
			goto Proc_End;\
		}\
	}while(0)\



// Assert

#define trcAssert(condi) \
				{ \
					if( !(condi) ) {\
						defTrace( Trace::TRC_ASSERT, "{0}({1}): Assert occure : {2}", __FILE__, __LINE__, #condi ); Trace::Flush();\
						trcErr(E_SYSTEM_UNEXPECTED);\
					}\
				} \


// Assert with expression
#define trcAssertExp(condi,expr) \
				do{ \
					if( !(condi) ) {\
						defTrace( Trace::TRC_ASSERT, "{0}({1}): Assert occure : {2} : {3}", __FILE__, __LINE__, #condi, expr ); Trace::Flush();\
						trcErr(E_SYSTEM_UNEXPECTED);\
					}\
				} while(0); \


#define TrcAssertReturn(condi) \
				{ \
					if( !(condi) ) {\
						defTrace( Trace::TRC_ASSERT, "{0}({1}): Assert occure : {2}", __FILE__, __LINE__, #condi ); Trace::Flush();\
						return E_SYSTEM_UNEXPECTED;\
					}\
				} \


// Assert with expression
#define TrcAssertReturnExp(condi,expr) \
				do{ \
					if( !(condi) ) {\
						defTrace( Trace::TRC_ASSERT, "{0}({1}): Assert occure : {2} : {3}", __FILE__, __LINE__, #condi, expr ); Trace::Flush();\
						return E_SYSTEM_UNEXPECTED;\
					}\
				} while(0); \


#endif // SVRTRC_DBG



// Basic check&branch routine
#define trcChkSilent(checkState) \
	do{\
	hr = checkState;\
	if( FAILED(hr) )\
		{\
		goto Proc_End;\
		}\
	}while(0)\


#define trcErrSilent(errval)		\
	do {\
	hr = errval;\
	goto Proc_End;\
	} while(0);



// Assert even release mode
#define TrcAssertRel(condi) \
				do{ \
					if( !(condi) ) {\
						AssertRel(condi);\
						trcErr(E_SYSTEM_UNEXPECTED);\
					}\
				} while(0)\






///////////////////////////////////////////////////////////////////////////////////
//
// Default trace module
//

DEFINE_TRACE_MODULE(def)

#define defErr(e)			trcErr(e)
#define defChk(e)			trcChk(e)
#define defChkErr(ErrCode,exp)			{ do{ HRESULT hRes = exp; if( FAILED(hRes) ) TrcErrJmp(svr,ErrCode,hr); } while(0); }
#define defMem(a)			trcMem(a)
#define defChkPtr(a)		trcChkPtr(a)

#define defAssert(e)			trcAssert(e)
#define defAssertExp(e,expr)	trcAssertExp(e,expr)
#define defTrace(lModeMask, ...)				TRACE_OUT(def,lModeMask,__VA_ARGS__)

#define defChkSilent(e)		trcChkSilent(e)
#define defErrSilent(e)		trcErrSilent(e)


#include "Trace.inl"

