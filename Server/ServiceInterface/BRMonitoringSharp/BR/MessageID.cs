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

public class MessageID : global::System.IDisposable {
  private global::System.Runtime.InteropServices.HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal MessageID(global::System.IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
  }

  internal static global::System.Runtime.InteropServices.HandleRef getCPtr(MessageID obj) {
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

  public unsafe uint ID {
    set {
      BRMonitoringPINVOKE.MessageID_ID_set(swigCPtr, value);
    } 
    get {
      uint ret = BRMonitoringPINVOKE.MessageID_ID_get(swigCPtr);
      return ret;
    } 
  }

  public unsafe MessageID() : this(BRMonitoringPINVOKE.new_MessageID__SWIG_0(), true) {
  }

  public unsafe MessageID(uint uiID) : this(BRMonitoringPINVOKE.new_MessageID__SWIG_1(uiID), true) {
  }

  public unsafe MessageID(uint uiType, uint uiReliability, uint uiMobility, uint uiPolicy, uint uiCode) : this(BRMonitoringPINVOKE.new_MessageID__SWIG_2(uiType, uiReliability, uiMobility, uiPolicy, uiCode), true) {
  }

  public unsafe uint SetMessageID(uint uiType, uint uiReliability, uint uiMobility, uint uiPolicy, uint uiCode) {
    uint ret = BRMonitoringPINVOKE.MessageID_SetMessageID(swigCPtr, uiType, uiReliability, uiMobility, uiPolicy, uiCode);
    return ret;
  }

  public unsafe uint GetMsgID() {
    uint ret = BRMonitoringPINVOKE.MessageID_GetMsgID(swigCPtr);
    return ret;
  }

}

}
