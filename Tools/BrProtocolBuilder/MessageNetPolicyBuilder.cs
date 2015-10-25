﻿////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves Corporation.
// 
// Author : Generated
//
// Description : Metwork policy builder
//
////////////////////////////////////////////////////////////////////////////////

using System;
using System.Collections.Generic;
using System.Text;
using ProtocolXml;

namespace ProtocolBuilder
{
    class MessageNetPolicyBuilder : Builder
    { 
        // constructor
        public MessageNetPolicyBuilder(string strBasePath)
            : base(strBasePath + "Lib/Protocol/Policy")
        {
        }

        public virtual string OutputHeaderName()
        {
            return Group.Name + "NetPolicy.h";
        }

        public virtual string OutputCPPName()
        {
            return Group.Name + "NetPolicy.cpp";
        }

        public virtual string MessageClassHeaderName { get { return Group.Name + "MsgClass.h"; } }

        public virtual string IBasePolicyHeaderName { get { return Group.Name + "IPolicy.h"; } }
        public virtual string IBaseSvrPolicyName { get { return "ISvrPolicy" + Group.Name; } }
        public virtual string SvrPolicyClassName { get { return "NetSvrPolicy" + Group.Name; } }
        public virtual string IBasePolicyName { get { return "IPolicy" + Group.Name; } }
        public virtual string PolicyClassName { get { return "NetPolicy" + Group.Name; } }


        protected virtual void BuildHPrefix()
        {
            OutStream.WriteLine("////////////////////////////////////////////////////////////////////////////////");
            OutStream.WriteLine("// ");
            OutStream.WriteLine("// CopyRight (c) 2013 The Braves");
            OutStream.WriteLine("// ");
            OutStream.WriteLine("// Author : Generated");
            OutStream.WriteLine("// ");
            OutStream.WriteLine(string.Format("// Description : {0} Message debug definitions", Group.Name));
            OutStream.WriteLine("// ");
            OutStream.WriteLine("////////////////////////////////////////////////////////////////////////////////");
            OutStream.WriteLine("");
            OutStream.WriteLine("#pragma once");
            OutStream.WriteLine("");
            OutStream.WriteLine("#include \"Common/Typedefs.h\"");
            OutStream.WriteLine("#include \"Common/PolicyID.h\"");

            OutStream.WriteLine("#include \"Net/Netdef.h\"");
            OutStream.WriteLine("#include \"Net/NetPolicy.h\"");
            OutStream.WriteLine(string.Format("#include \"Protocol/Policy/{0}\"", IBasePolicyHeaderName));
            NewLine(3);

            // namespace definition
            OpenSection("namespace", PrjPrefix);
            OpenSection("namespace", "Policy");
        }


        void BuildCPPPrefix()
        {
            OutStream.WriteLine("////////////////////////////////////////////////////////////////////////////////");
            OutStream.WriteLine("// ");
            OutStream.WriteLine("// CopyRight (c) 2013 The Braves");
            OutStream.WriteLine("// ");
            OutStream.WriteLine("// Author : Generated");
            OutStream.WriteLine("// ");
            OutStream.WriteLine(string.Format("// Description : {0} Message debug implementations", Group.Name));
            OutStream.WriteLine("// ");
            OutStream.WriteLine("////////////////////////////////////////////////////////////////////////////////");
            OutStream.WriteLine("");
            OutStream.WriteLine("");
            OutStream.WriteLine(string.Format("#include \"{0}\"", PreCompiledHeader));
            OutStream.WriteLine("#include \"Common/Typedefs.h\"");
            OutStream.WriteLine("#include \"Common/PolicyID.h\"");
            OutStream.WriteLine("#include \"Net/Netdef.h\"");
            OutStream.WriteLine("#include \"Common/Message.h\"");
            OutStream.WriteLine("#include \"Common/BrSvrTypes.h\"");
            OutStream.WriteLine("#include \"Common/BrGameTypes.h\"");
            OutStream.WriteLine(string.Format("#include \"Protocol/Policy/{0}\"", IBasePolicyHeaderName));
            OutStream.WriteLine(string.Format("#include \"Protocol/Policy/{0}\"", OutputHeaderName()));
            OutStream.WriteLine(string.Format("#include \"Protocol/Message/{0}\"", MessageClassHeaderName));
            OutStream.WriteLine("#include \"Protocol/ProtocolTrace.h\"");
            
            
            NewLine(3);

            // namespace definition
            OpenSection("namespace", PrjPrefix);
            OpenSection("namespace", "Policy");
        }

