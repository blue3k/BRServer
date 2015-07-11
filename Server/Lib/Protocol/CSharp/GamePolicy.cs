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

		// C2S: Client heart bit
		public bool  HeartBitC2SEvt(  )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.HeartBitC2SEvt.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.HeartBitC2SEvt.BuildMessageBuffer(binWriter, ref header );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  HeartBitC2SEvt(  )


		// Cmd: Player connected from a login server and moved to game server
		public bool  JoinGameServerCmd( System.UInt64 InAccID, System.UInt64 InTicket, System.UInt64 InLoginEntityUID )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.JoinGameServerCmd.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.JoinGameServerCmd.BuildMessageBuffer(binWriter, ref header, InAccID, InTicket, InLoginEntityUID );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  JoinGameServerCmd( System.UInt64 InAccID, System.UInt64 InTicket, System.UInt64 InLoginEntityUID )

		// Cmd: player complition statues
		public bool  GetComplitionStateCmd(  )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.GetComplitionStateCmd.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.GetComplitionStateCmd.BuildMessageBuffer(binWriter, ref header );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  GetComplitionStateCmd(  )

		// Cmd: Player complition state
		public bool  SetComplitionStateCmd( string InComplitionState )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.SetComplitionStateCmd.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.SetComplitionStateCmd.BuildMessageBuffer(binWriter, ref header, InComplitionState );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  SetComplitionStateCmd( string InComplitionState )

		// Cmd: Register Google notification service ID, after this, the player will get notification from google. Only one notification ID can be active at a time
		public bool  RegisterGCMCmd( string InGCMRegisteredID )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.RegisterGCMCmd.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.RegisterGCMCmd.BuildMessageBuffer(binWriter, ref header, InGCMRegisteredID );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  RegisterGCMCmd( string InGCMRegisteredID )

		// Cmd: Unregister Google notification service ID
		public bool  UnregisterGCMCmd( string InGCMRegisteredID )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.UnregisterGCMCmd.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.UnregisterGCMCmd.BuildMessageBuffer(binWriter, ref header, InGCMRegisteredID );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  UnregisterGCMCmd( string InGCMRegisteredID )

		// Cmd: Invite friend
		public bool  InviteFriendCmd( System.UInt64 InFriendID )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.InviteFriendCmd.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.InviteFriendCmd.BuildMessageBuffer(binWriter, ref header, InFriendID );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  InviteFriendCmd( System.UInt64 InFriendID )

		// Cmd: Accept friend request
		public bool  AcceptFriendRequestCmd( System.UInt64 InInviterID, System.UInt64 InInviterFacebookUID )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.AcceptFriendRequestCmd.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.AcceptFriendRequestCmd.BuildMessageBuffer(binWriter, ref header, InInviterID, InInviterFacebookUID );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  AcceptFriendRequestCmd( System.UInt64 InInviterID, System.UInt64 InInviterFacebookUID )

		// Cmd: Remove friden form the friend list
		public bool  RemoveFriendCmd( System.UInt64 InFriendID )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.RemoveFriendCmd.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.RemoveFriendCmd.BuildMessageBuffer(binWriter, ref header, InFriendID );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  RemoveFriendCmd( System.UInt64 InFriendID )

		// Cmd: Get friend list
		public bool  GetFriendListCmd( System.UInt16 InStartIndex, System.UInt16 InCount )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.GetFriendListCmd.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.GetFriendListCmd.BuildMessageBuffer(binWriter, ref header, InStartIndex, InCount );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  GetFriendListCmd( System.UInt16 InStartIndex, System.UInt16 InCount )

		// Cmd: Query notification list
		public bool  GetNotificationListCmd(  )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.GetNotificationListCmd.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.GetNotificationListCmd.BuildMessageBuffer(binWriter, ref header );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  GetNotificationListCmd(  )

		// Cmd: Delete notification
		public bool  DeleteNotificationCmd( System.UInt32 InNotificationID )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.DeleteNotificationCmd.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.DeleteNotificationCmd.BuildMessageBuffer(binWriter, ref header, InNotificationID );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  DeleteNotificationCmd( System.UInt32 InNotificationID )

		// Cmd: Set notification is read
		public bool  SetNotificationReadCmd( System.UInt32 InNotificationID )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.SetNotificationReadCmd.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.SetNotificationReadCmd.BuildMessageBuffer(binWriter, ref header, InNotificationID );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  SetNotificationReadCmd( System.UInt32 InNotificationID )

		// Cmd: Accept notification
		public bool  AcceptNotificationCmd( System.UInt32 InNotificationID )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.AcceptNotificationCmd.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.AcceptNotificationCmd.BuildMessageBuffer(binWriter, ref header, InNotificationID );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  AcceptNotificationCmd( System.UInt32 InNotificationID )

		// Cmd: Query playerID list
		public bool  FindPlayerByEMailCmd( string InPlayerEMail )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.FindPlayerByEMailCmd.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.FindPlayerByEMailCmd.BuildMessageBuffer(binWriter, ref header, InPlayerEMail );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  FindPlayerByEMailCmd( string InPlayerEMail )

		// Cmd: *Request Player Status Update
		public bool  RequestPlayerStatusUpdateCmd( System.UInt64[] InTargetPlayerID )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.RequestPlayerStatusUpdateCmd.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.RequestPlayerStatusUpdateCmd.BuildMessageBuffer(binWriter, ref header, InTargetPlayerID );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  RequestPlayerStatusUpdateCmd( System.UInt64[] InTargetPlayerID )

		// Cmd: Get Ranking lise
		public bool  GetRankingListCmd( BR.RankingType InRankingType, System.Byte InBaseRanking, System.Byte InCount )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.GetRankingListCmd.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.GetRankingListCmd.BuildMessageBuffer(binWriter, ref header, InRankingType, InBaseRanking, InCount );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  GetRankingListCmd( BR.RankingType InRankingType, System.Byte InBaseRanking, System.Byte InCount )

		// Cmd: Game user game play information
		public bool  GetUserGamePlayerInfoCmd(  )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.GetUserGamePlayerInfoCmd.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.GetUserGamePlayerInfoCmd.BuildMessageBuffer(binWriter, ref header );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  GetUserGamePlayerInfoCmd(  )

		// Cmd: Game game play information
		public bool  GetGamePlayerInfoCmd( System.UInt64 InPlayerID )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.GetGamePlayerInfoCmd.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.GetGamePlayerInfoCmd.BuildMessageBuffer(binWriter, ref header, InPlayerID );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  GetGamePlayerInfoCmd( System.UInt64 InPlayerID )

		// Cmd: Change NickName
		public bool  SetNickNameCmd( string InNickName, System.Byte InIsCostFree )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.SetNickNameCmd.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.SetNickNameCmd.BuildMessageBuffer(binWriter, ref header, InNickName, InIsCostFree );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  SetNickNameCmd( string InNickName, System.Byte InIsCostFree )

		// Cmd: Create Party
		public bool  CreatePartyCmd(  )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.CreatePartyCmd.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.CreatePartyCmd.BuildMessageBuffer(binWriter, ref header );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  CreatePartyCmd(  )

		// Cmd: Join party
		public bool  JoinPartyCmd( System.UInt64 InPartyUID, System.UInt64 InInviterID )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.JoinPartyCmd.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.JoinPartyCmd.BuildMessageBuffer(binWriter, ref header, InPartyUID, InInviterID );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  JoinPartyCmd( System.UInt64 InPartyUID, System.UInt64 InInviterID )

		// Cmd: Leave party command
		public bool  LeavePartyCmd( System.UInt64 InPartyUID, System.UInt64 InPlayerID )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.LeavePartyCmd.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.LeavePartyCmd.BuildMessageBuffer(binWriter, ref header, InPartyUID, InPlayerID );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  LeavePartyCmd( System.UInt64 InPartyUID, System.UInt64 InPlayerID )

		// Cmd: Kick player from the party
		public bool  PartyKickPlayerCmd( System.UInt64 InPartyUID, System.UInt64 InPlayerID, System.UInt64 InPlayerToKick )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.PartyKickPlayerCmd.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.PartyKickPlayerCmd.BuildMessageBuffer(binWriter, ref header, InPartyUID, InPlayerID, InPlayerToKick );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  PartyKickPlayerCmd( System.UInt64 InPartyUID, System.UInt64 InPlayerID, System.UInt64 InPlayerToKick )

		// Cmd: Invite a player to the party
		public bool  PartyInviteCmd( System.UInt64 InInviteTargetID )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.PartyInviteCmd.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.PartyInviteCmd.BuildMessageBuffer(binWriter, ref header, InInviteTargetID );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  PartyInviteCmd( System.UInt64 InInviteTargetID )

		// Cmd: Send Party quick chat message
		public bool  PartyQuickChatMessageCmd( System.UInt32 InQuickChatID )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.PartyQuickChatMessageCmd.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.PartyQuickChatMessageCmd.BuildMessageBuffer(binWriter, ref header, InQuickChatID );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  PartyQuickChatMessageCmd( System.UInt32 InQuickChatID )

		// Cmd: Party chatting
		public bool  PartyChatMessageCmd( string InChatMessage )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.PartyChatMessageCmd.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.PartyChatMessageCmd.BuildMessageBuffer(binWriter, ref header, InChatMessage );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  PartyChatMessageCmd( string InChatMessage )

		// Cmd: Join to a game
		public bool  JoinGameCmd( System.UInt64 InPlayerID, System.UInt64 InTicket, System.UInt64 InInsUID )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.JoinGameCmd.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.JoinGameCmd.BuildMessageBuffer(binWriter, ref header, InPlayerID, InTicket, InInsUID );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  JoinGameCmd( System.UInt64 InPlayerID, System.UInt64 InTicket, System.UInt64 InInsUID )

		// Cmd: Leave Game
		public bool  LeaveGameCmd( System.UInt64 InGameInsUID, System.UInt64 InPlayerID, System.UInt64 InTicket )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.LeaveGameCmd.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.LeaveGameCmd.BuildMessageBuffer(binWriter, ref header, InGameInsUID, InPlayerID, InTicket );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  LeaveGameCmd( System.UInt64 InGameInsUID, System.UInt64 InPlayerID, System.UInt64 InTicket )

		// Cmd: Kick player
		public bool  KickPlayerCmd( System.UInt64 InGameInsUID, System.UInt64 InPlayerID, System.UInt64 InPlayerToKick )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.KickPlayerCmd.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.KickPlayerCmd.BuildMessageBuffer(binWriter, ref header, InGameInsUID, InPlayerID, InPlayerToKick );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  KickPlayerCmd( System.UInt64 InGameInsUID, System.UInt64 InPlayerID, System.UInt64 InPlayerToKick )

		// Cmd: Assign role + Game state reset
		public bool  AssignRoleCmd( System.UInt64 InGameInsUID, System.UInt64 InPlayerID, System.UInt64 InTicket )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.AssignRoleCmd.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.AssignRoleCmd.BuildMessageBuffer(binWriter, ref header, InGameInsUID, InPlayerID, InTicket );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  AssignRoleCmd( System.UInt64 InGameInsUID, System.UInt64 InPlayerID, System.UInt64 InTicket )

		// Cmd: Send chatting message to the game
		public bool  ChatMessageCmd( string InChatMessage, BR.PlayerRole InRole )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.ChatMessageCmd.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.ChatMessageCmd.BuildMessageBuffer(binWriter, ref header, InChatMessage, InRole );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  ChatMessageCmd( string InChatMessage, BR.PlayerRole InRole )

		// Cmd: Advance game
		public bool  AdvanceGameCmd( System.UInt64 InGameInsUID, System.UInt64 InPlayerID, System.UInt64 InTicket )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.AdvanceGameCmd.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.AdvanceGameCmd.BuildMessageBuffer(binWriter, ref header, InGameInsUID, InPlayerID, InTicket );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  AdvanceGameCmd( System.UInt64 InGameInsUID, System.UInt64 InPlayerID, System.UInt64 InTicket )

		// Cmd: Vote game advance
		public bool  VoteGameAdvanceCmd( System.UInt64 InGameInsUID, System.UInt64 InPlayerID, System.UInt64 InTicket )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.VoteGameAdvanceCmd.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.VoteGameAdvanceCmd.BuildMessageBuffer(binWriter, ref header, InGameInsUID, InPlayerID, InTicket );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  VoteGameAdvanceCmd( System.UInt64 InGameInsUID, System.UInt64 InPlayerID, System.UInt64 InTicket )

		// Cmd: Vote
		public bool  VoteCmd( System.UInt64 InGameInsUID, System.UInt64 InPlayerID, System.UInt64 InTicket, System.UInt64 InVoteTarget, System.UInt32 InActionSerial )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.VoteCmd.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.VoteCmd.BuildMessageBuffer(binWriter, ref header, InGameInsUID, InPlayerID, InTicket, InVoteTarget, InActionSerial );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  VoteCmd( System.UInt64 InGameInsUID, System.UInt64 InPlayerID, System.UInt64 InTicket, System.UInt64 InVoteTarget, System.UInt32 InActionSerial )

		// Cmd: Play again with the current players
		public bool  GamePlayAgainCmd(  )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.GamePlayAgainCmd.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.GamePlayAgainCmd.BuildMessageBuffer(binWriter, ref header );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  GamePlayAgainCmd(  )

		// Cmd: Player. reveal a player
		public bool  GameRevealPlayerCmd( System.UInt64[] InTargetPlayerID )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.GameRevealPlayerCmd.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.GameRevealPlayerCmd.BuildMessageBuffer(binWriter, ref header, InTargetPlayerID );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  GameRevealPlayerCmd( System.UInt64[] InTargetPlayerID )

		// Cmd: Player. revive himself
		public bool  GamePlayerReviveCmd(  )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.GamePlayerReviveCmd.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.GamePlayerReviveCmd.BuildMessageBuffer(binWriter, ref header );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  GamePlayerReviveCmd(  )

		// Cmd: Player. reset ranking
		public bool  GamePlayerResetRankCmd(  )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.GamePlayerResetRankCmd.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.GamePlayerResetRankCmd.BuildMessageBuffer(binWriter, ref header );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  GamePlayerResetRankCmd(  )

		// Cmd: Request Game match
		public bool  RequestGameMatchCmd( System.Byte InNumPlayer, BR.PlayerRole InRequestRole )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.RequestGameMatchCmd.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.RequestGameMatchCmd.BuildMessageBuffer(binWriter, ref header, InNumPlayer, InRequestRole );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  RequestGameMatchCmd( System.Byte InNumPlayer, BR.PlayerRole InRequestRole )

		// Cmd: Cancel Game match
		public bool  CancelGameMatchCmd(  )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.CancelGameMatchCmd.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.CancelGameMatchCmd.BuildMessageBuffer(binWriter, ref header );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  CancelGameMatchCmd(  )

		// Cmd: Buy shop item prepare
		public bool  BuyShopItemPrepareCmd( System.UInt32 InShopItemID )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.BuyShopItemPrepareCmd.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.BuyShopItemPrepareCmd.BuildMessageBuffer(binWriter, ref header, InShopItemID );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  BuyShopItemPrepareCmd( System.UInt32 InShopItemID )

		// Cmd: Buy shop item
		public bool  BuyShopItemCmd( System.UInt32 InShopItemID, string InPlatform, string InPackageName, string InPurchaseTransactionID, System.Byte[] InPurchaseToken )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.BuyShopItemCmd.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.BuyShopItemCmd.BuildMessageBuffer(binWriter, ref header, InShopItemID, InPlatform, InPackageName, InPurchaseTransactionID, InPurchaseToken );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  BuyShopItemCmd( System.UInt32 InShopItemID, string InPlatform, string InPackageName, string InPurchaseTransactionID, System.Byte[] InPurchaseToken )

		// Cmd: Give my stamina to other player
		public bool  GiveStaminaCmd( System.UInt64 InTargetPlayer )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.GiveStaminaCmd.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.GiveStaminaCmd.BuildMessageBuffer(binWriter, ref header, InTargetPlayer );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  GiveStaminaCmd( System.UInt64 InTargetPlayer )

		// Cmd: For debug, Change configue preset
		public bool  SetPresetGameConfigIDCmd( System.UInt32 InPresetID )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.SetPresetGameConfigIDCmd.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.SetPresetGameConfigIDCmd.BuildMessageBuffer(binWriter, ref header, InPresetID );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  SetPresetGameConfigIDCmd( System.UInt32 InPresetID )

		// Cmd: For Debug
		public bool  GainGameResourceCmd( System.Int32 InResource, System.Int32 InValue )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.GainGameResourceCmd.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.GainGameResourceCmd.BuildMessageBuffer(binWriter, ref header, InResource, InValue );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  GainGameResourceCmd( System.Int32 InResource, System.Int32 InValue )

	}; // public class PolicyGame : BR.Net.Policy
	public class SvrPolicyGame : BR.Net.Policy
	{
 
		public SvrPolicyGame( BR.Net.ConnectionMUDP connection )
			:base(connection)
		{
		}

		// Cmd: Player connected from a login server and moved to game server
		public bool  JoinGameServerRes( System.Int32 InResult, string InNickName, System.UInt64 InGameUID, System.UInt64 InPartyUID, System.UInt64 InPartyLeaderID, ref BR.MatchingQueueTicket InMatchingTicket )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.JoinGameServerRes.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.JoinGameServerRes.BuildMessageBuffer(binWriter, ref header, InResult, InNickName, InGameUID, InPartyUID, InPartyLeaderID,ref InMatchingTicket );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  JoinGameServerRes( System.Int32 InResult, string InNickName, System.UInt64 InGameUID, System.UInt64 InPartyUID, System.UInt64 InPartyLeaderID, ref BR.MatchingQueueTicket InMatchingTicket )


		// Cmd: player complition statues
		public bool  GetComplitionStateRes( System.Int32 InResult, string InComplitionState )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.GetComplitionStateRes.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.GetComplitionStateRes.BuildMessageBuffer(binWriter, ref header, InResult, InComplitionState );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  GetComplitionStateRes( System.Int32 InResult, string InComplitionState )


		// Cmd: Player complition state
		public bool  SetComplitionStateRes( System.Int32 InResult )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.SetComplitionStateRes.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.SetComplitionStateRes.BuildMessageBuffer(binWriter, ref header, InResult );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  SetComplitionStateRes( System.Int32 InResult )


		// Cmd: Register Google notification service ID, after this, the player will get notification from google. Only one notification ID can be active at a time
		public bool  RegisterGCMRes( System.Int32 InResult )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.RegisterGCMRes.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.RegisterGCMRes.BuildMessageBuffer(binWriter, ref header, InResult );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  RegisterGCMRes( System.Int32 InResult )


		// Cmd: Unregister Google notification service ID
		public bool  UnregisterGCMRes( System.Int32 InResult )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.UnregisterGCMRes.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.UnregisterGCMRes.BuildMessageBuffer(binWriter, ref header, InResult );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  UnregisterGCMRes( System.Int32 InResult )


		// Cmd: Invite friend
		public bool  InviteFriendRes( System.Int32 InResult )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.InviteFriendRes.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.InviteFriendRes.BuildMessageBuffer(binWriter, ref header, InResult );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  InviteFriendRes( System.Int32 InResult )


		// Cmd: Accept friend request
		public bool  AcceptFriendRequestRes( System.Int32 InResult, ref BR.FriendInformation InNewFriend )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.AcceptFriendRequestRes.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.AcceptFriendRequestRes.BuildMessageBuffer(binWriter, ref header, InResult,ref InNewFriend );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  AcceptFriendRequestRes( System.Int32 InResult, ref BR.FriendInformation InNewFriend )


		// S2C: Notification for friend request is accepted
		public bool  FriendRequestAcceptedS2CEvt( ref BR.FriendInformation InAccepter )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.FriendRequestAcceptedS2CEvt.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.FriendRequestAcceptedS2CEvt.BuildMessageBuffer(binWriter, ref header,ref InAccepter );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  FriendRequestAcceptedS2CEvt( ref BR.FriendInformation InAccepter )


		// Cmd: Remove friden form the friend list
		public bool  RemoveFriendRes( System.Int32 InResult, System.UInt64 InFriendID )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.RemoveFriendRes.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.RemoveFriendRes.BuildMessageBuffer(binWriter, ref header, InResult, InFriendID );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  RemoveFriendRes( System.Int32 InResult, System.UInt64 InFriendID )


		// S2C: Friend removed
		public bool  FriendRemovedS2CEvt( System.UInt64 InFriendID )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.FriendRemovedS2CEvt.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.FriendRemovedS2CEvt.BuildMessageBuffer(binWriter, ref header, InFriendID );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  FriendRemovedS2CEvt( System.UInt64 InFriendID )


		// Cmd: Get friend list
		public bool  GetFriendListRes( System.Int32 InResult, System.UInt16 InMaxFriendSlot, System.UInt16 InTotalNumberOfFriends, System.UInt16 InStartIndex, BR.FriendInformation[] InFriendList )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.GetFriendListRes.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.GetFriendListRes.BuildMessageBuffer(binWriter, ref header, InResult, InMaxFriendSlot, InTotalNumberOfFriends, InStartIndex, InFriendList );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  GetFriendListRes( System.Int32 InResult, System.UInt16 InMaxFriendSlot, System.UInt16 InTotalNumberOfFriends, System.UInt16 InStartIndex, BR.FriendInformation[] InFriendList )


		// Cmd: Query notification list
		public bool  GetNotificationListRes( System.Int32 InResult )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.GetNotificationListRes.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.GetNotificationListRes.BuildMessageBuffer(binWriter, ref header, InResult );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  GetNotificationListRes( System.Int32 InResult )


		// Cmd: Delete notification
		public bool  DeleteNotificationRes( System.Int32 InResult, System.UInt32 InNotificationID )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.DeleteNotificationRes.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.DeleteNotificationRes.BuildMessageBuffer(binWriter, ref header, InResult, InNotificationID );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  DeleteNotificationRes( System.Int32 InResult, System.UInt32 InNotificationID )


		// Cmd: Set notification is read
		public bool  SetNotificationReadRes( System.Int32 InResult, System.UInt32 InNotificationID )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.SetNotificationReadRes.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.SetNotificationReadRes.BuildMessageBuffer(binWriter, ref header, InResult, InNotificationID );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  SetNotificationReadRes( System.Int32 InResult, System.UInt32 InNotificationID )


		// Cmd: Accept notification
		public bool  AcceptNotificationRes( System.Int32 InResult, System.UInt32 InNotificationID )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.AcceptNotificationRes.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.AcceptNotificationRes.BuildMessageBuffer(binWriter, ref header, InResult, InNotificationID );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  AcceptNotificationRes( System.Int32 InResult, System.UInt32 InNotificationID )


		// S2C: Notify new notification
		public bool  NotifyS2CEvt( System.UInt32 InNotificationID, System.Int16 InMessageID, System.UInt64 InMessageParam0, System.UInt64 InMessageParam1, string InMessageText, System.Byte InIsRead, System.UInt64 InTimeStamp )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.NotifyS2CEvt.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.NotifyS2CEvt.BuildMessageBuffer(binWriter, ref header, InNotificationID, InMessageID, InMessageParam0, InMessageParam1, InMessageText, InIsRead, InTimeStamp );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  NotifyS2CEvt( System.UInt32 InNotificationID, System.Int16 InMessageID, System.UInt64 InMessageParam0, System.UInt64 InMessageParam1, string InMessageText, System.Byte InIsRead, System.UInt64 InTimeStamp )


		// Cmd: Query playerID list
		public bool  FindPlayerByEMailRes( System.Int32 InResult, ref BR.PlayerInformation InPlayer )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.FindPlayerByEMailRes.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.FindPlayerByEMailRes.BuildMessageBuffer(binWriter, ref header, InResult,ref InPlayer );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  FindPlayerByEMailRes( System.Int32 InResult, ref BR.PlayerInformation InPlayer )


		// Cmd: *Request Player Status Update
		public bool  RequestPlayerStatusUpdateRes( System.Int32 InResult )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.RequestPlayerStatusUpdateRes.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.RequestPlayerStatusUpdateRes.BuildMessageBuffer(binWriter, ref header, InResult );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  RequestPlayerStatusUpdateRes( System.Int32 InResult )


		// S2C: *Notify Player Status Updated
		public bool  NotifyPlayerStatusUpdatedS2CEvt( System.UInt64 InPlayerID, System.UInt32 InLatestActiveTime, System.Byte InIsInGame )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.NotifyPlayerStatusUpdatedS2CEvt.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.NotifyPlayerStatusUpdatedS2CEvt.BuildMessageBuffer(binWriter, ref header, InPlayerID, InLatestActiveTime, InIsInGame );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  NotifyPlayerStatusUpdatedS2CEvt( System.UInt64 InPlayerID, System.UInt32 InLatestActiveTime, System.Byte InIsInGame )


		// Cmd: Get Ranking lise
		public bool  GetRankingListRes( System.Int32 InResult, BR.TotalRankingPlayerInformation[] InRanking )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.GetRankingListRes.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.GetRankingListRes.BuildMessageBuffer(binWriter, ref header, InResult, InRanking );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  GetRankingListRes( System.Int32 InResult, BR.TotalRankingPlayerInformation[] InRanking )


		// Cmd: Game user game play information
		public bool  GetUserGamePlayerInfoRes( System.Int32 InResult, System.Int16 InLevel, System.Int64 InExp, System.Int64 InGameMoney, System.Int64 InGem, System.Int16 InStamina, System.Int32 InTotalPlayed, System.Int32 InWinPlaySC, System.Int32 InWinPlaySM, System.Int32 InWinPlaySS, System.Int32 InLosePlaySC, System.Int32 InLosePlaySM, System.Int32 InLosePlaySS, System.Int32 InWinPlayNC, System.Int32 InWinPlayNM, System.Int32 InWinPlayNS, System.Int32 InLosePlayNC, System.Int32 InLosePlayNM, System.Int32 InLosePlayNS, System.Int32 InWeeklyWin, System.Int32 InWeeklyLose )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.GetUserGamePlayerInfoRes.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.GetUserGamePlayerInfoRes.BuildMessageBuffer(binWriter, ref header, InResult, InLevel, InExp, InGameMoney, InGem, InStamina, InTotalPlayed, InWinPlaySC, InWinPlaySM, InWinPlaySS, InLosePlaySC, InLosePlaySM, InLosePlaySS, InWinPlayNC, InWinPlayNM, InWinPlayNS, InLosePlayNC, InLosePlayNM, InLosePlayNS, InWeeklyWin, InWeeklyLose );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  GetUserGamePlayerInfoRes( System.Int32 InResult, System.Int16 InLevel, System.Int64 InExp, System.Int64 InGameMoney, System.Int64 InGem, System.Int16 InStamina, System.Int32 InTotalPlayed, System.Int32 InWinPlaySC, System.Int32 InWinPlaySM, System.Int32 InWinPlaySS, System.Int32 InLosePlaySC, System.Int32 InLosePlaySM, System.Int32 InLosePlaySS, System.Int32 InWinPlayNC, System.Int32 InWinPlayNM, System.Int32 InWinPlayNS, System.Int32 InLosePlayNC, System.Int32 InLosePlayNM, System.Int32 InLosePlayNS, System.Int32 InWeeklyWin, System.Int32 InWeeklyLose )


		// Cmd: Game game play information
		public bool  GetGamePlayerInfoRes( System.Int32 InResult, System.UInt64 InPlayerID, System.Int16 InLevel, System.Int32 InTotalPlayed, System.Int32 InWinPlaySC, System.Int32 InWinPlaySM, System.Int32 InWinPlaySS, System.Int32 InLosePlaySC, System.Int32 InLosePlaySM, System.Int32 InLosePlaySS, System.Int32 InWinPlayNC, System.Int32 InWinPlayNM, System.Int32 InWinPlayNS, System.Int32 InLosePlayNC, System.Int32 InLosePlayNM, System.Int32 InLosePlayNS, System.Int32 InWeeklyWin, System.Int32 InWeeklyLose )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.GetGamePlayerInfoRes.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.GetGamePlayerInfoRes.BuildMessageBuffer(binWriter, ref header, InResult, InPlayerID, InLevel, InTotalPlayed, InWinPlaySC, InWinPlaySM, InWinPlaySS, InLosePlaySC, InLosePlaySM, InLosePlaySS, InWinPlayNC, InWinPlayNM, InWinPlayNS, InLosePlayNC, InLosePlayNM, InLosePlayNS, InWeeklyWin, InWeeklyLose );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  GetGamePlayerInfoRes( System.Int32 InResult, System.UInt64 InPlayerID, System.Int16 InLevel, System.Int32 InTotalPlayed, System.Int32 InWinPlaySC, System.Int32 InWinPlaySM, System.Int32 InWinPlaySS, System.Int32 InLosePlaySC, System.Int32 InLosePlaySM, System.Int32 InLosePlaySS, System.Int32 InWinPlayNC, System.Int32 InWinPlayNM, System.Int32 InWinPlayNS, System.Int32 InLosePlayNC, System.Int32 InLosePlayNM, System.Int32 InLosePlayNS, System.Int32 InWeeklyWin, System.Int32 InWeeklyLose )


		// S2C: Player level up event
		public bool  LevelUpS2CEvt( System.UInt64 InCurrentTotalExp, System.UInt32 InCurrentLevel )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.LevelUpS2CEvt.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.LevelUpS2CEvt.BuildMessageBuffer(binWriter, ref header, InCurrentTotalExp, InCurrentLevel );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  LevelUpS2CEvt( System.UInt64 InCurrentTotalExp, System.UInt32 InCurrentLevel )


		// Cmd: Change NickName
		public bool  SetNickNameRes( System.Int32 InResult, System.UInt64 InTotalGem, System.UInt64 InTotalGameMoney )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.SetNickNameRes.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.SetNickNameRes.BuildMessageBuffer(binWriter, ref header, InResult, InTotalGem, InTotalGameMoney );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  SetNickNameRes( System.Int32 InResult, System.UInt64 InTotalGem, System.UInt64 InTotalGameMoney )


		// Cmd: Create Party
		public bool  CreatePartyRes( System.Int32 InResult, System.UInt64 InPartyUID )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.CreatePartyRes.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.CreatePartyRes.BuildMessageBuffer(binWriter, ref header, InResult, InPartyUID );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  CreatePartyRes( System.Int32 InResult, System.UInt64 InPartyUID )


		// Cmd: Join party
		public bool  JoinPartyRes( System.Int32 InResult, System.UInt64 InPartyUID, System.UInt64 InPartyLeaderID, System.Byte[] InChatHistoryData )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.JoinPartyRes.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.JoinPartyRes.BuildMessageBuffer(binWriter, ref header, InResult, InPartyUID, InPartyLeaderID, InChatHistoryData );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  JoinPartyRes( System.Int32 InResult, System.UInt64 InPartyUID, System.UInt64 InPartyLeaderID, System.Byte[] InChatHistoryData )


		// S2C: Player Joined event
		public bool  PartyPlayerJoinedS2CEvt( System.UInt64 InPartyUID, ref BR.PlayerInformation InJoinedPlayer )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.PartyPlayerJoinedS2CEvt.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.PartyPlayerJoinedS2CEvt.BuildMessageBuffer(binWriter, ref header, InPartyUID,ref InJoinedPlayer );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  PartyPlayerJoinedS2CEvt( System.UInt64 InPartyUID, ref BR.PlayerInformation InJoinedPlayer )


		// S2C: Party leader changed event
		public bool  PartyLeaderChangedS2CEvt( System.UInt64 InPartyUID, System.UInt64 InNewLeaderID )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.PartyLeaderChangedS2CEvt.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.PartyLeaderChangedS2CEvt.BuildMessageBuffer(binWriter, ref header, InPartyUID, InNewLeaderID );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  PartyLeaderChangedS2CEvt( System.UInt64 InPartyUID, System.UInt64 InNewLeaderID )


		// Cmd: Leave party command
		public bool  LeavePartyRes( System.Int32 InResult )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.LeavePartyRes.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.LeavePartyRes.BuildMessageBuffer(binWriter, ref header, InResult );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  LeavePartyRes( System.Int32 InResult )


		// S2C: Party Player left event
		public bool  PartyPlayerLeftS2CEvt( System.UInt64 InPartyUID, System.UInt64 InLeftPlayerID )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.PartyPlayerLeftS2CEvt.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.PartyPlayerLeftS2CEvt.BuildMessageBuffer(binWriter, ref header, InPartyUID, InLeftPlayerID );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  PartyPlayerLeftS2CEvt( System.UInt64 InPartyUID, System.UInt64 InLeftPlayerID )


		// Cmd: Kick player from the party
		public bool  PartyKickPlayerRes( System.Int32 InResult )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.PartyKickPlayerRes.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.PartyKickPlayerRes.BuildMessageBuffer(binWriter, ref header, InResult );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  PartyKickPlayerRes( System.Int32 InResult )


		// S2C: Party Player kicked message
		public bool  PartyPlayerKickedS2CEvt( System.UInt64 InPartyUID, System.UInt64 InKickedPlayerID )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.PartyPlayerKickedS2CEvt.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.PartyPlayerKickedS2CEvt.BuildMessageBuffer(binWriter, ref header, InPartyUID, InKickedPlayerID );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  PartyPlayerKickedS2CEvt( System.UInt64 InPartyUID, System.UInt64 InKickedPlayerID )


		// Cmd: Invite a player to the party
		public bool  PartyInviteRes( System.Int32 InResult )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.PartyInviteRes.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.PartyInviteRes.BuildMessageBuffer(binWriter, ref header, InResult );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  PartyInviteRes( System.Int32 InResult )


		// S2C: Party invite requested
		public bool  PartyInviteRequestedS2CEvt( System.UInt64 InInviterID, string InInviterName, System.UInt64 InPartyToJoinUID )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.PartyInviteRequestedS2CEvt.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.PartyInviteRequestedS2CEvt.BuildMessageBuffer(binWriter, ref header, InInviterID, InInviterName, InPartyToJoinUID );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  PartyInviteRequestedS2CEvt( System.UInt64 InInviterID, string InInviterName, System.UInt64 InPartyToJoinUID )


		// Cmd: Send Party quick chat message
		public bool  PartyQuickChatMessageRes( System.Int32 InResult )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.PartyQuickChatMessageRes.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.PartyQuickChatMessageRes.BuildMessageBuffer(binWriter, ref header, InResult );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  PartyQuickChatMessageRes( System.Int32 InResult )


		// S2C: Party Chatting message event
		public bool  PartyQuickChatMessageS2CEvt( System.UInt64 InSenderID, System.UInt32 InQuickChatID )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.PartyQuickChatMessageS2CEvt.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.PartyQuickChatMessageS2CEvt.BuildMessageBuffer(binWriter, ref header, InSenderID, InQuickChatID );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  PartyQuickChatMessageS2CEvt( System.UInt64 InSenderID, System.UInt32 InQuickChatID )


		// Cmd: Party chatting
		public bool  PartyChatMessageRes( System.Int32 InResult )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.PartyChatMessageRes.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.PartyChatMessageRes.BuildMessageBuffer(binWriter, ref header, InResult );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  PartyChatMessageRes( System.Int32 InResult )


		// S2C: Party Chatting message event
		public bool  PartyChatMessageS2CEvt( System.UInt64 InSenderID, string InSenderName, string InChatMessage )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.PartyChatMessageS2CEvt.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.PartyChatMessageS2CEvt.BuildMessageBuffer(binWriter, ref header, InSenderID, InSenderName, InChatMessage );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  PartyChatMessageS2CEvt( System.UInt64 InSenderID, string InSenderName, string InChatMessage )


		// Cmd: Join to a game
		public bool  JoinGameRes( System.Int32 InResult, System.UInt64 InInsUID, System.UInt32 InTimeStamp, BR.GameStateID InGameState, System.Byte InDay, System.Byte InMaxPlayer, System.Byte InPlayerIndex, System.Byte InPlayerCharacter, BR.PlayerRole InRole, System.Byte InDead, System.Byte[] InChatHistoryData, System.Byte[] InGameLogData )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.JoinGameRes.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.JoinGameRes.BuildMessageBuffer(binWriter, ref header, InResult, InInsUID, InTimeStamp, InGameState, InDay, InMaxPlayer, InPlayerIndex, InPlayerCharacter, InRole, InDead, InChatHistoryData, InGameLogData );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  JoinGameRes( System.Int32 InResult, System.UInt64 InInsUID, System.UInt32 InTimeStamp, BR.GameStateID InGameState, System.Byte InDay, System.Byte InMaxPlayer, System.Byte InPlayerIndex, System.Byte InPlayerCharacter, BR.PlayerRole InRole, System.Byte InDead, System.Byte[] InChatHistoryData, System.Byte[] InGameLogData )


		// S2C: Player Joined in the game
		public bool  PlayerJoinedS2CEvt( System.UInt64 InGameInsUID, ref BR.PlayerInformation InJoinedPlayer, BR.PlayerRole InJoinedPlayerRole, System.Byte InJoinedPlayerDead, System.Byte InJoinedPlayerIndex, System.Byte InJoinedPlayerCharacter )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.PlayerJoinedS2CEvt.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.PlayerJoinedS2CEvt.BuildMessageBuffer(binWriter, ref header, InGameInsUID,ref InJoinedPlayer, InJoinedPlayerRole, InJoinedPlayerDead, InJoinedPlayerIndex, InJoinedPlayerCharacter );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  PlayerJoinedS2CEvt( System.UInt64 InGameInsUID, ref BR.PlayerInformation InJoinedPlayer, BR.PlayerRole InJoinedPlayerRole, System.Byte InJoinedPlayerDead, System.Byte InJoinedPlayerIndex, System.Byte InJoinedPlayerCharacter )


		// Cmd: Leave Game
		public bool  LeaveGameRes( System.Int32 InResult )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.LeaveGameRes.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.LeaveGameRes.BuildMessageBuffer(binWriter, ref header, InResult );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  LeaveGameRes( System.Int32 InResult )


		// S2C: Player left event
		public bool  PlayerLeftS2CEvt( System.UInt64 InGameInsUID, System.UInt64 InLeftPlayerID )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.PlayerLeftS2CEvt.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.PlayerLeftS2CEvt.BuildMessageBuffer(binWriter, ref header, InGameInsUID, InLeftPlayerID );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  PlayerLeftS2CEvt( System.UInt64 InGameInsUID, System.UInt64 InLeftPlayerID )


		// Cmd: Kick player
		public bool  KickPlayerRes( System.Int32 InResult )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.KickPlayerRes.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.KickPlayerRes.BuildMessageBuffer(binWriter, ref header, InResult );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  KickPlayerRes( System.Int32 InResult )


		// S2C: Player kicked
		public bool  PlayerKickedS2CEvt( System.UInt64 InGameInsUID, System.UInt64 InKickedPlayerID )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.PlayerKickedS2CEvt.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.PlayerKickedS2CEvt.BuildMessageBuffer(binWriter, ref header, InGameInsUID, InKickedPlayerID );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  PlayerKickedS2CEvt( System.UInt64 InGameInsUID, System.UInt64 InKickedPlayerID )


		// Cmd: Assign role + Game state reset
		public bool  AssignRoleRes( System.Int32 InResult )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.AssignRoleRes.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.AssignRoleRes.BuildMessageBuffer(binWriter, ref header, InResult );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  AssignRoleRes( System.Int32 InResult )


		// S2C: Role assigned event
		public bool  RoleAssignedS2CEvt( System.UInt64 InGameInsUID, System.UInt64 InPlayerID, BR.PlayerRole InRole )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.RoleAssignedS2CEvt.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.RoleAssignedS2CEvt.BuildMessageBuffer(binWriter, ref header, InGameInsUID, InPlayerID, InRole );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  RoleAssignedS2CEvt( System.UInt64 InGameInsUID, System.UInt64 InPlayerID, BR.PlayerRole InRole )


		// Cmd: Send chatting message to the game
		public bool  ChatMessageRes( System.Int32 InResult )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.ChatMessageRes.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.ChatMessageRes.BuildMessageBuffer(binWriter, ref header, InResult );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  ChatMessageRes( System.Int32 InResult )


		// S2C: Chatting message event 
		public bool  ChatMessageS2CEvt( System.UInt64 InSenderID, BR.PlayerRole InRole, string InSenderName, string InChatMessage )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.ChatMessageS2CEvt.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.ChatMessageS2CEvt.BuildMessageBuffer(binWriter, ref header, InSenderID, InRole, InSenderName, InChatMessage );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  ChatMessageS2CEvt( System.UInt64 InSenderID, BR.PlayerRole InRole, string InSenderName, string InChatMessage )


		// Cmd: Advance game
		public bool  AdvanceGameRes( System.Int32 InResult )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.AdvanceGameRes.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.AdvanceGameRes.BuildMessageBuffer(binWriter, ref header, InResult );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  AdvanceGameRes( System.Int32 InResult )


		// S2C: The game state is advanced
		public bool  GameAdvancedS2CEvt( System.UInt64 InGameInsUID, System.UInt32 InTimeStamp, BR.GameStateID InGameState, System.Byte InDay )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.GameAdvancedS2CEvt.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.GameAdvancedS2CEvt.BuildMessageBuffer(binWriter, ref header, InGameInsUID, InTimeStamp, InGameState, InDay );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  GameAdvancedS2CEvt( System.UInt64 InGameInsUID, System.UInt32 InTimeStamp, BR.GameStateID InGameState, System.Byte InDay )


		// S2C: Game is ended
		public bool  GameEndedS2CEvt( System.UInt64 InGameInsUID, BR.GameWinner InWinner, System.UInt32 InGainedExp, System.UInt32 InGainedGameMoney )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.GameEndedS2CEvt.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.GameEndedS2CEvt.BuildMessageBuffer(binWriter, ref header, InGameInsUID, InWinner, InGainedExp, InGainedGameMoney );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  GameEndedS2CEvt( System.UInt64 InGameInsUID, BR.GameWinner InWinner, System.UInt32 InGainedExp, System.UInt32 InGainedGameMoney )


		// Cmd: Vote game advance
		public bool  VoteGameAdvanceRes( System.Int32 InResult )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.VoteGameAdvanceRes.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.VoteGameAdvanceRes.BuildMessageBuffer(binWriter, ref header, InResult );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  VoteGameAdvanceRes( System.Int32 InResult )


		// S2C: GameAdvance is Voted
		public bool  GameAdvanceVotedS2CEvt( System.UInt64 InGameInsUID, System.UInt64 InVoter )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.GameAdvanceVotedS2CEvt.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.GameAdvanceVotedS2CEvt.BuildMessageBuffer(binWriter, ref header, InGameInsUID, InVoter );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  GameAdvanceVotedS2CEvt( System.UInt64 InGameInsUID, System.UInt64 InVoter )


		// Cmd: Vote
		public bool  VoteRes( System.Int32 InResult )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.VoteRes.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.VoteRes.BuildMessageBuffer(binWriter, ref header, InResult );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  VoteRes( System.Int32 InResult )


		// S2C: Player Voted
		public bool  VotedS2CEvt( System.UInt64 InGameInsUID, System.UInt64 InVoter, System.UInt64 InVotedTarget )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.VotedS2CEvt.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.VotedS2CEvt.BuildMessageBuffer(binWriter, ref header, InGameInsUID, InVoter, InVotedTarget );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  VotedS2CEvt( System.UInt64 InGameInsUID, System.UInt64 InVoter, System.UInt64 InVotedTarget )


		// S2C: Vote is ended
		public bool  VoteEndS2CEvt( System.UInt64 InGameInsUID, System.UInt64[] InVoted )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.VoteEndS2CEvt.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.VoteEndS2CEvt.BuildMessageBuffer(binWriter, ref header, InGameInsUID, InVoted );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  VoteEndS2CEvt( System.UInt64 InGameInsUID, System.UInt64[] InVoted )


		// S2C: Player Killed
		public bool  PlayerKilledS2CEvt( System.UInt64 InGameInsUID, System.UInt64 InKilledPlayer, BR.PlayerKilledReason InReason )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.PlayerKilledS2CEvt.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.PlayerKilledS2CEvt.BuildMessageBuffer(binWriter, ref header, InGameInsUID, InKilledPlayer, InReason );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  PlayerKilledS2CEvt( System.UInt64 InGameInsUID, System.UInt64 InKilledPlayer, BR.PlayerKilledReason InReason )


		// S2C: Player Voted
		public bool  PlayerRevealedS2CEvt( System.UInt64 InGameInsUID, System.UInt64 InRevealedPlayerID, BR.PlayerRole InRole, BR.PlayerRevealedReason InReason )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.PlayerRevealedS2CEvt.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.PlayerRevealedS2CEvt.BuildMessageBuffer(binWriter, ref header, InGameInsUID, InRevealedPlayerID, InRole, InReason );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  PlayerRevealedS2CEvt( System.UInt64 InGameInsUID, System.UInt64 InRevealedPlayerID, BR.PlayerRole InRole, BR.PlayerRevealedReason InReason )


		// Cmd: Play again with the current players
		public bool  GamePlayAgainRes( System.Int32 InResult, System.UInt64 InTotalGem, System.UInt64 InTotalGameMoney )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.GamePlayAgainRes.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.GamePlayAgainRes.BuildMessageBuffer(binWriter, ref header, InResult, InTotalGem, InTotalGameMoney );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  GamePlayAgainRes( System.Int32 InResult, System.UInt64 InTotalGem, System.UInt64 InTotalGameMoney )


		// S2C: Somebody pressed play again. Only one of PartyUID and GameInsUID can have a value
		public bool  GamePlayAgainS2CEvt( System.UInt64 InPartyUID, System.UInt64 InLeadPlayer )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.GamePlayAgainS2CEvt.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.GamePlayAgainS2CEvt.BuildMessageBuffer(binWriter, ref header, InPartyUID, InLeadPlayer );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  GamePlayAgainS2CEvt( System.UInt64 InPartyUID, System.UInt64 InLeadPlayer )


		// Cmd: Player. reveal a player
		public bool  GameRevealPlayerRes( System.Int32 InResult, System.UInt64[] InRevealedPlayerID, BR.PlayerRole[] InRevealedRole, System.UInt64 InTotalGem, System.UInt64 InTotalGameMoney )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.GameRevealPlayerRes.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.GameRevealPlayerRes.BuildMessageBuffer(binWriter, ref header, InResult, InRevealedPlayerID, InRevealedRole, InTotalGem, InTotalGameMoney );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  GameRevealPlayerRes( System.Int32 InResult, System.UInt64[] InRevealedPlayerID, BR.PlayerRole[] InRevealedRole, System.UInt64 InTotalGem, System.UInt64 InTotalGameMoney )


		// Cmd: Player. revive himself
		public bool  GamePlayerReviveRes( System.Int32 InResult, System.UInt64 InTotalGem, System.UInt64 InTotalGameMoney )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.GamePlayerReviveRes.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.GamePlayerReviveRes.BuildMessageBuffer(binWriter, ref header, InResult, InTotalGem, InTotalGameMoney );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  GamePlayerReviveRes( System.Int32 InResult, System.UInt64 InTotalGem, System.UInt64 InTotalGameMoney )


		// S2C: Player is revived
		public bool  GamePlayerRevivedS2CEvt( System.UInt64 InRevivedPlayerID )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.GamePlayerRevivedS2CEvt.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.GamePlayerRevivedS2CEvt.BuildMessageBuffer(binWriter, ref header, InRevivedPlayerID );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  GamePlayerRevivedS2CEvt( System.UInt64 InRevivedPlayerID )


		// Cmd: Player. reset ranking
		public bool  GamePlayerResetRankRes( System.Int32 InResult, System.UInt64 InTotalGem, System.UInt64 InTotalGameMoney )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.GamePlayerResetRankRes.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.GamePlayerResetRankRes.BuildMessageBuffer(binWriter, ref header, InResult, InTotalGem, InTotalGameMoney );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  GamePlayerResetRankRes( System.Int32 InResult, System.UInt64 InTotalGem, System.UInt64 InTotalGameMoney )


		// Cmd: Request Game match
		public bool  RequestGameMatchRes( System.Int32 InResult, System.UInt64 InTotalGem, System.UInt64 InTotalGameMoney )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.RequestGameMatchRes.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.RequestGameMatchRes.BuildMessageBuffer(binWriter, ref header, InResult, InTotalGem, InTotalGameMoney );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  RequestGameMatchRes( System.Int32 InResult, System.UInt64 InTotalGem, System.UInt64 InTotalGameMoney )


		// S2C: Game matched
		public bool  GameMatchedS2CEvt( System.UInt64 InInsUID, System.UInt32 InTimeStamp, BR.GameStateID InGameState, System.Byte InDay, System.Byte InMaxPlayer, System.Byte InPlayerIndex, System.Byte InPlayerCharacter, BR.PlayerRole InRole, System.Byte InDead, System.Byte[] InChatHistoryData, System.Byte[] InGameLogData, System.UInt32 InStamina, System.UInt64 InTotalGem, System.UInt64 InTotalGameMoney )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.GameMatchedS2CEvt.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.GameMatchedS2CEvt.BuildMessageBuffer(binWriter, ref header, InInsUID, InTimeStamp, InGameState, InDay, InMaxPlayer, InPlayerIndex, InPlayerCharacter, InRole, InDead, InChatHistoryData, InGameLogData, InStamina, InTotalGem, InTotalGameMoney );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  GameMatchedS2CEvt( System.UInt64 InInsUID, System.UInt32 InTimeStamp, BR.GameStateID InGameState, System.Byte InDay, System.Byte InMaxPlayer, System.Byte InPlayerIndex, System.Byte InPlayerCharacter, BR.PlayerRole InRole, System.Byte InDead, System.Byte[] InChatHistoryData, System.Byte[] InGameLogData, System.UInt32 InStamina, System.UInt64 InTotalGem, System.UInt64 InTotalGameMoney )


		// S2C: Game match failed
		public bool  GameMatchFailedS2CEvt( System.Int32 InFailedReason )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.GameMatchFailedS2CEvt.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.GameMatchFailedS2CEvt.BuildMessageBuffer(binWriter, ref header, InFailedReason );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  GameMatchFailedS2CEvt( System.Int32 InFailedReason )


		// S2C: Game matching started
		public bool  GameMatchingStartedS2CEvt(  )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.GameMatchingStartedS2CEvt.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.GameMatchingStartedS2CEvt.BuildMessageBuffer(binWriter, ref header );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  GameMatchingStartedS2CEvt(  )


		// Cmd: Cancel Game match
		public bool  CancelGameMatchRes( System.Int32 InResult )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.CancelGameMatchRes.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.CancelGameMatchRes.BuildMessageBuffer(binWriter, ref header, InResult );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  CancelGameMatchRes( System.Int32 InResult )


		// S2C: game matching canceled
		public bool  GameMatchingCanceledS2CEvt(  )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.GameMatchingCanceledS2CEvt.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.GameMatchingCanceledS2CEvt.BuildMessageBuffer(binWriter, ref header );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  GameMatchingCanceledS2CEvt(  )


		// Cmd: Buy shop item prepare
		public bool  BuyShopItemPrepareRes( System.Int32 InResult, System.UInt32 InShopItemID, string InPurchaseID )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.BuyShopItemPrepareRes.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.BuyShopItemPrepareRes.BuildMessageBuffer(binWriter, ref header, InResult, InShopItemID, InPurchaseID );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  BuyShopItemPrepareRes( System.Int32 InResult, System.UInt32 InShopItemID, string InPurchaseID )


		// Cmd: Buy shop item
		public bool  BuyShopItemRes( System.Int32 InResult, System.UInt32 InShopItemID )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.BuyShopItemRes.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.BuyShopItemRes.BuildMessageBuffer(binWriter, ref header, InResult, InShopItemID );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  BuyShopItemRes( System.Int32 InResult, System.UInt32 InShopItemID )


		// Cmd: Give my stamina to other player
		public bool  GiveStaminaRes( System.Int32 InResult, System.UInt64 InTargetPlayer, System.UInt64 InTimeStamp )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.GiveStaminaRes.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.GiveStaminaRes.BuildMessageBuffer(binWriter, ref header, InResult, InTargetPlayer, InTimeStamp );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  GiveStaminaRes( System.Int32 InResult, System.UInt64 InTargetPlayer, System.UInt64 InTimeStamp )


		// Cmd: For debug, Change configue preset
		public bool  SetPresetGameConfigIDRes( System.Int32 InResult )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.SetPresetGameConfigIDRes.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.SetPresetGameConfigIDRes.BuildMessageBuffer(binWriter, ref header, InResult );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  SetPresetGameConfigIDRes( System.Int32 InResult )


		// Cmd: For Debug
		public bool  GainGameResourceRes( System.Int32 InResult )
		{
 			MessageHeader header = new MessageHeader(BR.Message.Game.GainGameResourceRes.MID);
			var binWriter = m_Connection.PrepareMessgeBuffer();
			BR.Message.Game.GainGameResourceRes.BuildMessageBuffer(binWriter, ref header, InResult );
			return m_Connection.PendingReliableSend(ref header, binWriter);

		} // public bool  GainGameResourceRes( System.Int32 InResult )


	}; // public class SvrPolicyGame : BR.Net.Policy




} // namespace BR.Policy


