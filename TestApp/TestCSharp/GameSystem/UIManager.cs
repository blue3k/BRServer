using System;
using BR.GameGraphics;
using System.Collections.Generic;
using Cocos2D;
using BR.Message;
using BR.Message.Game;

using conspiracy;

#if ANDROID
using Android.App;
using Android.Widget;
#endif

namespace BR.GameSystem
{
    public class UIManager : GameSystemComponent
    {
        #region Types
        public enum ZOrder
        {
            UI = 1,
            Popup = 2,
            GamePopup = 3,
        }

        public enum Mode
        {
            InGame,
            OutGame,
        }
        #endregion

        #region Fields
        private CCGameScene gameScene = null;
        private CCUILayer currentLayer = null;
        //private CCUILayer popupRoot = null;
        private Popup currentPopup = null;
        private Dictionary<Type, Popup> popupList = new Dictionary<Type, Popup>();
        private Queue<Popup> popupQueue = new Queue<Popup>();

        private int m_Freezing = 0;
        private bool m_bLock = false;

        #endregion


        // constructor
        public UIManager(BRGame game)
            : base(game)
        {
        }


        #region Methods
        public override bool Initialize()
        {
            if (!base.Initialize())
                return false;

            return true;
        }

        public CCUILayer CurrentLayer { get { return currentLayer;} }

        public CCGameLayer GetGameLayer()
        {
            return gameScene.GameLayer;
        }

        void RunOnRenderThread( Action action )
        {
            var application = CCApplication.SharedApplication;
            if( application == null )
            {
                action();
                return;
            }

            application.RunOnRenderThread(action);
        }

        public void ShowVote(float duration)
        {
            if (null == CCGameScene.Instance)
            {
                return;
            }

            CCGameScene.Instance.PostActionToPopupLayer(() =>
            {
                //CCGameScene.Instance.PopupLayer.ShowVotePopup(duration);
            });
        }
        public void ShowFinalVote(float duration)
        {
            if (null == CCGameScene.Instance)
            {
                return;
            }

            CCGameScene.Instance.PostActionToPopupLayer(() =>
            {
                //CCGameScene.Instance.PopupLayer.ShowFinalVotePopup(duration);
            });
        }
        public void ShowSeerVote(float duration)
        {
            if (null == CCGameScene.Instance)
            {
                return;
            }

            CCGameScene.Instance.PostActionToPopupLayer(() =>
            {
                //CCGameScene.Instance.PopupLayer.ShowSeerVotePopup(duration);
            });
        }
        public void ShowMonsterVote(float duration)
        {
            if (null == CCGameScene.Instance)
            {
                return;
            }

            CCGameScene.Instance.PostActionToPopupLayer(() =>
            {
                //CCGameScene.Instance.PopupLayer.ShowMonsterVotePopup(duration);
            });
        }

        public void ShowGameResultTurn(bool monster_victory, float duration, Action endAction)
        {
            if (null == CCGameScene.Instance)
            {
                return;
            }

            CCGameScene.Instance.PostActionToPopupLayer(() =>
            {
                //CCGameScene.Instance.PopupLayer.ShowGameResultTrun(monster_victory, duration, endAction);
            });            
        }

        // popup ui interface
        public void ShowGameResult(float duration)
        {
            if (null == CCGameScene.Instance)
            {
                return;
            }

            CCGameScene.Instance.PostActionToPopupLayer(() =>
            {
                //CCGameScene.Instance.PopupLayer.ShowGameResultPopup(duration);
            });
        }

        public void ShowVoteResult(float duration)
        {
            if (null == CCGameScene.Instance)
            {
                return;
            }

            CCGameScene.Instance.PostActionToPopupLayer(() =>
            {
                //CCGameScene.Instance.PopupLayer.ShowVoteResultPopup(duration);
            });
        }

        public void ShowOption(float duration)
        {
            if (null == CCGameScene.Instance)
            {
                return;
            }

            CCGameScene.Instance.PostActionToPopupLayer(() =>
            {
                //CCGameScene.Instance.PopupLayer.ShowOptionPopup(duration);
            });
        }

        public void ShowDailyReport(float duration)
        {
            if (null == CCGameScene.Instance)
            {
                return;
            }

            CCGameScene.Instance.PostActionToPopupLayer(() =>
            {
                //CCGameScene.Instance.PopupLayer.ShowDailyReportPopup(duration);
            });
        }
        
