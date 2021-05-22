using System;
using System.IO;
using System.Collections.Generic;
using SF;
using SF.Tool;
using Azure.Core;
using Azure.Identity;
using Azure.Storage;
using Azure.Storage.Blobs;
using Azure.Storage.Blobs.Models;
using Azure.Storage.Sas;
using MySql.Data.MySqlClient;
using System.Data;
using System.Text;
using System.Threading.Tasks;
using MySqlX.XDevAPI;

namespace BR
{
    public class VersionDB
    {
        readonly string m_ConnectionStringFormat = "server={0};port={1};user={2};password={3};";
        string m_ConnectionString;
        string m_DBName;

        VersionControlPath m_PathControl;


        public VersionDB(VersionControlPath pathControl)
        {
            m_PathControl = pathControl;
            m_DBName = AppConfig.GetValue<string>("DBDatabase");

            // Open
            if (string.IsNullOrEmpty(m_DBName))
            {
                System.Diagnostics.Process.Start(AppConfig.LocalConfigPath);
            }

            m_ConnectionString = string.Format(m_ConnectionStringFormat,
                AppConfig.GetValue<string>("DBServer"),
                AppConfig.GetValue<string>("DBPort"),
                AppConfig.GetValue<string>("DBUser"),
                AppConfig.GetValue<string>("DBPw"));

        }

        public MySqlX.XDevAPI.Session OpenSession()
        {
            try
            {
                var session = MySQLX.GetSession(m_ConnectionString);
                session.SetCurrentSchema(m_DBName);
                return session;
            }
            catch (Exception exp)
            {
                SF.Log.Error("{0} => {1}", exp.Message, exp.StackTrace.ToString());
            }

            return null;
        }

        public List<IVersionControl.VersionInfo> GetVersionList(Int64 minChangeNumber = -1)
        {
            var result = new List<IVersionControl.VersionInfo>();
            using (var session = OpenSession())
            {
                try
                {
                    var queryResult = session.SQL("call spGetChangeList(?);").Bind(minChangeNumber).Execute();
                    while (queryResult.Next())
                    {
                        var row = queryResult.Current;

                        var item = new IVersionControl.VersionInfo();

                        item.ChangeNumber = (Int64)Convert.ChangeType(row["ChangeNumber"], typeof(Int64));
                        item.Description = row["Description"] as string;
                        item.User = row["User"] as string;
                        item.Modified = (DateTime)Convert.ChangeType(row["Commited"], typeof(DateTime));

                        result.Add(item);
                    }

                    session.Close();
                }
                catch (Exception exp)
                {
                    SF.Log.Error("{0} => {1}", exp.Message, exp.StackTrace.ToString());
                }
            }

            return result;
        }

        public List<VersionFileInfo> GetFileList(string remotePrefix = null)
        {
            string remotePath = m_PathControl.NormalizeRemotePath(remotePrefix);

            var result = new List<VersionFileInfo>();

            using (var session = OpenSession())
            {
                try
                {
                    if (!string.IsNullOrEmpty(remotePath))
                        remotePath = "%";
                    else
                        remotePath = remotePath + "%";

                    var queryResult = session.SQL("call spGetFileList(?);").Bind(remotePath).Execute();
                    while (queryResult.Next())
                    {
                        var row = queryResult.Current;

                        var item = new VersionFileInfo();

                        item.RemoteFilePath = row["FilePath"] as string;
                        item.LocalFilePath = m_PathControl.ToLocalPath(item.RemoteFilePath);
                        item.FileVersion = (int)Convert.ChangeType(row["Version"], typeof(int));
                        item.Size = (long)Convert.ChangeType(row["FileSize"], typeof(long));
                        item.Modified = ((DateTime)row["LastModified"]).ToUniversalTime();

                        result.Add(item);
                    }

                    session.Close();
                }
                catch (Exception exp)
                {
                    SF.Log.Error("{0} => {1}", exp.Message, exp.StackTrace.ToString());
                }
            }

            return result;
        }

        public void UpdateFileLocks(List<VersionFileInfo> fileInfos, bool bIsLock)
        {
            using (var session = OpenSession())
            {
                try
                {
                    UpdateFileLocks(session, fileInfos, bIsLock);

                    session.Close();
                }
                catch (Exception exp)
                {
                    SF.Log.Error("{0} => {1}", exp.Message, exp.StackTrace.ToString());
                }
            }
        }

