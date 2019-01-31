////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description:   Game state first night vote class
//
////////////////////////////////////////////////////////////////////////////////

using System;
using BR;

using Cocos2D;
using BR.GameGraphics;

namespace BR.GameSystem
{
    public class GameStateInGame : GameState
    {
        static bool m_GlobalIsInitialized = false;

        public GameStateInGame(BRGame game, FSM fsm)
            : base(game, fsm)
        {
        }

		public override void Awake()
		{
			base.Awake();
		}

        static public void InitializeGlobal()
        {
            if (m_GlobalIsInitialized)
                return;

            m_GlobalIsInitialized = true;

            //var contentManager = XnaGame.Instance.Content;
            //// precache character data
            //contentManager.Load<BR.Engine.Node>(CCActorData.CharEffect.KillPlayer, Microsoft.Xna.Framework.Content.ContentManager.ResourceContext.Global);
            //contentManager.Load<BR.Engine.Node>(CCActorData.CharEffect.Dead, Microsoft.Xna.Framework.Content.ContentManager.ResourceContext.Global);
            //contentManager.Load<BR.Engine.Node>(CCActorData.CharEffect.Sleep, Microsoft.Xna.Framework.Content.ContentManager.ResourceContext.Global);
            //contentManager.Load<BR.Engine.Node>(CCActorData.CharEffect.Light, Microsoft.Xna.Framework.Content.ContentManager.ResourceContext.Global);
        }

        protected virtual void SwitchCharacterUI()
        {
            CCGameScene.Instance.PostActionToGameLayer(() =>
            {

                //CCGameScene.Instance.GameLayer.SwitchViewMode();

            });
        }

        protected virtual void InitGameState()
        {
            CCGameScene.Instance.PostActionToGameLayer(() =>
            {
                //var ingame = BRGame.Instance.UIManager.CurrentLayer.GetCanvas<CCInGameTop>();
                //if (null == ingame)
                //{
                //    return;
                //}

                //ingame.SetProgressState(BRGame.Instance.GameManager.GameStateID);
/*
                var chatting = BRGame.Instance.UIManager.CurrentLayer.GetCanvas<CCInGameChatting>();
                if (null == chatting)
                {
                    return;
                }
                chatting.ChangeBackground();
*/
            });
        }

		public override void OnEnter()
		{
			base.OnEnter ();

            InitializeGlobal();

            if (Game.Platform.CurrentUIState != PlatformInterface.UIState.Game)
            {
                Game.Platform.SetUIState(PlatformInterface.UIState.Game);

                //Game.UIManager.ShowUI<CCInGame>();

                var myActor = Game.PlayerManager.MyPlayer.GameInfo.Actor;
                if (myActor.FSM.GetCurState<ActorStateNone>() != null)
                {
                    myActor.FSM.SetNextState<ActorStateIdle>();
                }
            }

            SwitchCharacterUI();
            InitGameState();
        }

		public override void OnLeave()
		{
			base.OnLeave();
        }
    }
}