        public void ShowCharacterIntro(float duration)
        {
            if (null == CCGameScene.Instance)
            {
                return;
            }

            CCGameScene.Instance.PostActionToPopupLayer(() =>
            {
               // CCGameScene.Instance.PopupLayer.ShowCharacterIntroPopup(duration);
            });
        }

        public void ShowTurn(float duration, Action endAction = null)
        {
            if (null == CCGameScene.Instance)
                return;

            CCGameScene.Instance.PostActionToPopupLayer(() =>
            {
                //CCGameScene.Instance.PopupLayer.ShowTurnPopup(Game.GameManager.GameDay, Game.GameManager.IsNight, duration, endAction);
            });
        }

        //public void ShowForceProcess(int votedcount, float duration, Action<CCGamePopupForceProgress.ProgressResult> action)
        //{
        //    if (null == CCGameScene.Instance)
        //    {
        //        return;
        //    }

        //    CCGameScene.Instance.PostActionToPopupLayer(() =>
        //    {
        //        //CCGameScene.Instance.PopupLayer.ShowForceProcessPopup(votedcount, duration, action);
        //    });
        //}

        public void ShowExitRoom(float duration)
        {
            if (null == CCGameScene.Instance)
            {
                return;
            }

            CCGameScene.Instance.PostActionToPopupLayer(() =>
            {
                //CCGameScene.Instance.PopupLayer.ShowExitRoomPopup(duration);
            });
        }

        public void HidePopup()
        {
            if (null == CCGameScene.Instance)
            {
                return;
            }

            CCGameScene.Instance.PostActionToPopupLayer(() =>
            {
               // CCGameScene.Instance.PopupLayer.HidePopup();
            });
        }

        public void HidePopupAll()
        {
            if (null == CCGameScene.Instance)
            {
                return;
            }

            CCGameScene.Instance.PostActionToPopupLayer(() =>
            {
                //CCGameScene.Instance.PopupLayer.HidePopupAll();
            });
        }

        public override void Start()
        {
            RunOnRenderThread(() =>
            {
                CCDirector pDirector = CCDirector.SharedDirector;

                //gameScene = new CCGameScene(this.Game);

                //// out game popup
                //popupRoot = new CCPopup(this.Game);
                //popupRoot.ZOrder = (int)ZOrder.Popup;
                //gameScene.AddChild(popupRoot);

                //pDirector.RunWithScene(gameScene);
            });
        }

        public void ShowUI<T>()
        {
            RunOnRenderThread(() =>
            {
                if (currentLayer != null)
                {
                    if (currentLayer.GetType() == typeof(T))
                        return;

                    CloseUI();
                }


                //if (typeof(T) == typeof(CCLobbyMain))
                //{
                //    currentLayer = new CCLobbyMain(this.Game);
                //}
                //else if (typeof(T) == typeof(CCShop))
                //{
                //    currentLayer = new CCShop(this.Game);
                //}
                //else if (typeof(T) == typeof(CCMail))
                //{
                //    currentLayer = new CCMail(this.Game);
                //}
                //else if (typeof(T) == typeof(CCFriend))
                //{
                //    currentLayer = new CCFriend(this.Game);
                //}
                //else if (typeof(T) == typeof(CCStandByRoom))
                //{
                //    currentLayer = new CCStandByRoom(this.Game);
                //}
                //else if (typeof(T) == typeof(CCInGame))
                //{
                //    currentLayer = new CCInGame(this.Game);
                //    gameScene.InitializeInGame(Game.GameManager.MaxPlayer);
                //    m_Freezing = 0;

                //}

                if (currentLayer == null)
                {
                    BRDebug.Error(string.Format("UIManager.ChangeUI - Invalid outGame : {0}", typeof(T).Name));
                    return;
                }

                //currentLayer.ZOrder = (int)ZOrder.UI;
                //gameScene.AddChild(currentLayer);
            });
        }

        public void CloseUI()
        {
            RunOnRenderThread(() =>
            {
                if (currentLayer == null)
                    return;

                //if (currentLayer.GetType() == typeof(CCInGame))
                //{
                //    gameScene.UninitializeInGame();
                //}

                //gameScene.RemoveChild(currentLayer);
                currentLayer = null;
            });
        }

