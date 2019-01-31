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
using BR.Message.Game;



namespace BR.Policy
{
 

	public class PolicyGame : BR.Net.Policy
	{
 
		public PolicyGame( BR.Net.ConnectionMUDP connection )
			:base(connection)
		{
		}

		// Cmd: Player connected from a login server and moved to game server
		public void  JoinGameServerCmd( System.UInt64 InAccID, System.UInt64 InTicket, System.UInt64 InLoginEntityUID )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.JoinGameServerCmd.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.JoinGameServerCmd.BuildMessageBuffer(binWriter, ref header, InAccID, InTicket, InLoginEntityUID );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  JoinGameServerCmd( System.UInt64 InAccID, System.UInt64 InTicket, System.UInt64 InLoginEntityUID )

		// Cmd: Register Google notification service ID, after this, the player will get notification from google. Only one notification ID can be active at a time
		public void  RegisterGCMCmd( string InGCMRegisteredID )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.RegisterGCMCmd.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.RegisterGCMCmd.BuildMessageBuffer(binWriter, ref header, InGCMRegisteredID );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  RegisterGCMCmd( string InGCMRegisteredID )

		// Cmd: Unregister Google notification service ID
		public void  UnregisterGCMCmd( string InGCMRegisteredID )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.UnregisterGCMCmd.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.UnregisterGCMCmd.BuildMessageBuffer(binWriter, ref header, InGCMRegisteredID );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  UnregisterGCMCmd( string InGCMRegisteredID )

		// Cmd: Invite friend
		public void  InviteFriendCmd( System.UInt64 InFriendID )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.InviteFriendCmd.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.InviteFriendCmd.BuildMessageBuffer(binWriter, ref header, InFriendID );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  InviteFriendCmd( System.UInt64 InFriendID )

		// Cmd: Accept friend request
		public void  AcceptFriendRequestCmd( System.UInt64 InInviterID, System.UInt64 InInviterFacebookUID )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.AcceptFriendRequestCmd.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.AcceptFriendRequestCmd.BuildMessageBuffer(binWriter, ref header, InInviterID, InInviterFacebookUID );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  AcceptFriendRequestCmd( System.UInt64 InInviterID, System.UInt64 InInviterFacebookUID )

		// Cmd: Remove friden form the friend list
		public void  RemoveFriendCmd( System.UInt64 InFriendID )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.RemoveFriendCmd.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.RemoveFriendCmd.BuildMessageBuffer(binWriter, ref header, InFriendID );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  RemoveFriendCmd( System.UInt64 InFriendID )

		// Cmd: Get friend list
		public void  GetFriendListCmd(  )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.GetFriendListCmd.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.GetFriendListCmd.BuildMessageBuffer(binWriter, ref header );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  GetFriendListCmd(  )

		// Cmd: Query notification list
		public void  GetNotificationListCmd(  )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.GetNotificationListCmd.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.GetNotificationListCmd.BuildMessageBuffer(binWriter, ref header );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  GetNotificationListCmd(  )

		// Cmd: Delete notification
		public void  DeleteNotificationCmd( System.UInt32 InNotificationID )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.DeleteNotificationCmd.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.DeleteNotificationCmd.BuildMessageBuffer(binWriter, ref header, InNotificationID );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  DeleteNotificationCmd( System.UInt32 InNotificationID )

		// Cmd: Set notification is read
		public void  SetNotificationReadCmd( System.UInt32 InNotificationID )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.SetNotificationReadCmd.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.SetNotificationReadCmd.BuildMessageBuffer(binWriter, ref header, InNotificationID );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  SetNotificationReadCmd( System.UInt32 InNotificationID )

		// Cmd: Query playerID list
		public void  FindPlayerByEMailCmd( string InPlayerEMail )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.FindPlayerByEMailCmd.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.FindPlayerByEMailCmd.BuildMessageBuffer(binWriter, ref header, InPlayerEMail );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  FindPlayerByEMailCmd( string InPlayerEMail )

		// Cmd: *Request Player Status Update
		public void  RequestPlayerStatusUpdateCmd( System.UInt64[] InTargetPlayerID )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.RequestPlayerStatusUpdateCmd.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.RequestPlayerStatusUpdateCmd.BuildMessageBuffer(binWriter, ref header, InTargetPlayerID );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  RequestPlayerStatusUpdateCmd( System.UInt64[] InTargetPlayerID )

		// Cmd: Get Ranking lise
		public void  GetRankingListCmd( BR.RankingType InRankingType, System.Byte InBaseRanking, System.Byte InCount )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.GetRankingListCmd.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.GetRankingListCmd.BuildMessageBuffer(binWriter, ref header, InRankingType, InBaseRanking, InCount );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  GetRankingListCmd( BR.RankingType InRankingType, System.Byte InBaseRanking, System.Byte InCount )

		// Cmd: Game user game play information
		public void  GetUserGamePlayerInfoCmd(  )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.GetUserGamePlayerInfoCmd.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.GetUserGamePlayerInfoCmd.BuildMessageBuffer(binWriter, ref header );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  GetUserGamePlayerInfoCmd(  )

		// Cmd: Game game play information
		public void  GetGamePlayerInfoCmd( System.UInt64 InPlayerID )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.GetGamePlayerInfoCmd.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.GetGamePlayerInfoCmd.BuildMessageBuffer(binWriter, ref header, InPlayerID );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  GetGamePlayerInfoCmd( System.UInt64 InPlayerID )

		// Cmd: Change NickName
		public void  SetNickNameCmd( string InNickName )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.SetNickNameCmd.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.SetNickNameCmd.BuildMessageBuffer(binWriter, ref header, InNickName );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  SetNickNameCmd( string InNickName )

