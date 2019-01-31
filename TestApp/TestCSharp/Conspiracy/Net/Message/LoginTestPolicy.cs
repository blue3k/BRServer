////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : Game Message implementations
// 
////////////////////////////////////////////////////////////////////////////////


using System;
using System.Collections.Generic;
using System.Diagnostics;
using BR;
using BR.Message;
using BR.Net;
using BR.Message.Login;

using System.Threading;

namespace BR.Policy
{
	public class LoginTestPolicy : BR.Net.Policy
	{
        MessageHeader m_Header;
        PacketMemoryWriter m_BinWriter;

        public LoginTestPolicy(BR.Net.ConnectionTest connection)
			:base(connection)
		{
            m_Header = new MessageHeader();
			m_BinWriter = new PacketMemoryWriter(m_Connection.AllocMessageBuffer());
		}

		private PacketMemoryReader GetReader()
		{
			var reader = new PacketMemoryReader (m_BinWriter.Buffer);
//			var headerReader = new MessageHeader();
//			headerReader.Read (reader);

			return reader;
		}

		private void InitializeMsgData(MessageID id)
		{
			m_Header.MsgID = id;
			m_Header.PeerID = m_Connection.PeerID;
			m_Header.Sequence = m_Connection.NewSequence;
			m_BinWriter.Buffer = m_Connection.AllocMessageBuffer();
			m_BinWriter.Length = 0;
			m_BinWriter.Seek(0);
		}

		// Cmd: Login
        public void LoginByFacebookCmd(GameID gameID, System.UInt64 InUID, string InFaceBookName, string InEMail, string InFacebookToken)
		{
			BRDebug.Log ("PolicyTest >> LoginByFacebookCmd");
            var gameServerAddress = BR.GameSystem.BRGame.Instance.Net.GameServerAddress;

            InitializeMsgData(BR.Message.Login.LoginByFacebookRes.MID);

            BR.Message.Login.LoginByFacebookRes.BuildMessageBuffer(m_BinWriter, ref m_Header, 0, ref gameServerAddress, 1, 0, 2, "Test");

			m_Connection.MessageProcessor.AddTestMessage (new LoginByFacebookRes(), ref m_Header, GetReader ());
		}

		// Cmd: Login
		public void  LoginCmd( GameID gameID, string InID, string InPassword )
		{
			BRDebug.Log ("PolicyTest >> LoginCmd");
            var gameServerAddress = BR.GameSystem.BRGame.Instance.Net.GameServerAddress;

            InitializeMsgData(BR.Message.Login.LoginRes.MID);

            BR.Message.Login.LoginRes.BuildMessageBuffer(m_BinWriter, ref m_Header, 0,ref gameServerAddress, 1, 2, 1, "Test");

			m_Connection.MessageProcessor.AddTestMessage (new LoginRes(), ref m_Header, GetReader ());
		}

	}
}
