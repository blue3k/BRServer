using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace Cocos2D
{
    public class Popup
    {

    }

    public class CCUILayer
    {

    }

    public class CCGameLayer : CCUILayer
    {

    }

    public class CCScene
    {

    }

    public class CCGameScene : CCScene
    {
        public CCGameLayer GameLayer;

        static public CCGameScene Instance;

        public void PostActionToPopupLayer(Action act)
        {

        }
        public void PostInitGameStateActionToGameLayer(Action act)
        {

        }

        public void PostActionToGameLayer(Action act)
        {

        }
    }

    public class CCApplication
    {
        static public CCApplication SharedApplication;

        public void RunOnRenderThread(Action act)
        {

        }
    }

    public class CCDirector
    {
        static public CCDirector SharedDirector;

    }
}