		// Cmd: Create Party
		public void  CreatePartyCmd(  )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.CreatePartyCmd.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.CreatePartyCmd.BuildMessageBuffer(binWriter, ref header );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  CreatePartyCmd(  )

		// Cmd: Join party
		public void  JoinPartyCmd( System.UInt64 InPartyUID, System.UInt64 InInviterID )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.JoinPartyCmd.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.JoinPartyCmd.BuildMessageBuffer(binWriter, ref header, InPartyUID, InInviterID );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  JoinPartyCmd( System.UInt64 InPartyUID, System.UInt64 InInviterID )

		// Cmd: Leave party command
		public void  LeavePartyCmd( System.UInt64 InPartyUID, System.UInt64 InPlayerID )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.LeavePartyCmd.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.LeavePartyCmd.BuildMessageBuffer(binWriter, ref header, InPartyUID, InPlayerID );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  LeavePartyCmd( System.UInt64 InPartyUID, System.UInt64 InPlayerID )

		// Cmd: Kick player from the party
		public void  PartyKickPlayerCmd( System.UInt64 InPartyUID, System.UInt64 InPlayerID, System.UInt64 InPlayerToKick )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.PartyKickPlayerCmd.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.PartyKickPlayerCmd.BuildMessageBuffer(binWriter, ref header, InPartyUID, InPlayerID, InPlayerToKick );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  PartyKickPlayerCmd( System.UInt64 InPartyUID, System.UInt64 InPlayerID, System.UInt64 InPlayerToKick )

		// Cmd: Invite a player to the party
		public void  PartyInviteCmd( System.UInt64 InInviteTargetID )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.PartyInviteCmd.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.PartyInviteCmd.BuildMessageBuffer(binWriter, ref header, InInviteTargetID );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  PartyInviteCmd( System.UInt64 InInviteTargetID )

		// Cmd: Send Party quick chat message
		public void  PartyQuickChatMessageCmd( System.UInt32 InQuickChatID )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.PartyQuickChatMessageCmd.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.PartyQuickChatMessageCmd.BuildMessageBuffer(binWriter, ref header, InQuickChatID );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  PartyQuickChatMessageCmd( System.UInt32 InQuickChatID )

		// Cmd: Party chatting
		public void  PartyChatMessageCmd( string InChatMessage )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.PartyChatMessageCmd.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.PartyChatMessageCmd.BuildMessageBuffer(binWriter, ref header, InChatMessage );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  PartyChatMessageCmd( string InChatMessage )

		// Cmd: Join to a game
		public void  JoinGameCmd( System.UInt64 InPlayerID, System.UInt64 InTicket, System.UInt64 InInsUID )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.JoinGameCmd.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.JoinGameCmd.BuildMessageBuffer(binWriter, ref header, InPlayerID, InTicket, InInsUID );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  JoinGameCmd( System.UInt64 InPlayerID, System.UInt64 InTicket, System.UInt64 InInsUID )

		// Cmd: Leave Game
		public void  LeaveGameCmd( System.UInt64 InGameInsUID, System.UInt64 InPlayerID, System.UInt64 InTicket )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.LeaveGameCmd.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.LeaveGameCmd.BuildMessageBuffer(binWriter, ref header, InGameInsUID, InPlayerID, InTicket );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  LeaveGameCmd( System.UInt64 InGameInsUID, System.UInt64 InPlayerID, System.UInt64 InTicket )

		// Cmd: Kick player
		public void  KickPlayerCmd( System.UInt64 InGameInsUID, System.UInt64 InPlayerID, System.UInt64 InPlayerToKick )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.KickPlayerCmd.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.KickPlayerCmd.BuildMessageBuffer(binWriter, ref header, InGameInsUID, InPlayerID, InPlayerToKick );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  KickPlayerCmd( System.UInt64 InGameInsUID, System.UInt64 InPlayerID, System.UInt64 InPlayerToKick )

		// Cmd: Assign role + Game state reset
		public void  AssignRoleCmd( System.UInt64 InGameInsUID, System.UInt64 InPlayerID, System.UInt64 InTicket )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.AssignRoleCmd.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.AssignRoleCmd.BuildMessageBuffer(binWriter, ref header, InGameInsUID, InPlayerID, InTicket );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  AssignRoleCmd( System.UInt64 InGameInsUID, System.UInt64 InPlayerID, System.UInt64 InTicket )

		// Cmd: Send chatting message to the game
		public void  ChatMessageCmd( string InChatMessage, BR.PlayerRole InRole )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.ChatMessageCmd.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.ChatMessageCmd.BuildMessageBuffer(binWriter, ref header, InChatMessage, InRole );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  ChatMessageCmd( string InChatMessage, BR.PlayerRole InRole )

		// Cmd: Advance game
		public void  AdvanceGameCmd( System.UInt64 InGameInsUID, System.UInt64 InPlayerID, System.UInt64 InTicket )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.AdvanceGameCmd.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.AdvanceGameCmd.BuildMessageBuffer(binWriter, ref header, InGameInsUID, InPlayerID, InTicket );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  AdvanceGameCmd( System.UInt64 InGameInsUID, System.UInt64 InPlayerID, System.UInt64 InTicket )

		// Cmd: Vote game advance
		public void  VoteGameAdvanceCmd( System.UInt64 InGameInsUID, System.UInt64 InPlayerID, System.UInt64 InTicket )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.VoteGameAdvanceCmd.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.VoteGameAdvanceCmd.BuildMessageBuffer(binWriter, ref header, InGameInsUID, InPlayerID, InTicket );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  VoteGameAdvanceCmd( System.UInt64 InGameInsUID, System.UInt64 InPlayerID, System.UInt64 InTicket )

