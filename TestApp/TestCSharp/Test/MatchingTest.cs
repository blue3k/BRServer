using System;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using BR.Message.Game;
using BR.Message.Login;
using BR.Message;
using BR;
using BR.GameSystem;
using BR.Net;


namespace TestCSharp
{
    class MatchingTest : TestBase
    {
        public MatchingTest()
        {
            TestName = GetType().ToString();
            TestComponents.Add(new TestComponentFriend(this));
            TestComponents.Add(new TestComponentGame(this));
            TestComponents.Add(new TestComponentNotify(this));
            TestComponents.Add(new TestComponentParty(this));
        }

        public override bool OnInput(string strInput)
        {
            if (base.OnInput(strInput)) return true;

            if (strInput[0] == '/')
            {
                string[] parsed = strInput.Split(' ');
                string command = parsed[0].ToLower();
                
            }

            if (Game.GameManager.IsInGame)
            {
                Game.GameManager.SendChatMessage(strInput);
            }
            else if (Game.PartyManager.PartyUID != 0)
            {
                Game.PartyManager.SendChatMessage(strInput);
            }

            return false;
        }


        void RegisterMessagehandler()
        {
            Game.Net.RegisterMessageHandler(LoginRes.MID, OnLoginRes);
            Game.Net.RegisterMessageHandler(LoginByFacebookRes.MID, OnLoginByFacebookRes);

            Game.Net.RegisterMessageHandler(JoinGameRes.MID, OnJoinGameRes);

            Game.Net.RegisterMessageHandler(GameMatchedS2CEvt.MID, OnGameMatchedS2CEvt);

        }


        public override bool RunTest()
        {
            //Game.Net.Connection.OnConnectionEventCallBack += OnConnectionEvent;

            base.RunTest();

            RegisterMessagehandler();

            //Game.Net.Connect();

            return true;
        }

        void OnConnectionEvent(Connection connection, Connection.Event evt)
        {
            switch (evt)
            {
                case Connection.Event.Connected:
                    Game.Net.Login(ID, Password);
                    break;
            }
        }

        public int OnLoginRes(MessageBase msg)
        {
            LoginRes msgRes = (LoginRes)msg;

            if (msgRes.Result < 0)
            {
                PrintMessage(string.Format("LogIn failed: {0}", msgRes.Result));
                return msgRes.Result;
            }

            PrintMessage(string.Format("LogIn success: {0}, {0}", msgRes.AccID, msgRes.Ticket));

            //if (msgRes.GameUID != 0)
            //    Game.GameManager.JoinGame(msgRes.GameUID);
            //else
            //{
            //    Game.GameManager.RequestGameMatch(4);
            //}

            return msgRes.Result;
        }

        public int OnLoginByFacebookRes(MessageBase msg)
        {
            LoginByFacebookRes msgRes = (LoginByFacebookRes)msg;

            if (msgRes.Result < 0)
            {
                BRDebug.Error("Login Failed");
                return msgRes.Result;
            }

            //Game.Net.JoinGame();

            return msgRes.Result;
        }

        public int OnJoinGameRes(MessageBase msg)
        {
            JoinGameRes msgRes = (JoinGameRes)msg;

            if (msgRes.Result < 0)
            {
                PrintMessage(string.Format("JoinGame failed: {0}", msgRes.Result));
                return msgRes.Result;
            }

            PrintMessage("JoinGame Successed");

            return 0;
        }


        public int OnGameMatchedS2CEvt(MessageBase msg)
        {
            GameMatchedS2CEvt msgEvt = (GameMatchedS2CEvt)msg;

            //Game.GameManager.JoinGame(msgEvt.InsUID);

            return 0;
        }

    }
}
