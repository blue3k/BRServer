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

public class GameLogGameStateChange : GameLogItem {
  private global::System.Runtime.InteropServices.HandleRef swigCPtr;

  internal GameLogGameStateChange(global::System.IntPtr cPtr, bool cMemoryOwn) : base(BRMonitoringPINVOKE.GameLogGameStateChange_SWIGUpcast(cPtr), cMemoryOwn) {
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
  }

  internal static global::System.Runtime.InteropServices.HandleRef getCPtr(GameLogGameStateChange obj) {
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

  public unsafe GameStateID GameState {
    set {
      BRMonitoringPINVOKE.GameLogGameStateChange_GameState_set(swigCPtr, (int)value);
    } 
    get {
      GameStateID ret = (GameStateID)BRMonitoringPINVOKE.GameLogGameStateChange_GameState_get(swigCPtr);
      return ret;
    } 
  }

  public unsafe GameLogGameStateChange(uint timeStamp) : this(BRMonitoringPINVOKE.new_GameLogGameStateChange(timeStamp), true) {
  }

  public unsafe void SetState(GameStateID gameState) {
    BRMonitoringPINVOKE.GameLogGameStateChange_SetState(swigCPtr, (int)gameState);
  }

}

}
