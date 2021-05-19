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


namespace BR
{
    public class VersionDB
    {
        readonly string m_ConnectionStringFormat = "server={0};database={1};uid={2};pwd={3}";
        string m_ConnectionString;

        VersionControlPath m_PathControl;


        public VersionDB(VersionControlPath pathControl)
        {
            m_PathControl = pathControl;

            m_ConnectionString = string.Format(m_ConnectionStringFormat,
                AppConfig.GetValue<string>("DBServer"),
                AppConfig.GetValue<string>("DBDatabase"),
                AppConfig.GetValue<string>("DBUser"),
                AppConfig.GetValue<string>("DBPw"));
        }

        public MySqlConnection Open()
        {
            MySqlConnection conn = null;

            try
            {
                conn = new MySqlConnection(m_ConnectionString);
                conn.Open();
            }
            catch (MySqlException ex)
            {
                Console.WriteLine(ex.Message);
                //MessageBox.Show(ex.Message);
            }

            return conn;
        }

        public List<IVersionControl.FileItem> GetFileList(string remotePrefix = null)
        {
            string remotePath = m_PathControl.NormalizeRemotePath(remotePrefix);

            var result = new List<IVersionControl.FileItem>();

            using (var conn = Open())
            using(MySqlCommand cmd = new MySqlCommand("spGetFileList"))
            {
                try
                {
                    cmd.Connection = conn;
                    cmd.CommandType = CommandType.StoredProcedure;

                    if (string.IsNullOrEmpty(remotePath))
                        remotePath = "%";
                    else
                        remotePath = remotePath + "%";
                    cmd.Parameters.AddWithValue("inPrefixPattern", remotePath);
                    cmd.Parameters["inPrefixPattern"].Direction = ParameterDirection.Input;

                    using(MySqlDataReader reader = cmd.ExecuteReader())
                    {
                        while (reader.Read())
                        {
                            IVersionControl.FileItem item = new IVersionControl.FileItem();

                            item.RemoteFilePath = reader["RemoteFilePath"] as string;
                            item.LocalFilePath = m_PathControl.ToLocalPath(item.RemoteFilePath);
                            item.FileVersion = (int)Convert.ChangeType(reader["Version"], typeof(int));
                            item.Size = (long)Convert.ChangeType(reader["FileSize"], typeof(long));
                            item.Modified = (DateTimeOffset)Convert.ChangeType(reader["DateTime"], typeof(DateTimeOffset));

                            result.Add(item);
                        }
                    }
                }
                catch(MySqlException ex)
                {
                    Console.WriteLine(ex.Message);
                }

            }
            
            return result;
        }


        void ReserveChangeNumber(MySqlTransaction transaction, string userName, out Int64 changeNumber)
        {
            MySqlConnection conn = transaction != null ? transaction.Connection : Open();
            try
            {
                using (MySqlCommand cmd = new MySqlCommand("spReserveChange"))
                {
                    cmd.Connection = conn;
                    cmd.Transaction = transaction;
                    cmd.CommandType = CommandType.StoredProcedure;

                    cmd.Parameters.AddWithValue("inUserName", userName);
                    cmd.Parameters["inUserName"].Direction = ParameterDirection.Input;

                    cmd.Parameters.Add("outChangeNumber", MySqlDbType.Int64);
                    cmd.Parameters["outChangeNumber"].Direction = ParameterDirection.Output;

                    //cmd.Parameters.Add("outResult", MySqlDbType.Int32);
                    //cmd.Parameters["outResult"].Direction = ParameterDirection.Output;

                    cmd.ExecuteNonQuery();

                    //var outResult = (int)cmd.Parameters["outResult"].Value;
                    //if (outResult != 0)
                    //{
                    //    throw new Exception(string.Format("Query spUpdateFileInfo has failed with result:{0}", outResult));
                    //}

                    changeNumber = (Int64)cmd.Parameters["outChangeNumber"].Value;
                    if (changeNumber != 0)
                    {
                        throw new Exception(string.Format("Query spReserveChange has failed with changeNumber:{0}", changeNumber));
                    }
                }
            }
            finally
            {
                if (transaction == null && conn != null)
                    conn.Dispose();
            }
        }

