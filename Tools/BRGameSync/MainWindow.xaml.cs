using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using BR;
using SF;
using SF.Tool;

namespace BRGameSync
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        VersionControlClient m_VersionControl;
        FolderWatcher m_Watcher;

        Task m_ExclusiveTask;

        public MainWindow()
        {
            InitializeComponent();

            SF.Log.LogHandler += PrintLog;

            var latestProfile = AppConfig.GetValue<string>("LatestProfile");
            var profileSet = AppConfig.GetValueSet(latestProfile);
            string LocalPath = "";
            string HostAddress = "";
            try
            {
                LocalPath = profileSet["Local"] as string;
                HostAddress = profileSet["Host"] as string;
                var splitIndex = HostAddress.IndexOf('/');
                string HostPath = "/";
                if (splitIndex >= 0)
                {
                    HostPath = HostAddress.Substring(splitIndex);
                    HostAddress = HostAddress.Substring(0, splitIndex);
                }

                m_VersionControl = new VersionControlClient();
                m_VersionControl.Initialize(LocalPath, HostPath);
                m_Watcher = new FolderWatcher();
                m_Watcher.Initialize(m_VersionControl);
            }
            catch (Exception exp)
            {
                SF.Log.Error("{0} => {1}", exp.Message, exp.StackTrace.ToString());
                Close();
                MessageBox.Show(string.Format("{0} => {1}", exp.Message, exp.StackTrace.ToString()));
                return;
            }

            textLocal.Text = LocalPath;
            textRemote.Text = HostAddress;

            listChangedFiles.VersionClient = m_VersionControl;
            listChangedFiles.UpdateVersionList();

            listLocalChanges.VersionClient = m_VersionControl;

            m_ExclusiveTask = m_VersionControl.ReconcileLocalChanges();

            // show itself
            Show();
        }

        void PrintLog(SF.Log.Level level, string message)
        {
            var newLogEntry = new LogEntry();
            newLogEntry.Message = string.Format("{0}:{1}", level, message);
            Dispatcher.BeginInvoke((Action)(() => LogOutput.LogEntries.Add(newLogEntry)));
        }

        protected override void OnClosing(CancelEventArgs e)
        {
            SF.Log.LogHandler -= PrintLog;
        }

        async Task GetLatestAsync()
        {
            Dispatcher.Invoke(() => m_Watcher.Enabled = false);
            await m_VersionControl.GetLatestAsync();
            Dispatcher.Invoke(() => m_Watcher.Enabled = true);
        }

        private void OnGetLatestClicked(object sender, RoutedEventArgs e)
        {
            if (m_ExclusiveTask != null && !m_ExclusiveTask.IsCompleted)
            {
                SF.Log.Error("Application is busy");
                return;
            }
            m_ExclusiveTask = GetLatestAsync();
        }

        private void OnReconcileClicked(object sender, RoutedEventArgs e)
        {
            if (m_ExclusiveTask != null && !m_ExclusiveTask.IsCompleted)
            {
                SF.Log.Error("Application is busy");
                return;
            }
            m_ExclusiveTask = m_VersionControl.ReconcileLocalChanges();
        }

        async Task CommitChangesAsync(string description)
        {
            await m_VersionControl.CommitChanges(description);
            Dispatcher.Invoke(()=> listChangedFiles.UpdateVersionList());
        }

        private void OnCommitClicked(object sender, RoutedEventArgs e)
        {
            if (m_ExclusiveTask != null && !m_ExclusiveTask.IsCompleted)
            {
                SF.Log.Error("Application is busy");
                return;
            }

            if (m_VersionControl.ModifiedFileList.Count == 0)
            {
                SF.Log.Error("Nothing to commit");
                return;
            }

            var commitWindow = new CommitWindow();
            commitWindow.FileList = m_VersionControl.ModifiedFileList.FileInfoList;
            var bRes = commitWindow.ShowDialog();
            if (bRes.HasValue && bRes.Value)
            {
                m_ExclusiveTask = CommitChangesAsync(commitWindow.CommitData.Description);
            }
        }
    }
}

