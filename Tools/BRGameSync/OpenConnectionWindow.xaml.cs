using System;
using System.Collections.Generic;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using SF.Tool;

namespace BRGameSync
{
    /// <summary>
    /// Interaction logic for OpenConnectionWindow.xaml
    /// </summary>
    public partial class OpenConnectionWindow : Window
    {

        public OpenConnectionWindow()
        {
            InitializeComponent();

            var latestProfile = AppConfig.GetValue<string>("LatestProfile");
            if (!string.IsNullOrEmpty(latestProfile))
            {
                LoadProfile(latestProfile);

                comboProfiles.Items.Add(latestProfile);
            }
            else
            {
                latestProfile = "default";
                comboProfiles.Items.Add(latestProfile);
            }
            

            // Load profile list
            for (int iProfile = 0; iProfile < 10; iProfile++)
            {
                var value = AppConfig.GetValue<string>(iProfile.ToString());
                if (value == null)
                    break;

                if (!comboProfiles.Items.Contains(value))
                    comboProfiles.Items.Add(value);
            }

            comboProfiles.SelectedItem = latestProfile;
        }

        void LoadProfile(string profileName)
        {
            var profileSet = AppConfig.GetValueSet(profileName);
            try
            {
                textHost.Text = profileSet["Host"] as string;
                textLocal.Text = profileSet["Local"] as string;
            }
            catch(Exception)
            {

            }
        }

        void SaveProfile(string profileName)
        {
            AppConfig.SetValue("LatestProfile", profileName);
            for (int iProfile = 10; iProfile >= 0; iProfile--)
            {
                var value = AppConfig.GetValue<string>(iProfile.ToString());
                if (value == null)
                    continue;

                var key2 = (iProfile + 1).ToString();
                AppConfig.SetValue(key2, value);
            }
            AppConfig.SetValue("0", profileName);

            AppConfig.SetValue(profileName, "Host", textHost.Text);
            AppConfig.SetValue(profileName, "Local", textLocal.Text);
        }

        private void OnConnectClicked(object sender, RoutedEventArgs e)
        {
            var profile = comboProfiles.SelectedItem as string;
            if (string.IsNullOrEmpty(profile))
            {
                profile = comboProfiles.Text;
                if (string.IsNullOrEmpty(profile))
                    return;
            }

            // Save current values before it loaded back by changing combo profile selection below
            SaveProfile(profile);

            // Move item to the top
            if (comboProfiles.Items.Contains(profile))
                comboProfiles.Items.Remove(profile);
            comboProfiles.Items.Insert(0, profile);


            var profileSet = AppConfig.GetValueSet(profile);
            try
            {
                profileSet["Host"] = textHost.Text;
                profileSet["Local"] = textLocal.Text;
            }
            catch (Exception)
            {
            }

            AppConfig.SaveLocalConfig();


            // Open main
            var newMain = new MainWindow();
            newMain.Show();

            // Close this
            Close();
        }

        private void OnProfileSelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            var profile = comboProfiles.SelectedItem as string;
            if (string.IsNullOrEmpty(profile))
            {
                profile = comboProfiles.Text;
                if (string.IsNullOrEmpty(profile))
                    return;
            }

            LoadProfile(profile);
        }

        private void OnClickBrowseLocalPath(object sender, RoutedEventArgs e)
        {
        }
    }
}
