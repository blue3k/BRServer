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

public class NetPolicyMonitoring : global::System.IDisposable {
  private global::System.Runtime.InteropServices.HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal NetPolicyMonitoring(global::System.IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
  }

  internal static global::System.Runtime.InteropServices.HandleRef getCPtr(NetPolicyMonitoring obj) {
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

  public unsafe NetPolicyMonitoring(IConnection pConn) : this(BRMonitoringPINVOKE.new_NetPolicyMonitoring(IConnection.getCPtr(pConn)), true) {
  }

  public unsafe int GetInstanceListCmd(ulong InContext) {
    int ret = BRMonitoringPINVOKE.NetPolicyMonitoring_GetInstanceListCmd(swigCPtr, InContext);
    return ret;
  }

  public unsafe int RequestCounterValuesCmd(ulong InContext, EntityUID InInstanceUID) {
    int ret = BRMonitoringPINVOKE.NetPolicyMonitoring_RequestCounterValuesCmd(swigCPtr, InContext, EntityUID.getCPtr(InInstanceUID));
    if (BRMonitoringPINVOKE.SWIGPendingException.Pending) throw BRMonitoringPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public unsafe int PerformanceCounterNewC2SEvt(string InInstanceName, EntityUID InInstanceUID, ArrayPerformanceCounterInfo InNewCounters) {
    int ret = BRMonitoringPINVOKE.NetPolicyMonitoring_PerformanceCounterNewC2SEvt(swigCPtr, InInstanceName, EntityUID.getCPtr(InInstanceUID), ArrayPerformanceCounterInfo.getCPtr(InNewCounters));
    if (BRMonitoringPINVOKE.SWIGPendingException.Pending) throw BRMonitoringPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public unsafe int PerformanceCounterFreeC2SEvt(ArrayEntityUID InFreeInstances) {
    int ret = BRMonitoringPINVOKE.NetPolicyMonitoring_PerformanceCounterFreeC2SEvt(swigCPtr, ArrayEntityUID.getCPtr(InFreeInstances));
    if (BRMonitoringPINVOKE.SWIGPendingException.Pending) throw BRMonitoringPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public unsafe int PerformanceCounterUpdateC2SEvt(EntityUID InInstanceUID, ArrayUInt64 InCounterValues) {
    int ret = BRMonitoringPINVOKE.NetPolicyMonitoring_PerformanceCounterUpdateC2SEvt(swigCPtr, EntityUID.getCPtr(InInstanceUID), ArrayUInt64.getCPtr(InCounterValues));
    if (BRMonitoringPINVOKE.SWIGPendingException.Pending) throw BRMonitoringPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

}

}