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

public class PerformanceCounterServer : global::System.IDisposable {
  private global::System.Runtime.InteropServices.HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal PerformanceCounterServer(global::System.IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
  }

  internal static global::System.Runtime.InteropServices.HandleRef getCPtr(PerformanceCounterServer obj) {
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

  public class MessageHandler : global::System.IDisposable {
    private global::System.Runtime.InteropServices.HandleRef swigCPtr;
    protected bool swigCMemOwn;
  
    internal MessageHandler(global::System.IntPtr cPtr, bool cMemoryOwn) {
      swigCMemOwn = cMemoryOwn;
      swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
    }
  
    internal static global::System.Runtime.InteropServices.HandleRef getCPtr(MessageHandler obj) {
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
  
    public unsafe MessageHandler(PerformanceCounterServer CounterServer) : this(BRMonitoringPINVOKE.new_PerformanceCounterServer_MessageHandler(PerformanceCounterServer.getCPtr(CounterServer)), true) {
    }
  
    public unsafe int OnRecv(SWIGTYPE_p_sockaddr_in6 remoteAddr, MessageData pMsg) {
      int ret = BRMonitoringPINVOKE.PerformanceCounterServer_MessageHandler_OnRecv(swigCPtr, SWIGTYPE_p_sockaddr_in6.getCPtr(remoteAddr), MessageData.getCPtr(pMsg));
      if (BRMonitoringPINVOKE.SWIGPendingException.Pending) throw BRMonitoringPINVOKE.SWIGPendingException.Retrieve();
      return ret;
    }
  
  }

  public unsafe static int Initialize(string serverAddress, uint port) {
    int ret = BRMonitoringPINVOKE.PerformanceCounterServer_Initialize__SWIG_0(serverAddress, port);
    return ret;
  }

  public unsafe static int Initialize(NetAddress serverAddress) {
    int ret = BRMonitoringPINVOKE.PerformanceCounterServer_Initialize__SWIG_1(NetAddress.getCPtr(serverAddress));
    if (BRMonitoringPINVOKE.SWIGPendingException.Pending) throw BRMonitoringPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public unsafe static int Terminate() {
    int ret = BRMonitoringPINVOKE.PerformanceCounterServer_Terminate();
    return ret;
  }

  public unsafe static uint GetInstanceCount() {
    uint ret = BRMonitoringPINVOKE.PerformanceCounterServer_GetInstanceCount();
    return ret;
  }

  public unsafe static uint GetInstanceList(uint startIndex, ArrayPerformanceCounterInstancePtr instanceList) {
    uint ret = BRMonitoringPINVOKE.PerformanceCounterServer_GetInstanceList(startIndex, ArrayPerformanceCounterInstancePtr.getCPtr(instanceList));
    if (BRMonitoringPINVOKE.SWIGPendingException.Pending) throw BRMonitoringPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public unsafe static int GetInstance(ulong instanceUID, PerformanceCounterInstancePtr pInstance) {
    int ret = BRMonitoringPINVOKE.PerformanceCounterServer_GetInstance__SWIG_0(instanceUID, PerformanceCounterInstancePtr.getCPtr(pInstance));
    if (BRMonitoringPINVOKE.SWIGPendingException.Pending) throw BRMonitoringPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public unsafe static int GetInstance(EntityUID instanceUID, PerformanceCounterInstancePtr pInstance) {
    int ret = BRMonitoringPINVOKE.PerformanceCounterServer_GetInstance__SWIG_1(EntityUID.getCPtr(instanceUID), PerformanceCounterInstancePtr.getCPtr(pInstance));
    if (BRMonitoringPINVOKE.SWIGPendingException.Pending) throw BRMonitoringPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public unsafe static readonly int TIMER_TIMOUT = BRMonitoringPINVOKE.PerformanceCounterServer_TIMER_TIMOUT_get();

}

}