        protected string ParamString(Parameter[] parameter)
        {
            return ParamInString(parameter);
        }

        protected virtual void BuildServerPolicyH()
        {
            Parameter[] newparams;

            OpenSection("class", string.Format("{0} : public {1}", SvrPolicyClassName, IBaseSvrPolicyName));
            MatchIndent(); OutStream.WriteLine("public:");
            MatchIndent(); OutStream.WriteLine("// Constructor");
            MatchIndent(); OutStream.WriteLine(
                string.Format("{0} ( Net::IConnection* pConn ) : {1}(pConn)", SvrPolicyClassName, IBaseSvrPolicyName));
            MatchIndent(); OutStream.WriteLine("{}");
            NewLine();

            foreach (MessageBase baseMsg in Group.Items)
            {
                if (baseMsg is ProtocolsProtocolGroupCommand)
                {
                    MatchIndent(); OutStream.WriteLine("// Cmd: " + baseMsg.Desc);
                    ProtocolsProtocolGroupCommand msg = baseMsg as ProtocolsProtocolGroupCommand;

                    newparams = MakeParameters(MsgType.Res, msg.Res);
                    MatchIndent(); OutStream.WriteLine(
                        string.Format("HRESULT {0}Res( {1} );", msg.Name, ParamString(newparams)));
                }

                if (baseMsg is ProtocolsProtocolGroupS2CEvent)
                {
                    MatchIndent(); OutStream.WriteLine("// S2C: " + baseMsg.Desc);
                    ProtocolsProtocolGroupS2CEvent msg = baseMsg as ProtocolsProtocolGroupS2CEvent;

                    newparams = MakeParameters(MsgType.Evt, msg.Params);
                    MatchIndent(); OutStream.WriteLine(
                        string.Format("HRESULT {0}S2CEvt( {1} );", msg.Name, ParamString(newparams)));
                }
            }

            NewLine();

            CloseSection();
        }

        void BuildIClientPolicyH()
        {
            Parameter[] newparams;

            OpenSection("class", string.Format("{0} : public {1}", PolicyClassName, IBasePolicyName));
            MatchIndent(); OutStream.WriteLine("public:");
            MatchIndent(); OutStream.WriteLine("// Constructor");
            MatchIndent(); OutStream.WriteLine(
                string.Format("{0} ( Net::IConnection* pConn ) : {1}(pConn)",PolicyClassName, IBasePolicyName));
            MatchIndent(); OutStream.WriteLine("{}");
            NewLine();

            foreach (MessageBase baseMsg in Group.Items)
            {
                if (baseMsg is ProtocolsProtocolGroupCommand)
                {
                    MatchIndent(); OutStream.WriteLine("// Cmd: " + baseMsg.Desc);
                    ProtocolsProtocolGroupCommand msg = baseMsg as ProtocolsProtocolGroupCommand;

                    newparams = MakeParameters(MsgType.Cmd, msg.Cmd);
                    MatchIndent(); OutStream.WriteLine(
                        string.Format("HRESULT {0}Cmd( {1} );", msg.Name, ParamInString(newparams)));
                }

                if (baseMsg is ProtocolsProtocolGroupC2SEvent)
                {
                    MatchIndent(); OutStream.WriteLine("// C2S: " + baseMsg.Desc);
                    ProtocolsProtocolGroupC2SEvent msg = baseMsg as ProtocolsProtocolGroupC2SEvent;

                    newparams = MakeParameters(MsgType.Evt, msg.Params);
                    MatchIndent(); OutStream.WriteLine(
                        string.Format("HRESULT {0}C2SEvt( {1} );", msg.Name, ParamInString(newparams)));
                }
            }

            NewLine();

            CloseSection();
        }