		// Cmd: Vote
		public void  VoteCmd( System.UInt64 InGameInsUID, System.UInt64 InPlayerID, System.UInt64 InTicket, System.UInt64 InVoteTarget, System.UInt32 InActionSerial )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.VoteCmd.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.VoteCmd.BuildMessageBuffer(binWriter, ref header, InGameInsUID, InPlayerID, InTicket, InVoteTarget, InActionSerial );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  VoteCmd( System.UInt64 InGameInsUID, System.UInt64 InPlayerID, System.UInt64 InTicket, System.UInt64 InVoteTarget, System.UInt32 InActionSerial )

		// Cmd: Request Game match
		public void  RequestGameMatchCmd( System.Byte InNumPlayer )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.RequestGameMatchCmd.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.RequestGameMatchCmd.BuildMessageBuffer(binWriter, ref header, InNumPlayer );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  RequestGameMatchCmd( System.Byte InNumPlayer )

		// Cmd: Cancel Game match
		public void  CancelGameMatchCmd(  )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.CancelGameMatchCmd.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.CancelGameMatchCmd.BuildMessageBuffer(binWriter, ref header );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  CancelGameMatchCmd(  )

		// Cmd: Buy shop item
		public void  BuyShopItemCmd( System.UInt32 InShopItemID, string InParamString )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.BuyShopItemCmd.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.BuyShopItemCmd.BuildMessageBuffer(binWriter, ref header, InShopItemID, InParamString );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  BuyShopItemCmd( System.UInt32 InShopItemID, string InParamString )

		// Cmd: Give my stamina to other player
		public void  GiveStaminaCmd( System.UInt64 InTargetPlayer )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.GiveStaminaCmd.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.GiveStaminaCmd.BuildMessageBuffer(binWriter, ref header, InTargetPlayer );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  GiveStaminaCmd( System.UInt64 InTargetPlayer )

		// Cmd: For debug, Change configue preset
		public void  SetPresetGameConfigIDCmd( System.UInt32 InPresetID )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.SetPresetGameConfigIDCmd.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.SetPresetGameConfigIDCmd.BuildMessageBuffer(binWriter, ref header, InPresetID );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  SetPresetGameConfigIDCmd( System.UInt32 InPresetID )

		// Cmd: For Debug
		public void  GainGameResourceCmd( System.Int32 InResource, System.Int32 InValue )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.GainGameResourceCmd.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.GainGameResourceCmd.BuildMessageBuffer(binWriter, ref header, InResource, InValue );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  GainGameResourceCmd( System.Int32 InResource, System.Int32 InValue )

	}; // public class PolicyGame : BR.Net.Policy
	public class SvrPolicyGame : BR.Net.Policy
	{
 
		public SvrPolicyGame( BR.Net.ConnectionMUDP connection )
			:base(connection)
		{
		}

		// Cmd: Player connected from a login server and moved to game server
		public void  JoinGameServerRes( System.Int32 InResult, string InNickName, System.UInt64 InGameUID, System.UInt64 InPartyUID, System.UInt64 InPartyLeaderID, ref BR.MatchingQueueTicket InMatchingTicket )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.JoinGameServerRes.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.JoinGameServerRes.BuildMessageBuffer(binWriter, ref header, InResult, InNickName, InGameUID, InPartyUID, InPartyLeaderID,ref InMatchingTicket );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  JoinGameServerRes( System.Int32 InResult, string InNickName, System.UInt64 InGameUID, System.UInt64 InPartyUID, System.UInt64 InPartyLeaderID, ref BR.MatchingQueueTicket InMatchingTicket )


		// Cmd: Register Google notification service ID, after this, the player will get notification from google. Only one notification ID can be active at a time
		public void  RegisterGCMRes( System.Int32 InResult )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.RegisterGCMRes.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.RegisterGCMRes.BuildMessageBuffer(binWriter, ref header, InResult );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  RegisterGCMRes( System.Int32 InResult )


		// Cmd: Unregister Google notification service ID
		public void  UnregisterGCMRes( System.Int32 InResult )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.UnregisterGCMRes.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.UnregisterGCMRes.BuildMessageBuffer(binWriter, ref header, InResult );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  UnregisterGCMRes( System.Int32 InResult )


		// Cmd: Invite friend
		public void  InviteFriendRes( System.Int32 InResult )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.InviteFriendRes.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.InviteFriendRes.BuildMessageBuffer(binWriter, ref header, InResult );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  InviteFriendRes( System.Int32 InResult )


		// Cmd: Accept friend request
		public void  AcceptFriendRequestRes( System.Int32 InResult, ref BR.FriendInformation InNewFriend )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.AcceptFriendRequestRes.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.AcceptFriendRequestRes.BuildMessageBuffer(binWriter, ref header, InResult,ref InNewFriend );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  AcceptFriendRequestRes( System.Int32 InResult, ref BR.FriendInformation InNewFriend )


		// S2C: Notification for friend request is accepted
		public void  FriendRequestAcceptedS2CEvt( ref BR.FriendInformation InAccepter )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.FriendRequestAcceptedS2CEvt.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.FriendRequestAcceptedS2CEvt.BuildMessageBuffer(binWriter, ref header,ref InAccepter );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  FriendRequestAcceptedS2CEvt( ref BR.FriendInformation InAccepter )


		// Cmd: Remove friden form the friend list
		public void  RemoveFriendRes( System.Int32 InResult, System.UInt64 InFriendID )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.RemoveFriendRes.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.RemoveFriendRes.BuildMessageBuffer(binWriter, ref header, InResult, InFriendID );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  RemoveFriendRes( System.Int32 InResult, System.UInt64 InFriendID )


