////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2014 The Braves
// 
// Author : KyungKun Ko
//
// Description : Table Unreal builder class
//                  Generates .u, .CPP
//
////////////////////////////////////////////////////////////////////////////////

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Data;


using System.Reflection;






namespace TableBuilder
{
    /// <summary>
    /// Builder class for building interface
    /// </summary>
    internal partial class UnrealTableBuilder : TableBuilder
    {
        private string _unrealGenerateClassName;
        protected string UnrealGenerateClassName
        {
            get { return _unrealGenerateClassName; }
        }

        private string _unrealGenerateRowTypeName;
        protected string UnrealGenerateRowTypeName
        {
            get { return _unrealGenerateRowTypeName; }
            set { _unrealGenerateRowTypeName = value; }
        }

        // C# -> C++ type generated from UC conversion map  
        protected Dictionary<string, string> _typeCPPConvMap = new Dictionary<string, string>();

        // Constructor
        public UnrealTableBuilder(string basePath, string className, KeyInfo keyInfo,
            string baseParserName)
            : base(basePath, keyInfo, baseParserName)
        {
            ClassName = className;
            _unrealGenerateClassName = "U" + className;

            _includesAtHeader = new List<string>();
            _includesAtCpp = new List<string>();
            BuildCPPTypeConvMap();
        }

        public UnrealTableBuilder(string basePath, string className, 
            KeyInfo keyInfo1, KeyInfo keyInfo2, string baseParserName)
            : base(basePath, keyInfo1, keyInfo2, baseParserName)
        {
            ClassName = className;
            _unrealGenerateClassName = "U" + className;

            _includesAtHeader = new List<string>();
            _includesAtCpp = new List<string>();
            BuildCPPTypeConvMap();
        }

        // Output header file name
        public string OutputHeaderName()
        {
            return ClassName + ".h";
        }

        // Output cpp file name
        public string OutputCPPName()
        {
            return ClassName + ".cpp";
        }



        /// <summary>
        /// UC Prefix generation
        /// </summary>
        private void BuildUCPrefix()
        {
            string statement = "";
            OutStream.WriteLine("////////////////////////////////////////////////////////////////////////////////");
            OutStream.WriteLine("// ");
            OutStream.WriteLine("// CopyRight (c) 2014 The Braves Co.");
            OutStream.WriteLine("// ");
            OutStream.WriteLine("// Author : Generated");
            OutStream.WriteLine("// ");
            OutStream.WriteLine(string.Format("// Description : {0}  definitions", ClassName));
            OutStream.WriteLine("// ");
            OutStream.WriteLine("////////////////////////////////////////////////////////////////////////////////");
            OutStream.WriteLine("");
            OutStream.WriteLine("");

            NewLine(3);

            statement = string.Format("class {0} extends Object", ClassName);
            WriteStatement(statement);
            WriteStatement("native", 1);

            foreach (string strInc in _includesAtHeader)
            {
                //statement = string.for
                OutStream.WriteLine("dependson({0})", strInc);
            }
            WriteStatement(";");
            NewLine(2);

        }


        /// <summary>
        /// CPP Prefix generation
        /// </summary>
        private void BuildCPPPrefix()
        {
            OutStream.WriteLine("////////////////////////////////////////////////////////////////////////////////");
            OutStream.WriteLine("// ");
            OutStream.WriteLine("// CopyRight (c) 2014 The Braves Co.");
            OutStream.WriteLine("// ");
            OutStream.WriteLine("// Author : Generated");
            OutStream.WriteLine("// ");
            OutStream.WriteLine(string.Format("// Description : {0} implementations", ClassName));
            OutStream.WriteLine("// ");
            OutStream.WriteLine("////////////////////////////////////////////////////////////////////////////////");
            OutStream.WriteLine("");
            OutStream.WriteLine("");

            foreach (string strInc in _includesAtCpp)
            {
                OutStream.WriteLine("#include \"{0}\"", strInc);
            }

            //OutStream.WriteLine(string.Format("#include \"SRWOGameTableClasses.h\""));
            NewLine(3);

            // namespace definition
            //OpenSection("namespace", Builder._tableNameSpace);
            //OpenSection("class", ClassName);
            string statement = "//" + "\t" + UnrealGenerateClassName;
            WriteStatement("//////////////////////////////////////////////////////////////////////////");
            WriteStatement(statement);
            WriteStatement("//////////////////////////////////////////////////////////////////////////");
            statement = string.Format("IMPLEMENT_CLASS({0});", UnrealGenerateClassName);
            WriteStatement(statement);
        }



