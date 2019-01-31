using System;
using System.IO;
using System.Threading;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net.FtpClient;
using System.Net;

namespace BR.FTPSync
{
    public class FTPPathSynchronizer : IDisposable
    {
        HashSet<string> m_UpdatedFiles = new HashSet<string>();
        HashSet<string> m_DeletedFiles = new HashSet<string>();
        FileSystemWatcher m_FileSystemWatcher;
        FtpClient m_FTPClient;
        string m_LocalPath;
        string m_RemotePath;

        public delegate void delLogFunction(string fromat, params object[] args);
        public delLogFunction Log;

        public void Initialize(string localPath, string remotePath, string ftpAddress, string userName, string pw)
        {
            Log("Start Initialize...");

            m_LocalPath = Path.GetFullPath(localPath);
            if (!m_LocalPath.EndsWith("\\"))
                m_LocalPath = m_LocalPath + "\\";

            m_RemotePath = remotePath;
            if (!m_RemotePath.EndsWith("/"))
                m_RemotePath = m_RemotePath + "/";

            // Initialize file system watcher
            m_FileSystemWatcher = new System.IO.FileSystemWatcher();

            m_FileSystemWatcher.Path = localPath;// "D:\\Work\\BRServer";
            m_FileSystemWatcher.IncludeSubdirectories = true;

            m_FileSystemWatcher.NotifyFilter = NotifyFilters.DirectoryName | NotifyFilters.FileName | NotifyFilters.LastWrite | NotifyFilters.CreationTime;
            m_FileSystemWatcher.InternalBufferSize = 512 * 1024;

            m_FileSystemWatcher.Created += (object sender, FileSystemEventArgs e) =>
            {
                if (IsIgnore(e.FullPath)) return;
                AddUpdatedFile(e.FullPath);
                Log("Created:{0}", e.FullPath);
            };

            m_FileSystemWatcher.Deleted += (object sender, FileSystemEventArgs e) =>
            {
                if (IsIgnore(e.FullPath)) return;
                AddDeletedFile(e.FullPath);
                Log("Deleted:{0}", e.FullPath);
            };

            m_FileSystemWatcher.Changed += (object sender, FileSystemEventArgs e) =>
            {
                if (IsIgnore(e.FullPath)) return;
                AddUpdatedFile(e.FullPath);
                Log("Changed:{0}", e.FullPath);
            };

            m_FileSystemWatcher.Renamed += (object sender, RenamedEventArgs e) =>
            {
                if (IsIgnore(e.FullPath)) return;
                AddDeletedFile(e.OldFullPath);
                AddUpdatedFile(e.FullPath);
                Log("Renamed:{0}", e.FullPath);
            };

            m_FileSystemWatcher.EnableRaisingEvents = true;

            //initialize ftp client
            var ftpURI = new Uri(ftpAddress);
            m_FTPClient = new FtpClient();
            m_FTPClient.Credentials = new NetworkCredential(userName, pw);
            m_FTPClient.Host = ftpURI.Host;
            m_FTPClient.Port = ftpURI.Port;
            m_FTPClient.EnableThreadSafeDataConnections = true;
            // for test
            m_FTPClient.DataConnectionType = FtpDataConnectionType.PASV;
            m_FTPClient.Connect();

            // run full sync
            SyncFull();
        }

        public void Dispose()
        {
            if (m_FileSystemWatcher != null) m_FileSystemWatcher.Dispose();
            m_FileSystemWatcher = null;
        }



        HashSet<string> m_IgnoreExts = new HashSet<string>();
        HashSet<string> m_IgnoreFolder = new HashSet<string>();

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

