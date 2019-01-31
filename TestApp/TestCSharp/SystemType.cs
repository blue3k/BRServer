////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : system types
//
////////////////////////////////////////////////////////////////////////////////

using System;
using System.Collections.Generic;


namespace BR
{
    using String = String;
    using INT8 = SByte;
    using UINT8 = Byte;
    using INT16 = Int16;
    using UINT16 = UInt16;
    using INT32 = Int32;
    using UINT32 = UInt32;
    using INT64 = Int64;
    using UINT64 = UInt64;
    using BYTE = Byte;
    using WORD = UInt16;
    using DWORD = UInt32;
    using QWORD = UInt64;
    using FLOAT = Single;
    using Context = UInt64;
    using AccountID = UInt64;
    using AuthTicket = UInt64;
    using GameInstanceID = UInt32;
    using GameInstanceUID = UInt64;
    using HRESULT = Int32;
    using EntityID = UInt32;
    using EntityUID = UInt64;
    using ServerID = UInt32;
    using ServerUID = UInt64;

    // structure data type attribute, this is used for message builder
    public class StructAttribute : Attribute
    {
    }

    [Struct()]
    public struct BRVector2
    {
        public float x;
        public float y;
    };

    [Struct()]
    public struct BRVector3
    {
        public float x;
        public float y;
        public float z;
    };

    [Struct()]
    public struct NetAddress
    {
        public string Address;
        public UInt16 Port;
    };

    [Struct()]
    public struct RouteContext
    {
        public ServerID From;
        public ServerID To;
    }

}
