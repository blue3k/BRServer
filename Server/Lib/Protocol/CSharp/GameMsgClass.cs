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
 	namespace Game
	{
 

		// C2S: Client heart bit
		[Preserve (AllMembers = true)]
		public class HeartBitC2SEvt : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.C2SEVT, true, true, POLICY.GAME, 0);

			public HeartBitC2SEvt()
				{}

			public HeartBitC2SEvt( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:HeartBitC2SEvt: ",
												Prefix); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header   );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header   )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 ;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header   )

		} // public class HeartBitC2SEvt : MessageBase

		// Cmd: Player connected from a login server and moved to game server
		[Preserve (AllMembers = true)]
		public class JoinGameServerCmd : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.CMD, true, true, POLICY.GAME, 1);

			public System.UInt64 AccID;
			public System.UInt64 Ticket;
			public System.UInt64 LoginEntityUID;
			public JoinGameServerCmd()
				{}

			public JoinGameServerCmd( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:JoinGameServerCmd: , AccID:{1}, Ticket:{2}, LoginEntityUID:{3}",
												Prefix, AccID.ToString(), Ticket.ToString(), LoginEntityUID.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				AccID = binReader.ReadUInt64();

				Ticket = binReader.ReadUInt64();

				LoginEntityUID = binReader.ReadUInt64();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , AccID, Ticket, LoginEntityUID );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InAccID, System.UInt64 InTicket, System.UInt64 InLoginEntityUID )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 8
					+ 8
					+ 8;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InAccID);
				binWriter.Write( InTicket);
				binWriter.Write( InLoginEntityUID);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InAccID, System.UInt64 InTicket, System.UInt64 InLoginEntityUID )

		} // public class JoinGameServerCmd : MessageBase
		[Preserve (AllMembers = true)]
		public class JoinGameServerRes : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.RES, true, true, POLICY.GAME, 1);

			public System.Int32 Result;
			public string NickName { get; private set; }
			public System.UInt64 GameUID;
			public System.UInt64 PartyUID;
			public System.UInt64 PartyLeaderID;
			public BR.MatchingQueueTicket MatchingTicket;
			public JoinGameServerRes()
				{}

			public JoinGameServerRes( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:JoinGameServerRes: , Result:{1}, NickName:{2}, GameUID:{3}, PartyUID:{4}, PartyLeaderID:{5}, MatchingTicket:{6}",
												Prefix, Result.ToString(), NickName, GameUID.ToString(), PartyUID.ToString(), PartyLeaderID.ToString(), MatchingTicket.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				Result = binReader.ReadInt32();

				NickName = binReader.ReadString();

				GameUID = binReader.ReadUInt64();

				PartyUID = binReader.ReadUInt64();

				PartyLeaderID = binReader.ReadUInt64();

				MatchingTicket = binReader.ReadMatchingQueueTicket();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , Result, NickName, GameUID, PartyUID, PartyLeaderID, ref MatchingTicket );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult, string InNickName, System.UInt64 InGameUID, System.UInt64 InPartyUID, System.UInt64 InPartyLeaderID, ref BR.MatchingQueueTicket InMatchingTicket )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 4
					+ binWriter.StringEncoder.GetByteCount(InNickName) + 1 + 2
					+ 8
					+ 8
					+ 8
					+ 8;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InResult);
				binWriter.Write( InNickName);
				binWriter.Write( InGameUID);
				binWriter.Write( InPartyUID);
				binWriter.Write( InPartyLeaderID);
				binWriter.Write(ref InMatchingTicket);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult, string InNickName, System.UInt64 InGameUID, System.UInt64 InPartyUID, System.UInt64 InPartyLeaderID, ref BR.MatchingQueueTicket InMatchingTicket )

		} // public class JoinGameServerRes : MessageBase

		// Cmd: Register Google notification service ID, after this, the player will get notification from google. Only one notification ID can be active at a time
		[Preserve (AllMembers = true)]
		public class RegisterGCMCmd : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.CMD, true, true, POLICY.GAME, 2);

			public string GCMRegisteredID { get; private set; }
			public RegisterGCMCmd()
				{}

			public RegisterGCMCmd( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:RegisterGCMCmd: , GCMRegisteredID:{1}",
												Prefix, GCMRegisteredID); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				GCMRegisteredID = binReader.ReadString();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , GCMRegisteredID );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , string InGCMRegisteredID )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ binWriter.StringEncoder.GetByteCount(InGCMRegisteredID) + 1 + 2;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InGCMRegisteredID);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , string InGCMRegisteredID )

		} // public class RegisterGCMCmd : MessageBase
		[Preserve (AllMembers = true)]
		public class RegisterGCMRes : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.RES, true, true, POLICY.GAME, 2);

			public System.Int32 Result;
			public RegisterGCMRes()
				{}

			public RegisterGCMRes( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:RegisterGCMRes: , Result:{1}",
												Prefix, Result.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				Result = binReader.ReadInt32();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , Result );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 4;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InResult);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult )

		} // public class RegisterGCMRes : MessageBase

		// Cmd: Unregister Google notification service ID
		[Preserve (AllMembers = true)]
		public class UnregisterGCMCmd : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.CMD, true, true, POLICY.GAME, 3);

			public string GCMRegisteredID { get; private set; }
			public UnregisterGCMCmd()
				{}

			public UnregisterGCMCmd( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:UnregisterGCMCmd: , GCMRegisteredID:{1}",
												Prefix, GCMRegisteredID); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				GCMRegisteredID = binReader.ReadString();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , GCMRegisteredID );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , string InGCMRegisteredID )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ binWriter.StringEncoder.GetByteCount(InGCMRegisteredID) + 1 + 2;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InGCMRegisteredID);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , string InGCMRegisteredID )

		} // public class UnregisterGCMCmd : MessageBase
		[Preserve (AllMembers = true)]
		public class UnregisterGCMRes : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.RES, true, true, POLICY.GAME, 3);

			public System.Int32 Result;
			public UnregisterGCMRes()
				{}

			public UnregisterGCMRes( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:UnregisterGCMRes: , Result:{1}",
												Prefix, Result.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				Result = binReader.ReadInt32();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , Result );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 4;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InResult);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult )

		} // public class UnregisterGCMRes : MessageBase

		// Cmd: Invite friend
		[Preserve (AllMembers = true)]
		public class InviteFriendCmd : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.CMD, true, true, POLICY.GAME, 4);

			public System.UInt64 FriendID;
			public InviteFriendCmd()
				{}

			public InviteFriendCmd( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:InviteFriendCmd: , FriendID:{1}",
												Prefix, FriendID.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				FriendID = binReader.ReadUInt64();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , FriendID );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InFriendID )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 8;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InFriendID);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InFriendID )

		} // public class InviteFriendCmd : MessageBase
		[Preserve (AllMembers = true)]
		public class InviteFriendRes : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.RES, true, true, POLICY.GAME, 4);

			public System.Int32 Result;
			public InviteFriendRes()
				{}

			public InviteFriendRes( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:InviteFriendRes: , Result:{1}",
												Prefix, Result.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				Result = binReader.ReadInt32();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , Result );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 4;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InResult);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult )

		} // public class InviteFriendRes : MessageBase

		// Cmd: Accept friend request
		[Preserve (AllMembers = true)]
		public class AcceptFriendRequestCmd : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.CMD, true, true, POLICY.GAME, 5);

			public System.UInt64 InviterID;
			public System.UInt64 InviterFacebookUID;
			public AcceptFriendRequestCmd()
				{}

			public AcceptFriendRequestCmd( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:AcceptFriendRequestCmd: , InviterID:{1}, InviterFacebookUID:{2}",
												Prefix, InviterID.ToString(), InviterFacebookUID.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				InviterID = binReader.ReadUInt64();

				InviterFacebookUID = binReader.ReadUInt64();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , InviterID, InviterFacebookUID );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InInviterID, System.UInt64 InInviterFacebookUID )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 8
					+ 8;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InInviterID);
				binWriter.Write( InInviterFacebookUID);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InInviterID, System.UInt64 InInviterFacebookUID )

		} // public class AcceptFriendRequestCmd : MessageBase
		[Preserve (AllMembers = true)]
		public class AcceptFriendRequestRes : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.RES, true, true, POLICY.GAME, 5);

			public System.Int32 Result;
			public BR.FriendInformation NewFriend;
			public AcceptFriendRequestRes()
				{}

			public AcceptFriendRequestRes( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:AcceptFriendRequestRes: , Result:{1}, NewFriend:{2}",
												Prefix, Result.ToString(), NewFriend.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				Result = binReader.ReadInt32();

				NewFriend = binReader.ReadFriendInformation();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , Result, ref NewFriend );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult, ref BR.FriendInformation InNewFriend )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 4
					+ 112;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InResult);
				binWriter.Write(ref InNewFriend);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult, ref BR.FriendInformation InNewFriend )

		} // public class AcceptFriendRequestRes : MessageBase

		// S2C: Notification for friend request is accepted
		[Preserve (AllMembers = true)]
		public class FriendRequestAcceptedS2CEvt : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.S2CEVT, true, true, POLICY.GAME, 6);

			public BR.FriendInformation Accepter;
			public FriendRequestAcceptedS2CEvt()
				{}

			public FriendRequestAcceptedS2CEvt( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:FriendRequestAcceptedS2CEvt: , Accepter:{1}",
												Prefix, Accepter.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				Accepter = binReader.ReadFriendInformation();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , ref Accepter );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , ref BR.FriendInformation InAccepter )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 112;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write(ref InAccepter);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , ref BR.FriendInformation InAccepter )

		} // public class FriendRequestAcceptedS2CEvt : MessageBase

		// Cmd: Remove friden form the friend list
		[Preserve (AllMembers = true)]
		public class RemoveFriendCmd : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.CMD, true, true, POLICY.GAME, 7);

			public System.UInt64 FriendID;
			public RemoveFriendCmd()
				{}

			public RemoveFriendCmd( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:RemoveFriendCmd: , FriendID:{1}",
												Prefix, FriendID.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				FriendID = binReader.ReadUInt64();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , FriendID );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InFriendID )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 8;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InFriendID);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InFriendID )

		} // public class RemoveFriendCmd : MessageBase
		[Preserve (AllMembers = true)]
		public class RemoveFriendRes : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.RES, true, true, POLICY.GAME, 7);

			public System.Int32 Result;
			public System.UInt64 FriendID;
			public RemoveFriendRes()
				{}

			public RemoveFriendRes( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:RemoveFriendRes: , Result:{1}, FriendID:{2}",
												Prefix, Result.ToString(), FriendID.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				Result = binReader.ReadInt32();

				FriendID = binReader.ReadUInt64();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , Result, FriendID );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult, System.UInt64 InFriendID )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 4
					+ 8;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InResult);
				binWriter.Write( InFriendID);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult, System.UInt64 InFriendID )

		} // public class RemoveFriendRes : MessageBase

		// S2C: Friend removed
		[Preserve (AllMembers = true)]
		public class FriendRemovedS2CEvt : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.S2CEVT, true, true, POLICY.GAME, 8);

			public System.UInt64 FriendID;
			public FriendRemovedS2CEvt()
				{}

			public FriendRemovedS2CEvt( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:FriendRemovedS2CEvt: , FriendID:{1}",
												Prefix, FriendID.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				FriendID = binReader.ReadUInt64();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , FriendID );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InFriendID )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 8;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InFriendID);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InFriendID )

		} // public class FriendRemovedS2CEvt : MessageBase

		// Cmd: Get friend list
		[Preserve (AllMembers = true)]
		public class GetFriendListCmd : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.CMD, true, true, POLICY.GAME, 9);

			public System.UInt16 StartIndex;
			public System.UInt16 Count;
			public GetFriendListCmd()
				{}

			public GetFriendListCmd( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:GetFriendListCmd: , StartIndex:{1}, Count:{2}",
												Prefix, StartIndex.ToString(), Count.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				StartIndex = binReader.ReadUInt16();

				Count = binReader.ReadUInt16();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , StartIndex, Count );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt16 InStartIndex, System.UInt16 InCount )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 2
					+ 2;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InStartIndex);
				binWriter.Write( InCount);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt16 InStartIndex, System.UInt16 InCount )

		} // public class GetFriendListCmd : MessageBase
		[Preserve (AllMembers = true)]
		public class GetFriendListRes : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.RES, true, true, POLICY.GAME, 9);

			public System.Int32 Result;
			public System.UInt16 MaxFriendSlot;
			public System.UInt16 TotalNumberOfFriends;
			public System.UInt16 StartIndex;
			public BR.FriendInformation[] FriendList;
			public GetFriendListRes()
				{}

			public GetFriendListRes( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:GetFriendListRes: , Result:{1}, MaxFriendSlot:{2}, TotalNumberOfFriends:{3}, StartIndex:{4}, FriendList:{5}",
												Prefix, Result.ToString(), MaxFriendSlot.ToString(), TotalNumberOfFriends.ToString(), StartIndex.ToString(), FriendList.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				Result = binReader.ReadInt32();

				MaxFriendSlot = binReader.ReadUInt16();

				TotalNumberOfFriends = binReader.ReadUInt16();

				StartIndex = binReader.ReadUInt16();

				UInt16 uiSizeOfFriendList = 0;
				uiSizeOfFriendList = binReader.ReadUInt16();
				FriendList = new BR.FriendInformation[uiSizeOfFriendList];
				for( int iElement = 0; iElement < uiSizeOfFriendList; iElement++) {
				FriendList[iElement] = binReader.ReadFriendInformation();
				}

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , Result, MaxFriendSlot, TotalNumberOfFriends, StartIndex, FriendList );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult, System.UInt16 InMaxFriendSlot, System.UInt16 InTotalNumberOfFriends, System.UInt16 InStartIndex, BR.FriendInformation[] InFriendList )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 4
					+ 2
					+ 2
					+ 2
					+ 112*(InFriendList.Length) + sizeof(UInt16);

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InResult);
				binWriter.Write( InMaxFriendSlot);
				binWriter.Write( InTotalNumberOfFriends);
				binWriter.Write( InStartIndex);
				binWriter.Write(ref InFriendList);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult, System.UInt16 InMaxFriendSlot, System.UInt16 InTotalNumberOfFriends, System.UInt16 InStartIndex, BR.FriendInformation[] InFriendList )

		} // public class GetFriendListRes : MessageBase

		// Cmd: Query notification list
		[Preserve (AllMembers = true)]
		public class GetNotificationListCmd : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.CMD, true, true, POLICY.GAME, 10);

			public GetNotificationListCmd()
				{}

			public GetNotificationListCmd( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:GetNotificationListCmd: ",
												Prefix); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header   );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header   )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 ;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header   )

		} // public class GetNotificationListCmd : MessageBase
		[Preserve (AllMembers = true)]
		public class GetNotificationListRes : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.RES, true, true, POLICY.GAME, 10);

			public System.Int32 Result;
			public GetNotificationListRes()
				{}

			public GetNotificationListRes( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:GetNotificationListRes: , Result:{1}",
												Prefix, Result.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				Result = binReader.ReadInt32();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , Result );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 4;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InResult);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult )

		} // public class GetNotificationListRes : MessageBase

		// Cmd: Delete notification
		[Preserve (AllMembers = true)]
		public class DeleteNotificationCmd : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.CMD, true, true, POLICY.GAME, 11);

			public System.UInt32 NotificationID;
			public DeleteNotificationCmd()
				{}

			public DeleteNotificationCmd( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:DeleteNotificationCmd: , NotificationID:{1}",
												Prefix, NotificationID.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				NotificationID = binReader.ReadUInt32();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , NotificationID );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt32 InNotificationID )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 4;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InNotificationID);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt32 InNotificationID )

		} // public class DeleteNotificationCmd : MessageBase
		[Preserve (AllMembers = true)]
		public class DeleteNotificationRes : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.RES, true, true, POLICY.GAME, 11);

			public System.Int32 Result;
			public System.UInt32 NotificationID;
			public DeleteNotificationRes()
				{}

			public DeleteNotificationRes( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:DeleteNotificationRes: , Result:{1}, NotificationID:{2}",
												Prefix, Result.ToString(), NotificationID.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				Result = binReader.ReadInt32();

				NotificationID = binReader.ReadUInt32();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , Result, NotificationID );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult, System.UInt32 InNotificationID )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 4
					+ 4;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InResult);
				binWriter.Write( InNotificationID);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult, System.UInt32 InNotificationID )

		} // public class DeleteNotificationRes : MessageBase

		// Cmd: Set notification is read
		[Preserve (AllMembers = true)]
		public class SetNotificationReadCmd : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.CMD, true, true, POLICY.GAME, 12);

			public System.UInt32 NotificationID;
			public SetNotificationReadCmd()
				{}

			public SetNotificationReadCmd( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:SetNotificationReadCmd: , NotificationID:{1}",
												Prefix, NotificationID.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				NotificationID = binReader.ReadUInt32();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , NotificationID );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt32 InNotificationID )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 4;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InNotificationID);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt32 InNotificationID )

		} // public class SetNotificationReadCmd : MessageBase
		[Preserve (AllMembers = true)]
		public class SetNotificationReadRes : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.RES, true, true, POLICY.GAME, 12);

			public System.Int32 Result;
			public System.UInt32 NotificationID;
			public SetNotificationReadRes()
				{}

			public SetNotificationReadRes( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:SetNotificationReadRes: , Result:{1}, NotificationID:{2}",
												Prefix, Result.ToString(), NotificationID.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				Result = binReader.ReadInt32();

				NotificationID = binReader.ReadUInt32();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , Result, NotificationID );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult, System.UInt32 InNotificationID )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 4
					+ 4;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InResult);
				binWriter.Write( InNotificationID);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult, System.UInt32 InNotificationID )

		} // public class SetNotificationReadRes : MessageBase

		// Cmd: Accept notification
		[Preserve (AllMembers = true)]
		public class AcceptNotificationCmd : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.CMD, true, true, POLICY.GAME, 13);

			public System.UInt32 NotificationID;
			public AcceptNotificationCmd()
				{}

			public AcceptNotificationCmd( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:AcceptNotificationCmd: , NotificationID:{1}",
												Prefix, NotificationID.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				NotificationID = binReader.ReadUInt32();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , NotificationID );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt32 InNotificationID )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 4;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InNotificationID);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt32 InNotificationID )

		} // public class AcceptNotificationCmd : MessageBase
		[Preserve (AllMembers = true)]
		public class AcceptNotificationRes : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.RES, true, true, POLICY.GAME, 13);

			public System.Int32 Result;
			public System.UInt32 NotificationID;
			public AcceptNotificationRes()
				{}

			public AcceptNotificationRes( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:AcceptNotificationRes: , Result:{1}, NotificationID:{2}",
												Prefix, Result.ToString(), NotificationID.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				Result = binReader.ReadInt32();

				NotificationID = binReader.ReadUInt32();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , Result, NotificationID );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult, System.UInt32 InNotificationID )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 4
					+ 4;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InResult);
				binWriter.Write( InNotificationID);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult, System.UInt32 InNotificationID )

		} // public class AcceptNotificationRes : MessageBase

		// S2C: Notify new notification
		[Preserve (AllMembers = true)]
		public class NotifyS2CEvt : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.S2CEVT, true, true, POLICY.GAME, 14);

			public System.UInt32 NotificationID;
			public System.Int16 MessageID;
			public System.UInt64 MessageParam0;
			public System.UInt64 MessageParam1;
			public string MessageText { get; private set; }
			public System.Byte IsRead;
			public System.UInt64 TimeStamp;
			public NotifyS2CEvt()
				{}

			public NotifyS2CEvt( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:NotifyS2CEvt: , NotificationID:{1}, MessageID:{2}, MessageParam0:{3}, MessageParam1:{4}, MessageText:{5}, IsRead:{6}, TimeStamp:{7}",
												Prefix, NotificationID.ToString(), MessageID.ToString(), MessageParam0.ToString(), MessageParam1.ToString(), MessageText, IsRead.ToString(), TimeStamp.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				NotificationID = binReader.ReadUInt32();

				MessageID = binReader.ReadInt16();

				MessageParam0 = binReader.ReadUInt64();

				MessageParam1 = binReader.ReadUInt64();

				MessageText = binReader.ReadString();

				IsRead = binReader.ReadByte();

				TimeStamp = binReader.ReadUInt64();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , NotificationID, MessageID, MessageParam0, MessageParam1, MessageText, IsRead, TimeStamp );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt32 InNotificationID, System.Int16 InMessageID, System.UInt64 InMessageParam0, System.UInt64 InMessageParam1, string InMessageText, System.Byte InIsRead, System.UInt64 InTimeStamp )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 4
					+ 2
					+ 8
					+ 8
					+ binWriter.StringEncoder.GetByteCount(InMessageText) + 1 + 2
					+ 1
					+ 8;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InNotificationID);
				binWriter.Write( InMessageID);
				binWriter.Write( InMessageParam0);
				binWriter.Write( InMessageParam1);
				binWriter.Write( InMessageText);
				binWriter.Write( InIsRead);
				binWriter.Write( InTimeStamp);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt32 InNotificationID, System.Int16 InMessageID, System.UInt64 InMessageParam0, System.UInt64 InMessageParam1, string InMessageText, System.Byte InIsRead, System.UInt64 InTimeStamp )

		} // public class NotifyS2CEvt : MessageBase

		// Cmd: Query playerID list
		[Preserve (AllMembers = true)]
		public class FindPlayerByEMailCmd : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.CMD, true, true, POLICY.GAME, 15);

			public string PlayerEMail { get; private set; }
			public FindPlayerByEMailCmd()
				{}

			public FindPlayerByEMailCmd( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:FindPlayerByEMailCmd: , PlayerEMail:{1}",
												Prefix, PlayerEMail); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				PlayerEMail = binReader.ReadString();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , PlayerEMail );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , string InPlayerEMail )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ binWriter.StringEncoder.GetByteCount(InPlayerEMail) + 1 + 2;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InPlayerEMail);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , string InPlayerEMail )

		} // public class FindPlayerByEMailCmd : MessageBase
		[Preserve (AllMembers = true)]
		public class FindPlayerByEMailRes : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.RES, true, true, POLICY.GAME, 15);

			public System.Int32 Result;
			public BR.PlayerInformation Player;
			public FindPlayerByEMailRes()
				{}

			public FindPlayerByEMailRes( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:FindPlayerByEMailRes: , Result:{1}, Player:{2}",
												Prefix, Result.ToString(), Player.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				Result = binReader.ReadInt32();

				Player = binReader.ReadPlayerInformation();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , Result, ref Player );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult, ref BR.PlayerInformation InPlayer )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 4
					+ 96;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InResult);
				binWriter.Write(ref InPlayer);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult, ref BR.PlayerInformation InPlayer )

		} // public class FindPlayerByEMailRes : MessageBase

		// Cmd: *Request Player Status Update
		[Preserve (AllMembers = true)]
		public class RequestPlayerStatusUpdateCmd : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.CMD, true, true, POLICY.GAME, 16);

			public System.UInt64[] TargetPlayerID;
			public RequestPlayerStatusUpdateCmd()
				{}

			public RequestPlayerStatusUpdateCmd( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:RequestPlayerStatusUpdateCmd: , TargetPlayerID:{1}",
												Prefix, TargetPlayerID.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				UInt16 uiSizeOfTargetPlayerID = 0;
				uiSizeOfTargetPlayerID = binReader.ReadUInt16();
				TargetPlayerID = new System.UInt64[uiSizeOfTargetPlayerID];
				for( int iElement = 0; iElement < uiSizeOfTargetPlayerID; iElement++) {
				TargetPlayerID[iElement] = binReader.ReadUInt64();
				}

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , TargetPlayerID );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64[] InTargetPlayerID )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 8*(InTargetPlayerID.Length) + sizeof(UInt16);

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InTargetPlayerID);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64[] InTargetPlayerID )

		} // public class RequestPlayerStatusUpdateCmd : MessageBase
		[Preserve (AllMembers = true)]
		public class RequestPlayerStatusUpdateRes : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.RES, true, true, POLICY.GAME, 16);

			public System.Int32 Result;
			public RequestPlayerStatusUpdateRes()
				{}

			public RequestPlayerStatusUpdateRes( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:RequestPlayerStatusUpdateRes: , Result:{1}",
												Prefix, Result.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				Result = binReader.ReadInt32();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , Result );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 4;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InResult);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult )

		} // public class RequestPlayerStatusUpdateRes : MessageBase

		// S2C: *Notify Player Status Updated
		[Preserve (AllMembers = true)]
		public class NotifyPlayerStatusUpdatedS2CEvt : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.S2CEVT, true, true, POLICY.GAME, 17);

			public System.UInt64 PlayerID;
			public System.UInt32 LatestActiveTime;
			public System.Byte IsInGame;
			public NotifyPlayerStatusUpdatedS2CEvt()
				{}

			public NotifyPlayerStatusUpdatedS2CEvt( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:NotifyPlayerStatusUpdatedS2CEvt: , PlayerID:{1}, LatestActiveTime:{2}, IsInGame:{3}",
												Prefix, PlayerID.ToString(), LatestActiveTime.ToString(), IsInGame.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				PlayerID = binReader.ReadUInt64();

				LatestActiveTime = binReader.ReadUInt32();

				IsInGame = binReader.ReadByte();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , PlayerID, LatestActiveTime, IsInGame );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InPlayerID, System.UInt32 InLatestActiveTime, System.Byte InIsInGame )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 8
					+ 4
					+ 1;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InPlayerID);
				binWriter.Write( InLatestActiveTime);
				binWriter.Write( InIsInGame);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InPlayerID, System.UInt32 InLatestActiveTime, System.Byte InIsInGame )

		} // public class NotifyPlayerStatusUpdatedS2CEvt : MessageBase

		// Cmd: Get Ranking lise
		[Preserve (AllMembers = true)]
		public class GetRankingListCmd : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.CMD, true, true, POLICY.GAME, 18);

			public BR.RankingType RankingType;
			public System.Byte BaseRanking;
			public System.Byte Count;
			public GetRankingListCmd()
				{}

			public GetRankingListCmd( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:GetRankingListCmd: , RankingType:{1}, BaseRanking:{2}, Count:{3}",
												Prefix, RankingType.ToString(), BaseRanking.ToString(), Count.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				RankingType = binReader.ReadRankingType();

				BaseRanking = binReader.ReadByte();

				Count = binReader.ReadByte();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , RankingType, BaseRanking, Count );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , BR.RankingType InRankingType, System.Byte InBaseRanking, System.Byte InCount )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 1
					+ 1
					+ 1;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InRankingType);
				binWriter.Write( InBaseRanking);
				binWriter.Write( InCount);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , BR.RankingType InRankingType, System.Byte InBaseRanking, System.Byte InCount )

		} // public class GetRankingListCmd : MessageBase
		[Preserve (AllMembers = true)]
		public class GetRankingListRes : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.RES, true, true, POLICY.GAME, 18);

			public System.Int32 Result;
			public BR.TotalRankingPlayerInformation[] Ranking;
			public GetRankingListRes()
				{}

			public GetRankingListRes( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:GetRankingListRes: , Result:{1}, Ranking:{2}",
												Prefix, Result.ToString(), Ranking.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				Result = binReader.ReadInt32();

				UInt16 uiSizeOfRanking = 0;
				uiSizeOfRanking = binReader.ReadUInt16();
				Ranking = new BR.TotalRankingPlayerInformation[uiSizeOfRanking];
				for( int iElement = 0; iElement < uiSizeOfRanking; iElement++) {
				Ranking[iElement] = binReader.ReadTotalRankingPlayerInformation();
				}

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , Result, Ranking );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult, BR.TotalRankingPlayerInformation[] InRanking )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 4
					+ 100*(InRanking.Length) + sizeof(UInt16);

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InResult);
				binWriter.Write(ref InRanking);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult, BR.TotalRankingPlayerInformation[] InRanking )

		} // public class GetRankingListRes : MessageBase

		// Cmd: Game user game play information
		[Preserve (AllMembers = true)]
		public class GetUserGamePlayerInfoCmd : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.CMD, true, true, POLICY.GAME, 19);

			public GetUserGamePlayerInfoCmd()
				{}

			public GetUserGamePlayerInfoCmd( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:GetUserGamePlayerInfoCmd: ",
												Prefix); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header   );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header   )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 ;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header   )

		} // public class GetUserGamePlayerInfoCmd : MessageBase
		[Preserve (AllMembers = true)]
		public class GetUserGamePlayerInfoRes : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.RES, true, true, POLICY.GAME, 19);

			public System.Int32 Result;
			public System.Int16 Level;
			public System.Int64 Exp;
			public System.Int64 GameMoney;
			public System.Int64 Gem;
			public System.Int16 Stamina;
			public System.Int32 TotalPlayed;
			public System.Int32 WinPlaySC;
			public System.Int32 WinPlaySM;
			public System.Int32 WinPlaySS;
			public System.Int32 LosePlaySC;
			public System.Int32 LosePlaySM;
			public System.Int32 LosePlaySS;
			public System.Int32 WinPlayNC;
			public System.Int32 WinPlayNM;
			public System.Int32 WinPlayNS;
			public System.Int32 LosePlayNC;
			public System.Int32 LosePlayNM;
			public System.Int32 LosePlayNS;
			public System.Int32 WeeklyWin;
			public System.Int32 WeeklyLose;
			public GetUserGamePlayerInfoRes()
				{}

			public GetUserGamePlayerInfoRes( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:GetUserGamePlayerInfoRes: , Result:{1}, Level:{2}, Exp:{3}, GameMoney:{4}, Gem:{5}, Stamina:{6}, TotalPlayed:{7}, WinPlaySC:{8}, WinPlaySM:{9}, WinPlaySS:{10}, LosePlaySC:{11}, LosePlaySM:{12}, LosePlaySS:{13}, WinPlayNC:{14}, WinPlayNM:{15}, WinPlayNS:{16}, LosePlayNC:{17}, LosePlayNM:{18}, LosePlayNS:{19}, WeeklyWin:{20}, WeeklyLose:{21}",
												Prefix, Result.ToString(), Level.ToString(), Exp.ToString(), GameMoney.ToString(), Gem.ToString(), Stamina.ToString(), TotalPlayed.ToString(), WinPlaySC.ToString(), WinPlaySM.ToString(), WinPlaySS.ToString(), LosePlaySC.ToString(), LosePlaySM.ToString(), LosePlaySS.ToString(), WinPlayNC.ToString(), WinPlayNM.ToString(), WinPlayNS.ToString(), LosePlayNC.ToString(), LosePlayNM.ToString(), LosePlayNS.ToString(), WeeklyWin.ToString(), WeeklyLose.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				Result = binReader.ReadInt32();

				Level = binReader.ReadInt16();

				Exp = binReader.ReadInt64();

				GameMoney = binReader.ReadInt64();

				Gem = binReader.ReadInt64();

				Stamina = binReader.ReadInt16();

				TotalPlayed = binReader.ReadInt32();

				WinPlaySC = binReader.ReadInt32();

				WinPlaySM = binReader.ReadInt32();

				WinPlaySS = binReader.ReadInt32();

				LosePlaySC = binReader.ReadInt32();

				LosePlaySM = binReader.ReadInt32();

				LosePlaySS = binReader.ReadInt32();

				WinPlayNC = binReader.ReadInt32();

				WinPlayNM = binReader.ReadInt32();

				WinPlayNS = binReader.ReadInt32();

				LosePlayNC = binReader.ReadInt32();

				LosePlayNM = binReader.ReadInt32();

				LosePlayNS = binReader.ReadInt32();

				WeeklyWin = binReader.ReadInt32();

				WeeklyLose = binReader.ReadInt32();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , Result, Level, Exp, GameMoney, Gem, Stamina, TotalPlayed, WinPlaySC, WinPlaySM, WinPlaySS, LosePlaySC, LosePlaySM, LosePlaySS, WinPlayNC, WinPlayNM, WinPlayNS, LosePlayNC, LosePlayNM, LosePlayNS, WeeklyWin, WeeklyLose );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult, System.Int16 InLevel, System.Int64 InExp, System.Int64 InGameMoney, System.Int64 InGem, System.Int16 InStamina, System.Int32 InTotalPlayed, System.Int32 InWinPlaySC, System.Int32 InWinPlaySM, System.Int32 InWinPlaySS, System.Int32 InLosePlaySC, System.Int32 InLosePlaySM, System.Int32 InLosePlaySS, System.Int32 InWinPlayNC, System.Int32 InWinPlayNM, System.Int32 InWinPlayNS, System.Int32 InLosePlayNC, System.Int32 InLosePlayNM, System.Int32 InLosePlayNS, System.Int32 InWeeklyWin, System.Int32 InWeeklyLose )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 4
					+ 2
					+ 8
					+ 8
					+ 8
					+ 2
					+ 4
					+ 4
					+ 4
					+ 4
					+ 4
					+ 4
					+ 4
					+ 4
					+ 4
					+ 4
					+ 4
					+ 4
					+ 4
					+ 4
					+ 4;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InResult);
				binWriter.Write( InLevel);
				binWriter.Write( InExp);
				binWriter.Write( InGameMoney);
				binWriter.Write( InGem);
				binWriter.Write( InStamina);
				binWriter.Write( InTotalPlayed);
				binWriter.Write( InWinPlaySC);
				binWriter.Write( InWinPlaySM);
				binWriter.Write( InWinPlaySS);
				binWriter.Write( InLosePlaySC);
				binWriter.Write( InLosePlaySM);
				binWriter.Write( InLosePlaySS);
				binWriter.Write( InWinPlayNC);
				binWriter.Write( InWinPlayNM);
				binWriter.Write( InWinPlayNS);
				binWriter.Write( InLosePlayNC);
				binWriter.Write( InLosePlayNM);
				binWriter.Write( InLosePlayNS);
				binWriter.Write( InWeeklyWin);
				binWriter.Write( InWeeklyLose);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult, System.Int16 InLevel, System.Int64 InExp, System.Int64 InGameMoney, System.Int64 InGem, System.Int16 InStamina, System.Int32 InTotalPlayed, System.Int32 InWinPlaySC, System.Int32 InWinPlaySM, System.Int32 InWinPlaySS, System.Int32 InLosePlaySC, System.Int32 InLosePlaySM, System.Int32 InLosePlaySS, System.Int32 InWinPlayNC, System.Int32 InWinPlayNM, System.Int32 InWinPlayNS, System.Int32 InLosePlayNC, System.Int32 InLosePlayNM, System.Int32 InLosePlayNS, System.Int32 InWeeklyWin, System.Int32 InWeeklyLose )

		} // public class GetUserGamePlayerInfoRes : MessageBase

		// Cmd: Game game play information
		[Preserve (AllMembers = true)]
		public class GetGamePlayerInfoCmd : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.CMD, true, true, POLICY.GAME, 20);

			public System.UInt64 PlayerID;
			public GetGamePlayerInfoCmd()
				{}

			public GetGamePlayerInfoCmd( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:GetGamePlayerInfoCmd: , PlayerID:{1}",
												Prefix, PlayerID.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				PlayerID = binReader.ReadUInt64();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , PlayerID );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InPlayerID )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 8;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InPlayerID);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InPlayerID )

		} // public class GetGamePlayerInfoCmd : MessageBase
		[Preserve (AllMembers = true)]
		public class GetGamePlayerInfoRes : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.RES, true, true, POLICY.GAME, 20);

			public System.Int32 Result;
			public System.UInt64 PlayerID;
			public System.Int16 Level;
			public System.Int32 TotalPlayed;
			public System.Int32 WinPlaySC;
			public System.Int32 WinPlaySM;
			public System.Int32 WinPlaySS;
			public System.Int32 LosePlaySC;
			public System.Int32 LosePlaySM;
			public System.Int32 LosePlaySS;
			public System.Int32 WinPlayNC;
			public System.Int32 WinPlayNM;
			public System.Int32 WinPlayNS;
			public System.Int32 LosePlayNC;
			public System.Int32 LosePlayNM;
			public System.Int32 LosePlayNS;
			public System.Int32 WeeklyWin;
			public System.Int32 WeeklyLose;
			public GetGamePlayerInfoRes()
				{}

			public GetGamePlayerInfoRes( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:GetGamePlayerInfoRes: , Result:{1}, PlayerID:{2}, Level:{3}, TotalPlayed:{4}, WinPlaySC:{5}, WinPlaySM:{6}, WinPlaySS:{7}, LosePlaySC:{8}, LosePlaySM:{9}, LosePlaySS:{10}, WinPlayNC:{11}, WinPlayNM:{12}, WinPlayNS:{13}, LosePlayNC:{14}, LosePlayNM:{15}, LosePlayNS:{16}, WeeklyWin:{17}, WeeklyLose:{18}",
												Prefix, Result.ToString(), PlayerID.ToString(), Level.ToString(), TotalPlayed.ToString(), WinPlaySC.ToString(), WinPlaySM.ToString(), WinPlaySS.ToString(), LosePlaySC.ToString(), LosePlaySM.ToString(), LosePlaySS.ToString(), WinPlayNC.ToString(), WinPlayNM.ToString(), WinPlayNS.ToString(), LosePlayNC.ToString(), LosePlayNM.ToString(), LosePlayNS.ToString(), WeeklyWin.ToString(), WeeklyLose.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				Result = binReader.ReadInt32();

				PlayerID = binReader.ReadUInt64();

				Level = binReader.ReadInt16();

				TotalPlayed = binReader.ReadInt32();

				WinPlaySC = binReader.ReadInt32();

				WinPlaySM = binReader.ReadInt32();

				WinPlaySS = binReader.ReadInt32();

				LosePlaySC = binReader.ReadInt32();

				LosePlaySM = binReader.ReadInt32();

				LosePlaySS = binReader.ReadInt32();

				WinPlayNC = binReader.ReadInt32();

				WinPlayNM = binReader.ReadInt32();

				WinPlayNS = binReader.ReadInt32();

				LosePlayNC = binReader.ReadInt32();

				LosePlayNM = binReader.ReadInt32();

				LosePlayNS = binReader.ReadInt32();

				WeeklyWin = binReader.ReadInt32();

				WeeklyLose = binReader.ReadInt32();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , Result, PlayerID, Level, TotalPlayed, WinPlaySC, WinPlaySM, WinPlaySS, LosePlaySC, LosePlaySM, LosePlaySS, WinPlayNC, WinPlayNM, WinPlayNS, LosePlayNC, LosePlayNM, LosePlayNS, WeeklyWin, WeeklyLose );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult, System.UInt64 InPlayerID, System.Int16 InLevel, System.Int32 InTotalPlayed, System.Int32 InWinPlaySC, System.Int32 InWinPlaySM, System.Int32 InWinPlaySS, System.Int32 InLosePlaySC, System.Int32 InLosePlaySM, System.Int32 InLosePlaySS, System.Int32 InWinPlayNC, System.Int32 InWinPlayNM, System.Int32 InWinPlayNS, System.Int32 InLosePlayNC, System.Int32 InLosePlayNM, System.Int32 InLosePlayNS, System.Int32 InWeeklyWin, System.Int32 InWeeklyLose )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 4
					+ 8
					+ 2
					+ 4
					+ 4
					+ 4
					+ 4
					+ 4
					+ 4
					+ 4
					+ 4
					+ 4
					+ 4
					+ 4
					+ 4
					+ 4
					+ 4
					+ 4;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InResult);
				binWriter.Write( InPlayerID);
				binWriter.Write( InLevel);
				binWriter.Write( InTotalPlayed);
				binWriter.Write( InWinPlaySC);
				binWriter.Write( InWinPlaySM);
				binWriter.Write( InWinPlaySS);
				binWriter.Write( InLosePlaySC);
				binWriter.Write( InLosePlaySM);
				binWriter.Write( InLosePlaySS);
				binWriter.Write( InWinPlayNC);
				binWriter.Write( InWinPlayNM);
				binWriter.Write( InWinPlayNS);
				binWriter.Write( InLosePlayNC);
				binWriter.Write( InLosePlayNM);
				binWriter.Write( InLosePlayNS);
				binWriter.Write( InWeeklyWin);
				binWriter.Write( InWeeklyLose);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult, System.UInt64 InPlayerID, System.Int16 InLevel, System.Int32 InTotalPlayed, System.Int32 InWinPlaySC, System.Int32 InWinPlaySM, System.Int32 InWinPlaySS, System.Int32 InLosePlaySC, System.Int32 InLosePlaySM, System.Int32 InLosePlaySS, System.Int32 InWinPlayNC, System.Int32 InWinPlayNM, System.Int32 InWinPlayNS, System.Int32 InLosePlayNC, System.Int32 InLosePlayNM, System.Int32 InLosePlayNS, System.Int32 InWeeklyWin, System.Int32 InWeeklyLose )

		} // public class GetGamePlayerInfoRes : MessageBase

		// S2C: Player level up event
		[Preserve (AllMembers = true)]
		public class LevelUpS2CEvt : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.S2CEVT, true, true, POLICY.GAME, 21);

			public System.UInt64 CurrentTotalExp;
			public System.UInt32 CurrentLevel;
			public LevelUpS2CEvt()
				{}

			public LevelUpS2CEvt( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:LevelUpS2CEvt: , CurrentTotalExp:{1}, CurrentLevel:{2}",
												Prefix, CurrentTotalExp.ToString(), CurrentLevel.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				CurrentTotalExp = binReader.ReadUInt64();

				CurrentLevel = binReader.ReadUInt32();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , CurrentTotalExp, CurrentLevel );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InCurrentTotalExp, System.UInt32 InCurrentLevel )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 8
					+ 4;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InCurrentTotalExp);
				binWriter.Write( InCurrentLevel);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InCurrentTotalExp, System.UInt32 InCurrentLevel )

		} // public class LevelUpS2CEvt : MessageBase

		// Cmd: Change NickName
		[Preserve (AllMembers = true)]
		public class SetNickNameCmd : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.CMD, true, true, POLICY.GAME, 22);

			public string NickName { get; private set; }
			public SetNickNameCmd()
				{}

			public SetNickNameCmd( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:SetNickNameCmd: , NickName:{1}",
												Prefix, NickName); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				NickName = binReader.ReadString();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , NickName );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , string InNickName )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ binWriter.StringEncoder.GetByteCount(InNickName) + 1 + 2;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InNickName);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , string InNickName )

		} // public class SetNickNameCmd : MessageBase
		[Preserve (AllMembers = true)]
		public class SetNickNameRes : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.RES, true, true, POLICY.GAME, 22);

			public System.Int32 Result;
			public SetNickNameRes()
				{}

			public SetNickNameRes( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:SetNickNameRes: , Result:{1}",
												Prefix, Result.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				Result = binReader.ReadInt32();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , Result );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 4;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InResult);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult )

		} // public class SetNickNameRes : MessageBase

		// Cmd: Create Party
		[Preserve (AllMembers = true)]
		public class CreatePartyCmd : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.CMD, true, true, POLICY.GAME, 23);

			public CreatePartyCmd()
				{}

			public CreatePartyCmd( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:CreatePartyCmd: ",
												Prefix); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header   );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header   )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 ;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header   )

		} // public class CreatePartyCmd : MessageBase
		[Preserve (AllMembers = true)]
		public class CreatePartyRes : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.RES, true, true, POLICY.GAME, 23);

			public System.Int32 Result;
			public System.UInt64 PartyUID;
			public CreatePartyRes()
				{}

			public CreatePartyRes( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:CreatePartyRes: , Result:{1}, PartyUID:{2}",
												Prefix, Result.ToString(), PartyUID.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				Result = binReader.ReadInt32();

				PartyUID = binReader.ReadUInt64();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , Result, PartyUID );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult, System.UInt64 InPartyUID )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 4
					+ 8;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InResult);
				binWriter.Write( InPartyUID);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult, System.UInt64 InPartyUID )

		} // public class CreatePartyRes : MessageBase

		// Cmd: Join party
		[Preserve (AllMembers = true)]
		public class JoinPartyCmd : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.CMD, true, true, POLICY.GAME, 24);

			public System.UInt64 PartyUID;
			public System.UInt64 InviterID;
			public JoinPartyCmd()
				{}

			public JoinPartyCmd( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:JoinPartyCmd: , PartyUID:{1}, InviterID:{2}",
												Prefix, PartyUID.ToString(), InviterID.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				PartyUID = binReader.ReadUInt64();

				InviterID = binReader.ReadUInt64();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , PartyUID, InviterID );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InPartyUID, System.UInt64 InInviterID )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 8
					+ 8;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InPartyUID);
				binWriter.Write( InInviterID);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InPartyUID, System.UInt64 InInviterID )

		} // public class JoinPartyCmd : MessageBase
		[Preserve (AllMembers = true)]
		public class JoinPartyRes : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.RES, true, true, POLICY.GAME, 24);

			public System.Int32 Result;
			public System.UInt64 PartyUID;
			public System.UInt64 PartyLeaderID;
			public System.Byte[] ChatHistoryData;
			public JoinPartyRes()
				{}

			public JoinPartyRes( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:JoinPartyRes: , Result:{1}, PartyUID:{2}, PartyLeaderID:{3}, ChatHistoryData:{4}",
												Prefix, Result.ToString(), PartyUID.ToString(), PartyLeaderID.ToString(), ChatHistoryData.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				Result = binReader.ReadInt32();

				PartyUID = binReader.ReadUInt64();

				PartyLeaderID = binReader.ReadUInt64();

				UInt16 uiSizeOfChatHistoryData = 0;
				uiSizeOfChatHistoryData = binReader.ReadUInt16();
				ChatHistoryData = new System.Byte[uiSizeOfChatHistoryData];
				binReader.ReadBytes(ChatHistoryData);

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , Result, PartyUID, PartyLeaderID, ChatHistoryData );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult, System.UInt64 InPartyUID, System.UInt64 InPartyLeaderID, System.Byte[] InChatHistoryData )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 4
					+ 8
					+ 8
					+ 1*(InChatHistoryData.Length) + sizeof(UInt16);

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InResult);
				binWriter.Write( InPartyUID);
				binWriter.Write( InPartyLeaderID);
				binWriter.Write( InChatHistoryData);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult, System.UInt64 InPartyUID, System.UInt64 InPartyLeaderID, System.Byte[] InChatHistoryData )

		} // public class JoinPartyRes : MessageBase

		// S2C: Player Joined event
		[Preserve (AllMembers = true)]
		public class PartyPlayerJoinedS2CEvt : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.S2CEVT, true, true, POLICY.GAME, 25);

			public System.UInt64 PartyUID;
			public BR.PlayerInformation JoinedPlayer;
			public PartyPlayerJoinedS2CEvt()
				{}

			public PartyPlayerJoinedS2CEvt( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:PartyPlayerJoinedS2CEvt: , PartyUID:{1}, JoinedPlayer:{2}",
												Prefix, PartyUID.ToString(), JoinedPlayer.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				PartyUID = binReader.ReadUInt64();

				JoinedPlayer = binReader.ReadPlayerInformation();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , PartyUID, ref JoinedPlayer );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InPartyUID, ref BR.PlayerInformation InJoinedPlayer )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 8
					+ 96;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InPartyUID);
				binWriter.Write(ref InJoinedPlayer);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InPartyUID, ref BR.PlayerInformation InJoinedPlayer )

		} // public class PartyPlayerJoinedS2CEvt : MessageBase

		// S2C: Party leader changed event
		[Preserve (AllMembers = true)]
		public class PartyLeaderChangedS2CEvt : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.S2CEVT, true, true, POLICY.GAME, 26);

			public System.UInt64 PartyUID;
			public System.UInt64 NewLeaderID;
			public PartyLeaderChangedS2CEvt()
				{}

			public PartyLeaderChangedS2CEvt( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:PartyLeaderChangedS2CEvt: , PartyUID:{1}, NewLeaderID:{2}",
												Prefix, PartyUID.ToString(), NewLeaderID.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				PartyUID = binReader.ReadUInt64();

				NewLeaderID = binReader.ReadUInt64();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , PartyUID, NewLeaderID );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InPartyUID, System.UInt64 InNewLeaderID )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 8
					+ 8;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InPartyUID);
				binWriter.Write( InNewLeaderID);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InPartyUID, System.UInt64 InNewLeaderID )

		} // public class PartyLeaderChangedS2CEvt : MessageBase

		// Cmd: Leave party command
		[Preserve (AllMembers = true)]
		public class LeavePartyCmd : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.CMD, true, true, POLICY.GAME, 27);

			public System.UInt64 PartyUID;
			public System.UInt64 PlayerID;
			public LeavePartyCmd()
				{}

			public LeavePartyCmd( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:LeavePartyCmd: , PartyUID:{1}, PlayerID:{2}",
												Prefix, PartyUID.ToString(), PlayerID.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				PartyUID = binReader.ReadUInt64();

				PlayerID = binReader.ReadUInt64();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , PartyUID, PlayerID );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InPartyUID, System.UInt64 InPlayerID )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 8
					+ 8;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InPartyUID);
				binWriter.Write( InPlayerID);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InPartyUID, System.UInt64 InPlayerID )

		} // public class LeavePartyCmd : MessageBase
		[Preserve (AllMembers = true)]
		public class LeavePartyRes : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.RES, true, true, POLICY.GAME, 27);

			public System.Int32 Result;
			public LeavePartyRes()
				{}

			public LeavePartyRes( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:LeavePartyRes: , Result:{1}",
												Prefix, Result.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				Result = binReader.ReadInt32();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , Result );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 4;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InResult);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult )

		} // public class LeavePartyRes : MessageBase

		// S2C: Party Player left event
		[Preserve (AllMembers = true)]
		public class PartyPlayerLeftS2CEvt : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.S2CEVT, true, true, POLICY.GAME, 28);

			public System.UInt64 PartyUID;
			public System.UInt64 LeftPlayerID;
			public PartyPlayerLeftS2CEvt()
				{}

			public PartyPlayerLeftS2CEvt( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:PartyPlayerLeftS2CEvt: , PartyUID:{1}, LeftPlayerID:{2}",
												Prefix, PartyUID.ToString(), LeftPlayerID.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				PartyUID = binReader.ReadUInt64();

				LeftPlayerID = binReader.ReadUInt64();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , PartyUID, LeftPlayerID );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InPartyUID, System.UInt64 InLeftPlayerID )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 8
					+ 8;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InPartyUID);
				binWriter.Write( InLeftPlayerID);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InPartyUID, System.UInt64 InLeftPlayerID )

		} // public class PartyPlayerLeftS2CEvt : MessageBase

		// Cmd: Kick player from the party
		[Preserve (AllMembers = true)]
		public class PartyKickPlayerCmd : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.CMD, true, true, POLICY.GAME, 29);

			public System.UInt64 PartyUID;
			public System.UInt64 PlayerID;
			public System.UInt64 PlayerToKick;
			public PartyKickPlayerCmd()
				{}

			public PartyKickPlayerCmd( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:PartyKickPlayerCmd: , PartyUID:{1}, PlayerID:{2}, PlayerToKick:{3}",
												Prefix, PartyUID.ToString(), PlayerID.ToString(), PlayerToKick.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				PartyUID = binReader.ReadUInt64();

				PlayerID = binReader.ReadUInt64();

				PlayerToKick = binReader.ReadUInt64();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , PartyUID, PlayerID, PlayerToKick );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InPartyUID, System.UInt64 InPlayerID, System.UInt64 InPlayerToKick )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 8
					+ 8
					+ 8;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InPartyUID);
				binWriter.Write( InPlayerID);
				binWriter.Write( InPlayerToKick);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InPartyUID, System.UInt64 InPlayerID, System.UInt64 InPlayerToKick )

		} // public class PartyKickPlayerCmd : MessageBase
		[Preserve (AllMembers = true)]
		public class PartyKickPlayerRes : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.RES, true, true, POLICY.GAME, 29);

			public System.Int32 Result;
			public PartyKickPlayerRes()
				{}

			public PartyKickPlayerRes( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:PartyKickPlayerRes: , Result:{1}",
												Prefix, Result.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				Result = binReader.ReadInt32();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , Result );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 4;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InResult);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult )

		} // public class PartyKickPlayerRes : MessageBase

		// S2C: Party Player kicked message
		[Preserve (AllMembers = true)]
		public class PartyPlayerKickedS2CEvt : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.S2CEVT, true, true, POLICY.GAME, 30);

			public System.UInt64 PartyUID;
			public System.UInt64 KickedPlayerID;
			public PartyPlayerKickedS2CEvt()
				{}

			public PartyPlayerKickedS2CEvt( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:PartyPlayerKickedS2CEvt: , PartyUID:{1}, KickedPlayerID:{2}",
												Prefix, PartyUID.ToString(), KickedPlayerID.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				PartyUID = binReader.ReadUInt64();

				KickedPlayerID = binReader.ReadUInt64();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , PartyUID, KickedPlayerID );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InPartyUID, System.UInt64 InKickedPlayerID )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 8
					+ 8;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InPartyUID);
				binWriter.Write( InKickedPlayerID);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InPartyUID, System.UInt64 InKickedPlayerID )

		} // public class PartyPlayerKickedS2CEvt : MessageBase

		// Cmd: Invite a player to the party
		[Preserve (AllMembers = true)]
		public class PartyInviteCmd : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.CMD, true, true, POLICY.GAME, 31);

			public System.UInt64 InviteTargetID;
			public PartyInviteCmd()
				{}

			public PartyInviteCmd( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:PartyInviteCmd: , InviteTargetID:{1}",
												Prefix, InviteTargetID.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				InviteTargetID = binReader.ReadUInt64();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , InviteTargetID );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InInviteTargetID )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 8;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InInviteTargetID);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InInviteTargetID )

		} // public class PartyInviteCmd : MessageBase
		[Preserve (AllMembers = true)]
		public class PartyInviteRes : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.RES, true, true, POLICY.GAME, 31);

			public System.Int32 Result;
			public PartyInviteRes()
				{}

			public PartyInviteRes( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:PartyInviteRes: , Result:{1}",
												Prefix, Result.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				Result = binReader.ReadInt32();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , Result );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 4;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InResult);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult )

		} // public class PartyInviteRes : MessageBase

		// S2C: Party invite requested
		[Preserve (AllMembers = true)]
		public class PartyInviteRequestedS2CEvt : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.S2CEVT, true, true, POLICY.GAME, 32);

			public System.UInt64 InviterID;
			public string InviterName { get; private set; }
			public System.UInt64 PartyToJoinUID;
			public PartyInviteRequestedS2CEvt()
				{}

			public PartyInviteRequestedS2CEvt( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:PartyInviteRequestedS2CEvt: , InviterID:{1}, InviterName:{2}, PartyToJoinUID:{3}",
												Prefix, InviterID.ToString(), InviterName, PartyToJoinUID.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				InviterID = binReader.ReadUInt64();

				InviterName = binReader.ReadString();

				PartyToJoinUID = binReader.ReadUInt64();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , InviterID, InviterName, PartyToJoinUID );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InInviterID, string InInviterName, System.UInt64 InPartyToJoinUID )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 8
					+ binWriter.StringEncoder.GetByteCount(InInviterName) + 1 + 2
					+ 8;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InInviterID);
				binWriter.Write( InInviterName);
				binWriter.Write( InPartyToJoinUID);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InInviterID, string InInviterName, System.UInt64 InPartyToJoinUID )

		} // public class PartyInviteRequestedS2CEvt : MessageBase

		// Cmd: Send Party quick chat message
		[Preserve (AllMembers = true)]
		public class PartyQuickChatMessageCmd : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.CMD, true, true, POLICY.GAME, 33);

			public System.UInt32 QuickChatID;
			public PartyQuickChatMessageCmd()
				{}

			public PartyQuickChatMessageCmd( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:PartyQuickChatMessageCmd: , QuickChatID:{1}",
												Prefix, QuickChatID.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				QuickChatID = binReader.ReadUInt32();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , QuickChatID );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt32 InQuickChatID )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 4;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InQuickChatID);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt32 InQuickChatID )

		} // public class PartyQuickChatMessageCmd : MessageBase
		[Preserve (AllMembers = true)]
		public class PartyQuickChatMessageRes : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.RES, true, true, POLICY.GAME, 33);

			public System.Int32 Result;
			public PartyQuickChatMessageRes()
				{}

			public PartyQuickChatMessageRes( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:PartyQuickChatMessageRes: , Result:{1}",
												Prefix, Result.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				Result = binReader.ReadInt32();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , Result );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 4;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InResult);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult )

		} // public class PartyQuickChatMessageRes : MessageBase

		// S2C: Party Chatting message event
		[Preserve (AllMembers = true)]
		public class PartyQuickChatMessageS2CEvt : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.S2CEVT, true, true, POLICY.GAME, 34);

			public System.UInt64 SenderID;
			public System.UInt32 QuickChatID;
			public PartyQuickChatMessageS2CEvt()
				{}

			public PartyQuickChatMessageS2CEvt( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:PartyQuickChatMessageS2CEvt: , SenderID:{1}, QuickChatID:{2}",
												Prefix, SenderID.ToString(), QuickChatID.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				SenderID = binReader.ReadUInt64();

				QuickChatID = binReader.ReadUInt32();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , SenderID, QuickChatID );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InSenderID, System.UInt32 InQuickChatID )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 8
					+ 4;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InSenderID);
				binWriter.Write( InQuickChatID);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InSenderID, System.UInt32 InQuickChatID )

		} // public class PartyQuickChatMessageS2CEvt : MessageBase

		// Cmd: Party chatting
		[Preserve (AllMembers = true)]
		public class PartyChatMessageCmd : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.CMD, true, true, POLICY.GAME, 35);

			public string ChatMessage { get; private set; }
			public PartyChatMessageCmd()
				{}

			public PartyChatMessageCmd( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:PartyChatMessageCmd: , ChatMessage:{1}",
												Prefix, ChatMessage); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				ChatMessage = binReader.ReadString();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , ChatMessage );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , string InChatMessage )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ binWriter.StringEncoder.GetByteCount(InChatMessage) + 1 + 2;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InChatMessage);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , string InChatMessage )

		} // public class PartyChatMessageCmd : MessageBase
		[Preserve (AllMembers = true)]
		public class PartyChatMessageRes : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.RES, true, true, POLICY.GAME, 35);

			public System.Int32 Result;
			public PartyChatMessageRes()
				{}

			public PartyChatMessageRes( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:PartyChatMessageRes: , Result:{1}",
												Prefix, Result.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				Result = binReader.ReadInt32();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , Result );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 4;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InResult);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult )

		} // public class PartyChatMessageRes : MessageBase

		// S2C: Party Chatting message event
		[Preserve (AllMembers = true)]
		public class PartyChatMessageS2CEvt : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.S2CEVT, true, true, POLICY.GAME, 36);

			public System.UInt64 SenderID;
			public string SenderName { get; private set; }
			public string ChatMessage { get; private set; }
			public PartyChatMessageS2CEvt()
				{}

			public PartyChatMessageS2CEvt( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:PartyChatMessageS2CEvt: , SenderID:{1}, SenderName:{2}, ChatMessage:{3}",
												Prefix, SenderID.ToString(), SenderName, ChatMessage); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				SenderID = binReader.ReadUInt64();

				SenderName = binReader.ReadString();

				ChatMessage = binReader.ReadString();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , SenderID, SenderName, ChatMessage );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InSenderID, string InSenderName, string InChatMessage )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 8
					+ binWriter.StringEncoder.GetByteCount(InSenderName) + 1 + 2
					+ binWriter.StringEncoder.GetByteCount(InChatMessage) + 1 + 2;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InSenderID);
				binWriter.Write( InSenderName);
				binWriter.Write( InChatMessage);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InSenderID, string InSenderName, string InChatMessage )

		} // public class PartyChatMessageS2CEvt : MessageBase

		// Cmd: Join to a game
		[Preserve (AllMembers = true)]
		public class JoinGameCmd : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.CMD, true, true, POLICY.GAME, 37);

			public System.UInt64 PlayerID;
			public System.UInt64 Ticket;
			public System.UInt64 InsUID;
			public JoinGameCmd()
				{}

			public JoinGameCmd( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:JoinGameCmd: , PlayerID:{1}, Ticket:{2}, InsUID:{3}",
												Prefix, PlayerID.ToString(), Ticket.ToString(), InsUID.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				PlayerID = binReader.ReadUInt64();

				Ticket = binReader.ReadUInt64();

				InsUID = binReader.ReadUInt64();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , PlayerID, Ticket, InsUID );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InPlayerID, System.UInt64 InTicket, System.UInt64 InInsUID )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 8
					+ 8
					+ 8;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InPlayerID);
				binWriter.Write( InTicket);
				binWriter.Write( InInsUID);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InPlayerID, System.UInt64 InTicket, System.UInt64 InInsUID )

		} // public class JoinGameCmd : MessageBase
		[Preserve (AllMembers = true)]
		public class JoinGameRes : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.RES, true, true, POLICY.GAME, 37);

			public System.Int32 Result;
			public System.UInt64 InsUID;
			public System.UInt32 TimeStamp;
			public BR.GameStateID GameState;
			public System.Byte Day;
			public System.Byte MaxPlayer;
			public System.Byte PlayerIndex;
			public System.Byte PlayerCharacter;
			public BR.PlayerRole Role;
			public System.Byte Dead;
			public System.Byte[] ChatHistoryData;
			public System.Byte[] GameLogData;
			public JoinGameRes()
				{}

			public JoinGameRes( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:JoinGameRes: , Result:{1}, InsUID:{2}, TimeStamp:{3}, GameState:{4}, Day:{5}, MaxPlayer:{6}, PlayerIndex:{7}, PlayerCharacter:{8}, Role:{9}, Dead:{10}, ChatHistoryData:{11}, GameLogData:{12}",
												Prefix, Result.ToString(), InsUID.ToString(), TimeStamp.ToString(), GameState.ToString(), Day.ToString(), MaxPlayer.ToString(), PlayerIndex.ToString(), PlayerCharacter.ToString(), Role.ToString(), Dead.ToString(), ChatHistoryData.ToString(), GameLogData.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				Result = binReader.ReadInt32();

				InsUID = binReader.ReadUInt64();

				TimeStamp = binReader.ReadUInt32();

				GameState = binReader.ReadGameStateID();

				Day = binReader.ReadByte();

				MaxPlayer = binReader.ReadByte();

				PlayerIndex = binReader.ReadByte();

				PlayerCharacter = binReader.ReadByte();

				Role = binReader.ReadPlayerRole();

				Dead = binReader.ReadByte();

				UInt16 uiSizeOfChatHistoryData = 0;
				uiSizeOfChatHistoryData = binReader.ReadUInt16();
				ChatHistoryData = new System.Byte[uiSizeOfChatHistoryData];
				binReader.ReadBytes(ChatHistoryData);

				UInt16 uiSizeOfGameLogData = 0;
				uiSizeOfGameLogData = binReader.ReadUInt16();
				GameLogData = new System.Byte[uiSizeOfGameLogData];
				binReader.ReadBytes(GameLogData);

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , Result, InsUID, TimeStamp, GameState, Day, MaxPlayer, PlayerIndex, PlayerCharacter, Role, Dead, ChatHistoryData, GameLogData );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult, System.UInt64 InInsUID, System.UInt32 InTimeStamp, BR.GameStateID InGameState, System.Byte InDay, System.Byte InMaxPlayer, System.Byte InPlayerIndex, System.Byte InPlayerCharacter, BR.PlayerRole InRole, System.Byte InDead, System.Byte[] InChatHistoryData, System.Byte[] InGameLogData )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 4
					+ 8
					+ 4
					+ 1
					+ 1
					+ 1
					+ 1
					+ 1
					+ 1
					+ 1
					+ 1*(InChatHistoryData.Length) + sizeof(UInt16)
					+ 1*(InGameLogData.Length) + sizeof(UInt16);

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InResult);
				binWriter.Write( InInsUID);
				binWriter.Write( InTimeStamp);
				binWriter.Write( InGameState);
				binWriter.Write( InDay);
				binWriter.Write( InMaxPlayer);
				binWriter.Write( InPlayerIndex);
				binWriter.Write( InPlayerCharacter);
				binWriter.Write( InRole);
				binWriter.Write( InDead);
				binWriter.Write( InChatHistoryData);
				binWriter.Write( InGameLogData);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult, System.UInt64 InInsUID, System.UInt32 InTimeStamp, BR.GameStateID InGameState, System.Byte InDay, System.Byte InMaxPlayer, System.Byte InPlayerIndex, System.Byte InPlayerCharacter, BR.PlayerRole InRole, System.Byte InDead, System.Byte[] InChatHistoryData, System.Byte[] InGameLogData )

		} // public class JoinGameRes : MessageBase

		// S2C: Player Joined in the game
		[Preserve (AllMembers = true)]
		public class PlayerJoinedS2CEvt : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.S2CEVT, true, true, POLICY.GAME, 38);

			public System.UInt64 GameInsUID;
			public BR.PlayerInformation JoinedPlayer;
			public BR.PlayerRole JoinedPlayerRole;
			public System.Byte JoinedPlayerDead;
			public System.Byte JoinedPlayerIndex;
			public System.Byte JoinedPlayerCharacter;
			public PlayerJoinedS2CEvt()
				{}

			public PlayerJoinedS2CEvt( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:PlayerJoinedS2CEvt: , GameInsUID:{1}, JoinedPlayer:{2}, JoinedPlayerRole:{3}, JoinedPlayerDead:{4}, JoinedPlayerIndex:{5}, JoinedPlayerCharacter:{6}",
												Prefix, GameInsUID.ToString(), JoinedPlayer.ToString(), JoinedPlayerRole.ToString(), JoinedPlayerDead.ToString(), JoinedPlayerIndex.ToString(), JoinedPlayerCharacter.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				GameInsUID = binReader.ReadUInt64();

				JoinedPlayer = binReader.ReadPlayerInformation();

				JoinedPlayerRole = binReader.ReadPlayerRole();

				JoinedPlayerDead = binReader.ReadByte();

				JoinedPlayerIndex = binReader.ReadByte();

				JoinedPlayerCharacter = binReader.ReadByte();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , GameInsUID, ref JoinedPlayer, JoinedPlayerRole, JoinedPlayerDead, JoinedPlayerIndex, JoinedPlayerCharacter );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InGameInsUID, ref BR.PlayerInformation InJoinedPlayer, BR.PlayerRole InJoinedPlayerRole, System.Byte InJoinedPlayerDead, System.Byte InJoinedPlayerIndex, System.Byte InJoinedPlayerCharacter )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 8
					+ 96
					+ 1
					+ 1
					+ 1
					+ 1;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InGameInsUID);
				binWriter.Write(ref InJoinedPlayer);
				binWriter.Write( InJoinedPlayerRole);
				binWriter.Write( InJoinedPlayerDead);
				binWriter.Write( InJoinedPlayerIndex);
				binWriter.Write( InJoinedPlayerCharacter);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InGameInsUID, ref BR.PlayerInformation InJoinedPlayer, BR.PlayerRole InJoinedPlayerRole, System.Byte InJoinedPlayerDead, System.Byte InJoinedPlayerIndex, System.Byte InJoinedPlayerCharacter )

		} // public class PlayerJoinedS2CEvt : MessageBase

		// Cmd: Leave Game
		[Preserve (AllMembers = true)]
		public class LeaveGameCmd : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.CMD, true, true, POLICY.GAME, 39);

			public System.UInt64 GameInsUID;
			public System.UInt64 PlayerID;
			public System.UInt64 Ticket;
			public LeaveGameCmd()
				{}

			public LeaveGameCmd( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:LeaveGameCmd: , GameInsUID:{1}, PlayerID:{2}, Ticket:{3}",
												Prefix, GameInsUID.ToString(), PlayerID.ToString(), Ticket.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				GameInsUID = binReader.ReadUInt64();

				PlayerID = binReader.ReadUInt64();

				Ticket = binReader.ReadUInt64();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , GameInsUID, PlayerID, Ticket );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InGameInsUID, System.UInt64 InPlayerID, System.UInt64 InTicket )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 8
					+ 8
					+ 8;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InGameInsUID);
				binWriter.Write( InPlayerID);
				binWriter.Write( InTicket);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InGameInsUID, System.UInt64 InPlayerID, System.UInt64 InTicket )

		} // public class LeaveGameCmd : MessageBase
		[Preserve (AllMembers = true)]
		public class LeaveGameRes : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.RES, true, true, POLICY.GAME, 39);

			public System.Int32 Result;
			public LeaveGameRes()
				{}

			public LeaveGameRes( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:LeaveGameRes: , Result:{1}",
												Prefix, Result.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				Result = binReader.ReadInt32();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , Result );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 4;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InResult);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult )

		} // public class LeaveGameRes : MessageBase

		// S2C: Player left event
		[Preserve (AllMembers = true)]
		public class PlayerLeftS2CEvt : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.S2CEVT, true, true, POLICY.GAME, 40);

			public System.UInt64 GameInsUID;
			public System.UInt64 LeftPlayerID;
			public PlayerLeftS2CEvt()
				{}

			public PlayerLeftS2CEvt( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:PlayerLeftS2CEvt: , GameInsUID:{1}, LeftPlayerID:{2}",
												Prefix, GameInsUID.ToString(), LeftPlayerID.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				GameInsUID = binReader.ReadUInt64();

				LeftPlayerID = binReader.ReadUInt64();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , GameInsUID, LeftPlayerID );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InGameInsUID, System.UInt64 InLeftPlayerID )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 8
					+ 8;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InGameInsUID);
				binWriter.Write( InLeftPlayerID);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InGameInsUID, System.UInt64 InLeftPlayerID )

		} // public class PlayerLeftS2CEvt : MessageBase

		// Cmd: Kick player
		[Preserve (AllMembers = true)]
		public class KickPlayerCmd : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.CMD, true, true, POLICY.GAME, 41);

			public System.UInt64 GameInsUID;
			public System.UInt64 PlayerID;
			public System.UInt64 PlayerToKick;
			public KickPlayerCmd()
				{}

			public KickPlayerCmd( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:KickPlayerCmd: , GameInsUID:{1}, PlayerID:{2}, PlayerToKick:{3}",
												Prefix, GameInsUID.ToString(), PlayerID.ToString(), PlayerToKick.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				GameInsUID = binReader.ReadUInt64();

				PlayerID = binReader.ReadUInt64();

				PlayerToKick = binReader.ReadUInt64();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , GameInsUID, PlayerID, PlayerToKick );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InGameInsUID, System.UInt64 InPlayerID, System.UInt64 InPlayerToKick )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 8
					+ 8
					+ 8;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InGameInsUID);
				binWriter.Write( InPlayerID);
				binWriter.Write( InPlayerToKick);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InGameInsUID, System.UInt64 InPlayerID, System.UInt64 InPlayerToKick )

		} // public class KickPlayerCmd : MessageBase
		[Preserve (AllMembers = true)]
		public class KickPlayerRes : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.RES, true, true, POLICY.GAME, 41);

			public System.Int32 Result;
			public KickPlayerRes()
				{}

			public KickPlayerRes( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:KickPlayerRes: , Result:{1}",
												Prefix, Result.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				Result = binReader.ReadInt32();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , Result );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 4;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InResult);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult )

		} // public class KickPlayerRes : MessageBase

		// S2C: Player kicked
		[Preserve (AllMembers = true)]
		public class PlayerKickedS2CEvt : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.S2CEVT, true, true, POLICY.GAME, 42);

			public System.UInt64 GameInsUID;
			public System.UInt64 KickedPlayerID;
			public PlayerKickedS2CEvt()
				{}

			public PlayerKickedS2CEvt( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:PlayerKickedS2CEvt: , GameInsUID:{1}, KickedPlayerID:{2}",
												Prefix, GameInsUID.ToString(), KickedPlayerID.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				GameInsUID = binReader.ReadUInt64();

				KickedPlayerID = binReader.ReadUInt64();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , GameInsUID, KickedPlayerID );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InGameInsUID, System.UInt64 InKickedPlayerID )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 8
					+ 8;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InGameInsUID);
				binWriter.Write( InKickedPlayerID);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InGameInsUID, System.UInt64 InKickedPlayerID )

		} // public class PlayerKickedS2CEvt : MessageBase

		// Cmd: Assign role + Game state reset
		[Preserve (AllMembers = true)]
		public class AssignRoleCmd : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.CMD, true, true, POLICY.GAME, 43);

			public System.UInt64 GameInsUID;
			public System.UInt64 PlayerID;
			public System.UInt64 Ticket;
			public AssignRoleCmd()
				{}

			public AssignRoleCmd( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:AssignRoleCmd: , GameInsUID:{1}, PlayerID:{2}, Ticket:{3}",
												Prefix, GameInsUID.ToString(), PlayerID.ToString(), Ticket.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				GameInsUID = binReader.ReadUInt64();

				PlayerID = binReader.ReadUInt64();

				Ticket = binReader.ReadUInt64();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , GameInsUID, PlayerID, Ticket );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InGameInsUID, System.UInt64 InPlayerID, System.UInt64 InTicket )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 8
					+ 8
					+ 8;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InGameInsUID);
				binWriter.Write( InPlayerID);
				binWriter.Write( InTicket);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InGameInsUID, System.UInt64 InPlayerID, System.UInt64 InTicket )

		} // public class AssignRoleCmd : MessageBase
		[Preserve (AllMembers = true)]
		public class AssignRoleRes : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.RES, true, true, POLICY.GAME, 43);

			public System.Int32 Result;
			public AssignRoleRes()
				{}

			public AssignRoleRes( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:AssignRoleRes: , Result:{1}",
												Prefix, Result.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				Result = binReader.ReadInt32();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , Result );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 4;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InResult);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult )

		} // public class AssignRoleRes : MessageBase

		// S2C: Role assigned event
		[Preserve (AllMembers = true)]
		public class RoleAssignedS2CEvt : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.S2CEVT, true, true, POLICY.GAME, 44);

			public System.UInt64 GameInsUID;
			public System.UInt64 PlayerID;
			public BR.PlayerRole Role;
			public RoleAssignedS2CEvt()
				{}

			public RoleAssignedS2CEvt( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:RoleAssignedS2CEvt: , GameInsUID:{1}, PlayerID:{2}, Role:{3}",
												Prefix, GameInsUID.ToString(), PlayerID.ToString(), Role.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				GameInsUID = binReader.ReadUInt64();

				PlayerID = binReader.ReadUInt64();

				Role = binReader.ReadPlayerRole();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , GameInsUID, PlayerID, Role );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InGameInsUID, System.UInt64 InPlayerID, BR.PlayerRole InRole )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 8
					+ 8
					+ 1;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InGameInsUID);
				binWriter.Write( InPlayerID);
				binWriter.Write( InRole);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InGameInsUID, System.UInt64 InPlayerID, BR.PlayerRole InRole )

		} // public class RoleAssignedS2CEvt : MessageBase

		// Cmd: Send chatting message to the game
		[Preserve (AllMembers = true)]
		public class ChatMessageCmd : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.CMD, true, true, POLICY.GAME, 45);

			public string ChatMessage { get; private set; }
			public BR.PlayerRole Role;
			public ChatMessageCmd()
				{}

			public ChatMessageCmd( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:ChatMessageCmd: , ChatMessage:{1}, Role:{2}",
												Prefix, ChatMessage, Role.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				ChatMessage = binReader.ReadString();

				Role = binReader.ReadPlayerRole();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , ChatMessage, Role );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , string InChatMessage, BR.PlayerRole InRole )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ binWriter.StringEncoder.GetByteCount(InChatMessage) + 1 + 2
					+ 1;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InChatMessage);
				binWriter.Write( InRole);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , string InChatMessage, BR.PlayerRole InRole )

		} // public class ChatMessageCmd : MessageBase
		[Preserve (AllMembers = true)]
		public class ChatMessageRes : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.RES, true, true, POLICY.GAME, 45);

			public System.Int32 Result;
			public ChatMessageRes()
				{}

			public ChatMessageRes( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:ChatMessageRes: , Result:{1}",
												Prefix, Result.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				Result = binReader.ReadInt32();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , Result );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 4;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InResult);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult )

		} // public class ChatMessageRes : MessageBase

		// S2C: Chatting message event 
		[Preserve (AllMembers = true)]
		public class ChatMessageS2CEvt : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.S2CEVT, true, true, POLICY.GAME, 46);

			public System.UInt64 SenderID;
			public BR.PlayerRole Role;
			public string SenderName { get; private set; }
			public string ChatMessage { get; private set; }
			public ChatMessageS2CEvt()
				{}

			public ChatMessageS2CEvt( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:ChatMessageS2CEvt: , SenderID:{1}, Role:{2}, SenderName:{3}, ChatMessage:{4}",
												Prefix, SenderID.ToString(), Role.ToString(), SenderName, ChatMessage); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				SenderID = binReader.ReadUInt64();

				Role = binReader.ReadPlayerRole();

				SenderName = binReader.ReadString();

				ChatMessage = binReader.ReadString();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , SenderID, Role, SenderName, ChatMessage );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InSenderID, BR.PlayerRole InRole, string InSenderName, string InChatMessage )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 8
					+ 1
					+ binWriter.StringEncoder.GetByteCount(InSenderName) + 1 + 2
					+ binWriter.StringEncoder.GetByteCount(InChatMessage) + 1 + 2;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InSenderID);
				binWriter.Write( InRole);
				binWriter.Write( InSenderName);
				binWriter.Write( InChatMessage);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InSenderID, BR.PlayerRole InRole, string InSenderName, string InChatMessage )

		} // public class ChatMessageS2CEvt : MessageBase

		// Cmd: Advance game
		[Preserve (AllMembers = true)]
		public class AdvanceGameCmd : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.CMD, true, true, POLICY.GAME, 47);

			public System.UInt64 GameInsUID;
			public System.UInt64 PlayerID;
			public System.UInt64 Ticket;
			public AdvanceGameCmd()
				{}

			public AdvanceGameCmd( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:AdvanceGameCmd: , GameInsUID:{1}, PlayerID:{2}, Ticket:{3}",
												Prefix, GameInsUID.ToString(), PlayerID.ToString(), Ticket.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				GameInsUID = binReader.ReadUInt64();

				PlayerID = binReader.ReadUInt64();

				Ticket = binReader.ReadUInt64();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , GameInsUID, PlayerID, Ticket );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InGameInsUID, System.UInt64 InPlayerID, System.UInt64 InTicket )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 8
					+ 8
					+ 8;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InGameInsUID);
				binWriter.Write( InPlayerID);
				binWriter.Write( InTicket);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InGameInsUID, System.UInt64 InPlayerID, System.UInt64 InTicket )

		} // public class AdvanceGameCmd : MessageBase
		[Preserve (AllMembers = true)]
		public class AdvanceGameRes : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.RES, true, true, POLICY.GAME, 47);

			public System.Int32 Result;
			public AdvanceGameRes()
				{}

			public AdvanceGameRes( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:AdvanceGameRes: , Result:{1}",
												Prefix, Result.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				Result = binReader.ReadInt32();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , Result );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 4;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InResult);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult )

		} // public class AdvanceGameRes : MessageBase

		// S2C: The game state is advanced
		[Preserve (AllMembers = true)]
		public class GameAdvancedS2CEvt : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.S2CEVT, true, true, POLICY.GAME, 48);

			public System.UInt64 GameInsUID;
			public System.UInt32 TimeStamp;
			public BR.GameStateID GameState;
			public System.Byte Day;
			public GameAdvancedS2CEvt()
				{}

			public GameAdvancedS2CEvt( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:GameAdvancedS2CEvt: , GameInsUID:{1}, TimeStamp:{2}, GameState:{3}, Day:{4}",
												Prefix, GameInsUID.ToString(), TimeStamp.ToString(), GameState.ToString(), Day.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				GameInsUID = binReader.ReadUInt64();

				TimeStamp = binReader.ReadUInt32();

				GameState = binReader.ReadGameStateID();

				Day = binReader.ReadByte();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , GameInsUID, TimeStamp, GameState, Day );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InGameInsUID, System.UInt32 InTimeStamp, BR.GameStateID InGameState, System.Byte InDay )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 8
					+ 4
					+ 1
					+ 1;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InGameInsUID);
				binWriter.Write( InTimeStamp);
				binWriter.Write( InGameState);
				binWriter.Write( InDay);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InGameInsUID, System.UInt32 InTimeStamp, BR.GameStateID InGameState, System.Byte InDay )

		} // public class GameAdvancedS2CEvt : MessageBase

		// S2C: Game is ended
		[Preserve (AllMembers = true)]
		public class GameEndedS2CEvt : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.S2CEVT, true, true, POLICY.GAME, 49);

			public System.UInt64 GameInsUID;
			public BR.GameWinner Winner;
			public System.UInt32 GainedExp;
			public System.UInt32 GainedGameMoney;
			public GameEndedS2CEvt()
				{}

			public GameEndedS2CEvt( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:GameEndedS2CEvt: , GameInsUID:{1}, Winner:{2}, GainedExp:{3}, GainedGameMoney:{4}",
												Prefix, GameInsUID.ToString(), Winner.ToString(), GainedExp.ToString(), GainedGameMoney.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				GameInsUID = binReader.ReadUInt64();

				Winner = binReader.ReadGameWinner();

				GainedExp = binReader.ReadUInt32();

				GainedGameMoney = binReader.ReadUInt32();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , GameInsUID, Winner, GainedExp, GainedGameMoney );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InGameInsUID, BR.GameWinner InWinner, System.UInt32 InGainedExp, System.UInt32 InGainedGameMoney )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 8
					+ 1
					+ 4
					+ 4;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InGameInsUID);
				binWriter.Write( InWinner);
				binWriter.Write( InGainedExp);
				binWriter.Write( InGainedGameMoney);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InGameInsUID, BR.GameWinner InWinner, System.UInt32 InGainedExp, System.UInt32 InGainedGameMoney )

		} // public class GameEndedS2CEvt : MessageBase

		// Cmd: Vote game advance
		[Preserve (AllMembers = true)]
		public class VoteGameAdvanceCmd : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.CMD, true, true, POLICY.GAME, 50);

			public System.UInt64 GameInsUID;
			public System.UInt64 PlayerID;
			public System.UInt64 Ticket;
			public VoteGameAdvanceCmd()
				{}

			public VoteGameAdvanceCmd( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:VoteGameAdvanceCmd: , GameInsUID:{1}, PlayerID:{2}, Ticket:{3}",
												Prefix, GameInsUID.ToString(), PlayerID.ToString(), Ticket.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				GameInsUID = binReader.ReadUInt64();

				PlayerID = binReader.ReadUInt64();

				Ticket = binReader.ReadUInt64();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , GameInsUID, PlayerID, Ticket );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InGameInsUID, System.UInt64 InPlayerID, System.UInt64 InTicket )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 8
					+ 8
					+ 8;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InGameInsUID);
				binWriter.Write( InPlayerID);
				binWriter.Write( InTicket);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InGameInsUID, System.UInt64 InPlayerID, System.UInt64 InTicket )

		} // public class VoteGameAdvanceCmd : MessageBase
		[Preserve (AllMembers = true)]
		public class VoteGameAdvanceRes : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.RES, true, true, POLICY.GAME, 50);

			public System.Int32 Result;
			public VoteGameAdvanceRes()
				{}

			public VoteGameAdvanceRes( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:VoteGameAdvanceRes: , Result:{1}",
												Prefix, Result.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				Result = binReader.ReadInt32();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , Result );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 4;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InResult);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult )

		} // public class VoteGameAdvanceRes : MessageBase

		// S2C: GameAdvance is Voted
		[Preserve (AllMembers = true)]
		public class GameAdvanceVotedS2CEvt : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.S2CEVT, true, true, POLICY.GAME, 51);

			public System.UInt64 GameInsUID;
			public System.UInt64 Voter;
			public GameAdvanceVotedS2CEvt()
				{}

			public GameAdvanceVotedS2CEvt( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:GameAdvanceVotedS2CEvt: , GameInsUID:{1}, Voter:{2}",
												Prefix, GameInsUID.ToString(), Voter.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				GameInsUID = binReader.ReadUInt64();

				Voter = binReader.ReadUInt64();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , GameInsUID, Voter );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InGameInsUID, System.UInt64 InVoter )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 8
					+ 8;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InGameInsUID);
				binWriter.Write( InVoter);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InGameInsUID, System.UInt64 InVoter )

		} // public class GameAdvanceVotedS2CEvt : MessageBase

		// Cmd: Vote
		[Preserve (AllMembers = true)]
		public class VoteCmd : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.CMD, true, true, POLICY.GAME, 52);

			public System.UInt64 GameInsUID;
			public System.UInt64 PlayerID;
			public System.UInt64 Ticket;
			public System.UInt64 VoteTarget;
			public System.UInt32 ActionSerial;
			public VoteCmd()
				{}

			public VoteCmd( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:VoteCmd: , GameInsUID:{1}, PlayerID:{2}, Ticket:{3}, VoteTarget:{4}, ActionSerial:{5}",
												Prefix, GameInsUID.ToString(), PlayerID.ToString(), Ticket.ToString(), VoteTarget.ToString(), ActionSerial.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				GameInsUID = binReader.ReadUInt64();

				PlayerID = binReader.ReadUInt64();

				Ticket = binReader.ReadUInt64();

				VoteTarget = binReader.ReadUInt64();

				ActionSerial = binReader.ReadUInt32();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , GameInsUID, PlayerID, Ticket, VoteTarget, ActionSerial );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InGameInsUID, System.UInt64 InPlayerID, System.UInt64 InTicket, System.UInt64 InVoteTarget, System.UInt32 InActionSerial )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 8
					+ 8
					+ 8
					+ 8
					+ 4;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InGameInsUID);
				binWriter.Write( InPlayerID);
				binWriter.Write( InTicket);
				binWriter.Write( InVoteTarget);
				binWriter.Write( InActionSerial);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InGameInsUID, System.UInt64 InPlayerID, System.UInt64 InTicket, System.UInt64 InVoteTarget, System.UInt32 InActionSerial )

		} // public class VoteCmd : MessageBase
		[Preserve (AllMembers = true)]
		public class VoteRes : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.RES, true, true, POLICY.GAME, 52);

			public System.Int32 Result;
			public VoteRes()
				{}

			public VoteRes( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:VoteRes: , Result:{1}",
												Prefix, Result.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				Result = binReader.ReadInt32();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , Result );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 4;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InResult);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult )

		} // public class VoteRes : MessageBase

		// S2C: Player Voted
		[Preserve (AllMembers = true)]
		public class VotedS2CEvt : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.S2CEVT, true, true, POLICY.GAME, 53);

			public System.UInt64 GameInsUID;
			public System.UInt64 Voter;
			public System.UInt64 VotedTarget;
			public VotedS2CEvt()
				{}

			public VotedS2CEvt( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:VotedS2CEvt: , GameInsUID:{1}, Voter:{2}, VotedTarget:{3}",
												Prefix, GameInsUID.ToString(), Voter.ToString(), VotedTarget.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				GameInsUID = binReader.ReadUInt64();

				Voter = binReader.ReadUInt64();

				VotedTarget = binReader.ReadUInt64();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , GameInsUID, Voter, VotedTarget );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InGameInsUID, System.UInt64 InVoter, System.UInt64 InVotedTarget )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 8
					+ 8
					+ 8;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InGameInsUID);
				binWriter.Write( InVoter);
				binWriter.Write( InVotedTarget);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InGameInsUID, System.UInt64 InVoter, System.UInt64 InVotedTarget )

		} // public class VotedS2CEvt : MessageBase

		// S2C: Vote is ended
		[Preserve (AllMembers = true)]
		public class VoteEndS2CEvt : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.S2CEVT, true, true, POLICY.GAME, 54);

			public System.UInt64 GameInsUID;
			public System.UInt64[] Voted;
			public VoteEndS2CEvt()
				{}

			public VoteEndS2CEvt( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:VoteEndS2CEvt: , GameInsUID:{1}, Voted:{2}",
												Prefix, GameInsUID.ToString(), Voted.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				GameInsUID = binReader.ReadUInt64();

				UInt16 uiSizeOfVoted = 0;
				uiSizeOfVoted = binReader.ReadUInt16();
				Voted = new System.UInt64[uiSizeOfVoted];
				for( int iElement = 0; iElement < uiSizeOfVoted; iElement++) {
				Voted[iElement] = binReader.ReadUInt64();
				}

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , GameInsUID, Voted );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InGameInsUID, System.UInt64[] InVoted )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 8
					+ 8*(InVoted.Length) + sizeof(UInt16);

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InGameInsUID);
				binWriter.Write( InVoted);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InGameInsUID, System.UInt64[] InVoted )

		} // public class VoteEndS2CEvt : MessageBase

		// S2C: Player Killed
		[Preserve (AllMembers = true)]
		public class PlayerKilledS2CEvt : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.S2CEVT, true, true, POLICY.GAME, 55);

			public System.UInt64 GameInsUID;
			public System.UInt64 KilledPlayer;
			public BR.PlayerKilledReason Reason;
			public PlayerKilledS2CEvt()
				{}

			public PlayerKilledS2CEvt( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:PlayerKilledS2CEvt: , GameInsUID:{1}, KilledPlayer:{2}, Reason:{3}",
												Prefix, GameInsUID.ToString(), KilledPlayer.ToString(), Reason.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				GameInsUID = binReader.ReadUInt64();

				KilledPlayer = binReader.ReadUInt64();

				Reason = binReader.ReadPlayerKilledReason();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , GameInsUID, KilledPlayer, Reason );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InGameInsUID, System.UInt64 InKilledPlayer, BR.PlayerKilledReason InReason )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 8
					+ 8
					+ 1;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InGameInsUID);
				binWriter.Write( InKilledPlayer);
				binWriter.Write( InReason);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InGameInsUID, System.UInt64 InKilledPlayer, BR.PlayerKilledReason InReason )

		} // public class PlayerKilledS2CEvt : MessageBase

		// S2C: Player Voted
		[Preserve (AllMembers = true)]
		public class PlayerRevealedS2CEvt : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.S2CEVT, true, true, POLICY.GAME, 56);

			public System.UInt64 GameInsUID;
			public System.UInt64 RevealedPlayerID;
			public BR.PlayerRole Role;
			public BR.PlayerRevealedReason Reason;
			public PlayerRevealedS2CEvt()
				{}

			public PlayerRevealedS2CEvt( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:PlayerRevealedS2CEvt: , GameInsUID:{1}, RevealedPlayerID:{2}, Role:{3}, Reason:{4}",
												Prefix, GameInsUID.ToString(), RevealedPlayerID.ToString(), Role.ToString(), Reason.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				GameInsUID = binReader.ReadUInt64();

				RevealedPlayerID = binReader.ReadUInt64();

				Role = binReader.ReadPlayerRole();

				Reason = binReader.ReadPlayerRevealedReason();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , GameInsUID, RevealedPlayerID, Role, Reason );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InGameInsUID, System.UInt64 InRevealedPlayerID, BR.PlayerRole InRole, BR.PlayerRevealedReason InReason )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 8
					+ 8
					+ 1
					+ 1;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InGameInsUID);
				binWriter.Write( InRevealedPlayerID);
				binWriter.Write( InRole);
				binWriter.Write( InReason);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InGameInsUID, System.UInt64 InRevealedPlayerID, BR.PlayerRole InRole, BR.PlayerRevealedReason InReason )

		} // public class PlayerRevealedS2CEvt : MessageBase

		// Cmd: Play again with the current players
		[Preserve (AllMembers = true)]
		public class GamePlayAgainCmd : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.CMD, true, true, POLICY.GAME, 57);

			public GamePlayAgainCmd()
				{}

			public GamePlayAgainCmd( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:GamePlayAgainCmd: ",
												Prefix); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header   );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header   )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 ;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header   )

		} // public class GamePlayAgainCmd : MessageBase
		[Preserve (AllMembers = true)]
		public class GamePlayAgainRes : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.RES, true, true, POLICY.GAME, 57);

			public System.Int32 Result;
			public GamePlayAgainRes()
				{}

			public GamePlayAgainRes( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:GamePlayAgainRes: , Result:{1}",
												Prefix, Result.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				Result = binReader.ReadInt32();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , Result );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 4;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InResult);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult )

		} // public class GamePlayAgainRes : MessageBase

		// S2C: Somebody pressed play again. Only one of PartyUID and GameInsUID can have a value
		[Preserve (AllMembers = true)]
		public class GamePlayAgainS2CEvt : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.S2CEVT, true, true, POLICY.GAME, 58);

			public System.UInt64 PartyUID;
			public System.UInt64 LeadPlayer;
			public GamePlayAgainS2CEvt()
				{}

			public GamePlayAgainS2CEvt( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:GamePlayAgainS2CEvt: , PartyUID:{1}, LeadPlayer:{2}",
												Prefix, PartyUID.ToString(), LeadPlayer.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				PartyUID = binReader.ReadUInt64();

				LeadPlayer = binReader.ReadUInt64();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , PartyUID, LeadPlayer );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InPartyUID, System.UInt64 InLeadPlayer )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 8
					+ 8;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InPartyUID);
				binWriter.Write( InLeadPlayer);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InPartyUID, System.UInt64 InLeadPlayer )

		} // public class GamePlayAgainS2CEvt : MessageBase

		// Cmd: Player. revive himself
		[Preserve (AllMembers = true)]
		public class GameRevealPlayerCmd : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.CMD, true, true, POLICY.GAME, 59);

			public System.UInt64 TargetPlayerID;
			public GameRevealPlayerCmd()
				{}

			public GameRevealPlayerCmd( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:GameRevealPlayerCmd: , TargetPlayerID:{1}",
												Prefix, TargetPlayerID.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				TargetPlayerID = binReader.ReadUInt64();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , TargetPlayerID );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InTargetPlayerID )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 8;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InTargetPlayerID);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InTargetPlayerID )

		} // public class GameRevealPlayerCmd : MessageBase
		[Preserve (AllMembers = true)]
		public class GameRevealPlayerRes : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.RES, true, true, POLICY.GAME, 59);

			public System.Int32 Result;
			public System.UInt64 RevealedPlayerID;
			public BR.PlayerRole RevealedRole;
			public GameRevealPlayerRes()
				{}

			public GameRevealPlayerRes( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:GameRevealPlayerRes: , Result:{1}, RevealedPlayerID:{2}, RevealedRole:{3}",
												Prefix, Result.ToString(), RevealedPlayerID.ToString(), RevealedRole.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				Result = binReader.ReadInt32();

				RevealedPlayerID = binReader.ReadUInt64();

				RevealedRole = binReader.ReadPlayerRole();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , Result, RevealedPlayerID, RevealedRole );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult, System.UInt64 InRevealedPlayerID, BR.PlayerRole InRevealedRole )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 4
					+ 8
					+ 1;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InResult);
				binWriter.Write( InRevealedPlayerID);
				binWriter.Write( InRevealedRole);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult, System.UInt64 InRevealedPlayerID, BR.PlayerRole InRevealedRole )

		} // public class GameRevealPlayerRes : MessageBase

		// Cmd: Player. revive himself
		[Preserve (AllMembers = true)]
		public class GamePlayerReviveCmd : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.CMD, true, true, POLICY.GAME, 60);

			public GamePlayerReviveCmd()
				{}

			public GamePlayerReviveCmd( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:GamePlayerReviveCmd: ",
												Prefix); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header   );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header   )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 ;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header   )

		} // public class GamePlayerReviveCmd : MessageBase
		[Preserve (AllMembers = true)]
		public class GamePlayerReviveRes : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.RES, true, true, POLICY.GAME, 60);

			public System.Int32 Result;
			public GamePlayerReviveRes()
				{}

			public GamePlayerReviveRes( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:GamePlayerReviveRes: , Result:{1}",
												Prefix, Result.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				Result = binReader.ReadInt32();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , Result );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 4;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InResult);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult )

		} // public class GamePlayerReviveRes : MessageBase

		// S2C: Player is revived
		[Preserve (AllMembers = true)]
		public class GamePlayerRevivedS2CEvt : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.S2CEVT, true, true, POLICY.GAME, 61);

			public System.UInt64 RevivedPlayerID;
			public GamePlayerRevivedS2CEvt()
				{}

			public GamePlayerRevivedS2CEvt( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:GamePlayerRevivedS2CEvt: , RevivedPlayerID:{1}",
												Prefix, RevivedPlayerID.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				RevivedPlayerID = binReader.ReadUInt64();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , RevivedPlayerID );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InRevivedPlayerID )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 8;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InRevivedPlayerID);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InRevivedPlayerID )

		} // public class GamePlayerRevivedS2CEvt : MessageBase

		// Cmd: Request Game match
		[Preserve (AllMembers = true)]
		public class RequestGameMatchCmd : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.CMD, true, true, POLICY.GAME, 62);

			public System.Byte NumPlayer;
			public RequestGameMatchCmd()
				{}

			public RequestGameMatchCmd( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:RequestGameMatchCmd: , NumPlayer:{1}",
												Prefix, NumPlayer.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				NumPlayer = binReader.ReadByte();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , NumPlayer );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Byte InNumPlayer )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 1;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InNumPlayer);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Byte InNumPlayer )

		} // public class RequestGameMatchCmd : MessageBase
		[Preserve (AllMembers = true)]
		public class RequestGameMatchRes : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.RES, true, true, POLICY.GAME, 62);

			public System.Int32 Result;
			public RequestGameMatchRes()
				{}

			public RequestGameMatchRes( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:RequestGameMatchRes: , Result:{1}",
												Prefix, Result.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				Result = binReader.ReadInt32();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , Result );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 4;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InResult);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult )

		} // public class RequestGameMatchRes : MessageBase

		// S2C: Game matched
		[Preserve (AllMembers = true)]
		public class GameMatchedS2CEvt : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.S2CEVT, true, true, POLICY.GAME, 63);

			public System.UInt64 InsUID;
			public System.UInt32 TimeStamp;
			public BR.GameStateID GameState;
			public System.Byte Day;
			public System.Byte MaxPlayer;
			public System.Byte PlayerIndex;
			public System.Byte PlayerCharacter;
			public BR.PlayerRole Role;
			public System.Byte Dead;
			public System.Byte[] ChatHistoryData;
			public System.Byte[] GameLogData;
			public GameMatchedS2CEvt()
				{}

			public GameMatchedS2CEvt( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:GameMatchedS2CEvt: , InsUID:{1}, TimeStamp:{2}, GameState:{3}, Day:{4}, MaxPlayer:{5}, PlayerIndex:{6}, PlayerCharacter:{7}, Role:{8}, Dead:{9}, ChatHistoryData:{10}, GameLogData:{11}",
												Prefix, InsUID.ToString(), TimeStamp.ToString(), GameState.ToString(), Day.ToString(), MaxPlayer.ToString(), PlayerIndex.ToString(), PlayerCharacter.ToString(), Role.ToString(), Dead.ToString(), ChatHistoryData.ToString(), GameLogData.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				InsUID = binReader.ReadUInt64();

				TimeStamp = binReader.ReadUInt32();

				GameState = binReader.ReadGameStateID();

				Day = binReader.ReadByte();

				MaxPlayer = binReader.ReadByte();

				PlayerIndex = binReader.ReadByte();

				PlayerCharacter = binReader.ReadByte();

				Role = binReader.ReadPlayerRole();

				Dead = binReader.ReadByte();

				UInt16 uiSizeOfChatHistoryData = 0;
				uiSizeOfChatHistoryData = binReader.ReadUInt16();
				ChatHistoryData = new System.Byte[uiSizeOfChatHistoryData];
				binReader.ReadBytes(ChatHistoryData);

				UInt16 uiSizeOfGameLogData = 0;
				uiSizeOfGameLogData = binReader.ReadUInt16();
				GameLogData = new System.Byte[uiSizeOfGameLogData];
				binReader.ReadBytes(GameLogData);

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , InsUID, TimeStamp, GameState, Day, MaxPlayer, PlayerIndex, PlayerCharacter, Role, Dead, ChatHistoryData, GameLogData );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InInsUID, System.UInt32 InTimeStamp, BR.GameStateID InGameState, System.Byte InDay, System.Byte InMaxPlayer, System.Byte InPlayerIndex, System.Byte InPlayerCharacter, BR.PlayerRole InRole, System.Byte InDead, System.Byte[] InChatHistoryData, System.Byte[] InGameLogData )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 8
					+ 4
					+ 1
					+ 1
					+ 1
					+ 1
					+ 1
					+ 1
					+ 1
					+ 1*(InChatHistoryData.Length) + sizeof(UInt16)
					+ 1*(InGameLogData.Length) + sizeof(UInt16);

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InInsUID);
				binWriter.Write( InTimeStamp);
				binWriter.Write( InGameState);
				binWriter.Write( InDay);
				binWriter.Write( InMaxPlayer);
				binWriter.Write( InPlayerIndex);
				binWriter.Write( InPlayerCharacter);
				binWriter.Write( InRole);
				binWriter.Write( InDead);
				binWriter.Write( InChatHistoryData);
				binWriter.Write( InGameLogData);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InInsUID, System.UInt32 InTimeStamp, BR.GameStateID InGameState, System.Byte InDay, System.Byte InMaxPlayer, System.Byte InPlayerIndex, System.Byte InPlayerCharacter, BR.PlayerRole InRole, System.Byte InDead, System.Byte[] InChatHistoryData, System.Byte[] InGameLogData )

		} // public class GameMatchedS2CEvt : MessageBase

		// S2C: Game match failed
		[Preserve (AllMembers = true)]
		public class GameMatchFailedS2CEvt : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.S2CEVT, true, true, POLICY.GAME, 64);

			public System.Int32 FailedReason;
			public GameMatchFailedS2CEvt()
				{}

			public GameMatchFailedS2CEvt( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:GameMatchFailedS2CEvt: , FailedReason:{1}",
												Prefix, FailedReason.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				FailedReason = binReader.ReadInt32();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , FailedReason );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InFailedReason )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 4;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InFailedReason);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InFailedReason )

		} // public class GameMatchFailedS2CEvt : MessageBase

		// S2C: Game matching started
		[Preserve (AllMembers = true)]
		public class GameMatchingStartedS2CEvt : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.S2CEVT, true, true, POLICY.GAME, 65);

			public GameMatchingStartedS2CEvt()
				{}

			public GameMatchingStartedS2CEvt( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:GameMatchingStartedS2CEvt: ",
												Prefix); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header   );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header   )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 ;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header   )

		} // public class GameMatchingStartedS2CEvt : MessageBase

		// Cmd: Cancel Game match
		[Preserve (AllMembers = true)]
		public class CancelGameMatchCmd : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.CMD, true, true, POLICY.GAME, 66);

			public CancelGameMatchCmd()
				{}

			public CancelGameMatchCmd( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:CancelGameMatchCmd: ",
												Prefix); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header   );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header   )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 ;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header   )

		} // public class CancelGameMatchCmd : MessageBase
		[Preserve (AllMembers = true)]
		public class CancelGameMatchRes : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.RES, true, true, POLICY.GAME, 66);

			public System.Int32 Result;
			public CancelGameMatchRes()
				{}

			public CancelGameMatchRes( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:CancelGameMatchRes: , Result:{1}",
												Prefix, Result.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				Result = binReader.ReadInt32();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , Result );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 4;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InResult);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult )

		} // public class CancelGameMatchRes : MessageBase

		// S2C: game matching canceled
		[Preserve (AllMembers = true)]
		public class GameMatchingCanceledS2CEvt : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.S2CEVT, true, true, POLICY.GAME, 67);

			public GameMatchingCanceledS2CEvt()
				{}

			public GameMatchingCanceledS2CEvt( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:GameMatchingCanceledS2CEvt: ",
												Prefix); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header   );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header   )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 ;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header   )

		} // public class GameMatchingCanceledS2CEvt : MessageBase

		// Cmd: Buy shop item
		[Preserve (AllMembers = true)]
		public class BuyShopItemCmd : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.CMD, true, true, POLICY.GAME, 68);

			public System.UInt32 ShopItemID;
			public string ParamString { get; private set; }
			public BuyShopItemCmd()
				{}

			public BuyShopItemCmd( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:BuyShopItemCmd: , ShopItemID:{1}, ParamString:{2}",
												Prefix, ShopItemID.ToString(), ParamString); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				ShopItemID = binReader.ReadUInt32();

				ParamString = binReader.ReadString();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , ShopItemID, ParamString );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt32 InShopItemID, string InParamString )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 4
					+ binWriter.StringEncoder.GetByteCount(InParamString) + 1 + 2;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InShopItemID);
				binWriter.Write( InParamString);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt32 InShopItemID, string InParamString )

		} // public class BuyShopItemCmd : MessageBase
		[Preserve (AllMembers = true)]
		public class BuyShopItemRes : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.RES, true, true, POLICY.GAME, 68);

			public System.Int32 Result;
			public System.UInt32 ShopItemID;
			public BuyShopItemRes()
				{}

			public BuyShopItemRes( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:BuyShopItemRes: , Result:{1}, ShopItemID:{2}",
												Prefix, Result.ToString(), ShopItemID.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				Result = binReader.ReadInt32();

				ShopItemID = binReader.ReadUInt32();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , Result, ShopItemID );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult, System.UInt32 InShopItemID )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 4
					+ 4;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InResult);
				binWriter.Write( InShopItemID);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult, System.UInt32 InShopItemID )

		} // public class BuyShopItemRes : MessageBase

		// Cmd: Give my stamina to other player
		[Preserve (AllMembers = true)]
		public class GiveStaminaCmd : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.CMD, true, true, POLICY.GAME, 69);

			public System.UInt64 TargetPlayer;
			public GiveStaminaCmd()
				{}

			public GiveStaminaCmd( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:GiveStaminaCmd: , TargetPlayer:{1}",
												Prefix, TargetPlayer.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				TargetPlayer = binReader.ReadUInt64();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , TargetPlayer );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InTargetPlayer )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 8;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InTargetPlayer);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt64 InTargetPlayer )

		} // public class GiveStaminaCmd : MessageBase
		[Preserve (AllMembers = true)]
		public class GiveStaminaRes : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.RES, true, true, POLICY.GAME, 69);

			public System.Int32 Result;
			public System.UInt64 TargetPlayer;
			public System.UInt64 TimeStamp;
			public GiveStaminaRes()
				{}

			public GiveStaminaRes( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:GiveStaminaRes: , Result:{1}, TargetPlayer:{2}, TimeStamp:{3}",
												Prefix, Result.ToString(), TargetPlayer.ToString(), TimeStamp.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				Result = binReader.ReadInt32();

				TargetPlayer = binReader.ReadUInt64();

				TimeStamp = binReader.ReadUInt64();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , Result, TargetPlayer, TimeStamp );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult, System.UInt64 InTargetPlayer, System.UInt64 InTimeStamp )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 4
					+ 8
					+ 8;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InResult);
				binWriter.Write( InTargetPlayer);
				binWriter.Write( InTimeStamp);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult, System.UInt64 InTargetPlayer, System.UInt64 InTimeStamp )

		} // public class GiveStaminaRes : MessageBase

		// Cmd: For debug, Change configue preset
		[Preserve (AllMembers = true)]
		public class SetPresetGameConfigIDCmd : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.CMD, true, true, POLICY.GAME, 70);

			public System.UInt32 PresetID;
			public SetPresetGameConfigIDCmd()
				{}

			public SetPresetGameConfigIDCmd( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:SetPresetGameConfigIDCmd: , PresetID:{1}",
												Prefix, PresetID.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				PresetID = binReader.ReadUInt32();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , PresetID );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt32 InPresetID )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 4;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InPresetID);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.UInt32 InPresetID )

		} // public class SetPresetGameConfigIDCmd : MessageBase
		[Preserve (AllMembers = true)]
		public class SetPresetGameConfigIDRes : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.RES, true, true, POLICY.GAME, 70);

			public System.Int32 Result;
			public SetPresetGameConfigIDRes()
				{}

			public SetPresetGameConfigIDRes( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:SetPresetGameConfigIDRes: , Result:{1}",
												Prefix, Result.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				Result = binReader.ReadInt32();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , Result );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 4;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InResult);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult )

		} // public class SetPresetGameConfigIDRes : MessageBase

		// Cmd: For Debug
		[Preserve (AllMembers = true)]
		public class GainGameResourceCmd : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.CMD, true, true, POLICY.GAME, 71);

			public System.Int32 Resource;
			public System.Int32 Value;
			public GainGameResourceCmd()
				{}

			public GainGameResourceCmd( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:GainGameResourceCmd: , Resource:{1}, Value:{2}",
												Prefix, Resource.ToString(), Value.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				Resource = binReader.ReadInt32();

				Value = binReader.ReadInt32();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , Resource, Value );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResource, System.Int32 InValue )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 4
					+ 4;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InResource);
				binWriter.Write( InValue);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResource, System.Int32 InValue )

		} // public class GainGameResourceCmd : MessageBase
		[Preserve (AllMembers = true)]
		public class GainGameResourceRes : MessageBase
		{
 
			public static MessageID MID = new MessageID(MSGTYPE.RES, true, true, POLICY.GAME, 71);

			public System.Int32 Result;
			public GainGameResourceRes()
				{}

			public GainGameResourceRes( ref MessageHeader msgHeader )
				:base(ref msgHeader)
				{}


			public override void TraceOut(string Prefix)
			{
 				Debug.Print("{{0}}:GainGameResourceRes: , Result:{1}",
												Prefix, Result.ToString()); 
			} // public override void TraceOut(string Prefix)


			public override void ParseMsg(BinaryMemoryReader binaryReader)
			{
 				var binReader = (PacketMemoryReader)binaryReader;
				Result = binReader.ReadInt32();

			} // public override void ParseMsg(BinaryMemoryReader binaryReader)


			public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)
			{
 				BuildMessageBuffer( binWriter, ref Header , Result );
			} // public override void BuildMessageBuffer(BinaryMemoryWriter binWriter)

			static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult )
			{
 				var binWriter = (PacketMemoryWriter)binaryWriter;
				int __uiMessageSize = sizeof(UInt64)*2 
					+ 4;

				BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);

				header.Length = (uint)__uiMessageSize;
				binWriter.Write( InResult);

			} // static public void  BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header , System.Int32 InResult )

		} // public class GainGameResourceRes : MessageBase





	} // namespace Game
} // namespace BR.Message


