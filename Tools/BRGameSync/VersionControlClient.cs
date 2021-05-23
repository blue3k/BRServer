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
using System.Collections.Specialized;
using BRGameSync;
using System.Collections.ObjectModel;
using System.Windows;

namespace BR
{

    public class VersionFileInfoCollection : Dictionary<string, VersionFileInfo>
    {
        public ObservableCollection<VersionFileInfo> FileInfoList { get; private set; }

        public VersionFileInfoCollection()
        {
            FileInfoList = new ObservableCollection<VersionFileInfo>();
        }

        public new void Clear()
        {
            Application.Current.Dispatcher.BeginInvoke((Action)(() =>
            {
                FileInfoList.Clear();
            }));
            base.Clear();
        }

        public void Add(VersionFileInfo fileInfo)
        {
            Add(fileInfo.LocalFilePath, fileInfo);
            Application.Current.Dispatcher.BeginInvoke((Action)(() =>
            {
                FileInfoList.Add(fileInfo);
            }));
        }

        public void Remove(VersionFileInfo fileInfo)
        {
            Remove(fileInfo.LocalFilePath);
            Application.Current.Dispatcher.BeginInvoke((Action)(() =>
            {
                FileInfoList.Remove(fileInfo);
            }));
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

        VersionFileInfoCollection m_ModifiedFiles = new VersionFileInfoCollection();
        public VersionFileInfoCollection ModifiedFileList
        {
            get
            {
                return m_ModifiedFiles;
            }
        }

        public VersionControlClient()
        {
            m_PathControl = new VersionControlPath();
            m_FileStorage = new VersionFileStorage(m_PathControl);
            m_VersionDB = new VersionDB(m_PathControl);
        }

        public void Initialize(string localPath, string remotePath)
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
                    {
                        m_LocalFileState.Add(fileInfo.LocalFilePath, fileInfo);
                        if (!m_ModifiedFiles.ContainsKey(fileInfo.LocalFilePath))
                            m_ModifiedFiles.Add(fileInfo);
                    }
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
                VersionFileInfo fileInfo;
                if (m_LocalFileState.TryGetValue(fileName, out fileInfo))
                {
                    m_LocalFileState.Remove(fileName);
                    fileInfo.Deleted = true;
                    if (!m_ModifiedFiles.ContainsKey(fileInfo.LocalFilePath))
                        m_ModifiedFiles.Add(fileInfo);
                    else
                    {
                        if (fileInfo.FileVersion <= 1) // Added file has deleted
                            m_ModifiedFiles.Remove(fileInfo);
                    }
                }
            }
        }

        public IList<VersionFileInfo> GetFileList()
        {
            // Create a client that can authenticate using our token credential
            return m_VersionDB.GetFileList();
        }

        public IList<IVersionControl.VersionInfo> GetVersionList(Int64 minChangeNumber = -1)
        {
            return m_VersionDB.GetVersionList(minChangeNumber);
        }

