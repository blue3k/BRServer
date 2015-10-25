﻿////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves Corporation.
// 
// Author : Generated
//
// Description : Message parser builder
//
////////////////////////////////////////////////////////////////////////////////

using System;
using System.Collections.Generic;
using System.Text;
using ProtocolXml;

namespace ProtocolBuilder
{
    class MessageBuilder : Builder
    {
        // Msg type string
        static string[] MsgTypeString = new string[] {
                    "MSGTYPE_NETCONTROL",
                    "MSGTYPE_COMMAND",
                    "MSGTYPE_RESULT",
                    "MSGTYPE_EVENT",
                };

        // Message guaranted level string
        static string[] MsgReliablityString = new string[]{
                "MSGTYPE_NONE",
                "MSGTYPE_RELIABLE",
            };

        // Message guaranted level string
        static string[] MsgMobilityString = new string[]{
                "MSGTYPE_NONE",
                "MSGTYPE_MOBILE",
            };


        // Message code generation serial
        int m_MessageCodeIndex;


        // constructor
        public MessageBuilder(string strBasePath)
            : base(strBasePath + "Lib/Protocol/Message")
        {
            GenParameterRouteHopCount = true;
        }

        public string BuilderNamespace
        {
            get { return "Message"; }
        }

        public string OutputClassHeaderName()
        {
            return Group.Name + "MsgClass.h";
        }

        public string OutputClassCPPName()
        {
            return Group.Name + "MsgClass.cpp";
        }

        void BuildHPrefix()
        {
            OutStream.WriteLine("////////////////////////////////////////////////////////////////////////////////");
            OutStream.WriteLine("// ");
            OutStream.WriteLine("// CopyRight (c) 2013 The Braves");
            OutStream.WriteLine("// ");
            OutStream.WriteLine("// Author : Generated");
            OutStream.WriteLine("// ");
            OutStream.WriteLine(string.Format( "// Description : {0} Message parser definitions", Group.Name) );
            OutStream.WriteLine("// ");
            OutStream.WriteLine("////////////////////////////////////////////////////////////////////////////////");
            OutStream.WriteLine("");
            OutStream.WriteLine("#pragma once");
            OutStream.WriteLine("");
            OutStream.WriteLine("#include \"Common/PolicyID.h\"");
            OutStream.WriteLine("#include \"Common/Message.h\"");
            OutStream.WriteLine("#include \"Common/BrSvrTypes.h\"");
            OutStream.WriteLine("#include \"Common/BrGameTypes.h\"");
            NewLine(3);

            // namespace definition
            OpenSection("namespace", PrjPrefix );
            OpenSection("namespace", BuilderNamespace);
            OpenSection("namespace", Group.Name);
        }

        void BuildCPPPrefix( string HeaderName )
        {
            OutStream.WriteLine("////////////////////////////////////////////////////////////////////////////////");
            OutStream.WriteLine("// ");
            OutStream.WriteLine("// CopyRight (c) 2013 The Braves");
            OutStream.WriteLine("// ");
            OutStream.WriteLine("// Author : Generated");
            OutStream.WriteLine("// ");
            OutStream.WriteLine(string.Format("// Description : {0} Message parser implementations", Group.Name));
            OutStream.WriteLine("// ");
            OutStream.WriteLine("////////////////////////////////////////////////////////////////////////////////");
            OutStream.WriteLine("");
            OutStream.WriteLine("");
            OutStream.WriteLine( string.Format("#include \"{0}\"", PreCompiledHeader ) );
            OutStream.WriteLine("#include \"Common/PolicyID.h\"");
            OutStream.WriteLine("#include \"Common/ToStringSvr.h\"");
            OutStream.WriteLine("#include \"Common/ToStringGame.h\"");
            OutStream.WriteLine("#include \"Common/ArrayUtil.h\"");
            OutStream.WriteLine("#include \"Protocol/ProtocolMessageCPPInc.h\"");
            OutStream.WriteLine("#include \"Protocol/ProtocolTrace.h\"");
            OutStream.WriteLine("#include \"Protocol/ProtocolHelper.h\"");
            OutStream.WriteLine(string.Format("#include \"Protocol/Message/{0}\"", HeaderName));
            OutStream.WriteLine("#include \"Protocol/ProtocolTrace.h\"");
            if (Group.IsServer)
                OutStream.WriteLine("#include \"Common/SvrPolicyID.h\"");

            NewLine(3);

            // namespace definition
            OpenSection("namespace", PrjPrefix);
            OpenSection("namespace", BuilderNamespace);
            OpenSection("namespace", Group.Name);
        }


