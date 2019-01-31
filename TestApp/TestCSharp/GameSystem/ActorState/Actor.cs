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

using Cocos2D;
using BR.GameGraphics;

namespace BR.GameSystem
{
    public class Actor
    {
        #region Fields
        private BRGame game = null;
        private ActorFSM fsm;
        #endregion


        #region Fields
        public ActorFSM FSM { get { return fsm; } }
        #endregion


        #region Constructors
        public Actor(BRGame game, PlayerInfo player)
        {
            this.game = game;
            fsm = new ActorFSM(game, player);
        }
        #endregion


        #region Methods

        public void InitState()
        {
            fsm.SetNextState<ActorStateShow>();
        }

        public void Update()
        {
            fsm.UpdateFSM();
        }

        public void Hide()
        {
            //CCGameScene.Instance.PostActionToGameLayer(() =>
            //{
            //    var gameLayer = game.UIManager.GetGameLayer();
            //    gameLayer.RemoveCharacter(fsm.Player.PlayerID);
            //    fsm.SetNextState<ActorStateNone>();
            //});
        }
        #endregion
    }
}
