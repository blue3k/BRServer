using System;
using System.Net;
using BR.Message.Game;
using BR.Message;
using BR;
using BR.Net;
using System.Collections;
using System.Collections.Generic;
using Cocos2D;

namespace BR.GameSystem
{
    // ranking
    public class RankingManager : GameSystemComponent
    {
        const int MAX_RANKING = 100;

        public class RankingInformation
        {
            public uint WeeklyWin;      // Ranking between friends
            public uint WeeklyLose;
            public UInt64 RankingValue; // Ranking ordering value 
        }

        List<TotalRankingPlayerInformation> m_TotalRanking = new List<TotalRankingPlayerInformation>(MAX_RANKING);
        public List<TotalRankingPlayerInformation> TotalRanking { get { return m_TotalRanking; } }

        // Friend list
        List<PlayerInfo> m_FriendRanking = new List<PlayerInfo>(MAX_RANKING);
        public List<PlayerInfo> FriendRanking { get { return m_FriendRanking; } }


        #region Commands

        public void GetRankingList(RankingType rankingType, uint startRanking, uint numRankingToQuery)
        {
            if( Game == null || Game.Net == null )
                return;

            Game.Net.GamePolicy.GetRankingListCmd(rankingType, (byte)startRanking, (byte)numRankingToQuery);
        }

        UInt64 GetRankingValue(PlayerInfo player)
        {
            if (player == null) return 0;

            float winRate = player.RankingInfo.WeeklyLose == 0 ? 1 : (float)player.RankingInfo.WeeklyWin / (float)player.RankingInfo.WeeklyLose;

            return (((UInt64)player.RankingInfo.WeeklyWin) << 32) + (UInt64)(winRate * 10000);
        }

        public void UpdateFriendRanking()
        {
            m_FriendRanking.Clear();

            foreach (var friend in Game.FriendManager)
            {
                UInt64 currentRankValue = GetRankingValue(friend);

                friend.RankingInfo.RankingValue = currentRankValue;

                if( m_FriendRanking.Count == 0 )
                {
                    m_FriendRanking.Add(friend);
                    continue;
                }

                // binary search
                int min = 0, max = m_FriendRanking.Count - 1;
                while( min < max )
                {
                    int testIndex = (min + max) >> 1;

                    if (currentRankValue < m_FriendRanking[testIndex].RankingInfo.RankingValue && max != testIndex)
                        max = testIndex;
                    else if (currentRankValue > m_FriendRanking[testIndex].RankingInfo.RankingValue && min != testIndex)
                    {
                        min = testIndex;
                    }
                    else// if (currentRankValue == m_FriendRanking[testIndex].RankingInfo.RankingValue)
                    {
                        min = max = testIndex;
                        break;
                    }
                }

                // insert
                m_FriendRanking.Insert(min,friend);

                // crop useless one
                const int SearchMaximum = MAX_RANKING * 2;
                if (m_FriendRanking.Count > SearchMaximum)
                {
                    m_FriendRanking.RemoveRange(SearchMaximum, m_FriendRanking.Count - SearchMaximum);
                }
            }

            m_FriendRanking.RemoveRange(MAX_RANKING, m_FriendRanking.Count - MAX_RANKING);
        }

        #endregion

        public RankingManager(BRGame game)
            : base(game)
        {
        }

        public override bool Initialize()
        {
            if (!base.Initialize())
                return false;


            RegisterMessagehandler();

            return true;
        }

        private void RegisterMessagehandler()
        {
            Game.Net.RegisterMessageHandler(GetRankingListRes.MID, OnGetRankingListRes);
        }


        #region Message handlers

        private int OnGetRankingListRes(MessageBase msg)
        {
            GetRankingListRes msgRes = (GetRankingListRes)msg;

            if (msgRes.Result < 0)
            {
                return msgRes.Result;
            }


            foreach (var rankPlayer in msgRes.Ranking)
            {
                while (m_TotalRanking.Count <= rankPlayer.RankingID)
                {
                    m_TotalRanking.Add(null);
                }

                m_TotalRanking[(int)rankPlayer.RankingID] = rankPlayer;
            }

            return msgRes.Result;
        }

        //private int OnLevelUpS2CEvt(MessageBase msg)
        //{
        //    LevelUpS2CEvt msgEvt = (LevelUpS2CEvt)msg;

        //    Level = msgEvt.CurrentLevel;
        //    Exp = msgEvt.CurrentTotalExp;

        //    return 0;
        //}

        #endregion
    }
} // namespace BR.GameSystem
