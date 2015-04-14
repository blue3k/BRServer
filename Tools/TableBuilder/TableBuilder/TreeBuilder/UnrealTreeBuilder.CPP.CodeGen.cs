using System;
using System.Collections.Generic;
using System.Linq;

namespace TableBuilder
{
    internal partial class UnrealTreeBuilder
    {
        protected override void DefineTableParser(Type rootTypeInfo)
        {
            NewLine(1);
            string statement;
            HashSet<string> attributeHashSet    = new HashSet<string>();
            List<string> elementPathList = new List<string>();

            // Get Reverse Enumerable!
            IEnumerable<KeyValuePair<string, TreeInfo>> schemaInfo = SchemaInfo.Reverse();

            // Start - Parser class definition
            ParserName = string.Format("{0}Parser", ClassName);
            statement = string.Format("// {0} decl/impl", ParserName);
            WriteStatement(statement);

            statement = string.Format("{0} : public BR::{1}", ParserName, BaseParserName);
            OpenSection("class", statement);
            WriteStatement("public:", -1);
            
            // Build Attribute Hash
            BuildAttributeHashSet(ref attributeHashSet);


            // Start - Define property enum
            statement = string.Format("ATT_ID_{0}", ClassName);
            OpenSection("enum", statement);

            foreach (string attributeName in attributeHashSet)
            {
                statement = string.Format("EATT_{0},", attributeName);
                WriteStatement(statement);
            }
            CloseSection();
            // End - Define property enum

            // Start - Define member data
            NewLine(1);
            WriteStatement("private:", -1);

            statement = string.Format("U{0}*\t\tm_p{0};", ClassName);
            WriteStatement(statement);

            foreach (KeyValuePair<string, TreeInfo> kvp in schemaInfo)
            {
                TreeInfo treeInfo = kvp.Value;

                foreach (KeyValuePair<string, ElementInfo> kvp1 in treeInfo.ElementMap)
                {
                    ElementInfo elementInfo = kvp1.Value;


                    if (elementInfo.TypeName == ClassName)
                        continue;

                    statement = string.Format("F{0}\t\tm_{0};", elementInfo.TypeName);
                    WriteStatement(statement);
                } 
            }
            // End - Define member data

            // Define m_ElementPath
            NewLine(1);
            WriteStatement("std::string m_ElementPath;");

            // Start - Define parser constructor 
            // Start - Add Handler
            NewLine(1);
            WriteStatement("public:", -1);
            statement = string.Format("{0}({1}* {2}) : {3}({2})", ParserName, UnrealGenerateClassName,
                   "p" + ClassName, "m_p" + ClassName);

            WriteStatement(statement);
            OpenSection();

            foreach (string elementName in ElementNameSet)
            {
                statement = string.Format("AddElementHandler( \"{0}\", ElementStartHandler(&{1}::Start{0}), ElementEndHandler(&{1}::End{0}) );",
                                elementName, ParserName);
                WriteStatement(statement);
            }
            // End - Add Handler

            // Start - Add AttributeID
            NewLine(1);
            foreach (string attributeName in attributeHashSet)
            {
                statement = string.Format("AddAttributeID( \"{0}\", EATT_{0} );", attributeName);
                WriteStatement(statement);
            }
            // End - Add AttributeID
            
            // Start - Initialize Data Member!
            NewLine(1);
            foreach (KeyValuePair<string, TreeInfo> kvp in schemaInfo)
            {
                TreeInfo treeInfo = kvp.Value;

                foreach (KeyValuePair<string, ElementInfo> kvp1 in treeInfo.ElementMap)
                {
                    ElementInfo elementInfo = kvp1.Value;

                    if (elementInfo.TypeName == ClassName)
                        continue;

                    statement = string.Format("appMemzero(&m_{0}, sizeof(F{0}));", elementInfo.TypeName);
                    WriteStatement(statement);
                }
            }
            // End - Initialize Data Member!
            
            CloseSection();
            // Define parser constructor end


            // Define element handler start
            NewLine(1);
            foreach (string elementName in ElementNameSet)
            {
                NewLine(1);
                statement = string.Format("void Start{0}(", elementName);
                WriteStatement(statement);
                WriteStatement("const xmlChar * localname,", 1);
                WriteStatement("const xmlChar * prefix,", 1);
                WriteStatement("const xmlChar * URI,", 1);
                WriteStatement("int nb_namespaces,", 1);
                WriteStatement("const xmlChar ** namespaces,", 1);
                WriteStatement("int nb_attributes,", 1);
                WriteStatement("int nb_defaulted,", 1);
                WriteStatement("const xmlChar ** attributes )", 1);
                OpenSection();

                //Assign m_ElementPath
                statement = string.Format("m_ElementPath += \"{0}\";", elementName);
                WriteStatement(statement);

                // Get elementPath List!
                elementPathList.Clear();
                GetElementPathList(elementName, ref elementPathList);

                NewLine(1);
                WriteStatement("unsigned int index = 0;");
                WriteStatement("for ( int indexAttribute = 0;");
                WriteStatement("indexAttribute < nb_attributes;", 1);
                WriteStatement("++indexAttribute, index += 5 )", 1);
                OpenSection();
                WriteStatement("const xmlChar *attName = attributes[index];");
                WriteStatement("const xmlChar *prefix = attributes[index+1];");
                WriteStatement("const xmlChar *nsURI = attributes[index+2];");
                WriteStatement("const xmlChar *valueBegin = attributes[index+3];");
                WriteStatement("const xmlChar *valueEnd = attributes[index+4];");
                WriteStatement("std::string value( (const char *)valueBegin, (const char *)valueEnd );");
                
                NewLine(1);
                foreach (string key in elementPathList)
                {
                    NewLine(1);
                    string elementPath      = key.Replace("/", "");
                    ElementInfo elementInfo = ElementStructure[key];

                    statement = string.Format("if (m_ElementPath == \"{0}\")", elementPath);
                    WriteStatement(statement);
                    OpenSection();

                    if (HasAttribute(elementInfo))
                    {
                        NewLine(1);
                        WriteStatement("switch( GetAttributeID((char*)attName) )");
                        OpenSection();

                        foreach (System.Reflection.PropertyInfo prop in elementInfo.TypeInfo.GetProperties())
                        {
                            if ((prop.PropertyType.IsClass && prop.PropertyType != "".GetType()) ||
                                HasIgnoreAttribute(prop))
                                continue;

                            statement = string.Format("case EATT_{0}:", prop.Name);
                            WriteStatement(statement, -1);

                            string typeName = GetBuiltInTypeName(prop);

                            Type typeInfo = prop.PropertyType;
                            string word;
                            if (elementInfo.ParentTypeName == "")
                                word = string.Format("m_p{0}->{1}", ClassName, prop.Name);
                            else
                                word = string.Format("m_{0}.{1}", elementInfo.TypeName, prop.Name);

                            if (typeName == "int")
                            {
                                //statement = string.Format("m_p{0}->{1} = atoi( value.c_str() );", elementInfo.TypeName, prop.Name);
                                statement = word + " = atoi( value.c_str() );";
                                WriteStatement(statement);
                            }
                            else if (typeName == "string")
                            {
                                //statement = string.Format("m_p{0}->{1} = value;", elementInfo.TypeName, prop.Name);
                                statement = word + " = UTF8_TO_TCHAR(value.c_str());";
                                WriteStatement(statement);
                            }
                            else if (typeName == "float")
                            {
                                //statement = string.Format("m_p{0}->{1} = atof(value.c_str() );", elementInfo.TypeName, prop.Name);
                                statement = word + " = (float)atof(value.c_str() );";
                                WriteStatement(statement);
                            }
                            else if (typeName == "bool")
                            {
                                //statement = string.Format("m_p{0}->{1} = (value == \"true\");", elementInfo.TypeName, prop.Name);
                                statement = word + " = (value == \"true\");";
                                WriteStatement(statement);
                            }
                            else if (typeName == "byte")
                            {
                                //statement = string.Format("m_p{0}->{1} = (char)atoi( value.c_str() );", elementInfo.TypeName, prop.Name);
                                statement = word + " = (char)atoi( value.c_str() );";
                                WriteStatement(statement);
                            }
                            else if (typeInfo.IsEnum)
                            {
                                statement = string.Format("{0}Class eVar;", GetTypeName(typeInfo.Name));
                                WriteStatement(statement);

                                statement = string.Format("eVar = value.c_str();");
                                WriteStatement(statement);

                                WriteStatement("if (!eVar.IsValid())");
                                statement = string.Format("Error(\"GameTable: Invalid enum value(%s-%s-value:%s)!\", \"{0}\", \"{1}\", value.c_str());", ClassName, prop.Name);
                                WriteStatement(statement, 1);

                                statement = word + " = eVar;";
                                WriteStatement(statement);
                            }
                            else
                            {
                                statement = "-------------- Error type not supported --------------------";
                                WriteStatement(statement);
                            }
                            
                            WriteStatement("break;");
                        }

                        WriteStatement("default:", -1);
                        WriteStatement("Error(\"Invalid AttributeID!\");");
                        WriteStatement("break;");
                
                        CloseSection();
                    }

                    CloseSection();
                }

                CloseSection();
                CloseSection();

                NewLine(1);
                statement = string.Format("void End{0}( ", elementName);
                WriteStatement(statement);
                WriteStatement("const xmlChar * localname,", 1);
                WriteStatement("const xmlChar * prefix,", 1);
                WriteStatement("const xmlChar * URI )", 1);
                OpenSection();

                _varNameSet.Clear();
                foreach (string key in elementPathList)
                {
                    string elementPath = key.Replace("/", "");

                    ElementInfo elementInfo = ElementStructure[key];

                    if (elementInfo.TypeName == ClassName)
                        continue;

                    statement = string.Format("if (m_ElementPath == \"{0}\")", elementPath);
                    WriteStatement(statement);

                    OpenSection();

                    // Insert row to table!
                    if (elementInfo.ParentTypeName == ClassName)
                    {
                        if (IsArray(GetTypeInfo(ClassName), elementInfo.TypeName) || IsSwitchType(elementInfo.ParentTypeName, elementInfo.ElementName))
                        {
                            statement = string.Format("m_p{0}->{1}s.AddItem(m_{2});", ClassName, GetVarName(ClassName, elementInfo.ElementName, ref _varNameSet),
                                elementInfo.TypeName);
                            WriteStatement(statement);
                            statement = string.Format("appMemzero(&m_{0}, sizeof(F{0}));", elementInfo.TypeName);
                            WriteStatement(statement);
                        }
                        else
                        {
                            statement = string.Format("m_p{0}->{1} = m_{2};", ClassName, GetVarName(ClassName, elementInfo.ElementName, ref _varNameSet),
                                    elementInfo.TypeName);
                            WriteStatement(statement);
                            statement = string.Format("appMemzero(&m_{0}, sizeof(F{0}));", elementInfo.TypeName);
                            WriteStatement(statement);
                        }
                    }
                    //else if (IsComplexRoot(elementInfo.TypeName))
                    //{
                    //    Console.WriteLine("Test");
                    //}
                    else
                    {
                        if (IsArray(GetTypeInfo(ClassName), elementInfo.TypeName) || IsSwitchType(elementInfo.ParentTypeName, elementInfo.ElementName))
                        {
                            //ElementInfo parentElementInfo = GetElementInfoByTree(elementInfo.ParentTypeName);
                            statement = string.Format("m_{0}.{1}s.AddItem(m_{2});",
                                GetComplexTypeName(elementInfo.ParentTypeName), 
                                GetVarName(elementInfo.ParentTypeName, elementInfo.ElementName, ref _varNameSet), elementInfo.TypeName);
                            WriteStatement(statement);
                            statement = string.Format("appMemzero(&m_{0}, sizeof(F{0}));", elementInfo.TypeName);
                            WriteStatement(statement);
                        }
                        else
                        {
                            //ElementInfo parentElementInfo = GetElementInfoByTree(elementInfo.ParentTypeName);
                            statement = string.Format("m_{0}.{1} = m_{2};", 
                                GetComplexTypeName(elementInfo.ParentTypeName), 
                                GetVarName(elementInfo.ParentTypeName, elementInfo.ElementName, ref _varNameSet), elementInfo.TypeName);
                            WriteStatement(statement);
                            statement = string.Format("appMemzero(&m_{0}, sizeof(F{0}));", elementInfo.TypeName);
                            WriteStatement(statement);
                        }
                    }

                    CloseSection();
                }

                NewLine(1);
                statement = string.Format("std::string strElementName = \"{0}\";", elementName);
                WriteStatement(statement);
                WriteStatement("int iSize = m_ElementPath.size() - strElementName.size();");
                WriteStatement("m_ElementPath = m_ElementPath.substr(0, iSize);");

                CloseSection();
            }

            // Define parser member function start
            NewLine(1);
            WriteStatement("bool LoadTable( const char *strFileName )");
            OpenSection();
            WriteStatement("int result = xmlSAXUserParseFile( *this, this, strFileName );");
            NewLine(1);
            WriteStatement("if (result != 0)");
            OpenSection();
            WriteStatement("// error log");
            WriteStatement("return false;");
            CloseSection();
            WriteStatement("xmlCleanupParser();");
            WriteStatement("return true;");
            CloseSection();

            CloseSection();
            // Define parser class end
        }