		// S2C: Friend removed
		public void  FriendRemovedS2CEvt( System.UInt64 InFriendID )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.FriendRemovedS2CEvt.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.FriendRemovedS2CEvt.BuildMessageBuffer(binWriter, ref header, InFriendID );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  FriendRemovedS2CEvt( System.UInt64 InFriendID )


		// Cmd: Get friend list
		public void  GetFriendListRes( System.Int32 InResult, BR.FriendInformation[] InFriendList )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.GetFriendListRes.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.GetFriendListRes.BuildMessageBuffer(binWriter, ref header, InResult, InFriendList );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  GetFriendListRes( System.Int32 InResult, BR.FriendInformation[] InFriendList )


		// Cmd: Query notification list
		public void  GetNotificationListRes( System.Int32 InResult )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.GetNotificationListRes.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.GetNotificationListRes.BuildMessageBuffer(binWriter, ref header, InResult );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  GetNotificationListRes( System.Int32 InResult )


		// Cmd: Delete notification
		public void  DeleteNotificationRes( System.Int32 InResult, System.UInt32 InNotificationID )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.DeleteNotificationRes.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.DeleteNotificationRes.BuildMessageBuffer(binWriter, ref header, InResult, InNotificationID );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  DeleteNotificationRes( System.Int32 InResult, System.UInt32 InNotificationID )


		// Cmd: Set notification is read
		public void  SetNotificationReadRes( System.Int32 InResult, System.UInt32 InNotificationID )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.SetNotificationReadRes.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.SetNotificationReadRes.BuildMessageBuffer(binWriter, ref header, InResult, InNotificationID );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  SetNotificationReadRes( System.Int32 InResult, System.UInt32 InNotificationID )


		// S2C: Notify new notification
		public void  NotifyS2CEvt( System.UInt32 InNotificationID, System.Int16 InMessageID, System.UInt64 InMessageParam0, System.UInt64 InMessageParam1, string InMessageText, System.Byte InIsRead, System.UInt64 InTimeStamp )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.NotifyS2CEvt.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.NotifyS2CEvt.BuildMessageBuffer(binWriter, ref header, InNotificationID, InMessageID, InMessageParam0, InMessageParam1, InMessageText, InIsRead, InTimeStamp );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  NotifyS2CEvt( System.UInt32 InNotificationID, System.Int16 InMessageID, System.UInt64 InMessageParam0, System.UInt64 InMessageParam1, string InMessageText, System.Byte InIsRead, System.UInt64 InTimeStamp )


		// Cmd: Query playerID list
		public void  FindPlayerByEMailRes( System.Int32 InResult, ref BR.PlayerInformation InPlayer )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.FindPlayerByEMailRes.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.FindPlayerByEMailRes.BuildMessageBuffer(binWriter, ref header, InResult,ref InPlayer );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  FindPlayerByEMailRes( System.Int32 InResult, ref BR.PlayerInformation InPlayer )


		// Cmd: *Request Player Status Update
		public void  RequestPlayerStatusUpdateRes( System.Int32 InResult )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.RequestPlayerStatusUpdateRes.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.RequestPlayerStatusUpdateRes.BuildMessageBuffer(binWriter, ref header, InResult );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  RequestPlayerStatusUpdateRes( System.Int32 InResult )


		// S2C: *Notify Player Status Updated
		public void  NotifyPlayerStatusUpdatedS2CEvt( System.UInt64 InPlayerID, System.UInt32 InLatestActiveTime, System.Byte InIsInGame )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.NotifyPlayerStatusUpdatedS2CEvt.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.NotifyPlayerStatusUpdatedS2CEvt.BuildMessageBuffer(binWriter, ref header, InPlayerID, InLatestActiveTime, InIsInGame );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  NotifyPlayerStatusUpdatedS2CEvt( System.UInt64 InPlayerID, System.UInt32 InLatestActiveTime, System.Byte InIsInGame )


		// Cmd: Get Ranking lise
		public void  GetRankingListRes( System.Int32 InResult, BR.TotalRankingPlayerInformation[] InRanking )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.GetRankingListRes.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.GetRankingListRes.BuildMessageBuffer(binWriter, ref header, InResult, InRanking );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  GetRankingListRes( System.Int32 InResult, BR.TotalRankingPlayerInformation[] InRanking )


		// Cmd: Game user game play information
		public void  GetUserGamePlayerInfoRes( System.Int32 InResult, System.Int16 InLevel, System.Int64 InExp, System.Int64 InGameMoney, System.Int64 InGem, System.Int16 InStamina, System.Int32 InTotalPlayed, System.Int32 InWinPlaySC, System.Int32 InWinPlaySM, System.Int32 InLosePlaySC, System.Int32 InLosePlaySM, System.Int32 InWinPlayNC, System.Int32 InWinPlayNM, System.Int32 InLosePlayNC, System.Int32 InLosePlayNM, System.Int32 InWeeklyWin, System.Int32 InWeeklyLose )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.GetUserGamePlayerInfoRes.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.GetUserGamePlayerInfoRes.BuildMessageBuffer(binWriter, ref header, InResult, InLevel, InExp, InGameMoney, InGem, InStamina, InTotalPlayed, InWinPlaySC, InWinPlaySM, InLosePlaySC, InLosePlaySM, InWinPlayNC, InWinPlayNM, InLosePlayNC, InLosePlayNM, InWeeklyWin, InWeeklyLose );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  GetUserGamePlayerInfoRes( System.Int32 InResult, System.Int16 InLevel, System.Int64 InExp, System.Int64 InGameMoney, System.Int64 InGem, System.Int16 InStamina, System.Int32 InTotalPlayed, System.Int32 InWinPlaySC, System.Int32 InWinPlaySM, System.Int32 InLosePlaySC, System.Int32 InLosePlaySM, System.Int32 InWinPlayNC, System.Int32 InWinPlayNM, System.Int32 InLosePlayNC, System.Int32 InLosePlayNM, System.Int32 InWeeklyWin, System.Int32 InWeeklyLose )


