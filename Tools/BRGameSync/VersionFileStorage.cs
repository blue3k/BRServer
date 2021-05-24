﻿using System;
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
using System.IdentityModel.Tokens.Jwt;
using System.Linq;
using Azure;
using System.Threading;

namespace BR
{

    public class VersionFileStorage
    {
        static DefaultAzureCredentialOptions m_CredentialOptions = new DefaultAzureCredentialOptions()
        {
            //ExcludeEnvironmentCredential = true,
            //ExcludeManagedIdentityCredential = true,
            ExcludeSharedTokenCacheCredential = false,
            ExcludeInteractiveBrowserCredential = false,
            ExcludeAzureCliCredential = true,
            //ExcludeVisualStudioCredential = true,
            //ExcludeVisualStudioCodeCredential = true,
            //ExcludeAzurePowerShellCredential = true,
        };
        TokenCredential m_Credential;
        Uri m_AccountUri;

        VersionControlPath m_PathControl;

        BlobContainerClient m_FileStorage;

        public string UserName { get; private set; }

        public VersionFileStorage(VersionControlPath pathControl)
        {
            m_CredentialOptions.InteractiveBrowserTenantId = AppConfig.GetValue<string>("AzureTenantId");
            m_AccountUri = new Uri(AppConfig.GetValue<string>("AzureStorage"));

            var browserOption = new InteractiveBrowserCredentialOptions()
            {
                TenantId = AppConfig.GetValue<string>("AzureTenantId"),
                ClientId = "1af8fa92-2a2a-4f71-88b9-3ddd59303160",
                TokenCachePersistenceOptions = new TokenCachePersistenceOptions(),
                RedirectUri = new Uri("https://login.microsoftonline.com/common/oauth2/nativeclient")
            };

            //m_Credential = new InteractiveBrowserCredential(browserOption);
            //var cancelToken2 = new CancellationToken();
            //var record = m_Credential.AuthenticateAsync();
            m_Credential = new Azure.Identity.DefaultAzureCredential(m_CredentialOptions);
            m_PathControl = pathControl;
            m_FileStorage = new BlobContainerClient(m_AccountUri, m_Credential);

            m_FileStorage.GetBlobs(prefix: ".git");// kick off connect&login by accessing something

            // accessing local
            var nameCredential = new Azure.Identity.DefaultAzureCredential(m_CredentialOptions);
            string[] scopes = new string[] { "https://graph.microsoft.com/.default" };
            var cancelToken = new System.Threading.CancellationToken();
            var token = nameCredential.GetToken(new Azure.Core.TokenRequestContext(scopes), cancelToken);

            var handler = new JwtSecurityTokenHandler();
            var jsonToken = handler.ReadToken(token.Token) as JwtSecurityToken;

            string userIdentity = "";
            string[] IdentityItemNames = new string[] { "name"/*, "email"*/ };
            foreach (var identityName in IdentityItemNames)
            {
                var identityItem = jsonToken.Claims.First(c => c.Type == identityName);
                if (identityItem != null)
                {
                    if (userIdentity.Length > 0)
                        userIdentity += ",";
                    userIdentity += identityItem.Value;
                }
            }

            UserName = userIdentity;// Environment.UserName;
        }

