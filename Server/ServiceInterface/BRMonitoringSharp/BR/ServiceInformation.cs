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

public class ServiceInformation : global::System.IDisposable {
  private global::System.Runtime.InteropServices.HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal ServiceInformation(global::System.IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
  }

  internal static global::System.Runtime.InteropServices.HandleRef getCPtr(ServiceInformation obj) {
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

  public unsafe EntityUID UID {
    set {
      BRMonitoringPINVOKE.ServiceInformation_UID_set(swigCPtr, EntityUID.getCPtr(value));
    } 
    get {
      global::System.IntPtr cPtr = BRMonitoringPINVOKE.ServiceInformation_UID_get(swigCPtr);
      EntityUID ret = (cPtr == global::System.IntPtr.Zero) ? null : new EntityUID(cPtr, false);
      return ret;
    } 
  }

  public unsafe ClusterMembership Membership {
    set {
      BRMonitoringPINVOKE.ServiceInformation_Membership_set(swigCPtr, (int)value);
    } 
    get {
      ClusterMembership ret = (ClusterMembership)BRMonitoringPINVOKE.ServiceInformation_Membership_get(swigCPtr);
      return ret;
    } 
  }

  public unsafe ServiceStatus Status {
    set {
      BRMonitoringPINVOKE.ServiceInformation_Status_set(swigCPtr, (int)value);
    } 
    get {
      ServiceStatus ret = (ServiceStatus)BRMonitoringPINVOKE.ServiceInformation_Status_get(swigCPtr);
      return ret;
    } 
  }

  public unsafe NetClass ServerClass {
    set {
      BRMonitoringPINVOKE.ServiceInformation_ServerClass_set(swigCPtr, (int)value);
    } 
    get {
      NetClass ret = (NetClass)BRMonitoringPINVOKE.ServiceInformation_ServerClass_get(swigCPtr);
      return ret;
    } 
  }

  public unsafe NetAddress ServerAddress {
    set {
      BRMonitoringPINVOKE.ServiceInformation_ServerAddress_set(swigCPtr, NetAddress.getCPtr(value));
    } 
    get {
      global::System.IntPtr cPtr = BRMonitoringPINVOKE.ServiceInformation_ServerAddress_get(swigCPtr);
      NetAddress ret = (cPtr == global::System.IntPtr.Zero) ? null : new NetAddress(cPtr, false);
      return ret;
    } 
  }

  public unsafe ulong ServerUpTime {
    set {
      BRMonitoringPINVOKE.ServiceInformation_ServerUpTime_set(swigCPtr, value);
    } 
    get {
      ulong ret = BRMonitoringPINVOKE.ServiceInformation_ServerUpTime_get(swigCPtr);
      return ret;
    } 
  }

  public unsafe uint Workload {
    set {
      BRMonitoringPINVOKE.ServiceInformation_Workload_set(swigCPtr, value);
    } 
    get {
      uint ret = BRMonitoringPINVOKE.ServiceInformation_Workload_get(swigCPtr);
      return ret;
    } 
  }

  public unsafe ServiceInformation() : this(BRMonitoringPINVOKE.new_ServiceInformation__SWIG_0(), true) {
  }

  public unsafe ServiceInformation(ServiceInformation src) : this(BRMonitoringPINVOKE.new_ServiceInformation__SWIG_1(ServiceInformation.getCPtr(src)), true) {
    if (BRMonitoringPINVOKE.SWIGPendingException.Pending) throw BRMonitoringPINVOKE.SWIGPendingException.Retrieve();
  }

  public unsafe ServiceInformation(EntityUID entityUID, ClusterMembership membership, ServiceStatus status, NetClass netClass, NetAddress address, ulong serverUpTime, uint workload) : this(BRMonitoringPINVOKE.new_ServiceInformation__SWIG_2(EntityUID.getCPtr(entityUID), (int)membership, (int)status, (int)netClass, NetAddress.getCPtr(address), serverUpTime, workload), true) {
    if (BRMonitoringPINVOKE.SWIGPendingException.Pending) throw BRMonitoringPINVOKE.SWIGPendingException.Retrieve();
  }

  public unsafe ServiceInformation(int initValue) : this(BRMonitoringPINVOKE.new_ServiceInformation__SWIG_3(initValue), true) {
  }

  public unsafe ServiceInformation CopyFrom(ServiceInformation src) {
    ServiceInformation ret = new ServiceInformation(BRMonitoringPINVOKE.ServiceInformation_CopyFrom(swigCPtr, ServiceInformation.getCPtr(src)), false);
    if (BRMonitoringPINVOKE.SWIGPendingException.Pending) throw BRMonitoringPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public unsafe bool EqualTo(ServiceInformation op) {
    bool ret = BRMonitoringPINVOKE.ServiceInformation_EqualTo(swigCPtr, ServiceInformation.getCPtr(op));
    if (BRMonitoringPINVOKE.SWIGPendingException.Pending) throw BRMonitoringPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

}

}
