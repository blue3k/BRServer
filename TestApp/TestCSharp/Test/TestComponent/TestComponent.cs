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
    public class TestComponent
    {
        TestBase m_TestBase;

        public BR.GameSystem.BRGame Game
        {
            get
            {
                return m_TestBase.Game;
            }
        }

        public TestComponent(TestBase testBase)
        {
            m_TestBase = testBase;
        }

        protected void PrintMessage(string strMsg)
        {
            m_TestBase.PrintMessage(strMsg);
        }

        public virtual bool OnInput(string strInput)
        {
            return false;
        }

        public virtual void RegisterMessagehandler()
        {
            //Game.Net.RegisterMessageHandler(LoginRes.MID, OnLoginRes);
            //Game.Net.RegisterMessageHandler(LoginByFacebookRes.MID, OnLoginByFacebookRes);

        }


    }
}
