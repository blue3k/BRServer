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
using BR.GameSystem;
using BR.Message;
using BR.Net;
using BR.Message.Game;

using System.Threading;

namespace BR.Policy
{
	public class PolicyTest : BR.Net.Policy
	{
        MessageHeader m_Header;
        PacketMemoryWriter m_BinWriter;
        Random rand = new Random();

		public PolicyTest( BR.Net.ConnectionTest connection )
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

        public void JoinGameServerCmd(System.UInt64 InAccID, System.UInt64 InTicket, System.UInt64 InLoginEntityUID)
        {
            MatchingQueueTicket matchingTicket;
            matchingTicket.QueueItemID = 0;
            matchingTicket.QueueUID = 0;

            BRDebug.Log("PolicyTest >> JoinGameServerCmd");

            // JoinGameServerRes
            InitializeMsgData(BR.Message.Game.JoinGameServerRes.MID);
            BR.Message.Game.JoinGameServerRes.BuildMessageBuffer(m_BinWriter, ref m_Header, 0, "test", 0, 0, 0, ref matchingTicket);
            m_Connection.MessageProcessor.AddTestMessage(new JoinGameServerRes(), ref m_Header, GetReader());
        }

		// Cmd: Master
		public void  RegisterGCMCmd( string InGCMRegisteredID )
		{
			BRDebug.Log ("PolicyTest >> RegisterGCMCmd");

            // RegisterGCMRes
			InitializeMsgData (BR.Message.Game.RegisterGCMRes.MID);
			BR.Message.Game.RegisterGCMRes.BuildMessageBuffer(m_BinWriter, ref m_Header, 0);
			m_Connection.MessageProcessor.AddTestMessage (new RegisterGCMRes(), ref m_Header, GetReader ());
		}

		// Cmd: Master
		public void  UnregisterGCMCmd( string InGCMRegisteredID )
		{
			BRDebug.Log ("PolicyTest >> UnregisterGCMCmd");

            // UnregisterGCMRes
			InitializeMsgData (BR.Message.Game.UnregisterGCMRes.MID);
			BR.Message.Game.UnregisterGCMRes.BuildMessageBuffer(m_BinWriter, ref m_Header, 0);
			m_Connection.MessageProcessor.AddTestMessage (new UnregisterGCMRes(), ref m_Header, GetReader ());
		}

        // Cmd: Request Player Status Update
		public void RequestPlayerStatusUpdateCmd( System.UInt64[] InTargetPlayerID )
        {
            BRDebug.Log("PolicyTest >> RequestPlayerStatusUpdateCmd");

            // InviteFriendRes
            InitializeMsgData(BR.Message.Game.RequestPlayerStatusUpdateRes.MID);
            BR.Message.Game.RequestPlayerStatusUpdateRes.BuildMessageBuffer(m_BinWriter, ref m_Header, 0);
            m_Connection.MessageProcessor.AddTestMessage(new RequestPlayerStatusUpdateRes(), ref m_Header, GetReader());

            var currentTime = TimeUTC.Now();
            foreach( var targetPlayerID in InTargetPlayerID )
            {
                uint activeTime = (uint)(currentTime - rand.Next(10000));
                if( rand.Next(3) == 0 ) activeTime = 0;
                InitializeMsgData(BR.Message.Game.NotifyPlayerStatusUpdatedS2CEvt.MID);
                BR.Message.Game.NotifyPlayerStatusUpdatedS2CEvt.BuildMessageBuffer(m_BinWriter, ref m_Header, targetPlayerID, activeTime, (byte)rand.Next(1));
                m_Connection.MessageProcessor.AddTestMessage(new NotifyPlayerStatusUpdatedS2CEvt(), ref m_Header, GetReader());
            }

        } // public void  RequestPlayerStatusUpdateCmd( System.UInt64[] InTargetPlayerID )

		// Cmd: Master
		public void  InviteFriendCmd( System.UInt64 InFriendID )
		{
			BRDebug.Log ("PolicyTest >> InviteFriendCmd");

            // InviteFriendRes
			InitializeMsgData (BR.Message.Game.InviteFriendRes.MID);
			BR.Message.Game.InviteFriendRes.BuildMessageBuffer(m_BinWriter, ref m_Header, 0);
			m_Connection.MessageProcessor.AddTestMessage (new InviteFriendRes(), ref m_Header, GetReader ());
		}

