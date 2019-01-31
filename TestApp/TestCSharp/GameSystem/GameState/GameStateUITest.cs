////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description:   Game state ui test class
//
////////////////////////////////////////////////////////////////////////////////

using System;
using BR;

using Cocos2D;
using BR.GameGraphics;

namespace BR.GameSystem
{
    public class GameStateUITest : GameState
    {
        public GameStateUITest(BRGame game, FSM fsm)
            : base(game, fsm)
        {
        }

        public override void OnEnter()
        {
            base.OnEnter();

            Game.Platform.SetUIState(PlatformInterface.UIState.UI_TEST);
        }

        public override void OnLeave()
        {
            base.OnLeave();
        }
    }
}
