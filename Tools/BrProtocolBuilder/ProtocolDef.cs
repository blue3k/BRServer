﻿//------------------------------------------------------------------------------
// <auto-generated>
//     This code was generated by a tool.
//     Runtime Version:4.0.30319.42000
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
// </auto-generated>
//------------------------------------------------------------------------------

// 
// This source code was auto-generated by xsd, Version=4.6.81.0.
// 
namespace ProtocolXml {
    using System.Xml.Serialization;
    
    
    /// <remarks/>
    [System.CodeDom.Compiler.GeneratedCodeAttribute("xsd", "4.6.81.0")]
    [System.SerializableAttribute()]
    [System.Diagnostics.DebuggerStepThroughAttribute()]
    [System.ComponentModel.DesignerCategoryAttribute("code")]
    [System.Xml.Serialization.XmlTypeAttribute(AnonymousType=true, Namespace="http://www.TheBraves.com/XMLProtocol")]
    [System.Xml.Serialization.XmlRootAttribute(Namespace="http://www.TheBraves.com/XMLProtocol", IsNullable=false)]
    public partial class Protocols {
        
        private ProtocolsProtocolGroup[] protocolGroupField;
        
        /// <remarks/>
        [System.Xml.Serialization.XmlElementAttribute("ProtocolGroup")]
        public ProtocolsProtocolGroup[] ProtocolGroup {
            get {
                return this.protocolGroupField;
            }
            set {
                this.protocolGroupField = value;
            }
        }
    }
    
    /// <remarks/>
    [System.CodeDom.Compiler.GeneratedCodeAttribute("xsd", "4.6.81.0")]
    [System.SerializableAttribute()]
    [System.Diagnostics.DebuggerStepThroughAttribute()]
    [System.ComponentModel.DesignerCategoryAttribute("code")]
    [System.Xml.Serialization.XmlTypeAttribute(AnonymousType=true, Namespace="http://www.TheBraves.com/XMLProtocol")]
    public partial class ProtocolsProtocolGroup {
        
        private MessageBase[] itemsField;
        
        private string nameField;
        
        private bool genParameterContextField;
        
        private bool genParameterRouteContextField;
        
        private bool isMobileField;
        
        private bool isServerField;
        
        private bool genParameterRouteHopCountField;
        
        private bool genNetField;
        
        private bool genCSharpField;
        
        private string descField;
        
        public ProtocolsProtocolGroup() {
            this.genParameterContextField = true;
            this.genParameterRouteContextField = false;
            this.isMobileField = false;
            this.isServerField = false;
            this.genParameterRouteHopCountField = false;
            this.genNetField = false;
            this.genCSharpField = false;
        }
        
        /// <remarks/>
        [System.Xml.Serialization.XmlElementAttribute("C2SEvent", typeof(ProtocolsProtocolGroupC2SEvent))]
        [System.Xml.Serialization.XmlElementAttribute("Command", typeof(ProtocolsProtocolGroupCommand))]
        [System.Xml.Serialization.XmlElementAttribute("S2CEvent", typeof(ProtocolsProtocolGroupS2CEvent))]
        public MessageBase[] Items {
            get {
                return this.itemsField;
            }
            set {
                this.itemsField = value;
            }
        }
        
        /// <remarks/>
        [System.Xml.Serialization.XmlAttributeAttribute()]
        public string Name {
            get {
                return this.nameField;
            }
            set {
                this.nameField = value;
            }
        }
        
        /// <remarks/>
        [System.Xml.Serialization.XmlAttributeAttribute()]
        [System.ComponentModel.DefaultValueAttribute(true)]
        public bool GenParameterContext {
            get {
                return this.genParameterContextField;
            }
            set {
                this.genParameterContextField = value;
            }
        }
        
        /// <remarks/>
        [System.Xml.Serialization.XmlAttributeAttribute()]
        [System.ComponentModel.DefaultValueAttribute(false)]
        public bool GenParameterRouteContext {
            get {
                return this.genParameterRouteContextField;
            }
            set {
                this.genParameterRouteContextField = value;
            }
        }
        
        /// <remarks/>
        [System.Xml.Serialization.XmlAttributeAttribute()]
        [System.ComponentModel.DefaultValueAttribute(false)]
        public bool IsMobile {
            get {
                return this.isMobileField;
            }
            set {
                this.isMobileField = value;
            }
        }
        
        /// <remarks/>
        [System.Xml.Serialization.XmlAttributeAttribute()]
        [System.ComponentModel.DefaultValueAttribute(false)]
        public bool IsServer {
            get {
                return this.isServerField;
            }
            set {
                this.isServerField = value;
            }
        }
        