        public void AcceptFriendRequestCmd(System.UInt64 InInviterID, System.UInt64 InInviterFacebookUID)
        {
            BRDebug.Log("PolicyTest >> AcceptFriendRequestCmd");

            // AcceptFriendRequestRes
            InitializeMsgData(BR.Message.Game.AcceptFriendRequestRes.MID);
            var playerInfo = new FriendInformation();
            BR.Message.Game.AcceptFriendRequestRes.BuildMessageBuffer(m_BinWriter, ref m_Header, 0, ref playerInfo);
            m_Connection.MessageProcessor.AddTestMessage(new AcceptFriendRequestRes(), ref m_Header, GetReader());
        }

        public void RemoveFriendCmd(System.UInt64 InFriendID)
        {
            BRDebug.Log("PolicyTest >> RemoveFriendCmd");

            // RemoveFriendRes
            InitializeMsgData(BR.Message.Game.RemoveFriendRes.MID);
            BR.Message.Game.RemoveFriendRes.BuildMessageBuffer(m_BinWriter, ref m_Header, 0, 1);
            m_Connection.MessageProcessor.AddTestMessage(new RemoveFriendRes(), ref m_Header, GetReader());
        }

		public void  GetFriendListCmd(  )
		{
			BRDebug.Log ("PolicyTest >> GetFriendListCmd");

            // GetFriendListRes
			InitializeMsgData (BR.Message.Game.GetFriendListRes.MID);
            var playerInfoList = new List<FriendInformation>();

			for (int i = 0; i < 8; ++i) 
			{
				var playerInfo = new FriendInformation ();
                playerInfo.FacebookUID = 1 + (ulong)i;
				playerInfo.PlayerID = 1+(ulong)i;
				playerInfo.NickName = "test"+i.ToString();

                if (0 == i % 2)
                    playerInfo.IsPlayingGame = true;
                else
                    playerInfo.IsPlayingGame = false;

				playerInfoList.Add (playerInfo);
			}

			BR.Message.Game.GetFriendListRes.BuildMessageBuffer(m_BinWriter, ref m_Header, 0, playerInfoList.ToArray ());
			m_Connection.MessageProcessor.AddTestMessage (new GetFriendListRes(), ref m_Header, GetReader ());
		}

		// Cmd: Query notification list
		public void  GetNotificationListCmd(  )
		{
			BRDebug.Log ("PolicyTest >> GetNotificationListCmd");

            // GetNotificationListRes
			InitializeMsgData (BR.Message.Game.GetNotificationListRes.MID);
			BR.Message.Game.GetNotificationListRes.BuildMessageBuffer(m_BinWriter, ref m_Header, 0);
			m_Connection.MessageProcessor.AddTestMessage (new GetNotificationListRes(), ref m_Header, GetReader ());

            for (int i = 0; i < 2; ++i )
            {
                InitializeMsgData(BR.Message.Game.NotifyS2CEvt.MID);
                BR.Message.Game.NotifyS2CEvt.BuildMessageBuffer(m_BinWriter, ref m_Header, (uint)i, (short)i, (ulong)i, (ulong)i, "test", 0, 0);
                m_Connection.MessageProcessor.AddTestMessage(new NotifyS2CEvt(), ref m_Header, GetReader());
            }
		}

		// Cmd: Delete notification
		public void  DeleteNotificationCmd( System.UInt32 InNotificationID )
		{
			BRDebug.Log ("PolicyTest >> DeleteNotificationCmd");

            // DeleteNotificationRes
			InitializeMsgData (BR.Message.Game.DeleteNotificationRes.MID);
			BR.Message.Game.DeleteNotificationRes.BuildMessageBuffer(m_BinWriter, ref m_Header, 0, InNotificationID);
			m_Connection.MessageProcessor.AddTestMessage (new DeleteNotificationRes(), ref m_Header, GetReader ());
		}

		public void SetNotificationReadCmd( System.UInt32 inNotificationID )
		{
			BRDebug.Log ("PolicyTest >> SetNotificationReadCmd");

			// DeleteNotificationRes
			InitializeMsgData (BR.Message.Game.SetNotificationReadRes.MID);
			BR.Message.Game.SetNotificationReadRes.BuildMessageBuffer(m_BinWriter, ref m_Header, 0, inNotificationID);
			m_Connection.MessageProcessor.AddTestMessage (new SetNotificationReadRes(), ref m_Header, GetReader ());
		}