		// Cmd: Game game play information
		public void  GetGamePlayerInfoRes( System.Int32 InResult, System.UInt64 InPlayerID, System.Int16 InLevel, System.Int32 InTotalPlayed, System.Int32 InWinPlaySC, System.Int32 InWinPlaySM, System.Int32 InLosePlaySC, System.Int32 InLosePlaySM, System.Int32 InWinPlayNC, System.Int32 InWinPlayNM, System.Int32 InLosePlayNC, System.Int32 InLosePlayNM, System.Int32 InWeeklyWin, System.Int32 InWeeklyLose )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.GetGamePlayerInfoRes.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.GetGamePlayerInfoRes.BuildMessageBuffer(binWriter, ref header, InResult, InPlayerID, InLevel, InTotalPlayed, InWinPlaySC, InWinPlaySM, InLosePlaySC, InLosePlaySM, InWinPlayNC, InWinPlayNM, InLosePlayNC, InLosePlayNM, InWeeklyWin, InWeeklyLose );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  GetGamePlayerInfoRes( System.Int32 InResult, System.UInt64 InPlayerID, System.Int16 InLevel, System.Int32 InTotalPlayed, System.Int32 InWinPlaySC, System.Int32 InWinPlaySM, System.Int32 InLosePlaySC, System.Int32 InLosePlaySM, System.Int32 InWinPlayNC, System.Int32 InWinPlayNM, System.Int32 InLosePlayNC, System.Int32 InLosePlayNM, System.Int32 InWeeklyWin, System.Int32 InWeeklyLose )


		// S2C: Player level up event
		public void  LevelUpS2CEvt( System.UInt64 InCurrentTotalExp, System.UInt32 InCurrentLevel )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.LevelUpS2CEvt.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.LevelUpS2CEvt.BuildMessageBuffer(binWriter, ref header, InCurrentTotalExp, InCurrentLevel );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  LevelUpS2CEvt( System.UInt64 InCurrentTotalExp, System.UInt32 InCurrentLevel )


		// Cmd: Change NickName
		public void  SetNickNameRes( System.Int32 InResult )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.SetNickNameRes.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.SetNickNameRes.BuildMessageBuffer(binWriter, ref header, InResult );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  SetNickNameRes( System.Int32 InResult )


		// Cmd: Create Party
		public void  CreatePartyRes( System.Int32 InResult, System.UInt64 InPartyUID )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.CreatePartyRes.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.CreatePartyRes.BuildMessageBuffer(binWriter, ref header, InResult, InPartyUID );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  CreatePartyRes( System.Int32 InResult, System.UInt64 InPartyUID )


		// Cmd: Join party
		public void  JoinPartyRes( System.Int32 InResult, System.UInt64 InPartyUID, System.UInt64 InPartyLeaderID, System.Byte[] InChatHistoryData )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.JoinPartyRes.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.JoinPartyRes.BuildMessageBuffer(binWriter, ref header, InResult, InPartyUID, InPartyLeaderID, InChatHistoryData );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  JoinPartyRes( System.Int32 InResult, System.UInt64 InPartyUID, System.UInt64 InPartyLeaderID, System.Byte[] InChatHistoryData )


		// S2C: Player Joined event
		public void  PartyPlayerJoinedS2CEvt( System.UInt64 InPartyUID, ref BR.PlayerInformation InJoinedPlayer )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.PartyPlayerJoinedS2CEvt.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.PartyPlayerJoinedS2CEvt.BuildMessageBuffer(binWriter, ref header, InPartyUID,ref InJoinedPlayer );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  PartyPlayerJoinedS2CEvt( System.UInt64 InPartyUID, ref BR.PlayerInformation InJoinedPlayer )


		// S2C: Party leader changed event
		public void  PartyLeaderChangedS2CEvt( System.UInt64 InPartyUID, System.UInt64 InNewLeaderID )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.PartyLeaderChangedS2CEvt.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.PartyLeaderChangedS2CEvt.BuildMessageBuffer(binWriter, ref header, InPartyUID, InNewLeaderID );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  PartyLeaderChangedS2CEvt( System.UInt64 InPartyUID, System.UInt64 InNewLeaderID )


		// Cmd: Leave party command
		public void  LeavePartyRes( System.Int32 InResult )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.LeavePartyRes.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.LeavePartyRes.BuildMessageBuffer(binWriter, ref header, InResult );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  LeavePartyRes( System.Int32 InResult )


		// S2C: Party Player left event
		public void  PartyPlayerLeftS2CEvt( System.UInt64 InPartyUID, System.UInt64 InLeftPlayerID )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.PartyPlayerLeftS2CEvt.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.PartyPlayerLeftS2CEvt.BuildMessageBuffer(binWriter, ref header, InPartyUID, InLeftPlayerID );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  PartyPlayerLeftS2CEvt( System.UInt64 InPartyUID, System.UInt64 InLeftPlayerID )


		// Cmd: Kick player from the party
		public void  PartyKickPlayerRes( System.Int32 InResult )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.PartyKickPlayerRes.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.PartyKickPlayerRes.BuildMessageBuffer(binWriter, ref header, InResult );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  PartyKickPlayerRes( System.Int32 InResult )


