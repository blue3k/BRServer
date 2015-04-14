////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : Login Message implementations
// 
////////////////////////////////////////////////////////////////////////////////


using System;
using System.Diagnostics;
using BR;
using BR.Message;
using BR.Net;
using BR.Message.Login;



namespace BR.Policy
{
 

	public class PolicyLogin : BR.Net.Policy
	{
 
		public PolicyLogin( BR.Net.ConnectionMUDP connection )
			:base(connection)
		{
		}

		// Cmd: Login request
		public bool  LoginCmd( BR.GameID InGameID, string InID, string InPassword )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Login.LoginCmd.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Login.LoginCmd.BuildMessageBuffer(binWriter, ref header, InGameID, InID, InPassword );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  LoginCmd( BR.GameID InGameID, string InID, string InPassword )

		// Cmd: Login request with Facebook UID
		public bool  LoginByFacebookCmd( BR.GameID InGameID, System.UInt64 InUID, string InFaceBookName, string InEMail, string InFacebookToken )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Login.LoginByFacebookCmd.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Login.LoginByFacebookCmd.BuildMessageBuffer(binWriter, ref header, InGameID, InUID, InFaceBookName, InEMail, InFacebookToken );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  LoginByFacebookCmd( BR.GameID InGameID, System.UInt64 InUID, string InFaceBookName, string InEMail, string InFacebookToken )

	}; // public class PolicyLogin : BR.Net.Policy
	public class SvrPolicyLogin : BR.Net.Policy
	{
 
		public SvrPolicyLogin( BR.Net.ConnectionMUDP connection )
			:base(connection)
		{
		}

		// Cmd: Login request
		public bool  LoginRes( System.Int32 InResult, ref BR.NetAddress InGameServerAddr, System.UInt64 InAccID, System.UInt64 InTicket, System.UInt64 InLoginEntityUID )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Login.LoginRes.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Login.LoginRes.BuildMessageBuffer(binWriter, ref header, InResult,ref InGameServerAddr, InAccID, InTicket, InLoginEntityUID );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  LoginRes( System.Int32 InResult, ref BR.NetAddress InGameServerAddr, System.UInt64 InAccID, System.UInt64 InTicket, System.UInt64 InLoginEntityUID )


		// Cmd: Login request with Facebook UID
		public bool  LoginByFacebookRes( System.Int32 InResult, ref BR.NetAddress InGameServerAddr, System.UInt64 InAccID, System.UInt64 InTicket, System.UInt64 InLoginEntityUID )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Login.LoginByFacebookRes.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Login.LoginByFacebookRes.BuildMessageBuffer(binWriter, ref header, InResult,ref InGameServerAddr, InAccID, InTicket, InLoginEntityUID );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  LoginByFacebookRes( System.Int32 InResult, ref BR.NetAddress InGameServerAddr, System.UInt64 InAccID, System.UInt64 InTicket, System.UInt64 InLoginEntityUID )


	}; // public class SvrPolicyLogin : BR.Net.Policy




} // namespace BR.Policy


