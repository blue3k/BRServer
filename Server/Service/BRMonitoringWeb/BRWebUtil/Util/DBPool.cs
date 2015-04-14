using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using MySql.Data.MySqlClient;


namespace BRWebUtil
{
    
    public class DBPool
    {
        public static DBConnectionPool MonitoringDB = new DBConnectionPool();
    }
}