		// S2C: Party Player kicked message
		public void  PartyPlayerKickedS2CEvt( System.UInt64 InPartyUID, System.UInt64 InKickedPlayerID )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.PartyPlayerKickedS2CEvt.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.PartyPlayerKickedS2CEvt.BuildMessageBuffer(binWriter, ref header, InPartyUID, InKickedPlayerID );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  PartyPlayerKickedS2CEvt( System.UInt64 InPartyUID, System.UInt64 InKickedPlayerID )


		// Cmd: Invite a player to the party
		public void  PartyInviteRes( System.Int32 InResult )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.PartyInviteRes.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.PartyInviteRes.BuildMessageBuffer(binWriter, ref header, InResult );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  PartyInviteRes( System.Int32 InResult )


		// S2C: Party invite requested
		public void  PartyInviteRequestedS2CEvt( System.UInt64 InInviterID, string InInviterName, System.UInt64 InPartyToJoinUID )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.PartyInviteRequestedS2CEvt.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.PartyInviteRequestedS2CEvt.BuildMessageBuffer(binWriter, ref header, InInviterID, InInviterName, InPartyToJoinUID );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  PartyInviteRequestedS2CEvt( System.UInt64 InInviterID, string InInviterName, System.UInt64 InPartyToJoinUID )


		// Cmd: Send Party quick chat message
		public void  PartyQuickChatMessageRes( System.Int32 InResult )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.PartyQuickChatMessageRes.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.PartyQuickChatMessageRes.BuildMessageBuffer(binWriter, ref header, InResult );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  PartyQuickChatMessageRes( System.Int32 InResult )


		// S2C: Party Chatting message event
		public void  PartyQuickChatMessageS2CEvt( System.UInt64 InSenderID, System.UInt32 InQuickChatID )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.PartyQuickChatMessageS2CEvt.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.PartyQuickChatMessageS2CEvt.BuildMessageBuffer(binWriter, ref header, InSenderID, InQuickChatID );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  PartyQuickChatMessageS2CEvt( System.UInt64 InSenderID, System.UInt32 InQuickChatID )


		// Cmd: Party chatting
		public void  PartyChatMessageRes( System.Int32 InResult )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.PartyChatMessageRes.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.PartyChatMessageRes.BuildMessageBuffer(binWriter, ref header, InResult );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  PartyChatMessageRes( System.Int32 InResult )


		// S2C: Party Chatting message event
		public void  PartyChatMessageS2CEvt( System.UInt64 InSenderID, string InSenderName, string InChatMessage )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.PartyChatMessageS2CEvt.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.PartyChatMessageS2CEvt.BuildMessageBuffer(binWriter, ref header, InSenderID, InSenderName, InChatMessage );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  PartyChatMessageS2CEvt( System.UInt64 InSenderID, string InSenderName, string InChatMessage )


		// Cmd: Join to a game
		public void  JoinGameRes( System.Int32 InResult, System.UInt64 InInsUID, System.UInt32 InTimeStamp, BR.GameStateID InGameState, System.Byte InDay, System.Byte InMaxPlayer, System.Byte InPlayerIndex, System.Byte InPlayerCharacter, BR.PlayerRole InRole, System.Byte InDead, System.Byte[] InChatHistoryData, System.Byte[] InGameLogData )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.JoinGameRes.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.JoinGameRes.BuildMessageBuffer(binWriter, ref header, InResult, InInsUID, InTimeStamp, InGameState, InDay, InMaxPlayer, InPlayerIndex, InPlayerCharacter, InRole, InDead, InChatHistoryData, InGameLogData );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  JoinGameRes( System.Int32 InResult, System.UInt64 InInsUID, System.UInt32 InTimeStamp, BR.GameStateID InGameState, System.Byte InDay, System.Byte InMaxPlayer, System.Byte InPlayerIndex, System.Byte InPlayerCharacter, BR.PlayerRole InRole, System.Byte InDead, System.Byte[] InChatHistoryData, System.Byte[] InGameLogData )


		// S2C: Player Joined in the game
		public void  PlayerJoinedS2CEvt( System.UInt64 InGameInsUID, ref BR.PlayerInformation InJoinedPlayer, BR.PlayerRole InJoinedPlayerRole, System.Byte InJoinedPlayerDead, System.Byte InJoinedPlayerIndex, System.Byte InJoinedPlayerCharacter )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.PlayerJoinedS2CEvt.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.PlayerJoinedS2CEvt.BuildMessageBuffer(binWriter, ref header, InGameInsUID,ref InJoinedPlayer, InJoinedPlayerRole, InJoinedPlayerDead, InJoinedPlayerIndex, InJoinedPlayerCharacter );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  PlayerJoinedS2CEvt( System.UInt64 InGameInsUID, ref BR.PlayerInformation InJoinedPlayer, BR.PlayerRole InJoinedPlayerRole, System.Byte InJoinedPlayerDead, System.Byte InJoinedPlayerIndex, System.Byte InJoinedPlayerCharacter )


		// Cmd: Leave Game
		public void  LeaveGameRes( System.Int32 InResult )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.LeaveGameRes.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.LeaveGameRes.BuildMessageBuffer(binWriter, ref header, InResult );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  LeaveGameRes( System.Int32 InResult )


		// S2C: Player left event
		public void  PlayerLeftS2CEvt( System.UInt64 InGameInsUID, System.UInt64 InLeftPlayerID )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.PlayerLeftS2CEvt.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.PlayerLeftS2CEvt.BuildMessageBuffer(binWriter, ref header, InGameInsUID, InLeftPlayerID );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  PlayerLeftS2CEvt( System.UInt64 InGameInsUID, System.UInt64 InLeftPlayerID )


		// Cmd: Kick player
		public void  KickPlayerRes( System.Int32 InResult )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.KickPlayerRes.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.KickPlayerRes.BuildMessageBuffer(binWriter, ref header, InResult );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  KickPlayerRes( System.Int32 InResult )