        VersionFileInfo LocalFullPathToVersionFileInfo(string localFullPath)
        {
            var localFileInfo = new FileInfo(localFullPath);
            if (localFileInfo.Attributes == FileAttributes.Directory)
                return null;

            // remove milliseconds from local modified time
            var localNormalizedPath = m_PathControl.NormalizeLocalPath(localFullPath);
            if (m_Ignore.IsIgnore(m_PathControl.ToFullLocalPath(localNormalizedPath)))
                return null;

            var localModified = new DateTimeOffset(localFileInfo.LastWriteTimeUtc, new TimeSpan(0));

            var fileItem = new VersionFileInfo();
            fileItem.LocalFilePath = localNormalizedPath;
            fileItem.RemoteFilePath = m_PathControl.ToRemotePath(localNormalizedPath);
            fileItem.Modified = localModified;
            fileItem.Size = localFileInfo.Length;

            VersionFileInfo remoteFileInfo;
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

        Dictionary<string, VersionFileInfo> m_LocalFileState = new Dictionary<string, VersionFileInfo>();
        async Task UpdateLocalFileInfo()
        {
            SF.Log.Info("Updating local file state");
            m_LocalFileState.Clear();

            var subTask = new Task(() =>
            {
                string[] localFiles = Directory.GetFileSystemEntries(m_PathControl.LocalBasePath, "*", SearchOption.AllDirectories);

                foreach (var fileFullPath in localFiles)
                {
                    var fileItem = LocalFullPathToVersionFileInfo(fileFullPath);
                    if (fileItem == null)
                        continue;

                    lock (m_LocalFileState)
                        m_LocalFileState.Add(fileItem.LocalFilePath, fileItem);
                }
            });
            subTask.Start();

            SF.Log.Info("{0} local file info has updated", m_LocalFileState.Count);

            await subTask;
            return;
        }

        Dictionary<string, VersionFileInfo> m_RemoteFileState = new Dictionary<string, VersionFileInfo>();
        async Task UpdateRemoteFileState()
        {
            SF.Log.Info("Reading remote file status");
            m_RemoteFileState.Clear();

            var subTask = new Task(() =>
            {
                var remoteFileList = m_VersionDB.GetFileList();
                foreach (var remoteFile in remoteFileList)
                {
                    lock(m_RemoteFileState)
                        m_RemoteFileState.Add(remoteFile.LocalFilePath, remoteFile);
                }
            });
            subTask.Start();

            await subTask;

            SF.Log.Info("{0} remote file info has received", m_RemoteFileState.Count);
        }

        public async Task<bool> GetLatestAsync()
        {
            await UpdateRemoteFileState();
            await UpdateLocalFileInfo();

            List<VersionFileInfo> downloadList = new List<VersionFileInfo>();

            foreach (var remoteFileInfo in m_RemoteFileState)
            {
                VersionFileInfo localFileInfo;
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
                SF.Log.Info("Downloading {0} files", downloadList.Count);

                await m_FileStorage.DownloadFiles(downloadList);

                await UpdateLocalFileInfo();
                SF.Log.Info("GetLatest done. {0} files are downloaded", downloadList.Count);
            }
            else
            {
                SF.Log.Info("GetLatest done. Nothing to download", downloadList.Count);
            }

            return true;
        }

        public async Task ReconcileLocalChanges()
        {
            await UpdateRemoteFileState();
            await UpdateLocalFileInfo();

            m_ModifiedFiles.Clear();

            foreach (var remoteFileInfo in m_RemoteFileState)
            {
                VersionFileInfo localFileInfo;
                if (m_LocalFileState.TryGetValue(remoteFileInfo.Value.LocalFilePath, out localFileInfo))
                {
                    if (localFileInfo.FileVersion > remoteFileInfo.Value.FileVersion)
                    {
                        m_ModifiedFiles.Add(localFileInfo);
                    }
                }
                else
                {
                    var fileInfo = remoteFileInfo.Value.Clone();
                    fileInfo.Deleted = true;
                    m_ModifiedFiles.Add(fileInfo);
                }
            }

            // handle added files
            foreach (var localFileInfo in m_LocalFileState)
            {
                if (m_RemoteFileState.ContainsKey(localFileInfo.Value.LocalFilePath))
                    continue;

                m_ModifiedFiles.Add(localFileInfo.Value);
            }

            SF.Log.Info("Reconcile done. You have {0} local changes", m_ModifiedFiles.Count);
        }

        public async Task<bool> CommitChanges(string description)
        {
            await UpdateRemoteFileState();

            SF.Log.Info("Validating changes before commit");

            // Validate changes
            List<VersionFileInfo> fileList = new List<VersionFileInfo>();
            foreach (var changedFileInfo in m_ModifiedFiles)
            {
                var curFileInfo = changedFileInfo.Value;
                VersionFileInfo remoteFileInfo;

                if (m_RemoteFileState.TryGetValue(curFileInfo.LocalFilePath, out remoteFileInfo))
                {
                    if (!curFileInfo.Deleted && curFileInfo.FileVersion <= remoteFileInfo.FileVersion)
                    {
                        SF.Log.Error("Commit failed: {0} has invalid file version!", curFileInfo.LocalFilePath, curFileInfo.FileVersion);
                        return false;
                    }
                }
                else
                {
                    if (curFileInfo.FileVersion != 1)
                    {
                        SF.Log.Error("Commit failed: {0} has invalid file version!", curFileInfo.LocalFilePath, curFileInfo.FileVersion);
                        return false;
                    }
                }

                fileList.Add(curFileInfo);
            }

            if (fileList.Count == 0)
            {
                SF.Log.Info("Commit has finished. Nothing to commit");
                return true;
            }

            Int64 changeNumber;
            var subTask = new Task(() =>
            {
                using (var session = m_VersionDB.OpenSession())
                {
                    session.StartTransaction();
                    try
                    {
                        SF.Log.Info("Lock changed files");
                        m_VersionDB.UpdateFileLocks(session, fileList, true);


                        Task.WaitAll(m_FileStorage.UploadFiles(fileList));

                        SF.Log.Info("Committing change");
                        m_VersionDB.CommitChange(session, m_FileStorage.UserName, description, fileList, out changeNumber);

                        session.Commit();
                        session.Close();

                        m_ModifiedFiles.Clear();

                        SF.Log.Info("Commit has finished. Change number {0}, {1} files are uploaded", changeNumber, fileList.Count);
                    }
                    catch (Exception exp)
                    {
                        session.Rollback();
                        SF.Log.Error("Commit has failed. {0} => {1}", exp.Message, exp.StackTrace.ToString());
                        return;
                    }
                }
            });

            subTask.Start();

            await subTask;

            return true;
        }

    }

}