        /// <remarks/>
        [System.Xml.Serialization.XmlAttributeAttribute()]
        [System.ComponentModel.DefaultValueAttribute(false)]
        public bool GenParameterRouteHopCount {
            get {
                return this.genParameterRouteHopCountField;
            }
            set {
                this.genParameterRouteHopCountField = value;
            }
        }
        
        /// <remarks/>
        [System.Xml.Serialization.XmlAttributeAttribute()]
        [System.ComponentModel.DefaultValueAttribute(false)]
        public bool GenNet {
            get {
                return this.genNetField;
            }
            set {
                this.genNetField = value;
            }
        }
        
        /// <remarks/>
        [System.Xml.Serialization.XmlAttributeAttribute()]
        [System.ComponentModel.DefaultValueAttribute(false)]
        public bool GenCSharp {
            get {
                return this.genCSharpField;
            }
            set {
                this.genCSharpField = value;
            }
        }
        
        /// <remarks/>
        [System.Xml.Serialization.XmlAttributeAttribute()]
        public string Desc {
            get {
                return this.descField;
            }
            set {
                this.descField = value;
            }
        }
    }
    
    /// <remarks/>
    [System.CodeDom.Compiler.GeneratedCodeAttribute("xsd", "4.6.81.0")]
    [System.SerializableAttribute()]
    [System.Diagnostics.DebuggerStepThroughAttribute()]
    [System.ComponentModel.DesignerCategoryAttribute("code")]
    [System.Xml.Serialization.XmlTypeAttribute(AnonymousType=true, Namespace="http://www.TheBraves.com/XMLProtocol")]
    public partial class ProtocolsProtocolGroupC2SEvent : MessageBase {
        
        private Parameter[] paramsField;
        
        /// <remarks/>
        [System.Xml.Serialization.XmlElementAttribute("Params")]
        public Parameter[] Params {
            get {
                return this.paramsField;
            }
            set {
                this.paramsField = value;
            }
        }
    }
    
    /// <remarks/>
    [System.CodeDom.Compiler.GeneratedCodeAttribute("xsd", "4.6.81.0")]
    [System.SerializableAttribute()]
    [System.Diagnostics.DebuggerStepThroughAttribute()]
    [System.ComponentModel.DesignerCategoryAttribute("code")]
    [System.Xml.Serialization.XmlTypeAttribute(Namespace="http://www.TheBraves.com/XMLProtocol")]
    public partial class Parameter {
        
        private string nameField;
        
        private ParameterType typeField;
        
        private bool isArrayField;
        
        private bool isArrayFieldSpecified;
        
        private string descField;
        
        /// <remarks/>
        [System.Xml.Serialization.XmlAttributeAttribute()]
        public string Name {
            get {
                return this.nameField;
            }
            set {
                this.nameField = value;
            }
        }
        
        /// <remarks/>
        [System.Xml.Serialization.XmlAttributeAttribute()]
        public ParameterType Type {
            get {
                return this.typeField;
            }
            set {
                this.typeField = value;
            }
        }
        
        /// <remarks/>
        [System.Xml.Serialization.XmlAttributeAttribute()]
        public bool IsArray {
            get {
                return this.isArrayField;
            }
            set {
                this.isArrayField = value;
            }
        }
        
        /// <remarks/>
        [System.Xml.Serialization.XmlIgnoreAttribute()]
        public bool IsArraySpecified {
            get {
                return this.isArrayFieldSpecified;
            }
            set {
                this.isArrayFieldSpecified = value;
            }
        }
        
        /// <remarks/>
        [System.Xml.Serialization.XmlAttributeAttribute()]
        public string Desc {
            get {
                return this.descField;
            }
            set {
                this.descField = value;
            }
        }
    }
    
    /// <remarks/>
    [System.CodeDom.Compiler.GeneratedCodeAttribute("xsd", "4.6.81.0")]
    [System.SerializableAttribute()]
    [System.Xml.Serialization.XmlTypeAttribute(AnonymousType=true, Namespace="http://www.TheBraves.com/XMLProtocol")]
    public enum ParameterType {
        
        /// <remarks/>
        String,
        
        /// <remarks/>
        INT8,
        
        /// <remarks/>
        UINT8,
        
        /// <remarks/>
        INT16,
        
        /// <remarks/>
        UINT16,
        
        /// <remarks/>
        INT32,
        
        /// <remarks/>
        UINT32,
        
        /// <remarks/>
        INT64,
        
        /// <remarks/>
        UINT64,
        
        /// <remarks/>
        BYTE,
        
        /// <remarks/>
        WORD,
        
        /// <remarks/>
        DWORD,
        
        /// <remarks/>
        QWORD,
        
