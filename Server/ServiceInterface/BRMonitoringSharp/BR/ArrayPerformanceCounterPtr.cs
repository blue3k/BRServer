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

public class ArrayPerformanceCounterPtr : global::System.IDisposable {
  private global::System.Runtime.InteropServices.HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal ArrayPerformanceCounterPtr(global::System.IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
  }

  internal static global::System.Runtime.InteropServices.HandleRef getCPtr(ArrayPerformanceCounterPtr obj) {
    return (obj == null) ? new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero) : obj.swigCPtr;
  }

  ~ArrayPerformanceCounterPtr() {
    Dispose();
  }

  public virtual void Dispose() {
    lock(this) {
      if (swigCPtr.Handle != global::System.IntPtr.Zero) {
        if (swigCMemOwn) {
          swigCMemOwn = false;
          BRMonitoringPINVOKE.delete_ArrayPerformanceCounterPtr(swigCPtr);
        }
        swigCPtr = new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero);
      }
      global::System.GC.SuppressFinalize(this);
    }
  }

  public unsafe uint GetSize() {
    uint ret = BRMonitoringPINVOKE.ArrayPerformanceCounterPtr_GetSize(swigCPtr);
    return ret;
  }

  public unsafe uint size() {
    uint ret = BRMonitoringPINVOKE.ArrayPerformanceCounterPtr_size(swigCPtr);
    return ret;
  }

  public unsafe int SetSize(uint szNewSize) {
    int ret = BRMonitoringPINVOKE.ArrayPerformanceCounterPtr_SetSize(swigCPtr, szNewSize);
    return ret;
  }

  public unsafe void resize(uint szNewSize) {
    BRMonitoringPINVOKE.ArrayPerformanceCounterPtr_resize(swigCPtr, szNewSize);
  }

  public unsafe void Clear() {
    BRMonitoringPINVOKE.ArrayPerformanceCounterPtr_Clear(swigCPtr);
  }

  public unsafe bool GetConserveDataOnResize() {
    bool ret = BRMonitoringPINVOKE.ArrayPerformanceCounterPtr_GetConserveDataOnResize(swigCPtr);
    return ret;
  }

  public unsafe void SetConserveDataOnResize(bool conserveDataOnResize) {
    BRMonitoringPINVOKE.ArrayPerformanceCounterPtr_SetConserveDataOnResize(swigCPtr, conserveDataOnResize);
  }

  public unsafe uint GetAllocatedSize() {
    uint ret = BRMonitoringPINVOKE.ArrayPerformanceCounterPtr_GetAllocatedSize(swigCPtr);
    return ret;
  }

  public unsafe uint GetIncreaseSize() {
    uint ret = BRMonitoringPINVOKE.ArrayPerformanceCounterPtr_GetIncreaseSize(swigCPtr);
    return ret;
  }

  public unsafe void SetIncreaseSize(uint szNewIncSize) {
    BRMonitoringPINVOKE.ArrayPerformanceCounterPtr_SetIncreaseSize(swigCPtr, szNewIncSize);
  }

  public unsafe int Reserve(uint szReserv) {
    int ret = BRMonitoringPINVOKE.ArrayPerformanceCounterPtr_Reserve(swigCPtr, szReserv);
    return ret;
  }

  public unsafe SWIGTYPE_p_p_BR__Svr__PerformanceCounter data() {
    global::System.IntPtr cPtr = BRMonitoringPINVOKE.ArrayPerformanceCounterPtr_data(swigCPtr);
    SWIGTYPE_p_p_BR__Svr__PerformanceCounter ret = (cPtr == global::System.IntPtr.Zero) ? null : new SWIGTYPE_p_p_BR__Svr__PerformanceCounter(cPtr, false);
    return ret;
  }

  public unsafe int push_back(PerformanceCounter NewData) {
    int ret = BRMonitoringPINVOKE.ArrayPerformanceCounterPtr_push_back(swigCPtr, PerformanceCounter.getCPtr(NewData));
    return ret;
  }

  public unsafe int AddItems(uint numItems, SWIGTYPE_p_p_BR__Svr__PerformanceCounter NewData) {
    int ret = BRMonitoringPINVOKE.ArrayPerformanceCounterPtr_AddItems(swigCPtr, numItems, SWIGTYPE_p_p_BR__Svr__PerformanceCounter.getCPtr(NewData));
    return ret;
  }

  public unsafe int RemoveAt(int iIndex) {
    int ret = BRMonitoringPINVOKE.ArrayPerformanceCounterPtr_RemoveAt(swigCPtr, iIndex);
    return ret;
  }

  public unsafe int RemoveItem(PerformanceCounter RemoveData) {
    int ret = BRMonitoringPINVOKE.ArrayPerformanceCounterPtr_RemoveItem(swigCPtr, PerformanceCounter.getCPtr(RemoveData));
    return ret;
  }

  public unsafe int FindItem(PerformanceCounter FindData) {
    int ret = BRMonitoringPINVOKE.ArrayPerformanceCounterPtr_FindItem(swigCPtr, PerformanceCounter.getCPtr(FindData));
    return ret;
  }

  public unsafe PerformanceCounter GetAt(uint iElement) {
    global::System.IntPtr cPtr = BRMonitoringPINVOKE.ArrayPerformanceCounterPtr_GetAt(swigCPtr, iElement);
    PerformanceCounter ret = (cPtr == global::System.IntPtr.Zero) ? null : new PerformanceCounter(cPtr, false);
    return ret;
  }

}

}
