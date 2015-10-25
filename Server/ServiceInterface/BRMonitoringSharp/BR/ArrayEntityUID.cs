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

public class ArrayEntityUID : global::System.IDisposable {
  private global::System.Runtime.InteropServices.HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal ArrayEntityUID(global::System.IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
  }

  internal static global::System.Runtime.InteropServices.HandleRef getCPtr(ArrayEntityUID obj) {
    return (obj == null) ? new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero) : obj.swigCPtr;
  }

  ~ArrayEntityUID() {
    Dispose();
  }

  public virtual void Dispose() {
    lock(this) {
      if (swigCPtr.Handle != global::System.IntPtr.Zero) {
        if (swigCMemOwn) {
          swigCMemOwn = false;
          BRMonitoringPINVOKE.delete_ArrayEntityUID(swigCPtr);
        }
        swigCPtr = new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero);
      }
      global::System.GC.SuppressFinalize(this);
    }
  }

  public unsafe uint GetSize() {
    uint ret = BRMonitoringPINVOKE.ArrayEntityUID_GetSize(swigCPtr);
    return ret;
  }

  public unsafe uint size() {
    uint ret = BRMonitoringPINVOKE.ArrayEntityUID_size(swigCPtr);
    return ret;
  }

  public unsafe int SetSize(uint szNewSize) {
    int ret = BRMonitoringPINVOKE.ArrayEntityUID_SetSize(swigCPtr, szNewSize);
    return ret;
  }

  public unsafe void resize(uint szNewSize) {
    BRMonitoringPINVOKE.ArrayEntityUID_resize(swigCPtr, szNewSize);
  }

  public unsafe void Clear() {
    BRMonitoringPINVOKE.ArrayEntityUID_Clear(swigCPtr);
  }

  public unsafe bool GetConserveDataOnResize() {
    bool ret = BRMonitoringPINVOKE.ArrayEntityUID_GetConserveDataOnResize(swigCPtr);
    return ret;
  }

  public unsafe void SetConserveDataOnResize(bool conserveDataOnResize) {
    BRMonitoringPINVOKE.ArrayEntityUID_SetConserveDataOnResize(swigCPtr, conserveDataOnResize);
  }

  public unsafe uint GetAllocatedSize() {
    uint ret = BRMonitoringPINVOKE.ArrayEntityUID_GetAllocatedSize(swigCPtr);
    return ret;
  }

  public unsafe uint GetIncreaseSize() {
    uint ret = BRMonitoringPINVOKE.ArrayEntityUID_GetIncreaseSize(swigCPtr);
    return ret;
  }

  public unsafe void SetIncreaseSize(uint szNewIncSize) {
    BRMonitoringPINVOKE.ArrayEntityUID_SetIncreaseSize(swigCPtr, szNewIncSize);
  }

  public unsafe int Reserve(uint szReserv) {
    int ret = BRMonitoringPINVOKE.ArrayEntityUID_Reserve(swigCPtr, szReserv);
    return ret;
  }

  public unsafe EntityUID data() {
    global::System.IntPtr cPtr = BRMonitoringPINVOKE.ArrayEntityUID_data(swigCPtr);
    EntityUID ret = (cPtr == global::System.IntPtr.Zero) ? null : new EntityUID(cPtr, false);
    return ret;
  }

  public unsafe int push_back(EntityUID NewData) {
    int ret = BRMonitoringPINVOKE.ArrayEntityUID_push_back(swigCPtr, EntityUID.getCPtr(NewData));
    if (BRMonitoringPINVOKE.SWIGPendingException.Pending) throw BRMonitoringPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public unsafe int AddItems(uint numItems, EntityUID NewData) {
    int ret = BRMonitoringPINVOKE.ArrayEntityUID_AddItems(swigCPtr, numItems, EntityUID.getCPtr(NewData));
    return ret;
  }

  public unsafe int RemoveAt(int iIndex) {
    int ret = BRMonitoringPINVOKE.ArrayEntityUID_RemoveAt(swigCPtr, iIndex);
    return ret;
  }

  public unsafe int RemoveItem(EntityUID RemoveData) {
    int ret = BRMonitoringPINVOKE.ArrayEntityUID_RemoveItem(swigCPtr, EntityUID.getCPtr(RemoveData));
    if (BRMonitoringPINVOKE.SWIGPendingException.Pending) throw BRMonitoringPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public unsafe int FindItem(EntityUID FindData) {
    int ret = BRMonitoringPINVOKE.ArrayEntityUID_FindItem(swigCPtr, EntityUID.getCPtr(FindData));
    if (BRMonitoringPINVOKE.SWIGPendingException.Pending) throw BRMonitoringPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public unsafe EntityUID GetAt(uint iElement) {
    EntityUID ret = new EntityUID(BRMonitoringPINVOKE.ArrayEntityUID_GetAt(swigCPtr, iElement), false);
    return ret;
  }

}

}