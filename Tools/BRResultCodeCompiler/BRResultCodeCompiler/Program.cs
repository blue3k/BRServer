////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 Blue3k
// 
// Author : KyungKun Ko
//
// Description : 
//
////////////////////////////////////////////////////////////////////////////////

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Serialization;
using BR;
using BR.ToolLib;
using BR.Table;

using System.IO;
using System.Diagnostics;


namespace BR.Table
{
    public partial class ResultCodesResultCodeItem
    {
        public ResultCode ResultCode;
    }
}

namespace BRResultCodeCompiler
{
    class Program
    {
        static int Main(string[] args)
        {
            ToolEnvironment.CheckDebugOption("brresultcodecompiler");

            string outDir = ToolEnvironment.GetSettingString("out");
            string outDirSharp = ToolEnvironment.GetSettingString("outSharp");
            string tableOutPath = ToolEnvironment.GetSettingString("tablepath");
            var inputPath = ToolEnvironment.GetValueSet("in");
            string facilitiyPath = ToolEnvironment.GetSettingString("facilitiy");
            if (inputPath == null)
            {
                Console.WriteLine("Empty input paramater:");
                return -1;
            }

            if (string.IsNullOrEmpty(outDir))
            {
                Console.WriteLine("Empty outDir paramater:");
                return -1;
            }

            if (string.IsNullOrEmpty(outDirSharp))
            {
                Console.WriteLine("Empty outDirSharp paramater:");
                return -1;
            }

            if (string.IsNullOrEmpty(tableOutPath))
            {
                Console.WriteLine("Empty tableOutPath paramater:");
                return -1;
            }


            if (string.IsNullOrEmpty(facilitiyPath))
            {
                Console.WriteLine("Empty facilities paramater:");
                return -1;
            }

            try
            {
                LoadFacility(facilitiyPath);

                var exporterXml = new ResultCodeExporterXml();

                foreach(var input in inputPath)
                {
                    LoadCodes(input.Key);

                    string inputName = Path.GetFileNameWithoutExtension(input.Key);
                    UpdateResultCode(m_Facilities, m_Codes);

                    GenerateHeaders(m_Codes, inputName, outDir);
                    GenerateSharp(m_Codes, inputName, outDirSharp);

                    exporterXml.AddNewTable(m_Codes);
                }

                exporterXml.Export(tableOutPath);
            }
            catch (Exception exp)
            {
                Console.WriteLine("ERROR: Exception:{0}, at {1}", exp.Message, exp.StackTrace.ToString());
            }

            return 0;
        }

        static Dictionary<string, ResultCodeFacility> m_Facilities = new Dictionary<string, ResultCodeFacility>();
        static void LoadFacility(string facilitiyPath)
        {
            ResultCodeFacilities facilities;
            XmlSerializer serializer = new XmlSerializer(typeof(ResultCodeFacilities));
            using (System.IO.StreamReader file = new System.IO.StreamReader(facilitiyPath))
            {
                facilities = (ResultCodeFacilities)serializer.Deserialize(file);
            }

            foreach(var facility in facilities.Facility)
            {
                m_Facilities.Add(facility.Facility, facility);
            }
        }

        static ResultCodes m_Codes;
        static void LoadCodes(string inputPath)
        {
            XmlSerializer serializer = new XmlSerializer(typeof(BR.Table.ResultCodes));
            using (System.IO.StreamReader file = new System.IO.StreamReader(inputPath))
            {
                m_Codes = (ResultCodes)serializer.Deserialize(file);
            }
        }

        static ResultCode.SeverityType ToLocalSeverityType(BR.Table.Severity serverity)
        {
            switch (serverity)
            {
                case Severity.Success: return ResultCode.SeverityType.Success;
                case Severity.Informational: return ResultCode.SeverityType.Informational;
                case Severity.Warning: return ResultCode.SeverityType.Warning;
                case Severity.Error:
                default:
                    return ResultCode.SeverityType.Error;
            }
        }

        static void UpdateResultCode(Dictionary<string, ResultCodeFacility> Facilities, ResultCodes Codes)
        {
            var facility = m_Codes.Facility;
            var facilityID = m_Facilities[m_Codes.Facility];
            int facilityValue = Convert.ToInt32(facilityID.Value, 16);

            ResultCode codeValue = new ResultCode();
            codeValue.Facility = facilityValue;

            int codeIndex = 0;
            foreach (var codeItem in m_Codes.ResultCodeItem)
            {
                codeValue.Severity = ToLocalSeverityType(codeItem.Severity);
                codeValue.Custom = codeItem.Custom;
                codeValue.Code = codeIndex;
                codeIndex++;

                codeItem.ResultCode = codeValue;
            }
        }


        static void GenerateHeaders(ResultCodes Codes, string inputName, string outDir)
        {
            var exporter = new ResultCodeExporterH();
            exporter.Export(Codes, inputName, outDir);
        }

        static void GenerateSharp(ResultCodes Codes, string inputName, string outDir)
        {
            var exporter = new ResultCodeExporterSharp();
            exporter.Export(Codes, inputName, outDir);
        }

    }
}
