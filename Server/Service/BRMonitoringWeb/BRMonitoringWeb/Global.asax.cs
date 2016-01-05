using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Mvc;
using System.Web.Optimization;
using System.Web.Routing;
using BR.Config;
using BR.DB;
using System.Net;
using System.Net.Sockets;
using System.Runtime.InteropServices;

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
                if (ip.IsIPv4MappedToIPv6) continue;
                if (ip.AddressFamily == AddressFamily.InterNetworkV6) continue;

                localIP = ip.ToString();
            }
            return localIP;
        }

        void AddDBCluster(BR.Config.DBCluster dbClusterConfig)
        {
            var cluster = DBConnectionManager.AddCluster(dbClusterConfig.ClusterName);

            cluster.AddShard(0, dbClusterConfig.ConnectionString);
        }

        void AddDBCluster(BR.Config.DBCluster[] dbClusterConfig)
        {
            if (dbClusterConfig == null || dbClusterConfig.Length == 0)
                return;

            var cluster = DBConnectionManager.AddCluster(dbClusterConfig[0].ClusterName);

            int shard = 0;
            foreach (var shardConfig in dbClusterConfig)
            {
                cluster.AddShard(shard, shardConfig.ConnectionString);
                shard++;
            }
        }


        [DllImport("kernel32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        private static extern IntPtr LoadLibrary(string libname);

        [DllImport("kernel32.dll", CharSet = CharSet.Auto)]
        private static extern bool FreeLibrary(IntPtr hModule);

        void DLLLoadTest()
        {
            IntPtr Handle = LoadLibrary("BRMonitoring");
            if (Handle == IntPtr.Zero)
            {
                int errorCode = Marshal.GetLastWin32Error();
                return;
            }

            if (Handle != IntPtr.Zero)
                FreeLibrary(Handle);
        }

        protected void Application_Start()
        {
            AreaRegistration.RegisterAllAreas();
            FilterConfig.RegisterGlobalFilters(GlobalFilters.Filters);
            RouteConfig.RegisterRoutes(RouteTable.Routes);
            BundleConfig.RegisterBundles(BundleTable.Bundles);


            try
            {
                string serverPath = Server.MapPath("~/");
                string serverConfigPath = Server.MapPath("~/config.xml");


                if (!MonitoringConfig.LoadConfig(serverConfigPath))
                {
                    throw new Exception("Failed to load Config!");
                }


                string serverAddress = LocalIPAddress();

                DBConnectionManager.InitializeManager();

                AddDBCluster(MonitoringConfig.Instance.AccountDB);
                AddDBCluster(MonitoringConfig.Instance.GameDB);
                AddDBCluster(MonitoringConfig.Instance.RankingDB);
                AddDBCluster(MonitoringConfig.Instance.TableDB);

                //DLLLoadTest();


                BR.BRMonitoring.InitializeNativeSystem("BRManigement", serverPath, "traceConfig.cfg");
                BR.PerformanceCounterServer.Initialize(serverAddress, (uint)MonitoringConfig.Instance.Monitoring.MonitoringListenPort);
            }
            catch (Exception exp)
            {
                System.Diagnostics.Debug.Print(exp.Message);
                throw exp;
            }

        }


        protected void Application_End()
        {
            DBConnectionManager.TerminateManager();

            BR.PerformanceCounterServer.Terminate();

            BR.BRMonitoring.TerminateNativeSystem();
        }
    }
}
