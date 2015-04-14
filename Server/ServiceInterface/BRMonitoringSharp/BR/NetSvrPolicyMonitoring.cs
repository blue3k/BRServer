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

public class NetSvrPolicyMonitoring : global::System.IDisposable {
  private global::System.Runtime.InteropServices.HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal NetSvrPolicyMonitoring(global::System.IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
  }

  internal static global::System.Runtime.InteropServices.HandleRef getCPtr(NetSvrPolicyMonitoring obj) {
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

  public unsafe NetSvrPolicyMonitoring(IConnection pConn) : this(BRMonitoringPINVOKE.new_NetSvrPolicyMonitoring(IConnection.getCPtr(pConn)), true) {
  }

  public unsafe int GetInstanceListRes(ulong InContext, int InResult, ArrayPerformanceCounterInstanceInfo InCounterInstances, uint InTotalInstanceCount) {
    int ret = BRMonitoringPINVOKE.NetSvrPolicyMonitoring_GetInstanceListRes(swigCPtr, InContext, InResult, ArrayPerformanceCounterInstanceInfo.getCPtr(InCounterInstances), InTotalInstanceCount);
    if (BRMonitoringPINVOKE.SWIGPendingException.Pending) throw BRMonitoringPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public unsafe int RequestCounterValuesRes(ulong InContext, int InResult, EntityUID InInstanceUID, ArrayUInt64 InCounterValues) {
    int ret = BRMonitoringPINVOKE.NetSvrPolicyMonitoring_RequestCounterValuesRes(swigCPtr, InContext, InResult, EntityUID.getCPtr(InInstanceUID), ArrayUInt64.getCPtr(InCounterValues));
    if (BRMonitoringPINVOKE.SWIGPendingException.Pending) throw BRMonitoringPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public unsafe int PerformanceCounterUpdateCounterInfoS2CEvt(EntityUID InInstanceUID) {
    int ret = BRMonitoringPINVOKE.NetSvrPolicyMonitoring_PerformanceCounterUpdateCounterInfoS2CEvt(swigCPtr, EntityUID.getCPtr(InInstanceUID));
    if (BRMonitoringPINVOKE.SWIGPendingException.Pending) throw BRMonitoringPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

}

}