        void CommitChange(MySqlTransaction transaction, string fullDescription, Int64 changeNumber)
        {
            MySqlConnection conn = transaction != null ? transaction.Connection : Open();
            try
            {
                using (MySqlCommand cmd = new MySqlCommand("spCommitChange"))
                {
                    cmd.Connection = conn;
                    cmd.Transaction = transaction;
                    cmd.CommandType = CommandType.StoredProcedure;

                    cmd.Parameters.AddWithValue("inChangeNumber", changeNumber);
                    cmd.Parameters["inChangeNumber"].Direction = ParameterDirection.Input;

                    cmd.Parameters.AddWithValue("inDescription", fullDescription);
                    cmd.Parameters["inDescription"].Direction = ParameterDirection.Input;

                    cmd.Parameters.Add("outResult", MySqlDbType.Int32);
                    cmd.Parameters["outResult"].Direction = ParameterDirection.Output;

                    cmd.ExecuteNonQuery();

                    var outResult = (int)cmd.Parameters["outResult"].Value;
                    if (outResult != 0)
                    {
                        throw new Exception(string.Format("Query spCommitChange has failed with result:{0}", outResult));
                    }
                }
            }
            finally
            {
                if (transaction == null && conn != null)
                    conn.Dispose();
            }
        }

        void UpdateFileInfo(MySqlTransaction transaction, Int64 changeNumber, List<IVersionControl.FileItem> fileInfos)
        {
            MySqlConnection conn = transaction != null ? transaction.Connection : Open();
            try
            {
                using (MySqlCommand cmd = new MySqlCommand("spUpdateFileInfo"))
                {
                    cmd.Connection = conn;
                    cmd.Transaction = transaction;
                    cmd.CommandType = CommandType.StoredProcedure;

                    cmd.Parameters.Add("inRemoteFilePath", MySqlDbType.VarChar);
                    cmd.Parameters["inRemoteFilePath"].Direction = ParameterDirection.Input;

                    cmd.Parameters.Add("inChangeNumber", MySqlDbType.Int64);
                    cmd.Parameters["inChangeNumber"].Direction = ParameterDirection.Input;

                    cmd.Parameters.AddWithValue("inFileSize", MySqlDbType.Int64);
                    cmd.Parameters["inFileSize"].Direction = ParameterDirection.Input;

                    cmd.Parameters.AddWithValue("inDeleted", MySqlDbType.Byte);
                    cmd.Parameters["inDeleted"].Direction = ParameterDirection.Input;

                    cmd.Parameters.Add("outResult", MySqlDbType.Int32);
                    cmd.Parameters["outResult"].Direction = ParameterDirection.Output;

                    foreach (var fileInfo in fileInfos)
                    {
                        cmd.Parameters["inRemoteFilePath"].Value = fileInfo.RemoteFilePath;
                        cmd.Parameters["inChangeNumber"].Value = changeNumber;
                        cmd.Parameters["inFileSize"].Value = fileInfo.Size;
                        cmd.Parameters["inDeleted"].Value = fileInfo.Deleted ? 1 : 0;
                        cmd.Parameters["outResult"].Value = 0;

                        cmd.ExecuteNonQuery();

                        var outResult = (int)cmd.Parameters["outResult"].Value;
                        if (outResult != 0)
                        {
                            throw new Exception(string.Format("Query spUpdateFileInfo has failed with result:{0}", outResult));
                        }
                    }
                }
            }
            finally
            {
                if (transaction == null && conn != null)
                    conn.Dispose();
            }
        }


        public bool CommitChange(string desc, List<IVersionControl.FileItem> fileInfos, out Int64 changeNumber)
        {
            changeNumber = 0;
            StringBuilder fullDescription = new StringBuilder();
            fullDescription.AppendFormat("{0}\n\n", desc);

            foreach (var fileInfo in fileInfos)
            {
                fullDescription.AppendFormat("\t{0}\n", fileInfo.RemoteFilePath, fileInfo.FileVersion, fileInfo.FileVersion, fileInfo.Deleted ? "Deleted" : "");
            }


            using (var conn = Open())
            using (var transaction = conn.BeginTransaction())
            {
                try
                {
                    ReserveChangeNumber(transaction, fullDescription.ToString(), out changeNumber);
                    UpdateFileInfo(transaction, changeNumber, fileInfos);
                    CommitChange(transaction, fullDescription.ToString(), changeNumber);

                    transaction.Commit();
                }
                catch (Exception exp)
                {
                    transaction.Rollback();
                    Console.WriteLine(exp.Message);
                    return false;
                }
            }

            return true;
        }
    }

