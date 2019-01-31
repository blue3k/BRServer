using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Threading;
using BR.ToolLib;


namespace BR.FTPSync
{
    public partial class MainForm : Form
    {
        FTPPathSynchronizer m_FTPSynchronizer = new FTPPathSynchronizer();
        System.Threading.Thread m_Thread;
        System.Windows.Forms.Timer m_Timer = new System.Windows.Forms.Timer();
        string m_LocalPath = "C:\\work\\BRServerGit";
        string m_RemotePath = "/home/blue3k/work/BRServer";
        string m_RemoteAddress = "ftp://192.168.1.81:10122";
        string m_RemoteID = "blue3k";
        string m_RemotePW = "zpdlrns12#";

        public MainForm()
        {
            InitializeComponent();
            InitializeSetting();
            InitializeSynchronizer();
            //textBoxLog.MaxLength = 10000;
        }

        void InitializeSetting()
        {
            m_LocalPath = ToolEnvironment.GetSetting<string>("localpath", m_LocalPath);
            m_RemotePath = ToolEnvironment.GetSetting<string>("remotepath", m_RemotePath);
            m_RemoteAddress = ToolEnvironment.GetSetting<string>("remoteAddress", m_RemoteAddress);
            m_RemoteID = ToolEnvironment.GetSetting<string>("ID", m_RemoteID);
            m_RemotePW = ToolEnvironment.GetSetting<string>("PW", m_RemotePW);
        }

        void InitializeSynchronizer()
        {
            m_FTPSynchronizer.AddIgnoreExt(".pdb");
            m_FTPSynchronizer.AddIgnoreExt(".pch");
            m_FTPSynchronizer.AddIgnoreExt(".ilk");
            m_FTPSynchronizer.AddIgnoreExt(".obj");
            m_FTPSynchronizer.AddIgnoreExt(".suo");
            m_FTPSynchronizer.AddIgnoreExt(".cmd");
            m_FTPSynchronizer.AddIgnoreExt(".user");
            m_FTPSynchronizer.AddIgnoreExt(".sdf");
            m_FTPSynchronizer.AddIgnoreExt(".opensdf");
            m_FTPSynchronizer.AddIgnoreExt(".recipe");
            m_FTPSynchronizer.AddIgnoreExt(".vs");
            m_FTPSynchronizer.AddIgnoreExt(".o");
            m_FTPSynchronizer.AddIgnoreExt(".pdf");
            m_FTPSynchronizer.AddIgnoreExt(".tlb");
            m_FTPSynchronizer.AddIgnoreExt(".lib");
            m_FTPSynchronizer.AddIgnoreExt(".exe");
            m_FTPSynchronizer.AddIgnoreExt(".dll");
            m_FTPSynchronizer.AddIgnoreExt(".zip");
            m_FTPSynchronizer.AddIgnoreExt(".log");
            m_FTPSynchronizer.AddIgnoreExt(".tlog");
            m_FTPSynchronizer.AddIgnoreExt(".idb");
            m_FTPSynchronizer.AddIgnoreExt(".manifest");
            m_FTPSynchronizer.AddIgnoreExt(".rc");
            m_FTPSynchronizer.AddIgnoreExt(".git");

            m_FTPSynchronizer.AddIgnoreFolder("debug");
            m_FTPSynchronizer.AddIgnoreFolder("release");
            m_FTPSynchronizer.AddIgnoreFolder("x64Debug");
            m_FTPSynchronizer.AddIgnoreFolder("x64Release");
            m_FTPSynchronizer.AddIgnoreFolder("bin");
            m_FTPSynchronizer.AddIgnoreFolder("temp");
            m_FTPSynchronizer.AddIgnoreFolder(".git");
            m_FTPSynchronizer.AddIgnoreFolder(".gitattributes");
            m_FTPSynchronizer.AddIgnoreFolder(".vs");
            m_FTPSynchronizer.AddIgnoreFolder("tmp32");
            m_FTPSynchronizer.AddIgnoreFolder("tmp32.dbg");
            m_FTPSynchronizer.AddIgnoreFolder("ipch");

            m_Thread = new System.Threading.Thread(TickThread);
            m_Thread.Start();

            // set Path
            textBoxLocalPath.Text = m_LocalPath;
            textBoxRemotePath.Text = m_RemotePath;

            // Message update
            m_Timer.Tick += new EventHandler((sender, evt) => 
            {
                var count = m_MessageQueue.Count;
                for(int iMessage = 0; iMessage < count; iMessage++)
                {
                    string message;
                    if (m_MessageQueue.TryDequeue(out message))
                    {
                        textBoxLog.AppendText(message);
                    }
                }
            });
            m_Timer.Interval = 500;
            m_Timer.Enabled = true;
            m_Timer.Start();
        }


        ConcurrentQueue<string> m_MessageQueue = new ConcurrentQueue<string>();

        void Log(string format, params object[] args)
        {
            var message = string.Format(format, args);
            m_MessageQueue.Enqueue(message + "\n");
        }

        protected override void OnFormClosed(FormClosedEventArgs e)
        {
            m_Timer.Stop();
            m_Timer.Dispose();
            m_Timer = null;

            m_Thread.Abort();
            m_Thread = null;

            m_FTPSynchronizer.Dispose();
            m_FTPSynchronizer = null;


        }

        void TickThread()
        {
            m_FTPSynchronizer.Log = Log;
            m_FTPSynchronizer.Initialize(m_LocalPath, m_RemotePath, m_RemoteAddress, m_RemoteID, m_RemotePW);

            while (true)
            {
                m_FTPSynchronizer.SyncFolder();
                Thread.Sleep(500);
            }
        }

    }
}
