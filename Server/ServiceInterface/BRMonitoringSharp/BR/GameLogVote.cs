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

  public unsafe GameLogVote(uint timeStamp, uint numVoter) : this(BRMonitoringPINVOKE.new_GameLogVote(timeStamp, numVoter), true) {
  }

  public unsafe int InitializeVote(GameVoteType type, int voterCount) {
    int ret = BRMonitoringPINVOKE.GameLogVote_InitializeVote(swigCPtr, (int)type, voterCount);
    return ret;
  }

  public unsafe int SetVoteStatus(ulong voter, ulong voted) {
    int ret = BRMonitoringPINVOKE.GameLogVote_SetVoteStatus(swigCPtr, voter, voted);
    return ret;
  }

}

}
