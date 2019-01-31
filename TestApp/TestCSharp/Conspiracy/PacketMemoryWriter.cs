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
    // Unpacked packet header
    public class PacketMemoryWriter : BR.BinaryMemoryWriter
    {
        public PacketMemoryWriter(byte[] memBuffer)
            : base(memBuffer)
        {
        }

        public void Write(GameID gameID)
        {
            Write((UInt16)gameID);
        }

        public void Write(ref PlayerInformation[] values)
        {
            Write((UInt16)values.Length);
            foreach (PlayerInformation value in values)
            {
                Write(value);
            }
        }

        public void Write(ref FriendInformation[] values)
        {
            Write((UInt16)values.Length);
            foreach (FriendInformation value in values)
            {
                FriendInformation temp = value;
                Write(ref temp);
            }
        }

        public void Write(ref RankingPlayerInformation[] values)
        {
            Write((UInt16)values.Length);
            foreach (RankingPlayerInformation value in values)
            {
                RankingPlayerInformation temp = value;
                Write(ref temp);
            }
        }

        public void Write(ref TotalRankingPlayerInformation[] values)
        {
            Write((UInt16)values.Length);
            foreach (TotalRankingPlayerInformation value in values)
            {
                TotalRankingPlayerInformation temp = value;
                Write(ref temp);
            }
        }

        public void Write(PlayerInformation value)
        {
            Write(value.PlayerID);
            Write(value.FacebookUID);
            Write(value.NickName, PlayerInformation.MAX_NAME);
            Write(value.LastActiveTime);
            Write(value.Level);
            Write((UInt32)(value.IsPlayingGame ? 1 : 0));
        }

        public void Write(RankingPlayerInformation value)
        {
            PlayerInformation playerInfo = value;
            Write(playerInfo);
            Write(value.WeeklyWin);
            Write(value.WeeklyLose);
        }

        public void Write(TotalRankingPlayerInformation value)
        {
            Write(value.RankingID);
            Write(value.Ranking);
            Write(value.PlayerID);
            Write(value.FacebookUID);
            Write(value.NickName, TotalRankingPlayerInformation.MAX_NAME);
            Write(value.Level);
            Write(value.Win);
            Write(value.Lose);
        }

        public void Write(FriendInformation value)
        {
            RankingPlayerInformation playerInfo = value;
            Write(playerInfo);
            Write(value.StaminaTime);
        }

        public void Write(ref PlayerInformation value)
        {
            Write(value);
        }

        public void Write(ref RankingPlayerInformation value)
        {
            Write(value);
        }

        public void Write(ref FriendInformation value)
        {
            Write(value);
        }

        public void Write(ref TotalRankingPlayerInformation value)
        {
            Write(value);
        }


        public void Write(GameStateID value)
        {
            Write((Byte)value);
        }

        public void Write(PlayerRole value)
        {
            Write((Byte)value);
        }

        public void Write(GameWinner value)
        {
            Write((Byte)value);
        }

        public void Write(PlayerKilledReason value)
        {
            Write((Byte)value);
        }

        public void Write(PlayerRevealedReason value)
        {
            Write((Byte)value);
        }

        public void Write(ref MatchingQueueTicket value)
        {
            Write(value.QueueUID);
            Write(value.QueueItemID);
        }

        public void Write(RankingType value)
        {
            Write((Byte)value);
        }

    };

}
