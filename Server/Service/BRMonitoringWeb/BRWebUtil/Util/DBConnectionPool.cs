using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using MySql.Data.MySqlClient;


namespace BRWebUtil
{
    
    public class PlayerInfo
    {
        public string Name { get; set; }
        public UInt64 UID { get; set; }
        public uint NumCounters { get; set; }
    }

    public class GameLogInfo
    {
        public string Name { get; set; }
        public UInt64 UID { get; set; }
        public uint NumCounters { get; set; }
    }


    public class DBConnection : IDisposable
    {
        MySqlConnection m_Connection;

        public DBConnection( MySqlConnection conn )
        {
            m_Connection = conn;
        }

        public void Dispose()
        {
            if (m_Connection != null)
                m_Connection.Dispose();
            m_Connection = null;

        }

        PlayerInfo ReadPlayerInfo(MySqlDataReader reader)
        {
            var playerInfo = new PlayerInfo();
            var playerInfoType = playerInfo.GetType();

            for (int iField = 0; iField < reader.FieldCount; iField++)
            {
                var fieldName = reader.GetName(iField);
                var property = playerInfoType.GetProperty(fieldName);
                if (property == null)
                    continue;

                switch (property.GetType().Name)
                {
                    case "string":
                        property.SetValue(playerInfo, reader.GetFieldValue<string>(iField));
                        break;
                    case "Int32":
                        property.SetValue(playerInfo, reader.GetFieldValue<Int32>(iField));
                        break;
                    case "UInt32":
                        property.SetValue(playerInfo, reader.GetFieldValue<UInt32>(iField));
                        break;
                    case "Int64":
                        property.SetValue(playerInfo, reader.GetFieldValue<Int64>(iField));
                        break;
                    case "UInt64":
                        property.SetValue(playerInfo, reader.GetFieldValue<UInt64>(iField));
                        break;
                    case "Single":
                        property.SetValue(playerInfo, reader.GetFieldValue<float>(iField));
                        break;
                    case "DateTime":
                        property.SetValue(playerInfo, reader.GetFieldValue<DateTime>(iField));
                        break;
                    default:
                        throw new Exception("Unhandled datatype: " + reader.GetFieldType(iField));
                }
            }

            return playerInfo;
        }

        GameLogInfo ReadGameLog(MySqlDataReader reader)
        {
            var logInfo = new GameLogInfo();
            var logInfoType = logInfo.GetType();

            for (int iField = 0; iField < reader.FieldCount; iField++)
            {
                var fieldName = reader.GetName(iField);
                var property = logInfoType.GetProperty(fieldName);
                if (property == null)
                    continue;

                switch (property.GetType().Name)
                {
                    case "string":
                        property.SetValue(logInfo, reader.GetFieldValue<string>(iField));
                        break;
                    case "Int32":
                        property.SetValue(logInfo, reader.GetFieldValue<Int32>(iField));
                        break;
                    case "UInt32":
                        property.SetValue(logInfo, reader.GetFieldValue<UInt32>(iField));
                        break;
                    case "Int64":
                        property.SetValue(logInfo, reader.GetFieldValue<Int64>(iField));
                        break;
                    case "UInt64":
                        property.SetValue(logInfo, reader.GetFieldValue<UInt64>(iField));
                        break;
                    case "Single":
                        property.SetValue(logInfo, reader.GetFieldValue<float>(iField));
                        break;
                    case "DateTime":
                        property.SetValue(logInfo, reader.GetFieldValue<DateTime>(iField));
                        break;
                    default:
                        throw new Exception("Unhandled datatype: " + reader.GetFieldType(iField));
                }
            }

            return logInfo;
        }

        public PlayerInfo QueryFindPlayer( UInt64 playerID )
        {
            if (m_Connection == null) return null;

            using(var cmd = new MySqlCommand())
            {
                cmd.Connection = m_Connection;
                cmd.CommandText = "select * from tbluser where PlayerID = @inPlayerID";
                cmd.Prepare();

                cmd.Parameters.AddWithValue("@inPlayerID", playerID);
                var reader = cmd.ExecuteReader();
                if (reader.NextResult())
                {
                    return ReadPlayerInfo(reader);
                }
            }

            return null;
        }

        public List<PlayerInfo> QueryFindPlayers(string pattern)
        {
            if (m_Connection == null) return null;

            var playerInfoList = new List<PlayerInfo>();
            using (var cmd = new MySqlCommand())
            {
                cmd.Connection = m_Connection;
                cmd.CommandText = "select * from tbluser where PlayerID like @inPattern";
                cmd.Prepare();

                cmd.Parameters.AddWithValue("@inPattern", pattern);
                var reader = cmd.ExecuteReader();
                if (reader.NextResult())
                {
                    playerInfoList.Add(ReadPlayerInfo(reader));
                }
            }

            return playerInfoList;
        }

        public List<GameLogInfo> QueryGameLogs(UInt64 playerID, DateTime from, DateTime to)
        {
            if (m_Connection == null) return null;

            var logInfoList = new List<GameLogInfo>();
            using (var cmd = new MySqlCommand())
            {
                cmd.Connection = m_Connection;
                cmd.CommandText = "select * from vgametransaction where PlayerID like @inPattern";
                cmd.Prepare();

                cmd.Parameters.AddWithValue("@inPlayerID", playerID);
                var reader = cmd.ExecuteReader();
                if (reader.NextResult())
                {
                    logInfoList.Add( ReadGameLog(reader));
                }
            }

            return logInfoList;
        }

    }


    public class DBConnectionPool
    {
        public string ConnectionString { get; set; }

        public DBConnection GetConnection()
        {
            var conn = new MySqlConnection(ConnectionString);
            if (conn == null) return null;
            conn.Open();
            return new DBConnection(conn);
        }
    }


}