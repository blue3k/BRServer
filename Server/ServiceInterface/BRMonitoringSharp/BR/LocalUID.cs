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

public class LocalUID : global::System.IDisposable {
  private global::System.Runtime.InteropServices.HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal LocalUID(global::System.IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
  }

  internal static global::System.Runtime.InteropServices.HandleRef getCPtr(LocalUID obj) {
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
      BRMonitoringPINVOKE.LocalUID_Time_set(swigCPtr, value);
    } 
    get {
      uint ret = BRMonitoringPINVOKE.LocalUID_Time_get(swigCPtr);
      return ret;
    } 
  }

  public unsafe uint ID {
    set {
      BRMonitoringPINVOKE.LocalUID_ID_set(swigCPtr, value);
    } 
    get {
      uint ret = BRMonitoringPINVOKE.LocalUID_ID_get(swigCPtr);
      return ret;
    } 
  }

  public unsafe ulong UID {
    set {
      BRMonitoringPINVOKE.LocalUID_UID_set(swigCPtr, value);
    } 
    get {
      ulong ret = BRMonitoringPINVOKE.LocalUID_UID_get(swigCPtr);
      return ret;
    } 
  }

  public unsafe LocalUID() : this(BRMonitoringPINVOKE.new_LocalUID__SWIG_0(), true) {
  }

  public unsafe LocalUID(LocalUID src) : this(BRMonitoringPINVOKE.new_LocalUID__SWIG_1(LocalUID.getCPtr(src)), true) {
    if (BRMonitoringPINVOKE.SWIGPendingException.Pending) throw BRMonitoringPINVOKE.SWIGPendingException.Retrieve();
  }

  public unsafe LocalUID(uint time, uint id) : this(BRMonitoringPINVOKE.new_LocalUID__SWIG_2(time, id), true) {
  }

  public unsafe LocalUID(ulong initValue) : this(BRMonitoringPINVOKE.new_LocalUID__SWIG_3(initValue), true) {
  }

  public unsafe LocalUID CopyFrom(LocalUID src) {
    LocalUID ret = new LocalUID(BRMonitoringPINVOKE.LocalUID_CopyFrom(swigCPtr, LocalUID.getCPtr(src)), false);
    if (BRMonitoringPINVOKE.SWIGPendingException.Pending) throw BRMonitoringPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public unsafe bool EqualTo(LocalUID op) {
    bool ret = BRMonitoringPINVOKE.LocalUID_EqualTo(swigCPtr, LocalUID.getCPtr(op));
    if (BRMonitoringPINVOKE.SWIGPendingException.Pending) throw BRMonitoringPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public unsafe bool NotEqualTo(LocalUID op) {
    bool ret = BRMonitoringPINVOKE.LocalUID_NotEqualTo(swigCPtr, LocalUID.getCPtr(op));
    if (BRMonitoringPINVOKE.SWIGPendingException.Pending) throw BRMonitoringPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

}

}