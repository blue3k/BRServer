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
    class MessageServerServiceBuilder : Builder
    {
        Parameter m_ParameterRouteHopContext;

        Parameter m_ParamSenderEntityID;

        // constructor
        public MessageServerServiceBuilder(string strBasePath)
            : base(strBasePath+"Lib/ServerSystem/ServerService")
        {
            GenParameterRouteHopCount = true;
            m_ParameterRouteHopContext = ParamRouteContext;
            ParamRouteContext = null;

            m_ParamSenderEntityID = new Parameter();
            m_ParamSenderEntityID.IsArray = false;
            m_ParamSenderEntityID.IsArraySpecified = false;
            m_ParamSenderEntityID.Name = "SenderEntityID";
            m_ParamSenderEntityID.Type = ParameterType.EntityID;

        }


        public virtual string OutputHeaderName { get { return Group.Name + "Service.h"; } }
        public virtual string OutputCppName { get { return Group.Name + "Service.cpp"; } }
        public virtual string ServiceClassName { get { return Group.Name + "Service"; } }

        public virtual string GetPolicyFunctionName { get { return "GetPolicy" + Group.Name + "()"; } }

        public virtual string IBasePolicyHeaderName { get { return Group.Name + "IPolicy.h"; } }
        //public virtual string IBaseSvrPolicyName { get { return "ISvrPolicy" + Group.Name; } }

        //public virtual string SvrPolicyClassName { get { return "NetSvrPolicy" + Group.Name; } }
        public virtual string IBasePolicyName { get { return "IPolicy" + Group.Name; } }
        //public virtual string PolicyClassName { get { return "NetPolicy" + Group.Name; } }


        protected virtual void BuildHPrefix()
        {
            OutStream.WriteLine("////////////////////////////////////////////////////////////////////////////////");
            OutStream.WriteLine("// ");
            OutStream.WriteLine("// CopyRight (c) 2013 The Braves");
            OutStream.WriteLine("// ");
            OutStream.WriteLine("// Author : Generated");
            OutStream.WriteLine("// ");
            OutStream.WriteLine(string.Format("// Description : {0} Service definitions", Group.Name));
            OutStream.WriteLine("// ");
            OutStream.WriteLine("////////////////////////////////////////////////////////////////////////////////");
            OutStream.WriteLine("");
            OutStream.WriteLine("#pragma once");
            OutStream.WriteLine("");
            OutStream.WriteLine("#include \"Common/Typedefs.h\"");
            OutStream.WriteLine("#include \"Common/PolicyID.h\"");
            OutStream.WriteLine("#include \"ServerSystem/ServerServiceBase.h\"");

            OutStream.WriteLine("#include \"Net/NetDef.h\"");
            OutStream.WriteLine("#include \"Net/NetPolicy.h\"");
            OutStream.WriteLine(string.Format("#include \"Protocol/Policy/{0}\"", IBasePolicyHeaderName));
            NewLine(3);

            // namespace definition
            OpenSection("namespace", "BR");
            OpenSection("namespace", "Svr");
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
            OutStream.WriteLine("#include \"Common/BrBaseTypes.h\"");
            OutStream.WriteLine("#include \"ServerSystem/ServerEntity.h\"");
            OutStream.WriteLine("#include \"ServerSystem/BrServer.h\"");
            OutStream.WriteLine("#include \"ServerSystem/BrServerUtil.h\"");
            OutStream.WriteLine("#include \"ServerSystem/ServiceEntity/EntityInformation.h\"");
            OutStream.WriteLine(string.Format("#include \"ServerSystem/ServerService/{0}\"", OutputHeaderName));
            OutStream.WriteLine("#include \"ServerSystem/SvrTrace.h\"");

            NewLine(3);

            // namespace definition
            OpenSection("namespace", PrjPrefix);
            OpenSection("namespace", "Svr");
        }

        protected string ParamString(Parameter[] parameter)
        {
            return ParamInString(parameter);
        }

        Parameter[] MakeParametersForEvent(Parameter[] parameter)
        {
            List<Parameter> list = new List<Parameter>(MakeParameters(MsgType.Evt, parameter));
            list.Insert(0, m_ParamSenderEntityID);
            return list.ToArray();
        }

        protected virtual void BuildServerServiceH()
        {
            Parameter[] newparams;

            OpenSection("class", string.Format("{0} : public ServerServiceBase", ServiceClassName));
            MatchIndent(); OutStream.WriteLine("private:");
            MatchIndent(1); OutStream.WriteLine("Policy::IPolicy{0}* {1} {{ return GetPolicy<Policy::IPolicy{0}>(); }}", Group.Name, GetPolicyFunctionName);
            MatchIndent(); OutStream.WriteLine("public:");
            MatchIndent(1); OutStream.WriteLine("enum {{ ID_SERVICEPOLICY = Policy::IPolicy{0}::ID_POLICY }};", Group.Name);
            MatchIndent(); OutStream.WriteLine("// Constructor");
            MatchIndent(); OutStream.WriteLine("{0} ( ServerServiceInformation* pService );", ServiceClassName);
            NewLine();

            foreach (MessageBase baseMsg in Group.Items)
            {
                if (baseMsg is ProtocolsProtocolGroupCommand)
                {
                    MatchIndent(); OutStream.WriteLine("// Cmd: " + baseMsg.Desc);
                    ProtocolsProtocolGroupCommand msg = baseMsg as ProtocolsProtocolGroupCommand;

                    newparams = MakeParameters(MsgType.Cmd, msg.Cmd);
                    MatchIndent(); OutStream.WriteLine("Result {0}Cmd( {1} );", msg.Name, ParamString(newparams));
                }

                if (baseMsg is ProtocolsProtocolGroupC2SEvent)
                {
                    MatchIndent(); OutStream.WriteLine("// C2S: " + baseMsg.Desc);
                    ProtocolsProtocolGroupC2SEvent msg = baseMsg as ProtocolsProtocolGroupC2SEvent;

                    if( Group.GenParameterRouteContext )
                        newparams = MakeParametersForEvent(msg.Params);
                    else
                        newparams = MakeParameters(MsgType.Evt, msg.Params);
                    MatchIndent(); OutStream.WriteLine("Result {0}C2SEvt( {1} );", msg.Name, ParamString(newparams));
                }
            }

            NewLine();

            CloseSection();
        }


        // Build parser class implementation
        void BuildServerServiceMethodImpl(MsgType type, Parameter[] originalParameters, string Name, string typeName, Parameter[] parameters)
        {
            OpenSection("Result", string.Format("{0}::{1}{2}( {3} )", ServiceClassName, Name, typeName, ParamInString(parameters)));

            DefaultHRESULT(); NewLine();

            if (Group.GenParameterRouteContext)
            {
                if (type == MsgType.Cmd)
                {
                    MatchIndent(); OutStream.WriteLine("RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );");
                    ParamRouteContext = m_ParameterRouteHopContext;
                    parameters = MakeParameters(type, originalParameters);
                    ParamRouteContext = null;

                    MatchIndent(); OutStream.WriteLine("svrChk({0}->{1}{2}( {3} ) );", GetPolicyFunctionName, Name, typeName, ParamArgument(parameters));
                }
                else if (type == MsgType.Evt)
                {
                    MatchIndent(); OutStream.WriteLine("RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );");
                    ParamRouteContext = m_ParameterRouteHopContext;
                    parameters = MakeParameters(type, originalParameters);
                    ParamRouteContext = null;

                    MatchIndent(); OutStream.WriteLine("svrChk({0}->{1}{2}( {3} ) );", GetPolicyFunctionName, Name, typeName, ParamArgument(parameters));
                }
            }
            else
            {
                MatchIndent(); OutStream.WriteLine(" svrChk({0}->{1}{2}( {3} ) );", GetPolicyFunctionName, Name, typeName, ParamArgument(parameters));
            }

            NewLine();

            ProcEnd(); NewLine();
            ReturnHR(); NewLine();

            CloseSection();
        }

        void BuildServerServiceCPP()
        {

            MatchIndent(); OutStream.WriteLine("{0}::{0}( ServerServiceInformation* pService)", ServiceClassName);
            MatchIndent(1); OutStream.WriteLine(": ServerServiceBase(pService, Policy::{0}::ID_POLICY)", IBasePolicyName);
            MatchIndent(); OutStream.WriteLine("{");
            MatchIndent(1); OutStream.WriteLine("static_assert((UINT)Policy::{0}::ID_POLICY == (UINT)ID_SERVICEPOLICY,\"Invalid Policy ID for a Servicebase \");", IBasePolicyName);
            MatchIndent(1); OutStream.WriteLine("Assert({0});",GetPolicyFunctionName);
            MatchIndent(); OutStream.WriteLine("}");

            NewLine(2);

            Parameter[] newparams;
            foreach (MessageBase baseMsg in Group.Items)
            {
                if (baseMsg is ProtocolsProtocolGroupCommand)
                {
                    MatchIndent(); OutStream.WriteLine("// Cmd: " + baseMsg.Desc);
                    ProtocolsProtocolGroupCommand msg = baseMsg as ProtocolsProtocolGroupCommand;

                    newparams = MakeParameters(MsgType.Cmd, msg.Cmd);
                    BuildServerServiceMethodImpl(MsgType.Cmd, msg.Cmd, msg.Name, "Cmd", newparams);
                }

                if (baseMsg is ProtocolsProtocolGroupC2SEvent)
                {
                    MatchIndent(); OutStream.WriteLine("// C2S: " + baseMsg.Desc);
                    ProtocolsProtocolGroupC2SEvent msg = baseMsg as ProtocolsProtocolGroupC2SEvent;

                    if (Group.GenParameterRouteContext)
                        newparams = MakeParametersForEvent(msg.Params);
                    else
                        newparams = MakeParameters(MsgType.Evt, msg.Params);
                    BuildServerServiceMethodImpl(MsgType.Evt, msg.Params, msg.Name, "C2SEvt", newparams);
                }
            }
        }

        void BuildH()
        {
            // build IParser
            OpenOutFile(OutputHeaderName);

            BuildHPrefix();

            BuildServerServiceH();
            NewLine(2);

            CloseOutFile();
        }

        void BuildCPP()
        {
            OpenOutFile(OutputCppName);

            BuildCPPPrefix();

            BuildServerServiceCPP();
            NewLine(2);

            CloseOutFile();
        }

        public override void Build()
        {
            if (!Group.IsServer)
                return;

            BuildH();
            BuildCPP();
        }

    } //  class MessageIPolicyBuilder : Builder

} // namespace ProtocolBuilder
