﻿////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves Corporation.
// 
// Author : MadK
//
// Description : CSharp Message parser builder
//
////////////////////////////////////////////////////////////////////////////////

using System;
using System.Collections.Generic;
using System.Text;
using ProtocolXml;
using BR;

namespace ProtocolBuilder
{
    class MessageParserCSharpBuilder : Builder
    {
        // Message code index for message ID generation
        private int MessageCodeIndex = 0;

        // constructor
        public MessageParserCSharpBuilder(string strBasePath)
            : base(strBasePath + "Lib/Protocol/CSharp")
        {
        }

        public string BuilderNamespace
        {
            get { return "Message"; }
        }

        public string OutputFileName()
        {
            return Group.Name + "MsgClass.cs";
        }

        override public string ArrayLenType
        {
            get { return "UInt16"; }
        }

        void BuildPrefix()
        {
            OutStream.WriteLine("////////////////////////////////////////////////////////////////////////////////");
            OutStream.WriteLine("// ");
            OutStream.WriteLine("// CopyRight (c) 2013 The Braves");
            OutStream.WriteLine("// ");
            OutStream.WriteLine("// Author : Generated");
            OutStream.WriteLine("// ");
            OutStream.WriteLine(string.Format("// Description : {0} Message implementations", Group.Name));
            OutStream.WriteLine("// ");
            OutStream.WriteLine("////////////////////////////////////////////////////////////////////////////////");
            OutStream.WriteLine("");
            OutStream.WriteLine("");
            OutStream.WriteLine("using System;");
            OutStream.WriteLine("using System.Diagnostics;");
            OutStream.WriteLine("using BR;");
            OutStream.WriteLine("using BR.Message;");
            OutStream.WriteLine("using BR.Net;");
            OutStream.WriteLine("#if ANDROID");
            OutStream.WriteLine("using Preserve = Android.Runtime.PreserveAttribute;");
            OutStream.WriteLine("#elif IOS");
            OutStream.WriteLine("using Preserve = MonoTouch.Foundation.PreserveAttribute;");
            OutStream.WriteLine("#else");
            OutStream.WriteLine("using Preserve = MonoGame.Framework.PreserveAttribute;");
            OutStream.WriteLine("#endif");

            NewLine(3);

            // namespace definition
            OpenSection("namespace", PrjPrefix + "." + BuilderNamespace, false);
            OpenSection("namespace", Group.Name, false);
        }

        // Message trace
        void BuildMessageTrace(string Name, string typeName, string traceChannel, Parameter[] parameters)
        {
            OpenSection("public override void", "TraceOut(string Prefix)", false);

            string strTrace = "\"{{0}}:" + Name + typeName + ": ";
            string strTraceMember = "Prefix";
            int ParamCount = 1;

            if (parameters != null)
            {
                foreach (Parameter param in parameters)
                {
                    switch (param.Type)
                    {
                        case ParameterType.String:
                            {
                                strTrace += string.Format(", {0}:{{{1}}}", param.Name, ParamCount++);
                                strTraceMember += string.Format(", {0}", param.Name);
                            } break;
                        default:
                            if (param.IsArray)
                            {
                                strTrace += string.Format(", {0}:{{{1}}}", param.Name, ParamCount++);
                                strTraceMember += string.Format(", {0}.ToString()", param.Name);
                            }
                            else
                            {
                                if (param.Type == ParameterType.HRESULT)
                                {
                                    strTrace += string.Format(", {0}:{{{1}}}", param.Name, ParamCount++);
                                    strTraceMember += string.Format(", {0}.ToString()", param.Name);
                                }
                                else
                                {
                                    strTrace += string.Format(", {0}:{{{1}}}", param.Name, ParamCount++);
                                    strTraceMember += string.Format(", {0}.ToString()", param.Name);
                                }
                            }
                            break;
                    }
                }
            }

            MatchIndent(); OutStream.WriteLine(string.Format("Debug.Print({0},", strTrace + "\""));
            MatchIndent(7); OutStream.WriteLine(string.Format("\t{0}); ", strTraceMember));

            CloseSection();

            NewLine();
        }

