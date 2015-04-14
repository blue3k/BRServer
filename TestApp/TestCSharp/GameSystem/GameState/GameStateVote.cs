////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description:   Game state first night vote class
//
////////////////////////////////////////////////////////////////////////////////

using System;
using BR;

using Cocos2D;
using BR.GameGraphics;

namespace BR.GameSystem
{
    public class GameStateVote : GameStateInGame
	{
        public static float GAME_STATE_UI_POPUP_DELAY_TURN = 2.0f;

        public GameStateVote(BRGame game, FSM fsm)
            : base(game, fsm)
        {
        }

        protected bool CheckVoted(GameVoteType type)
        {
            var logItem = Game.GameManager.GameHistory.FindVoteState(Game.GameManager.GameDay, type);
            if (null == logItem)
            {
                return false;
            }

            foreach (var voted in logItem.VoteStatus)
            {
                if (Game.MyPlayerInfo.PlayerID == voted.Voter)
                {
                    return true;
                }
            }

            return false;
        }

		public override void OnEnter()
		{
			base.OnEnter ();
        }

		public override void OnLeave()
		{
			base.OnLeave();
		}
	}
}

