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

public class EntityUID : global::System.IDisposable {
  private global::System.Runtime.InteropServices.HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal EntityUID(global::System.IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
  }

  internal static global::System.Runtime.InteropServices.HandleRef getCPtr(EntityUID obj) {
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

  public unsafe EntityID EntityID {
    set {
      BRMonitoringPINVOKE.EntityUID_EntityID_set(swigCPtr, EntityID.getCPtr(value));
    } 
    get {
      global::System.IntPtr cPtr = BRMonitoringPINVOKE.EntityUID_EntityID_get(swigCPtr);
      EntityID ret = (cPtr == global::System.IntPtr.Zero) ? null : new EntityID(cPtr, false);
      return ret;
    } 
  }

  public unsafe uint SvrID {
    set {
      BRMonitoringPINVOKE.EntityUID_SvrID_set(swigCPtr, value);
    } 
    get {
      uint ret = BRMonitoringPINVOKE.EntityUID_SvrID_get(swigCPtr);
      return ret;
    } 
  }

  public unsafe ulong UID {
    set {
      BRMonitoringPINVOKE.EntityUID_UID_set(swigCPtr, value);
    } 
    get {
      ulong ret = BRMonitoringPINVOKE.EntityUID_UID_get(swigCPtr);
      return ret;
    } 
  }

  public unsafe EntityUID() : this(BRMonitoringPINVOKE.new_EntityUID__SWIG_0(), true) {
  }

  public unsafe EntityUID(EntityUID entityUID) : this(BRMonitoringPINVOKE.new_EntityUID__SWIG_1(EntityUID.getCPtr(entityUID)), true) {
    if (BRMonitoringPINVOKE.SWIGPendingException.Pending) throw BRMonitoringPINVOKE.SWIGPendingException.Retrieve();
  }

  public unsafe EntityUID(uint serverID, uint entityID) : this(BRMonitoringPINVOKE.new_EntityUID__SWIG_2(serverID, entityID), true) {
  }

  public unsafe EntityUID(ulong context) : this(BRMonitoringPINVOKE.new_EntityUID__SWIG_3(context), true) {
  }

  public unsafe EntityUID CopyFrom(EntityUID entityUID) {
    EntityUID ret = new EntityUID(BRMonitoringPINVOKE.EntityUID_CopyFrom(swigCPtr, EntityUID.getCPtr(entityUID)), false);
    if (BRMonitoringPINVOKE.SWIGPendingException.Pending) throw BRMonitoringPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public unsafe bool EqualTo(EntityUID src) {
    bool ret = BRMonitoringPINVOKE.EntityUID_EqualTo(swigCPtr, EntityUID.getCPtr(src));
    if (BRMonitoringPINVOKE.SWIGPendingException.Pending) throw BRMonitoringPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public unsafe bool NotEqualTo(EntityUID src) {
    bool ret = BRMonitoringPINVOKE.EntityUID_NotEqualTo(swigCPtr, EntityUID.getCPtr(src));
    if (BRMonitoringPINVOKE.SWIGPendingException.Pending) throw BRMonitoringPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

}

}