    public class VersionFileStorage
    {
        static DefaultAzureCredentialOptions m_CredentialOptions = new DefaultAzureCredentialOptions()
        {
            InteractiveBrowserTenantId = "909b5db3-4dbe-40eb-8f04-870b21991923",
            ExcludeInteractiveBrowserCredential = false,
            ExcludeAzureCliCredential = false
        };
        TokenCredential m_Credential = new Azure.Identity.DefaultAzureCredential(m_CredentialOptions);
        readonly Uri m_AccountUri = new Uri("https://bravesstorage.blob.core.windows.net/fishing");

        VersionControlPath m_PathControl;

        BlobContainerClient m_FileStorage;

        public VersionFileStorage(VersionControlPath pathControl)
        {
            m_PathControl = pathControl;
            m_FileStorage = new BlobContainerClient(m_AccountUri, m_Credential);

            m_FileStorage.GetBlobs(prefix: ".git");// kick off connect&login by accessing something
        }

        public List<IVersionControl.FileItem> GetFileList(string remotePrefix = null)
        {
            var result = new List<IVersionControl.FileItem>();
            Azure.Pageable<BlobItem> blobs;
            blobs = m_FileStorage.GetBlobs(prefix: remotePrefix);
            foreach (BlobItem blobItem in blobs)
            {
                if (blobItem.Deleted)
                    continue;

                IVersionControl.FileItem item = new IVersionControl.FileItem();

                item.RemoteFilePath = blobItem.Name;
                item.LocalFilePath = m_PathControl.ToLocalPath(item.RemoteFilePath);
                item.FileVersion = 0;
                item.Size = blobItem.Properties.ContentLength?? 0;
                item.Modified = blobItem.Properties.LastModified?? DateTimeOffset.MinValue;

                result.Add(item);
            }

            return result;
        }

        public void UploadFiles(List<IVersionControl.FileItem> fileInfos)
        {
            foreach(var fileInfo in fileInfos)
            {
                if (string.IsNullOrEmpty(fileInfo.RemoteFilePath))
                {
                    throw new Exception("Invalid file path for uploading");
                }

                m_FileStorage.UploadBlob(fileInfo.RemoteFilePath, File.OpenRead(fileInfo.LocalFilePath));
            }
        }

        public void DownloadFiles(List<IVersionControl.FileItem> fileInfos)
        {
            List<Task<Azure.Response>> downloadTasks = new List<Task<Azure.Response>>();
            foreach (var fileInfo in fileInfos)
            {
                if (string.IsNullOrEmpty(fileInfo.RemoteFilePath))
                {
                    throw new Exception("Invalid file path for uploading");
                }

                var blobClient = m_FileStorage.GetBlobClient(fileInfo.RemoteFilePath);
                downloadTasks.Add(blobClient.DownloadToAsync(fileInfo.LocalFilePath));
            }

            Task.WaitAll(downloadTasks.ToArray());
        }
    }

    public class FileIgnoreList
    {

        HashSet<string> m_IgnoreExts = new HashSet<string>();
        HashSet<string> m_IgnoreFolder = new HashSet<string>();

        public void LoadIgnore(string ignoreFile)
        {
            char[] endlineChars = { ' ', '\t', '#' };

            try
            {
                using (StreamReader reader = new StreamReader(ignoreFile))
                {
                    string line;
                    do
                    {
                        line = reader.ReadLine();

                        line = line.TrimStart();
                        var end = line.IndexOfAny(endlineChars);
                        if (end >= 0)
                        {
                            line = line.Substring(0, end);
                        }

                        if (string.IsNullOrEmpty(line))
                            continue;

                        if (line.StartsWith('*'))
                        {
                            var ext = Path.GetExtension(line);
                            AddIgnoreExt(ext);
                        }
                        else if (line.EndsWith('/'))
                        {
                            AddIgnoreFolder(line);
                        }

                    } while (!reader.EndOfStream);
                }
            }
            catch(Exception exp)
            {
                Console.WriteLine(exp.Message);
                //exp.Message;
            }
        }

        public void AddIgnoreExt(string ignoreExt)
        {
            ignoreExt = ignoreExt.ToLower();
            if (m_IgnoreExts.Contains(ignoreExt)) return;

            m_IgnoreExts.Add(ignoreExt);
        }

