using BR;
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
using System.Windows.Shapes;

namespace BRGameSync
{
    public class CommitViewData
    {
        public string Description { get; set; }
    }

    /// <summary>
    /// Interaction logic for CommitWindow.xaml
    /// </summary>
    public partial class CommitWindow : Window
    {
        public CommitViewData CommitData { get; private set; }

        public ObservableCollection<VersionFileInfo> FileList { set { listFileInfo.FileInfos = value; } }

        public CommitWindow()
        {
            DataContext = CommitData = new CommitViewData();

            InitializeComponent();
        }

        private void OnCommitClicked(object sender, RoutedEventArgs e)
        {
            if (!string.IsNullOrEmpty(CommitData.Description))
            {
                DialogResult = true;
                Close();
            }
        }
    }
}
