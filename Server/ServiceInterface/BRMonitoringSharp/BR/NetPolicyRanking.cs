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

public class NetPolicyRanking : global::System.IDisposable {
  private global::System.Runtime.InteropServices.HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal NetPolicyRanking(global::System.IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
  }

  internal static global::System.Runtime.InteropServices.HandleRef getCPtr(NetPolicyRanking obj) {
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

  public unsafe NetPolicyRanking(IConnection pConn) : this(BRMonitoringPINVOKE.new_NetPolicyRanking(IConnection.getCPtr(pConn)), true) {
  }

  public unsafe SWIGTYPE_p_Result AddPlayerCmd(RouteContext InRouteContext, TransactionID InTransactionID, ulong InPlayerID, ulong InRankingScore, ArrayBYTE InPlayerInfo) {
    SWIGTYPE_p_Result ret = new SWIGTYPE_p_Result(BRMonitoringPINVOKE.NetPolicyRanking_AddPlayerCmd(swigCPtr, RouteContext.getCPtr(InRouteContext), TransactionID.getCPtr(InTransactionID), InPlayerID, InRankingScore, ArrayBYTE.getCPtr(InPlayerInfo)), true);
    if (BRMonitoringPINVOKE.SWIGPendingException.Pending) throw BRMonitoringPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public unsafe SWIGTYPE_p_Result RemovePlayerCmd(RouteContext InRouteContext, TransactionID InTransactionID, ulong InPlayerID) {
    SWIGTYPE_p_Result ret = new SWIGTYPE_p_Result(BRMonitoringPINVOKE.NetPolicyRanking_RemovePlayerCmd(swigCPtr, RouteContext.getCPtr(InRouteContext), TransactionID.getCPtr(InTransactionID), InPlayerID), true);
    if (BRMonitoringPINVOKE.SWIGPendingException.Pending) throw BRMonitoringPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public unsafe SWIGTYPE_p_Result GetPlayerRankingCmd(RouteContext InRouteContext, TransactionID InTransactionID, ulong InPlayerID) {
    SWIGTYPE_p_Result ret = new SWIGTYPE_p_Result(BRMonitoringPINVOKE.NetPolicyRanking_GetPlayerRankingCmd(swigCPtr, RouteContext.getCPtr(InRouteContext), TransactionID.getCPtr(InTransactionID), InPlayerID), true);
    if (BRMonitoringPINVOKE.SWIGPendingException.Pending) throw BRMonitoringPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public unsafe SWIGTYPE_p_Result UpdatePlayerCmd(RouteContext InRouteContext, TransactionID InTransactionID, ulong InPlayerID, ulong InRankingScore, ArrayBYTE InPlayerInfo) {
    SWIGTYPE_p_Result ret = new SWIGTYPE_p_Result(BRMonitoringPINVOKE.NetPolicyRanking_UpdatePlayerCmd(swigCPtr, RouteContext.getCPtr(InRouteContext), TransactionID.getCPtr(InTransactionID), InPlayerID, InRankingScore, ArrayBYTE.getCPtr(InPlayerInfo)), true);
    if (BRMonitoringPINVOKE.SWIGPendingException.Pending) throw BRMonitoringPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public unsafe SWIGTYPE_p_Result GetRankingCmd(RouteContext InRouteContext, TransactionID InTransactionID, uint InFrom, uint InCount) {
    SWIGTYPE_p_Result ret = new SWIGTYPE_p_Result(BRMonitoringPINVOKE.NetPolicyRanking_GetRankingCmd(swigCPtr, RouteContext.getCPtr(InRouteContext), TransactionID.getCPtr(InTransactionID), InFrom, InCount), true);
    if (BRMonitoringPINVOKE.SWIGPendingException.Pending) throw BRMonitoringPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

}

}
