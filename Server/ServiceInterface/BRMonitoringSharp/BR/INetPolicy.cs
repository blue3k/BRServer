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

public class INetPolicy : IPolicyBase {
  private global::System.Runtime.InteropServices.HandleRef swigCPtr;

  internal INetPolicy(global::System.IntPtr cPtr, bool cMemoryOwn) : base(BRMonitoringPINVOKE.INetPolicy_SWIGUpcast(cPtr), cMemoryOwn) {
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
  }

  internal static global::System.Runtime.InteropServices.HandleRef getCPtr(INetPolicy obj) {
    return (obj == null) ? new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero) : obj.swigCPtr;
  }

  ~INetPolicy() {
    Dispose();
  }

  public override void Dispose() {
    lock(this) {
      if (swigCPtr.Handle != global::System.IntPtr.Zero) {
        if (swigCMemOwn) {
          swigCMemOwn = false;
          BRMonitoringPINVOKE.delete_INetPolicy(swigCPtr);
        }
        swigCPtr = new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero);
      }
      global::System.GC.SuppressFinalize(this);
      base.Dispose();
    }
  }

  public unsafe INetPolicy(uint uiPolicyID, IConnection pConn) : this(BRMonitoringPINVOKE.new_INetPolicy(uiPolicyID, IConnection.getCPtr(pConn)), true) {
  }

  public unsafe IConnection GetConnection() {
    global::System.IntPtr cPtr = BRMonitoringPINVOKE.INetPolicy_GetConnection(swigCPtr);
    IConnection ret = (cPtr == global::System.IntPtr.Zero) ? null : new IConnection(cPtr, false);
    return ret;
  }

}

}