        void BuildMessageTrace(string Name, string typeName, string traceChannel, Parameter[] parameters)
        {
            string strClassName = MsgClassName(Name, typeName);
            OpenSection("VOID", strClassName + "::TraceOut(const char* Prefix, MessageData* pMsg)");

            string strTrace = string.Format("Trace::TRC_{0}, \"%0%:{1}{2}:%1%:%2% ", traceChannel, Name, typeName);
            string strTraceMember = "Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32";
            int ParamCount = 3;

            MatchIndent(); OutStream.WriteLine("Prefix;");

            if (parameters != null)
            {
                foreach (Parameter param in parameters)
                {
                    switch (param.Type)
                    {
                        case ParameterType.String:
                            {
                                strTrace += string.Format(", {0}:%{1}%", param.Name, ParamCount++);
                                strTraceMember += string.Format(", m_{0}", param.Name);
                            } break;
                        default:
                            if (param.IsArray)
                            {
                                strTrace += string.Format(", {0}:%{1}%", param.Name, ParamCount++);
                                strTraceMember += string.Format(", ArgArray<{0}>(m_{1})", param.Type, param.Name);
                            }
                            else
                            {
                                if (param.Type == ParameterType.HRESULT)
                                {
                                    strTrace += string.Format(", {0}:%{1}%", param.Name, ParamCount++);
                                    strTraceMember += string.Format(", ArgHex32(m_{0})", param.Name);
                                }
                                else
                                {
                                    strTrace += string.Format(", {0}:%{1}%", param.Name, ParamCount++);
                                    strTraceMember += string.Format(", m_{0}", param.Name);
                                }
                            }
                            break;
                    }
                }
            }

            MatchIndent(); OutStream.WriteLine(string.Format("protocolTrace({0},", strTrace + "\""));
            MatchIndent(7); OutStream.WriteLine(string.Format("\t{0}); ", strTraceMember));

            CloseSection();

            NewLine();
        }

        // build pointer initialize block
        void BuildParserClassInit( string strDelimiter, Parameter[] parameters)
        {
            if (parameters != null)
            {
                foreach (Parameter param in parameters)
                {
                    switch (param.Type)
                    {
                        case ParameterType.String:
                            MatchIndent(); OutStream.WriteLine(
                                string.Format("{0}m_{1}(nullptr)", strDelimiter, param.Name));
                            strDelimiter = ",";
                            break;
                        default:
                            //if (param.IsArray)
                            //{
                            //    MatchIndent(); OutStream.WriteLine(
                            //        string.Format("{0}m_{1}(0)", strDelimiter, ArrayLenName(param.Name)));
                            //    strDelimiter = ",";
                            //    MatchIndent(); OutStream.WriteLine(
                            //        string.Format("{0}m_{1}(NULL)", strDelimiter, param.Name));
                                
                            //}
                            break;
                    }
                }
            }
        }

        // Builder parameter string
        string BuilderParamString(Parameter[] parameter)
        {
            string param = ParamInString(parameter);
            if (param.Length > 0)
                return "OUT MessageData* &pMsg, " + param;
            else
                return "OUT MessageData* &pMsg";
        }

        // Make message ID macro string 
        string MakeMsgIDStr(MsgType type, bool reliablity, bool mobility, string MsgName)
        {
            string strRes = string.Format("MessageID({0}, {1}, {2}, POLICY_{3}, {4})",
                MsgTypeString[(int)type],
                MsgReliablityString[reliablity ? 1 : 0],
                MsgMobilityString[mobility ? 1 : 0],
                Group.Name.ToUpper(),
                m_MessageCodeIndex);
            return strRes;
        }

