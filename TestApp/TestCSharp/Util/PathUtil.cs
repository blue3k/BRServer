using System;
using System.IO;


namespace BR.Engine
{
    public static class PathUtil
    {
        static public string NormalizeBasePath(string basePath)
        {
            basePath = Path.GetFullPath(basePath.Replace('\\', Path.DirectorySeparatorChar)).ToLower();
            char lastCharacter = basePath[basePath.Length - 1];
            if (Path.DirectorySeparatorChar != lastCharacter && Path.AltDirectorySeparatorChar != lastCharacter)
                basePath = basePath + Path.DirectorySeparatorChar;
            return basePath;
        }

        // 
        static public string NormalizeFilePath(string filePath)
        {
            filePath = Path.GetFullPath(filePath.Replace('\\', Path.DirectorySeparatorChar)).ToLower();
            return filePath;
        }

        // 
        static public string MakeRelativePath(string basePath, string filePath)
        {
            filePath = Path.GetFullPath(filePath.Replace('\\', Path.DirectorySeparatorChar)).ToLower();

            System.Diagnostics.Debug.Assert(filePath.StartsWith(basePath));

            return filePath.Substring(basePath.Length, filePath.Length - basePath.Length);
        }

        // Delete whole tree
        static public void DeleteTree(string basePath)
        {
            string[] fileList = Directory.GetFiles(basePath, "*.*");
            foreach (var file in fileList)
            {
                File.Delete(file);
            }


            string[] directoryList = Directory.GetDirectories(basePath, "*.*");
            foreach (var directory in directoryList)
            {
                DeleteTree(directory);
            }

            if( Directory.Exists(basePath) )
                Directory.Delete(basePath);
        }

    }
}