        public void FindPlayerByEMailCmd(string InPlayerEMail)
        {
            BRDebug.Log("PolicyTest >> FindPlayerByEMailCmd");

            // FindPlayerByEMailRes
            InitializeMsgData(BR.Message.Game.FindPlayerByEMailRes.MID);
            var playerInfo = new PlayerInformation();
            BR.Message.Game.FindPlayerByEMailRes.BuildMessageBuffer(m_BinWriter, ref m_Header, 0, ref playerInfo);
            m_Connection.MessageProcessor.AddTestMessage(new FindPlayerByEMailRes(), ref m_Header, GetReader());
        }

        // Cmd: Master
        public void SetNickNameCmd(string InNickName)
        {
            BRDebug.Log("PolicyTest >> SetNickNameCmd");

            // SetNickNameRes
            InitializeMsgData(BR.Message.Game.SetNickNameRes.MID);
            BR.Message.Game.SetNickNameRes.BuildMessageBuffer(m_BinWriter, ref m_Header, 0);
            m_Connection.MessageProcessor.AddTestMessage(new SetNickNameRes(), ref m_Header, GetReader());
        }

		// Cmd: Join Game
		public void  CreatePartyCmd(  )
		{
			BRDebug.Log ("PolicyTest >> CreatePartyCmd");

            // CreatePartyRes
			InitializeMsgData (BR.Message.Game.CreatePartyRes.MID);
			BR.Message.Game.CreatePartyRes.BuildMessageBuffer(m_BinWriter, ref m_Header, 0, 0);
			m_Connection.MessageProcessor.AddTestMessage (new CreatePartyRes(), ref m_Header, GetReader ());
		}

		// Cmd: Join Party
		public void  JoinPartyCmd( System.UInt64 InPartyUID, System.UInt64 InInviterID)
		{
			BRDebug.Log ("PolicyTest >> JoinPartyCmd");

            // JoinPartyRes
			InitializeMsgData (BR.Message.Game.JoinPartyRes.MID);
			BR.Message.Game.JoinPartyRes.BuildMessageBuffer(m_BinWriter, ref m_Header, 0, 0, 0, null );
			m_Connection.MessageProcessor.AddTestMessage (new JoinPartyRes(), ref m_Header, GetReader ());
		}

		// Cmd: Kick player
		public void  LeavePartyCmd( System.UInt64 InPartyUID, System.UInt64 InPlayerID )
		{
			BRDebug.Log ("PolicyTest >> LeavePartyCmd");

            // LeavePartyRes
			InitializeMsgData (BR.Message.Game.LeavePartyRes.MID);
			BR.Message.Game.LeavePartyRes.BuildMessageBuffer(m_BinWriter, ref m_Header, 0 );
			m_Connection.MessageProcessor.AddTestMessage (new LeavePartyRes(), ref m_Header, GetReader ());
		}

        // Cmd: Kick player
        public void PartyKickPlayerCmd(System.UInt64 InPartyUID, System.UInt64 InPlayerID, System.UInt64 InPlayerToKick)
        {
            BRDebug.Log("PolicyTest >> PartyKickPlayerCmd");

            // PartyKickPlayerRes
            InitializeMsgData(BR.Message.Game.PartyKickPlayerRes.MID);
            BR.Message.Game.PartyKickPlayerRes.BuildMessageBuffer(m_BinWriter, ref m_Header, 0);
            m_Connection.MessageProcessor.AddTestMessage(new PartyKickPlayerRes(), ref m_Header, GetReader());
        }

		// Cmd: Join Party
		public void  PartyInviteCmd( System.UInt64 InInviteTargetID )
		{
			BRDebug.Log ("PolicyTest >> PartyInviteCmd");

            // PartyInviteRes
			InitializeMsgData (BR.Message.Game.PartyInviteRes.MID);
			BR.Message.Game.PartyInviteRes.BuildMessageBuffer(m_BinWriter, ref m_Header, 0 );
			m_Connection.MessageProcessor.AddTestMessage (new PartyInviteRes(), ref m_Header, GetReader ());
		}

		public void  PartyQuickChatMessageCmd( System.UInt32 InQuickChatID )
		{
			BRDebug.Log ("PolicyTest >> PartyQuickChatMessageCmd");

			// PartyChatMessageRes
			InitializeMsgData (BR.Message.Game.PartyQuickChatMessageRes.MID);
			BR.Message.Game.PartyQuickChatMessageRes.BuildMessageBuffer(m_BinWriter, ref m_Header, 0 );
			m_Connection.MessageProcessor.AddTestMessage (new PartyQuickChatMessageRes(), ref m_Header, GetReader ());

			foreach (var player in BR.GameSystem.BRGame.Instance.PlayerManager)
			{
				InitializeMsgData (BR.Message.Game.PartyQuickChatMessageS2CEvt.MID);
				BR.Message.Game.PartyQuickChatMessageS2CEvt.BuildMessageBuffer(m_BinWriter, ref m_Header, player.PlayerID, InQuickChatID );
				m_Connection.MessageProcessor.AddTestMessage (new PartyQuickChatMessageS2CEvt(), ref m_Header, GetReader ());
			}
		}

