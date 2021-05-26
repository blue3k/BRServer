using BR;
using SF;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace BRGameSync
{

    public class ChangeListViewItem : PropertyChangedBase
    {
        IVersionControl.VersionInfo m_VersionInfo;

        public Int64 ChangeNumber { get { return m_VersionInfo.ChangeNumber; } }
        public string Description { get { return m_VersionInfo.Description.Replace('\n', ' '); } }
        public string User { get { return m_VersionInfo.User; } }
        public DateTimeOffset SubmittedTime { get { return m_VersionInfo.Modified; } }

        public ChangeListViewItem()
        {

        }

        public ChangeListViewItem(IVersionControl.VersionInfo versionInfo)
        {
            m_VersionInfo = versionInfo;
        }
    }


    /// <summary>
    /// Interaction logic for ChangeListControl.xaml
    /// </summary>
    public partial class ChangeListControl : UserControl
    {
        ObservableCollection<ChangeListViewItem> m_ChangeList;
        public ObservableCollection<ChangeListViewItem> ChangeList
        {
            get { return m_ChangeList; }
            set { DataContext = m_ChangeList = value; }
        }

        VersionControlClient m_VersionClient;
        public VersionControlClient VersionClient
        {
            get { return m_VersionClient; }
            set { m_VersionClient = value;; }
        }

        public ChangeListControl()
        {
            InitializeComponent();

            DataContext = ChangeList = new ObservableCollection<ChangeListViewItem>();

            AddHotKeys();
        }


        private void AddHotKeys()
        {
            RoutedCommand firstSettings = new RoutedCommand();
            firstSettings.InputGestures.Add(new KeyGesture(Key.F5));
            CommandBindings.Add(new CommandBinding(firstSettings, OnRefresh));
        }

        void OnRefresh(object sender, ExecutedRoutedEventArgs e)
        {
            if (m_VersionClient == null)
                return;

            UpdateVersionList();
        }
        public void UpdateVersionList()
        {
            if (m_VersionClient == null)
            {
                SF.Log.Error("UpdateVersionList, Not initialized");
                return;
            }

            m_ChangeList.Clear();
            foreach (var versionInfo in m_VersionClient.GetVersionList())
            {
                m_ChangeList.Add(new ChangeListViewItem(versionInfo));
            }
            SF.Log.Info("ChangeList has updated");
        }


        private void ScrollViewer_ScrollChanged(object sender, ScrollChangedEventArgs e)
        {
            
        }

    }
}
