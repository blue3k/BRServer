////////////////////////////////////////////////////////////////////////////////
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
    class MessagePolicyCSharpBuilder : Builder
    {

        // constructor
        public MessagePolicyCSharpBuilder(string strBasePath)
            : base(strBasePath + "Lib/Protocol/CSharp")
        {
        }

        public string BuilderNamespace
        {
            get { return "Policy"; }
        }

        public string OutputFileName()
        {
            return Group.Name + "Policy.cs";
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
            OutStream.WriteLine(string.Format("using BR.Message.{0};", Group.Name));
            NewLine(3);

            // namespace definition
            OpenSection("namespace", PrjPrefix + "." + BuilderNamespace, false);
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
        public string ParamInListString(string strPrefix, Parameter[] parameter)
        {
            string strParams = "", strComma = ",";

            if (parameter == null)
                return strParams;

            foreach (Parameter param in parameter)
            {
                strParams += strComma;

                Type csType = SystemTypeMap.ToCSharpType(param.Type);
                bool bIsStruct = IsStruct(csType);

                if (IsStrType(param)) // string type
                {
                    strParams += string.Format(" {0}{1}", strPrefix, InParamName(param.Name));
                }
                else if (param.IsArray) // array
                {
                    strParams += string.Format(" {0}{1}", strPrefix, InParamName(param.Name));
                }
                else // generic type
                {
                    if (bIsStruct)
                    {
                        strParams += string.Format("ref {0}{1}",  strPrefix, InParamName(param.Name));
                    }
                    else
                    {
                        strParams += string.Format(" {0}{1}",  strPrefix, InParamName(param.Name));
                    }
                }
            }

            return strParams;
        }
        
        // Build parser class implementation
        void BuildMessagePolicyFunction(MessageBase baseMsg, string msgTypeName, Parameter[] parameters)
        {
            string strMessageName = string.Format("BR.Message.{0}.{1}{2}", Group.Name,baseMsg.Name, msgTypeName);

            OpenSection("public bool ", 
                string.Format("{0}{1}( {2} )",
                baseMsg.Name, msgTypeName, ParamInString("", parameters)), false);

            MatchIndent(); OutStream.WriteLine(string.Format("MessageHeader header = new MessageHeader({0}.MID);", strMessageName));
            MatchIndent(); OutStream.WriteLine("var binWriter = m_Connection.PrepareMessgeBuffer();");
            MatchIndent(); OutStream.WriteLine(string.Format("{0}.BuildMessageBuffer(binWriter, ref header{1} );", strMessageName, ParamInListString("", parameters)));

            if (baseMsg.Reliable)
            {
                MatchIndent(); OutStream.WriteLine("return m_Connection.PendingReliableSend(ref header, binWriter);");
            }
            else
            {
                MatchIndent(); OutStream.WriteLine("return m_Connection.PendingSend(ref header, binWriter);");
            }

            NewLine();

            CloseSection();
        }

        void BuildClassMember( string strClassName )
        {
            NewLine();
            MatchIndent(); OutStream.WriteLine(string.Format("public {0}( BR.Net.ConnectionMUDP connection )", strClassName));
            MatchIndent(1); OutStream.WriteLine(":base(connection)");
            MatchIndent(); OutStream.WriteLine("{");
            MatchIndent(); OutStream.WriteLine("}");
            NewLine();
        }

        void BuildMessagePolicyClassCSharp()
        {
            // Packer interface
            Parameter[] newparams;

            OpenSection("public class", "Policy"+Group.Name+" : BR.Net.Policy");

            BuildClassMember("Policy" + Group.Name);

            foreach (MessageBase baseMsg in Group.Items)
            {
                if (baseMsg is ProtocolsProtocolGroupCommand)
                {
                    MatchIndent(); OutStream.WriteLine("// Cmd: " + baseMsg.Desc);
                    ProtocolsProtocolGroupCommand msg = baseMsg as ProtocolsProtocolGroupCommand;

                    newparams = MakeParameters(MsgType.Cmd, msg.Cmd);
                    BuildMessagePolicyFunction(msg, "Cmd", newparams);
                    NewLine();
                }

                if (baseMsg is ProtocolsProtocolGroupC2SEvent)
                {
                    MatchIndent(); OutStream.WriteLine("// C2S: " + baseMsg.Desc);
                    ProtocolsProtocolGroupC2SEvent msg = baseMsg as ProtocolsProtocolGroupC2SEvent;

                    newparams = MakeParameters(MsgType.Evt, msg.Params);
                    BuildMessagePolicyFunction(msg, "C2SEvt", newparams); NewLine();
                    NewLine();
                }
            }

            CloseSection();



            OpenSection("public class", "SvrPolicy" + Group.Name + " : BR.Net.Policy");

            BuildClassMember("SvrPolicy" + Group.Name);

            foreach (MessageBase baseMsg in Group.Items)
            {
                if (baseMsg is ProtocolsProtocolGroupCommand)
                {
                    MatchIndent(); OutStream.WriteLine("// Cmd: " + baseMsg.Desc);
                    ProtocolsProtocolGroupCommand msg = baseMsg as ProtocolsProtocolGroupCommand;

                    newparams = MakeParameters(MsgType.Res, msg.Res);
                    BuildMessagePolicyFunction(msg, "Res", newparams); NewLine();
                    NewLine();
                }

                if (baseMsg is ProtocolsProtocolGroupS2CEvent)
                {
                    MatchIndent(); OutStream.WriteLine("// S2C: " + baseMsg.Desc);
                    ProtocolsProtocolGroupS2CEvent msg = baseMsg as ProtocolsProtocolGroupS2CEvent;

                    newparams = MakeParameters(MsgType.Evt, msg.Params);
                    BuildMessagePolicyFunction(msg, "S2CEvt", newparams); NewLine();
                    NewLine();
                }

            }

            CloseSection();

        }


        public override void Build()
        {
            if (!Group.GenCSharp)
                return;

            // Build Parser class
            OpenOutFile(OutputFileName());

            BuildPrefix();
            NewLine(2);


            BuildMessagePolicyClassCSharp();
            NewLine(2);

            NewLine(2);
            CloseOutFile();
        }

    }
}
