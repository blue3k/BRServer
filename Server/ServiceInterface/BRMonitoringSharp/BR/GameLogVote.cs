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

public class GameLogVote : GameLogItem {
  private global::System.Runtime.InteropServices.HandleRef swigCPtr;

  internal GameLogVote(global::System.IntPtr cPtr, bool cMemoryOwn) : base(BRMonitoringPINVOKE.GameLogVote_SWIGUpcast(cPtr), cMemoryOwn) {
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
  }

  internal static global::System.Runtime.InteropServices.HandleRef getCPtr(GameLogVote obj) {
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

  public unsafe GameVoteType Type {
    set {
      BRMonitoringPINVOKE.GameLogVote_Type_set(swigCPtr, (int)value);
    } 
    get {
      GameVoteType ret = (GameVoteType)BRMonitoringPINVOKE.GameLogVote_Type_get(swigCPtr);
      return ret;
    } 
  }

  public unsafe ushort NumberOfVoter {
    set {
      BRMonitoringPINVOKE.GameLogVote_NumberOfVoter_set(swigCPtr, value);
    } 
    get {
      ushort ret = BRMonitoringPINVOKE.GameLogVote_NumberOfVoter_get(swigCPtr);
      return ret;
    } 
  }

  public class VoteInfo : global::System.IDisposable {
    private global::System.Runtime.InteropServices.HandleRef swigCPtr;
    protected bool swigCMemOwn;
  
    internal VoteInfo(global::System.IntPtr cPtr, bool cMemoryOwn) {
      swigCMemOwn = cMemoryOwn;
      swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
    }
  
    internal static global::System.Runtime.InteropServices.HandleRef getCPtr(VoteInfo obj) {
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
  
    public unsafe ulong Voter {
      set {
        BRMonitoringPINVOKE.GameLogVote_VoteInfo_Voter_set(swigCPtr, value);
      } 
      get {
        ulong ret = BRMonitoringPINVOKE.GameLogVote_VoteInfo_Voter_get(swigCPtr);
        return ret;
      } 
    }
  
    public unsafe ulong Voted {
      set {
        BRMonitoringPINVOKE.GameLogVote_VoteInfo_Voted_set(swigCPtr, value);
      } 
      get {
        ulong ret = BRMonitoringPINVOKE.GameLogVote_VoteInfo_Voted_get(swigCPtr);
        return ret;
      } 
    }
  
  }

  public unsafe GameLogVote.VoteInfo VoteStatus {
    set {
      BRMonitoringPINVOKE.GameLogVote_VoteStatus_set(swigCPtr, GameLogVote.VoteInfo.getCPtr(value));
    } 
    get {
      global::System.IntPtr cPtr = BRMonitoringPINVOKE.GameLogVote_VoteStatus_get(swigCPtr);
      GameLogVote.VoteInfo ret = (cPtr == global::System.IntPtr.Zero) ? null : new GameLogVote.VoteInfo(cPtr, false);
      return ret;
    } 
  }

  public unsafe GameLogVote(SWIGTYPE_p_std__chrono__time_pointT_ClockType_DurationSec_t timeStamp, uint numVoter) : this(BRMonitoringPINVOKE.new_GameLogVote(SWIGTYPE_p_std__chrono__time_pointT_ClockType_DurationSec_t.getCPtr(timeStamp), numVoter), true) {
    if (BRMonitoringPINVOKE.SWIGPendingException.Pending) throw BRMonitoringPINVOKE.SWIGPendingException.Retrieve();
  }

  public unsafe SWIGTYPE_p_Result InitializeVote(GameVoteType type, int voterCount) {
    SWIGTYPE_p_Result ret = new SWIGTYPE_p_Result(BRMonitoringPINVOKE.GameLogVote_InitializeVote(swigCPtr, (int)type, voterCount), true);
    return ret;
  }

  public unsafe SWIGTYPE_p_Result SetVoteStatus(ulong voter, ulong voted) {
    SWIGTYPE_p_Result ret = new SWIGTYPE_p_Result(BRMonitoringPINVOKE.GameLogVote_SetVoteStatus(swigCPtr, voter, voted), true);
    return ret;
  }

}

}
