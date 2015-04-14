/* File : example.i */
%module BRMonitoring



%{
#include "Common/typedefs.h"
#include "Common/BrBaseTypes.h"
#include "Common/BrCommonTypes.h"
#include "Common/BrGameTypes.h"
#include "Common/BrSvrTypes.h"
#include "Common/ArrayUtil.h"
#include "Common/PolicyID.h"
#include "Common/Message.h"
#include "Common/SharedObject.h"
#include "Common/SharedPointer.h"
#include "Net/NetDef.h"
#include "Net/NetPolicy.h"
%}

%include "swigarch.i" // This one has bug with win64
%include "typemaps.i"
%include "cpointer.i"
%include "cmalloc.i"
//%include "math.i"
%include "arrays_csharp.i"
%include "carrays.i"
%include "stdint.i"
%include "stl.i"
%include "windows.i"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Macros
//

%csmethodmodifiers "public unsafe";

%define INOUT_CLASS_REFERENCE(CTYPE, CSTYPE)
%typemap(cstype)	CTYPE& "CSTYPE"
%typemap(csin)		CTYPE& "CSTYPE.getCPtr($csinput).Handle"
%typemap(csout, excode=SWIGEXCODE) 
					CTYPE& "{ var result = $imcall; return result == null ? null : new CSTYPE(result,false); }"
%typemap(ctype)		CTYPE& "CTYPE"
%typemap(imtype)	CTYPE& "global::System.IntPtr"
//%typemap(in)		CTYPE& "$1 = $input;"
%typemap(out)		CTYPE& "jresult = *$1;"
//%typemap(argin)		CTYPE& "CTYPE&"
//%typemap(argout)	CTYPE& "CTYPE"
%enddef


%define INOUT_REFERENCE(CTYPE, CSTYPE)
%typemap(cstype)	CTYPE& "ref CSTYPE"
%typemap(csin,pre="fixed(CSTYPE* temp$csinput = &$csinput)")    
					CTYPE& "(global::System.IntPtr)temp$csinput"
%typemap(ctype)		CTYPE& "CTYPE*"
%typemap(imtype)	CTYPE& "global::System.IntPtr"
%typemap(in)		CTYPE& "$1 = $input;"
%typemap(argin)		CTYPE& "CTYPE*"
%enddef


%define INOUT_VALUE(CTYPE, CSTYPE)
%typemap(cstype)	CTYPE "CSTYPE"
%typemap(csin,pre="fixed(CSTYPE* temp$csinput = &$csinput)")    
					CTYPE "(global::System.IntPtr)temp$csinput"
%typemap(csout, excode=SWIGEXCODE) 
					CTYPE "{ return $imcall; }"
%typemap(ctype)		CTYPE "CTYPE"
%typemap(imtype)	CTYPE "CSTYPE"      // im-function type
%typemap(in)		CTYPE "$1 = $input;"
%typemap(out)		CTYPE "$result = $1;"
%enddef


%define IN_ARRAY(CTYPE, CSTYPE)
%typemap(cstype)	const CTYPE* "CSTYPE[]"
%typemap(csin)		const CTYPE* "$csinput"
%typemap(ctype)		const CTYPE* "const CTYPE*"
%typemap(imtype, inattributes="[System.Runtime.InteropServices.In, System.Runtime.InteropServices.Out, global::System.Runtime.InteropServices.MarshalAs(global::System.Runtime.InteropServices.UnmanagedType.LPArray)]")
					const CTYPE* "CSTYPE[]"
%typemap(in)		const CTYPE* "$1 = $input;"
%typemap(argin)		const CTYPE* "CTYPE*"
%enddef

%define INOUT_ARRAY(CTYPE, CSTYPE)
%typemap(cstype)	CTYPE* "CSTYPE[]"
%typemap(csin)		CTYPE* "$csinput"
%typemap(ctype)		CTYPE* "CTYPE*"
%typemap(imtype, inattributes="[System.Runtime.InteropServices.In, System.Runtime.InteropServices.Out, global::System.Runtime.InteropServices.MarshalAs(global::System.Runtime.InteropServices.UnmanagedType.LPArray)]")
					CTYPE* "CSTYPE[]"
%typemap(in)		CTYPE* "$1 = $input;"
%typemap(argin)		CTYPE* "CTYPE*"
%enddef


//
//	Macros
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//typedef unsigned long long Context;
//typedef unsigned long long EntityUID;
//typedef unsigned long long SysUInt;

//typedef UINT64 EntityUID;


%rename(Execute) operator();
%rename(CopyFrom) operator=;
%rename(EqualTo) operator==;
%rename(Increase) operator++;
%rename(NotEqualTo) operator!=;
%rename(GetAt) operator[];
%rename(AddTo) operator+=;
%rename(NativeDispose) Dispose;

%rename(ToSharedObjectPtr) operator SharedObject*();
%ignore operator SharedObject*();
%rename(ToSharedPointer) operator SharedPointer();

INOUT_CLASS_REFERENCE(BR::SharedReferenceManager*,SharedReferenceManager)
INOUT_VALUE(INT64,System.Int64)
INOUT_VALUE(BR::EntityFaculty,System.UInt32)


%include "Common/typedefs.h"
%include "Common/BrBaseTypes.h"
%include "Common/BrCommonTypes.h"
%include "Common/BrGameTypes.h"
%include "Common/BrSvrTypes.h"
%include "Common/ArrayUtil.h"
%include "Common/PolicyID.h"
%include "Common/Message.h"
%include "Common/SharedObject.h"
%include "Common/SharedPointer.h"
%include "Net/NetDef.h"
%include "Net/NetPolicy.h"


%template(ArrayInt64) BR::Array<INT64>;
%template(ArrayUInt64) BR::Array<UINT64>;
%template(ArrayInt32) BR::Array<INT32>;
%template(ArrayUInt32) BR::Array<UINT32>;
%template(ArrayBYTE) BR::Array<BYTE>;
%template(ArrayChar) BR::Array<char>;
%template(ArrayEntityUID) BR::Array<BR::EntityUID>;



%include SWIG_Protocol.i