        // build parser class
        void BuildParserClass(ProtocolXml.MessageBase msg, MsgType msgType, string typeName, Parameter[] parameters)
        {
            string Name = msg.Name;
            string strClassName = MsgClassName(Name, typeName);
            OpenSection("class", strClassName + " : public MessageBase");
            MatchIndent(-1); OutStream.WriteLine("public:");

            MatchIndent(); OutStream.WriteLine(string.Format("static const MessageID MID;"));

            //if (Group.GenParameterRouteContext)
            //{
            //    MatchIndent(); OutStream.WriteLine(string.Format("struct HasRouteContext {{ int Dummy; }};"));
            //}

            MatchIndent(-1); OutStream.WriteLine("private:");
            // member section
            if (parameters != null)
            {
                foreach (Parameter param in parameters)
                {
                    switch (param.Type)
                    {
                        case ParameterType.String:
                            MatchIndent(); OutStream.WriteLine(
                                string.Format("const {0} m_{1};", StrTypeString(param), param.Name));
                            break;
                        default:
                            if (param.IsArray)
                            {
                                MatchIndent(); OutStream.WriteLine(
                                    string.Format("LinkedArray<{0}> m_{1};", param.Type.ToString(), param.Name));
                                //MatchIndent(); OutStream.WriteLine(
                                //    string.Format("{0} m_{1};", ArrayLenType, ArrayLenName(param.Name)));
                                //MatchIndent(); OutStream.WriteLine(
                                //    string.Format("{0} *m_{1};", param.Type.ToString(), param.Name));
                            }
                            else
                            {
                                MatchIndent(); OutStream.WriteLine(
                                    string.Format("{0} m_{1};", param.Type.ToString(), param.Name));
                            }
                            break;
                    }

                }
                
            }

            // Constructor
            MatchIndent(-1); OutStream.WriteLine("public:");
            MatchIndent(); OutStream.WriteLine(strClassName + "()");
            BuildParserClassInit(":", parameters);
            MatchIndent(1); OutStream.WriteLine("{}");
            NewLine();

            MatchIndent(); OutStream.WriteLine(strClassName + "( MessageData* &pMsg )");
            MatchIndent(1); OutStream.WriteLine(":MessageBase(pMsg)");
            BuildParserClassInit(",", parameters);
            MatchIndent(1); OutStream.WriteLine("{}");
            NewLine();

            // Usage
            MatchIndent(1); OutStream.WriteLine("MessageUsage GetMessageUsage() {{ return MessageUsage_{0}; }}",msg.Usage.ToString());
            NewLine();

            // Get functions
            if (parameters != null)
            {
                foreach (Parameter param in parameters)
                {
                    switch (param.Type)
                    {
                    case ParameterType.String:
                        MatchIndent(); OutStream.WriteLine(
                            string.Format("const {0} Get{1}() const\t{{ return m_{1}; }};", StrTypeString(param), param.Name));
                        break;
                    default:
                        if (param.IsArray)
                        {
                            MatchIndent(); OutStream.WriteLine(
                                string.Format("const Array<{0}>& Get{1}() const\t{{ return m_{1}; }};", param.Type.ToString(), param.Name));
                            //MatchIndent(); OutStream.WriteLine(
                            //    string.Format("const {0}& Get{1}() const\t{{ return m_{1}; }};", ArrayLenType, ArrayLenName(param.Name)));
                            //MatchIndent(); OutStream.WriteLine(
                            //    string.Format("const {0}* Get{1}() const\t{{ return m_{1}; }};", param.Type.ToString(), param.Name));
                        }
                        else
                        {
                            MatchIndent(); OutStream.WriteLine(
                                string.Format("const {0}& Get{1}() const\t{{ return m_{1}; }};", param.Type.ToString(), param.Name));
                        }
                        break;
                    }
                }
            }

            NewLine();
            // message trace function
            MatchIndent(); OutStream.WriteLine("void TraceOut(const char* Prefix, MessageData* pMsg);");
            NewLine();

            // Parse function
            MatchIndent(); OutStream.WriteLine("virtual HRESULT ParseIMsg( IN MessageData* pIMsg );");
            NewLine();

            // Build function
            MatchIndent(); OutStream.WriteLine("static HRESULT BuildIMsg( {0} );", BuilderParamString(parameters));
            NewLine();

            // Override route context function
            if (Group.GenParameterRouteContext)
            {
                MatchIndent(); OutStream.WriteLine("HRESULT OverrideRouteContextDestination( EntityUID to );");
                NewLine();
            }

            // Override route hop function
            if (Group.GenParameterRouteHopCount)
            {
                MatchIndent(); OutStream.WriteLine("HRESULT OverrideRouteInformation( EntityUID to, UINT hopCount );");
                NewLine();
            }

            CloseSection();
        }

