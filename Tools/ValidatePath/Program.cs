using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using BR.ToolLib;

using System.IO;
using System.Diagnostics;

namespace shellbridge
{
    class Program
    {
        static int Main(string[] args)
        {
            ToolEnvironment.CheckDebugOption("pathconverter");

            string path = ToolEnvironment.GetSettingString("path");
            if (string.IsNullOrEmpty(path))
            {
                Console.WriteLine("Empty path paramater:");
                return -1;
            }


            PathTool.NormalizePathAndCreate(path);

            return 0;
        }
    }
}
