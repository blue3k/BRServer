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

public class NetSvrPolicyRanking : global::System.IDisposable {
  private global::System.Runtime.InteropServices.HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal NetSvrPolicyRanking(global::System.IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
  }

  internal static global::System.Runtime.InteropServices.HandleRef getCPtr(NetSvrPolicyRanking obj) {
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

  public unsafe NetSvrPolicyRanking(IConnection pConn) : this(BRMonitoringPINVOKE.new_NetSvrPolicyRanking(IConnection.getCPtr(pConn)), true) {
  }

  public unsafe SWIGTYPE_p_Result AddPlayerRes(RouteContext InRouteContext, TransactionID InTransactionID, SWIGTYPE_p_Result InResult, uint InRanking) {
    SWIGTYPE_p_Result ret = new SWIGTYPE_p_Result(BRMonitoringPINVOKE.NetSvrPolicyRanking_AddPlayerRes(swigCPtr, RouteContext.getCPtr(InRouteContext), TransactionID.getCPtr(InTransactionID), SWIGTYPE_p_Result.getCPtr(InResult), InRanking), true);
    if (BRMonitoringPINVOKE.SWIGPendingException.Pending) throw BRMonitoringPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public unsafe SWIGTYPE_p_Result RemovePlayerRes(RouteContext InRouteContext, TransactionID InTransactionID, SWIGTYPE_p_Result InResult) {
    SWIGTYPE_p_Result ret = new SWIGTYPE_p_Result(BRMonitoringPINVOKE.NetSvrPolicyRanking_RemovePlayerRes(swigCPtr, RouteContext.getCPtr(InRouteContext), TransactionID.getCPtr(InTransactionID), SWIGTYPE_p_Result.getCPtr(InResult)), true);
    if (BRMonitoringPINVOKE.SWIGPendingException.Pending) throw BRMonitoringPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public unsafe SWIGTYPE_p_Result GetPlayerRankingRes(RouteContext InRouteContext, TransactionID InTransactionID, SWIGTYPE_p_Result InResult, uint InRanking) {
    SWIGTYPE_p_Result ret = new SWIGTYPE_p_Result(BRMonitoringPINVOKE.NetSvrPolicyRanking_GetPlayerRankingRes(swigCPtr, RouteContext.getCPtr(InRouteContext), TransactionID.getCPtr(InTransactionID), SWIGTYPE_p_Result.getCPtr(InResult), InRanking), true);
    if (BRMonitoringPINVOKE.SWIGPendingException.Pending) throw BRMonitoringPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public unsafe SWIGTYPE_p_Result UpdatePlayerRes(RouteContext InRouteContext, TransactionID InTransactionID, SWIGTYPE_p_Result InResult, uint InRanking) {
    SWIGTYPE_p_Result ret = new SWIGTYPE_p_Result(BRMonitoringPINVOKE.NetSvrPolicyRanking_UpdatePlayerRes(swigCPtr, RouteContext.getCPtr(InRouteContext), TransactionID.getCPtr(InTransactionID), SWIGTYPE_p_Result.getCPtr(InResult), InRanking), true);
    if (BRMonitoringPINVOKE.SWIGPendingException.Pending) throw BRMonitoringPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public unsafe SWIGTYPE_p_Result GetRankingRes(RouteContext InRouteContext, TransactionID InTransactionID, SWIGTYPE_p_Result InResult, uint InPlayerRanking) {
    SWIGTYPE_p_Result ret = new SWIGTYPE_p_Result(BRMonitoringPINVOKE.NetSvrPolicyRanking_GetRankingRes(swigCPtr, RouteContext.getCPtr(InRouteContext), TransactionID.getCPtr(InTransactionID), SWIGTYPE_p_Result.getCPtr(InResult), InPlayerRanking), true);
    if (BRMonitoringPINVOKE.SWIGPendingException.Pending) throw BRMonitoringPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

}

}