        // build parser class header
        void BuildMessageParserClassH()
        {
            Parameter[] newparams;

            foreach (MessageBase baseMsg in Group.Items)
            {
                if (baseMsg is ProtocolsProtocolGroupCommand)
                {
                    MatchIndent(); OutStream.WriteLine("// Cmd: " + baseMsg.Desc);
                    ProtocolsProtocolGroupCommand msg = baseMsg as ProtocolsProtocolGroupCommand;

                    newparams = MakeParameters(MsgType.Cmd, msg.Cmd);
                    BuildParserClass(msg, MsgType.Cmd, "Cmd", newparams);
                    NewLine();

                    newparams = MakeParameters(MsgType.Res, msg.Res);
                    BuildParserClass(msg, MsgType.Res, "Res", newparams);
                    NewLine();
                }

                if (baseMsg is ProtocolsProtocolGroupC2SEvent)
                {
                    MatchIndent(); OutStream.WriteLine("// C2S: " + baseMsg.Desc);
                    ProtocolsProtocolGroupC2SEvent msg = baseMsg as ProtocolsProtocolGroupC2SEvent;

                    newparams = MakeParameters(MsgType.Evt, msg.Params);
                    BuildParserClass(msg, MsgType.Evt, "C2SEvt", newparams);
                    NewLine();
                }

                if (baseMsg is ProtocolsProtocolGroupS2CEvent)
                {
                    MatchIndent(); OutStream.WriteLine("// S2C: " + baseMsg.Desc);
                    ProtocolsProtocolGroupS2CEvent msg = baseMsg as ProtocolsProtocolGroupS2CEvent;

                    newparams = MakeParameters(MsgType.Evt, msg.Params);
                    BuildParserClass(msg, MsgType.Evt, "S2CEvt", newparams);
                    NewLine();
                }
            }
        }

        void BuildMessageIDImpl(MessageBase msg, MsgType msgType, string typeName)
        {
            MatchIndent(); OutStream.WriteLine("const MessageID {0}::MID = {1};", MsgClassName(msg.Name, typeName), MakeMsgIDStr(msgType, msg.Reliable, Group.IsMobile, msg.Name));
        }

        void BuildParseFunctionImpl(Parameter[] parameters)
        {
            MatchIndent(); OutStream.WriteLine("INT iMsgSize;");
            MatchIndent(); OutStream.WriteLine("BYTE* pCur;");
            NewLine();
            MatchIndent(); OutStream.WriteLine("protocolChkPtr(pIMsg);");
            NewLine();

            // array len types
            if (Group.IsMobile)
            {
                MatchIndent(); OutStream.WriteLine("iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);");
            }
            else
            {
                MatchIndent(); OutStream.WriteLine("iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);");
            }
            MatchIndent(); OutStream.WriteLine("pCur = pIMsg->GetMessageData();");
            NewLine();

            if (parameters == null)
            {
                // nothing to process
                MatchIndent(); OutStream.WriteLine("goto Proc_End;");
            }
            else
            {

                foreach (Parameter param in parameters)
                {
                    switch (param.Type)
                    {
                        case ParameterType.String:
                            MatchIndent(); OutStream.WriteLine("{0} {1} = 0;", ArrayLenType, ArrayLenName(param.Name));
                            MatchIndent(); OutStream.WriteLine("protocolChk( Protocol::StreamParamCopy( &{1}, pCur, iMsgSize, sizeof({0}) ) );", ArrayLenType, ArrayLenName(param.Name));
                            MatchIndent(); OutStream.WriteLine("protocolChk( Protocol::StreamParamLnk( m_{0}, pCur, iMsgSize, sizeof(char)*{1} ) );", param.Name, ArrayLenName(param.Name));
                            break;
                        default:
                            if (param.IsArray)
                            {
                                MatchIndent(); OutStream.WriteLine("{0} numberof{1} = 0; {2}* p{1} = nullptr;", ArrayLenType, param.Name, param.Type.ToString());
                                MatchIndent(); OutStream.WriteLine("protocolChk( Protocol::StreamParamCopy( &numberof{1}, pCur, iMsgSize, sizeof({0}) ) );", ArrayLenType, param.Name);
                                MatchIndent(); OutStream.WriteLine("protocolChk( Protocol::StreamParamLnk( p{0}, pCur, iMsgSize, sizeof({1})*numberof{0} ) );", param.Name, param.Type.ToString());
                                MatchIndent(); OutStream.WriteLine("m_{0}.SetLinkedBuffer(numberof{0}, numberof{0}, p{0});", param.Name);
                                //MatchIndent(); OutStream.WriteLine("protocolChk( Protocol::StreamParamCopy( &m_{1}, pCur, iMsgSize, sizeof({0}) ) );", ArrayLenType, ArrayLenName(param.Name));
                                //MatchIndent(); OutStream.WriteLine("protocolChk( Protocol::StreamParamLnk( m_{0}, pCur, iMsgSize, sizeof({1})*m_{2} ) );", param.Name, param.Type.ToString(), ArrayLenName(param.Name));
                            }
                            else
                            {
                                MatchIndent(); OutStream.WriteLine("protocolChk( Protocol::StreamParamCopy( &m_{0}, pCur, iMsgSize, sizeof({1}) ) );", param.Name, param.Type);
                            }
                            break;
                    }
                }
            }
        }