		public void  PartyChatMessageCmd( string InChatMessage )
		{
			BRDebug.Log ("PolicyTest >> PartyChatMessageCmd");

            // PartyChatMessageRes
			InitializeMsgData (BR.Message.Game.PartyChatMessageRes.MID);
			BR.Message.Game.PartyChatMessageRes.BuildMessageBuffer(m_BinWriter, ref m_Header, 0 );
			m_Connection.MessageProcessor.AddTestMessage (new PartyChatMessageRes(), ref m_Header, GetReader ());


			var thread = new Thread(()=> 
			{
				for (uint i = 1; i < 8; ++i)
				{
                    // PartyChatMessageS2CEvt
					InitializeMsgData (BR.Message.Game.PartyChatMessageS2CEvt.MID);
					BR.Message.Game.PartyChatMessageS2CEvt.BuildMessageBuffer(m_BinWriter, ref m_Header, i, "test", InChatMessage );
					m_Connection.MessageProcessor.AddTestMessage (new PartyChatMessageS2CEvt(), ref m_Header, GetReader ());
					Thread.Sleep(1000);
				}
			});
			thread.Start ();
		}

        public void JoinGameCmd(System.UInt64 InPlayerID, System.UInt64 InTicket, System.UInt64 InInsUID)
        {
            const int NumPlayerToTest = 7;

            BRDebug.Log("PolicyTest >> JoinGameCmd");

            // JoinGameRes
            InitializeMsgData(BR.Message.Game.JoinGameRes.MID);

            BinaryMemoryWriter chatHistory = new BinaryMemoryWriter(new byte[9]);
            chatHistory.Write((UInt16)GameLogType.GameStateChange);
            chatHistory.Write((UInt16)9);
            chatHistory.Write((uint)0);
            //chatHistory.Write((byte)GameStateID.FirstNightVote);
            chatHistory.Write((byte)GameStateID.FreeDebate);

            BinaryMemoryWriter gameLogData = new BinaryMemoryWriter(new byte[9]);
            gameLogData.Write((UInt16)GameLogType.GameStateChange);
            gameLogData.Write((UInt16)9);
            gameLogData.Write((uint)0);
            //gameLogData.Write((byte)GameStateID.FirstNightVote);
            gameLogData.Write((byte)GameStateID.NightVote);

            int characterVariation = rand.Next(100);

            BR.Message.Game.JoinGameRes.BuildMessageBuffer(m_BinWriter, ref m_Header, 0, 1, TimeUTC.Now(), GameStateID.None, 1, 8, 0, (byte)characterVariation, PlayerRole.Villager, 0, chatHistory.Buffer, gameLogData.Buffer);
            m_Connection.MessageProcessor.AddTestMessage(new JoinGameRes(), ref m_Header, GetReader());

            // PlayerJoinedS2CEvt
            for (int iPlayer = 0; iPlayer < NumPlayerToTest; iPlayer++)
            {
                InitializeMsgData(BR.Message.Game.PlayerJoinedS2CEvt.MID);
                var playerInfo = new PlayerInformation();
                playerInfo.NickName = "friend" + iPlayer;
                playerInfo.PlayerID = 2 + (uint)iPlayer;
                BR.Message.Game.PlayerJoinedS2CEvt.BuildMessageBuffer(m_BinWriter, ref m_Header, 0, ref playerInfo, PlayerRole.Villager, 0, (byte)(iPlayer + 1), (byte)(characterVariation+iPlayer+1));
                m_Connection.MessageProcessor.AddTestMessage(new PlayerJoinedS2CEvt(), ref m_Header, GetReader());
            }

        }

        public void LeaveGameCmd(System.UInt64 InGameInsUID, System.UInt64 InPlayerID, System.UInt64 InTicket)
        {
            BRDebug.Log("PolicyTest >> LeaveGameCmd");

            // LeaveGameRes
            InitializeMsgData(BR.Message.Game.LeaveGameRes.MID);
            BR.Message.Game.LeaveGameRes.BuildMessageBuffer(m_BinWriter, ref m_Header, 0);
            m_Connection.MessageProcessor.AddTestMessage(new LeaveGameRes(), ref m_Header, GetReader());
        }

