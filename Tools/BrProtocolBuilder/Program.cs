////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves Corporation.
// 
// Author : MadK
//
// Description : Main program 
//
////////////////////////////////////////////////////////////////////////////////

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Data;


using System.Reflection;






namespace ProtocolBuilder
{
    class Program
    {
        static ProtocolData m_protocolData;

        static string strTargetPath = "../../Test";
        //static string strCSharpTargetPath = "../../Test/CSharp";
        static string strTempPath = "Temp/";


        static void Main(string[] args)
        {
            if (args.Length < 1)
            {
                Console.WriteLine("Need more parameter");
                Console.WriteLine("Usage : ProtocolBuilder.exe -out:OutputPath xmlFile1 [xmlFile2...]");
                return;
            }

            m_protocolData = new ProtocolData();

            // Load protocol XML
            for (int iArg = 0; iArg < args.Length; iArg++)
            {
                string strArg = args[iArg];

                if (strArg.StartsWith("-dir:", true, null))
                {
                    strArg.ToLower();
                    strTargetPath = strArg.Remove(0, ("-dir:").Length);
                }
                else if (strArg.StartsWith("-gen:", true, null))
                {
                }
                else if (!m_protocolData.LoadFile(strArg))
                {
                    Console.WriteLine("XML Loading Failed ");
                    return;
                }
            }

            // Generate
            try
            {
                List<Builder> builders = new List<Builder>();

                //builders.Add(new MessageIDBuilder(strTempPath));
                builders.Add(new MessageBuilder(strTempPath));
                builders.Add(new MessageDebugBuilder(strTempPath));
                builders.Add(new MessageIPolicyBuilder(strTempPath));
                builders.Add(new MessageNetPolicyBuilder(strTempPath));
                builders.Add(new MessageServerServiceBuilder(strTempPath));
                //builders.Add(new MessageClusterNetPolicyBuilder(strTempPath));
                builders.Add(new MessageParserCSharpBuilder(strTempPath));
                builders.Add(new MessagePolicyCSharpBuilder(strTempPath));
                

                // Generate Server/Client Network Policy
                foreach (ProtocolXml.ProtocolsProtocolGroup group in m_protocolData.ProtocolGroupList)
                {
                    foreach (Builder builder in builders)
                    {
                        builder.Group = group;
                        builder.Build();
                    }
                }

                // copy only changed file
                string[] files = Directory.GetFiles(strTempPath, "*.*", SearchOption.AllDirectories);
                foreach (string file in files)
                {
                    CopyIfChanged(file);
                }
           }
            catch (System.IO.IOException sysex)
            {
                Console.WriteLine("System IO exception: " + sysex.Message);
            }
            catch (InvalidOperationException ioe)
            {
                Console.WriteLine("Invalid Operation: " + ioe.Message);
            }
            catch (SystemException ioe)
            {
                Console.WriteLine("System Exception: " + ioe.Message);
            }
            finally
            {
                Console.WriteLine("Work Done!");
            }
        }

        // copy if changed
        static void CopyIfChanged(string sourceFilePath)
        {
            string targetFilePath = strTargetPath + sourceFilePath.Remove(0,strTempPath.Length);
            string targetDir = Path.GetDirectoryName(targetFilePath);

            Directory.CreateDirectory(targetDir);

            char[] sourceFileBuffer = new char[7 * 1024];
            int sourceFileReadSize = 0;
            char[] targetFileBuffer = new char[7 * 1024];
            int targetFileReadSize = 0;

            bool bIsMissMatched = false;
            using (FileStream source = new FileStream(sourceFilePath, FileMode.Open, FileAccess.Read, FileShare.Read))
            using (StreamReader sourceFile = new StreamReader(source, Encoding.UTF8))
            {
                try
                {
                    using (FileStream target = new FileStream(targetFilePath, FileMode.Open, FileAccess.Read, FileShare.Read))
                    using (StreamReader targetFile = new StreamReader(target, Encoding.UTF8))
                    {
                        while (!bIsMissMatched)
                        {
                            sourceFileReadSize = sourceFile.Read(sourceFileBuffer, 0, sourceFileBuffer.Length);
                            targetFileReadSize = targetFile.Read(targetFileBuffer, 0, targetFileBuffer.Length);
                            if (targetFileReadSize != sourceFileReadSize)
                            {
                                bIsMissMatched = true;
                                break;
                            }
                            else if (targetFileReadSize == 0)
                            {
                                break;
                            }

                            int iChar = 0;
                            for (iChar = 0; iChar < sourceFileReadSize; iChar++)
                            {
                                if (sourceFileBuffer[iChar] != targetFileBuffer[iChar])
                                {
                                    bIsMissMatched = true;
                                    break;
                                }
                            }
                        }
                    }
                }
                catch (System.IO.IOException)
                {
                    // maybe file not exist, create new one
                    bIsMissMatched = true;
                }
            }

            if( !bIsMissMatched )
                return;

            Console.WriteLine( "Copying : " + targetFilePath );

            File.Copy(sourceFilePath, targetFilePath, true);
        }
   
    }
}