        public void AddIgnoreFolder(string folder)
        {
            folder = folder.ToLower();
            if (m_IgnoreFolder.Contains(folder)) return;

            m_IgnoreFolder.Add(folder);
        }

        public bool IsIgnore(string RemoteFilePath)
        {
            var fileFullPath = Path.GetFullPath(RemoteFilePath);
            var fileExt = Path.GetExtension(fileFullPath);

            if (m_IgnoreExts.Contains(fileExt)) return true;

            try
            {
                FileAttributes localFileAttr = File.GetAttributes(fileFullPath);
                var directoryPath = localFileAttr.HasFlag(FileAttributes.Directory) ? fileFullPath : Path.GetDirectoryName(fileFullPath);
                var dirNames = directoryPath.Split(Path.DirectorySeparatorChar);
                foreach (var dirName in dirNames)
                {
                    var dirExt = Path.GetExtension(fileFullPath);
                    if (m_IgnoreExts.Contains(dirExt)) return true;
                    if (m_IgnoreFolder.Contains(dirName.ToLower())) return true;
                }
            }
            catch (Exception)
            {
                // maybe no local file, ignore it
                return true;
            }

            return false;
        }

    }

    /// <summary>
    /// Braves version control system
    /// </summary>
    public class VersionControlClient
    {
        VersionControlPath m_PathControl;
        VersionFileStorage m_FileStorage;
        VersionDB m_VersionDB;
        FileIgnoreList m_Ignore = new FileIgnoreList();

        HashSet<string> m_UpdatedFiles = new HashSet<string>();
        HashSet<string> m_DeletedFiles = new HashSet<string>();

        public delegate void delLogFunction(string fromat, params object[] args);
        public delLogFunction Log;

        public VersionControlClient()
        {
            m_PathControl = new VersionControlPath();
            m_FileStorage = new VersionFileStorage(m_PathControl);
            m_VersionDB = new VersionDB(m_PathControl);
        }

        public void Initialize(string localPath, string remotePath, string serverAddress)
        {
            m_PathControl.LocalBasePath = localPath;
            m_PathControl.RemoteBasePath = remotePath;

            m_Ignore.LoadIgnore(Path.Combine(m_PathControl.LocalBasePath, ".gitignore"));

            //GetFileList(m_PathControl.RemoteBasePath, ListOption.Recursive);

        }

        public VersionControlPath PathControl { get { return m_PathControl; } }



        /// <summary>
        /// 
        /// </summary>
        /// <param name="fileName"></param>
        public void AddUpdatedFile(string fileName)
        {
            fileName = m_PathControl.NormalizeLocalPath(fileName);

            if (m_Ignore.IsIgnore(fileName))
                return;

            lock (m_UpdatedFiles)
            {
                if (!m_UpdatedFiles.Contains(fileName))
                    m_UpdatedFiles.Add(fileName);

                if (m_DeletedFiles.Contains(fileName))
                    m_DeletedFiles.Remove(fileName);
            }

        }

        public void AddDeletedFile(string fileName)
        {
            fileName = m_PathControl.NormalizeLocalPath(fileName);
            if (m_Ignore.IsIgnore(fileName))
                return;

            lock (m_UpdatedFiles)
            {
                if (m_UpdatedFiles.Contains(fileName))
                    m_UpdatedFiles.Remove(fileName);

                if (!m_DeletedFiles.Contains(fileName))
                    m_DeletedFiles.Add(fileName);
            }
        }

        public IList<IVersionControl.FileItem> GetFileList()
        {
            List<IVersionControl.FileItem> result = new List<IVersionControl.FileItem>();
            // Create a client that can authenticate using our token credential
            result = m_VersionDB.GetFileList();

            return result;
        }


        public void GetLatest()
        {
            var fileList = m_VersionDB.GetFileList();
            m_FileStorage.DownloadFiles(fileList);
        }

