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
    public class TestComponentNotify : TestComponent
    {
        public TestComponentNotify(TestBase testBase)
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
                if (command == "/notifyyes")
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

            Game.Net.RegisterMessageHandler(GetNotificationListRes.MID, OnGetNotificationListRes);
            Game.Net.RegisterMessageHandler(DeleteNotificationRes.MID, OnDeleteNotificationRes);
            Game.Net.RegisterMessageHandler(NotifyS2CEvt.MID, OnNotifyEvent);

        }


        public int OnGetNotificationListRes(MessageBase msg)
        {
            GetNotificationListRes msgRes = (GetNotificationListRes)msg;

            if (msgRes.Result < 0)
            {
                BRDebug.Warning(string.Format("GetNotificationListRes failed {0}", msgRes.Result));
                return msgRes.Result;
            }

            return 0;
        }

        public int OnDeleteNotificationRes(MessageBase msg)
        {
            DeleteNotificationRes msgRes = (DeleteNotificationRes)msg;

            if (msgRes.Result < 0)
            {
                BRDebug.Warning(string.Format("DeleteNotificationRes failed {0}", msgRes.Result));
                return msgRes.Result;
            }

            PrintMessage(string.Format("Notification Deleted {0}", msgRes.NotificationID));

            return 0;
        }


        public int OnNotifyEvent(MessageBase msg)
        {
            NotifyS2CEvt msgEvt = (NotifyS2CEvt)msg;

            PrintMessage(string.Format("Notification {0},{1},{2},{3},{4}", msgEvt.NotificationID, (NotificationType)msgEvt.MessageID, msgEvt.MessageParam0, msgEvt.MessageParam1, msgEvt.MessageText));

            return 0;
        }

    }
}
