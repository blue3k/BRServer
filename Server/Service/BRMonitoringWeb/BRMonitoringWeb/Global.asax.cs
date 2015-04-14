using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Mvc;
using System.Web.Optimization;
using System.Web.Routing;
using System.Net;
using System.Net.Sockets;
using BRWebUtil;

namespace BRMonitoringWeb
{

    public class MvcApplication : System.Web.HttpApplication
    {
        public string LocalIPAddress()
        {
            IPHostEntry host;
            string localIP = "";
            host = Dns.GetHostEntry(Dns.GetHostName());
            foreach (IPAddress ip in host.AddressList)
            {
                if (ip.IsIPv6LinkLocal) continue;

                if (ip.AddressFamily == AddressFamily.InterNetworkV6)
                {
                    localIP = ip.ToString();
                    break;
                }
            }
            return localIP;
        }

        int GetSettingInt(string name, int defaultValue = -1)
        {
            int value = defaultValue;
            if (!int.TryParse(System.Web.Configuration.WebConfigurationManager.AppSettings[name], out value))
                value = defaultValue;
            return value;
        }

        bool GetSettingBool(string name, bool defaultValue = false)
        {
            bool value = defaultValue;
            string valueString = System.Web.Configuration.WebConfigurationManager.AppSettings[name];
            if (string.IsNullOrEmpty(valueString)) return false;

            bool.TryParse(valueString, out value);

            return value;
        }

        string GetSettingString(string name)
        {
            return System.Web.Configuration.WebConfigurationManager.AppSettings[name];
        }


        protected void Application_Start()
        {
            AreaRegistration.RegisterAllAreas();
            FilterConfig.RegisterGlobalFilters(GlobalFilters.Filters);
            RouteConfig.RegisterRoutes(RouteTable.Routes);
            BundleConfig.RegisterBundles(BundleTable.Bundles);

            BR.BRMonitoring.InitializeNativeSystem();

            string serverAddress = LocalIPAddress();
            uint port = (uint)GetSettingInt("MonitoringListenPort", 0);
            BR.PerformanceCounterServer.Initialize(serverAddress, port);

            var monitoringDB = GetSettingString("AccountDBConnectionString");
            DBPool.MonitoringDB.ConnectionString = monitoringDB;
        }

        protected void Application_End()
        {
            BR.PerformanceCounterServer.Terminate();

            BR.BRMonitoring.TerminateNativeSystem();
        }
    }
}
