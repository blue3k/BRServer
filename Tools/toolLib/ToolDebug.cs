using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Threading;
using System.Threading.Tasks;
using System.Text;

namespace BR
{
    static public class ToolDebug
    {
        static public Action<String> LogRouter { get; set; }
        static public ThreadLocal<Action<String>> TLSLogHandler = new ThreadLocal<Action<String>>();
        static public bool DisableOutput_DebugOutput = false;
        static public bool Disable_Log = false;
        static public bool Disable_DebugLog = false;
        static public bool Disable_Warning = false;
        static public bool Disable_Error = false;

        static public void Log(string message, params object[] args)
        {
            if (Disable_Log) return;

            message = string.Format("Log: " + message, message, args);

            if (!DisableOutput_DebugOutput)
                System.Diagnostics.Debug.Print(message);
            if (LogRouter != null) LogRouter(message);
            if (TLSLogHandler.Value != null) TLSLogHandler.Value(message);
        }

        static public void DebugLog(string message, params object[] args)
        {
#if DEBUG
            if (Disable_DebugLog) return;

            message = string.Format("DbgLog: " + message, message, args);

            if (!DisableOutput_DebugOutput)
                System.Diagnostics.Debug.Print(message);
            if (LogRouter != null) LogRouter(message);
            if (TLSLogHandler.Value != null) TLSLogHandler.Value(message);
#endif
        }

        static public void Warning(string message, params object[] args)
        {
            if (Disable_Warning) return;

            message = string.Format("Warning: " + message, message, args);
            if (!DisableOutput_DebugOutput)
                System.Diagnostics.Debug.Print(message);

            if (LogRouter != null) LogRouter(message);
            if (TLSLogHandler.Value != null) TLSLogHandler.Value(message);
        }

        static public void Error(string message, params object[] args)
        {
            if (Disable_Error) return;

            message = string.Format("Error: " + message, message, args);

            if (!DisableOutput_DebugOutput)
            {
#if !DEBUG
                System.Diagnostics.Debug.Fail(message);
#else
                StringBuilder builder = new StringBuilder();
                builder.Append(message);
                var stackTrace = new StackTrace();
                for (int iStackFrame = 1; iStackFrame < stackTrace.FrameCount; iStackFrame++)
                {
                    var stackFrame = stackTrace.GetFrame(iStackFrame);
                    var methodBase = stackFrame.GetMethod();
                    builder.AppendFormat("\n => {0}::{1},0x{2:X},{3}", methodBase.ReflectedType.ToString(), methodBase.Name, stackFrame.GetNativeOffset(), stackFrame.GetFileLineNumber());
                }
                System.Diagnostics.Debug.Print(builder.ToString());
#endif
            }

            if (LogRouter != null) LogRouter(message);
            if (TLSLogHandler.Value != null) TLSLogHandler.Value(message);
        }

        static public void Assert(bool bCondition)
        {
#if DEBUG
            System.Diagnostics.Debug.Assert(bCondition);
#else
            if (!bCondition) Error("Asster! ");
#endif
            if (!bCondition)
            {
                if (LogRouter != null) LogRouter("Assert:");
                if (TLSLogHandler.Value != null) TLSLogHandler.Value("Assert:");
            }
        }

        static public void Assert(bool bCondition, string message, params object[] args)
        {
#if DEBUG
            System.Diagnostics.Debug.Assert(bCondition);
#else
            if (!bCondition) Error("Asster! " + message, args);
#endif

            if (!bCondition)
            {
                message = string.Format("Assert: {0}", message);
                if (LogRouter != null) LogRouter(message);
                if (TLSLogHandler.Value != null) TLSLogHandler.Value(message);
            }
        }
    }
}
