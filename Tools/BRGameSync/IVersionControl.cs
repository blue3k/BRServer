using System;
using System.IO;
using System.Collections.Generic;
using SF;

namespace BR
{

    public class VersionControlPath
    {
        string m_LocalBasePath, m_LocalBasePathLower;
        public string LocalBasePath
        {
            get { return m_LocalBasePath; }
            set
            {
                m_LocalBasePath = value;
                m_LocalBasePath.Trim();
                if (!m_LocalBasePath.EndsWith(Path.DirectorySeparatorChar))
                    m_LocalBasePath = m_LocalBasePath + Path.DirectorySeparatorChar;
                m_LocalBasePathLower = m_LocalBasePath.ToLower();
            }
        }

        string m_RemoteBasePath, m_RemoteBasePathLower;
        public string RemoteBasePath
        {
            get { return m_RemoteBasePath; }
            set
            {
                m_RemoteBasePath = value;
                m_RemoteBasePath.Trim();
                if (!m_RemoteBasePath.EndsWith('/'))
                    m_RemoteBasePath = m_RemoteBasePath + '/';
                m_RemoteBasePathLower = m_RemoteBasePath.ToLower();
            }
        }


        public string NormalizeLocalPath(string localPath)
        {
            localPath.Trim();
            if (Path.DirectorySeparatorChar != '/')
                localPath.Replace('/', Path.DirectorySeparatorChar);
            else
                localPath.Replace('\\', Path.DirectorySeparatorChar);

            int basePathStart = localPath.ToLower().IndexOf(m_LocalBasePathLower);
            if (basePathStart >= 0)
            {
                localPath = localPath.Substring(basePathStart + m_LocalBasePathLower.Length);
            }

            if (localPath.StartsWith(Path.DirectorySeparatorChar))
                localPath.Remove(0, 1);

            return localPath;
        }
        public string NormalizeRemotePath(string remotePath)
        {
            remotePath.Trim();
            remotePath.Replace('\\', '/');

            int basePathStart = remotePath.ToLower().IndexOf(m_RemoteBasePathLower);
            if (basePathStart >= 0)
            {
                remotePath = remotePath.Substring(basePathStart + m_RemoteBasePathLower.Length);
            }

            if (remotePath.StartsWith('/'))
                remotePath.Remove(0, 1);

            return remotePath;
        }

        public string ToRemotePath(string localPath)
        {
            return NormalizeRemotePath(RemoteBasePath + NormalizeLocalPath(localPath));
        }

        public string ToLocalPath(string remotePath)
        {
            return NormalizeLocalPath(LocalBasePath + NormalizeRemotePath(remotePath));
        }

    }


    public abstract class IVersionControl
    {
        [Flags]
        public enum ListOption
        {
            None = 0,
            Recursive = 1,
        }

        public class FileItem
        {
            public Int32 FileVersion;
            public string LocalFilePath;
            public string RemoteFilePath;
            public long Size;
            public bool Deleted;
            public DateTimeOffset Modified;
        }



        public abstract IList<FileItem> GetFileList(string remotePath, ListOption listOption = ListOption.None);

        public abstract bool FileExists(string remotePath);

        // Mark for delete
        public abstract void MarkDelete(string remotePath);
        public abstract void MarkChanged(string remotePath);
        public abstract void MarkAdd(string remotePath);


        public abstract Result SetChangeDescription(string description);
        public abstract Result SubmitChanges();

    }
}


