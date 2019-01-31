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
    public class ActorStateNone : ActorState
    {
        public ActorStateNone(BRGame game, FSM fsm)
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

            BRDebug.DebugLog("ActorStateNone.OnEnter!");
        }

        public override void OnLeave()
        {
            base.OnLeave();

            BRDebug.DebugLog("ActorStateNone.OnLeave!");
        }

        public override void UpdateState()
        {

        }
    }
}