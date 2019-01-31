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
    public class ActorStateDead : ActorState
    {
        public ActorStateDead(BRGame game, FSM fsm)
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

            BRDebug.DebugLog("ActorStateDead.OnEnter!");

            //CCGameScene.Instance.PostActionToGameLayer(() =>
            //{
            //    var gameLayer = Game.UIManager.GetGameLayer();
            //    gameLayer.SetCharacterState(ActorFSM.Player.PlayerID, CCGameActor.ActorStates.Dead);
            //});
        }

        public override void OnLeave()
        {
            base.OnLeave();

            BRDebug.Log("ActorStateDead.OnLeave!");
        }

        public override void UpdateState()
        {

        }
    }
}