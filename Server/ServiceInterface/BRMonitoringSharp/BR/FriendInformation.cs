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

public class FriendInformation : RankingPlayerInformation {
  private global::System.Runtime.InteropServices.HandleRef swigCPtr;

  internal FriendInformation(global::System.IntPtr cPtr, bool cMemoryOwn) : base(BRMonitoringPINVOKE.FriendInformation_SWIGUpcast(cPtr), cMemoryOwn) {
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
  }

  internal static global::System.Runtime.InteropServices.HandleRef getCPtr(FriendInformation obj) {
    return (obj == null) ? new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero) : obj.swigCPtr;
  }

  public override void Dispose() {
    lock(this) {
      if (swigCPtr.Handle != global::System.IntPtr.Zero) {
        if (swigCMemOwn) {
          swigCMemOwn = false;
          throw new global::System.MethodAccessException("C++ destructor does not have public access");
        }
        swigCPtr = new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero);
      }
      global::System.GC.SuppressFinalize(this);
      base.Dispose();
    }
  }

  public unsafe ulong LastStaminaSent {
    set {
      BRMonitoringPINVOKE.FriendInformation_LastStaminaSent_set(swigCPtr, value);
    } 
    get {
      ulong ret = BRMonitoringPINVOKE.FriendInformation_LastStaminaSent_get(swigCPtr);
      return ret;
    } 
  }

  public unsafe FriendInformation() : this(BRMonitoringPINVOKE.new_FriendInformation__SWIG_0(), true) {
  }

  public unsafe FriendInformation(FriendInformation src) : this(BRMonitoringPINVOKE.new_FriendInformation__SWIG_1(FriendInformation.getCPtr(src)), true) {
    if (BRMonitoringPINVOKE.SWIGPendingException.Pending) throw BRMonitoringPINVOKE.SWIGPendingException.Retrieve();
  }

  public unsafe FriendInformation(ulong playerID, ulong fbUID, string nickName, uint level, uint weeklyWin, uint weeklyLose, byte isPlayingGame, ulong lastActiveTime, ulong LastStaminaSent) : this(BRMonitoringPINVOKE.new_FriendInformation__SWIG_2(playerID, fbUID, nickName, level, weeklyWin, weeklyLose, isPlayingGame, lastActiveTime, LastStaminaSent), true) {
  }

  public unsafe FriendInformation CopyFrom(FriendInformation src) {
    FriendInformation ret = new FriendInformation(BRMonitoringPINVOKE.FriendInformation_CopyFrom(swigCPtr, FriendInformation.getCPtr(src)), false);
    if (BRMonitoringPINVOKE.SWIGPendingException.Pending) throw BRMonitoringPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public unsafe bool EqualTo(FriendInformation src) {
    bool ret = BRMonitoringPINVOKE.FriendInformation_EqualTo(swigCPtr, FriendInformation.getCPtr(src));
    if (BRMonitoringPINVOKE.SWIGPendingException.Pending) throw BRMonitoringPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

}

}