        // Cmd: Kick player
        public void KickPlayerCmd(System.UInt64 InGameInsUID, System.UInt64 InPlayerID, System.UInt64 InPlayerToKick)
        {
            BRDebug.Log("PolicyTest >> KickPlayerCmd");

            // KickPlayerRes
            InitializeMsgData(BR.Message.Game.KickPlayerRes.MID);
            BR.Message.Game.KickPlayerRes.BuildMessageBuffer(m_BinWriter, ref m_Header, 0);
            m_Connection.MessageProcessor.AddTestMessage(new KickPlayerRes(), ref m_Header, GetReader());
        }

        // Cmd: Assign role
        public void AssignRoleCmd(System.UInt64 InGameInsUID, System.UInt64 InPlayerID, System.UInt64 InTicket)
        {
            BRDebug.Log("PolicyTest >> AssignRoleCmd");

            // AssignRoleRes
            InitializeMsgData(BR.Message.Game.AssignRoleRes.MID);
            BR.Message.Game.AssignRoleRes.BuildMessageBuffer(m_BinWriter, ref m_Header, 0);
            m_Connection.MessageProcessor.AddTestMessage(new AssignRoleRes(), ref m_Header, GetReader());
        }

        // Cmd: Chat Message
        public void ChatMessageCmd(string InChatMessage, BR.PlayerRole InRole)
        {
            BRDebug.Log("PolicyTest >> ChatMessageCmd");

            // ChatMessageRes
            InitializeMsgData(BR.Message.Game.ChatMessageRes.MID);
            BR.Message.Game.ChatMessageRes.BuildMessageBuffer(m_BinWriter, ref m_Header, 0);
            m_Connection.MessageProcessor.AddTestMessage(new ChatMessageRes(), ref m_Header, GetReader());

            InitializeMsgData(BR.Message.Game.ChatMessageS2CEvt.MID);
            BR.Message.Game.ChatMessageS2CEvt.BuildMessageBuffer(m_BinWriter, ref m_Header, 1, InRole, "aa", InChatMessage);
            m_Connection.MessageProcessor.AddTestMessage(new ChatMessageS2CEvt(), ref m_Header, GetReader());
        }

        public void VoteGameAdvanceCmd(System.UInt64 InGameInsUID, System.UInt64 InPlayerID, System.UInt64 InTicket)
        {
            BRDebug.Log("PolicyTest >> VoteGameAvdanceCmd");

            // ChatMessageRes
            InitializeMsgData(BR.Message.Game.VoteGameAdvanceRes.MID);
            BR.Message.Game.VoteGameAdvanceRes.BuildMessageBuffer(m_BinWriter, ref m_Header, 0);
            m_Connection.MessageProcessor.AddTestMessage(new VoteGameAdvanceRes(), ref m_Header, GetReader());

            TestGameAdvence();
        }

        private GameStateID[] testGameProgress = { 
                                                     GameStateID.FreeDebate,
                                                     GameStateID.NightVote, 

                                                     GameStateID.MorningDebate, 
                                                     GameStateID.FreeDebate, 
                                                     GameStateID.VoteForSuspects,
                                                     GameStateID.DefenceOfSuspects,
                                                     GameStateID.VoteForHanging,
                                                     GameStateID.NightVote,

                                                     GameStateID.MorningDebate,
                                                     GameStateID.FreeDebate,
                                                     GameStateID.VoteForSuspects,
                                                     GameStateID.DefenceOfSuspects,
                                                     GameStateID.VoteForHanging,
                                                     GameStateID.NightVote,

                                                     GameStateID.MorningDebate,
                                                     GameStateID.VoteForSuspects,
                                                     GameStateID.DefenceOfSuspects,
                                                     GameStateID.VoteForHanging,
                                                     GameStateID.End
                                                 };