        /// <remarks/>
        FLOAT,
        
        /// <remarks/>
        Context,
        
        /// <remarks/>
        ClusterID,
        
        /// <remarks/>
        AccountID,
        
        /// <remarks/>
        PlayerID,
        
        /// <remarks/>
        FacebookUID,
        
        /// <remarks/>
        AuthTicket,
        
        /// <remarks/>
        GameInsID,
        
        /// <remarks/>
        GameInsUID,
        
        /// <remarks/>
        PartyUID,
        
        /// <remarks/>
        NotificationType,
        
        /// <remarks/>
        RankingType,
        
        /// <remarks/>
        PlayerRole,
        
        /// <remarks/>
        GameStateID,
        
        /// <remarks/>
        GameWinner,
        
        /// <remarks/>
        PlayerKilledReason,
        
        /// <remarks/>
        PlayerRevealedReason,
        
        /// <remarks/>
        ClusterType,
        
        /// <remarks/>
        ClusterMembership,
        
        /// <remarks/>
        ServiceStatus,
        
        /// <remarks/>
        GameID,
        
        /// <remarks/>
        MatchingPlayerInformation,
        
        /// <remarks/>
        MatchingQueueTicket,
        
        /// <remarks/>
        LocalUID,
        
        /// <remarks/>
        Vector2,
        
        /// <remarks/>
        Vector3,
        
        /// <remarks/>
        HRESULT,
        
        /// <remarks/>
        NetClass,
        
        /// <remarks/>
        NetAddress,
        
        /// <remarks/>
        EntityID,
        
        /// <remarks/>
        EntityUID,
        
        /// <remarks/>
        ServerID,
        
        /// <remarks/>
        RouteContext,
        
        /// <remarks/>
        ServiceInformation,
        
        /// <remarks/>
        PlayerInformation,
        
        /// <remarks/>
        RankingPlayerInformation,
        
        /// <remarks/>
        FriendInformation,
        
        /// <remarks/>
        ServerFriendInformation,
        
        /// <remarks/>
        TotalRankingPlayerInformation,
        
        /// <remarks/>
        PerformanceCounterInfo,
        
        /// <remarks/>
        PerformanceCounterInstanceInfo,
    }
    
    /// <remarks/>
    [System.CodeDom.Compiler.GeneratedCodeAttribute("xsd", "4.6.81.0")]
    [System.SerializableAttribute()]
    [System.Diagnostics.DebuggerStepThroughAttribute()]
    [System.ComponentModel.DesignerCategoryAttribute("code")]
    [System.Xml.Serialization.XmlTypeAttribute(Namespace="http://www.TheBraves.com/XMLProtocol")]
    public partial class MessageBase {
        
        private string nameField;
        
        private bool reliableField;
        
        private MessageBaseCrypto cryptoField;
        
        private MessageBaseTrace traceField;
        
        private MessageBaseUsage usageField;
        
        private MessageBaseChecksum checksumField;
        
        private string descField;
        
        public MessageBase() {
            this.usageField = MessageBaseUsage.None;
            this.checksumField = MessageBaseChecksum.NONE;
        }
        
        /// <remarks/>
        [System.Xml.Serialization.XmlAttributeAttribute()]
        public string Name {
            get {
                return this.nameField;
            }
            set {
                this.nameField = value;
            }
        }
        
        /// <remarks/>
        [System.Xml.Serialization.XmlAttributeAttribute()]
        public bool Reliable {
            get {
                return this.reliableField;
            }
            set {
                this.reliableField = value;
            }
        }
        
        /// <remarks/>
        [System.Xml.Serialization.XmlAttributeAttribute()]
        public MessageBaseCrypto Crypto {
            get {
                return this.cryptoField;
            }
            set {
                this.cryptoField = value;
            }
        }
        
        /// <remarks/>
        [System.Xml.Serialization.XmlAttributeAttribute()]
        public MessageBaseTrace Trace {
            get {
                return this.traceField;
            }
            set {
                this.traceField = value;
            }
        }
        
        /// <remarks/>
        [System.Xml.Serialization.XmlAttributeAttribute()]
        [System.ComponentModel.DefaultValueAttribute(MessageBaseUsage.None)]
        public MessageBaseUsage Usage {
            get {
                return this.usageField;
            }
            set {
                this.usageField = value;
            }
        }
        
        /// <remarks/>
        [System.Xml.Serialization.XmlAttributeAttribute()]
        [System.ComponentModel.DefaultValueAttribute(MessageBaseChecksum.NONE)]
        public MessageBaseChecksum Checksum {
            get {
                return this.checksumField;
            }
            set {
                this.checksumField = value;
            }
        }
        
