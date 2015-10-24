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

public class ArrayUInt64 : global::System.IDisposable {
  private global::System.Runtime.InteropServices.HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal ArrayUInt64(global::System.IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
  }

  internal static global::System.Runtime.InteropServices.HandleRef getCPtr(ArrayUInt64 obj) {
    return (obj == null) ? new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero) : obj.swigCPtr;
  }

  ~ArrayUInt64() {
    Dispose();
  }

  public virtual void Dispose() {
    lock(this) {
      if (swigCPtr.Handle != global::System.IntPtr.Zero) {
        if (swigCMemOwn) {
          swigCMemOwn = false;
          BRMonitoringPINVOKE.delete_ArrayUInt64(swigCPtr);
        }
        swigCPtr = new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero);
      }
      global::System.GC.SuppressFinalize(this);
    }
  }

  public unsafe uint GetSize() {
    uint ret = BRMonitoringPINVOKE.ArrayUInt64_GetSize(swigCPtr);
    return ret;
  }

  public unsafe uint size() {
    uint ret = BRMonitoringPINVOKE.ArrayUInt64_size(swigCPtr);
    return ret;
  }

  public unsafe int SetSize(uint szNewSize) {
    int ret = BRMonitoringPINVOKE.ArrayUInt64_SetSize(swigCPtr, szNewSize);
    return ret;
  }

  public unsafe void resize(uint szNewSize) {
    BRMonitoringPINVOKE.ArrayUInt64_resize(swigCPtr, szNewSize);
  }

  public unsafe void Clear() {
    BRMonitoringPINVOKE.ArrayUInt64_Clear(swigCPtr);
  }

  public unsafe bool GetConserveDataOnResize() {
    bool ret = BRMonitoringPINVOKE.ArrayUInt64_GetConserveDataOnResize(swigCPtr);
    return ret;
  }

  public unsafe void SetConserveDataOnResize(bool conserveDataOnResize) {
    BRMonitoringPINVOKE.ArrayUInt64_SetConserveDataOnResize(swigCPtr, conserveDataOnResize);
  }

  public unsafe uint GetAllocatedSize() {
    uint ret = BRMonitoringPINVOKE.ArrayUInt64_GetAllocatedSize(swigCPtr);
    return ret;
  }

  public unsafe uint GetIncreaseSize() {
    uint ret = BRMonitoringPINVOKE.ArrayUInt64_GetIncreaseSize(swigCPtr);
    return ret;
  }

  public unsafe void SetIncreaseSize(uint szNewIncSize) {
    BRMonitoringPINVOKE.ArrayUInt64_SetIncreaseSize(swigCPtr, szNewIncSize);
  }

  public unsafe int Reserve(uint szReserv) {
    int ret = BRMonitoringPINVOKE.ArrayUInt64_Reserve(swigCPtr, szReserv);
    return ret;
  }

  public unsafe SWIGTYPE_p_unsigned___int64 data() {
    global::System.IntPtr cPtr = BRMonitoringPINVOKE.ArrayUInt64_data(swigCPtr);
    SWIGTYPE_p_unsigned___int64 ret = (cPtr == global::System.IntPtr.Zero) ? null : new SWIGTYPE_p_unsigned___int64(cPtr, false);
    return ret;
  }

  public unsafe int push_back(ulong NewData) {
    int ret = BRMonitoringPINVOKE.ArrayUInt64_push_back(swigCPtr, NewData);
    return ret;
  }

  public unsafe int AddItems(uint numItems, SWIGTYPE_p_unsigned___int64 NewData) {
    int ret = BRMonitoringPINVOKE.ArrayUInt64_AddItems(swigCPtr, numItems, SWIGTYPE_p_unsigned___int64.getCPtr(NewData));
    return ret;
  }

  public unsafe int RemoveAt(int iIndex) {
    int ret = BRMonitoringPINVOKE.ArrayUInt64_RemoveAt(swigCPtr, iIndex);
    return ret;
  }

  public unsafe int RemoveItem(ulong RemoveData) {
    int ret = BRMonitoringPINVOKE.ArrayUInt64_RemoveItem(swigCPtr, RemoveData);
    return ret;
  }

  public unsafe int FindItem(ulong FindData) {
    int ret = BRMonitoringPINVOKE.ArrayUInt64_FindItem(swigCPtr, FindData);
    return ret;
  }

  public unsafe ulong GetAt(uint iElement) {
    ulong ret = BRMonitoringPINVOKE.ArrayUInt64_GetAt(swigCPtr, iElement);
    return ret;
  }

}

}
