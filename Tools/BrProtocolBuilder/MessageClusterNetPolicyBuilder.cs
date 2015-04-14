////////////////////////////////////////////////////////////////////////////////
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
    class MessageClusterNetPolicyBuilder: MessageNetPolicyBuilder
    { 
        // constructor
        public MessageClusterNetPolicyBuilder(string strBasePath)
            : base(strBasePath)
        {
            GenParameterRouteHopCount = true;
        }

        public override string OutputHeaderName()
        {
            return Group.Name + "NetCluserPolicy.h";
        }

        public override string OutputCPPName()
        {
            return Group.Name + "NetClusterPolicy.cpp";
        }

        public override string IBasePolicyHeaderName { get { return Group.Name + "IClusterPolicy.h"; } }
        public override string IBaseSvrPolicyName { get { return "ISvrClusterPolicy" + Group.Name; } }
        public override string SvrPolicyClassName { get { return "NetSvrClusterPolicy" + Group.Name; } }
        public override string IBasePolicyName { get { return "IClusterPolicy" + Group.Name; } }
        public override string PolicyClassName { get { return "NetClusterPolicy" + Group.Name; } }


        public override void Build()
        {
            if (Group.GenParameterRouteHopCount)
                base.Build();
        }
    }
}
