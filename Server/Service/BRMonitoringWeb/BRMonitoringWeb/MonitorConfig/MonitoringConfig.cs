using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Web;
using System.Xml.Serialization;

namespace BR.Config
{
    public partial class MonitoringConfig
    {
        static public MonitoringConfig Instance { get; protected set; }

        static public bool LoadConfig(string serverConfigPath)
        {
            MonitoringConfig config = null;
            try
            {
                XmlSerializer serializer = new XmlSerializer(typeof(MonitoringConfig));
                using (var configFile = new FileStream(serverConfigPath, FileMode.Open))
                {
                    config = (MonitoringConfig)serializer.Deserialize(configFile);
                }
            }
            catch (Exception exp)
            {
                System.Diagnostics.Debug.Assert(false, exp.Message);
                return false;
            }

            Instance = config;
            return true;
        }
    }
}