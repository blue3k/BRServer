
//%module(directors="1") BRMonitoring
%module BRMonitoring


%csmethodmodifiers "public unsafe";


%include SWIG_server.i

//%feature("director") ArrayPerformanceCounterInfo;
//%feature("director") ArrayPerformanceCounterInstanceInfo;

%template(ArrayPerformanceCounterInfo) BR::Array<BR::PerformanceCounterInfo>;
%template(ArrayPerformanceCounterInstanceInfo) BR::Array<BR::PerformanceCounterInstanceInfo>;



%{
#include "ServerSystem/PerformanceCounter/PerformanceCounter.h"
#include "ServerSystem/PerformanceCounter/PerformanceCounterInstance.h"
#include "ServerSystem/PerformanceCounter/PerformanceCounterServer.h"
%}


%include "ServerSystem/PerformanceCounter/PerformanceCounter.h"

%template(ArrayPerformanceCounterPtr) BR::Array<BR::Svr::PerformanceCounter*>;

%include "ServerSystem/PerformanceCounter/PerformanceCounterInstance.h"



INOUT_ARRAY(UINT64,System.UInt64)
INOUT_REFERENCE(UINT,uint)
INOUT_CLASS_REFERENCE(BR::Svr::PerformanceCounterInstance*,PerformanceCounterInstance)
INOUT_CLASS_REFERENCE(BR::Svr::PerformanceCounter*,PerformanceCounter)


%template(PerformanceCounterInstancePtr) BR::SharedPointerT<BR::Svr::PerformanceCounterInstance>;
%template(ArrayPerformanceCounterInstancePtr) BR::Array<BR::SharedPointerT<BR::Svr::PerformanceCounterInstance>>;

%typemap(csout, excode=SWIGEXCODE) BR::Array<BR::SharedPointerT<BR::Svr::PerformanceCounterInstance>>* 
	"{
		var result = $imcall; 
		return result == null ? null : new ArrayPerformanceCounterInstancePtr(result,true);
	}"
//INOUT_CLASS_REFERENCE(BR::Array<BR::SharedPointerT<BR::Svr::PerformanceCounterInstance>>,ArrayPerformanceCounterInstancePtr)

%typemap(csout, excode=SWIGEXCODE) BR::SharedPointerT<BR::Svr::PerformanceCounterInstance>* 
	"{
		var result = $imcall; 
		return result == null ? null : new PerformanceCounterInstancePtr(result,true);
	}"


%typemap(ctype)   BR::Svr::PerformanceCounterInstance** "BR::Svr::PerformanceCounterInstance**"
%typemap(cstype)  BR::Svr::PerformanceCounterInstance** "PerformanceCounterInstance[]"
%typemap(imtype, inattributes="[System.Runtime.InteropServices.In, System.Runtime.InteropServices.Out, global::System.Runtime.InteropServices.MarshalAs(global::System.Runtime.InteropServices.UnmanagedType.LPArray)]") BR::Svr::PerformanceCounterInstance** "PerformanceCounterInstance[]"
%typemap(csin)    BR::Svr::PerformanceCounterInstance** "$csinput"
%typemap(in)      BR::Svr::PerformanceCounterInstance** "$1 = $input;"
%typemap(freearg) BR::Svr::PerformanceCounterInstance** ""
%typemap(argout)  BR::Svr::PerformanceCounterInstance** ""
//%apply PerformanceCounterInstance FIXED[] { BR::Svr::PerformanceCounterInstance** pInstanceBuffer }
//%apply PerformanceCounterInstance FIXED[] { BR::Svr::PerformanceCounterInstance** pInstanceBuffer }
//%apply UINT INOUT[] { UINT& bufferSize }
//%apply UINT64 OUT[] { UINT64* instanceList }

%include "ServerSystem/PerformanceCounter/PerformanceCounterServer.h"

%{
#include "Protocol/Policy/MonitoringIPolicy.h"
#include "Protocol/Policy/MonitoringNetPolicy.h"
%}

//%include "Protocol/Policy/MonitoringIPolicy.h"
%include "Protocol/Policy/MonitoringNetPolicy.h"

%{
#include "Protocol/Policy/RankingIPolicy.h"
#include "Protocol/Policy/RankingNetPolicy.h"
%}
//%include "Protocol/Policy/RankingIPolicy.h"
%include "Protocol/Policy/RankingNetPolicy.h"





%{
#include "..\..\BRMonitoring\BRMonitoring.h"
%}


%include "..\..\BRMonitoring\BRMonitoring.h"