        protected override void DefineEnumDefinition(string name, Type typeInfo)
        {
            string enumClassName = name + "Class";

            string statement;
            statement = string.Format("// {0} enum class defition", enumClassName);
            WriteStatement(statement);
            OpenSection("class", enumClassName);

            // Declare enum class method
            statement = string.Format("EnumMap : public std::tr1::unordered_map<std::string, {0}>", name);
            OpenSection("class", statement);
            WriteStatement("public:", -1);
            WriteStatement("EnumMap();");
            CloseSection();
            NewLine(1);
            WriteStatement("typedef EnumMap::iterator EnumMapItr;", 0);
            NewLine(1);
            WriteStatement("public:", -1);
            statement = string.Format("{0} operator=(int i);", name);
            WriteStatement(statement);
            WriteStatement("operator int () const;", 0);
            NewLine(1);
            statement = string.Format("{0} operator=(const char* sz);", name);
            WriteStatement(statement);
            WriteStatement("bool operator==(const char* sz);", 0);
            WriteStatement("operator const char* () const;", 0);
            NewLine(1);
            statement = string.Format("static {0} AsEnum(const char* sz);", name);
            WriteStatement(statement);
            statement = string.Format("static {0} AsEnum(int e);", name);
            WriteStatement(statement);
            WriteStatement("static const char* AsName(int e);", 0);
            WriteStatement("bool IsValid();");

            // Declare enum class member
            NewLine(1);
            WriteStatement("private:", -1);

            statement = string.Format("{0} m_Enum;", name);
            WriteStatement(statement);
            WriteStatement("static EnumMap m_Map;");

            CloseSection();



            NewLine(1);
            statement = string.Format("// {0} structure defition", enumClassName);
            WriteStatement(statement);

            // static data definition
            statement = string.Format("{0}::EnumMap {0}::m_Map;", enumClassName);
            WriteStatement(statement);

            // Initialize function
            NewLine(1);
            statement = string.Format("{0}::EnumMap::EnumMap()", enumClassName);
            WriteStatement(statement);
            OpenSection();
            foreach (string strEnum in Enum.GetNames(typeInfo))
            {
                statement = string.Format("m_Map.insert(std::make_pair(\"{0}\", {1}));", name + "_" + strEnum, name + "_" + strEnum);
                WriteStatement(statement);
            }
            CloseSection();

            // operator =(int i)
            NewLine(1);
            statement = string.Format("{0} {1}::operator =(int i)", name, enumClassName);
            WriteStatement(statement);
            OpenSection();
            WriteStatement("m_Enum = AsEnum(i);");
            WriteStatement("return m_Enum;");
            CloseSection();

            // operator int () const
            NewLine(1);
            statement = string.Format("{0}::operator int () const", enumClassName);
            WriteStatement(statement);
            OpenSection();
            WriteStatement("return m_Enum;");
            CloseSection();

            // operator =(const char* sz)
            NewLine(1);
            statement = string.Format("{0} {1}::operator =(const char* sz)", name, enumClassName);
            WriteStatement(statement);
            OpenSection();
            WriteStatement("m_Enum = AsEnum(sz);");
            WriteStatement("return m_Enum;");
            CloseSection();

            // operator ==(const char* sz)
            NewLine(1);
            statement = string.Format("bool {0}::operator ==(const char* sz)", enumClassName);
            WriteStatement(statement);
            OpenSection();
            WriteStatement("return m_Enum == AsEnum(sz);");
            CloseSection();

            // operator const char* () const
            NewLine(1);
            statement = string.Format("{0}::operator const char* () const", enumClassName);
            WriteStatement(statement);
            OpenSection();
            WriteStatement("return AsName(m_Enum);");
            CloseSection();

            // AsEnum(const char* sz)
            NewLine(1);
            statement = string.Format("{0} {1}::AsEnum(const char* sz)", name, enumClassName);
            WriteStatement(statement);
            OpenSection();
            statement = string.Format("std::string strEnum = \"{0}\";", name);
            WriteStatement(statement);
            WriteStatement("strEnum += \"_\";");
            WriteStatement("strEnum += sz;");
            WriteStatement("EnumMapItr itr =  m_Map.find(strEnum);");
            WriteStatement("if (itr == m_Map.end())");
            statement = string.Format("return {0}_MAX;", name);
            WriteStatement(statement, 1);
            NewLine(1);
            WriteStatement("return itr->second;");
            CloseSection();

            // AsEnum(int e)
            NewLine(1);
            statement = string.Format("{0} {1}::AsEnum(int e)", name, enumClassName);
            WriteStatement(statement);
            OpenSection();
            WriteStatement("for (EnumMapItr itr = m_Map.begin(); itr != m_Map.end(); ++itr)");
            OpenSection();
            WriteStatement("if (itr->second == e)");
            WriteStatement("return itr->second;", 1);
            CloseSection();
            statement = string.Format("return {0}_MAX;", name);
            WriteStatement(statement);
            CloseSection();

            // AsName(int e)
            NewLine(1);
            statement = string.Format("const char* {0}::AsName(int e)", enumClassName);
            WriteStatement(statement);
            OpenSection();
            WriteStatement("for (EnumMapItr itr = m_Map.begin(); itr != m_Map.end(); ++itr)");
            OpenSection();
            WriteStatement("if (itr->second == e)");
            WriteStatement("return itr->first.c_str();", 1);
            CloseSection();
            statement = string.Format("return \"{0}_MAX\";", name);
            WriteStatement(statement);
            CloseSection();

            // IsValid()
            NewLine(1);
            statement = string.Format("bool {0}::IsValid()", enumClassName);
            WriteStatement(statement);
            OpenSection();
            statement = string.Format("if (m_Enum == {0}_MAX)", name);
            WriteStatement(statement);
            WriteStatement("return false;", 1);
            NewLine(1);
            WriteStatement("return true;");
            CloseSection();
        }

