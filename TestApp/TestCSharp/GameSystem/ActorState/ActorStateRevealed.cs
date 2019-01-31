////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2014 The Braves
// 
// Author : KyungKun Ko
//
// Description:   Actor state revealed class
//
////////////////////////////////////////////////////////////////////////////////

using System;
using BR;

using Cocos2D;
using BR.GameGraphics;

namespace BR.GameSystem
{
    public class ActorStateRevealed : ActorState
    {
        public ActorStateRevealed(BRGame game, FSM fsm)
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

            BRDebug.DebugLog("ActorStateRevealed.OnEnter!");
        }

        public override void OnLeave()
        {
            base.OnLeave();

            BRDebug.DebugLog("ActorStateRevealed.OnLeave!");
        }

        public override void UpdateState()
        {

        }
    }
}