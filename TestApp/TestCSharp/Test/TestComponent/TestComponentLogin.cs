using System;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using BR.Message.Game;
using BR.Message;
using BR;
using BR.GameSystem;
using BR.Net;


namespace TestCSharp
{
    public class TestComponentLogin : TestComponent
    {
        public TestComponentLogin(TestBase testBase)
            : base(testBase)
        {
        }

        public override bool OnInput(string strInput)
        {
            if (base.OnInput(strInput)) return true;

            return false;
        }

        public override void RegisterMessagehandler()
        {
            base.RegisterMessagehandler();
            //Game.Net.RegisterMessageHandler(LoginRes.MID, OnLoginRes);
            //Game.Net.RegisterMessageHandler(LoginByFacebookRes.MID, OnLoginByFacebookRes);

        }


    }
}
