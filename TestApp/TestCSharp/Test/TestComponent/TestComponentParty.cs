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
    public class TestComponentParty : TestComponent
    {
        public TestComponentParty(TestBase testBase)
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
                if (command == "/createparty")
                {
                    Game.PartyManager.CreateParty();
                    return true;
                }
                else if (command == "/partyplayerlist")
                {
                    PrintMessage(string.Format("{0} players are in this party", Game.PartyManager.Count));
                    foreach (var itPlayer in Game.PartyManager)
                    {
                        //PrintMessage(string.Format("{0}:{1}, {2}", itPlayer.NickName, itPlayer.FacebookUID, itPlayer.UserIconURI));
                    }
                    return true;
                }
                else if (command == "/pinviteingame")
                {
                    UInt64 targetPlayer = 0;
                    if (parsed.Length >= 2)
                    {
                        targetPlayer = Game.PlayerManager.NickNameToID(parsed[1]);
                        if (targetPlayer != 0)
                        {
                            Game.PartyManager.Invite(targetPlayer);
                        }
                    }
                    else
                    {
                        PrintMessage("Invalid invite player: Can't find player name");
                    }
                    return true;
                }
                else if (command == "/pinviteinfriend")
                {
                    UInt64 targetPlayer = 0;
                    if (parsed.Length >= 2)
                    {
                        targetPlayer = Game.FriendManager.NickNameToID(parsed[1]);
                        if (targetPlayer != 0)
                        {
                            Game.PartyManager.Invite(targetPlayer);
                        }
                    }
                    else
                    {
                        PrintMessage("Invalid invite player in friendlist: Can't find player name");
                    }
                    return true;
                }
                else if (command == "/kickpartyplayer")
                {
                    UInt64 targetPlayer = 0;
                    if (parsed.Length >= 2)
                    {
                        targetPlayer = Game.PartyManager.NickNameToID(parsed[1]);
                        if (targetPlayer != 0)
                        {
                            Game.PartyManager.PlayerKick(targetPlayer);
                        }
                    }
                    else
                    {
                        PrintMessage("Invalid kickpartyplayer: Can't find player name");
                    }
                    return true;
                }
                else if (command == "/leaveparty")
                {
                    Game.PartyManager.LeaveParty();
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

        }


    }
}