        // Build parser class implementation
        void BuildNetPolicyClassParserImpl( string Name, string typeName, Parameter[] parameters)
        {
            if((typeName == "Cmd") || (typeName == "C2SEvt"))
                OpenSection("HRESULT", string.Format("{0}::{1}{2}( {3} )", PolicyClassName, Name, typeName, ParamInString(parameters)));
            else
                OpenSection("HRESULT", string.Format("{0}::{1}{2}( {3} )", SvrPolicyClassName, Name, typeName, ParamInString(parameters)));
            
            DefaultHRESULT(); NewLine();

            MatchIndent(); OutStream.WriteLine(" Message::MessageData * pMsg = nullptr;");
            NewLine();


            if (parameters != null && parameters.Length > 0)
            {
                MatchIndent(); OutStream.WriteLine(" protocolChk(Message::{0}::{1}{2}::BuildIMsg(pMsg, {3}));", Group.Name, Name, typeName, ParamArgument(parameters));
            }
            else
            {
                MatchIndent(); OutStream.WriteLine(" protocolChk(Message::{0}::{1}{2}::BuildIMsg(pMsg));", Group.Name, Name, typeName);
            }
            
            NewLine();

            MatchIndent(); OutStream.WriteLine(" protocolChkPtr(GetConnection());");
            NewLine();

            MatchIndent(); OutStream.WriteLine(" protocolChk(GetConnection()->Send( pMsg ));");
            NewLine();
            
            ProcEnd(); NewLine();
            ReturnHR(); NewLine();

            CloseSection();
        }

        void BuildSvrPolicyCPP()
        {
            Parameter[] newparams;

            foreach (MessageBase baseMsg in Group.Items)
            {
                if (baseMsg is ProtocolsProtocolGroupCommand)
                {
                    MatchIndent(); OutStream.WriteLine("// Cmd: " + baseMsg.Desc);
                    ProtocolsProtocolGroupCommand msg = baseMsg as ProtocolsProtocolGroupCommand;

                    newparams = MakeParameters(MsgType.Res, msg.Res);
                    BuildNetPolicyClassParserImpl(msg.Name, "Res", newparams);
                }

                if (baseMsg is ProtocolsProtocolGroupS2CEvent)
                {
                    MatchIndent(); OutStream.WriteLine("// S2C: " + baseMsg.Desc);
                    ProtocolsProtocolGroupS2CEvent msg = baseMsg as ProtocolsProtocolGroupS2CEvent;

                    newparams = MakeParameters(MsgType.Evt, msg.Params);
                    BuildNetPolicyClassParserImpl(msg.Name, "S2CEvt", newparams);
                }
            }

        }

        void BuildClientPolicyCPP()
        {
            Parameter[] newparams;

            foreach (MessageBase baseMsg in Group.Items)
            {
                if (baseMsg is ProtocolsProtocolGroupCommand)
                {
                    MatchIndent(); OutStream.WriteLine("// Cmd: " + baseMsg.Desc);
                    ProtocolsProtocolGroupCommand msg = baseMsg as ProtocolsProtocolGroupCommand;

                    newparams = MakeParameters(MsgType.Cmd, msg.Cmd);
                    BuildNetPolicyClassParserImpl(msg.Name, "Cmd", newparams);
                }

                if (baseMsg is ProtocolsProtocolGroupC2SEvent)
                {
                    MatchIndent(); OutStream.WriteLine("// C2S: " + baseMsg.Desc);
                    ProtocolsProtocolGroupC2SEvent msg = baseMsg as ProtocolsProtocolGroupC2SEvent;

                    newparams = MakeParameters(MsgType.Evt, msg.Params);
                    BuildNetPolicyClassParserImpl(msg.Name, "C2SEvt", newparams);
                }
            }
        }

        void BuildH()
        {
            // build IParser
            OpenOutFile(OutputHeaderName());

            BuildHPrefix();

            BuildIClientPolicyH();
            NewLine(2);

            BuildServerPolicyH();
            NewLine(2);

            CloseOutFile();
        }

        void BuildCPP()
        {
            OpenOutFile(OutputCPPName());

            BuildCPPPrefix();

            BuildClientPolicyCPP();
            NewLine(2);

            BuildSvrPolicyCPP();
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