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