        protected override void DefineMemberFunction()
        {
            NewLine(1);
            string statement;
            
            // LoadTable
            statement = string.Format("UBOOL {0}::LoadTable(const FString& strFileName)", UnrealGenerateClassName);
            WriteStatement(statement);
            OpenSection();
            WriteStatement("FString strFilePath = appGameDir()+TEXT(\"\\\\\") + strFileName;");
            NewLine(1);
            statement = string.Format("{0} parser(this);", ParserName);
            WriteStatement(statement);
            WriteStatement("if (!parser.LoadTable(TCHAR_TO_ANSI(*strFilePath)))");
            OpenSection();
            WriteStatement("// error log");
            WriteStatement("return FALSE;");
            CloseSection();
            WriteStatement("return TRUE;");
            CloseSection();
            NewLine(1);

            //if (ThisTableType == TableType.TTYPE_SINGLEKEY)
            //{
            //    WriteClearTable(this["first"]);
            //    NewLine(1);

            //    WriteFindItem(this["first"]);
            //    NewLine(1);

            //    WriteAllRows(this["first"]);
            //}
            //else
            //{
            //    foreach (KeyValuePair<string, KeyInfo> kvp in _keyInfos)
            //    {
            //        // ClearTable
            //        if (kvp.Value == _keyInfos["first"])
            //            WriteMultiKeyClearTable(kvp.Value, true);
            //        else
            //            WriteMultiKeyClearTable(kvp.Value, false);

            //        NewLine(1);

            //        // FindItem
            //        WriteMultiKeyFindItem(kvp.Value);
            //        NewLine(1);

            //        // AllRows
            //        WriteMultiKeyAllRows(kvp.Value);
            //    }

            //    // MultiKeyClearTable
            //    WriteMultiKeyClearTable();
            //}
        }
    }
}