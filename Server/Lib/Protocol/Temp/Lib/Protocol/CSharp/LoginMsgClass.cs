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
#if ANDROID
using Preserve = Android.Runtime.PreserveAttribute;
#elif IOS
using Preserve = MonoTouch.Foundation.PreserveAttribute;
#else
using Preserve = MonoGame.Framework.PreserveAttribute;
#endif



namespace BR.Message
{
 	namespace Login
	{
 

		// Cmd: Login request
		[Preserve (AllMembers = true)]
		public class LoginCmd : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.CMD, true, true, POLICY.LOGIN, 0);

			public BR.GameID GameID;
			public string ID { get; private set; }
			public string Password { get; private set; }
			public LoginCmd()
				{}

			public LoginCmd( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:LoginCmd: , GameID:{1}, ID:{2}, Password:{3}",
												Prefix, GameID.ToString(), ID, Password); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				GameID = binReader.ReadGameID();

				ID = binReader.ReadString();

				Password = binReader.ReadString();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , GameID, ID, Password );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , BR.GameID InGameID, string InID, string InPassword )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 2
					+ binWriter.StringEncoder.GetByteCount(InID) + 1 + 2
					+ binWriter.StringEncoder.GetByteCount(InPassword) + 1 + 2;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InGameID);
				binWriter.Write( InID);
				binWriter.Write( InPassword);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , BR.GameID InGameID, string InID, string InPassword )

		} // public class LoginCmd : MessageBase
		[Preserve (AllMembers = true)]
		public class LoginRes : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.RES, true, true, POLICY.LOGIN, 0);

			public System.Int32 Result;
			public BR.NetAddress GameServerAddr;
			public System.UInt64 AccID;
			public System.UInt64 Ticket;
			public System.UInt64 LoginEntityUID;
			public LoginRes()
				{}

			public LoginRes( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:LoginRes: , Result:{1}, GameServerAddr:{2}, AccID:{3}, Ticket:{4}, LoginEntityUID:{5}",
												Prefix, Result.ToString(), GameServerAddr.ToString(), AccID.ToString(), Ticket.ToString(), LoginEntityUID.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				Result = binReader.ReadInt32();

				GameServerAddr = binReader.ReadNetAddress();

				AccID = binReader.ReadUInt64();

				Ticket = binReader.ReadUInt64();

				LoginEntityUID = binReader.ReadUInt64();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , Result, ref GameServerAddr, AccID, Ticket, LoginEntityUID );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult, ref BR.NetAddress InGameServerAddr, System.UInt64 InAccID, System.UInt64 InTicket, System.UInt64 InLoginEntityUID )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 4
					+ 18
					+ 8
					+ 8
					+ 8;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InResult);
				binWriter.Write(ref InGameServerAddr);
				binWriter.Write( InAccID);
				binWriter.Write( InTicket);
				binWriter.Write( InLoginEntityUID);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult, ref BR.NetAddress InGameServerAddr, System.UInt64 InAccID, System.UInt64 InTicket, System.UInt64 InLoginEntityUID )

		} // public class LoginRes : MessageBase

		// Cmd: Login request with Facebook UID
		[Preserve (AllMembers = true)]
		public class LoginByFacebookCmd : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.CMD, true, true, POLICY.LOGIN, 1);

			public BR.GameID GameID;
			public System.UInt64 UID;
			public string FaceBookName { get; private set; }
			public string EMail { get; private set; }
			public string FacebookToken { get; private set; }
			public LoginByFacebookCmd()
				{}

			public LoginByFacebookCmd( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:LoginByFacebookCmd: , GameID:{1}, UID:{2}, FaceBookName:{3}, EMail:{4}, FacebookToken:{5}",
												Prefix, GameID.ToString(), UID.ToString(), FaceBookName, EMail, FacebookToken); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				GameID = binReader.ReadGameID();

				UID = binReader.ReadUInt64();

				FaceBookName = binReader.ReadString();

				EMail = binReader.ReadString();

				FacebookToken = binReader.ReadString();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , GameID, UID, FaceBookName, EMail, FacebookToken );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , BR.GameID InGameID, System.UInt64 InUID, string InFaceBookName, string InEMail, string InFacebookToken )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 2
					+ 8
					+ binWriter.StringEncoder.GetByteCount(InFaceBookName) + 1 + 2
					+ binWriter.StringEncoder.GetByteCount(InEMail) + 1 + 2
					+ binWriter.StringEncoder.GetByteCount(InFacebookToken) + 1 + 2;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InGameID);
				binWriter.Write( InUID);
				binWriter.Write( InFaceBookName);
				binWriter.Write( InEMail);
				binWriter.Write( InFacebookToken);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , BR.GameID InGameID, System.UInt64 InUID, string InFaceBookName, string InEMail, string InFacebookToken )

		} // public class LoginByFacebookCmd : MessageBase
		[Preserve (AllMembers = true)]
		public class LoginByFacebookRes : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.RES, true, true, POLICY.LOGIN, 1);

			public System.Int32 Result;
			public BR.NetAddress GameServerAddr;
			public System.UInt64 AccID;
			public System.UInt64 Ticket;
			public System.UInt64 LoginEntityUID;
			public LoginByFacebookRes()
				{}

			public LoginByFacebookRes( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:LoginByFacebookRes: , Result:{1}, GameServerAddr:{2}, AccID:{3}, Ticket:{4}, LoginEntityUID:{5}",
												Prefix, Result.ToString(), GameServerAddr.ToString(), AccID.ToString(), Ticket.ToString(), LoginEntityUID.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				Result = binReader.ReadInt32();

				GameServerAddr = binReader.ReadNetAddress();

				AccID = binReader.ReadUInt64();

				Ticket = binReader.ReadUInt64();

				LoginEntityUID = binReader.ReadUInt64();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , Result, ref GameServerAddr, AccID, Ticket, LoginEntityUID );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult, ref BR.NetAddress InGameServerAddr, System.UInt64 InAccID, System.UInt64 InTicket, System.UInt64 InLoginEntityUID )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 4
					+ 18
					+ 8
					+ 8
					+ 8;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InResult);
				binWriter.Write(ref InGameServerAddr);
				binWriter.Write( InAccID);
				binWriter.Write( InTicket);
				binWriter.Write( InLoginEntityUID);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult, ref BR.NetAddress InGameServerAddr, System.UInt64 InAccID, System.UInt64 InTicket, System.UInt64 InLoginEntityUID )

		} // public class LoginByFacebookRes : MessageBase





	} // namespace Login
} // namespace BR.Message


