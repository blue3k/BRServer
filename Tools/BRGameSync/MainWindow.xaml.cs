using System;
using System.Collections.Generic;
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
        public MainWindow()
        {
            var latestProfile = AppConfig.GetValue<string>("LatestProfile");
            var profileSet = AppConfig.GetValueSet(latestProfile);
            try
            {
                string LocalPath = profileSet["Local"] as string;
                string HostAddress = profileSet["Host"] as string;
                var splitIndex = HostAddress.IndexOf('/');
                string HostPath = "/";
                if (splitIndex >= 0)
                {
                    HostPath = HostAddress.Substring(splitIndex);
                    HostAddress = HostAddress.Substring(0, splitIndex);
                }

                m_VersionControl = new VersionControlClient();
                m_VersionControl.Initialize(LocalPath, HostPath, HostAddress);
                m_Watcher = new FolderWatcher();
                m_Watcher.Initialize(m_VersionControl);
            }
            catch (Exception)
            {

            }


            InitializeComponent();
        }
    }
}
