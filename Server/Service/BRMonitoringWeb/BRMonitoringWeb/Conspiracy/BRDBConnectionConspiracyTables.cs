using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Text;
using MySql.Data.MySqlClient;
using BR;

namespace BR.DB
{


    public class DBConnectionConspiracyTable : IDisposable
    {
        DBConnection m_Conn;

        public DBConnectionConspiracyTable(DBConnection conn)
        {
            m_Conn = conn;
            if (m_Conn == null) throw new Exception("Null DB connection");
        }

        public void Dispose()
        {
            if (m_Conn != null)
                m_Conn.Dispose();
            m_Conn = null;
        }

        public bool CreateTable(string tableName, string keyElementName, Type tableType)
        {
            return m_Conn.CreateTable("gtbl_" + tableName, keyElementName, tableType);
        }

        public bool DropTable(string tableName)
        {
            return m_Conn.DropTable("gtbl_" + tableName);
        }

        public bool UploadTableData(string tableName, Array tableObjects)
        {
            if (m_Conn == null) return false;
            if (tableObjects == null) return false;

            foreach (var tableObject in tableObjects)
            {
                StringBuilder queryString = new StringBuilder();
                queryString.AppendFormat("insert into gtbl_{0} (", tableName);
                string elementPostfix = "";

                var properties = tableObject.GetType().GetProperties();
                foreach (var property in properties)
                {
                    if (!property.CanRead) continue;
                    queryString.AppendFormat("{0} `{1}`", elementPostfix, property.Name);
                    elementPostfix = ", ";
                }

                queryString.AppendFormat(")");
                queryString.AppendFormat("VALUES (");
                elementPostfix = "";
                foreach (var property in properties)
                {
                    if (!property.CanRead) continue;
                    if(property.PropertyType == typeof(string))
                        queryString.AppendFormat("{0} '{1}'", elementPostfix, property.GetValue(tableObject));
                    else if (property.PropertyType.IsEnum)
                        queryString.AppendFormat("{0} {1}", elementPostfix, Convert.ChangeType(property.GetValue(tableObject),typeof(int)));
                    else
                        queryString.AppendFormat("{0} {1}", elementPostfix, property.GetValue(tableObject));
                    elementPostfix = ", ";
                }

                queryString.AppendFormat(");", tableName);

                using (var cmd = new MySqlCommand())
                {
                    cmd.Connection = m_Conn.SqlConnection;
                    cmd.CommandText = queryString.ToString();
                    cmd.Prepare();

                    var reader = cmd.ExecuteNonQuery();
                }

            }

            return true;
        }

    }

}

