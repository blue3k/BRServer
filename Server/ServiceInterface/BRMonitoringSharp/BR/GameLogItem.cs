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

public class GameLogItem : global::System.IDisposable {
  private global::System.Runtime.InteropServices.HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal GameLogItem(global::System.IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
  }

  internal static global::System.Runtime.InteropServices.HandleRef getCPtr(GameLogItem obj) {
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

  public unsafe GameLogType LogType {
    set {
      BRMonitoringPINVOKE.GameLogItem_LogType_set(swigCPtr, (int)value);
    } 
    get {
      GameLogType ret = (GameLogType)BRMonitoringPINVOKE.GameLogItem_LogType_get(swigCPtr);
      return ret;
    } 
  }

  public unsafe ushort LogItemSize {
    set {
      BRMonitoringPINVOKE.GameLogItem_LogItemSize_set(swigCPtr, value);
    } 
    get {
      ushort ret = BRMonitoringPINVOKE.GameLogItem_LogItemSize_get(swigCPtr);
      return ret;
    } 
  }

  public unsafe SWIGTYPE_p_std__chrono__time_pointT_ClockType_DurationSec_t TimeStamp {
    set {
      BRMonitoringPINVOKE.GameLogItem_TimeStamp_set(swigCPtr, SWIGTYPE_p_std__chrono__time_pointT_ClockType_DurationSec_t.getCPtr(value));
      if (BRMonitoringPINVOKE.SWIGPendingException.Pending) throw BRMonitoringPINVOKE.SWIGPendingException.Retrieve();
    } 
    get {
      SWIGTYPE_p_std__chrono__time_pointT_ClockType_DurationSec_t ret = new SWIGTYPE_p_std__chrono__time_pointT_ClockType_DurationSec_t(BRMonitoringPINVOKE.GameLogItem_TimeStamp_get(swigCPtr), true);
      if (BRMonitoringPINVOKE.SWIGPendingException.Pending) throw BRMonitoringPINVOKE.SWIGPendingException.Retrieve();
      return ret;
    } 
  }

  public unsafe GameLogItem(GameLogType logType, SWIGTYPE_p_std__chrono__time_pointT_ClockType_DurationSec_t timeStamp, uint logItemSize) : this(BRMonitoringPINVOKE.new_GameLogItem((int)logType, SWIGTYPE_p_std__chrono__time_pointT_ClockType_DurationSec_t.getCPtr(timeStamp), logItemSize), true) {
    if (BRMonitoringPINVOKE.SWIGPendingException.Pending) throw BRMonitoringPINVOKE.SWIGPendingException.Retrieve();
  }

}

}
