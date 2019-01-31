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
    public class ActorStateRole : ActorState
    {
        public ActorStateRole(BRGame game, FSM fsm)
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

            BRDebug.DebugLog("ActorStateRole.OnEnter!");

            //CCGameScene.Instance.PostActionToGameLayer(() =>
            //{
            //    var gameLayer = Game.UIManager.GetGameLayer();
            //    gameLayer.SetCharacterState(ActorFSM.Player.PlayerID, CCGameActor.ActorStates.Role);
            //});
        }

        public override void OnLeave()
        {
            base.OnLeave();

            BRDebug.DebugLog("ActorStateRole.OnLeave!");
        }

        public override void UpdateState()
        {

        }
    }
}