        // Build parser class implementation
        void BuildParserImpl(string Name, string typeName, Parameter[] parameters)
        {
            string strClassName = MsgClassName(Name, typeName);
            OpenSection("HRESULT", strClassName + "::ParseIMsg( MessageData* pIMsg )");

            DefaultHRESULT(); NewLine();

            BuildParseFunctionImpl(parameters);

            NewLine(2);

            ProcEnd(); NewLine();
            ReturnHR(); NewLine();

            CloseSection();
        }


        // Build parser class implementation
        void BuildOverrideRouteContextImpl(string Name, string typeName, Parameter[] parameters)
        {
            if (!Group.GenParameterRouteContext)
                return;

            string strClassName = MsgClassName(Name, typeName);
            OpenSection("HRESULT", strClassName + "::OverrideRouteContextDestination( EntityUID to )");

            DefaultHRESULT(); NewLine();

            MatchIndent(); OutStream.WriteLine("INT iMsgSize;");
            MatchIndent(); OutStream.WriteLine("BYTE* pCur;");
            MatchIndent(); OutStream.WriteLine("MessageData* pIMsg = GetMessage();");
            MatchIndent(); OutStream.WriteLine("RouteContext routeContext;");
            NewLine();
            MatchIndent(); OutStream.WriteLine("protocolChkPtr(pIMsg);");
            NewLine();

            // array len types
            if (Group.IsMobile)
            {
                MatchIndent(); OutStream.WriteLine("iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);");
            }
            else
            {
                MatchIndent(); OutStream.WriteLine("iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);");
            }

            MatchIndent(); OutStream.WriteLine("pCur = pIMsg->GetMessageData();");
            NewLine();

            if (parameters == null)
            {
                // nothing to process
                MatchIndent(); OutStream.WriteLine("goto Proc_End;");
            }
            else
            {
                // Just follow the same process to parsing
                foreach (Parameter param in parameters)
                {
                    switch (param.Type)
                    {
                        case ParameterType.String:
                            MatchIndent(); OutStream.WriteLine("{0} {1} = 0;", ArrayLenType, ArrayLenName(param.Name));
                            MatchIndent(); OutStream.WriteLine("protocolChk( Protocol::StreamParamCopy( &{1}, pCur, iMsgSize, sizeof({0}) ) );", ArrayLenType, ArrayLenName(param.Name));
                            MatchIndent(); OutStream.WriteLine("pCur += sizeof(char)*{1}; iMsgSize -= sizeof(char)*{1};", ArrayLenName(param.Name));
                            break;
                        case ParameterType.RouteContext:
                            break;
                        default:
                            if (param.IsArray)
                            {
                                MatchIndent(); OutStream.WriteLine("pCur += sizeof({0}); iMsgSize -= sizeof({0});", ArrayLenType);
                                MatchIndent(); OutStream.WriteLine("pCur += sizeof({0})*m_{1}.GetSize(); iMsgSize -= (INT)(sizeof({0})*m_{1}.GetSize());", param.Type.ToString(), param.Name);
                                //MatchIndent(); OutStream.WriteLine("pCur += sizeof({0})*m_{1}; iMsgSize -= sizeof({0})*m_{1};", param.Type.ToString(), ArrayLenName(param.Name));
                            }
                            else
                            {
                                MatchIndent(); OutStream.WriteLine("pCur += sizeof({0}); iMsgSize -= sizeof({0});", param.Type);
                            }
                            break;
                    }

                    // All other process is same
                    if (param.Type == ParameterType.RouteContext)
                    {
                        MatchIndent(); OutStream.WriteLine("Assert( iMsgSize >= sizeof(RouteContext) );");
                        MatchIndent(); OutStream.WriteLine("memcpy( &routeContext, pCur, sizeof(RouteContext) );");
                        MatchIndent(); OutStream.WriteLine("routeContext.To = to;");
                        MatchIndent(); OutStream.WriteLine("memcpy( pCur, &routeContext, sizeof(RouteContext) );");
                        break;
                    }
                }
            }

            NewLine(2);

            ProcEnd(); NewLine();
            ReturnHR(); NewLine();

            CloseSection();
        }


