////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : network packet class
//
////////////////////////////////////////////////////////////////////////////////

using System;
using System.Net;
using System.Net.Sockets;
using System.Collections;
using System.IO;
using System.Threading;


using BR.Message;
using BR.Policy;

namespace BR.Net
{
    /// <summary> 
    /// Mobile UDP network connection class
    /// </summary>
    public class ConnectionConspiracy : ConnectionMUDP
    {
        /// <summary> 
        /// Constructor
        /// </summary>
        public ConnectionConspiracy()
        {
            MessageProcessor.RegisterMessage(BR.Message.Login.LoginRes.MID, typeof(BR.Message.Login.LoginRes));
            MessageProcessor.RegisterMessage(BR.Message.Login.LoginByFacebookRes.MID, typeof(BR.Message.Login.LoginByFacebookRes));
            MessageProcessor.RegisterMessageHandler(BR.Message.Login.LoginRes.MID, OnLoginRes);
            MessageProcessor.RegisterMessageHandler(BR.Message.Login.LoginByFacebookRes.MID, OnLoginByFacebookRes);
        }

        public override BinaryMemoryReader CreateReader(byte[] buffer)
        {
            return new PacketMemoryReader(buffer);
        }

        public override BinaryMemoryWriter CreateWriter(byte[] buffer)
        {
            return new PacketMemoryWriter(buffer);
        }
        
        
        int OnLoginRes(MessageBase msg)
        {
            BR.Message.Login.LoginRes res = msg as BR.Message.Login.LoginRes;

            PeerID = res.Ticket;

            return 0;
        }

        int OnLoginByFacebookRes(MessageBase msg)
        {
            BR.Message.Login.LoginByFacebookRes res = msg as BR.Message.Login.LoginByFacebookRes;

            PeerID = res.Ticket;

            return 0;
        }
        
    }
}
