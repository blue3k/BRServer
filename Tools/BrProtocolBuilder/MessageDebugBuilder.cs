////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves Corporation.
// 
// Author : Generated
//
// Description : Debug builder
//
////////////////////////////////////////////////////////////////////////////////

using System;
using System.Collections.Generic;
using System.Text;
using ProtocolXml;

namespace ProtocolBuilder
{
    class MessageDebugBuilder : Builder
    {

        // constructor
        public MessageDebugBuilder(string strBasePath)
            : base(strBasePath+"Lib/Protocol/Message")
        {
        }

        public string BuilderNamespace
        {
            get { return "Debug"; }
        }

        public string OutputHeaderName()
        {
            return Group.Name + "MsgDebug.h";
        }
        public string OutputCPPName()
        {
            return Group.Name + "MsgDebug.cpp";
        }

        void BuildHPrefix()
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
            OutStream.WriteLine("#include \"Net/NetDef.h\"");
            OutStream.WriteLine("#include \"Protocol/ProtocolMessageCPPInc.h\"");
            OutStream.WriteLine(string.Format("#include \"Protocol/Message/{0}MsgClass.h\"", Group.Name));
            NewLine(3);

            // namespace definition
            OpenSection("namespace", PrjPrefix);
            OpenSection("namespace", "Message");
            OpenSection("namespace", BuilderNamespace);
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
            OutStream.WriteLine("#include \"Net/NetDef.h\"");
            OutStream.WriteLine("#include \"Protocol/ProtocolTrace.h\"");
            OutStream.WriteLine(string.Format("#include \"Protocol/Message/{0}\"", OutputHeaderName()));
            OutStream.WriteLine(string.Format("#include \"Protocol/Message/{0}MsgClass.h\"", Group.Name));
            NewLine(3);

            // namespace definition
            OpenSection("namespace", PrjPrefix);
            OpenSection("namespace", "Message");
            OpenSection("namespace", BuilderNamespace);
        }

        void BuildIMessageDebugH()
        {
            NewLine();
            MatchIndent(); OutStream.WriteLine(string.Format("///////////////////////////////////////////////////////////////"));
            MatchIndent(); OutStream.WriteLine(string.Format("// {0} Debug output function ", Group.Name));
            NewLine();
            MatchIndent(); OutStream.WriteLine(string.Format("void RegisterDebugTrace{0}();", Group.Name));
            MatchIndent(); OutStream.WriteLine(string.Format("HRESULT DebugOut{0}( const char*Prefix, MessageData*pMsg );", Group.Name));
            NewLine();
        }

        string MappingTableName()
        {
            return "MessageDebugTraceMap" + Group.Name;
        }

        void BuildMsgMap(MsgType type, string typeString, string Name, Parameter[] inparam)
        {
            Parameter[] newparams;

            newparams = MakeParameters(type, inparam);
            string strMsgClass = string.Format("{0}::{1}", Group.Name, MsgClassName(Name, typeString));
            string strKey = string.Format("{0}::MID.IDSeq.MsgID", strMsgClass);

            string strFunction = "[](const char* prefix, MessageData* pMsg)->HRESULT{ ";
            strFunction += string.Format("  {0} parser; ", strMsgClass);
            strFunction += "parser.ParseIMsg(pMsg); ";
            strFunction += "parser.TraceOut(prefix,pMsg); ";
            strFunction += "return S_SYSTEM_OK; ";
            strFunction += "} ";

            MatchIndent(); OutStream.WriteLine(string.Format("{0}.insert(std::make_pair({1},{2}));", MappingTableName(), strKey, strFunction));
        }
        
        void BuildMsgMapImpl()
        {
            MatchIndent(); OutStream.WriteLine(string.Format("static std::unordered_map<UINT,std::function<HRESULT(const char* prefix,MessageData *pMsg)>> {0};", MappingTableName()));
            NewLine();

            OpenSection("void", string.Format("RegisterDebugTrace{0}()", Group.Name));

            foreach (MessageBase baseMsg in Group.Items)
            {
                if (baseMsg is ProtocolsProtocolGroupCommand)
                {
                    MatchIndent(); OutStream.WriteLine("// Cmd: " + baseMsg.Desc);
                    ProtocolsProtocolGroupCommand msg = baseMsg as ProtocolsProtocolGroupCommand;

                    BuildMsgMap(MsgType.Cmd, "Cmd", msg.Name, msg.Cmd);
                    BuildMsgMap(MsgType.Res, "Res", msg.Name, msg.Res);
                }

                if (baseMsg is ProtocolsProtocolGroupC2SEvent)
                {
                    MatchIndent(); OutStream.WriteLine("// C2S: " + baseMsg.Desc);
                    ProtocolsProtocolGroupC2SEvent msg = baseMsg as ProtocolsProtocolGroupC2SEvent;

                    BuildMsgMap(MsgType.Evt, "C2SEvt", msg.Name, msg.Params);
                }

                if (baseMsg is ProtocolsProtocolGroupS2CEvent)
                {
                    MatchIndent(); OutStream.WriteLine("// S2C: " + baseMsg.Desc);
                    ProtocolsProtocolGroupS2CEvent msg = baseMsg as ProtocolsProtocolGroupS2CEvent;

                    BuildMsgMap(MsgType.Evt, "S2CEvt", msg.Name, msg.Params);
                }
            }

            CloseSection();
        }

        void BuildIMessageDebugCPP()
        {
            NewLine();

            MatchIndent(); OutStream.WriteLine(string.Format("///////////////////////////////////////////////////////////////"));
            MatchIndent(); OutStream.WriteLine(string.Format("// {0} Debug trace mappping", Group.Name));
            BuildMsgMapImpl();
            NewLine(2);

            MatchIndent(); OutStream.WriteLine(string.Format("///////////////////////////////////////////////////////////////"));
            MatchIndent(); OutStream.WriteLine(string.Format("// {0} Debug trace", Group.Name));
            OpenSection("HRESULT", string.Format("DebugOut{0}( const char *Prefix, MessageData *pMsg )", Group.Name));
            NewLine();
            DefaultHRESULT();
            MatchIndent(); OutStream.WriteLine(string.Format("auto itFount = {0}.end();", MappingTableName()));
            NewLine();
            MatchIndent(); OutStream.WriteLine("protocolChkPtr(pMsg);");
            MatchIndent(); OutStream.WriteLine("protocolChkPtr(Prefix);");

            MatchIndent(); OutStream.WriteLine(string.Format("// {0} Debug trace", Group.Name));
            MatchIndent(); OutStream.WriteLine(string.Format("itFount = {0}.find(pMsg->GetMessageHeader()->msgID.IDSeq.MsgID);", MappingTableName()));
            MatchIndent(); OutStream.WriteLine(string.Format("if( itFount == {0}.end() ) goto Proc_End;", MappingTableName()));
            MatchIndent(); OutStream.WriteLine(string.Format("protocolChk(itFount->second(Prefix,pMsg));", Group.Name));
            
            NewLine();

            ProcEnd();
            ReturnHR();

            CloseSection();
        }

        void BuildH()
        {
            // build IParser
            OpenOutFile(OutputHeaderName());

            BuildHPrefix();

            BuildIMessageDebugH();
            NewLine(2);

            NewLine(2);
            CloseOutFile();
        }

        void BuildCPP()
        {
            OpenOutFile(OutputCPPName());

            BuildCPPPrefix();

            BuildIMessageDebugCPP();
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
