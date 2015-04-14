////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2014 The Braves Co.
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
//using System.Text;
//using System.IO;
//using System.Data;


using System.Reflection;



namespace TableBuilder
{
    /// <summary>
    /// Builder class for building interface
    /// </summary>
    internal partial class CPPTreeBuilder : TreeBuilder
    {
        // Constructor
        public CPPTreeBuilder(string basePath, string className, string baseParserName)
            : base(basePath, baseParserName)
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
            OutStream.WriteLine("// CopyRight (c) 2014 The Braves Co.");
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

            OutStream.WriteLine(string.Format("#include \"{0}.h\"", ClassName));
            NewLine(3);

            // namespace definition
            OpenSection("namespace", Builder._tableNameSpace);
            //OpenSection("class", ClassName);

        }

        private void BuildHBody()
        {
            IEnumerable<KeyValuePair<string, TreeInfo>> schemaInfo = SchemaInfo.Reverse();

            foreach (KeyValuePair<string, TreeInfo> kvp in schemaInfo)
            {
                TreeInfo treeInfo = kvp.Value;

                foreach (KeyValuePair<string, ElementInfo> kvp1 in treeInfo.ElementMap)
                {
                    ElementInfo elementInfo = kvp1.Value;

                    if (elementInfo.TypeInfo.IsEnum)
                        DeclareEnum(elementInfo.TypeName, elementInfo.TypeInfo);

                    if (elementInfo.TypeInfo.IsClass)
                        DeclareClass(elementInfo.TypeName, elementInfo.TypeInfo);
                }
            }
        }
      

        // Build CPP class implementation
        private void BuildCPPBody()
        {
            IEnumerable<KeyValuePair<string, TreeInfo>> schemaInfo = SchemaInfo.Reverse();

            foreach (KeyValuePair<string, TreeInfo> kvp in schemaInfo)
            {
                TreeInfo treeInfo = kvp.Value;

                foreach (KeyValuePair<string, ElementInfo> kvp1 in treeInfo.ElementMap)
                {
                    ElementInfo elementInfo = kvp1.Value;

                    if (elementInfo.TypeInfo.IsEnum)
                        DefineEnumDefinition(elementInfo.TypeName, elementInfo.TypeInfo);
                }
            }

            // Build type definition.
            DefineTableParser(GetTypeInfo(ClassName));

            // Define Static Data Member
            DefineMemberData(GetTypeInfo(ClassName));

            // Build Imp
            DefineMemberFunction();
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
            //_includesAtHeader.Add("string");
            //_includesAtHeader.Add("unordered_map");
            //if (KeyInformation.keyType == KeyType.EKEY_NONUNIQUE)
            //    _includesAtHeader.Add("unordered_set");
            //_includesAtHeader.Add("windows.h");
            BuildHPrefix();
            BuildHBody();
            CloseHBuild();


            OpenOutFile(ClassName + ".cpp", true);
            _includesAtCpp.Add("stdafx.h");
            _includesAtCpp.Add("Common/Typedefs.h");
            _includesAtCpp.Add("Common/BRXML.h");

            //if (ThisTableType == TableType.TTYPE_SINGLEKEY)
            //{
            //    if (this["first"].keyType == KeyType.EKEY_COMPOSIT)
            //        _includesAtCpp.Add("Common/Hashing.h");
            //}
            //else
            //{
            //    foreach (KeyValuePair<string, KeyInfo> kvp in _keyInfos)
            //    {
            //        if (kvp.Value.keyType == KeyType.EKEY_COMPOSIT)
            //        {
            //            _includesAtCpp.Add("Common/Hashing.h");
            //            break;
            //        }

            //    }
            //}

            BuildCPPPrefix();
            BuildCPPBody();
            CloseCPPBuild();
        }


        protected void DeclareVariable(PropertyInfo prop, bool BuiltInType, bool bStatic, ref HashSet<string> nameList)
        {
            string statement;
            string typeName = "";

            string varName = GetPropertyName(prop);
            varName = GetVarName(varName, ref nameList);
            
            if (BuiltInType)
            {
                typeName = GetBuiltInTypeName(prop);

                statement = string.Format("{0}\tm_{1};", typeName, varName);
            }
            else
            {
                if (prop.PropertyType.IsArray)
                {
                    typeName    = prop.PropertyType.Name.Substring(0, prop.PropertyType.Name.Length - 2);
                    statement = string.Format("std::vector<{0}*> m_{1}s;", typeName, varName);
                }
                else if (prop.PropertyType.IsEnum)
                {
                    statement = string.Format("{0}\tm_{1};", prop.PropertyType.Name, varName);
                }
                else 
                {
                    statement = string.Format("{0}*\tm_{1};", prop.PropertyType.Name, varName);
                }
            }

            if (bStatic)
                statement = "static " + statement;

            WriteStatement(statement);
        }