        /// <remarks/>
        [System.Xml.Serialization.XmlAttributeAttribute()]
        public string Desc {
            get {
                return this.descField;
            }
            set {
                this.descField = value;
            }
        }
    }
    
    /// <remarks/>
    [System.CodeDom.Compiler.GeneratedCodeAttribute("xsd", "4.6.81.0")]
    [System.SerializableAttribute()]
    [System.Xml.Serialization.XmlTypeAttribute(AnonymousType=true, Namespace="http://www.TheBraves.com/XMLProtocol")]
    public enum MessageBaseCrypto {
        
        /// <remarks/>
        No,
        
        /// <remarks/>
        Mask,
    }
    
    /// <remarks/>
    [System.CodeDom.Compiler.GeneratedCodeAttribute("xsd", "4.6.81.0")]
    [System.SerializableAttribute()]
    [System.Xml.Serialization.XmlTypeAttribute(AnonymousType=true, Namespace="http://www.TheBraves.com/XMLProtocol")]
    public enum MessageBaseTrace {
        
        /// <remarks/>
        DBG1,
        
        /// <remarks/>
        DBG2,
        
        /// <remarks/>
        DBG3,
        
        /// <remarks/>
        DBG4,
        
        /// <remarks/>
        DBG5,
        
        /// <remarks/>
        DBG6,
        
        /// <remarks/>
        DBG7,
        
        /// <remarks/>
        DBG8,
        
        /// <remarks/>
        DBG9,
        
        /// <remarks/>
        DBG10,
        
        /// <remarks/>
        [System.Xml.Serialization.XmlEnumAttribute("")]
        Item,
    }
    
    /// <remarks/>
    [System.CodeDom.Compiler.GeneratedCodeAttribute("xsd", "4.6.81.0")]
    [System.SerializableAttribute()]
    [System.Xml.Serialization.XmlTypeAttribute(AnonymousType=true, Namespace="http://www.TheBraves.com/XMLProtocol")]
    public enum MessageBaseUsage {
        
        /// <remarks/>
        None,
        
        /// <remarks/>
        ClusterDataRead,
        
        /// <remarks/>
        ClusterDataWrite,
        
        /// <remarks/>
        ClusterStatusRead,
        
        /// <remarks/>
        ClusterStatusWrite,
    }
    
    /// <remarks/>
    [System.CodeDom.Compiler.GeneratedCodeAttribute("xsd", "4.6.81.0")]
    [System.SerializableAttribute()]
    [System.Xml.Serialization.XmlTypeAttribute(AnonymousType=true, Namespace="http://www.TheBraves.com/XMLProtocol")]
    public enum MessageBaseChecksum {
        
        /// <remarks/>
        NONE,
        
        /// <remarks/>
        CRC32,
    }
    
    /// <remarks/>
    [System.CodeDom.Compiler.GeneratedCodeAttribute("xsd", "4.6.81.0")]
    [System.SerializableAttribute()]
    [System.Diagnostics.DebuggerStepThroughAttribute()]
    [System.ComponentModel.DesignerCategoryAttribute("code")]
    [System.Xml.Serialization.XmlTypeAttribute(AnonymousType=true, Namespace="http://www.TheBraves.com/XMLProtocol")]
    public partial class ProtocolsProtocolGroupCommand : MessageBase {
        
        private Parameter[] cmdField;
        
        private Parameter[] resField;
        
        /// <remarks/>
        [System.Xml.Serialization.XmlArrayItemAttribute("Params", IsNullable=false)]
        public Parameter[] Cmd {
            get {
                return this.cmdField;
            }
            set {
                this.cmdField = value;
            }
        }
        
        /// <remarks/>
        [System.Xml.Serialization.XmlArrayItemAttribute("Params", IsNullable=false)]
        public Parameter[] Res {
            get {
                return this.resField;
            }
            set {
                this.resField = value;
            }
        }
    }
    
    /// <remarks/>
    [System.CodeDom.Compiler.GeneratedCodeAttribute("xsd", "4.6.81.0")]
    [System.SerializableAttribute()]
    [System.Diagnostics.DebuggerStepThroughAttribute()]
    [System.ComponentModel.DesignerCategoryAttribute("code")]
    [System.Xml.Serialization.XmlTypeAttribute(AnonymousType=true, Namespace="http://www.TheBraves.com/XMLProtocol")]
    public partial class ProtocolsProtocolGroupS2CEvent : MessageBase {
        
        private Parameter[] paramsField;
        
        /// <remarks/>
        [System.Xml.Serialization.XmlElementAttribute("Params")]
        public Parameter[] Params {
            get {
                return this.paramsField;
            }
            set {
                this.paramsField = value;
            }
        }
    }
}
