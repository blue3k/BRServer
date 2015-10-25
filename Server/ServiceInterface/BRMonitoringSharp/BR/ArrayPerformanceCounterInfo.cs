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

public class ArrayPerformanceCounterInfo : global::System.IDisposable {
  private global::System.Runtime.InteropServices.HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal ArrayPerformanceCounterInfo(global::System.IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
  }

  internal static global::System.Runtime.InteropServices.HandleRef getCPtr(ArrayPerformanceCounterInfo obj) {
    return (obj == null) ? new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero) : obj.swigCPtr;
  }

  ~ArrayPerformanceCounterInfo() {
    Dispose();
  }

  public virtual void Dispose() {
    lock(this) {
      if (swigCPtr.Handle != global::System.IntPtr.Zero) {
        if (swigCMemOwn) {
          swigCMemOwn = false;
          BRMonitoringPINVOKE.delete_ArrayPerformanceCounterInfo(swigCPtr);
        }
        swigCPtr = new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero);
      }
      global::System.GC.SuppressFinalize(this);
    }
  }

  public unsafe uint GetSize() {
    uint ret = BRMonitoringPINVOKE.ArrayPerformanceCounterInfo_GetSize(swigCPtr);
    return ret;
  }

  public unsafe uint size() {
    uint ret = BRMonitoringPINVOKE.ArrayPerformanceCounterInfo_size(swigCPtr);
    return ret;
  }

  public unsafe int SetSize(uint szNewSize) {
    int ret = BRMonitoringPINVOKE.ArrayPerformanceCounterInfo_SetSize(swigCPtr, szNewSize);
    return ret;
  }

  public unsafe void resize(uint szNewSize) {
    BRMonitoringPINVOKE.ArrayPerformanceCounterInfo_resize(swigCPtr, szNewSize);
  }

  public unsafe void Clear() {
    BRMonitoringPINVOKE.ArrayPerformanceCounterInfo_Clear(swigCPtr);
  }

  public unsafe bool GetConserveDataOnResize() {
    bool ret = BRMonitoringPINVOKE.ArrayPerformanceCounterInfo_GetConserveDataOnResize(swigCPtr);
    return ret;
  }

  public unsafe void SetConserveDataOnResize(bool conserveDataOnResize) {
    BRMonitoringPINVOKE.ArrayPerformanceCounterInfo_SetConserveDataOnResize(swigCPtr, conserveDataOnResize);
  }

  public unsafe uint GetAllocatedSize() {
    uint ret = BRMonitoringPINVOKE.ArrayPerformanceCounterInfo_GetAllocatedSize(swigCPtr);
    return ret;
  }

  public unsafe uint GetIncreaseSize() {
    uint ret = BRMonitoringPINVOKE.ArrayPerformanceCounterInfo_GetIncreaseSize(swigCPtr);
    return ret;
  }

  public unsafe void SetIncreaseSize(uint szNewIncSize) {
    BRMonitoringPINVOKE.ArrayPerformanceCounterInfo_SetIncreaseSize(swigCPtr, szNewIncSize);
  }

  public unsafe int Reserve(uint szReserv) {
    int ret = BRMonitoringPINVOKE.ArrayPerformanceCounterInfo_Reserve(swigCPtr, szReserv);
    return ret;
  }

  public unsafe PerformanceCounterInfo data() {
    global::System.IntPtr cPtr = BRMonitoringPINVOKE.ArrayPerformanceCounterInfo_data(swigCPtr);
    PerformanceCounterInfo ret = (cPtr == global::System.IntPtr.Zero) ? null : new PerformanceCounterInfo(cPtr, false);
    return ret;
  }

  public unsafe int push_back(PerformanceCounterInfo NewData) {
    int ret = BRMonitoringPINVOKE.ArrayPerformanceCounterInfo_push_back(swigCPtr, PerformanceCounterInfo.getCPtr(NewData));
    if (BRMonitoringPINVOKE.SWIGPendingException.Pending) throw BRMonitoringPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public unsafe int AddItems(uint numItems, PerformanceCounterInfo NewData) {
    int ret = BRMonitoringPINVOKE.ArrayPerformanceCounterInfo_AddItems(swigCPtr, numItems, PerformanceCounterInfo.getCPtr(NewData));
    return ret;
  }

  public unsafe int RemoveAt(int iIndex) {
    int ret = BRMonitoringPINVOKE.ArrayPerformanceCounterInfo_RemoveAt(swigCPtr, iIndex);
    return ret;
  }

  public unsafe int RemoveItem(PerformanceCounterInfo RemoveData) {
    int ret = BRMonitoringPINVOKE.ArrayPerformanceCounterInfo_RemoveItem(swigCPtr, PerformanceCounterInfo.getCPtr(RemoveData));
    if (BRMonitoringPINVOKE.SWIGPendingException.Pending) throw BRMonitoringPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public unsafe int FindItem(PerformanceCounterInfo FindData) {
    int ret = BRMonitoringPINVOKE.ArrayPerformanceCounterInfo_FindItem(swigCPtr, PerformanceCounterInfo.getCPtr(FindData));
    if (BRMonitoringPINVOKE.SWIGPendingException.Pending) throw BRMonitoringPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public unsafe PerformanceCounterInfo GetAt(uint iElement) {
    PerformanceCounterInfo ret = new PerformanceCounterInfo(BRMonitoringPINVOKE.ArrayPerformanceCounterInfo_GetAt(swigCPtr, iElement), false);
    return ret;
  }

}

}