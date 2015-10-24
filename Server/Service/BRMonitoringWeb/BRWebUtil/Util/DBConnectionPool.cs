using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using MySql.Data.MySqlClient;
using System.Text;

namespace BR.DB
{
    
    public class DBConnection : IDisposable
    {
        MySqlConnection m_SqlConnection;
        public MySqlConnection SqlConnection { get {return m_SqlConnection;} }

        public DBConnection( MySqlConnection conn )
        {
            m_SqlConnection = conn;
        }

        public virtual void OnDispose()
        {

        }

        public void Dispose()
        {
            OnDispose();

            if (m_SqlConnection != null)
                m_SqlConnection.Dispose();
            m_SqlConnection = null;
        }


        public bool DropTable(string tableName)
        {
            if (m_SqlConnection == null) return false;

            try
            {
                using (var cmd = new MySqlCommand())
                {
                    cmd.Connection = m_SqlConnection;
                    cmd.CommandText = string.Format("Drop Table {0};", tableName);
                    cmd.Prepare();
                    cmd.ExecuteNonQuery();
                }
            }
            catch(Exception exp)
            {
                System.Diagnostics.Debug.Print(exp.Message);
            }

            return true;
        }

        static public string GetDBType(Type type)
        {
            switch (type.Name)
            {
                case "Boolean": return "bit";
                case "String": return "varchar(256)";
                case "int": return "int";
                case "UInt64": return "bigint";
                case "Int32": return "int";
                case "Int64": return "bigint";
                case "long": return "bigint";
                default:
                    System.Diagnostics.Debug.Assert(type.IsEnum);
                    return "int";
            }

            //return "int";
        }

        public bool CreateTable(string tableName, string keyElementName, Type tableType)
        {
            if (SqlConnection == null) return false;
            if (tableType == null) return false;

            DropTable(tableName);

            // create table query
            StringBuilder queryString = new StringBuilder();
            queryString.AppendFormat("Create Table {0} (", tableName);
            string elementPostfix = "";

            var properties = tableType.GetProperties();
            foreach (var property in properties)
            {
                if (!property.CanRead) continue;
                queryString.AppendFormat("{0} {1} {2} not null", elementPostfix, property.Name, GetDBType(property.PropertyType));
                elementPostfix = ", ";
            }

            if (keyElementName != null)
            {
                queryString.AppendFormat("{0} PRIMARY KEY (`{1}`)", elementPostfix, keyElementName);
            }

            queryString.AppendFormat(");", tableName);

            using (var cmd = new MySqlCommand())
            {
                cmd.Connection = SqlConnection;
                cmd.CommandText = queryString.ToString();
                cmd.Prepare();

                var reader = cmd.ExecuteNonQuery();
            }

            return true;
        }

        public void ReadRowProperties(object dataObject, MySqlDataReader reader)
        {
            Type instanceType = dataObject.GetType();
            for (int iField = 0; iField < reader.FieldCount; iField++)
            {
                var fieldName = reader.GetName(iField);
                //var fieldType = reader.GetFieldType(iField);
                var property = instanceType.GetProperty(fieldName);
                if (property == null)
                    continue;

                var value = reader.GetValue(iField);
                property.SetValue(dataObject, Convert.ChangeType(value, property.PropertyType));
            }
        }

    }


    public class ConnectionPool
    {
        public string ConnectionString { get; private set; }

        public ConnectionPool(string connectionString)
        {
            ConnectionString = connectionString;
        }

        public DBConnection GetConnection()
        {
            var conn = new MySqlConnection(ConnectionString);
            if (conn == null) return null;
            conn.Open();
            return new DBConnection(conn);
        }
    }


}