////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description:   Game state morning debate class
//
////////////////////////////////////////////////////////////////////////////////

using System;
using System.IO;
using System.Collections.Generic;
using BR;
using BR.Engine;
using BR.Message;
using BR.Message.Game;

using Microsoft.Xna.Framework;

namespace BR.GameSystem
{

	public class GameStatePatching : GameState
    {
        const int MaxRetryCount = 10;
        int m_RetryCount;

        // for disabling warning
        public int RetryCount { get { return m_RetryCount; } }

        public GameStatePatching(BRGame game, FSM fsm)
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
            m_RetryCount = 0;
        }

        public override void OnLeave()
        {
            base.OnLeave();

            BRDebug.Log("GameStatePatching.OnLeave!");
        }

		public override void UpdateState()
		{
            // disable patch on windows
            MyFSM.SetNextState<GameStateLoading>();

            base.UpdateState();
		}
    }


}

