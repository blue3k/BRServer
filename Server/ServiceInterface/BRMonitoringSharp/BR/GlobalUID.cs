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

public class GlobalUID : global::System.IDisposable {
  private global::System.Runtime.InteropServices.HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal GlobalUID(global::System.IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
  }

  internal static global::System.Runtime.InteropServices.HandleRef getCPtr(GlobalUID obj) {
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

  public unsafe uint Time {
    set {
      BRMonitoringPINVOKE.GlobalUID_Time_set(swigCPtr, value);
    } 
    get {
      uint ret = BRMonitoringPINVOKE.GlobalUID_Time_get(swigCPtr);
      return ret;
    } 
  }

  public unsafe uint ServerID {
    set {
      BRMonitoringPINVOKE.GlobalUID_ServerID_set(swigCPtr, value);
    } 
    get {
      uint ret = BRMonitoringPINVOKE.GlobalUID_ServerID_get(swigCPtr);
      return ret;
    } 
  }

  public unsafe uint ID {
    set {
      BRMonitoringPINVOKE.GlobalUID_ID_set(swigCPtr, value);
    } 
    get {
      uint ret = BRMonitoringPINVOKE.GlobalUID_ID_get(swigCPtr);
      return ret;
    } 
  }

  public unsafe ulong UID {
    set {
      BRMonitoringPINVOKE.GlobalUID_UID_set(swigCPtr, value);
    } 
    get {
      ulong ret = BRMonitoringPINVOKE.GlobalUID_UID_get(swigCPtr);
      return ret;
    } 
  }

  public unsafe GlobalUID() : this(BRMonitoringPINVOKE.new_GlobalUID__SWIG_0(), true) {
  }

  public unsafe GlobalUID(GlobalUID src) : this(BRMonitoringPINVOKE.new_GlobalUID__SWIG_1(GlobalUID.getCPtr(src)), true) {
    if (BRMonitoringPINVOKE.SWIGPendingException.Pending) throw BRMonitoringPINVOKE.SWIGPendingException.Retrieve();
  }

  public unsafe GlobalUID(uint serverID, uint time, uint id) : this(BRMonitoringPINVOKE.new_GlobalUID__SWIG_2(serverID, time, id), true) {
  }

  public unsafe GlobalUID(ulong initValue) : this(BRMonitoringPINVOKE.new_GlobalUID__SWIG_3(initValue), true) {
  }

  public unsafe GlobalUID CopyFrom(GlobalUID src) {
    GlobalUID ret = new GlobalUID(BRMonitoringPINVOKE.GlobalUID_CopyFrom(swigCPtr, GlobalUID.getCPtr(src)), false);
    if (BRMonitoringPINVOKE.SWIGPendingException.Pending) throw BRMonitoringPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public unsafe bool EqualTo(GlobalUID op) {
    bool ret = BRMonitoringPINVOKE.GlobalUID_EqualTo(swigCPtr, GlobalUID.getCPtr(op));
    if (BRMonitoringPINVOKE.SWIGPendingException.Pending) throw BRMonitoringPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public unsafe bool NotEqualTo(GlobalUID op) {
    bool ret = BRMonitoringPINVOKE.GlobalUID_NotEqualTo(swigCPtr, GlobalUID.getCPtr(op));
    if (BRMonitoringPINVOKE.SWIGPendingException.Pending) throw BRMonitoringPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

}

}