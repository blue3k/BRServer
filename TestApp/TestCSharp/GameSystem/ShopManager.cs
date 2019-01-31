using System;
using System.Collections.Generic;

using BR.Message.Game;
using BR.Message;
using BR;
using BR.Net;


namespace BR.GameSystem
{
    public class ShopManager : GameSystemComponent
    {
        #region Methods
        public ShopManager(BRGame game)
            : base(game)
        {
        }

        public override bool Initialize()
        {
            if (!base.Initialize())
                return false;

            RegisterMessagehandler();

            return true;
        }

        public void BuyShopItem(uint shopItemID)
        {
            NetworkManager netMgr = Game.Net;
            if (netMgr.IsConnected)
            {
                netMgr.GamePolicy.BuyShopItemCmd(shopItemID, string.Empty);
            }
        }
        #endregion


        #region Implements
        private void RegisterMessagehandler()
        {
            Game.Net.RegisterMessageHandler(BuyShopItemRes.MID, OnBuyShopItemRes);
        }
        #endregion


        #region Message handlers
        public int OnBuyShopItemRes(MessageBase msg)
        {
            BuyShopItemRes msgRes = (BuyShopItemRes)msg;

            if (msgRes.Result < 0)
            {
                BRDebug.Warning(string.Format("BuyShopItemRes failed {0}", msgRes.Result));
                return msgRes.Result;
            }

            return 0;
        }
        #endregion
    }
}
