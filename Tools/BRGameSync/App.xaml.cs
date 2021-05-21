using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Linq;
using System.Threading.Tasks;
using System.Windows;

namespace BRGameSync
{
    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    public partial class App : Application
    {
        protected override void OnStartup(StartupEventArgs e)
        {
            SF.GlobalEngine.Start("BRGameSync_"+ System.Environment.MachineName, SF.Tool.AppConfig.GetValue<string>("LogServer"));
            base.OnStartup(e);
        }

        protected override void OnExit(ExitEventArgs e)
        {
            SF.GlobalEngine.Stop();
            base.OnExit(e);
        }
    }
}
