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
using System.Collections.Generic;
using BR;
using BR.Message;
using BR.Message.Game;

using Cocos2D;
using BR.GameGraphics;

using Microsoft.Xna.Framework;

namespace BR.GameSystem
{

	public class GameStateLoading : GameState
    {
		public GameStateLoading(BRGame game, FSM fsm)
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

			Game.Platform.SetUIState(PlatformInterface.UIState.Loading);

			BRDebug.Log("GameStateLoading.OnEnter!");
        }

        public override void OnLeave()
        {
            base.OnLeave();

            // 
            Game.UIManager.Start();
            Game.TableManager.Start();

			BRDebug.Log("GameStateLoading.OnLeave!");
        }

		public override void UpdateState()
		{
//            var xnaGame = XnaGame.Instance;
//            if (xnaGame != null && xnaGame.Initialized)
//            {
//#if UI_TEST
//                MyFSM.SetNextState<GameStateUITest>();
//#else
//                MyFSM.SetNextState<GameStateLogin>();
//#endif
//            }
			base.UpdateState ();
		}
    }


}

