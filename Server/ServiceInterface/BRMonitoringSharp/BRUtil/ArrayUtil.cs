using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;


namespace BR {

    static public class ArrayUtil
    {
        static public byte[] ToArray(ArrayBYTE array)
        {
            var csharpArray = new byte[array.GetSize()];
            IntPtr cPtr = SWIGTYPE_p_unsigned_char.getCPtr(array.data()).Handle;
            Marshal.Copy(cPtr, csharpArray, 0, csharpArray.Length);
            return csharpArray;
        }
        static public Int64[] ToArray(ArrayInt64 array)
        {
            var csharpArray = new Int64[array.GetSize()];
            IntPtr cPtr = SWIGTYPE_p_signed___int64.getCPtr(array.data()).Handle;
            Marshal.Copy(cPtr, csharpArray, 0, csharpArray.Length);
            return csharpArray;
        }
        static public Int64[] ToArray(ArrayUInt64 array)
        {
            var csharpArray = new Int64[array.GetSize()];
            IntPtr cPtr = SWIGTYPE_p_unsigned___int64.getCPtr(array.data()).Handle;
            Marshal.Copy(cPtr, csharpArray, 0, csharpArray.Length);
            return csharpArray;
        }
    }
}
