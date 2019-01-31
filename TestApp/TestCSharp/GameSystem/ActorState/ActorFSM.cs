////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2014 The Braves
// 
// Author : KyungKun Ko
//
// Description:   Actor fsm class
//
////////////////////////////////////////////////////////////////////////////////

using System;
using System.Collections;
using System.Collections.Generic;
using BR;

namespace BR.GameSystem
{
    public class ActorFSM : FSM
    {
        #region Properties
        public PlayerInfo Player { get; private set; }
        #endregion


        //#region Constructors
        public ActorFSM(BRGame game, PlayerInfo player) : base()
        {
            Player = player;

            CreateState<ActorStateNone>(game);
            CreateState<ActorStateShow>(game);
            CreateState<ActorStateIdle>(game);
            CreateState<ActorStateSleep>(game);
            CreateState<ActorStateDead>(game);
            CreateState<ActorStateLevelUp>(game);
            CreateState<ActorStateRevealed>(game);
            CreateState<ActorStateRole>(game);
            CreateState<ActorStateTumb>(game);

            SetNextState<ActorStateNone>();
        }

        public override void SetNextState(Type type)
        {
            if (GetCurState() is ActorStateDead)
            {
                BRDebug.Log(string.Format(" ActorState Changing from:{0} to {1}", "ActorStateDead", type.FullName));
            }
            base.SetNextState(type);
        }
        //#endregion
    }
}