        public void ReconcileLocalChanges()
        {
            var remoteFileList = m_VersionDB.GetFileList();
            var remoteFileState = new Dictionary<string, IVersionControl.FileItem>();
            foreach (var remoteFile in remoteFileList)
            {
                remoteFileState.Add(remoteFile.LocalFilePath, remoteFile);
            }


            string[] localFiles = Directory.GetFileSystemEntries(m_PathControl.LocalBasePath, "*", SearchOption.AllDirectories);
            HashSet<string> localFileState = new HashSet<string>();
            foreach (var fileFullPath in localFiles)
            {
                var localFileInfo = new FileInfo(fileFullPath);
                // remove milliseconds from local modified time
                var localModified = new DateTimeOffset(localFileInfo.LastWriteTimeUtc.Year, localFileInfo.LastWriteTimeUtc.Month, localFileInfo.LastWriteTimeUtc.Day, localFileInfo.LastWriteTimeUtc.Hour, localFileInfo.LastWriteTimeUtc.Minute, localFileInfo.LastWriteTimeUtc.Second, new TimeSpan(0));
                var localNormalizedPath = m_PathControl.NormalizeLocalPath(fileFullPath);

                localFileState.Add(localNormalizedPath);

                IVersionControl.FileItem remoteFileInfo;

                if (!remoteFileState.TryGetValue(localNormalizedPath, out remoteFileInfo)
                    || remoteFileInfo.Size != localFileInfo.Length
                    || remoteFileInfo.Modified.ToUniversalTime() < localModified)
                {
                    AddUpdatedFile(localNormalizedPath);
                }
            }

            foreach (var remoteFile in remoteFileList)
            {
                if (!localFileState.Contains(remoteFile.LocalFilePath))
                    AddDeletedFile(remoteFile.LocalFilePath);
            }

            // Check current update list and remove invalid
            CleanupUpdateList(remoteFileState, localFileState);
        }

        void CleanupUpdateList(Dictionary<string, IVersionControl.FileItem> remoteFileState, HashSet<string> localFileState)
        {
            List<string> removeList = new List<string>();
            foreach (var file in m_DeletedFiles)
            {
                if (!remoteFileState.ContainsKey(file) || localFileState.Contains(file))
                {
                    removeList.Add(file);
                }
            }
            foreach (var file in removeList)
            {
                m_DeletedFiles.Remove(file);
            }

            removeList.Clear();
            foreach (var file in m_UpdatedFiles)
            {
                if (!localFileState.Contains(file))
                {
                    removeList.Add(file);
                }
            }
            foreach (var file in removeList)
            {
                m_UpdatedFiles.Remove(file);
            }
        }

        public Result SubmitChanges(string description)
        {
            var fileList = new List<IVersionControl.FileItem>();
            foreach (var file in m_DeletedFiles)
            {
                var fileItem = new IVersionControl.FileItem();
                fileItem.LocalFilePath = file;
                fileItem.RemoteFilePath = m_PathControl.ToRemotePath(file);
                fileItem.Size = 0;
                fileItem.Deleted = true;

                fileList.Add(fileItem);
            }

            foreach (var file in m_UpdatedFiles)
            {
                var fileItem = new IVersionControl.FileItem();
                fileItem.LocalFilePath = file;
                fileItem.RemoteFilePath = m_PathControl.ToRemotePath(file);
                fileItem.Size = 0;
                fileItem.Deleted = false;

                fileList.Add(fileItem);
            }

            Int64 changeNumber;
            m_FileStorage.UploadFiles(fileList);
            m_VersionDB.CommitChange(description, fileList, out changeNumber);

            return new Result(ResultCode.SUCCESS);
        }


        //public void SyncFull(bool bForce = false)
        //{
        //    Log("Full sync...");
        //    HashSet<string> fileList = new HashSet<string>();
        //    HashSet<string> directoryList = new HashSet<string>();

        //    string[] files = Directory.GetFileSystemEntries(m_PathControl.LocalBasePath, "*", SearchOption.AllDirectories);

        //    // grab all remote information
        //    Log("Updating remote file list...");
        //    var remoteFileList = GetFileList(m_PathControl.RemoteBasePath, IVersionControl.ListOption.Recursive);
        //    var remoteFileMap = new Dictionary<string, IVersionControl.FileItem>();
        //    foreach (var remoteFile in remoteFileList)
        //    {
        //        remoteFileMap.Add(remoteFile.RemoteFilePath, remoteFile);
        //    }

        //    foreach (var file in files)
        //    {
        //        if (m_Ignore.IsIgnore(file)) continue;

        //        var fileFullPath = Path.GetFullPath(file);
        //        var relativePath = m_PathControl.NormalizeLocalPath(fileFullPath);
        //        var remotePath = m_PathControl.ToRemotePath(fileFullPath);

