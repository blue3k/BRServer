//------------------------------------------------------------------------------
// <auto-generated />
//
// This file was automatically generated by SWIG (http://www.swig.org).
// Version 3.0.6
//
// Do not make changes to this file unless you know what you are doing--modify
// the SWIG interface file instead.
//------------------------------------------------------------------------------

namespace BR {

public class PerformanceCounterInfo : global::System.IDisposable {
  private global::System.Runtime.InteropServices.HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal PerformanceCounterInfo(global::System.IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
  }

  internal static global::System.Runtime.InteropServices.HandleRef getCPtr(PerformanceCounterInfo obj) {
    return (obj == null) ? new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero) : obj.swigCPtr;
  }

  public virtual void Dispose() {
    lock(this) {
      if (swigCPtr.Handle != global::System.IntPtr.Zero) {
        if (swigCMemOwn) {
          swigCMemOwn = false;
          throw new global::System.MethodAccessException("C++ destructor does not have public access");
        }
        swigCPtr = new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero);
      }
      global::System.GC.SuppressFinalize(this);
    }
  }

  public unsafe uint DateType {
    set {
      BRMonitoringPINVOKE.PerformanceCounterInfo_DateType_set(swigCPtr, value);
    } 
    get {
      uint ret = BRMonitoringPINVOKE.PerformanceCounterInfo_DateType_get(swigCPtr);
      return ret;
    } 
  }

  public unsafe string CounterName {
    set {
      BRMonitoringPINVOKE.PerformanceCounterInfo_CounterName_set(swigCPtr, value);
    } 
    get {
      string ret = BRMonitoringPINVOKE.PerformanceCounterInfo_CounterName_get(swigCPtr);
      return ret;
    } 
  }

  public unsafe bool EqualTo(PerformanceCounterInfo op) {
    bool ret = BRMonitoringPINVOKE.PerformanceCounterInfo_EqualTo(swigCPtr, PerformanceCounterInfo.getCPtr(op));
    if (BRMonitoringPINVOKE.SWIGPendingException.Pending) throw BRMonitoringPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

}

}