        private int testGameProgressStep = 0;
        private byte gameDay = 1;
        private int testVotedCount = 0;
        private void TestGameAdvence()
        {
            ++testVotedCount;

            InitializeMsgData(BR.Message.Game.GameAdvanceVotedS2CEvt.MID);
            BR.Message.Game.GameAdvanceVotedS2CEvt.BuildMessageBuffer(m_BinWriter, ref m_Header, 1, (ulong)testVotedCount);
            m_Connection.MessageProcessor.AddTestMessage(new GameAdvanceVotedS2CEvt(), ref m_Header, GetReader());

#if DEBUG_VOTE_COUNT
            if(testVotedCount < 5)
            {              
                return;
            }
#endif
            testVotedCount = 0;

            var curDay = gameDay;
            var curState = testGameProgress[testGameProgressStep];

            testGameProgressStep++;
            testGameProgressStep %= testGameProgress.Length;

            switch (curState)
            {
                case GameStateID.NightVote:
                    {
                        ++gameDay;
                    } break;
                case GameStateID.End:
                    {
                        gameDay = 1;
                    } break;
            }

            // GameAdvancedS2CEvt
            InitializeMsgData(BR.Message.Game.GameAdvancedS2CEvt.MID);
            BR.Message.Game.GameAdvancedS2CEvt.BuildMessageBuffer(m_BinWriter, ref m_Header, 1, TimeUTC.Now(), curState, curDay);
            m_Connection.MessageProcessor.AddTestMessage(new GameAdvancedS2CEvt(), ref m_Header, GetReader());
        }

        // Cmd: Advance game
        public void AdvanceGameCmd(System.UInt64 InGameInsUID, System.UInt64 InPlayerID, System.UInt64 InTicket)
        {
            BRDebug.Log("PolicyTest >> AdvanceGameCmd");

            // AdvanceGameRes
            InitializeMsgData(BR.Message.Game.AdvanceGameRes.MID);
            BR.Message.Game.AdvanceGameRes.BuildMessageBuffer(m_BinWriter, ref m_Header, 0);
            m_Connection.MessageProcessor.AddTestMessage(new AdvanceGameRes(), ref m_Header, GetReader());

            TestGameAdvence();
        }

        // Cmd: Vote
        public void VoteCmd(System.UInt64 InGameInsUID, System.UInt64 InPlayerID, System.UInt64 InTicket, System.UInt64 InVoteTarget, System.UInt32 InActionSerial)
        {
            BRDebug.Log("PolicyTest >> VoteCmd");

            // VoteRes
            InitializeMsgData(BR.Message.Game.VoteRes.MID);
            BR.Message.Game.VoteRes.BuildMessageBuffer(m_BinWriter, ref m_Header, 0);
            m_Connection.MessageProcessor.AddTestMessage(new VoteRes(), ref m_Header, GetReader());

            // Event VotedS2CEvt
            InitializeMsgData(BR.Message.Game.VotedS2CEvt.MID);
            BR.Message.Game.VotedS2CEvt.BuildMessageBuffer(m_BinWriter, ref m_Header, 1, InPlayerID, InVoteTarget);
            m_Connection.MessageProcessor.AddTestMessage(new VotedS2CEvt(), ref m_Header, GetReader());

            // Event VoteEndS2CEvt
            InitializeMsgData(BR.Message.Game.VoteEndS2CEvt.MID);
            BR.Message.Game.VoteEndS2CEvt.BuildMessageBuffer(m_BinWriter, ref m_Header, 1, new ulong[] { InVoteTarget, 1 });
            m_Connection.MessageProcessor.AddTestMessage(new VoteEndS2CEvt(), ref m_Header, GetReader());

            switch (BRGame.Instance.MyPlayerInfo.GameInfo.Role)
            {
                case PlayerRole.Werewolf:
                    {
                        InitializeMsgData(BR.Message.Game.PlayerKilledS2CEvt.MID);
                        BR.Message.Game.PlayerKilledS2CEvt.BuildMessageBuffer(m_BinWriter, ref m_Header, 1, InVoteTarget, PlayerKilledReason.ByWerewolf);
                        m_Connection.MessageProcessor.AddTestMessage(new PlayerKilledS2CEvt(), ref m_Header, GetReader());
                    }break;
                case PlayerRole.Seer:
                    {
                        InitializeMsgData(BR.Message.Game.PlayerRevealedS2CEvt.MID);
                        BR.Message.Game.PlayerRevealedS2CEvt.BuildMessageBuffer(m_BinWriter, ref m_Header, 1, InVoteTarget, PlayerRole.Werewolf, PlayerRevealedReason.SeersChoice);
                        m_Connection.MessageProcessor.AddTestMessage(new PlayerRevealedS2CEvt(), ref m_Header, GetReader());
                    }break;
                case PlayerRole.Villager:
                    {
                        InitializeMsgData(BR.Message.Game.PlayerKilledS2CEvt.MID);
                        BR.Message.Game.PlayerKilledS2CEvt.BuildMessageBuffer(m_BinWriter, ref m_Header, 1, InVoteTarget, PlayerKilledReason.ByHanging);
                        m_Connection.MessageProcessor.AddTestMessage(new PlayerKilledS2CEvt(), ref m_Header, GetReader());
                    }break;
            }

            //// GameAdvancedS2CEvt
            //InitializeMsgData(BR.Message.Game.GameAdvancedS2CEvt.MID);
            //BR.Message.Game.GameAdvancedS2CEvt.BuildMessageBuffer(m_BinWriter, ref m_Header, 1, GameStateID.FirstNightVote, 2);
            //m_Connection.MessageProcessor.AddTestMessage(new GameAdvancedS2CEvt(), ref m_Header, GetReader());

            // GameAdvancedS2CEvt
            //InitializeMsgData(BR.Message.Game.GameAdvancedS2CEvt.MID);
            //BR.Message.Game.GameAdvancedS2CEvt.BuildMessageBuffer(m_BinWriter, ref m_Header, 1, TimeUTC.Now(), GameStateID.FreeDebate, 2);
            //m_Connection.MessageProcessor.AddTestMessage(new GameAdvancedS2CEvt(), ref m_Header, GetReader());
        }

