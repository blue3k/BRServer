////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2014 The Braves
// 
// Author : KyungKun Ko
//
// Description:   Actor state sleep class
//
////////////////////////////////////////////////////////////////////////////////

using System;
using BR;

using Cocos2D;
using BR.GameGraphics;

namespace BR.GameSystem
{
    public class ActorStateSleep : ActorState
    {
        public ActorStateSleep(BRGame game, FSM fsm)
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

            //CCGameScene.Instance.PostActionToGameLayer(() =>
            //{
            //    var gameLayer = Game.UIManager.GetGameLayer();
            //    gameLayer.SetCharacterState(ActorFSM.Player.PlayerID, CCGameActor.ActorStates.Sleep);
            //});

            BRDebug.DebugLog("ActorStateSleep.OnEnter!");
        }

        public override void OnLeave()
        {
            base.OnLeave();

            BRDebug.DebugLog("ActorStateSleep.OnLeave!");
        }

        public override void UpdateState()
        {

        }
    }
}