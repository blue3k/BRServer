using System;
using System.IO;
using System.Collections.Generic;
using SF;
using SF.Tool;
using Azure.Core;
using Azure.Identity;
using Azure.Storage.Blobs;
using Azure.Storage.Blobs.Models;
using System.Threading.Tasks;
using System.IdentityModel.Tokens.Jwt;
using System.Linq;
using Azure;


namespace BR
{

    public class VersionFileStorageAzureBlob : VersionFileStorage
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

        BlobContainerClient m_FileStorage;


        public VersionFileStorageAzureBlob(VersionControlPath pathControl)
            : base(pathControl)
        {
            m_CredentialOptions.InteractiveBrowserTenantId = AppConfig.GetValue<string>("AzureTenantId");
            m_AccountUri = new Uri(AppConfig.GetValue<string>("AzureStorage"));

            var browserOption = new InteractiveBrowserCredentialOptions()
            {
                TenantId = AppConfig.GetValue<string>("AzureTenantId"),
                ClientId = AppConfig.GetValue<string>("AzureClientId"),
                TokenCachePersistenceOptions = new TokenCachePersistenceOptions(),
                RedirectUri = new Uri("http://localhost")
            };

            //BlobClient

            m_Credential = new InteractiveBrowserCredential(browserOption);
            //var cancelToken2 = new CancellationToken();
            //var record = m_Credential.AuthenticateAsync();
            //m_Credential = new Azure.Identity.DefaultAzureCredential(m_CredentialOptions);

            //WebProxy proxy = new WebProxy("http://158.247.199.48:3128");
            //HttpClientHandler  httpClientHandler = new HttpClientHandler()
            //{
            //    AllowAutoRedirect = true,
            //    //AutomaticDecompression = DecompressionMethods.Deflate | DecompressionMethods.GZip,
            //    Proxy = proxy,
            //};
            //var clientOptions = new BlobClientOptions()
            //{
            //     Transport = new HttpClientTransport(new HttpClient(httpClientHandler))
            //};
            m_FileStorage = new BlobContainerClient(m_AccountUri, m_Credential);

            m_FileStorage.GetBlobs(prefix: ".git");// kick off connect&login by accessing something

            // accessing local
            //var nameCredential = new InteractiveBrowserCredential(browserOption);
            //var nameCredential = new Azure.Identity.DefaultAzureCredential(m_CredentialOptions);
            var nameCredential = m_Credential;
            string[] scopes = new string[] { "https://graph.microsoft.com/.default" };
            var cancelToken = new System.Threading.CancellationToken();
            try
            {
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

                UserName = userIdentity;
            }
            catch (Exception exp)
            {
                SF.Log.Error("Getting username failed, using default. error:{0} => {1}", exp.Message, exp.StackTrace.ToString());
            }

            SF.Log.Info("User logged in, Username {0}", UserName);
        }

        public override async Task<bool> UploadFiles(List<VersionFileInfo> fileInfos)
        {
            SF.Log.Info("Uploading {0} files", fileInfos.Count);

            var MaxRequest = AppConfig.GetValue<int>("MaxRequest",10);
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

                    if (uploadTasks.Count > MaxRequest)
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

        public override async Task<bool> DownloadFiles(List<VersionFileInfo> fileInfos)
        {
            var downloadTasks = new List<Task<Azure.Response>>();

            var MaxRequest = AppConfig.GetValue<int>("MaxRequest", 10);

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

                if (downloadTasks.Count > MaxRequest)
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

}


