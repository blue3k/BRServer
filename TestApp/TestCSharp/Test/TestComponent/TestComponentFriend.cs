using System;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using BR.Message.Game;
using BR.Message;
using BR;
using BR.GameSystem;
using BR.Net;


namespace TestCSharp
{
    public class TestComponentFriend : TestComponent
    {
        public TestComponentFriend(TestBase testBase)
            : base(testBase)
        {
        }

        public override bool OnInput(string strInput)
        {
            if (base.OnInput(strInput)) return true;


            if (strInput[0] == '/')
            {
                string[] parsed = strInput.Split(' ');
                string command = parsed[0].ToLower();
                if (command == "/friendlist")
                {
                    PrintMessage(string.Format("{0} friends are in this room", Game.PlayerManager.Count));
                    foreach (var itPlayer in Game.FriendManager)
                    {
                        PrintMessage(string.Format("{0}:{1}", itPlayer.NickName, itPlayer.FacebookUID));
                    }
                    return true;
                }
                else if (command == "/invitefriend")
                {
                    UInt64 targetPlayer = 0;
                    if (parsed.Length >= 2)
                    {
                        targetPlayer = Game.PlayerManager.NickNameToID(parsed[1]);
                        if (targetPlayer != 0)
                        {
                            Game.FriendManager.InviteFriend(targetPlayer);
                        }
                    }
                    else
                    {
                        PrintMessage("Invalid invitefriend: Can't find player name");
                    }
                    return true;
                }
                else if (command == "/invitefriendbyemail")
                {
                    if (parsed.Length >= 2)
                    {
                        Game.FriendManager.FindFriend(parsed[1]);
                    }
                    else
                    {
                        PrintMessage("Invalid invitefriend: Can't find player name");
                    }
                    return true;
                }
                else if (command == "/removefriend")
                {
                    UInt64 targetPlayer = 0;
                    if (parsed.Length >= 2)
                    {
                        targetPlayer = Game.FriendManager.NickNameToID(parsed[1]);
                        if (targetPlayer != 0)
                        {
                            Game.FriendManager.RemoveFriend(targetPlayer);
                        }
                    }
                    else
                    {
                        PrintMessage("Invalid removefriend: Can't find player name");
                    }
                    return true;
                }
                else if (command == "/notifyyes")
                {
                    if (parsed.Length >= 2)
                    {
                        UInt32 notifyID;
                        if (UInt32.TryParse(parsed[1], out notifyID))
                        {
                            Game.NotificationManager.ProcessNotification(notifyID, true);
                        }
                    }
                    else
                    {
                        PrintMessage("Invalid acceptfriend: Can't find player name");
                    }
                    return true;
                }
                else if (command == "/notifyno")
                {
                    if (parsed.Length >= 2)
                    {
                        UInt32 notifyID;
                        if (UInt32.TryParse(parsed[1], out notifyID))
                        {
                            Game.NotificationManager.ProcessNotification(notifyID, false);
                        }
                    }
                    else
                    {
                        PrintMessage("Invalid acceptfriend: Can't find player name");
                    }
                    return true;
                }
            }

            return false;
        }

        public override void RegisterMessagehandler()
        {
            base.RegisterMessagehandler();
            //Game.Net.RegisterMessageHandler(LoginRes.MID, OnLoginRes);
            //Game.Net.RegisterMessageHandler(LoginByFacebookRes.MID, OnLoginByFacebookRes);

            Game.Net.RegisterMessageHandler(InviteFriendRes.MID, OnInviteFriendRes);
            Game.Net.RegisterMessageHandler(AcceptFriendRequestRes.MID, OnAcceptFriendRequestRes);
            Game.Net.RegisterMessageHandler(FriendRequestAcceptedS2CEvt.MID, OnFriendRequestAcceptedEvent);
            Game.Net.RegisterMessageHandler(RemoveFriendRes.MID, OnRemoveFriendRes);
            Game.Net.RegisterMessageHandler(FriendRemovedS2CEvt.MID, OnFriendRemovedEvent);
            Game.Net.RegisterMessageHandler(GetFriendListRes.MID, OnGetFriendListRes);
            Game.Net.RegisterMessageHandler(FindPlayerByEMailRes.MID, OnFindPlayerByEMailRes);
        }

        public int OnInviteFriendRes(MessageBase msg)
        {
            InviteFriendRes msgRes = (InviteFriendRes)msg;

            if (msgRes.Result < 0)
            {
                BRDebug.Error("OnInviteFriendRes Failed");
                return msgRes.Result;
            }

            PrintMessage("OnInviteFriendRes success");

            return msgRes.Result;
        }

        public int OnAcceptFriendRequestRes(MessageBase msg)
        {
            AcceptFriendRequestRes msgRes = (AcceptFriendRequestRes)msg;

            if (msgRes.Result < 0)
            {
                BRDebug.Error("OnAcceptFriendRequestRes Failed");
                return msgRes.Result;
            }

            PrintMessage(string.Format("OnAcceptFriendRequestRes {0}:{1}, {2}", msgRes.NewFriend.PlayerID, msgRes.NewFriend.NickName, msgRes.NewFriend.FacebookUID));

            return msgRes.Result;
        }

        public int OnFriendRequestAcceptedEvent(MessageBase msg)
        {
            FriendRequestAcceptedS2CEvt msgEvt = (FriendRequestAcceptedS2CEvt)msg;

            PrintMessage(string.Format("OnFriendRequestAcceptedEvent {0}:{1}, {2}", msgEvt.Accepter.PlayerID, msgEvt.Accepter.NickName, msgEvt.Accepter.FacebookUID));

            return 0;
        }

        public int OnRemoveFriendRes(MessageBase msg)
        {
            RemoveFriendRes msgRes = (RemoveFriendRes)msg;

            if (msgRes.Result < 0)
            {
                BRDebug.Error("RemoveFriendRes Failed");
                return msgRes.Result;
            }

            PrintMessage(string.Format("OnRemoveFriendRes {0} is removed", msgRes.FriendID));

            return msgRes.Result;
        }

        public int OnFriendRemovedEvent(MessageBase msg)
        {
            FriendRemovedS2CEvt msgEvt = (FriendRemovedS2CEvt)msg;

            PrintMessage(string.Format("OnFriendRemovedEvent {0} is removed", msgEvt.FriendID));

            return 0;
        }

        public int OnGetFriendListRes(MessageBase msg)
        {
            GetFriendListRes msgRes = (GetFriendListRes)msg;

            if (msgRes.Result < 0)
            {
                BRDebug.Error("GetFriendListRes Failed");
                return msgRes.Result;
            }

            PrintMessage(string.Format("OnGetFriendListRes success, Total {0} Friends", msgRes.FriendList.Length));
            foreach (var friend in Game.FriendManager)
            {
                PrintMessage(string.Format("Friend: {0}:{1}, {2} ", friend.PlayerID, friend.NickName, friend.FacebookUID));
            }

            return msgRes.Result;
        }

        public int OnFindPlayerByEMailRes(MessageBase msg)
        {
            FindPlayerByEMailRes msgRes = (FindPlayerByEMailRes)msg;

            if (msgRes.Result < 0)
            {
                BRDebug.Error("FindPlayerByEMailRes Failed");
                return msgRes.Result;
            }

            PrintMessage(string.Format("FindPlayerByEMailRes success, {0}:{1} ", msgRes.Player.NickName, msgRes.Player.PlayerID));

            Game.FriendManager.InviteFriend(msgRes.Player.PlayerID);

            return msgRes.Result;
        }
    }
}
