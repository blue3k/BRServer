using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using MySql.Data.MySqlClient;


namespace BR
{
    
    public class ShardInfo
    {
        public string ShardID { get; set; }
        public int WorkLoad { get; set; }
        public string DBName { get; set; }
        public string TableName { get; set; }
    }
    public class UserInfo
    {
        public string UserID { get; set; }
        public string UserName { get; set; }
        public string UserPassword { get; set; }
        public UInt64 FBUserID { get; set; }
        public DateTime LatestLoggedIn { get; set; }
        public DateTime CreatedDate { get; set; }
        public string LoginToken { get; set; }
        public string EMail { get; set; }
        public string GCMKeys { get; set; }
        public int ShardID { get; set; }
        public string CellPhone { get; set; }
    }

    public class FriendInfo
    {
        public UInt64 UserUID { get; set; }
        public UInt64 FriendUID { get; set; }
        public UInt64 FriendFacebookUID { get; set; }
        public UInt64 FriendStaminaTime { get; set; }
        public int FriendShardID { get; set; }
    }

    public class NotificationInfo
    {
        public int NotificationID { get; set; }
        public UInt64 UserUID { get; set; }
        public int MessageID { get; set; }
        public UInt64 MessageParam0 { get; set; }
        public UInt64 MessageParam1 { get; set; }
        public string MessageText { get; set; }
        public DateTime IssuedTime { get; set; }
        public int Read { get; set; }
        public UInt64 TimeStamp { get; set; }
    }

    public class PlayerInfo
    {
        public UInt64 PlayerID { get; set; }
        public int Grade { get; set; }
        public int Level { get; set; }
        public UInt64 Exp { get; set; }
        public UInt64 Money { get; set; }
        public UInt64 gem { get; set; }
        public int Stamina { get; set; }
        public int TotalPlayed { get; set; }
        public int WinPlaySC { get; set; }
        public int WinPlaySM { get; set; }
        public int WinPlaySS { get; set; }
        public int LosePlaySC { get; set; }
        public int LosePlaySM { get; set; }
        public int LosePlaySS { get; set; }
        public int WinPlayNC { get; set; }
        public int WinPlayNM { get; set; }
        public int WinPlayNS { get; set; }
        public int LosePlayNC { get; set; }
        public int LosePlayNM { get; set; }
        public int LosePlayNS { get; set; }
        public DateTime Updated { get; set; }
        public int AddedFriendSlot { get; set; }
        public UInt64 LatestTickTime { get; set; }
        public int LatestActiveTime { get; set; }
        public int WeeklyWin { get; set; }
        public int WeeklyLose { get; set; }
        public int PlayState { get; set; }
        public string GameNick { get; set; }
        public string ComplitionState { get; set; }
    }

    public class UserPurchaseTokenInfo
    {
        public DateTime PurchaseTime{ get; set; }
        public byte[] PurchaseID { get; set; }
        public string PurchaseToken { get; set; }
        public UInt64 PlayerID { get; set; }
    }


    public class GameLogInfo
    {
        public DateTime LogTime { get; set; }
        public int GameTime { get; set; }
        public UInt64 PlayerID { get; set; }
        public char LogCategory { get; set; }
        public int Consume { get; set; }
        public int Gain { get; set; }
        public UInt64 TotalValue { get; set; }
        public string LogMessage { get; set; }
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