        // Make shortest csharp type string
        public string CSharpTypeString(Type csType)
        {
            string[] splited = csType.ToString().Split('.');
            if (splited.Length > 1)
                return splited[splited.Length - 1];
            else
                return splited[0];
        }

        public bool IsStruct(ParameterType type)
        {
            Type csType = SystemTypeMap.ToCSharpType(type);
            return IsStruct(csType);
        }

        public bool IsStruct(Type csType)
        {
            bool bIsStruct = false;
            foreach (Attribute attribute in Attribute.GetCustomAttributes(csType))
            {
                if (attribute.GetType() == typeof(StructAttribute))
                {
                    bIsStruct = true;
                    break;
                }
            }
            return bIsStruct;
        }

        // Builder parameter string
        public string ParamInString( string strPrefix, Parameter[] parameter)
        {
            string strParams = "", strComma = "";

            if (parameter == null)
                return strParams;

            foreach (Parameter param in parameter)
            {
                strParams += strComma;
                if( strComma.Length == 0 )
                    strComma = ", ";

                Type csType = SystemTypeMap.ToCSharpType(param.Type);
                bool bIsStruct = IsStruct(csType);

                if (IsStrType(param)) // string type
                {
                    strParams += string.Format("string {0}{1}", strPrefix, InParamName(param.Name));
                }
                else if (param.IsArray) // array
                {
                    strParams += string.Format("{0}[] {1}{2}", csType.ToString(), strPrefix, InParamName(param.Name));
                }
                else // generic type
                {
                    if (bIsStruct)
                    {
                        strParams += string.Format("ref {0} {1}{2}", csType.ToString(), strPrefix, InParamName(param.Name));
                    }
                    else
                    {
                        strParams += string.Format("{0} {1}{2}", csType.ToString(), strPrefix, InParamName(param.Name));
                    }
                }
            }

            return strParams;
        }

        // Builder parameter string
        public string ParamMemberString(Parameter[] parameter)
        {
            string strParams = "", strComma = "";

            if (parameter == null)
                return strParams;

            foreach (Parameter param in parameter)
            {
                strParams += strComma;
                if (strComma.Length == 0)
                    strComma = ", ";

                Type csType = SystemTypeMap.ToCSharpType(param.Type);
                bool bIsStruct = false;
                foreach (Attribute attribute in Attribute.GetCustomAttributes(csType))
                {
                    if (attribute.GetType() == typeof(StructAttribute))
                    {
                        bIsStruct = true;
                        break;
                    }
                }

                if (IsStrType(param)) // string type
                {
                    strParams += string.Format("{0}", param.Name);
                }
                else if (param.IsArray) // array
                {
                    strParams += string.Format("{0}", param.Name);
                }
                else // generic type
                {
                    if (bIsStruct)
                    {
                        strParams += string.Format("ref {0}", param.Name);
                    }
                    else
                    {
                        strParams += string.Format("{0}", param.Name);
                    }
                }
            }

            return strParams;
        }
        
        // Builder parameters size
        public void BuildParamSize(Parameter[] parameters, string strSizeVarName)
        {
            // size calculation
            MatchIndent();
            OutStream.Write(string.Format("int {0} = sizeof(UInt64)*2 ", strSizeVarName));

            if (parameters != null)
            {
                foreach (Parameter param in parameters)
                {
                    NewLine(); MatchIndent(1);
                    BR.SystemTypeMap.TypeMap typeMap = SystemTypeMap.GetParameterInfo(param.Type);
                    if (param.Type == ParameterType.String)
                    {
                        OutStream.Write("+ binWriter.StringEncoder.GetByteCount({0}) + 1 + 2", InParamName(param.Name));
                    }
                    else
                    {
                        if (param.IsArray)
                        {
                            OutStream.Write("+ {0}*({1}.Length) + sizeof(UInt16)", typeMap.ByteSize, InParamName(param.Name));
                        }
                        else
                        {
                            OutStream.Write("+ {0}", typeMap.ByteSize );
                        }
                    }
                }
            }

            OutStream.WriteLine(";");
        }

        // Build parser class implementation
        void BuildBuilderSelfImpl(Parameter[] parameters)
        {
            OpenSection("public override void", "BuildMessageBuffer(BinaryMemoryWriter binWriter)", false);

            MatchIndent(); OutStream.WriteLine(string.Format("BuildMessageBuffer( binWriter, ref Header {0} {1} );", parameters.Length > 0 ? "," : "", ParamMemberString(parameters)));

            CloseSection();
        }

