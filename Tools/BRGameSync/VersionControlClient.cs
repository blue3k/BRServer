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


    /// <summary>
    /// Braves version control system
    /// </summary>
    public class VersionControlClient
    {
        VersionControlPath m_PathControl;
        VersionFileStorage m_FileStorage;
        VersionDB m_VersionDB;
        FileIgnoreList m_Ignore = new FileIgnoreList();


        Dictionary<string, IVersionControl.FileInfo> m_ModifiedFiles = new Dictionary<string, IVersionControl.FileInfo>();
        public Dictionary<string, IVersionControl.FileInfo> ModifiedFileList
        {
            get
            {
                return m_ModifiedFiles;
            }
        }

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

            // pushing it to connect
            m_FileStorage.GetFileList(".git");
        }

        public VersionControlPath PathControl { get { return m_PathControl; } }



        /// <summary>
        /// 
        /// </summary>
        /// <param name="fileName"></param>
        public void AddUpdatedFile(string fileName)
        {
            fileName = m_PathControl.NormalizeLocalPath(fileName);
            var fullPath = m_PathControl.ToFullLocalPath(fileName);

            if (m_Ignore.IsIgnore(fullPath))
                return;

            lock (m_LocalFileState)
            {
                if (!m_LocalFileState.ContainsKey(fileName))
                {
                    var fileInfo = LocalFullPathToVersionFileInfo(fullPath);
                    if (fileInfo != null)
                        m_LocalFileState.Add(fileInfo.LocalFilePath, fileInfo);
                }
            }

        }

        public void AddDeletedFile(string fileName)
        {
            fileName = m_PathControl.NormalizeLocalPath(fileName);
            var fullPath = m_PathControl.ToFullLocalPath(fileName);
            if (m_Ignore.IsIgnore(fullPath))
                return;

            lock (m_LocalFileState)
            {
                if (m_LocalFileState.ContainsKey(fileName))
                {
                    m_LocalFileState.Remove(fileName);
                }
            }
        }

        public IList<IVersionControl.FileInfo> GetFileList()
        {
            // Create a client that can authenticate using our token credential
            return m_VersionDB.GetFileList();
        }

        public IList<IVersionControl.VersionInfo> GetVersionList(Int64 minChangeNumber = -1)
        {
            return m_VersionDB.GetVersionList(minChangeNumber);
        }

        IVersionControl.FileInfo LocalFullPathToVersionFileInfo(string localFullPath)
        {
            var localFileInfo = new FileInfo(localFullPath);
            if (localFileInfo.Attributes == FileAttributes.Directory)
                return null;

            // remove milliseconds from local modified time
            var localNormalizedPath = m_PathControl.NormalizeLocalPath(localFullPath);
            if (m_Ignore.IsIgnore(m_PathControl.ToFullLocalPath(localNormalizedPath)))
                return null;

            var localModified = new DateTimeOffset(localFileInfo.LastWriteTimeUtc, new TimeSpan(0));

            var fileItem = new IVersionControl.FileInfo();
            fileItem.LocalFilePath = localNormalizedPath;
            fileItem.RemoteFilePath = m_PathControl.ToRemotePath(localNormalizedPath);
            fileItem.Modified = localModified;
            fileItem.Size = localFileInfo.Length;

            IVersionControl.FileInfo remoteFileInfo;
            if (!m_RemoteFileState.TryGetValue(localNormalizedPath, out remoteFileInfo))
            {
                fileItem.FileVersion = 1;
            }
            else if (remoteFileInfo.Size != localFileInfo.Length
                    || remoteFileInfo.Modified.ToUniversalTime() < localModified)
            {
                fileItem.FileVersion = remoteFileInfo.FileVersion + 1;
            }
            else
            {
                fileItem.FileVersion = remoteFileInfo.FileVersion;
            }

            return fileItem;
        }

        Dictionary<string, IVersionControl.FileInfo> m_LocalFileState = new Dictionary<string, IVersionControl.FileInfo>();
        void UpdateLocalFileInfo()
        {
            m_LocalFileState.Clear();

            string[] localFiles = Directory.GetFileSystemEntries(m_PathControl.LocalBasePath, "*", SearchOption.AllDirectories);
            foreach (var fileFullPath in localFiles)
            {
                var fileItem = LocalFullPathToVersionFileInfo(fileFullPath);
                if (fileItem == null)
                    continue;

                m_LocalFileState.Add(fileItem.LocalFilePath, fileItem);
            }
        }

        Dictionary<string, IVersionControl.FileInfo> m_RemoteFileState = new Dictionary<string, IVersionControl.FileInfo>();
        void UpdateRemoteFileState()
        {
            m_RemoteFileState.Clear();

            var remoteFileList = m_VersionDB.GetFileList();
            foreach (var remoteFile in remoteFileList)
            {
                m_RemoteFileState.Add(remoteFile.LocalFilePath, remoteFile);
            }
        }

        public void GetLatest()
        {
            UpdateRemoteFileState();
            UpdateLocalFileInfo();

            List<IVersionControl.FileInfo> downloadList = new List<IVersionControl.FileInfo>();

            foreach (var remoteFileInfo in m_RemoteFileState)
            {
                IVersionControl.FileInfo localFileInfo;
                if (m_LocalFileState.TryGetValue(remoteFileInfo.Value.LocalFilePath, out localFileInfo))
                {
                    if (localFileInfo.FileVersion < remoteFileInfo.Value.FileVersion)
                    {
                        downloadList.Add(remoteFileInfo.Value);
                    }
                }
                else
                {
                    downloadList.Add(remoteFileInfo.Value);
                }
            }

            if (downloadList.Count > 0)
            {
                m_FileStorage.DownloadFiles(downloadList);
                SF.Log.Info("GetLatest done. {0} files are downloaded", downloadList.Count);
            }
            else
            {
                SF.Log.Info("GetLatest done. Nothing to download", downloadList.Count);
            }
        }

        public void ReconcileLocalChanges()
        {
            UpdateRemoteFileState();
            UpdateLocalFileInfo();

            m_ModifiedFiles.Clear();

            foreach (var remoteFileInfo in m_RemoteFileState)
            {
                IVersionControl.FileInfo localFileInfo;
                if (m_LocalFileState.TryGetValue(remoteFileInfo.Value.LocalFilePath, out localFileInfo))
                {
                    if (localFileInfo.FileVersion > remoteFileInfo.Value.FileVersion)
                    {
                        m_ModifiedFiles.Add(localFileInfo.LocalFilePath, localFileInfo);
                    }
                }
                else
                {
                    var fileInfo = remoteFileInfo.Value.Clone();
                    fileInfo.Deleted = true;
                    m_ModifiedFiles.Add(fileInfo.LocalFilePath, fileInfo);
                }
            }

            // handle added files
            foreach (var localFileInfo in m_LocalFileState)
            {
                if (m_RemoteFileState.ContainsKey(localFileInfo.Value.LocalFilePath))
                    continue;

                m_ModifiedFiles.Add(localFileInfo.Key, localFileInfo.Value);
            }

            SF.Log.Info("Reconcile done. You have {0} local changes", m_ModifiedFiles.Count);
        }

        public Result SubmitChanges(string description)
        {
            UpdateRemoteFileState();

            // Validate changes
            List<IVersionControl.FileInfo> fileList = new List<IVersionControl.FileInfo>();
            foreach (var changedFileInfo in m_ModifiedFiles)
            {
                var curFileInfo = changedFileInfo.Value;
                IVersionControl.FileInfo remoteFileInfo;

                if (m_RemoteFileState.TryGetValue(curFileInfo.LocalFilePath, out remoteFileInfo))
                {
                    if (!curFileInfo.Deleted && curFileInfo.FileVersion <= remoteFileInfo.FileVersion)
                    {
                        SF.Log.Error("Submit failed: {0} has invalid file version!", curFileInfo.LocalFilePath, curFileInfo.FileVersion);
                        return new Result(ResultCode.FAIL);
                    }
                }
                else
                {
                    if (curFileInfo.FileVersion != 1)
                    {
                        SF.Log.Error("Submit failed: {0} has invalid file version!", curFileInfo.LocalFilePath, curFileInfo.FileVersion);
                        return new Result(ResultCode.FAIL);
                    }
                }

                fileList.Add(curFileInfo);
            }

            if (fileList.Count == 0)
            {
                SF.Log.Info("Submition has finished. Nothing to submit");
                return new Result(ResultCode.SUCCESS);
            }

            Int64 changeNumber;
            using (var session = m_VersionDB.OpenSession())
            {
                session.StartTransaction();
                try
                {
                    m_VersionDB.UpdateFileLocks(session, fileList, true);
                    m_FileStorage.UploadFiles(fileList);
                    m_VersionDB.CommitChange(session, description, fileList, out changeNumber);

                    session.Commit();
                    session.Close();

                    m_ModifiedFiles.Clear();

                    SF.Log.Info("Submition has finished. Change number {0}, {1} files are uploaded", changeNumber, fileList.Count);
                }
                catch (Exception exp)
                {
                    session.Rollback();
                    SF.Log.Error("Submition has failed. {0} => {1}", exp.Message, exp.StackTrace.ToString());
                    return new Result(ResultCode.UNEXPECTED);
                }
            }

            return new Result(ResultCode.SUCCESS);
        }

    }

}