		// S2C: Player kicked
		public void  PlayerKickedS2CEvt( System.UInt64 InGameInsUID, System.UInt64 InKickedPlayerID )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.PlayerKickedS2CEvt.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.PlayerKickedS2CEvt.BuildMessageBuffer(binWriter, ref header, InGameInsUID, InKickedPlayerID );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  PlayerKickedS2CEvt( System.UInt64 InGameInsUID, System.UInt64 InKickedPlayerID )


		// Cmd: Assign role + Game state reset
		public void  AssignRoleRes( System.Int32 InResult )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.AssignRoleRes.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.AssignRoleRes.BuildMessageBuffer(binWriter, ref header, InResult );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  AssignRoleRes( System.Int32 InResult )


		// S2C: Role assigned event
		public void  RoleAssignedS2CEvt( System.UInt64 InGameInsUID, System.UInt64 InPlayerID, BR.PlayerRole InRole )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.RoleAssignedS2CEvt.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.RoleAssignedS2CEvt.BuildMessageBuffer(binWriter, ref header, InGameInsUID, InPlayerID, InRole );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  RoleAssignedS2CEvt( System.UInt64 InGameInsUID, System.UInt64 InPlayerID, BR.PlayerRole InRole )


		// Cmd: Send chatting message to the game
		public void  ChatMessageRes( System.Int32 InResult )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.ChatMessageRes.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.ChatMessageRes.BuildMessageBuffer(binWriter, ref header, InResult );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  ChatMessageRes( System.Int32 InResult )


		// S2C: Chatting message event 
		public void  ChatMessageS2CEvt( System.UInt64 InSenderID, BR.PlayerRole InRole, string InSenderName, string InChatMessage )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.ChatMessageS2CEvt.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.ChatMessageS2CEvt.BuildMessageBuffer(binWriter, ref header, InSenderID, InRole, InSenderName, InChatMessage );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  ChatMessageS2CEvt( System.UInt64 InSenderID, BR.PlayerRole InRole, string InSenderName, string InChatMessage )


		// Cmd: Advance game
		public void  AdvanceGameRes( System.Int32 InResult )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.AdvanceGameRes.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.AdvanceGameRes.BuildMessageBuffer(binWriter, ref header, InResult );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  AdvanceGameRes( System.Int32 InResult )


		// S2C: The game state is advanced
		public void  GameAdvancedS2CEvt( System.UInt64 InGameInsUID, System.UInt32 InTimeStamp, BR.GameStateID InGameState, System.Byte InDay )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.GameAdvancedS2CEvt.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.GameAdvancedS2CEvt.BuildMessageBuffer(binWriter, ref header, InGameInsUID, InTimeStamp, InGameState, InDay );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  GameAdvancedS2CEvt( System.UInt64 InGameInsUID, System.UInt32 InTimeStamp, BR.GameStateID InGameState, System.Byte InDay )


		// S2C: Game is ended
		public void  GameEndedS2CEvt( System.UInt64 InGameInsUID, BR.GameWinner InWinner, System.UInt32 InGainedExp, System.UInt32 InGainedGameMoney )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.GameEndedS2CEvt.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.GameEndedS2CEvt.BuildMessageBuffer(binWriter, ref header, InGameInsUID, InWinner, InGainedExp, InGainedGameMoney );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  GameEndedS2CEvt( System.UInt64 InGameInsUID, BR.GameWinner InWinner, System.UInt32 InGainedExp, System.UInt32 InGainedGameMoney )


		// Cmd: Vote game advance
		public void  VoteGameAdvanceRes( System.Int32 InResult )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.VoteGameAdvanceRes.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.VoteGameAdvanceRes.BuildMessageBuffer(binWriter, ref header, InResult );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  VoteGameAdvanceRes( System.Int32 InResult )


		// S2C: GameAdvance is Voted
		public void  GameAdvanceVotedS2CEvt( System.UInt64 InGameInsUID, System.UInt64 InVoter )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.GameAdvanceVotedS2CEvt.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.GameAdvanceVotedS2CEvt.BuildMessageBuffer(binWriter, ref header, InGameInsUID, InVoter );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  GameAdvanceVotedS2CEvt( System.UInt64 InGameInsUID, System.UInt64 InVoter )


		// Cmd: Vote
		public void  VoteRes( System.Int32 InResult )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.VoteRes.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.VoteRes.BuildMessageBuffer(binWriter, ref header, InResult );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  VoteRes( System.Int32 InResult )


		// S2C: Player Voted
		public void  VotedS2CEvt( System.UInt64 InGameInsUID, System.UInt64 InVoter, System.UInt64 InVotedTarget )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.VotedS2CEvt.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.VotedS2CEvt.BuildMessageBuffer(binWriter, ref header, InGameInsUID, InVoter, InVotedTarget );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  VotedS2CEvt( System.UInt64 InGameInsUID, System.UInt64 InVoter, System.UInt64 InVotedTarget )


		// S2C: Vote is ended
		public void  VoteEndS2CEvt( System.UInt64 InGameInsUID, System.UInt64[] InVoted )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.VoteEndS2CEvt.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.VoteEndS2CEvt.BuildMessageBuffer(binWriter, ref header, InGameInsUID, InVoted );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  VoteEndS2CEvt( System.UInt64 InGameInsUID, System.UInt64[] InVoted )


		// S2C: Player Killed
		public void  PlayerKilledS2CEvt( System.UInt64 InGameInsUID, System.UInt64 InKilledPlayer, BR.PlayerKilledReason InReason )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.PlayerKilledS2CEvt.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.PlayerKilledS2CEvt.BuildMessageBuffer(binWriter, ref header, InGameInsUID, InKilledPlayer, InReason );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  PlayerKilledS2CEvt( System.UInt64 InGameInsUID, System.UInt64 InKilledPlayer, BR.PlayerKilledReason InReason )


