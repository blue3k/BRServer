using System;
using System.Windows.Forms;
using System.Collections.Generic;
using BR;
using BR.Message.Game;
using BR.Message;
using BR.Net;

namespace TestCSharp
{
    public abstract class TestBase
    {
        public string ID { get; set; }
        public string Password { get; set; }

        public BR.GameSystem.BRGame Game { get; set; }

        public delegate void delPrintMessage(string strMsg);
        public delPrintMessage PrintMessage;

        public List<TestComponent> TestComponents { get; private set; }

        public TestBase()
        {
            TestComponents = new List<TestComponent>();
        }



        public string TestName { get; protected set; }
        public virtual bool RunTest()
        {
            foreach (TestComponent comp in TestComponents)
            {
                comp.RegisterMessagehandler();
            }
            return true;
        }

        public virtual bool OnInput(string strInput)
        {
            foreach (TestComponent comp in TestComponents)
            {
                if (comp.OnInput(strInput)) return true;
            }
            return false;
        }

        public virtual void Update() { Game.Update(); }
    }
}
