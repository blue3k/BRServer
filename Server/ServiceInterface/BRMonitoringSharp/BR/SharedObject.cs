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

public class SharedObject : global::System.IDisposable {
  private global::System.Runtime.InteropServices.HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal SharedObject(global::System.IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
  }

  internal static global::System.Runtime.InteropServices.HandleRef getCPtr(SharedObject obj) {
    return (obj == null) ? new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero) : obj.swigCPtr;
  }

  ~SharedObject() {
    Dispose();
  }

  public virtual void Dispose() {
    lock(this) {
      if (swigCPtr.Handle != global::System.IntPtr.Zero) {
        if (swigCMemOwn) {
          swigCMemOwn = false;
          BRMonitoringPINVOKE.delete_SharedObject(swigCPtr);
        }
        swigCPtr = new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero);
      }
      global::System.GC.SuppressFinalize(this);
    }
  }

  public unsafe SharedObject() : this(BRMonitoringPINVOKE.new_SharedObject(), true) {
  }

  public unsafe long GetReferenceCount() {
    long ret = BRMonitoringPINVOKE.SharedObject_GetReferenceCount(swigCPtr);
    return ret;
  }

  public unsafe long GetWeakReferenceCount() {
    long ret = BRMonitoringPINVOKE.SharedObject_GetWeakReferenceCount(swigCPtr);
    return ret;
  }

  public unsafe long GetManagerReferenceCount() {
    long ret = BRMonitoringPINVOKE.SharedObject_GetManagerReferenceCount(swigCPtr);
    return ret;
  }

  public unsafe SWIGTYPE_p_BR__SharedReferenceManager GetReferenceManager() {
    global::System.IntPtr cPtr = BRMonitoringPINVOKE.SharedObject_GetReferenceManager(swigCPtr);
    SWIGTYPE_p_BR__SharedReferenceManager ret = (cPtr == global::System.IntPtr.Zero) ? null : new SWIGTYPE_p_BR__SharedReferenceManager(cPtr, false);
    return ret;
  }

  public unsafe void NativeDispose() {
    BRMonitoringPINVOKE.SharedObject_NativeDispose(swigCPtr);
  }

}

}
