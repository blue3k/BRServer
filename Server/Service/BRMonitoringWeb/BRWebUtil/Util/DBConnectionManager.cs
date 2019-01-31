using System;
using System.Collections.Generic;
using System.Collections.Concurrent;
using System.Linq;
using System.Web;
using MySql.Data.MySqlClient;

namespace BR.DB
{
    public class DBCluster
    {
        string m_ClusterName;
        public string ClusterName { get { return m_ClusterName; } }

        // Connection pool per DB instance
        ConcurrentDictionary<int, ConnectionPool> m_ConnectionPools = new ConcurrentDictionary<int, ConnectionPool>();

        public DBCluster(string clusterName)
        {
            m_ClusterName = clusterName;
        }

        public void AddShard(int shardID, string connectionString)
        {
            var newShard = new ConnectionPool(connectionString);
            bool addRes = m_ConnectionPools.TryAdd(shardID, newShard);
            System.Diagnostics.Debug.Assert(addRes, "Failed to Add sharad:"+shardID);
        }

        public DBConnection GetConnection(int shardID)
        {
            ConnectionPool connectionPool = null;
            m_ConnectionPools.TryGetValue(shardID, out connectionPool);
            if (connectionPool == null)
            {
                System.Diagnostics.Debug.Assert(false, "Unknown DB cluster:" + m_ClusterName + ", shard:" + shardID);
                return null;
            }

            return connectionPool.GetConnection();
        }
    }

    public class DBConnectionManager
    {
        ConcurrentDictionary<string, DBCluster> m_ClusterManager = new ConcurrentDictionary<string, DBCluster>();
        static DBConnectionManager stm_Instance;

        public static void InitializeManager()
        {
            stm_Instance = new DBConnectionManager();
        }
        public static void TerminateManager()
        {
            stm_Instance = null;
        }

        public static DBCluster AddCluster(string clusterName)
        {
            if (stm_Instance == null)
                return null;

            if(string.IsNullOrEmpty(clusterName))
                return null;

            DBCluster cluster = new DBCluster(clusterName);
            return stm_Instance.m_ClusterManager.GetOrAdd(cluster.ClusterName, cluster);
        }

        public static DBCluster GetCluster(string clusterName)
        {
            if (stm_Instance == null)
                return null;

            if (string.IsNullOrEmpty(clusterName))
                return null;

            DBCluster cluster = null;
            stm_Instance.m_ClusterManager.TryGetValue(clusterName, out cluster);
            if (cluster == null)
            {
                System.Diagnostics.Debug.Assert(false, "Unknown DB cluster:" + clusterName);
                return null;
            }

            return cluster;
        }

        public static DBConnection GetConnection(string clusterName, int shardID = 0)
        {
            if (stm_Instance == null)
                return null;

            if (string.IsNullOrEmpty(clusterName))
                return null;

            DBCluster cluster = null;
            stm_Instance.m_ClusterManager.TryGetValue(clusterName, out cluster);
            if(cluster == null)
            {
                System.Diagnostics.Debug.Assert(false, "Unknown DB cluster:" + clusterName);
                return null;
            }

            return cluster.GetConnection(shardID);
        }
    }
}
