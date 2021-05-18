using System;
using System.IO;
using System.Collections.Generic;
using SF;


namespace BR
{
    public class VersionDB
    {

    }

    public class VersionControlFileStorage
    {

    }

    public class FileIgnoreManager
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

        public bool IsIgnore(string filePath)
        {
            var fileFullPath = Path.GetFullPath(filePath);
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
    public class VersionControlClient : IVersionControl
    {
        FileIgnoreManager m_Ignore = new FileIgnoreManager();
        HashSet<string> m_UpdatedFiles = new HashSet<string>();
        HashSet<string> m_DeletedFiles = new HashSet<string>();

        string m_LocalPath;
        string m_RemotePath;

        public delegate void delLogFunction(string fromat, params object[] args);
        public delLogFunction Log;

        public void Initialize(string localPath, string remotePath, string serverAddress)
        {

            m_LocalPath = Path.GetFullPath(localPath);
            if (!m_LocalPath.EndsWith("\\"))
                m_LocalPath = m_LocalPath + "\\";

            m_RemotePath = remotePath;
            if (!m_RemotePath.EndsWith("/"))
                m_RemotePath = m_RemotePath + "/";

            m_Ignore.LoadIgnore(Path.Combine(m_LocalPath, ".gitignore"));

        }
        public override string LocalPath { get { return m_LocalPath; } }
        public override string RemotePath { get { return m_RemotePath; } }


        public string ToRelativePath(string basePath, string path)
        {
            if (path.StartsWith(basePath))
                return path.Substring(basePath.Length);

            return path;
        }

        public override string ToLocalPath(string relativePath)
        {
            return m_LocalPath + relativePath.Replace('/', '\\');
        }

        public override string ToRemotePath(string relativePath)
        {
            return m_RemotePath + relativePath.Replace('\\', '/');
        }

        public override IList<FileItem> GetFileList(string remotePath, ListOption listOption = ListOption.None)
        {
            return null;
        }

        public override bool FileExists(string remotePath)
        {
            return false;
        }

        // Mark for delete
        public override void MarkDelete(string remotePath)
        {
        }

        public override void MarkChanged(string remotePath)
        {
        }

        public override void MarkAdd(string remotePath)
        {
        }


        public override Result SetChangeDescription(string description)
        {
            return new Result(ResultCode.NOT_IMPLEMENTED);
        }

        public override Result SubmitChanges()
        {
            return new Result(ResultCode.NOT_IMPLEMENTED);
        }


        /// <summary>
        /// 
        /// </summary>
        /// <param name="fileName"></param>
        public void AddUpdatedFile(string fileName)
        {
            if (m_Ignore.IsIgnore(fileName)) return;

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
            if (m_Ignore.IsIgnore(fileName)) return;

            lock (m_UpdatedFiles)
            {
                if (m_UpdatedFiles.Contains(fileName))
                    m_UpdatedFiles.Remove(fileName);

                if (!m_DeletedFiles.Contains(fileName))
                    m_DeletedFiles.Add(fileName);
            }
        }


        public override void SyncFull(bool bForce = false)
        {
            Log("Full sync...");
            HashSet<string> fileList = new HashSet<string>();
            HashSet<string> directoryList = new HashSet<string>();

            string[] files = Directory.GetFileSystemEntries(m_LocalPath, "*", SearchOption.AllDirectories);

            // grab all remote information
            Log("Updating remote file list...");
            var remoteFileList = GetFileList(m_RemotePath, IVersionControl.ListOption.Recursive);
            var remoteFileMap = new Dictionary<string, IVersionControl.FileItem>();
            foreach (var remoteFile in remoteFileList)
            {
                remoteFileMap.Add(remoteFile.FilePath, remoteFile);
            }

            foreach (var file in files)
            {
                if (m_Ignore.IsIgnore(file)) continue;

                var fileFullPath = Path.GetFullPath(file);
                var relativePath = ToRelativePath(m_LocalPath, fileFullPath);

                var remotePath = ToRemotePath(relativePath);

                // Check whether it's a directory
                FileAttributes localFileAttr = File.GetAttributes(fileFullPath);
                if (localFileAttr.HasFlag(FileAttributes.Directory))
                {
                    directoryList.Add(relativePath);
                    //if (!m_FTPClient.DirectoryExists(remotePath))
                    //{
                    //    m_FTPClient.CreateDirectory(remotePath);
                    //}
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
                        IVersionControl.FileItem remoteFileInfo;
                        var fileInfo = new FileInfo(fileFullPath);
                        // remove milliseconds from local modified time
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
                var relativePath = ToRelativePath(m_RemotePath, remoteFile.FilePath).Replace('/', '\\');
                var localFullPath = ToLocalPath(relativePath);
                if (m_Ignore.IsIgnore(localFullPath)) continue;

                try
                {
                    if (!fileList.Contains(relativePath))
                        MarkDelete(remoteFile.FilePath);
                }
                catch (Exception exp)
                {
                    Log("Failed to delete remote file:{0}, {1}", remoteFile.FilePath, exp.Message);
                }
            }

            // sync updated file list
            SyncFolder();
        }

        public override void SyncFolder()
        {
            string[] updatedFiles = null;
            string[] deletedFiles = null;

            lock (m_UpdatedFiles)
            {
                updatedFiles = new string[m_UpdatedFiles.Count];
                m_UpdatedFiles.CopyTo(updatedFiles);

                deletedFiles = new string[m_DeletedFiles.Count];
                m_DeletedFiles.CopyTo(deletedFiles);

                m_UpdatedFiles.Clear();
                m_DeletedFiles.Clear();
            }

            if ((updatedFiles.Length + deletedFiles.Length) > 0)
                Log("Purge updated files...");

            foreach (var deletedFile in deletedFiles)
            {
                var fileFullPath = Path.GetFullPath(deletedFile);
                var relativePath = ToRelativePath(m_LocalPath, fileFullPath);

                var remotePath = ToRemotePath(relativePath);

                Log("Sync delete :{0}", remotePath);

                if (FileExists(remotePath))
                {
                    MarkDelete(remotePath);
                }
            }

            byte[] readBuffer = new byte[1024 * 1024];
            foreach (var updatedFile in updatedFiles)
            {
                var fileFullPath = Path.GetFullPath(updatedFile);
                var relativePath = ToRelativePath(m_LocalPath, fileFullPath);
                var remotePath = ToRemotePath(relativePath);

                if (m_Ignore.IsIgnore(fileFullPath)) continue;

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
                }
                else
                {
                    MarkChanged(remotePath);
                }
            }

            if ((updatedFiles.Length + deletedFiles.Length) > 0)
                Log("Purge updated Done");
        }
    }

}