        // Build parser class implementation
        void BuildBuilderImpl(Parameter[] parameters)
        {
            OpenSection("static public void ", string.Format("BuildMessageBuffer( BinaryMemoryWriter binaryWriter, ref MessageHeader header {0} {1} )", parameters.Length > 0 ? "," : "", ParamInString("", parameters)), false);

            MatchIndent(); OutStream.WriteLine("var binWriter = (PacketMemoryWriter)binaryWriter;");

            BuildParamSize(parameters, "__uiMessageSize");
            NewLine();

            MatchIndent(); OutStream.WriteLine("BRDebug.Assert((binWriter.Capacity - binWriter.Position) >= __uiMessageSize);");
            NewLine();
            MatchIndent(); OutStream.WriteLine("header.Length = (uint)__uiMessageSize;");
            //MatchIndent(); OutStream.WriteLine("header.Write(binWriter);");

            foreach (Parameter param in parameters)
            {
                if (param.IsArray)
                {
                    //MatchIndent(); OutStream.WriteLine("binWriter.Write((UInt16)In{0}.Length);", param.Name);
                }

                string refString = "";
                if (IsStruct(param.Type))
                    refString = "ref";

                MatchIndent(); OutStream.WriteLine("binWriter.Write({0} In{1});", refString, param.Name);
            }

            NewLine();

            CloseSection();
        }

        // build message class
        void BuildMessageClass( MessageBase baseMsg, string typeName, Parameter[] parameters)
        {
            string Name = baseMsg.Name;
            string strClassName = MsgClassName(Name, typeName);
            MatchIndent(); OutStream.WriteLine("[Preserve (AllMembers = true)]");
            OpenSection("public class", strClassName + " : MessageBase", false);

            NewLine();
            MatchIndent(); OutStream.WriteLine(string.Format("public static MessageID MID = new MessageID(MSGTYPE.{0}, {1}, {2}, POLICY.{3}, {4});",
                typeName.ToUpper(), baseMsg.Reliable.ToString().ToLower(), Group.IsMobile.ToString().ToLower(), Group.Name.ToUpper(), MessageCodeIndex));
            NewLine();

            // member section
            if (parameters != null)
            {
                foreach (Parameter param in parameters)
                {
                    switch (param.Type)
                    {
                        case ParameterType.String:
                            MatchIndent(); OutStream.WriteLine( string.Format("public string {0} {{ get; private set; }}", param.Name));
                            break;
                        default:
                            Type csType = SystemTypeMap.ToCSharpType(param.Type);
                            if (param.IsArray)
                            {
                                MatchIndent(); OutStream.WriteLine(
                                    string.Format("public {0}[] {1};", csType.ToString(), param.Name));
                            }
                            else
                            {
                                MatchIndent(); OutStream.WriteLine(
                                    string.Format("public {0} {1};", csType.ToString(), param.Name));
                            }
                            break;
                    }

                }
                
            }

            // Constructor
            MatchIndent(); OutStream.WriteLine("public "+strClassName + "()");
            MatchIndent(1); OutStream.WriteLine("{}");
            NewLine();

            MatchIndent(); OutStream.WriteLine("public " + strClassName + "( ref MessageHeader msgHeader )");
            MatchIndent(1); OutStream.WriteLine(":base(ref msgHeader)");
            MatchIndent(1); OutStream.WriteLine("{}");
            NewLine();

            NewLine();
            // message trace function
            BuildMessageTrace(Name, typeName, baseMsg.Trace.ToString(), parameters);
            NewLine();

            // Parse function
            BuildMessageClassParserImpl(baseMsg.Name, typeName, parameters); NewLine();
            NewLine();

            // Build function
            BuildBuilderSelfImpl(parameters);
            NewLine();

            BuildBuilderImpl(parameters);
            NewLine();

            CloseSection();
        }


