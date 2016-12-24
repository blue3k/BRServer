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

public class ArrayChar : global::System.IDisposable {
  private global::System.Runtime.InteropServices.HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal ArrayChar(global::System.IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
  }

  internal static global::System.Runtime.InteropServices.HandleRef getCPtr(ArrayChar obj) {
    return (obj == null) ? new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero) : obj.swigCPtr;
  }

  ~ArrayChar() {
    Dispose();
  }

  public virtual void Dispose() {
    lock(this) {
      if (swigCPtr.Handle != global::System.IntPtr.Zero) {
        if (swigCMemOwn) {
          swigCMemOwn = false;
          BRMonitoringPINVOKE.delete_ArrayChar(swigCPtr);
        }
        swigCPtr = new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero);
      }
      global::System.GC.SuppressFinalize(this);
    }
  }

  public unsafe uint GetSize() {
    uint ret = BRMonitoringPINVOKE.ArrayChar_GetSize(swigCPtr);
    return ret;
  }

  public unsafe uint size() {
    uint ret = BRMonitoringPINVOKE.ArrayChar_size(swigCPtr);
    return ret;
  }

  public unsafe SWIGTYPE_p_Result SetSize(uint szNewSize) {
    SWIGTYPE_p_Result ret = new SWIGTYPE_p_Result(BRMonitoringPINVOKE.ArrayChar_SetSize(swigCPtr, szNewSize), true);
    return ret;
  }

  public unsafe void resize(uint szNewSize) {
    BRMonitoringPINVOKE.ArrayChar_resize(swigCPtr, szNewSize);
  }

  public unsafe void Clear() {
    BRMonitoringPINVOKE.ArrayChar_Clear(swigCPtr);
  }

  public unsafe bool GetConserveDataOnResize() {
    bool ret = BRMonitoringPINVOKE.ArrayChar_GetConserveDataOnResize(swigCPtr);
    return ret;
  }

  public unsafe void SetConserveDataOnResize(bool conserveDataOnResize) {
    BRMonitoringPINVOKE.ArrayChar_SetConserveDataOnResize(swigCPtr, conserveDataOnResize);
  }

  public unsafe uint GetAllocatedSize() {
    uint ret = BRMonitoringPINVOKE.ArrayChar_GetAllocatedSize(swigCPtr);
    return ret;
  }

  public unsafe uint GetIncreaseSize() {
    uint ret = BRMonitoringPINVOKE.ArrayChar_GetIncreaseSize(swigCPtr);
    return ret;
  }

  public unsafe void SetIncreaseSize(uint szNewIncSize) {
    BRMonitoringPINVOKE.ArrayChar_SetIncreaseSize(swigCPtr, szNewIncSize);
  }

  public unsafe SWIGTYPE_p_Result Reserve(uint szReserv) {
    SWIGTYPE_p_Result ret = new SWIGTYPE_p_Result(BRMonitoringPINVOKE.ArrayChar_Reserve(swigCPtr, szReserv), true);
    return ret;
  }

  public unsafe string data() {
    string ret = BRMonitoringPINVOKE.ArrayChar_data(swigCPtr);
    return ret;
  }

  public unsafe SWIGTYPE_p_Result push_back(char NewData) {
    SWIGTYPE_p_Result ret = new SWIGTYPE_p_Result(BRMonitoringPINVOKE.ArrayChar_push_back(swigCPtr, NewData), true);
    return ret;
  }

  public unsafe SWIGTYPE_p_Result AddItems(uint numItems, string NewData) {
    SWIGTYPE_p_Result ret = new SWIGTYPE_p_Result(BRMonitoringPINVOKE.ArrayChar_AddItems(swigCPtr, numItems, NewData), true);
    return ret;
  }

  public unsafe SWIGTYPE_p_Result RemoveAt(int iIndex) {
    SWIGTYPE_p_Result ret = new SWIGTYPE_p_Result(BRMonitoringPINVOKE.ArrayChar_RemoveAt(swigCPtr, iIndex), true);
    return ret;
  }

  public unsafe SWIGTYPE_p_Result RemoveItem(char RemoveData) {
    SWIGTYPE_p_Result ret = new SWIGTYPE_p_Result(BRMonitoringPINVOKE.ArrayChar_RemoveItem(swigCPtr, RemoveData), true);
    return ret;
  }

  public unsafe int FindItem(char FindData) {
    int ret = BRMonitoringPINVOKE.ArrayChar_FindItem(swigCPtr, FindData);
    return ret;
  }

  public unsafe char GetAt(uint iElement) {
    char ret = BRMonitoringPINVOKE.ArrayChar_GetAt(swigCPtr, iElement);
    return ret;
  }

}

}
