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
    // My game info and ranking
    public class MyInformationManager : GameSystemComponent
    {
        public string ID { get; private set; }
        public string Password { get; private set; }
        public ulong PlayerID { get; private set; }
        public ulong Ticket { get; private set; }

        public UInt64 Exp;

        public UInt64 GameMoney { get; private set; }
        public UInt64 Gem { get; private set; }
        public int Stamina { get; private set; }

        // total played game count
        public int TotalPlayed { get; private set; }

        public struct GameRecord
        {
            public int Win;
            public int Lose;
        };

        GameRecord m_GameRecordSC;
        GameRecord m_GameRecordSM;
        GameRecord m_GameRecordNC;
        GameRecord m_GameRecordNM;
        GameRecord m_WeeklyRecord;

        public GameRecord GameRecordSC { get { return m_GameRecordSC; } }
        public GameRecord GameRecordSM { get { return m_GameRecordSM; } }
        public GameRecord GameRecordNC { get { return m_GameRecordNC; } }
        public GameRecord GameRecordNM { get { return m_GameRecordNM; } }
        public GameRecord WeeklyRecord { get { return m_WeeklyRecord; } }

        #region Commands

        public void GetMyGamePlayerInfo()
        {
            if( Game == null || Game.Net == null )
                return;

            Game.Net.GamePolicy.GetUserGamePlayerInfoCmd();
        }

        #endregion

        public MyInformationManager(BRGame game)
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
            Game.Net.RegisterMessageHandler(Message.Login.LoginRes.MID, OnLoginRes);
            Game.Net.RegisterMessageHandler(Message.Login.LoginByFacebookRes.MID, OnFacebookLoginRes);

            Game.Net.RegisterMessageHandler(JoinGameServerRes.MID, OnJoinGameServerRes);
            Game.Net.RegisterMessageHandler(GetUserGamePlayerInfoRes.MID, OnGetUserGamePlayerInfoRes);
            Game.Net.RegisterMessageHandler(LevelUpS2CEvt.MID, OnLevelUpS2CEvt);
            Game.Net.RegisterMessageHandler(GameEndedS2CEvt.MID, OnGameEndedS2CEvt);
        }



        public void SetMyPlayer(string userID, string password)
        {
            ID = userID;
            Password = password;
        }


        #region Message handlers

        private int OnLoginRes(MessageBase msg)
        {
            Message.Login.LoginRes msgRes = (Message.Login.LoginRes)msg;

            if (msgRes.Result < 0)
            {
                return msgRes.Result;
            }

            PlayerID = msgRes.AccID;
            Ticket = msgRes.Ticket;

            return msgRes.Result;
        }

        private int OnFacebookLoginRes(MessageBase msg)
        {
            Message.Login.LoginByFacebookRes msgRes = (Message.Login.LoginByFacebookRes)msg;

            if (msgRes.Result < 0)
            {
                return msgRes.Result;
            }

            PlayerID = msgRes.AccID;
            Ticket = msgRes.Ticket;

            return msgRes.Result;
        }

        private int OnJoinGameServerRes(MessageBase msg)
        {
            JoinGameServerRes msgRes = (JoinGameServerRes)msg;

            if (msgRes.Result < 0)
            {
                return msgRes.Result;
            }

            // request my information
            GetMyGamePlayerInfo();

            return msgRes.Result;
        }

        private int OnGetUserGamePlayerInfoRes(MessageBase msg)
        {
            GetUserGamePlayerInfoRes msgRes = (GetUserGamePlayerInfoRes)msg;

            if (msgRes.Result < 0)
            {
                return msgRes.Result;
            }

            Game.MyPlayerInfo.Level = (uint)msgRes.Level;
            Exp = (UInt64)msgRes.Exp;
            GameMoney = (UInt64)msgRes.GameMoney;
            Gem = (UInt64)msgRes.Gem;
            Stamina = msgRes.Stamina;

            TotalPlayed = msgRes.TotalPlayed;
            m_GameRecordSC.Win = msgRes.WinPlaySC;
            m_GameRecordSC.Lose = msgRes.LosePlaySC;
            m_GameRecordSM.Win = msgRes.WinPlaySM;
            m_GameRecordSM.Lose = msgRes.LosePlaySM;

            m_GameRecordNC.Win = msgRes.WinPlayNC;
            m_GameRecordNC.Lose = msgRes.LosePlayNC;
            m_GameRecordNM.Win = msgRes.WinPlayNM;
            m_GameRecordNM.Lose = msgRes.LosePlayNM;

            m_WeeklyRecord.Win = msgRes.WeeklyWin;
            m_WeeklyRecord.Lose = msgRes.WeeklyLose;

            return msgRes.Result;
        }

        private int OnLevelUpS2CEvt(MessageBase msg)
        {
            LevelUpS2CEvt msgEvt = (LevelUpS2CEvt)msg;

            Game.MyPlayerInfo.Level = msgEvt.CurrentLevel;
            Exp = msgEvt.CurrentTotalExp;

            return 0;
        }

        private int OnGameEndedS2CEvt(MessageBase msg)
        {
            GameEndedS2CEvt msgEvt = (GameEndedS2CEvt)msg;

            Exp = msgEvt.GainedExp;
            GameMoney = msgEvt.GainedGameMoney;

            //m_GameRecordNC.Win

            return 0;
        }

        #endregion
    }
} // namespace BR.GameSystem