        // Build parser class implementation
        void BuildMessageClassParserImpl(string Name, string typeName, Parameter[] parameters)
        {
            OpenSection("public override void", "ParseMsg(BinaryMemoryReader binaryReader)", false);

            MatchIndent(); OutStream.WriteLine("var binReader = (PacketMemoryReader)binaryReader;");

            if (parameters == null)
            {
                // nothing to process
                MatchIndent(); OutStream.WriteLine("return;");
            }
            else
            {

                foreach (Parameter param in parameters)
                {
                    Type csType = SystemTypeMap.ToCSharpType(param.Type);
                    switch (param.Type)
                    {
                        case ParameterType.String:
                            MatchIndent(); OutStream.WriteLine(
                                string.Format("{0} = binReader.ReadString();", param.Name));
                            break;
                        default:
                            if (param.IsArray)
                            {
                                MatchIndent(); OutStream.WriteLine(
                                    string.Format("UInt16 {0} = 0;", ArrayLenName(param.Name)));
                                MatchIndent(); OutStream.WriteLine(
                                    string.Format("{0} = binReader.ReadUInt16();", ArrayLenName(param.Name)));
                                MatchIndent(); OutStream.WriteLine(
                                    string.Format("{1} = new {0}[{2}];", csType.ToString(), param.Name, ArrayLenName(param.Name)));

                                if (csType == typeof(byte))
                                {
                                    MatchIndent(); OutStream.WriteLine("binReader.ReadBytes({0});", param.Name);
                                }
                                else
                                {
                                    MatchIndent(); OutStream.WriteLine(
                                        string.Format("for( int iElement = 0; iElement < {0}; iElement++) {{", ArrayLenName(param.Name)));
                                    MatchIndent(); OutStream.WriteLine(
                                        string.Format("{1}[iElement] = binReader.Read{0}();", CSharpTypeString(csType), param.Name, ArrayLenName(param.Name)));
                                    MatchIndent(); OutStream.WriteLine("}");
                                }
                            }
                            else
                            {
                                MatchIndent(); OutStream.WriteLine(
                                    string.Format("{1} = binReader.Read{0}();", CSharpTypeString(csType), param.Name, ArrayLenName(param.Name)));
                            }
                            break;
                    }
                    NewLine();
                }
            }

            CloseSection();
        }


        void BuildMessageParserClassCSharp()
        {
            // Packer interface
            Parameter[] newparams;

            MessageCodeIndex = 0;
            foreach (MessageBase baseMsg in Group.Items)
            {
                if (baseMsg is ProtocolsProtocolGroupCommand)
                {
                    MatchIndent(); OutStream.WriteLine("// Cmd: " + baseMsg.Desc);
                    ProtocolsProtocolGroupCommand msg = baseMsg as ProtocolsProtocolGroupCommand;

                    newparams = MakeParameters(MsgType.Cmd, msg.Cmd);
                    BuildMessageClass(msg, "Cmd", newparams);

                    newparams = MakeParameters(MsgType.Res, msg.Res);
                    BuildMessageClass(msg, "Res", newparams); NewLine();
                }

                if (baseMsg is ProtocolsProtocolGroupC2SEvent)
                {
                    MatchIndent(); OutStream.WriteLine("// C2S: " + baseMsg.Desc);
                    ProtocolsProtocolGroupC2SEvent msg = baseMsg as ProtocolsProtocolGroupC2SEvent;

                    newparams = MakeParameters(MsgType.Evt, msg.Params);
                    BuildMessageClass(msg, "C2SEvt", newparams); NewLine();
                }

                if (baseMsg is ProtocolsProtocolGroupS2CEvent)
                {
                    MatchIndent(); OutStream.WriteLine("// S2C: " + baseMsg.Desc);
                    ProtocolsProtocolGroupS2CEvent msg = baseMsg as ProtocolsProtocolGroupS2CEvent;

                    newparams = MakeParameters(MsgType.Evt, msg.Params);
                    BuildMessageClass(msg, "S2CEvt", newparams); NewLine();
                }

                MessageCodeIndex++;
            }
        }


        public override void Build()
        {
            if (!Group.GenCSharp)
                return;

            // Build Parser class
            OpenOutFile(OutputFileName());

            BuildPrefix();
            NewLine(2);


            BuildMessageParserClassCSharp();
            NewLine(2);

            NewLine(2);
            CloseOutFile();
        }

    }
}