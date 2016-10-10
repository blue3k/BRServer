////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves Corporation.
// 
// Author : Generated
//
// Description : I Policy Builder
//
////////////////////////////////////////////////////////////////////////////////

using System;
using System.Collections.Generic;
using System.Text;
using ProtocolXml;

namespace ProtocolBuilder
{
    class MessageIPolicyBuilder : Builder
    {
        // constructor
        public MessageIPolicyBuilder(string strBasePath)
            : base(strBasePath + "Lib/Protocol/Policy")
        {
        }

        public virtual string BuilderNamespace
        {
            get { return "Policy"; }
        }

        public virtual string OutputHeaderName()
        {
            return Group.Name + "IPolicy.h";
        }
        public virtual string OutputCPPName()
        {
            return Group.Name + "IPolicy.cpp";
        }

        public virtual string ISvrPolicyClassName { get { return "ISvrPolicy" + Group.Name; } }
        public virtual string IPolicyClassName { get { return "IPolicy" + Group.Name; } }


        void BuildHPrefix()
        {
            OutStream.WriteLine("////////////////////////////////////////////////////////////////////////////////");
            OutStream.WriteLine("// ");
            OutStream.WriteLine("// CopyRight (c) 2015 The Braves");
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
            if (Group.IsServer)
            {
                OutStream.WriteLine("#include \"Common/BrSvrTypes.h\"");
            }
            else
            {
                OutStream.WriteLine("#include \"Common/BrBaseTypes.h\"");
            }
            OutStream.WriteLine("#include \"Common/BrGameTypes.h\"");
            OutStream.WriteLine("#include \"Common/PolicyID.h\"");
            OutStream.WriteLine("#include \"Net/NetDef.h\"");
            OutStream.WriteLine("#include \"Net/NetPolicy.h\"");
            if( Group.IsServer )
                OutStream.WriteLine("#include \"Common/SvrPolicyID.h\"");
            NewLine(3);

            // namespace definition
            OpenSection("namespace", PrjPrefix);
            OpenSection("namespace", BuilderNamespace);
  
            NewLine();
        }


        void BuildCPPPrefix()
        {
            OutStream.WriteLine("////////////////////////////////////////////////////////////////////////////////");
            OutStream.WriteLine("// ");
            OutStream.WriteLine("// CopyRight (c) 2015 The Braves");
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
            OutStream.WriteLine("#include \"Common/Message.h\"");
            OutStream.WriteLine(string.Format("#include \"Protocol/Policy/{0}\"", OutputHeaderName()));
            NewLine(3);

            // namespace definition
            OpenSection("namespace", PrjPrefix);
            OpenSection("namespace", BuilderNamespace);
        }

        void BuildIServerPolicyH()
        {
            Parameter[] newparams;

            // Packer interface
            OpenSection("class", string.Format("{0} : public Net::INetPolicy", ISvrPolicyClassName));

            MatchIndent(); OutStream.WriteLine("public:");

            // ID definition
            MatchIndent(); OutStream.WriteLine( "enum { ID_POLICY = POLICY_SVR_" + Group.Name.ToUpper() + " };");
            NewLine();

            // constructor
            MatchIndent(); OutStream.WriteLine("// Constructor");
            MatchIndent(); OutStream.WriteLine(
                string.Format("{0}( Net::IConnection* pConn ) : Net::INetPolicy(ID_POLICY, pConn)", ISvrPolicyClassName));
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
                        string.Format("virtual Result {0}Res( {1} ) = 0;", msg.Name, ParamInString(newparams)));
                }

                if (baseMsg is ProtocolsProtocolGroupC2SEvent)
                {
                    MatchIndent(); OutStream.WriteLine("// C2S: " + baseMsg.Desc);
                    ProtocolsProtocolGroupC2SEvent msg = baseMsg as ProtocolsProtocolGroupC2SEvent;

                }

                if (baseMsg is ProtocolsProtocolGroupS2CEvent)
                {
                    MatchIndent(); OutStream.WriteLine("// S2C: " + baseMsg.Desc);
                    ProtocolsProtocolGroupS2CEvent msg = baseMsg as ProtocolsProtocolGroupS2CEvent;

                    newparams = MakeParameters(MsgType.Evt, msg.Params);
                    MatchIndent(); OutStream.WriteLine(
                        string.Format("virtual Result {0}S2CEvt( {1} ) = 0;", msg.Name, ParamInString(newparams)));
                }
            }

            NewLine();

            CloseSection();
        }

        void BuildIClientPolicyH()
        {
            Parameter[] newparams;

            // Packer interface
            OpenSection("class", string.Format("{0} : public Net::INetPolicy", IPolicyClassName));

            MatchIndent(); OutStream.WriteLine("public:");

            // ID definition
            MatchIndent(); OutStream.WriteLine("enum { ID_POLICY = POLICY_" + Group.Name.ToUpper() + " };");
            NewLine();

            //// Constructor
            MatchIndent(); OutStream.WriteLine("// Constructor");
            MatchIndent(); OutStream.WriteLine(
                string.Format("{0}( Net::IConnection* pConn ) : Net::INetPolicy(ID_POLICY, pConn)", IPolicyClassName));
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
                        string.Format("virtual Result {0}Cmd( {1} ) = 0;", msg.Name, ParamInString(newparams)));
                }

                if (baseMsg is ProtocolsProtocolGroupC2SEvent)
                {
                    MatchIndent(); OutStream.WriteLine("// C2S: " + baseMsg.Desc);
                    ProtocolsProtocolGroupC2SEvent msg = baseMsg as ProtocolsProtocolGroupC2SEvent;

                    newparams = MakeParameters(MsgType.Evt, msg.Params);
                    MatchIndent(); OutStream.WriteLine(
                        string.Format("virtual Result {0}C2SEvt( {1} ) = 0;", msg.Name, ParamInString(newparams)));
                }

                if (baseMsg is ProtocolsProtocolGroupS2CEvent)
                {
                    MatchIndent(); OutStream.WriteLine("// S2C: " + baseMsg.Desc);
                    ProtocolsProtocolGroupS2CEvent msg = baseMsg as ProtocolsProtocolGroupS2CEvent;

                }
            }
            NewLine();

            CloseSection();
        }


        void BuildH()
        {
            // build IParser
            OpenOutFile(OutputHeaderName());

            BuildHPrefix();

            BuildIServerPolicyH();
            NewLine(2);

            BuildIClientPolicyH();
            NewLine(2);

            CloseOutFile();
        }


        public override void Build()
        {
            BuildH();
        }


    } //  class MessageIPolicyBuilder : Builder

} // namespace ProtocolBuilder
