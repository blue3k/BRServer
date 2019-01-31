////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description:   Actor state base class
//
////////////////////////////////////////////////////////////////////////////////

using System;
using System.Collections;
using System.Collections.Generic;
using BR;

namespace BR.GameSystem
{
    public abstract class ActorState : FSM.State
    {
        public ActorFSM ActorFSM { get { return (ActorFSM)MyFSM; } }

        public ActorState(BRGame game, FSM fsm)
            : base(game, fsm)
        {
        }

        public override void Awake()
        {
        }

        public override void OnEnter()
        {
        }

        public override void OnLeave()
        {
        }
    }
}