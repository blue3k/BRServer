using System;
using BR;

using Cocos2D;
using BR.GameGraphics;

namespace BR.GameSystem
{
    public class ActorStateTumb : ActorState
    {
        public ActorStateTumb(BRGame game, FSM fsm)
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

            CCGameScene.Instance.PostActionToGameLayer(() =>
            {
                var gameLayer = Game.UIManager.GetGameLayer();
                //gameLayer.SetCharacterState(ActorFSM.Player.PlayerID, CCGameActor.ActorStates.Tumb);
            });
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