		public void  RequestGameMatchCmd( System.Byte InNumPlayer )
		{
            const int NumPlayerToTest = 8;
			BRDebug.Log ("PolicyTest >> RequestGameMatchCmd");

            // RequestGameMatchRes
			InitializeMsgData (BR.Message.Game.RequestGameMatchRes.MID);
			BR.Message.Game.RequestGameMatchRes.BuildMessageBuffer(m_BinWriter, ref m_Header, 0 );
			m_Connection.MessageProcessor.AddTestMessage (new RequestGameMatchRes(), ref m_Header, GetReader ());


            InitializeMsgData(BR.Message.Game.GameMatchedS2CEvt.MID);

            BinaryMemoryWriter chatHistory = new BinaryMemoryWriter(new byte[9]);
            chatHistory.Write((UInt16)GameLogType.GameStateChange);
            chatHistory.Write((UInt16)9);
            chatHistory.Write((uint)0);
            //chatHistory.Write((byte)GameStateID.FirstNightVote);
            chatHistory.Write((byte)GameStateID.NightVote);

            BinaryMemoryWriter gameLogData = new BinaryMemoryWriter(new byte[9]);
            gameLogData.Write((UInt16)GameLogType.GameStateChange);
            gameLogData.Write((UInt16)9);
            gameLogData.Write((uint)0);
            //gameLogData.Write((byte)GameStateID.FirstNightVote);
            gameLogData.Write((byte)GameStateID.NightVote);

            int characterVariation = 1;

            BR.Message.Game.GameMatchedS2CEvt.BuildMessageBuffer(m_BinWriter, ref m_Header, 1, TimeUTC.Now(), GameStateID.None, 1, 8, 0, (byte)characterVariation, PlayerRole.None, 0, chatHistory.Buffer, gameLogData.Buffer);
            m_Connection.MessageProcessor.AddTestMessage(new GameMatchedS2CEvt(), ref m_Header, GetReader());

            // PlayerJoinedS2CEvt
            for (int iPlayer = 0; iPlayer < NumPlayerToTest; iPlayer++)
            {
                InitializeMsgData(BR.Message.Game.PlayerJoinedS2CEvt.MID);
                var playerInfo = new PlayerInformation();
                playerInfo.NickName = "friend" + iPlayer;
                playerInfo.PlayerID = 1 + (uint)iPlayer;

                PlayerRole role = PlayerRole.Villager;

                byte is_dead = 0;

                switch(iPlayer)
                {
                    case 6:
                        {
                            role = PlayerRole.Werewolf;
                        }break;
                }

                BR.Message.Game.PlayerJoinedS2CEvt.BuildMessageBuffer(m_BinWriter, ref m_Header, 1, ref playerInfo, role, is_dead, (byte)iPlayer, (byte)(characterVariation + iPlayer + 1));
                m_Connection.MessageProcessor.AddTestMessage(new PlayerJoinedS2CEvt(), ref m_Header, GetReader());
            }

            InitializeMsgData(BR.Message.Game.RoleAssignedS2CEvt.MID);
            BR.Message.Game.RoleAssignedS2CEvt.BuildMessageBuffer(m_BinWriter, ref m_Header, 1, 1, PlayerRole.Seer);
            m_Connection.MessageProcessor.AddTestMessage(new RoleAssignedS2CEvt(), ref m_Header, GetReader());
		}

