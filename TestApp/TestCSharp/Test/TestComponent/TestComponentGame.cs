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
    public class TestComponentGame : TestComponent
    {
        public TestComponentGame(TestBase testBase)
            :base(testBase)
        {
        }

        public override bool OnInput(string strInput)
        {
            if (base.OnInput(strInput)) return true;

            if (strInput[0] == '/')
            {
                string[] parsed = strInput.Split(' ');
                string command = parsed[0].ToLower();
                if (command == "/gamematching")
                {
                    if (parsed.Length >= 2)
                    {
                        int numGameMember;
                        if (int.TryParse(parsed[1], out numGameMember))
                        {
                            Game.GameManager.RequestGameMatch(numGameMember);
                        }
                    }
                    else
                    {
                        PrintMessage("Invalid acceptfriend: Can't find player name");
                    }
                    return true;
                }
                else if (command == "/cancelmatching")
                {
                    Game.GameManager.CancelGameMatch();
                    return true;
                }
                else if (command == "/leavegame")
                {
                    Game.GameManager.LeaveGame();
                    return true;
                }
                else if (command == "/r")
                {
                    Game.GameManager.SendChatMessage(strInput);
                    return true;
                }
                else if (command == "/roleassign")
                {
                    Game.GameManager.AssignRole();
                    return true;
                }
                else if (command == "/advance")
                {
                    Game.GameManager.AdvanceGame();
                    return true;
                }
                else if (command == "/myrole")
                {
                    //PrintMessage(string.Format("MyRole is {0}", Game.PlayerManager.MyPlayer.PlayerInfo.Role));
                    return true;
                }
                else if (command == "/alive")
                {
                    ListAlivePlayers();
                    return true;
                }
                else if (command == "/playerlist")
                {
                    PrintMessage(string.Format("{0} players are in this room", Game.PlayerManager.Count));
                    foreach (var itPlayer in Game.PlayerManager.Alives)
                    {
                        //PrintMessage(string.Format("Player {0} : {1}, {2}", itPlayer.NickName, itPlayer.Role, itPlayer.bIsDead ? "Dead" : ""));
                    }
                    return true;
                }
                else if (command == "/vote")
                {
                    UInt64 targetPlayer = Game.PlayerManager.NickNameToID(parsed[1]);
                    if (parsed.Length >= 2
                        && targetPlayer != 0)
                    {
                        Game.GameManager.Vote(targetPlayer);
                    }
                    else
                    {
                        PrintMessage("Invalid vote: Can't find player name");
                    }
                    return true;
                }
                else if (command == "/kick")
                {
                    UInt64 targetPlayer = 0;
                    if (parsed.Length >= 2)
                    {
                        targetPlayer = Game.PlayerManager.NickNameToID(parsed[1]);
                        if (targetPlayer != 0)
                        {
                            Game.GameManager.Kick(targetPlayer);
                        }
                        else if (parsed[1] == "all")
                        {
                            Game.GameManager.KickAll();
                        }
                    }
                    else
                    {
                        PrintMessage("Invalid kick: Can't find player name");
                    }
                    return true;
                }
                else if (command == "/h" || command == "/help")
                {
                    PrintMessage("/r chat_message => role chat");
                    PrintMessage("/vote playerName => vote");
                    PrintMessage("/roleassign => role assignment");
                    PrintMessage("/advance => advance game");
                    PrintMessage("/myrole => show my role");
                    PrintMessage("/kick playerName => kick player");
                    PrintMessage("/kick all => kick all players");
                    PrintMessage("/playerlist => list all player in this room");
                    PrintMessage("/playerlist => list all player who is alive");
                    return true;
                }
            }

            return false;
        }

        public override void RegisterMessagehandler()
        {
            base.RegisterMessagehandler();
            Game.Net.RegisterMessageHandler(ChatMessageS2CEvt.MID, OnChatMessageEvent);

            Game.Net.RegisterMessageHandler(JoinGameRes.MID, OnJoinGameRes);
            Game.Net.RegisterMessageHandler(LeaveGameRes.MID, OnLeaveGameRes);
            Game.Net.RegisterMessageHandler(GameAdvancedS2CEvt.MID, OnGameAdvancedEvent);
            Game.Net.RegisterMessageHandler(GameEndedS2CEvt.MID, OnGameEndedEvent);
            Game.Net.RegisterMessageHandler(PlayerJoinedS2CEvt.MID, OnPlayerJoinedEvent);
            Game.Net.RegisterMessageHandler(RoleAssignedS2CEvt.MID, OnRoleAssignedS2CEvt);

            Game.Net.RegisterMessageHandler(VoteRes.MID, OnVoteRes);
            Game.Net.RegisterMessageHandler(VotedS2CEvt.MID, OnVotedEvent);
            Game.Net.RegisterMessageHandler(VoteEndS2CEvt.MID, OnVoteEndEvent);

            Game.Net.RegisterMessageHandler(PlayerRevealedS2CEvt.MID, OnPlayerRevealedEvent);
            Game.Net.RegisterMessageHandler(PlayerKilledS2CEvt.MID, OnPlayerKilledEvent);

            Game.Net.RegisterMessageHandler(RequestGameMatchRes.MID, OnRequestGameMatchRes);
            Game.Net.RegisterMessageHandler(GameMatchedS2CEvt.MID, OnGameMatchedS2CEvt);
            Game.Net.RegisterMessageHandler(CancelGameMatchRes.MID, OnCancelGameMatchRes);
            Game.Net.RegisterMessageHandler(GameMatchingStartedS2CEvt.MID, OnGameMatchingStartedS2CEvt);
            Game.Net.RegisterMessageHandler(GameMatchingCanceledS2CEvt.MID, OnGameMatchingCanceledS2CEvt);


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
            //foreach (var itPlayer in Game.PlayerManager.Alives)
            //{
            //    if (!itPlayer.bIsDead)
            //        PrintMessage(string.Format("Player {0} : {1}, {2}", itPlayer.NickName, itPlayer.Role, itPlayer.bIsDead ? "Dead" : ""));
            //}
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


        public int OnLeaveGameRes(MessageBase msg)
        {
            LeaveGameRes msgRes = (LeaveGameRes)msg;

            if (msgRes.Result < 0)
            {
                BRDebug.Assert(false);
                return msgRes.Result;
            }

            return 0;
        }

        public int OnChatMessageEvent(MessageBase msg)
        {
            ChatMessageS2CEvt msgEvt = (ChatMessageS2CEvt)msg;

            if ((PlayerRole)msgEvt.Role != PlayerRole.None)
            {
                PrintMessage(string.Format("[{0}] To {1}: {2}", msgEvt.SenderName, ((PlayerRole)msgEvt.Role).ToString(), msgEvt.ChatMessage));
            }
            else
            {
                PrintMessage(string.Format("[{0}]: {1}", msgEvt.SenderName, msgEvt.ChatMessage));
            }
            return 0;
        }


        public int OnGameAdvancedEvent(MessageBase msg)
        {
            GameAdvancedS2CEvt msgEvt = (GameAdvancedS2CEvt)msg;
            GameStateID state = msgEvt.GameState;

            //PlayerInfo myInfo = Game.PlayerManager.MyPlayer.PlayerInfo;

            //switch (state)
            //{
            //    case GameStateID.FreeDebate:
            //        break;
            //    case GameStateID.FirstNightVote:
            //        PrintMessage("---------------------------------Night-------------------------------------");
            //        ListAlivePlayers();
            //        if (!myInfo.bIsDead)
            //        {
            //            switch (myInfo.Role)
            //            {
            //                case PlayerRole.Seer:
            //                    PrintMessage("Vote a person to reveal!");
            //                    break;
            //                case PlayerRole.Werewolf:
            //                    PrintMessage("Vote a person to hunt!");
            //                    break;
            //                case PlayerRole.Owlman:
            //                    PrintMessage("Vote a person to follow!");
            //                    break;
            //            }
            //        }
            //        break;
            //    case GameStateID.SecondNightVote:
            //        PrintMessage("---------------------------------Night-------------------------------------");
            //        ListAlivePlayers();
            //        if (!myInfo.bIsDead)
            //        {
            //            switch (myInfo.Role)
            //            {
            //                case PlayerRole.Seer:
            //                    PrintMessage("Vote a person to reveal!");
            //                    break;
            //                case PlayerRole.Werewolf:
            //                    PrintMessage("Vote a person to hunt!");
            //                    break;
            //                case PlayerRole.Owlman:
            //                    PrintMessage("Vote a person to follow!");
            //                    break;
            //                case PlayerRole.Bodyguard:
            //                    PrintMessage("Vote a person to guard!");
            //                    break;
            //            }
            //        }
            //        break;
            //    case GameStateID.NightVote:
            //        PrintMessage("---------------------------------Night-------------------------------------");
            //        ListAlivePlayers();
            //        if (!myInfo.bIsDead)
            //        {
            //            switch (myInfo.Role)
            //            {
            //                case PlayerRole.Seer:
            //                    PrintMessage("Vote a person to reveal!");
            //                    break;
            //                case PlayerRole.Werewolf:
            //                    PrintMessage("Vote a person to hunt!");
            //                    break;
            //                case PlayerRole.Owlman:
            //                    PrintMessage("Vote a person to follow!");
            //                    break;
            //                case PlayerRole.Bodyguard:
            //                    PrintMessage("Vote a person to guard!");
            //                    break;
            //            }
            //        }

            //        break;
            //    case GameStateID.Mythomaniac:
            //        break;
            //    case GameStateID.MorningDebate:
            //        PrintMessage("----------------------------------Day-------------------------------------");
            //        break;
            //    case GameStateID.VoteForSuspects:
            //        ListAlivePlayers();
            //        if (!myInfo.bIsDead)
            //        {
            //            PrintMessage("Vote for suspect!");
            //        }
            //        break;
            //    case GameStateID.DefenceOfSuspects:
            //        break;
            //    case GameStateID.VoteForHanging:
            //        ListAlivePlayers();
            //        if (!myInfo.bIsDead)
            //        {
            //            PrintMessage("Vote for hange!");
            //        }
            //        break;
            //}
            return 0;
        }

        public int OnGameEndedEvent(MessageBase msg)
        {
            GameEndedS2CEvt msgEvt = (GameEndedS2CEvt)msg;

            PrintMessage(string.Format("GameEnd: Winner {0}", (GameWinner)msgEvt.Winner));

            return 0;
        }

        public int OnPlayerJoinedEvent(MessageBase msg)
        {
            PlayerJoinedS2CEvt msgEvt = (PlayerJoinedS2CEvt)msg;

            PrintMessage(string.Format("Player {0}:{1} Joined", msgEvt.JoinedPlayer.PlayerID, msgEvt.JoinedPlayer.NickName));
            return 0;
        }

        public int OnRoleAssignedS2CEvt(MessageBase msg)
        {
            RoleAssignedS2CEvt msgEvt = (RoleAssignedS2CEvt)msg;

            PlayerInfo player = Game.PlayerManager.GetPlayer(msgEvt.PlayerID);
            if (player == null)
            {
                PrintMessage(string.Format("Player {0} not found", msgEvt.PlayerID));
            }
            else
            {
                PrintMessage(string.Format("Player {0} : {1}", player.NickName, msgEvt.Role));
            }
            return 0;
        }


        public int OnVoteRes(MessageBase msg)
        {
            VoteRes msgRes = (VoteRes)msg;

            if (msgRes.Result < 0)
            {
                PrintMessage(string.Format("Vote result Failed:{0}", msgRes.Result));
                return msgRes.Result;
            }
            PrintMessage(string.Format("Vote Done"));

            return 0;
        }

        public int OnVotedEvent(MessageBase msg)
        {
            VotedS2CEvt msgEvt = (VotedS2CEvt)msg;

            PrintMessage(string.Format("{0} voted {1}", Game.PlayerManager.IDToNickName(msgEvt.Voter), Game.PlayerManager.IDToNickName(msgEvt.VotedTarget)));
            return 0;
        }

        public int OnVoteEndEvent(MessageBase msg)
        {
            VoteEndS2CEvt msgEvt = (VoteEndS2CEvt)msg;

            PrintMessage("VoteEndEvt: ");
            foreach (UInt64 voted in msgEvt.Voted)
            {
                PrintMessage(string.Format("VoteRanker: {0}:{1}", voted, Game.PlayerManager.IDToNickName(voted)));
            }
            return 0;
        }

        public int OnPlayerRevealedEvent(MessageBase msg)
        {
            PlayerRevealedS2CEvt msgEvt = (PlayerRevealedS2CEvt)msg;

            string revealedPlayerName = Game.PlayerManager.IDToNickName(msgEvt.RevealedPlayerID);

            switch ((PlayerRevealedReason)msgEvt.Reason)
            {
                case PlayerRevealedReason.Werewolf:
                    PrintMessage(string.Format("Werewolf: player {0} is a werewolf", revealedPlayerName));
                    break;
                case PlayerRevealedReason.SeersChoice:
                    PrintMessage(string.Format("Seer's choice: player {0} is {1}a werewolf", revealedPlayerName, (PlayerRole)msgEvt.Role == PlayerRole.Werewolf ? "" : "not "));
                    break;
                case PlayerRevealedReason.Medium:
                    PrintMessage(string.Format("Medium's sense: player {0} is {1}a werewolf", revealedPlayerName, (PlayerRole)msgEvt.Role == PlayerRole.Werewolf ? "" : "not "));
                    break;
                case PlayerRevealedReason.GameEnd:
                    PrintMessage(string.Format("GameEnd: {0} is a {1}", revealedPlayerName, (PlayerRole)msgEvt.Role));
                    break;
                default:
                    PrintMessage(string.Format("Player revealed by unknown reason: {0}:{1}, {2}", msgEvt.RevealedPlayerID, revealedPlayerName, (PlayerRole)msgEvt.Role));
                    break;
            }
            return 0;
        }

        public int OnPlayerKilledEvent(MessageBase msg)
        {
            PlayerKilledS2CEvt msgEvt = (PlayerKilledS2CEvt)msg;

            string killedPlayerName = Game.PlayerManager.IDToNickName(msgEvt.KilledPlayer);

            switch ((PlayerKilledReason)msgEvt.Reason)
            {
                case PlayerKilledReason.BlockedByBoardGuard:
                    PrintMessage(string.Format("Werewolf's attempt to {0}:{1} is blocked by a guard", msgEvt.KilledPlayer, killedPlayerName));
                    PrintMessage("Let's start the morning debate ");
                    break;
                case PlayerKilledReason.ByWerewolf:
                    PrintMessage(string.Format("Player {0}:{1} Killed {2}", msgEvt.KilledPlayer, killedPlayerName, (PlayerKilledReason)msgEvt.Reason));
                    PrintMessage("Let's start the morning debate ㅜ.ㅜ");
                    break;
                case PlayerKilledReason.ByHanging:
                    PrintMessage(string.Format("Player {0}:{1} Killed {2}", msgEvt.KilledPlayer, killedPlayerName, (PlayerKilledReason)msgEvt.Reason));
                    PrintMessage("Ah~~ Another night is comming ~ +_+");
                    break;
            }

            return 0;
        }

        public int OnRequestGameMatchRes(MessageBase msg)
        {
            RequestGameMatchRes msgRes = (RequestGameMatchRes)msg;

            if (msgRes.Result < 0)
            {
                BRDebug.Warning(string.Format("RequestGameMatchRes failed {0}", msgRes.Result));
                return msgRes.Result;
            }

            PrintMessage("RequestGameMatchRes");

            return 0;
        }

        public int OnGameMatchedS2CEvt(MessageBase msg)
        {
            GameMatchedS2CEvt msgEvt = (GameMatchedS2CEvt)msg;

            PrintMessage(string.Format("GameMatchedS2CEvt InsUID:{0}", msgEvt.InsUID));

            return 0;
        }

        public int OnCancelGameMatchRes(MessageBase msg)
        {
            CancelGameMatchRes msgRes = (CancelGameMatchRes)msg;

            if (msgRes.Result < 0)
            {
                BRDebug.Warning(string.Format("CancelGameMatchRes failed {0}", msgRes.Result));
                return msgRes.Result;
            }

            PrintMessage("CancelGameMatchRes");

            return 0;
        }

        public int OnGameMatchingStartedS2CEvt(MessageBase msg)
        {
            GameMatchingStartedS2CEvt msgEvt = (GameMatchingStartedS2CEvt)msg;

            PrintMessage("GameMatchingStarted");

            return 0;
        }

        public int OnGameMatchingCanceledS2CEvt(MessageBase msg)
        {
            GameMatchingCanceledS2CEvt msgEvt = (GameMatchingCanceledS2CEvt)msg;

            PrintMessage(string.Format("GameMatchingCanceled"));

            return 0;
        }

    }
}
