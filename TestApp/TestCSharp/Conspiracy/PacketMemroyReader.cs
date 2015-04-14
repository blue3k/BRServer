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
using System.Text;


namespace BR.Net
{


    // Binary memory reader
    public class PacketMemoryReader : BR.BinaryMemoryReader
    {
        public PacketMemoryReader(byte[] memBuffer)
            :base(memBuffer)
        {
        }


        public GameID ReadGameID()
        {
            return (GameID)ReadUInt16();
        }

        void ReadPlayerInformation(PlayerInformation playerInfo)
        {
            playerInfo.PlayerID = ReadUInt64();
            playerInfo.FacebookUID = ReadUInt64();
            playerInfo.NickName = ReadString(PlayerInformation.MAX_NAME);
            playerInfo.LastActiveTime = ReadUInt64();
            playerInfo.Level = ReadUInt32();
            playerInfo.IsPlayingGame = ReadInt32() != 0;
        }

        void ReadPlayerInformation(RankingPlayerInformation playerInfo)
        {
            ReadPlayerInformation((PlayerInformation)playerInfo);
            playerInfo.WeeklyWin = ReadUInt32();
            playerInfo.WeeklyLose = ReadUInt32();
        }

        public PlayerInformation ReadPlayerInformation()
        {
            PlayerInformation value = new PlayerInformation();
            ReadPlayerInformation(value);
            return value;
        }

        public RankingPlayerInformation ReadRankingPlayerInformation()
        {
            RankingPlayerInformation value = new RankingPlayerInformation();
            ReadPlayerInformation(value);
            return value;
        }

        public TotalRankingPlayerInformation ReadTotalRankingPlayerInformation()
        {
            TotalRankingPlayerInformation value = new TotalRankingPlayerInformation();
            value.RankingID = ReadUInt32();
            value.Ranking = ReadUInt32();
            value.PlayerID = ReadUInt64();
            value.FacebookUID = ReadUInt64();
            value.NickName = ReadString(TotalRankingPlayerInformation.MAX_NAME);
            value.Level = ReadUInt32();
            value.Win = ReadUInt32();
            value.Lose = ReadUInt32();

            return value;
        }

        public FriendInformation ReadFriendInformation()
        {
            FriendInformation value = new FriendInformation();
            ReadPlayerInformation((RankingPlayerInformation)value);
            value.StaminaTime = ReadUInt64();
            return value;
        }

        public GameStateID ReadGameStateID()
        {
            return (GameStateID)ReadByte();
        }

        public PlayerRole ReadPlayerRole()
        {
            return (PlayerRole)ReadByte();
        }

        public GameWinner ReadGameWinner()
        {
            return (GameWinner)ReadByte();
        }

        public PlayerKilledReason ReadPlayerKilledReason()
        {
            return (PlayerKilledReason)ReadByte();
        }

        public PlayerRevealedReason ReadPlayerRevealedReason()
        {
            return (PlayerRevealedReason)ReadByte();
        }

        public MatchingQueueTicket ReadMatchingQueueTicket()
        {
            MatchingQueueTicket ticket = default(MatchingQueueTicket);
            ticket.QueueUID = ReadUInt64();
            ticket.QueueItemID = ReadUInt64();
            return ticket;
        }

        public RankingType ReadRankingType()
        {
            return (RankingType)ReadByte();
        }

    };


}
