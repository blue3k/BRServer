////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2014 The Braves
// 
// Author : KyungKun Ko
//
// Description:   Actor state dead class
//
////////////////////////////////////////////////////////////////////////////////

using System;
using BR;

using Cocos2D;
using BR.GameGraphics;

namespace BR.GameSystem
{
    public class ActorStateShowRole : ActorState
    {
        public ActorStateShowRole(BRGame game, FSM fsm)
            : base(game, fsm)
        {
        }

        public override void Awake()
        {
            base.Awake();
        }

        public override void OnEnter()
        {
            base.OnEnter();
            BRDebug.DebugLog("ActorStateShowRole.OnEnter!");
            CCGameScene.Instance.PostInitGameStateActionToGameLayer(() =>
            {
                //Game.UIManager.GetGameLayer().AddCharacter(ActorFSM.Player.PlayerID, ActorFSM.Player.GameInfo.PlayerIndex, ActorFSM.Player.GameInfo.PlayerCharacter, ActorFSM.Player.GameInfo.bIsDead);
                //if (Game.GameManager.GetCurGameState() is GameStateNightVote
                //    || Game.GameManager.GetCurGameState() is GameStateEnd)
                //{
                //    ActorFSM.SetNextState<ActorStateRole>();
                //}
                //else if (ActorFSM.Player.GameInfo.bIsDead)
                //    ActorFSM.SetNextState<ActorStateTumb>();
                //else
                //    ActorFSM.SetNextState<ActorStateIdle>();
            });            
        }

        public override void OnLeave()
        {
            base.OnLeave();

            BRDebug.DebugLog("ActorStateShowRole.OnLeave!");
            CCGameScene.Instance.PostInitGameStateActionToGameLayer(() =>
            {
            });
        }

        public override void UpdateState()
        {
        }
    }
}