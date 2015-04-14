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
    class MessageClusterIPolicyBuilder : MessageIPolicyBuilder
    {
        // constructor
        public MessageClusterIPolicyBuilder(string strBasePath)
            : base(strBasePath)
        {
            GenParameterRouteHopCount = true;
        }

        public override string BuilderNamespace
        {
            get { return "Policy"; }
        }


        public override  string OutputHeaderName()
        {
            return Group.Name + "IClusterPolicy.h";
        }
        public override string OutputCPPName()
        {
            return Group.Name + "IClusterPolicy.cpp";
        }

        public override string ISvrPolicyClassName { get { return "ISvrClusterPolicy" + Group.Name; } }
        public override string IPolicyClassName { get { return "IClusterPolicy" + Group.Name; } }

        public override void Build()
        {
            if( Group.GenParameterRouteHopCount )
                base.Build();
        }

    } //  class MessageIPolicyBuilder : Builder

} // namespace ProtocolBuilder
