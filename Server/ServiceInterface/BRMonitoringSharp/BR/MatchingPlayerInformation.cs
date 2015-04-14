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

public class MatchingPlayerInformation : global::System.IDisposable {
  private global::System.Runtime.InteropServices.HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal MatchingPlayerInformation(global::System.IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
  }

  internal static global::System.Runtime.InteropServices.HandleRef getCPtr(MatchingPlayerInformation obj) {
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

  public unsafe EntityUID PlayerUID {
    set {
      BRMonitoringPINVOKE.MatchingPlayerInformation_PlayerUID_set(swigCPtr, EntityUID.getCPtr(value));
    } 
    get {
      global::System.IntPtr cPtr = BRMonitoringPINVOKE.MatchingPlayerInformation_PlayerUID_get(swigCPtr);
      EntityUID ret = (cPtr == global::System.IntPtr.Zero) ? null : new EntityUID(cPtr, false);
      return ret;
    } 
  }

  public unsafe ulong PlayerID {
    set {
      BRMonitoringPINVOKE.MatchingPlayerInformation_PlayerID_set(swigCPtr, value);
    } 
    get {
      ulong ret = BRMonitoringPINVOKE.MatchingPlayerInformation_PlayerID_get(swigCPtr);
      return ret;
    } 
  }

  public unsafe MatchingPlayerInformation() : this(BRMonitoringPINVOKE.new_MatchingPlayerInformation__SWIG_0(), true) {
  }

  public unsafe MatchingPlayerInformation(MatchingPlayerInformation src) : this(BRMonitoringPINVOKE.new_MatchingPlayerInformation__SWIG_1(MatchingPlayerInformation.getCPtr(src)), true) {
    if (BRMonitoringPINVOKE.SWIGPendingException.Pending) throw BRMonitoringPINVOKE.SWIGPendingException.Retrieve();
  }

  public unsafe MatchingPlayerInformation(EntityUID playerUID, ulong playerID) : this(BRMonitoringPINVOKE.new_MatchingPlayerInformation__SWIG_2(EntityUID.getCPtr(playerUID), playerID), true) {
    if (BRMonitoringPINVOKE.SWIGPendingException.Pending) throw BRMonitoringPINVOKE.SWIGPendingException.Retrieve();
  }

  public unsafe MatchingPlayerInformation(int initValue) : this(BRMonitoringPINVOKE.new_MatchingPlayerInformation__SWIG_3(initValue), true) {
  }

  public unsafe MatchingPlayerInformation CopyFrom(MatchingPlayerInformation src) {
    MatchingPlayerInformation ret = new MatchingPlayerInformation(BRMonitoringPINVOKE.MatchingPlayerInformation_CopyFrom(swigCPtr, MatchingPlayerInformation.getCPtr(src)), false);
    if (BRMonitoringPINVOKE.SWIGPendingException.Pending) throw BRMonitoringPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public unsafe bool EqualTo(MatchingPlayerInformation op) {
    bool ret = BRMonitoringPINVOKE.MatchingPlayerInformation_EqualTo(swigCPtr, MatchingPlayerInformation.getCPtr(op));
    if (BRMonitoringPINVOKE.SWIGPendingException.Pending) throw BRMonitoringPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

}

}
