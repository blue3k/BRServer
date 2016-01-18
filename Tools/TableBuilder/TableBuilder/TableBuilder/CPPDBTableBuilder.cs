////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2014 The Braves
// 
// Author : KyungKun Ko
//
// Description : Table CPP builder
//                  Generates .h, .CPP
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
    internal partial class CPPDBTableBuilder : TableBuilder
    {
        // Constructor
        public CPPDBTableBuilder(string basePath, string className, KeyInfo keyInfo, string baseParserName)
            : base(basePath, keyInfo, baseParserName)
        {
            ClassName = className;
        }

        public CPPDBTableBuilder(string basePath, string className, 
            KeyInfo keyInfo1, KeyInfo keyInfo2, string baseParserName)
            : base(basePath, keyInfo1, keyInfo2, baseParserName)
        {
            ClassName = className;
        }

        /// <summary>
        /// Header Prefix generation
        /// </summary>
        private void BuildHPrefix()
        {
            OutStream.WriteLine("////////////////////////////////////////////////////////////////////////////////");
            OutStream.WriteLine("// ");
            OutStream.WriteLine("// CopyRight (c) 2015 The Braves Co.");
            OutStream.WriteLine("// ");
            OutStream.WriteLine("// Author : Generated");
            OutStream.WriteLine("// ");
            OutStream.WriteLine(string.Format("// Description : {0}  definitions", ClassName));
            OutStream.WriteLine("// ");
            OutStream.WriteLine("////////////////////////////////////////////////////////////////////////////////");
            OutStream.WriteLine("");
            OutStream.WriteLine("#pragma once");
            OutStream.WriteLine("");

            foreach (string strInc in _includesAtHeader )
            {
                OutStream.WriteLine("#include \"{0}\"", strInc);
            }
            NewLine(3);


            // namespace definition
            OpenSection("namespace", Builder._tableNameSpace);

        }


        /// <summary>
        /// CPP Prefix generation
        /// </summary>
        private void BuildCPPPrefix()
        {
            OutStream.WriteLine("////////////////////////////////////////////////////////////////////////////////");
            OutStream.WriteLine("// ");
            OutStream.WriteLine("// CopyRight (c) 2015 The Braves Co.");
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

            OutStream.WriteLine(string.Format("#include \"{0}.h\"", ClassName));
            NewLine(3);

            // namespace definition
            OpenSection("namespace", Builder._tableNameSpace);
            //OpenSection("class", ClassName);

        }

        private void BuildHBody()
        {
            OpenSection("class", ClassName);
            WriteStatement("public:", -1);


            // Build type definition.
            foreach (KeyValuePair<string, string> kvp in _nameMap)
            {
                Type typeInfo = GetTypeInfo(kvp.Key);

                if (typeInfo.IsEnum)
                    DeclareEnum(kvp.Value, typeInfo);

                if (typeInfo.IsClass)
                    DeclareClass(kvp.Value, typeInfo);
            }

            // Build member data.
            DeclareMemberData(GetTypeInfo(ClassName));
            
            // Build member function.
            DeclareMemberFunction();
        }
      

        // Build CPP class implementation
        protected virtual void BuildCPPBody()
        {
            foreach (KeyValuePair<string, string> kvp in _nameMap)
            {
                Type typeInfo = GetTypeInfo(kvp.Key);

                if (typeInfo.IsEnum)
                    DefineEnumDefinition(kvp.Value, typeInfo);
            }

            //DefineLoaderElement();

            // Build type definition.
            //DefineTableParser(GetTypeInfo(ClassName));

            // Build Imp
            DefineMemberFunction();


            DefineSubElementMemberFunction();
        }


        // Close Header
        private void CloseHBuild()
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
                return BasePath + "/CPP/";

            return BasePath + "CPP/";  
        }

        protected override void BuildTypeConvMap()
        {
            _typeConvMap.Add("Int16", "signed short");
            _typeConvMap.Add("Int64", "__int64");
            _typeConvMap.Add("Single", "float");
            _typeConvMap.Add("Double", "double");
            _typeConvMap.Add("Byte", "unsigned char");
            _typeConvMap.Add("UInt64", "unsigned __int64");
            _typeConvMap.Add("UInt16", "unsigned short");
            _typeConvMap.Add("String", "std::string");
            _typeConvMap.Add("Boolean", "bool");
            _typeConvMap.Add("SByte", "char");
            _typeConvMap.Add("UInt32", "unsigned int");
            _typeConvMap.Add("Int32", "int");
            _typeConvMap.Add("nonNegativeInteger", "unsigned int");
        }

        public override void Build(string tableNameSpace)
        {
            if (Initialize(tableNameSpace, true) == false)
                return;

            OpenOutFile(ClassName + ".h", true);
            _includesAtHeader.Add("Common/Typedefs.h");
            _includesAtHeader.Add("Common/ResultCode/BRResultCodeSystem.h");
            //_includesAtHeader.Add("DB/Query.h");
            //_includesAtHeader.Add("DB/DBConst.h");
            //_includesAtHeader.Add("DB/QueryConst.h");

            BuildHPrefix();
            BuildHBody();
            CloseHBuild();


            OpenOutFile(ClassName + ".cpp", true);
            _includesAtCpp.Add("stdafx.h");
            _includesAtCpp.Add("Common/Typedefs.h");
            //_includesAtCpp.Add("DB/Query.h");
            //_includesAtCpp.Add("DB/DBConst.h");
            //_includesAtCpp.Add("DB/QueryConst.h");

            if (ThisTableType == TableType.TTYPE_SINGLEKEY)
            {
                if (this["first"].keyType == KeyType.EKEY_COMPOSIT)
                    _includesAtCpp.Add("Common/Indexing.h");
            }
            else
            {
                foreach (KeyValuePair<string, KeyInfo> kvp in _keyInfos)
                {
                    if (kvp.Value.keyType == KeyType.EKEY_COMPOSIT)
                    {
                        _includesAtCpp.Add("Common/Hashing.h");
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
