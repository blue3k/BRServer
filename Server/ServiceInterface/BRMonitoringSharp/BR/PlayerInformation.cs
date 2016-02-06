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

public class PlayerInformation : global::System.IDisposable {
  private global::System.Runtime.InteropServices.HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal PlayerInformation(global::System.IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
  }

  internal static global::System.Runtime.InteropServices.HandleRef getCPtr(PlayerInformation obj) {
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

  public unsafe ulong PlayerID {
    set {
      BRMonitoringPINVOKE.PlayerInformation_PlayerID_set(swigCPtr, value);
    } 
    get {
      ulong ret = BRMonitoringPINVOKE.PlayerInformation_PlayerID_get(swigCPtr);
      return ret;
    } 
  }

  public unsafe ulong FBUID {
    set {
      BRMonitoringPINVOKE.PlayerInformation_FBUID_set(swigCPtr, value);
    } 
    get {
      ulong ret = BRMonitoringPINVOKE.PlayerInformation_FBUID_get(swigCPtr);
      return ret;
    } 
  }

  public unsafe string NickName {
    set {
      BRMonitoringPINVOKE.PlayerInformation_NickName_set(swigCPtr, value);
    } 
    get {
      string ret = BRMonitoringPINVOKE.PlayerInformation_NickName_get(swigCPtr);
      return ret;
    } 
  }

  public unsafe ulong LastActiveTime {
    set {
      BRMonitoringPINVOKE.PlayerInformation_LastActiveTime_set(swigCPtr, value);
    } 
    get {
      ulong ret = BRMonitoringPINVOKE.PlayerInformation_LastActiveTime_get(swigCPtr);
      return ret;
    } 
  }

  public unsafe uint Level {
    set {
      BRMonitoringPINVOKE.PlayerInformation_Level_set(swigCPtr, value);
    } 
    get {
      uint ret = BRMonitoringPINVOKE.PlayerInformation_Level_get(swigCPtr);
      return ret;
    } 
  }

  public unsafe uint IsPlayingGame {
    set {
      BRMonitoringPINVOKE.PlayerInformation_IsPlayingGame_set(swigCPtr, value);
    } 
    get {
      uint ret = BRMonitoringPINVOKE.PlayerInformation_IsPlayingGame_get(swigCPtr);
      return ret;
    } 
  }

  public unsafe PlayerInformation() : this(BRMonitoringPINVOKE.new_PlayerInformation__SWIG_0(), true) {
  }

  public unsafe PlayerInformation(PlayerInformation src) : this(BRMonitoringPINVOKE.new_PlayerInformation__SWIG_1(PlayerInformation.getCPtr(src)), true) {
    if (BRMonitoringPINVOKE.SWIGPendingException.Pending) throw BRMonitoringPINVOKE.SWIGPendingException.Retrieve();
  }

  public unsafe PlayerInformation(ulong playerID, ulong fbUID, string nickName, uint level, byte isPlayingGame, ulong lastActiveTime) : this(BRMonitoringPINVOKE.new_PlayerInformation__SWIG_2(playerID, fbUID, nickName, level, isPlayingGame, lastActiveTime), true) {
  }

  public unsafe int InitPlayerInformation(ulong playerID, ulong fbUID, string nickName, uint level, byte isPlayingGame, ulong lastActiveTime) {
    int ret = BRMonitoringPINVOKE.PlayerInformation_InitPlayerInformation(swigCPtr, playerID, fbUID, nickName, level, isPlayingGame, lastActiveTime);
    return ret;
  }

  public unsafe PlayerInformation CopyFrom(PlayerInformation src) {
    PlayerInformation ret = new PlayerInformation(BRMonitoringPINVOKE.PlayerInformation_CopyFrom(swigCPtr, PlayerInformation.getCPtr(src)), false);
    if (BRMonitoringPINVOKE.SWIGPendingException.Pending) throw BRMonitoringPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public unsafe bool EqualTo(PlayerInformation src) {
    bool ret = BRMonitoringPINVOKE.PlayerInformation_EqualTo(swigCPtr, PlayerInformation.getCPtr(src));
    if (BRMonitoringPINVOKE.SWIGPendingException.Pending) throw BRMonitoringPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public unsafe static readonly int MAX_NAME = BRMonitoringPINVOKE.PlayerInformation_MAX_NAME_get();

}

}
