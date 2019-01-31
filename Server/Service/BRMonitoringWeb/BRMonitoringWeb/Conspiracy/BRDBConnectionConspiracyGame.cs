using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using MySql.Data.MySqlClient;
using BR;

namespace BR.DB
{


    public class DBConnectionConspiracyGame : IDisposable
    {
        DBConnection m_Conn;

        public DBConnectionConspiracyGame(DBConnection conn)
        {
            m_Conn = conn;
        }

        public void Dispose()
        {
            if (m_Conn != null)
                m_Conn.Dispose();
            m_Conn = null;
        }


        public PlayerInfo QueryFindPlayer(UInt64 playerID)
        {
            if (m_Conn == null) return null;

            using (var cmd = new MySqlCommand())
            {
                cmd.Connection = m_Conn.SqlConnection;
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

        PlayerInfo ReadPlayerInfo(MySqlDataReader reader)
        {
            PlayerInfo newPlayerInfo = new PlayerInfo();

            m_Conn.ReadRowProperties(newPlayerInfo, reader);

            return newPlayerInfo;
        }

    }


}