        protected void DeclareVariable(Type typeInfo, string name, bool isArray, bool bStatic, ref HashSet<string> nameList)
        {
            string statement;
            string varName = GetVarName(name, ref nameList);

            if (isArray)
            {
                statement = string.Format("std::vector<{0}*> m_{1}s;", typeInfo.Name, varName);
            }
            else if (typeInfo.IsEnum)
            {
                statement = string.Format("{0}\tm_{1};", typeInfo.Name, varName);
            }
            else
            {
                statement = string.Format("{0}*\tm_{1};", typeInfo.Name, varName);
            }

            if (bStatic)
                statement = "static " + statement;

            WriteStatement(statement);
        }

        protected void DefineVariable(PropertyInfo prop, bool BuiltInType, ref HashSet<string> nameList)
        {
            string statement;
            string typeName;
            string varName = GetVarName(prop.Name, ref nameList);

            if (BuiltInType)
            {
                typeName = GetBuiltInType(prop.PropertyType.Name);
                statement = string.Format("{0}\t{1}::m_{2};", typeName, ClassName, varName);
            }
            else
            {
                if (prop.PropertyType.IsArray)
                {
                    typeName = prop.PropertyType.Name.Substring(0, prop.PropertyType.Name.Length - 2);
                    statement = string.Format("std::vector<{0}*> {1}::m_{2}s;", typeName, ClassName, varName);
                }
                else if (prop.PropertyType.IsEnum)
                {
                    statement = string.Format("{0}\t{1}::m_{2};", prop.PropertyType.Name, ClassName, varName);
                }
                else
                {
                    statement = string.Format("{0}*\t{1}::m_{2};", prop.PropertyType.Name, ClassName, varName);
                }
            }

            WriteStatement(statement);
        }

        protected void DefineVariable(Type typeInfo, string name, bool isArray, ref HashSet<string> nameList)
        {
            string statement;
            string varName = GetVarName(name, ref nameList);

            if (isArray)
            {
                statement = string.Format("std::vector<{0}*> {1}::m_{2}s;", typeInfo.Name, ClassName, varName);
            }
            else if (typeInfo.IsEnum)
            {
                statement = string.Format("{0}\t{1}::m_{2};", typeInfo.Name, ClassName, varName);
            }
            else
            {
                statement = string.Format("{0}*\t{1}::m_{2};", typeInfo.Name, ClassName, varName);
            }

            WriteStatement(statement);
        }

        protected void ReleaseVariable(Type typeInfo, string name, ref HashSet<string> nameList)
        {
            string statement;
            string varName = GetVarName(name, ref nameList);

            if (typeInfo.IsArray)
            {
                string typeName = typeInfo.Name.Substring(0, typeInfo.Name.Length - 2);

                statement = string.Format("for (std::vector<{0}*>::iterator itr = m_{1}s.begin(); itr != m_{1}s.end(); ++itr)",
                typeName, varName);
                WriteStatement(statement);
                OpenSection();
                WriteStatement("if (*itr != NULL)");
                WriteStatement("delete *itr;", 1);
                CloseSection();

                statement = string.Format("m_{0}s.clear();", varName);
                WriteStatement(statement);
            }
            else
            {
                statement = string.Format("if (m_{0} != NULL)", varName);
                WriteStatement(statement);
                OpenSection();
                statement = string.Format("delete m_{0};", varName);
                WriteStatement(statement);
                statement = string.Format("m_{0} = NULL;", varName);
                WriteStatement(statement);
                CloseSection();
            }
        }

        protected void ReleaseVariable(Type typeInfo, string name, bool IsArray, ref HashSet<string> nameList)
        {
            string statement;
            string varName = GetVarName(name, ref nameList);

            if (IsArray)
            {
                string typeName = typeInfo.Name;

                statement = string.Format("for (std::vector<{0}*>::iterator itr = m_{1}s.begin(); itr != m_{1}s.end(); ++itr)",
                typeName, varName);
                WriteStatement(statement);
                OpenSection();
                WriteStatement("if (*itr != NULL)");
                WriteStatement("delete *itr;", 1);
                CloseSection();

                NewLine(1);
                statement = string.Format("m_{0}s.clear();", varName);
                WriteStatement(statement);
            }
            else
            {
                statement = string.Format("if (m_{0} != NULL)", varName);
                WriteStatement(statement);
                OpenSection();
                statement = string.Format("delete m_{0};", varName);
                WriteStatement(statement);

                NewLine(1);
                statement = string.Format("m_{0} = NULL;", varName);
                WriteStatement(statement);
                CloseSection();
            }
        }
    }
}
