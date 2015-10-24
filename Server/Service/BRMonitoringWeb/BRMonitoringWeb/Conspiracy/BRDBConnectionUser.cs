using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using MySql.Data.MySqlClient;
using BR;

namespace BR.DB
{

    public class DBConnectionConspiracyUser : IDisposable
    {
        DBConnection m_Conn;

        public DBConnectionConspiracyUser(DBConnection conn)
        {
            m_Conn = conn;
        }

        public void Dispose()
        {
            if (m_Conn != null)
                m_Conn.Dispose();
            m_Conn = null;
        }


        public List<UserInfo> FindUser(UInt64 userID)
        {
            if (m_Conn == null) return null;

            var results = new List<UserInfo>();
            using (var cmd = new MySqlCommand())
            {
                cmd.Connection = m_Conn.SqlConnection;
                cmd.CommandText = "select * from tbluser where UserUID = @inUserID";
                cmd.Prepare();

                cmd.Parameters.AddWithValue("@inUserID", userID);
                var reader = cmd.ExecuteReader();
                if (reader.NextResult())
                {
                    var newUserInfo = new UserInfo();
                    m_Conn.ReadRowProperties(newUserInfo, reader);
                    results.Add(newUserInfo);
                }
            }

            return results;
        }

        public List<UserInfo> FindUserByFBUID(UInt64 userFBID)
        {
            if (m_Conn == null) return null;

            var results = new List<UserInfo>();
            using (var cmd = new MySqlCommand())
            {
                cmd.Connection = m_Conn.SqlConnection;
                cmd.CommandText = "select * from tbluser where FBUserID like @inUserFBID";
                cmd.Prepare();

                cmd.Parameters.AddWithValue("@inUserFBID", userFBID);
                var reader = cmd.ExecuteReader();
                if (reader.NextResult())
                {
                    var newUserInfo = new UserInfo();
                    m_Conn.ReadRowProperties(newUserInfo, reader);
                    results.Add(newUserInfo);
                }
            }

            return results;
        }

        public List<UserInfo> FindUserByName(string userName)
        {
            if (m_Conn == null) return null;

            string searchPattern = "%" + userName + "%";

            var results = new List<UserInfo>();
            using (var cmd = new MySqlCommand())
            {
                cmd.Connection = m_Conn.SqlConnection;
                cmd.CommandText = "select * from tbluser where UserName like @inUserName";
                cmd.Prepare();

                cmd.Parameters.AddWithValue("@inUserName", searchPattern);
                var reader = cmd.ExecuteReader();
                if (reader.NextResult())
                {
                    var newUserInfo = new UserInfo();
                    m_Conn.ReadRowProperties(newUserInfo, reader);
                    results.Add(newUserInfo);
                }
            }

            return results;
        }

        public List<UserInfo> FindUserByEMail(string email)
        {
            if (m_Conn == null) return null;

            var results = new List<UserInfo>();
            using (var cmd = new MySqlCommand())
            {
                cmd.Connection = m_Conn.SqlConnection;
                cmd.CommandText = "select * from tbluser where EMail = @inEMail";
                cmd.Prepare();

                cmd.Parameters.AddWithValue("@inEMail", email.ToLower());
                var reader = cmd.ExecuteReader();
                if (reader.NextResult())
                {
                    var newUserInfo = new UserInfo();
                    m_Conn.ReadRowProperties(newUserInfo, reader);
                    results.Add(newUserInfo);
                }
            }

            return results;
        }

    }


}