		// S2C: Player Voted
		public void  PlayerRevealedS2CEvt( System.UInt64 InGameInsUID, System.UInt64 InRevealedPlayerID, BR.PlayerRole InRole, BR.PlayerRevealedReason InReason )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.PlayerRevealedS2CEvt.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.PlayerRevealedS2CEvt.BuildMessageBuffer(binWriter, ref header, InGameInsUID, InRevealedPlayerID, InRole, InReason );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  PlayerRevealedS2CEvt( System.UInt64 InGameInsUID, System.UInt64 InRevealedPlayerID, BR.PlayerRole InRole, BR.PlayerRevealedReason InReason )


		// Cmd: Request Game match
		public void  RequestGameMatchRes( System.Int32 InResult )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.RequestGameMatchRes.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.RequestGameMatchRes.BuildMessageBuffer(binWriter, ref header, InResult );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  RequestGameMatchRes( System.Int32 InResult )


		// S2C: Game matched
		public void  GameMatchedS2CEvt( System.UInt64 InInsUID, System.UInt32 InTimeStamp, BR.GameStateID InGameState, System.Byte InDay, System.Byte InMaxPlayer, System.Byte InPlayerIndex, System.Byte InPlayerCharacter, BR.PlayerRole InRole, System.Byte InDead, System.Byte[] InChatHistoryData, System.Byte[] InGameLogData )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.GameMatchedS2CEvt.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.GameMatchedS2CEvt.BuildMessageBuffer(binWriter, ref header, InInsUID, InTimeStamp, InGameState, InDay, InMaxPlayer, InPlayerIndex, InPlayerCharacter, InRole, InDead, InChatHistoryData, InGameLogData );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  GameMatchedS2CEvt( System.UInt64 InInsUID, System.UInt32 InTimeStamp, BR.GameStateID InGameState, System.Byte InDay, System.Byte InMaxPlayer, System.Byte InPlayerIndex, System.Byte InPlayerCharacter, BR.PlayerRole InRole, System.Byte InDead, System.Byte[] InChatHistoryData, System.Byte[] InGameLogData )


		// S2C: Game match failed
		public void  GameMatchFailedS2CEvt( System.Int32 InFailedReason )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.GameMatchFailedS2CEvt.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.GameMatchFailedS2CEvt.BuildMessageBuffer(binWriter, ref header, InFailedReason );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  GameMatchFailedS2CEvt( System.Int32 InFailedReason )


		// S2C: Game matching started
		public void  GameMatchingStartedS2CEvt(  )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.GameMatchingStartedS2CEvt.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.GameMatchingStartedS2CEvt.BuildMessageBuffer(binWriter, ref header );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  GameMatchingStartedS2CEvt(  )


		// Cmd: Cancel Game match
		public void  CancelGameMatchRes( System.Int32 InResult )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.CancelGameMatchRes.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.CancelGameMatchRes.BuildMessageBuffer(binWriter, ref header, InResult );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  CancelGameMatchRes( System.Int32 InResult )


		// S2C: game matching canceled
		public void  GameMatchingCanceledS2CEvt(  )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.GameMatchingCanceledS2CEvt.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.GameMatchingCanceledS2CEvt.BuildMessageBuffer(binWriter, ref header );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  GameMatchingCanceledS2CEvt(  )


		// Cmd: Buy shop item
		public void  BuyShopItemRes( System.Int32 InResult, System.UInt32 InShopItemID )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.BuyShopItemRes.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.BuyShopItemRes.BuildMessageBuffer(binWriter, ref header, InResult, InShopItemID );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  BuyShopItemRes( System.Int32 InResult, System.UInt32 InShopItemID )


		// Cmd: Give my stamina to other player
		public void  GiveStaminaRes( System.Int32 InResult, System.UInt64 InTargetPlayer, System.UInt64 InTimeStamp )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.GiveStaminaRes.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.GiveStaminaRes.BuildMessageBuffer(binWriter, ref header, InResult, InTargetPlayer, InTimeStamp );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  GiveStaminaRes( System.Int32 InResult, System.UInt64 InTargetPlayer, System.UInt64 InTimeStamp )


		// Cmd: For debug, Change configue preset
		public void  SetPresetGameConfigIDRes( System.Int32 InResult )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.SetPresetGameConfigIDRes.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.SetPresetGameConfigIDRes.BuildMessageBuffer(binWriter, ref header, InResult );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  SetPresetGameConfigIDRes( System.Int32 InResult )


		// Cmd: For Debug
		public void  GainGameResourceRes( System.Int32 InResult )
		{
 			MessageHeader header = new MessageHeader();
			PacketMemoryWriter binWriter = (PacketMemoryWriter)m_Connection.CreateWriter(m_Connection.AllocMessageBuffer());
			header.MsgID = BR.Message.Game.GainGameResourceRes.MID;
			header.PeerID = m_Connection.PeerID;
			uint messageSequence = m_Connection.NewSequence;
			header.Sequence = messageSequence;
			binWriter.Length = MessageHeader.SizeOfHeader;
			binWriter.Seek(MessageHeader.SizeOfHeader);
			BR.Message.Game.GainGameResourceRes.BuildMessageBuffer(binWriter, ref header, InResult );
			m_Connection.PendingReliableSend(messageSequence, ref header, binWriter);

		} // public void  GainGameResourceRes( System.Int32 InResult )


	}; // public class SvrPolicyGame : BR.Net.Policy




} // namespace BR.Policy


