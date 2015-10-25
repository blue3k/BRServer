using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace LoadTest
{
    class Program
    {
        public delegate void ExceptionDelegate(string message);

        [global::System.Runtime.InteropServices.DllImport("BRMonitoring", EntryPoint = "SWIGRegisterExceptionCallbacks_BRMonitoring")]
        public static extern void SWIGRegisterExceptionCallbacks_BRMonitoring(
                            ExceptionDelegate applicationDelegate,
                            ExceptionDelegate arithmeticDelegate,
                            ExceptionDelegate divideByZeroDelegate,
                            ExceptionDelegate indexOutOfRangeDelegate,
                            ExceptionDelegate invalidCastDelegate,
                            ExceptionDelegate invalidOperationDelegate,
                            ExceptionDelegate ioDelegate,
                            ExceptionDelegate nullReferenceDelegate,
                            ExceptionDelegate outOfMemoryDelegate,
                            ExceptionDelegate overflowDelegate,
                            ExceptionDelegate systemExceptionDelegate);

        [DllImport("kernel32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        private static extern IntPtr LoadLibrary(string libname);

        [DllImport("kernel32.dll", CharSet = CharSet.Auto)]
        private static extern bool FreeLibrary(IntPtr hModule);

        static void DLLLoadTest()
        {
            IntPtr Handle = LoadLibrary("BRMonitoring.dll");
            if (Handle == IntPtr.Zero)
            {
                int errorCode = Marshal.GetLastWin32Error();
                return;
            }

            if (Handle != IntPtr.Zero)
                FreeLibrary(Handle);
        }

        static void Main(string[] args)
        {
            DLLLoadTest();

            SWIGRegisterExceptionCallbacks_BRMonitoring(
                                null,
                                null,
                                null,
                                null,
                                null,
                                null,
                                null,
                                null,
                                null,
                                null,
                                null);

            BR.BRMonitoring.InitializeNativeSystem("BRManigement");
            BR.PerformanceCounterServer.Initialize("", (uint)1999);
        }
    }
}
