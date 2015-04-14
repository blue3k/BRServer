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
    class ChatTest : TestBase
    {
        public ChatTest()
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

            //if (strInput[0] == '/')
            //{
            //    string[] parsed = strInput.Split(' ');
            //    string command = parsed[0].ToLower();
            //    if (command == "/r")
            //    {
            //        Game.GameManager.SendChatMessage(strInput);
            //    }
            //    else if (command == "/roleassign")
            //    {
            //        Game.GameManager.AssignRole();
            //    }
            //    else if (command == "/advance")
            //    {
            //        Game.GameManager.AdvanceGame();
            //    }
            //    else if (command == "/myrole")
            //    {
            //        PrintMessage(string.Format("MyRole is {0}", Game.PlayerManager.MyPlayer.PlayerInfo.Role) );
            //    }
            //    else if (command == "/alive")
            //    {
            //        ListAlivePlayers();
            //    }
            //    else if (command == "/playerlist")
            //    {
            //        PrintMessage(string.Format("{0} players are in this room", Game.PlayerManager.Count));
            //        foreach (var itPlayer in Game.PlayerManager.Alives)
            //        {
            //            PrintMessage(string.Format("Player {0} : {1}, {2}", itPlayer.NickName, itPlayer.Role, itPlayer.bIsDead ? "Dead" : ""));
            //        }
            //    }
            //    else if (command == "/vote")
            //    {
            //        UInt64 targetPlayer = Game.PlayerManager.NickNameToID(parsed[1]);
            //        if( parsed.Length >= 2
            //            && targetPlayer != 0)
            //        {
            //            Game.GameManager.Vote(targetPlayer);
            //        }
            //        else
            //        {
            //            PrintMessage("Invalid vote: Can't find player name");
            //        }
            //    }
            //    else if (command == "/kick")
            //    {
            //        UInt64 targetPlayer = 0;
            //        if (parsed.Length >= 2)
            //        {
            //            targetPlayer = Game.PlayerManager.NickNameToID(parsed[1]);
            //            if (targetPlayer != 0)
            //            {
            //                Game.GameManager.Kick(targetPlayer);
            //            }
            //            else if (parsed[1] == "all")
            //            {
            //                Game.GameManager.KickAll();
            //            }
            //        }
            //        else
            //        {
            //            PrintMessage("Invalid kick: Can't find player name");
            //        }
            //    }
            //    else if (command == "/h" || command == "/help")
            //    {
            //        PrintMessage("/r chat_message => role chat");
            //        PrintMessage("/vote playerName => vote");
            //        PrintMessage("/roleassign => role assignment");
            //        PrintMessage("/advance => advance game");
            //        PrintMessage("/myrole => show my role");
            //        PrintMessage("/kick playerName => kick player");
            //        PrintMessage("/kick all => kick all players");
            //        PrintMessage("/playerlist => list all player in this room");
            //        PrintMessage("/playerlist => list all player who is alive");
            //    }
            //}
            //else
            //{
            //    Game.GameManager.SendChatMessage(strInput);
            //    return;
            //}

            if (Game.GameManager.IsInGame)
            {
                Game.GameManager.SendChatMessage(strInput);
            }
            else if (Game.PartyManager.PartyUID != 0)
            {
                Game.PartyManager.SendChatMessage(strInput);
            }

            //PrintMessage( strInput );
            return false;
        }


        void RegisterMessagehandler()
        {
            Game.Net.RegisterMessageHandler(LoginRes.MID, OnLoginRes);
            Game.Net.RegisterMessageHandler(LoginByFacebookRes.MID, OnLoginByFacebookRes);

            Game.Net.RegisterMessageHandler(RegisterGCMRes.MID, OnRegisterGCMRes);
            Game.Net.RegisterMessageHandler(UnregisterGCMRes.MID, OnUnregisterGCMRes);

        }

        public override bool RunTest()
        {
            //Game.Net.Connection.OnConnectionEventCallBack += OnConnectionEvent;
            base.RunTest();
            RegisterMessagehandler();

            //Game.Net.Connect();

            return true;
        }

        public void ListAlivePlayers()
        {
            //int iNumAlive = 0;
            //foreach (var itPlayer in Game.PlayerManager.Alives)
            //{
            //    if (!itPlayer.bIsDead)
            //        iNumAlive++;
            //}
            //PrintMessage(string.Format("{0} players are alive", iNumAlive));
            //foreach (var itPlayer in Game.PlayerManager.Deads)
            //{
            //    if (!itPlayer.bIsDead)
            //        PrintMessage(string.Format("Player {0} : {1}, {2}", itPlayer.NickName, itPlayer.Role, itPlayer.bIsDead ? "Dead" : ""));
            //}
        }

        void OnConnectionEvent( Connection connection, Connection.Event evt )
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
                PrintMessage(string.Format("LogIn failed: {0}",msgRes.Result));
                return msgRes.Result;
            }

            PrintMessage(string.Format("LogIn success: {0}, {0}", msgRes.AccID, msgRes.Ticket));

            Game.GameManager.JoinGame();

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

            //Game.GameManager.JoinGame();

            return msgRes.Result;
        }

        public int OnRegisterGCMRes(MessageBase msg)
        {
            RegisterGCMRes msgRes = (RegisterGCMRes)msg;

            if (msgRes.Result < 0)
            {
                BRDebug.Error("RegisterGCM Failed");
                return msgRes.Result;
            }

            return msgRes.Result;
        }

        public int OnUnregisterGCMRes(MessageBase msg)
        {

            UnregisterGCMRes msgRes = (UnregisterGCMRes)msg;

            if (msgRes.Result < 0)
            {
                BRDebug.Error("RegisterGCM Failed");
                return msgRes.Result;
            }

            return msgRes.Result;

        }


        //public int OnAddFriendRes(MessageBase msg)
        //{
        //    AddFriendRes msgRes = (AddFriendRes)msg;

        //    if (msgRes.Result < 0)
        //    {
        //        BRDebug.Error("AddFriendRes Failed");
        //        return msgRes.Result;
        //    }

        //    return msgRes.Result;
        //}


    }
}
