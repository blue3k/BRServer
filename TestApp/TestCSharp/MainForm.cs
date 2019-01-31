using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using Microsoft.Win32;
using System.Threading;


namespace TestCSharp
{
    public partial class MainForm : Form
    {
        TestBase CurrentTest;

        private System.Windows.Forms.Timer m_UpdateTimer;

        int m_InstanceIndex;
        Mutex m_Mutex;

        RegistryKey m_RegKey;

        public MainForm()
        {
            InitializeComponent();

            CreateUniqueMutex();

            textID.Text = "Tester" + m_InstanceIndex;

            textOutput.Text = "Welcome !\r\n";

            comboTestList.DisplayMember = "TestName";
            comboTestList.ValueMember = "TestName";

            comboTestList.Items.Add(new ParsingTest());
            //comboTestList.Items.Add(new ChatTest());
            comboTestList.Items.Add(new MatchingTest());
            comboTestList.SelectedIndex = 1;

            m_UpdateTimer = new System.Windows.Forms.Timer();
            m_UpdateTimer.Tick += new EventHandler(OnUpdateTimer);
            m_UpdateTimer.Interval = 100; // every 100 ms
            m_UpdateTimer.Start();

            m_RegKey = Registry.CurrentUser.OpenSubKey("Software").OpenSubKey("TestCSharp", true);
            if (m_RegKey == null)
            {
                m_RegKey = Registry.CurrentUser.OpenSubKey("Software", true).CreateSubKey("TestCSharp",RegistryKeyPermissionCheck.ReadWriteSubTree);
            }

            bool bAutoStart = (string)m_RegKey.GetValue("AutoStart", "false") == "true";
            if (bAutoStart)
                OnRunTestClicked(null,null);
        }

        void CreateUniqueMutex()
        {
            do
            {
                try
                {
                    m_Mutex = new Mutex(true, string.Format("TestCSharp{0}", m_InstanceIndex));
                    if (!m_Mutex.WaitOne(0))
                    {
                        m_Mutex = null;
                    }
                }
                catch (Exception )
                {
                    m_Mutex = null;
                }

                m_InstanceIndex++;

            } while (m_Mutex == null);
        }

        private void OnUpdateTimer(object source, EventArgs e)
        {
            if (CurrentTest != null)
                CurrentTest.Update();
        }

        private void OnRunTestClicked(object sender, EventArgs e)
        {
            CurrentTest = (TestBase)comboTestList.SelectedItem;
            //CurrentTest.IP = m_RegKey.GetValue("IP", "115.68.25.198") as string;
            CurrentTest.ID = textID.Text;
            CurrentTest.Password = textID.Text;

            CurrentTest.PrintMessage = (strOut) =>
            {
                textOutput.Text += strOut;
                textOutput.Text += "\r\n";
                textOutput.Select(textOutput.Text.Length, 0);
                textOutput.ScrollToCaret();

            };

            if (CurrentTest.Game != null)
            {
                CurrentTest.Game.Terminate();
            }
            CurrentTest.Game = new BR.GameSystem.BRGame();
            //CurrentTest.Game.PrintMessage = (strOut) =>
            //{
            //    textOutput.Text += strOut;
            //    textOutput.Text += "\r\n";
            //    textOutput.Select(textOutput.Text.Length, 0);
            //    textOutput.ScrollToCaret();
            //};

            CurrentTest.Game.Initialize("","",false);
            CurrentTest.Game.Net.LoginServerAddresses[0].Address = m_RegKey.GetValue("IP", "175.209.1.158") as string;

            CurrentTest.RunTest();
        }

        private void OnKeyDown(object sender, KeyEventArgs e)
        {
            if(CurrentTest == null) return;

            if (e.KeyCode == Keys.Enter
                && textInput.Text.Length > 0)
            {
                CurrentTest.OnInput(textInput.Text);
                textInput.Text = "";
            }
        }

        private void OnFormClosing(object sender, FormClosingEventArgs e)
        {
            m_Mutex.ReleaseMutex();
            m_Mutex = null;
        }
    }
}
