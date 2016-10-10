﻿////////////////////////////////////////////////////////////////////////////////
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


namespace BRResultCodeCompiler
{
    class ResultCodeExporterH
    {
        ResultCodes m_Codes;
        string m_InputName;


        public void Export(ResultCodes Codes, string inputName, string outDir)
        {
            StringBuilder output = new StringBuilder();

            //m_Facilities = Facilities;
            m_Codes = Codes;
            m_InputName = inputName;

            WritePreamble(output);
            WriteBody(output);
            WritePostamble(output);

            WriteToOutputHeader(outDir, output);
        }

        void WritePreamble(StringBuilder output)
        {
            output.Append("////////////////////////////////////////////////////////////////////////////////\n");
            output.Append("// \n");
            output.Append("// CopyRight (c) 2016 Blue3k\n");
            output.Append("// \n");
            output.Append("// Author : KyungKun Ko\n");
            output.Append("//\n");
            output.AppendFormat("// Description : Autogenerated header, {0}\n", m_Codes.Facility);
            output.Append("//\n");
            output.Append("////////////////////////////////////////////////////////////////////////////////\n");
            output.AppendLine("");
            output.AppendLine("#pragma once");
            output.AppendLine("#include <stdint.h>");
            output.AppendLine("");
            output.AppendLine("");
        }

        void WriteBody(StringBuilder output)
        {
            const int tabSize = 70;
            var facility = m_Codes.Facility;
            var facilityUpr = facility.ToUpper();

            output.AppendFormat("\n");
            output.AppendFormat("namespace BR {{\n");
            output.AppendFormat("\tnamespace ResultCode {{\n");
            output.AppendFormat("\n");
            output.AppendFormat("\t\tenum {{\n");
            output.AppendFormat("\n");

            foreach (var codeItem in m_Codes.ResultCodeItem)
            {
                var codeValue = codeItem.ResultCode;
                string strDefine = "";
                //if(m_Codes.UseFacilityName)
                //    strDefine = string.Format("#define {0}_{1}_{2}        ", ResultCode.ServerityToDefineString(codeValue.Severity), facilityUpr, codeItem.CodeName.ToUpper());
                //else
                //    strDefine = string.Format("#define {0}_{1}        ", ResultCode.ServerityToDefineString(codeValue.Severity), codeItem.CodeName.ToUpper());
                if (m_Codes.UseFacilityName)
                    strDefine = string.Format("\t\t\t{0}{1}_{2}        ", ResultCode.ServerityToDefineString(codeValue.Severity), facilityUpr, codeItem.CodeName.ToUpper());
                else
                    strDefine = string.Format("\t\t\t{0}{1}        ", ResultCode.ServerityToDefineString(codeValue.Severity), codeItem.CodeName.ToUpper());

                output.AppendFormat("\n");
                output.AppendFormat("\t\t\t// {0} \n", string.IsNullOrEmpty(codeItem.Desc) ? "" : codeItem.Desc);
                output.AppendFormat("{0}", strDefine);
                if (strDefine.Length <= tabSize)
                    output.Append(' ', tabSize - strDefine.Length);
                output.AppendFormat("= 0x{0:X8}L, \n", codeValue.ID);
            }


            output.AppendFormat("\n");
            output.AppendFormat("\t\t}};//enum \n");
            output.AppendFormat("\t}}//namespace ResultCode \n");
            output.AppendFormat("}}//namespace SF \n");
            output.AppendFormat("\n");
        }
        void WritePostamble(StringBuilder output)
        {
            output.AppendLine("");
            output.AppendLine("");
            output.AppendLine("");
            output.AppendLine("");
        }

        void WriteToOutputHeader(string outDir, StringBuilder output)
        {
            var outPath = Path.Combine(outDir, string.Format("BR{0}.h", m_InputName));

            string outputString = output.ToString();
            var fileData = Encoding.UTF8.GetBytes(outputString);
            var preamble = Encoding.UTF8.GetPreamble();

            var memoryStream = new MemoryStream(fileData.Length + preamble.Length);
            memoryStream.Write(preamble, 0, preamble.Length);
            memoryStream.Write(fileData, 0, fileData.Length);
            FileUtil.WriteIfChanged(outPath, memoryStream.GetBuffer(), output.Length);
        }
    }
}