        // Build override route hop count class implementation
        void BuildOverrideRouteHopCountImpl(string Name, string typeName, Parameter[] parameters)
        {
            if (!Group.GenParameterRouteHopCount)
                return;

            string strClassName = MsgClassName(Name, typeName);
            OpenSection("HRESULT", strClassName + "::OverrideRouteInformation( EntityUID to, UINT hopCount )");

            DefaultHRESULT(); NewLine();

            MatchIndent(); OutStream.WriteLine("INT iMsgSize;");
            MatchIndent(); OutStream.WriteLine("BYTE* pCur;");
            MatchIndent(); OutStream.WriteLine("MessageData* pIMsg = GetMessage();");
            MatchIndent(); OutStream.WriteLine("RouteContext routeContext;");
            NewLine();
            MatchIndent(); OutStream.WriteLine("protocolChkPtr(pIMsg);");
            NewLine();

            // array len types
            if (Group.IsMobile)
            {
                MatchIndent(); OutStream.WriteLine("iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);");
            }
            else
            {
                MatchIndent(); OutStream.WriteLine("iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);");
            }

            MatchIndent(); OutStream.WriteLine("pCur = pIMsg->GetMessageData();");
            NewLine();

            if (parameters == null)
            {
                // nothing to process
                MatchIndent(); OutStream.WriteLine("goto Proc_End;");
            }
            else
            {
                // Just follow the same process to parsing
                foreach (Parameter param in parameters)
                {
                    switch (param.Type)
                    {
                        case ParameterType.String:
                            MatchIndent(); OutStream.WriteLine("{0} {1} = 0;", ArrayLenType, ArrayLenName(param.Name));
                            MatchIndent(); OutStream.WriteLine("protocolChk( Protocol::StreamParamCopy( &{1}, pCur, iMsgSize, sizeof({0}) ) );", ArrayLenType, ArrayLenName(param.Name));
                            MatchIndent(); OutStream.WriteLine("pCur += sizeof(char)*{1}; iMsgSize -= sizeof(char)*{1};", ArrayLenName(param.Name));
                            break;
                        default:
                            if (param.Type == ParameterType.RouteContext && param.Name == ParamRouteContext.Name)
                            {
                                MatchIndent(); OutStream.WriteLine("Assert( iMsgSize >= sizeof(RouteContext) );");
                                MatchIndent(); OutStream.WriteLine("memcpy( &routeContext, pCur, sizeof(RouteContext) );");
                                MatchIndent(); OutStream.WriteLine("routeContext.To = to;");
                                MatchIndent(); OutStream.WriteLine("memcpy( pCur, &routeContext, sizeof(RouteContext) );");
                            }
                            else if (param.Name == ParamRouteHopCount.Name)
                            {
                                MatchIndent(); OutStream.WriteLine("Assert( iMsgSize >= sizeof({0}) );", ParamRouteHopCount.Type);
                                MatchIndent(); OutStream.WriteLine("*({0}*)pCur = hopCount;", ParamRouteHopCount.Type);
                            }

                            if (param.IsArray)
                            {
                                MatchIndent(); OutStream.WriteLine("pCur += sizeof({0}); iMsgSize -= sizeof({0});", ArrayLenType);
                                MatchIndent(); OutStream.WriteLine("pCur += sizeof({0})*m_{1}.GetSize(); iMsgSize -= (INT)(sizeof({0})*m_{1}.GetSize());", param.Type.ToString(), param.Name);
                                //MatchIndent(); OutStream.WriteLine("pCur += sizeof({0})*m_{1}; iMsgSize -= sizeof({0})*m_{1};", param.Type.ToString(), ArrayLenName(param.Name));
                            }
                            else
                            {
                                MatchIndent(); OutStream.WriteLine("pCur += sizeof({0}); iMsgSize -= sizeof({0});", param.Type);
                            }
                            break;
                    }

                    // All other process is same
                    if (param.Name == ParamRouteHopCount.Name)
                    {
                        break;
                    }
                }
            }

            NewLine(2);

            ProcEnd(); NewLine();
            ReturnHR(); NewLine();

            CloseSection();
        }

