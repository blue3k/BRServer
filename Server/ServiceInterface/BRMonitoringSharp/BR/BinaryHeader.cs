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

public class BinaryHeader : global::System.IDisposable {
  private global::System.Runtime.InteropServices.HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal BinaryHeader(global::System.IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
  }

  internal static global::System.Runtime.InteropServices.HandleRef getCPtr(BinaryHeader obj) {
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

  public unsafe ushort Magic {
    set {
      BRMonitoringPINVOKE.BinaryHeader_Magic_set(swigCPtr, value);
    } 
    get {
      ushort ret = BRMonitoringPINVOKE.BinaryHeader_Magic_get(swigCPtr);
      return ret;
    } 
  }

  public unsafe ushort Version {
    set {
      BRMonitoringPINVOKE.BinaryHeader_Version_set(swigCPtr, value);
    } 
    get {
      ushort ret = BRMonitoringPINVOKE.BinaryHeader_Version_get(swigCPtr);
      return ret;
    } 
  }

  public unsafe uint Count {
    set {
      BRMonitoringPINVOKE.BinaryHeader_Count_set(swigCPtr, value);
    } 
    get {
      uint ret = BRMonitoringPINVOKE.BinaryHeader_Count_get(swigCPtr);
      return ret;
    } 
  }

}

}
