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

        public List<IVersionControl.FileInfo> GetFileList(string remotePrefix = null)
        {
            var result = new List<IVersionControl.FileInfo>();
            Azure.Pageable<BlobItem> blobs;
            blobs = m_FileStorage.GetBlobs(prefix: remotePrefix);
            foreach (BlobItem blobItem in blobs)
            {
                if (blobItem.Deleted)
                    continue;

                IVersionControl.FileInfo item = new IVersionControl.FileInfo();

                var splitIndex = blobItem.Name.LastIndexOf('_');
                if (splitIndex >= 0)
                {
                    item.RemoteFilePath = blobItem.Name.Substring(0, splitIndex);
                    var versionString = blobItem.Name.Substring(splitIndex + 1, blobItem.Name.Length - (splitIndex + 1));
                    item.FileVersion = int.Parse(versionString);
                }
                item.LocalFilePath = m_PathControl.ToLocalPath(item.RemoteFilePath);
                item.Size = blobItem.Properties.ContentLength?? 0;
                item.Modified = blobItem.Properties.LastModified?? DateTimeOffset.MinValue;

                result.Add(item);
            }

            return result;
        }

        public void UploadFiles(List<IVersionControl.FileInfo> fileInfos)
        {
            foreach(var fileInfo in fileInfos)
            {
                if (string.IsNullOrEmpty(fileInfo.RemoteFilePath))
                {
                    throw new Exception("Invalid file path for uploading");
                }

                if (fileInfo.Deleted)
                    continue;

                var blobName = fileInfo.RemoteFilePath + "_" + fileInfo.FileVersion;
                m_FileStorage.DeleteBlobIfExists(blobName);
                m_FileStorage.UploadBlob(blobName, File.OpenRead(m_PathControl.ToFullLocalPath(fileInfo.LocalFilePath)));
            }
        }

        public void DownloadFiles(List<IVersionControl.FileInfo> fileInfos)
        {
            List<Task<Azure.Response>> downloadTasks = new List<Task<Azure.Response>>();
            foreach (var fileInfo in fileInfos)
            {
                if (string.IsNullOrEmpty(fileInfo.RemoteFilePath))
                {
                    throw new Exception("Invalid file path for uploading");
                }

                var blobName = fileInfo.RemoteFilePath + "_" + fileInfo.FileVersion;

                var blobClient = m_FileStorage.GetBlobClient(blobName);
                var downloadPath = m_PathControl.ToFullLocalPath(fileInfo.LocalFilePath);
                downloadTasks.Add(blobClient.DownloadToAsync(downloadPath));
            }

            Task.WaitAll(downloadTasks.ToArray());

            foreach (var fileInfo in fileInfos)
            {
                var downloadPath = m_PathControl.ToFullLocalPath(fileInfo.LocalFilePath);
                File.SetLastAccessTimeUtc(downloadPath, fileInfo.Modified.UtcDateTime);
                File.SetLastWriteTimeUtc(downloadPath, fileInfo.Modified.UtcDateTime);
            }
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
                        if (line == null) continue;

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
                Log.Error("{0} => {1}", exp.Message, exp.StackTrace.ToString());
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

        public bool IsIgnore(string localFileFullPath)
        {
            var fileExt = Path.GetExtension(localFileFullPath);

            if (m_IgnoreExts.Contains(fileExt)) return true;

            try
            {
                FileAttributes localFileAttr = File.GetAttributes(localFileFullPath);
                var directoryPath = localFileAttr.HasFlag(FileAttributes.Directory) ? localFileFullPath : Path.GetDirectoryName(localFileFullPath);
                var dirNames = directoryPath.Split(Path.DirectorySeparatorChar);
                foreach (var dirName in dirNames)
                {
                    var dirExt = Path.GetExtension(localFileFullPath);
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

}


