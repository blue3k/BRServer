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

public class EntityID : global::System.IDisposable {
  private global::System.Runtime.InteropServices.HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal EntityID(global::System.IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
  }

  internal static global::System.Runtime.InteropServices.HandleRef getCPtr(EntityID obj) {
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
      BRMonitoringPINVOKE.EntityID_ID_set(swigCPtr, value);
    } 
    get {
      uint ret = BRMonitoringPINVOKE.EntityID_ID_get(swigCPtr);
      return ret;
    } 
  }

  public unsafe EntityID() : this(BRMonitoringPINVOKE.new_EntityID__SWIG_0(), true) {
  }

  public unsafe EntityID(EntityUID entityUID) : this(BRMonitoringPINVOKE.new_EntityID__SWIG_1(EntityUID.getCPtr(entityUID)), true) {
    if (BRMonitoringPINVOKE.SWIGPendingException.Pending) throw BRMonitoringPINVOKE.SWIGPendingException.Retrieve();
  }

  public unsafe EntityID(EntityID entityID) : this(BRMonitoringPINVOKE.new_EntityID__SWIG_2(EntityID.getCPtr(entityID)), true) {
    if (BRMonitoringPINVOKE.SWIGPendingException.Pending) throw BRMonitoringPINVOKE.SWIGPendingException.Retrieve();
  }

  public unsafe EntityID(uint uiID) : this(BRMonitoringPINVOKE.new_EntityID__SWIG_3(uiID), true) {
  }

  public unsafe uint GetEntityLID() {
    uint ret = BRMonitoringPINVOKE.EntityID_GetEntityLID(swigCPtr);
    return ret;
  }

  public unsafe uint GetFacultyID() {
    uint ret = BRMonitoringPINVOKE.EntityID_GetFacultyID(swigCPtr);
    return ret;
  }

  public unsafe EntityID CopyFrom(EntityID entityID) {
    EntityID ret = new EntityID(BRMonitoringPINVOKE.EntityID_CopyFrom(swigCPtr, EntityID.getCPtr(entityID)), false);
    if (BRMonitoringPINVOKE.SWIGPendingException.Pending) throw BRMonitoringPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public unsafe static readonly int MAX_IDBIT = BRMonitoringPINVOKE.EntityID_MAX_IDBIT_get();

}

}