        // Builder params size
        public void BuildBuilderMessageSize(Parameter[] parameters, string strSizeVarName)
        {
            // size calculation
            string strSize = "";
            if (parameters != null)
            {
                foreach (Parameter param in parameters)
                {
                    string strLenName;
                    switch (param.Type)
                    {
                        case ParameterType.String:
                            strLenName = string.Format("__ui{0}Length", InParamName(param.Name));
                            MatchIndent(); OutStream.WriteLine(string.Format("{2} {0} = {1} ? ({2})(strlen({1})+1) : 1;", strLenName, InParamName(param.Name), ArrayLenType));
                            strSize += string.Format(" + sizeof(UINT16) + {0}", strLenName);
                            break;
                    }
                }
            }

            MatchIndent();
            string strMessageHeader = Group.IsMobile ? "MobileMessageHeader" : "MessageHeader";

            if (strSize.Length > 0)
                OutStream.Write(string.Format("UINT {0} = (UINT)(sizeof({1}) + {2} ", strSizeVarName, strMessageHeader, strSize));
            else
                OutStream.Write(string.Format("UINT {0} = (UINT)(sizeof({1}) ", strSizeVarName, strMessageHeader));

            if (parameters != null)
            {
                foreach (Parameter param in parameters)
                {
                    switch (param.Type)
                    {
                        case ParameterType.String:
                            continue;
                    }

                    NewLine();
                    MatchIndent(1);
                    if (param.IsArray)
                    {
                        OutStream.Write("+ sizeof({0})*{1}.GetSize() + sizeof({2})", param.Type.ToString(), InParamName(param.Name), ArrayLenType);
                    }
                    else
                    {
                        OutStream.Write("+ sizeof({0})", param.Type.ToString());
                    }
                }
                OutStream.WriteLine(");");
            }
            else
                OutStream.WriteLine(");");
        }

        void BuildParamCpy(Parameter[] parameters)
        {
            if (parameters == null)
                return;

            foreach (Parameter param in parameters)
            {
                string strLenName;
                if (IsStrType(param)) // string type
                {
                    strLenName = string.Format("__ui{0}Length", InParamName(param.Name));
                    MatchIndent(); OutStream.WriteLine(string.Format("Protocol::PackParamCopy( pMsgData, &{0}, sizeof(UINT16) );", strLenName));
                    MatchIndent(); OutStream.WriteLine(string.Format("Protocol::PackParamCopy( pMsgData, {0} ? {0} : \"\", {1} );", InParamName(param.Name), strLenName));
                }
                else if (param.IsArray) // array
                {
                    MatchIndent(); OutStream.WriteLine(string.Format("{1} numberOf{0} = ({1}){0}.GetSize(); ", InParamName(param.Name), ArrayLenType));
                    MatchIndent(); OutStream.WriteLine(string.Format("Protocol::PackParamCopy( pMsgData, &numberOf{0}, sizeof({1})); ", InParamName(param.Name), ArrayLenType));
                    MatchIndent(); OutStream.WriteLine(string.Format("Protocol::PackParamCopy( pMsgData, {0}.data(), (INT)(sizeof({1})*{0}.GetSize())); ", InParamName(param.Name), param.Type.ToString()));
                    //MatchIndent(); OutStream.Write(string.Format("Protocol::PackParamCopy( pMsgData, &{0}, sizeof({0})); ", ArrayLenName(param.Name))); NewLine();
                    //MatchIndent(); OutStream.Write(string.Format("Protocol::PackParamCopy( pMsgData, {0}, sizeof({1})*{2}); ", InParamName(param.Name), param.Type.ToString(), ArrayLenName(param.Name))); NewLine();
                }
                else // generic type
                {
                    MatchIndent(); OutStream.Write(string.Format("Protocol::PackParamCopy( pMsgData, &{0}, sizeof({1}));", InParamName(param.Name), param.Type.ToString())); NewLine();
                }
            }
        }


        // Build parser class implementation
        void BuildBuilderImpl(string Name, string typeName, Parameter[] parameters)
        {
            string strClassName = MsgClassName(Name, typeName);
            OpenSection("HRESULT", strClassName + string.Format("::BuildIMsg( {0} )", BuilderParamString(parameters)));

            DefaultHRESULT(); NewLine();

            MatchIndent(); OutStream.WriteLine("BYTE *pMsgData = nullptr;");
            NewLine();

            BuildBuilderMessageSize(parameters, "__uiMessageSize");
            NewLine();

            MatchIndent(); OutStream.WriteLine("MessageData *pNewMsg = NULL;");
            NewLine();

            MatchIndent(); OutStream.WriteLine(
                string.Format("protocolMem( pNewMsg = MessageData::NewMessage( {0}::{1}{2}::MID, __uiMessageSize ) );", Group.Name, Name, typeName));
            NewLine();

            MatchIndent(); OutStream.WriteLine("pMsgData = pNewMsg->GetMessageData();");
            NewLine();

            BuildParamCpy(parameters);
            NewLine();

            MatchIndent(); OutStream.WriteLine(
                string.Format("pMsg = pNewMsg;"));
            NewLine(2);

            ProcEnd(); NewLine();
            ReturnHR(); NewLine();


            CloseSection();
        }

