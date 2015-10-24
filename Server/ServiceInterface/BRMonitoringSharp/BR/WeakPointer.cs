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

public class WeakPointer : global::System.IDisposable {
  private global::System.Runtime.InteropServices.HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal WeakPointer(global::System.IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
  }

  internal static global::System.Runtime.InteropServices.HandleRef getCPtr(WeakPointer obj) {
    return (obj == null) ? new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero) : obj.swigCPtr;
  }

  ~WeakPointer() {
    Dispose();
  }

  public virtual void Dispose() {
    lock(this) {
      if (swigCPtr.Handle != global::System.IntPtr.Zero) {
        if (swigCMemOwn) {
          swigCMemOwn = false;
          BRMonitoringPINVOKE.delete_WeakPointer(swigCPtr);
        }
        swigCPtr = new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero);
      }
      global::System.GC.SuppressFinalize(this);
    }
  }

  public unsafe static WeakPointer NullValue {
    set {
      BRMonitoringPINVOKE.WeakPointer_NullValue_set(WeakPointer.getCPtr(value));
    } 
    get {
      global::System.IntPtr cPtr = BRMonitoringPINVOKE.WeakPointer_NullValue_get();
      WeakPointer ret = (cPtr == global::System.IntPtr.Zero) ? null : new WeakPointer(cPtr, false);
      return ret;
    } 
  }

  public unsafe void FromSharedObject(SharedObject pObject) {
    BRMonitoringPINVOKE.WeakPointer_FromSharedObject(swigCPtr, SharedObject.getCPtr(pObject));
  }

  public unsafe WeakPointer() : this(BRMonitoringPINVOKE.new_WeakPointer__SWIG_1(), true) {
  }

  public unsafe WeakPointer(WeakPointer src) : this(BRMonitoringPINVOKE.new_WeakPointer__SWIG_2(WeakPointer.getCPtr(src)), true) {
    if (BRMonitoringPINVOKE.SWIGPendingException.Pending) throw BRMonitoringPINVOKE.SWIGPendingException.Retrieve();
  }

  public unsafe void ReleaseReference() {
    BRMonitoringPINVOKE.WeakPointer_ReleaseReference(swigCPtr);
  }

  public unsafe SharedPointer ToSharedPointer() {
    SharedPointer ret = new SharedPointer(BRMonitoringPINVOKE.WeakPointer_ToSharedPointer(swigCPtr), true);
    return ret;
  }

  public unsafe bool EqualTo(SharedPointer src) {
    bool ret = BRMonitoringPINVOKE.WeakPointer_EqualTo__SWIG_0(swigCPtr, SharedPointer.getCPtr(src));
    if (BRMonitoringPINVOKE.SWIGPendingException.Pending) throw BRMonitoringPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public unsafe bool NotEqualTo(SharedPointer src) {
    bool ret = BRMonitoringPINVOKE.WeakPointer_NotEqualTo__SWIG_0(swigCPtr, SharedPointer.getCPtr(src));
    if (BRMonitoringPINVOKE.SWIGPendingException.Pending) throw BRMonitoringPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public unsafe bool EqualTo(WeakPointer src) {
    bool ret = BRMonitoringPINVOKE.WeakPointer_EqualTo__SWIG_1(swigCPtr, WeakPointer.getCPtr(src));
    if (BRMonitoringPINVOKE.SWIGPendingException.Pending) throw BRMonitoringPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public unsafe bool NotEqualTo(WeakPointer src) {
    bool ret = BRMonitoringPINVOKE.WeakPointer_NotEqualTo__SWIG_1(swigCPtr, WeakPointer.getCPtr(src));
    if (BRMonitoringPINVOKE.SWIGPendingException.Pending) throw BRMonitoringPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public unsafe bool EqualTo(SharedObject pRef) {
    bool ret = BRMonitoringPINVOKE.WeakPointer_EqualTo__SWIG_2(swigCPtr, SharedObject.getCPtr(pRef));
    return ret;
  }

  public unsafe bool NotEqualTo(SharedObject pRef) {
    bool ret = BRMonitoringPINVOKE.WeakPointer_NotEqualTo__SWIG_2(swigCPtr, SharedObject.getCPtr(pRef));
    return ret;
  }

  public unsafe WeakPointer CopyFrom(SharedPointer src) {
    WeakPointer ret = new WeakPointer(BRMonitoringPINVOKE.WeakPointer_CopyFrom__SWIG_0(swigCPtr, SharedPointer.getCPtr(src)), false);
    if (BRMonitoringPINVOKE.SWIGPendingException.Pending) throw BRMonitoringPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public unsafe WeakPointer CopyFrom(WeakPointer src) {
    WeakPointer ret = new WeakPointer(BRMonitoringPINVOKE.WeakPointer_CopyFrom__SWIG_1(swigCPtr, WeakPointer.getCPtr(src)), false);
    if (BRMonitoringPINVOKE.SWIGPendingException.Pending) throw BRMonitoringPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

}

}