        // Build Header class body
        private void BuildUCBody()
        {
            foreach (KeyValuePair<string, string> kvp in _nameMap)
            {
                Type typeInfo = GetTypeInfo(kvp.Key);

                if (typeInfo.IsEnum)
                    DeclareEnum(kvp.Value, typeInfo);

                if (typeInfo.IsClass)
                    DeclareClass(kvp.Value, typeInfo);
            }

            // Build member data.
            DefineMemberVariables(GetTypeInfo(ClassName));

            // Build member function.
            DeclareMemberFunction();
        }

        // Build CPP class implementation
        private void BuildCPPBody()
        {
            foreach (KeyValuePair<string, string> kvp in _nameMap)
            {
                Type typeInfo = GetTypeInfo(kvp.Key);

                if (typeInfo.IsEnum)
                    DefineEnumDefinition(kvp.Value, typeInfo);
            }

            // Build type definition.
            DefineTableParser(GetTypeInfo(ClassName));

            // Build Imp
            DefineMemberFunction();
        }

        // Close Header
        private void CloseUCBuild()
        {
            CloseAllSection();
            CloseOutFile();
        }

        // Close CPP
        private void CloseCPPBuild()
        {
            CloseAllSection();
            CloseOutFile();
        }


        //////////////////////////////////////////////////////////////////////////
        //
        //  Virtual interfaces
        //
        protected override string GetOutputFilePath()
        {
            if (!BasePath.EndsWith("/")
                && !BasePath.EndsWith("\\"))
                return BasePath + "/Unreal/";

            return BasePath + "Unreal/";
        }

        protected override void BuildTypeConvMap()
        {
            _typeConvMap.Add("Int16", "int");
            //_typeConvMap.Add("Int64", "__int64");
            _typeConvMap.Add("Single", "float");
            //_typeConvMap.Add("Double", "double");
            _typeConvMap.Add("Byte", "byte");
            //_typeConvMap.Add("UInt64", "unsigned __int64");
            _typeConvMap.Add("UInt16", "int");
            _typeConvMap.Add("String", "string");
            _typeConvMap.Add("Boolean", "bool");
            _typeConvMap.Add("SByte", "byte");
            _typeConvMap.Add("UInt32", "int");
            _typeConvMap.Add("Int32", "int");
            _typeConvMap.Add("nonNegativeInteger", "int");
        }

        protected void BuildCPPTypeConvMap()
        {
            _typeCPPConvMap.Add("Int16", "INT");
            //_typeConvMap.Add("Int64", "__int64");
            _typeCPPConvMap.Add("Single", "FLOAT");
            //_typeConvMap.Add("Double", "double");
            _typeCPPConvMap.Add("Byte", "BYTE");
            //_typeConvMap.Add("UInt64", "unsigned __int64");
            _typeCPPConvMap.Add("UInt16", "INT");
            _typeCPPConvMap.Add("String", "FString");
            _typeCPPConvMap.Add("Boolean", "UBOOL");
            _typeCPPConvMap.Add("SByte", "BYTE");
            _typeCPPConvMap.Add("UInt32", "INT");
            _typeCPPConvMap.Add("Int32", "INT"); 
        }

        protected string GetCPPBuiltInType(string typeName)
        {
            string CPPTypeName;

            try
            {
                CPPTypeName = _typeCPPConvMap[typeName];
            }
            catch (KeyNotFoundException ex)
            {
                Console.WriteLine("{0}", ex.Message);
                Console.WriteLine("{0} - This type don't support", typeName);
                return null;
            }

            return CPPTypeName;
        }


        public override void Build(string tableNameSpace)
        {
            if (Initialize(tableNameSpace, false) == false)
                return;
            
            UnrealGenerateRowTypeName = "F" + RowTypeName;
            
            OpenOutFile(ClassName + ".uc", false);
            BuildUCPrefix();
            BuildUCBody();
            CloseUCBuild();

            OpenOutFile(ClassName + ".cpp", false);
            _includesAtCpp.Add("SRWOGameTablePCH.h");
            _includesAtCpp.Add("SRWOGameTableClasses.h");
            _includesAtCpp.Add("BRXML.h");

            if (ThisTableType == TableType.TTYPE_SINGLEKEY)
            {
                if (this["first"].keyType == KeyType.EKEY_COMPOSIT)
                    _includesAtCpp.Add("Hashing.h");
            }
            else
            {
                foreach (KeyValuePair<string, KeyInfo> kvp in _keyInfos)
                {
                    if (kvp.Value.keyType == KeyType.EKEY_COMPOSIT)
                    {
                        _includesAtCpp.Add("Hashing.h");
                        break;
                    }

                }
            }
            
            BuildCPPPrefix();
            BuildCPPBody();
            CloseCPPBuild();
        }


    }
}