        void BuildMessageParserClassCPP()
        {
            // Packer interface
            Parameter[] newparams;
            m_MessageCodeIndex = 0;
            foreach (MessageBase baseMsg in Group.Items)
            {
                if (baseMsg is ProtocolsProtocolGroupCommand)
                {
                    MatchIndent(); OutStream.WriteLine("// Cmd: " + baseMsg.Desc);
                    ProtocolsProtocolGroupCommand msg = baseMsg as ProtocolsProtocolGroupCommand;

                    BuildMessageIDImpl( msg, MsgType.Cmd, "Cmd" );
                    newparams = MakeParameters(MsgType.Cmd, msg.Cmd);
                    BuildParserImpl(msg.Name, "Cmd", newparams); NewLine();
                    BuildBuilderImpl(msg.Name, "Cmd", newparams); NewLine();
                    BuildOverrideRouteContextImpl(msg.Name, "Cmd", newparams); NewLine();
                    BuildOverrideRouteHopCountImpl(msg.Name, "Cmd", newparams); NewLine();
                    BuildMessageTrace(msg.Name, "Cmd", msg.Trace.ToString(), newparams);

                    BuildMessageIDImpl( msg, MsgType.Res, "Res" );
                    newparams = MakeParameters(MsgType.Res, msg.Res);
                    BuildParserImpl(msg.Name, "Res", newparams); NewLine();
                    BuildBuilderImpl(msg.Name, "Res", newparams); NewLine();
                    BuildOverrideRouteContextImpl(msg.Name, "Res", newparams); NewLine();
                    BuildOverrideRouteHopCountImpl(msg.Name, "Res", newparams); NewLine();
                    BuildMessageTrace(msg.Name, "Res", msg.Trace.ToString(), newparams);
                }

                if (baseMsg is ProtocolsProtocolGroupC2SEvent)
                {
                    MatchIndent(); OutStream.WriteLine("// C2S: " + baseMsg.Desc);
                    ProtocolsProtocolGroupC2SEvent msg = baseMsg as ProtocolsProtocolGroupC2SEvent;

                    BuildMessageIDImpl( msg, MsgType.Evt, "C2SEvt" );
                    newparams = MakeParameters(MsgType.Evt, msg.Params);
                    BuildParserImpl(msg.Name, "C2SEvt", newparams); NewLine();
                    BuildBuilderImpl(msg.Name, "C2SEvt", newparams); NewLine();
                    BuildOverrideRouteContextImpl(msg.Name, "C2SEvt", newparams); NewLine();
                    BuildOverrideRouteHopCountImpl(msg.Name, "C2SEvt", newparams); NewLine();
                    BuildMessageTrace(msg.Name, "C2SEvt", msg.Trace.ToString(), newparams);
                }

                if (baseMsg is ProtocolsProtocolGroupS2CEvent)
                {
                    MatchIndent(); OutStream.WriteLine("// S2C: " + baseMsg.Desc);
                    ProtocolsProtocolGroupS2CEvent msg = baseMsg as ProtocolsProtocolGroupS2CEvent;

                    BuildMessageIDImpl(msg, MsgType.Evt, "S2CEvt");
                    newparams = MakeParameters(MsgType.Evt, msg.Params);
                    BuildParserImpl(msg.Name, "S2CEvt", newparams); NewLine();
                    BuildBuilderImpl(msg.Name, "S2CEvt", newparams); NewLine();
                    BuildOverrideRouteContextImpl(msg.Name, "S2CEvt", newparams); NewLine();
                    BuildOverrideRouteHopCountImpl(msg.Name, "S2CEvt", newparams); NewLine();
                    BuildMessageTrace(msg.Name, "S2CEvt", msg.Trace.ToString(), newparams);
                }
                m_MessageCodeIndex++;
            }
        }


        void BuildH()
        {
            // Build Parser class
            OpenOutFile(OutputClassHeaderName());

            BuildHPrefix();

            BuildMessageParserClassH();
            NewLine(2);

            NewLine(2);
            CloseOutFile();
        }

        void BuildCPP()
        {
            // write parser class cpp
            OpenOutFile(OutputClassCPPName());

            BuildCPPPrefix(OutputClassHeaderName());

            BuildMessageParserClassCPP();
            NewLine(2);


            CloseOutFile();
        }

        public override void Build()
        {
            BuildH();
            BuildCPP();
        }

    }
}