        public void UpdateFileLocks(MySqlX.XDevAPI.Session session, List<VersionFileInfo> fileInfos, bool bIsLock)
        {
            var lockUser = bIsLock ? AppConfig.GetValue<string>("DBUser") : null;
            
            foreach (var fileInfo in fileInfos)
            {
                session.SQL("call spLockFile(?, ?, @outResult);").Bind(fileInfo.RemoteFilePath, lockUser).Execute();
                var queryResult = session.SQL("select @outResult;").Execute();
                var row = queryResult.FetchOne();
                var res = (int)Convert.ChangeType(row[0], typeof(int));
                if (res != 0)
                {
                    throw new Exception(string.Format("Query spLockFile has failed with result:{0}", res));
                }
            }
        }

        void ReserveChangeNumber(MySqlX.XDevAPI.Session session, string userName, out Int64 changeNumber)
        {
            changeNumber = 0;
            session.SQL("call spReserveChange(?, @changeNumber);")
                                .Bind(userName).Execute();
            var queryResult = session.SQL("select @changeNumber;").Execute();
            var row = queryResult.FetchOne();

            changeNumber = (Int64)Convert.ChangeType(row[0], typeof(Int64));
            if (changeNumber == 0)
            {
                throw new Exception(string.Format("Query spReserveChange has failed with changeNumber:{0}", changeNumber));
            }
        }

        void UpdateCommitChange(MySqlX.XDevAPI.Session session, string fullDescription, Int64 changeNumber)
        {
            session.SQL("call spCommitChange(?, ?, @outResult);")
                                .Bind(changeNumber, fullDescription)
                                .Execute();
            var queryResult = session.SQL("select @outResult;").Execute();
            var row = queryResult.FetchOne();
            var res = (int)Convert.ChangeType(row[0], typeof(int));

            var outResult = (Int32)Convert.ChangeType(row[0], typeof(Int32)); ;
            if (outResult != 0)
            {
                throw new Exception(string.Format("Query spCommitChange has failed with result:{0}", outResult));
            }
        }

        void UpdateFileInfo(MySqlX.XDevAPI.Session session, Int64 changeNumber, List<VersionFileInfo> fileInfos)
        {
            foreach (var fileInfo in fileInfos)
            {
                session.SQL("call spUpdateFileInfo(?, ?, ?, ?, @outResult);")
                    .Bind(fileInfo.RemoteFilePath, changeNumber, fileInfo.Size, fileInfo.Deleted ? 1 : 0)
                    .Execute();
                var queryResult = session.SQL("select @outResult;").Execute();
                var row = queryResult.FetchOne();
                var res = (int)Convert.ChangeType(row[0], typeof(Int32));
                if (res != 0)
                {
                    throw new Exception(string.Format("Query spUpdateFileInfo has failed with result:{0}", res));
                }
            }
        }


        public bool CommitChange(string userName, string desc, List<VersionFileInfo> fileInfos, out Int64 changeNumber)
        {
            changeNumber = 0;

            using (var session = OpenSession())
            {
                session.StartTransaction();
                try
                {
                    CommitChange(session, userName, desc, fileInfos, out changeNumber);

                    session.Commit();
                    session.Close();
                }
                catch (Exception exp)
                {
                    session.Rollback();
                    Log.Error("{0} => {1}", exp.Message, exp.StackTrace.ToString());
                    return false;
                }
            }

            return true;
        }

        public bool CommitChange(MySqlX.XDevAPI.Session session, string userName, string desc, List<VersionFileInfo> fileInfos, out Int64 changeNumber)
        {
            changeNumber = 0;
            StringBuilder fullDescription = new StringBuilder();
            fullDescription.AppendFormat("{0}\n\n", desc);

            fullDescription.Append("FileList\n");
            foreach (var fileInfo in fileInfos)
            {
                fullDescription.AppendFormat(" {0}:{1} {2}\n", fileInfo.RemoteFilePath, fileInfo.FileVersion, fileInfo.Deleted ? "Deleted" : "");
            }

            ReserveChangeNumber(session, userName, out changeNumber);
            UpdateFileInfo(session, changeNumber, fileInfos);
            UpdateCommitChange(session, fullDescription.ToString(), changeNumber);

            return true;
        }
    }

}


