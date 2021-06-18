using System;
using System.IO;
using System.Collections.Generic;
using SF;
using SF.Tool;
using System.Data;
using System.Text;
using System.Threading.Tasks;
using System.Linq;
using System.Threading;
using System.Net.Http;
using System.Net;

namespace BR
{
    /// <summary>
    /// VersionFileStorage base class
    /// </summary>
    public abstract class VersionFileStorage
    {
        public string UserName { get; protected set; }

        protected VersionControlPath m_PathControl;

        public VersionFileStorage(VersionControlPath pathControl)
        {
            m_PathControl = pathControl;

            // default user name
            UserName = Environment.UserName;
        }

        public abstract Task<bool> UploadFiles(List<VersionFileInfo> fileInfos);

        public abstract Task<bool> DownloadFiles(List<VersionFileInfo> fileInfos);
    }


}


