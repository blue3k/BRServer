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
		public void  LoginCmd( BR.GameID InGameID, string InID, string InPassword )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Login.LoginCmd.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Login.LoginCmd.BuildMessageBuffer(binWriter, ref header, InGameID, InID, InPassword );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  LoginCmd( BR.GameID InGameID, string InID, string InPassword )

		// Cmd: Login request with Facebook UID
		public void  LoginByFacebookCmd( BR.GameID InGameID, System.UInt64 InUID, string InFaceBookName, string InEMail, string InFacebookToken )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Login.LoginByFacebookCmd.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Login.LoginByFacebookCmd.BuildMessageBuffer(binWriter, ref header, InGameID, InUID, InFaceBookName, InEMail, InFacebookToken );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  LoginByFacebookCmd( BR.GameID InGameID, System.UInt64 InUID, string InFaceBookName, string InEMail, string InFacebookToken )

	}; // public class PolicyLogin : BR.Net.Policy
	public class SvrPolicyLogin : BR.Net.Policy
	{
 
		public SvrPolicyLogin( BR.Net.ConnectionMUDP connection )
			:base(connection)
		{
		}

		// Cmd: Login request
		public void  LoginRes( System.Int32 InResult, ref BR.NetAddress InGameServerAddr, System.UInt64 InAccID, System.UInt64 InTicket, System.UInt64 InLoginEntityUID, string InNickName )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Login.LoginRes.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Login.LoginRes.BuildMessageBuffer(binWriter, ref header, InResult,ref InGameServerAddr, InAccID, InTicket, InLoginEntityUID, InNickName );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  LoginRes( System.Int32 InResult, ref BR.NetAddress InGameServerAddr, System.UInt64 InAccID, System.UInt64 InTicket, System.UInt64 InLoginEntityUID, string InNickName )


		// Cmd: Login request with Facebook UID
		public void  LoginByFacebookRes( System.Int32 InResult, ref BR.NetAddress InGameServerAddr, System.UInt64 InAccID, System.UInt64 InTicket, System.UInt64 InLoginEntityUID, string InNickName )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Login.LoginByFacebookRes.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Login.LoginByFacebookRes.BuildMessageBuffer(binWriter, ref header, InResult,ref InGameServerAddr, InAccID, InTicket, InLoginEntityUID, InNickName );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  LoginByFacebookRes( System.Int32 InResult, ref BR.NetAddress InGameServerAddr, System.UInt64 InAccID, System.UInt64 InTicket, System.UInt64 InLoginEntityUID, string InNickName )


	}; // public class SvrPolicyLogin : BR.Net.Policy




} // namespace BR.Policy