        public List<VersionFileInfo> GetFileList(string remotePrefix = null)
        {
            var result = new List<VersionFileInfo>();
            Azure.Pageable<BlobItem> blobs;
            blobs = m_FileStorage.GetBlobs(prefix: remotePrefix);
            foreach (BlobItem blobItem in blobs)
            {
                if (blobItem.Deleted)
                    continue;

                VersionFileInfo item = new VersionFileInfo();

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

        public async Task<bool> UploadFiles(List<VersionFileInfo> fileInfos)
        {
            SF.Log.Info("Uploading {0} files", fileInfos.Count);

            var fileStreamList = new List<FileStream>();
            var uploadTasks = new List<Task<Response<BlobContentInfo>>>();

            try
            {
                foreach (var fileInfo in fileInfos)
                {
                    if (string.IsNullOrEmpty(fileInfo.RemoteFilePath))
                    {
                        SF.Log.Error("Invalid remote path for uploading");
                        return false;
                    }

                    if (fileInfo.Deleted)
                        continue;

                    SF.Log.Info("Uploading: {0},{1}", fileInfo.RemoteFilePath, fileInfo.FileVersion);

                    var blobName = fileInfo.RemoteFilePath + "_" + fileInfo.FileVersion;
                    m_FileStorage.DeleteBlobIfExists(blobName);
                    var fileStream = File.OpenRead(m_PathControl.ToFullLocalPath(fileInfo.LocalFilePath));
                    fileStreamList.Add(fileStream);
                    var task = m_FileStorage.UploadBlobAsync(blobName, fileStream);
                    uploadTasks.Add(task);

                    if (uploadTasks.Count > 10)
                    {
                        SF.Log.Info("Flush uploading tasks");
                        await Task.WhenAll(uploadTasks.ToArray());
                        uploadTasks.Clear();
                    }
                }

                SF.Log.Info("Waiting upload tasks");
                await Task.WhenAll(uploadTasks.ToArray());
                SF.Log.Info("Upload finished");

            }
            finally
            {
                foreach (var fileStream in fileStreamList)
                {
                    fileStream.Close();
                    fileStream.Dispose();
                }
                fileStreamList.Clear();
            }

            return true;
        }

        public async Task<bool> DownloadFiles(List<VersionFileInfo> fileInfos)
        {
            var downloadTasks = new List<Task<Azure.Response>>();

            foreach (var fileInfo in fileInfos)
            {
                if (string.IsNullOrEmpty(fileInfo.RemoteFilePath))
                {
                    SF.Log.Error("Invalid remote file path for downloading");
                    return false;
                }

                SF.Log.Info("Downloading: {0},{1}", fileInfo.RemoteFilePath, fileInfo.FileVersion);

                var blobName = fileInfo.RemoteFilePath + "_" + fileInfo.FileVersion;

                var blobClient = m_FileStorage.GetBlobClient(blobName);
                var downloadPath = m_PathControl.ToFullLocalPath(fileInfo.LocalFilePath);
                var downloadDir = Path.GetDirectoryName(downloadPath);
                if (!Directory.Exists(downloadDir))
                {
                    Directory.CreateDirectory(downloadDir);
                }

                downloadTasks.Add(blobClient.DownloadToAsync(downloadPath));

                if (downloadTasks.Count > 10)
                {
                    SF.Log.Info("Flush download tasks");
                    await Task.WhenAll(downloadTasks.ToArray());
                    downloadTasks.Clear();
                }
            }

            SF.Log.Info("Waiting download tasks");

            await Task.WhenAll(downloadTasks.ToArray());

            SF.Log.Info("Download finished, updating file time");

            foreach (var fileInfo in fileInfos)
            {
                var downloadPath = m_PathControl.ToFullLocalPath(fileInfo.LocalFilePath);
                File.SetLastAccessTimeUtc(downloadPath, fileInfo.Modified.UtcDateTime); // Not working mostly, just putting it here
                File.SetLastWriteTimeUtc(downloadPath, fileInfo.Modified.UtcDateTime);
            }

            SF.Log.Info("Download finished");

            return true;
        }
    }

    public class FileIgnoreList
    {
        HashSet<string> m_IgnorePrefix = new HashSet<string>();
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

                        var index = line.IndexOf('*');
                        if (index == 0 // start with '*'
                            || line.StartsWith('.')) // 
                        {
                            var ext = Path.GetExtension(line);
                            AddIgnoreExt(ext);
                        }
                        else if (index > 0)
                        {
                            AddIgnorePrefix(line.Substring(0, index));
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

        public void AddIgnorePrefix(string ignorePrefix)
        {
            ignorePrefix = ignorePrefix.ToLower();
            if (m_IgnorePrefix.Contains(ignorePrefix)) return;

            m_IgnorePrefix.Add(ignorePrefix);
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
            var fileName = Path.GetFileName(localFileFullPath).ToLower();

            if (m_IgnoreExts.Contains(fileExt)) return true;
            foreach (var ignorePrefix in m_IgnorePrefix)
            {
                if (fileName.StartsWith(ignorePrefix))
                    return true;
            }

            try
            {
                FileAttributes localFileAttr = File.GetAttributes(localFileFullPath);
                var directoryPath = localFileAttr.HasFlag(FileAttributes.Directory) ? localFileFullPath : Path.GetDirectoryName(localFileFullPath);
                var dirNames = directoryPath.Split(Path.DirectorySeparatorChar);
                foreach (var dirName in dirNames)
                {
                    var testName = dirName.ToLower();
                    var dirExt = Path.GetExtension(directoryPath);
                    if (m_IgnoreExts.Contains(dirExt)) return true;
                    foreach(var ignorePrefix in m_IgnorePrefix)
                    {
                        if (testName.StartsWith(ignorePrefix))
                            return true;
                    }
                    if (m_IgnoreFolder.Contains(testName)) return true;
                }

            }
            catch (Exception)
            {
                // maybe no local file, ignore it
                return false;
            }

            return false;
        }

    }

}