        // popup interface
        public void AddPopup(Popup popup)
        {
            RunOnRenderThread(() =>
            {
                if (popup == null)
                {
                    BRDebug.Error("UIManager >> AddPopup >> popup is null!");
                    return;
                }

                popupList.Add(popup.GetType(), popup);
            });
        }

        public void ShowPopup<T>(Action<T> InitAction = null) where T : Popup
        {
            RunOnRenderThread(() =>
            {
                Popup popup = null;
                if (!popupList.TryGetValue(typeof(T), out popup))
                {
                    BRDebug.Error("UIManager >> ShowPopup >> Invalid popup type(" + typeof(T).ToString() + ")!");
                    return;
                }

                if (InitAction != null)
                {
                    InitAction((T)popup);
                }

                if (currentPopup != null)
                {
                    popupQueue.Enqueue(popup);
                }
                else
                {
                    //popup.SetVisible(true);
                    currentPopup = popup;
                }
            });
        }

        public void ClosePopup()
        {
            RunOnRenderThread(() =>
            {
                if (currentPopup == null)
                {
                    return;
                }

                //currentPopup.SetVisible(false);
                
                if (popupQueue.Count > 0)
                {
                    currentPopup = popupQueue.Dequeue();
                    //currentPopup.SetVisible(true);
                }
                else
                {
                    currentPopup = null;
                }
            });
        }

        public void ClosePopupAll()
        {
            //CCApplication.SharedApplication.RunOnRenderThread(() =>
            //{
            //});
        }

        public void HideKeyboard()
        {
//            CCInGameTop ingame = CurrentLayer.GetCanvas<CCInGameTop>();
//            if (ingame != null)
//            {
//                ingame.SetLayerVisible(false);
//            }

//            CurrentLayer.RunAction(new CCSequence(new CCDelayTime(0.3f), new CCCallFunc(() =>
//            {
//#if ANDROID
//                conspiracy.BRFragmentActivity activity = PlatformInterface_Android.ForgroundActivity;
//                activity.RunOnUiThread(() =>
//                {
//                    ((GUIPageGameMain)activity.CurrentView).HideKeyInput();
//                });
//#endif
//            })));
        }

        public void SetFreezing(bool bFreezing, bool bForce = false)
        {
            if (bFreezing)
                m_Freezing++;
            else
                m_Freezing--;

            if ((bForce && !bFreezing) || m_Freezing < 0 )
                m_Freezing = 0;

            BRDebug.Log(">>>>>>>> SetFreezing " + bFreezing + "    " + m_Freezing);

            //bool freezing = false;
            //if( m_Freezing > 0 )
            //    freezing = true;

//            CCInGameTop ingame = CurrentLayer.GetCanvas<CCInGameTop>();
//            if (ingame != null)
//            {
//                ingame.SetLayerVisible(false);
////                ingame.SetLayerPosition(0, 50);
//            }


//            if (m_bLock == true)
//                return;

//            GetGameLayer().SetFreezing(freezing);

//            CurrentLayer.RunAction(new CCSequence(new CCDelayTime(0.3f), new CCCallFunc(() =>
//            {
//#if ANDROID
//                conspiracy.BRFragmentActivity activity = PlatformInterface_Android.ForgroundActivity;
//                activity.RunOnUiThread(() =>
//                {
//                    ((GUIPageGameMain)activity.CurrentView).SetFreezing(freezing);
//                });
//#endif
//            })));

        }

        public void SetLockCharacterButton(bool enable, bool bLock = false)
        {
            BRDebug.Log(">>>>>>>> SetLockCharacterButton " + enable + "    " + m_Freezing + "    " + bLock);

            if (m_bLock == true && bLock == false)
                return;

            if( bLock == true )
                m_bLock = enable;

//            if (enable == true)
//                GetGameLayer().SetFreezing(enable);
//            else if (enable == false && m_Freezing <= 0)
//            {
//                GetGameLayer().SetFreezing(enable);
//            }

//            CurrentLayer.RunAction(new CCSequence(new CCDelayTime(0.3f), new CCCallFunc(() =>
//            {
//#if ANDROID
//                conspiracy.BRFragmentActivity activity = PlatformInterface_Android.ForgroundActivity;
//                activity.RunOnUiThread(() =>
//                {
//                    ((GUIPageGameMain)activity.CurrentView).SetLockCharacterButton(enable);
//                });
//#endif
//            })));
        }
        #endregion
    }
}
