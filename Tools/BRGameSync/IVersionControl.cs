using System;
using System.Collections.Generic;
using SF;

namespace BR
{
    public abstract class IVersionControl
    {
        [Flags]
        public enum ListOption
        {
            None = 0,
            Recursive = 1,
        }

        public struct FileItem
        {
            public UInt32 FileVersion;
            public string FilePath;
            public long Size;
            public DateTime Modified;
        }

        public abstract string LocalPath { get; }
        public abstract string RemotePath { get; }

        public abstract string ToLocalPath(string relativePath);

        public abstract string ToRemotePath(string relativePath);



        public abstract IList<FileItem> GetFileList(string remotePath, ListOption listOption = ListOption.None);

        public abstract bool FileExists(string remotePath);

        // Mark for delete
        public abstract void MarkDelete(string remotePath);
        public abstract void MarkChanged(string remotePath);
        public abstract void MarkAdd(string remotePath);


        public abstract Result SetChangeDescription(string description);
        public abstract Result SubmitChanges();

        public abstract void SyncFull(bool bForce = false);
        public abstract void SyncFolder();
    }
}