        bool IsIgnore(string filePath)
        {
            var fileFullPath = Path.GetFullPath(filePath);
            //var relativePath = ToRelativePath(m_LocalPath,fileFullPath);
            //var fileName = Path.GetFileName(relativePath);
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
            catch(Exception)
            {
                // maybe no local file, ignore it
                return true;
            }

            return false;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="fileName"></param>
        public void AddUpdatedFile(string fileName)
        {
            if (IsIgnore(fileName)) return;

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
            if (IsIgnore(fileName)) return;

            lock (m_UpdatedFiles)
            {
                if (m_UpdatedFiles.Contains(fileName))
                    m_UpdatedFiles.Remove(fileName);

                if (!m_DeletedFiles.Contains(fileName))
                    m_DeletedFiles.Add(fileName);
            }
        }

        string ToRelativePath(string basePath, string path)
        {
            return path.Substring(basePath.Length);
        }

        string ToLocalPath(string relativePath)
        {
            return m_LocalPath + relativePath.Replace('/', '\\');
        }

        string ToRemotePath(string relativePath)
        {
            return m_RemotePath + relativePath.Replace('\\', '/');
        }

        public void SyncFull(bool bForce = false)
        {
            Log("Full sync...");
            HashSet<string> fileList = new HashSet<string>();
            HashSet<string> directoryList = new HashSet<string>();

            string[] files = Directory.GetFileSystemEntries(m_LocalPath, "*", SearchOption.AllDirectories);

            // grep all remote information
            Log("Updating remote file list...");
            var remoteFileList = m_FTPClient.GetListing(m_RemotePath, FtpListOption.Recursive | FtpListOption.SizeModify);
            var remoteFileMap = new Dictionary<string, FtpListItem>();
            foreach (var remoteFile in remoteFileList)
            {
                remoteFileMap.Add(remoteFile.FullName, remoteFile);
            }

            foreach (var file in files)
            {
                if (IsIgnore(file)) continue;

                var fileFullPath = Path.GetFullPath(file);
                var relativePath = ToRelativePath(m_LocalPath, fileFullPath);

                var remotePath = ToRemotePath(relativePath);

                // Check whether it's a directory
                FileAttributes localFileAttr = File.GetAttributes(fileFullPath);
                if (localFileAttr.HasFlag(FileAttributes.Directory))
                {
                    directoryList.Add(relativePath);
                    if (!m_FTPClient.DirectoryExists(remotePath))
                    {
                        m_FTPClient.CreateDirectory(remotePath);
                    }
                }
                else
                {
                    if (bForce)
                    {
                        AddUpdatedFile(file);
                        continue;
                    }

                    try
                    {
                        FtpListItem remoteFileInfo;
                        var fileInfo = new FileInfo(fileFullPath);
                        // remove miliscond from local modified time
                        var localModified = new DateTime(fileInfo.LastWriteTimeUtc.Year, fileInfo.LastWriteTimeUtc.Month, fileInfo.LastWriteTimeUtc.Day, fileInfo.LastWriteTimeUtc.Hour, fileInfo.LastWriteTimeUtc.Minute, fileInfo.LastWriteTimeUtc.Second, DateTimeKind.Utc);

                        if (!remoteFileMap.TryGetValue(remotePath, out remoteFileInfo)
                            || remoteFileInfo.Size != fileInfo.Length
                            || remoteFileInfo.Modified.ToUniversalTime() < localModified)
                        {
                            AddUpdatedFile(file);
                        }
                    }
                    catch (Exception)
                    {
                        AddUpdatedFile(file);
                    }

                    fileList.Add(relativePath);
                }
            }

            // search remote and delete not exists locally
            Log("Delete remote files...");
            foreach (var remoteFile in remoteFileList)
            {
                var relativePath = ToRelativePath(m_RemotePath, remoteFile.FullName).Replace('/', '\\');
                var localFullPath = ToLocalPath(relativePath);
                if (IsIgnore(localFullPath)) continue;
                if (remoteFile.Type == FtpFileSystemObjectType.Directory)
                {
                    try
                    {
                        if (!directoryList.Contains(relativePath))
                            m_FTPClient.DeleteDirectory(remoteFile.FullName);
                    }
                    catch (Exception exp)
                    {
                        Log("Failed to delete remote folder:{0}, {1}", remoteFile.FullName, exp.Message);
                    }
                }
                else if (remoteFile.Type == FtpFileSystemObjectType.File)
                {
                    try
                    {
                        if (!fileList.Contains(relativePath))
                            m_FTPClient.DeleteFile(remoteFile.FullName);
                    }
                    catch (Exception exp)
                    {
                        Log("Failed to delete remote file:{0}, {1}", remoteFile.FullName, exp.Message);
                    }
                }
                else
                {
                    // Ignore links
                }
            }

            // sync updated file list
            SyncFolder();
        }

        public void SyncFolder()
        {
            string[] updatedFiles = null;
            string[] deletedFiles = null;

            lock (m_UpdatedFiles)
            {
                updatedFiles = m_UpdatedFiles.ToArray();
                deletedFiles = m_DeletedFiles.ToArray();
                m_UpdatedFiles.Clear();
                m_DeletedFiles.Clear();
            }

            if((updatedFiles.Length+ deletedFiles.Length) > 0)
                Log("Purge updated files...");

            foreach (var deletedFile in deletedFiles)
            {
                var fileFullPath = Path.GetFullPath(deletedFile);
                var relativePath = ToRelativePath(m_LocalPath, fileFullPath);

                var remotePath = ToRemotePath(relativePath);

                Log("Sync delete :{0}", remotePath);

                if (m_FTPClient.DirectoryExists(remotePath)) // if it's a directory
                {
                    try
                    {
                        m_FTPClient.DeleteDirectory(remotePath);
                    }
                    catch (Exception exp)
                    {
                        Log("Failed to delete remote folder:{0}, {1}", remotePath, exp.Message);
                    }
                }
                else if (m_FTPClient.FileExists(remotePath))
                {
                    try
                    {
                        m_FTPClient.DeleteFile(remotePath);
                    }
                    catch (Exception exp)
                    {
                        Log("Failed to delete remote file:{0}, {1}", remotePath, exp.Message);
                    }
                }
            }

            byte[] readBuffer = new byte[1024 * 1024];
            foreach (var updatedFile in updatedFiles)
            {
                var fileFullPath = Path.GetFullPath(updatedFile);
                var relativePath = ToRelativePath(m_LocalPath, fileFullPath);
                var remotePath = ToRemotePath(relativePath);

                if (IsIgnore(fileFullPath)) continue;

                Log("Sync update :{0}", remotePath);
                FileAttributes localFileAttr;
                try
                {
                    localFileAttr = File.GetAttributes(fileFullPath);
                }
                catch (Exception)
                {
                    continue;
                }

                if (localFileAttr.HasFlag(FileAttributes.Directory)) // if it's a directory
                {
                    if (!m_FTPClient.DirectoryExists(remotePath)) 
                    {
                        try
                        {
                            m_FTPClient.CreateDirectory(remotePath);
                        }
                        catch (Exception exp)
                        {
                            Log("Failed to delete remote folder:{0}, {1}", remotePath, exp.Message);
                        }
                    }
                }
                else
                {
                    Stream outputStream = null, inputStream = null;
                    try
                    {
                        inputStream = File.OpenRead(fileFullPath);
                        outputStream = m_FTPClient.OpenWrite(remotePath);

                        int readSize = 0;
                        for (int writen = 0; writen < inputStream.Length;)
                        {
                            readSize = inputStream.Read(readBuffer, 0, readBuffer.Length);
                            if (readSize == 0)
                            {
                                if (writen < inputStream.Length)
                                {
                                    Log("Failed to read file:{0}, {1}", relativePath);
                                }
                                break;
                            }
                            outputStream.Write(readBuffer, 0, readSize);
                            writen += readSize;
                        }

                    }
                    catch (Exception exp)
                    {
                        Log("Failed to upload file:{0} => {1}, {2}", relativePath, remotePath, exp.Message);
                    }
                    finally
                    {
                        if (outputStream != null) outputStream.Flush();
                        if (inputStream != null) inputStream.Close();
                    }
                }
            }

            if ((updatedFiles.Length + deletedFiles.Length) > 0)
                Log("Purge updated Done");
        }
    }
}


