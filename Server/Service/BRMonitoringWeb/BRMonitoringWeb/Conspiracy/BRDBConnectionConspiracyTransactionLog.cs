using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using MySql.Data.MySqlClient;
using BR;

namespace BR.DB
{
    
    public class DBConnectionConspiracyTransaction : IDisposable
    {
        DBConnection m_Conn;

        public DBConnectionConspiracyTransaction(DBConnection conn)
        {
            m_Conn = conn;
        }

        public void Dispose()
        {
            if (m_Conn != null)
                m_Conn.Dispose();
            m_Conn = null;
        }


        PlayerInfo ReadPlayerInfo(MySqlDataReader reader)
        {
            PlayerInfo newPlayerInfo = new PlayerInfo();

            m_Conn.ReadRowProperties(newPlayerInfo, reader);

            return newPlayerInfo;
        }

    }



}