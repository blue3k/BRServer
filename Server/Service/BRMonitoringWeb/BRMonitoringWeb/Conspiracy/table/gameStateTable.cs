﻿//------------------------------------------------------------------------------
// <auto-generated>
//     This code was generated by a tool.
//     Runtime Version:2.0.50727.8009
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
// </auto-generated>
//------------------------------------------------------------------------------

// 
// This source code was auto-generated by xsd, Version=2.0.50727.3038.
// 
namespace GameTbl {
    using System.Xml.Serialization;
    
    
    /// <remarks/>
    [System.CodeDom.Compiler.GeneratedCodeAttribute("xsd", "2.0.50727.3038")]
    [System.SerializableAttribute()]
    [System.Diagnostics.DebuggerStepThroughAttribute()]
    [System.ComponentModel.DesignerCategoryAttribute("code")]
    [System.Xml.Serialization.XmlTypeAttribute(AnonymousType=true)]
    [System.Xml.Serialization.XmlRootAttribute(Namespace="", IsNullable=false)]
    public partial class gameStateTbl {
        
        private gameStateTblStateItem[] stateItemField;
        
        /// <remarks/>
        [System.Xml.Serialization.XmlElementAttribute("StateItem")]
        public gameStateTblStateItem[] StateItem {
            get {
                return this.stateItemField;
            }
            set {
                this.stateItemField = value;
            }
        }
    }
    
    /// <remarks/>
    [System.CodeDom.Compiler.GeneratedCodeAttribute("xsd", "2.0.50727.3038")]
    [System.SerializableAttribute()]
    [System.Diagnostics.DebuggerStepThroughAttribute()]
    [System.ComponentModel.DesignerCategoryAttribute("code")]
    [System.Xml.Serialization.XmlTypeAttribute(AnonymousType=true)]
    public partial class gameStateTblStateItem {
        
        private gameStateTblStateItemStateItemID stateItemIDField;
        
        private int sectionField;
        
        private string bGMField;
        
        private bool bGMLoopField;
        
        private float delayField;
        
        public gameStateTblStateItem() {
            this.sectionField = 0;
            this.bGMField = "\"\"";
            this.bGMLoopField = true;
            this.delayField = ((float)(0F));
        }
        
        /// <remarks/>
        [System.Xml.Serialization.XmlAttributeAttribute()]
        public gameStateTblStateItemStateItemID StateItemID {
            get {
                return this.stateItemIDField;
            }
            set {
                this.stateItemIDField = value;
            }
        }
        
        /// <remarks/>
        [System.Xml.Serialization.XmlAttributeAttribute()]
        [System.ComponentModel.DefaultValueAttribute(0)]
        public int Section {
            get {
                return this.sectionField;
            }
            set {
                this.sectionField = value;
            }
        }
        
        /// <remarks/>
        [System.Xml.Serialization.XmlAttributeAttribute()]
        [System.ComponentModel.DefaultValueAttribute("\"\"")]
        public string BGM {
            get {
                return this.bGMField;
            }
            set {
                this.bGMField = value;
            }
        }
        
        /// <remarks/>
        [System.Xml.Serialization.XmlAttributeAttribute()]
        [System.ComponentModel.DefaultValueAttribute(true)]
        public bool BGMLoop {
            get {
                return this.bGMLoopField;
            }
            set {
                this.bGMLoopField = value;
            }
        }
        
        /// <remarks/>
        [System.Xml.Serialization.XmlAttributeAttribute()]
        [System.ComponentModel.DefaultValueAttribute(typeof(float), "0")]
        public float Delay {
            get {
                return this.delayField;
            }
            set {
                this.delayField = value;
            }
        }
    }
    
    /// <remarks/>
    [System.CodeDom.Compiler.GeneratedCodeAttribute("xsd", "2.0.50727.3038")]
    [System.SerializableAttribute()]
    [System.Xml.Serialization.XmlTypeAttribute(AnonymousType=true)]
    public enum gameStateTblStateItemStateItemID {
        
        /// <remarks/>
        None,
        
        /// <remarks/>
        FreeDebate,
        
        /// <remarks/>
        NightVote,
        
        /// <remarks/>
        MorningDebate,
        
        /// <remarks/>
        VoteForSuspects,
        
        /// <remarks/>
        DefenceOfSuspects,
        
        /// <remarks/>
        VoteForHanging,
        
        /// <remarks/>
        End,
        
        /// <remarks/>
        Hanging,
        
        /// <remarks/>
        Login,
        
        /// <remarks/>
        Lobby,
    }
}
