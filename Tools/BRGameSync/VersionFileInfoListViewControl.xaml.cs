using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Collections.Specialized;
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
using BR;

namespace BRGameSync
{

    /// <summary>
    /// Interaction logic for VersionFileInfoListViewControl.xaml
    /// </summary>
    public partial class VersionFileInfoListViewControl : UserControl
    {
        ObservableCollection<VersionFileInfo> m_FileInfos;
        public ObservableCollection<VersionFileInfo> FileInfos
        {
            get { return m_FileInfos; }
            set { DataContext = m_FileInfos = value; }
        }

        VersionControlClient m_VersionClient;
        public VersionControlClient VersionClient
        {
            get { return m_VersionClient; }
            set { m_VersionClient = value; FileInfos = m_VersionClient.ModifiedFileList.FileInfoList; }
        }

        public VersionFileInfoListViewControl()
        {
            InitializeComponent();
        }


        private bool AutoScroll = true;
        private void ScrollViewer_ScrollChanged(object sender, ScrollChangedEventArgs e)
        {
            // User scroll event : set or unset auto scroll mode
            if (e.ExtentHeightChange == 0)
            {   // Content unchanged : user scroll event
                if ((e.Source as ScrollViewer).VerticalOffset == (e.Source as ScrollViewer).ScrollableHeight)
                {   // Scroll bar is in bottom
                    // Set auto scroll mode
                    AutoScroll = true;
                }
                else
                {   // Scroll bar isn't in bottom
                    // Unset auto scroll mode
                    AutoScroll = false;
                }
            }

            // Content scroll event : auto scroll eventually
            if (AutoScroll && e.ExtentHeightChange != 0)
            {   // Content changed and auto scroll mode set
                // Auto scroll
                (e.Source as ScrollViewer).ScrollToVerticalOffset((e.Source as ScrollViewer).ExtentHeight);
            }
        }

    }
}
