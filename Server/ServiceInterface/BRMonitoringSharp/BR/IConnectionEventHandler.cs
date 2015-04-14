//------------------------------------------------------------------------------
// <auto-generated />
//
// This file was automatically generated by SWIG (http://www.swig.org).
// Version 3.0.5
//
// Do not make changes to this file unless you know what you are doing--modify
// the SWIG interface file instead.
//------------------------------------------------------------------------------

namespace BR {

public class IConnectionEventHandler : global::System.IDisposable {
  private global::System.Runtime.InteropServices.HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal IConnectionEventHandler(global::System.IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
  }

  internal static global::System.Runtime.InteropServices.HandleRef getCPtr(IConnectionEventHandler obj) {
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

  public unsafe void OnConnectionEvent(IConnection pConn, IConnection.tag_Event evt) {
    BRMonitoringPINVOKE.IConnectionEventHandler_OnConnectionEvent(swigCPtr, IConnection.getCPtr(pConn), IConnection.tag_Event.getCPtr(evt));
    if (BRMonitoringPINVOKE.SWIGPendingException.Pending) throw BRMonitoringPINVOKE.SWIGPendingException.Retrieve();
  }

  public unsafe int OnRecvMessage(IConnection pConn, MessageData pMsg) {
    int ret = BRMonitoringPINVOKE.IConnectionEventHandler_OnRecvMessage(swigCPtr, IConnection.getCPtr(pConn), MessageData.getCPtr(pMsg));
    return ret;
  }

  public unsafe int OnNetSyncMessage(IConnection pConn, NetCtrlIDs netCtrlID) {
    int ret = BRMonitoringPINVOKE.IConnectionEventHandler_OnNetSyncMessage(swigCPtr, IConnection.getCPtr(pConn), (int)netCtrlID);
    return ret;
  }

}

}
