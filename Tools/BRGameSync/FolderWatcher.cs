using System;
using System.IO;
using System.Collections.Generic;
using SF;

namespace BR
{

    public class FolderWatcher : IDisposable
    {
        FileSystemWatcher m_FileSystemWatcher;
        VersionControlClient m_VersionControl;

        bool m_Pause = false;
        public bool Pause { get { return m_Pause; } set { m_Pause = value; } }

        public delegate void delLogFunction(string format, params object[] args);
        public delLogFunction Log = (format, args)=> { };

        public void Initialize(VersionControlClient versionControl)
        {
            Log("Start Initialize...");

            m_VersionControl = versionControl;

            // Initialize file system watcher
            m_FileSystemWatcher = new System.IO.FileSystemWatcher();

            m_FileSystemWatcher.Path = m_VersionControl.PathControl.LocalBasePath;
            m_FileSystemWatcher.IncludeSubdirectories = true;

            m_FileSystemWatcher.NotifyFilter = NotifyFilters.DirectoryName | NotifyFilters.FileName | NotifyFilters.LastWrite | NotifyFilters.CreationTime;
            m_FileSystemWatcher.InternalBufferSize = 1024 * 1024;

            m_FileSystemWatcher.Created += (object sender, FileSystemEventArgs e) =>
            {
                if (!m_Pause)
                {
                    m_VersionControl.AddUpdatedFile(e.FullPath);
                    Log("Created:{0}", e.FullPath);
                }
            };

            m_FileSystemWatcher.Deleted += (object sender, FileSystemEventArgs e) =>
            {
                if (!m_Pause)
                {
                    m_VersionControl.AddDeletedFile(e.FullPath);
                    Log("Deleted:{0}", e.FullPath);
                }
            };

            m_FileSystemWatcher.Changed += (object sender, FileSystemEventArgs e) =>
            {
                if (!m_Pause)
                {
                    m_VersionControl.AddUpdatedFile(e.FullPath);
                    Log("Changed:{0}", e.FullPath);
                }
            };

            m_FileSystemWatcher.Renamed += (object sender, RenamedEventArgs e) =>
            {
                if (!m_Pause)
                {
                    m_VersionControl.AddDeletedFile(e.OldFullPath);
                    m_VersionControl.AddUpdatedFile(e.FullPath);
                    Log("Renamed:{0}", e.FullPath);
                }
            };

            m_FileSystemWatcher.EnableRaisingEvents = true;
        }

        public void Dispose()
        {
            if (m_FileSystemWatcher != null) m_FileSystemWatcher.Dispose();
            m_FileSystemWatcher = null;
        }

    }
}