		public void  CancelGameMatchCmd(  )
		{
			BRDebug.Log ("PolicyTest >> CancelGameMatchCmd");

            // CancelGameMatchRes
			InitializeMsgData (BR.Message.Game.CancelGameMatchRes.MID);
			BR.Message.Game.CancelGameMatchRes.BuildMessageBuffer(m_BinWriter, ref m_Header, 0 );
			m_Connection.MessageProcessor.AddTestMessage (new CancelGameMatchRes(), ref m_Header, GetReader ());

            //// GameMatchingCanceledS2CEvt
            //InitializeMsgData(BR.Message.Game.GameMatchingCanceledS2CEvt.MID);
            //BR.Message.Game.GameMatchingCanceledS2CEvt.BuildMessageBuffer(m_BinWriter, ref m_Header);
            //m_Connection.MessageProcessor.AddTestMessage(new GameMatchingCanceledS2CEvt(), ref m_Header, GetReader());
		}

        // Cmd: Set Game
        public void GetUserGamePlayerInfoCmd()
        {
            BRDebug.Log("PolicyTest >> GetUserGamePlayerInfoCmd");

            // CancelGameMatchRes
            InitializeMsgData(BR.Message.Game.GetUserGamePlayerInfoRes.MID);
            BR.Message.Game.GetUserGamePlayerInfoRes.BuildMessageBuffer(m_BinWriter, ref m_Header, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
            m_Connection.MessageProcessor.AddTestMessage(new GetUserGamePlayerInfoRes(), ref m_Header, GetReader());

        } // public void  GetUserGamePlayerInfoCmd(  )

        // Cmd: Buy Shop Item
        public void BuyShopItemCmd(System.UInt32 InShopItemID, string InParamString)
        {
            BRDebug.Log("PolicyTest >> BuyShopItemCmd");

            // BuyShopItemRes
            InitializeMsgData(BR.Message.Game.BuyShopItemRes.MID);
            BR.Message.Game.BuyShopItemRes.BuildMessageBuffer(m_BinWriter, ref m_Header, 0, InShopItemID);
            m_Connection.MessageProcessor.AddTestMessage(new BuyShopItemRes(), ref m_Header, GetReader());
        }

        // Cmd: SetPresetGameConfigIDCmd
        public void SetPresetGameConfigIDCmd(System.UInt32 InPresetID)
        {
            BRDebug.Log("PolicyTest >> SetPresetGameConfigIDCmd");

            // SetPresetGameConfigIDRes
            InitializeMsgData(BR.Message.Game.SetPresetGameConfigIDRes.MID);
            BR.Message.Game.SetPresetGameConfigIDRes.BuildMessageBuffer(m_BinWriter, ref m_Header, 0);
            m_Connection.MessageProcessor.AddTestMessage(new SetPresetGameConfigIDRes(), ref m_Header, GetReader());
        } // public void  SetPresetGameConfigIDCmd( System.UInt32 InPresetID )

        // Cmd: GainGameResourceCmd
        public void GainGameResourceCmd(System.Int32 InResource, System.Int32 InValue)
        {
            BRDebug.Log("PolicyTest >> GainGameResourceCmd");

            // SetPresetGameConfigIDRes
            InitializeMsgData(BR.Message.Game.GainGameResourceRes.MID);
            BR.Message.Game.GainGameResourceRes.BuildMessageBuffer(m_BinWriter, ref m_Header, 0);
            m_Connection.MessageProcessor.AddTestMessage(new GainGameResourceRes(), ref m_Header, GetReader());
        } // public void  GainGameResourceCmd( System.Int32 InResource, System.Int32 InValue )

		public void  GetRankingListCmd( BR.RankingType InRankingType, System.Byte InBaseRanking, System.Byte InCount )
        {
            InitializeMsgData(BR.Message.Game.GetRankingListCmd.MID);
            BR.Message.Game.GetRankingListCmd.BuildMessageBuffer(m_BinWriter, ref m_Header, InRankingType, InBaseRanking, InCount);
            m_Connection.MessageProcessor.AddTestMessage(new GetRankingListCmd(), ref m_Header, GetReader());
        }

		public void  GetGamePlayerInfoCmd( System.UInt64 InPlayerID )
        {
            InitializeMsgData(BR.Message.Game.GetGamePlayerInfoCmd.MID);
            BR.Message.Game.GetGamePlayerInfoCmd.BuildMessageBuffer(m_BinWriter, ref m_Header, InPlayerID);
            m_Connection.MessageProcessor.AddTestMessage(new GetGamePlayerInfoCmd(), ref m_Header, GetReader());
        }
	}
}