        //        // Check whether it's a directory
        //        FileAttributes localFileAttr = File.GetAttributes(fileFullPath);
        //        if (localFileAttr.HasFlag(FileAttributes.Directory))
        //        {
        //            directoryList.Add(relativePath);
        //            //if (!m_FTPClient.DirectoryExists(remotePath))
        //            //{
        //            //    m_FTPClient.CreateDirectory(remotePath);
        //            //}
        //        }
        //        else
        //        {
        //            if (bForce)
        //            {
        //                AddUpdatedFile(file);
        //                continue;
        //            }

        //            try
        //            {
        //                IVersionControl.FileItem remoteFileInfo;
        //                var fileInfo = new FileInfo(fileFullPath);
        //                // remove milliseconds from local modified time
        //                var localModified = new DateTime(fileInfo.LastWriteTimeUtc.Year, fileInfo.LastWriteTimeUtc.Month, fileInfo.LastWriteTimeUtc.Day, fileInfo.LastWriteTimeUtc.Hour, fileInfo.LastWriteTimeUtc.Minute, fileInfo.LastWriteTimeUtc.Second, DateTimeKind.Utc);

        //                if (!remoteFileMap.TryGetValue(remotePath, out remoteFileInfo)
        //                    || remoteFileInfo.Size != fileInfo.Length
        //                    || remoteFileInfo.Modified.ToUniversalTime() < localModified)
        //                {
        //                    AddUpdatedFile(file);
        //                }
        //            }
        //            catch (Exception)
        //            {
        //                AddUpdatedFile(file);
        //            }

        //            fileList.Add(relativePath);
        //        }
        //    }

        //    // search remote and delete not exists locally
        //    Log("Delete remote files...");
        //    foreach (var remoteFile in remoteFileList)
        //    {
        //        var localPath = m_PathControl.ToLocalPath(remoteFile.RemoteFilePath);
        //        if (m_Ignore.IsIgnore(localPath)) continue;

        //        try
        //        {
        //            if (!fileList.Contains(localPath))
        //                MarkDelete(remoteFile.RemoteFilePath);
        //        }
        //        catch (Exception exp)
        //        {
        //            Log("Failed to delete remote file:{0}, {1}", remoteFile.RemoteFilePath, exp.Message);
        //        }
        //    }

        //    // sync updated file list
        //    SyncFolder();
        //}

        //public void SyncFolder()
        //{
        //    string[] updatedFiles = null;
        //    string[] deletedFiles = null;

        //    lock (m_UpdatedFiles)
        //    {
        //        updatedFiles = new string[m_UpdatedFiles.Count];
        //        m_UpdatedFiles.CopyTo(updatedFiles);

        //        deletedFiles = new string[m_DeletedFiles.Count];
        //        m_DeletedFiles.CopyTo(deletedFiles);

        //        m_UpdatedFiles.Clear();
        //        m_DeletedFiles.Clear();
        //    }

        //    if ((updatedFiles.Length + deletedFiles.Length) > 0)
        //        Log("Purge updated files...");

        //    foreach (var deletedFile in deletedFiles)
        //    {
        //        var fileFullPath = Path.GetFullPath(deletedFile);
        //        var relativePath = m_PathControl.NormalizeLocalPath(fileFullPath);
        //        var remotePath = m_PathControl.ToRemotePath(relativePath);

        //        Log("Sync delete :{0}", remotePath);

        //        if (FileExists(remotePath))
        //        {
        //            MarkDelete(remotePath);
        //        }
        //    }

        //    byte[] readBuffer = new byte[1024 * 1024];
        //    foreach (var updatedFile in updatedFiles)
        //    {
        //        var fileFullPath = Path.GetFullPath(updatedFile);
        //        var remotePath = m_PathControl.ToRemotePath(fileFullPath);

        //        if (m_Ignore.IsIgnore(fileFullPath)) continue;

        //        Log("Sync update :{0}", remotePath);
        //        FileAttributes localFileAttr;
        //        try
        //        {
        //            localFileAttr = File.GetAttributes(fileFullPath);
        //        }
        //        catch (Exception)
        //        {
        //            continue;
        //        }

        //        if (localFileAttr.HasFlag(FileAttributes.Directory)) // if it's a directory
        //        {
        //        }
        //        else
        //        {
        //            MarkChanged(remotePath);
        //        }
        //    }

        //    if ((updatedFiles.Length + deletedFiles.Length) > 0)
        //        Log("Purge updated Done");
        //}
    }

}


