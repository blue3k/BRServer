using System;
using System.Collections.Generic;
using System.Linq;

namespace TableBuilder
{
    internal partial class CPPTreeBuilder
    {   
        protected override void DefineTableParser(Type rootTypeInfo)
        {
            NewLine(1);
            string statement;
            HashSet<string> attributeHashSet    = new HashSet<string>();
            List<string> elementPathList        = new List<string>();
            
            // Get Reverse Enumerable!
            IEnumerable<KeyValuePair<string, TreeInfo>> schemaInfo = SchemaInfo.Reverse();

            // Start - Parser class definition
            NewLine(1);
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

            foreach (KeyValuePair<string, TreeInfo> kvp in schemaInfo)
            {
                TreeInfo treeInfo = kvp.Value;

                foreach (KeyValuePair<string, ElementInfo> kvp1 in treeInfo.ElementMap)
                {
                    ElementInfo elementInfo = kvp1.Value;

                    if (elementInfo.TypeName == ClassName)
                        continue;

                    statement = string.Format("{1}* m_p{1};", ClassName, elementInfo.TypeName);
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
            statement = string.Format("{0}()", ParserName);
            WriteStatement(statement);
            OpenSection();

            foreach (string elementName in ElementNameSet)
            {
                statement = string.Format("AddElementHandler( \"{0}\", reinterpret_cast<ElementStartHandler>(&{1}::Start{0}), reinterpret_cast<ElementEndHandler>(&{1}::End{0}) );",
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
                foreach (string key in elementPathList)
                {
                    NewLine(1);
                    string elementPath = key.Replace("/", "");
                    ElementInfo elementInfo = ElementStructure[key];

                    statement = string.Format("if (m_ElementPath == \"{0}\")", elementPath);
                    WriteStatement(statement);
                    OpenSection();

                    // memory allocation!
                    if (elementInfo.TypeName != ClassName)
                    {
                        statement = string.Format("m_p{0} = new {0};", elementInfo.TypeName);
                        WriteStatement(statement);
                    }
                    CloseSection();
                }

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
                                word = string.Format("{0}::m_{1}", ClassName, prop.Name);
                            else
                                word = string.Format("m_p{0}->m_{1}", elementInfo.TypeName, prop.Name);

                            if (typeName == "unsigned int" ||
                                typeName == "int")
                            {
                                statement = word + " = atoi( value.c_str() );";
                                WriteStatement(statement);
                            }
                            else if (typeName == "std::string")
                            {
                                statement = word + " = value;";
                                WriteStatement(statement);
                            }
                            else if (typeName == "float" ||
                                    typeName == "double")
                            {
                                statement = word + " = (float)atof(value.c_str() );";
                                WriteStatement(statement);
                            }
                            else if (typeName == "bool")
                            {
                                statement = word + " = (value == \"true\");";
                                WriteStatement(statement);
                            }
                            else if (typeName == "char")
                            {
                                statement = word + " = (char)atoi( value.c_str() );";
                                WriteStatement(statement);
                            }
                            else if (typeInfo.IsEnum)
                            {
                                statement = word + " = value.c_str();";
                                WriteStatement(statement);

                                statement = string.Format("if ({0} == \"INVALID_ENUM\")", word);
                                WriteStatement(statement);

                                statement = string.Format("Error(\"GameTable: Invalid enum value(%s-%s-value:%s)!\", \"m_p{0}\", \"{1}\", value.c_str());", ClassName, prop.Name);
                                WriteStatement(statement, 1);
                            }
                            else
                            {
                                statement = "-------------- Error type not supported --------------------";
                                WriteStatement(statement);
                            }

                            WriteStatement("break;");
                        }

                        WriteStatement("default:", -1);
                        WriteStatement("Error(\"Invalid AttributeID!-(%s)\", attName);");
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

                    statement = string.Format("if (m_p{0})", elementInfo.TypeName);
                    WriteStatement(statement);

                    OpenSection();

                    if (elementInfo.ParentTypeName == ClassName)
                    {
                        if (IsArray(GetTypeInfo(ClassName), elementInfo.TypeName) || IsSwitchType(elementInfo.ParentTypeName, elementInfo.ElementName))
                            statement = string.Format("{0}::m_{1}s.push_back(m_p{2});", ClassName, GetVarName(ClassName, elementInfo.ElementName, ref _varNameSet), elementInfo.TypeName);
                        else
                            statement = string.Format("{0}::m_{1} = m_p{2};", ClassName, GetVarName(ClassName, elementInfo.ElementName, ref _varNameSet), elementInfo.TypeName);
                    }
                    //else if (IsComplexRoot(elementInfo.TypeName))
                    //{
                    //    string complexVariable = MakeComplexVariable(key);

                    //    statement = string.Format("{0} = m_p{1};", complexVariable, elementInfo.TypeName);
                    //}
                    else
                    {
                        if (IsArray(GetTypeInfo(ClassName), elementInfo.TypeName) || IsSwitchType(elementInfo.ParentTypeName, elementInfo.ElementName))
                            statement = string.Format("m_p{0}->m_{1}s.push_back(m_p{2});",
                                GetComplexTypeName(elementInfo.ParentTypeName), GetVarName(elementInfo.ParentTypeName, elementInfo.ElementName, ref _varNameSet), elementInfo.TypeName);
                        else
                            statement = string.Format("m_p{0}->m_{1} = m_p{2};",
                                GetComplexTypeName(elementInfo.ParentTypeName), GetVarName(elementInfo.ParentTypeName, elementInfo.ElementName, ref _varNameSet), elementInfo.TypeName);
                    }
                    WriteStatement(statement);

                    statement = string.Format("m_p{0} = NULL;", elementInfo.TypeName);
                    WriteStatement(statement);
                    CloseSection();
                        
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
            WriteStatement("BR::Result LoadTable( const char *strFileName )");
            OpenSection();
            WriteStatement("int result = xmlSAXUserParseFile( *this, this, strFileName );");
            NewLine(1);
            WriteStatement("if (result != 0)");
            OpenSection();
            WriteStatement("// error log");
            WriteStatement("return BR::ResultCode::FAIL;");
            CloseSection();
            WriteStatement("xmlCleanupParser();");
            WriteStatement("return BR::ResultCode::SUCCESS;");
            CloseSection();
            // Define parser member function end


            CloseSection();
            // Parser class definition end
        }

        protected override void DefineEnumDefinition(string name, Type typeInfo)
        {
            NewLine(1);
            string statement;
            statement = string.Format("// {0} structure defition", name);
            WriteStatement(statement);

            // static data definition
            statement = string.Format("{0}::{1}::EnumMap {0}::{1}::m_Map;", ClassName, name,
                ClassName, name);
            WriteStatement(statement);

            // Initialize function
            NewLine(1);
            statement = string.Format("{0}::{1}::EnumMap::EnumMap()", ClassName, name);
            WriteStatement(statement);
            OpenSection();
            foreach (string strEnum in Enum.GetNames(typeInfo))
            {
                statement = string.Format("m_Map.insert(std::make_pair(\"{0}\", {1}));", strEnum, strEnum);
                WriteStatement(statement);
            }
            CloseSection();

            // operator =(int i)
            NewLine(1);
            statement = string.Format("{0}::{1}::Enum {2}::{3}::operator =(int i)", ClassName, name, ClassName, name);
            WriteStatement(statement);
            OpenSection();
            WriteStatement("m_Enum = AsEnum(i);");
            WriteStatement("return m_Enum;");
            CloseSection();

            // operator int () const
            NewLine(1);
            statement = string.Format("{0}::{1}::operator int () const", ClassName, name);
            WriteStatement(statement);
            OpenSection();
            WriteStatement("return m_Enum;");
            CloseSection();

            // operator =(const char* sz)
            NewLine(1);
            statement = string.Format("{0}::{1}::Enum {2}::{3}::operator =(const char* sz)", ClassName, name, ClassName, name);
            WriteStatement(statement);
            OpenSection();
            WriteStatement("m_Enum = AsEnum(sz);");
            WriteStatement("return m_Enum;");
            CloseSection();

            // operator ==(const char* sz)
            NewLine(1);
            statement = string.Format("bool {0}::{1}::operator ==(const char* sz)", ClassName, name);
            WriteStatement(statement);
            OpenSection();
            WriteStatement("return m_Enum == AsEnum(sz);");
            CloseSection();

            // operator const char* () const
            NewLine(1);
            statement = string.Format("{0}::{1}::operator const char* () const", ClassName, name);
            WriteStatement(statement);
            OpenSection();
            WriteStatement("return AsName(m_Enum);");
            CloseSection();

            // AsEnum(const char* sz)
            NewLine(1);
            statement = string.Format("{0}::{1}::Enum {2}::{3}::AsEnum(const char* sz)", ClassName, name, ClassName, name);
            WriteStatement(statement);
            OpenSection();
            WriteStatement("EnumMapItr itr =  m_Map.find(sz);");
            WriteStatement("if (itr == m_Map.end())");
            WriteStatement("return INVALID_ENUM;", 1);
            NewLine(1);
            WriteStatement("return itr->second;");
            CloseSection();

            // AsEnum(int e)
            NewLine(1);
            statement = string.Format("{0}::{1}::Enum {2}::{3}::AsEnum(int e)", ClassName, name, ClassName, name);
            WriteStatement(statement);
            OpenSection();
            WriteStatement("for (EnumMapItr itr = m_Map.begin(); itr != m_Map.end(); ++itr)");
            OpenSection();
            WriteStatement("if (itr->second == e)");
            WriteStatement("return itr->second;", 1);
            CloseSection();
            WriteStatement("return INVALID_ENUM;");
            CloseSection();

            // AsName(int e)
            NewLine(1);
            statement = string.Format("const char* {0}::{1}::AsName(int e)", ClassName, name);
            WriteStatement(statement);
            OpenSection();
            WriteStatement("for (EnumMapItr itr = m_Map.begin(); itr != m_Map.end(); ++itr)");
            OpenSection();
            WriteStatement("if (itr->second == e)");
            WriteStatement("return itr->first.c_str();", 1);
            CloseSection();
            WriteStatement("return \"INVALID_ENUM\";");
            CloseSection();
        }

        protected override void DefineMemberFunction()
        {
            NewLine(1);
            string statement;
            
            // Start - LoadTable
            NewLine(1);
            statement = string.Format("BR::Result {0}::LoadTable( const char *strFileName )", ClassName);
            WriteStatement(statement);
            OpenSection();
            statement = string.Format("{0} parser;", ParserName);
            WriteStatement(statement);
            WriteStatement("if (FAILED(parser.LoadTable(strFileName)))");
            WriteStatement("return BR::ResultCode::FAIL;", 1);
            NewLine(1);
            WriteStatement("return BR::ResultCode::SUCCESS;");
            CloseSection();
            // End - LoadTable
            

            // Start - ClearTable
            NewLine(1);
            statement = string.Format("BR::Result {0}::ClearTable()", ClassName);
            WriteStatement(statement);
            OpenSection();

            Type typeInfo = GetTypeInfo(ClassName);

            foreach (System.Reflection.PropertyInfo prop in typeInfo.GetProperties())
            {
                if ((prop.PropertyType.IsEnum || prop.PropertyType.IsClass) &&
                    prop.PropertyType != "".GetType())
                {
                    NewLine(1);
                    // Support Choice!
                    if (prop.PropertyType.Name.StartsWith("Object"))
                    {
                        foreach (object attribute in prop.GetCustomAttributes(true))
                        {
                            NewLine(1);
                            System.Xml.Serialization.XmlArrayItemAttribute arrayItemAttribute
                                = attribute as System.Xml.Serialization.XmlArrayItemAttribute;

                            if (arrayItemAttribute != null)
                            {
                                statement = string.Format("for (std::vector<{0}*>::iterator itr = m_{1}s.begin(); itr != m_{1}s.end(); ++itr)",
                                    arrayItemAttribute.Type.Name, arrayItemAttribute.ElementName);
                                WriteStatement(statement);
                                WriteStatement("delete *itr;", 1);

                                statement = string.Format("m_{0}s.clear();", arrayItemAttribute.ElementName);
                                WriteStatement(statement);
                            }

                            System.Xml.Serialization.XmlElementAttribute elementAttribute
                                = attribute as System.Xml.Serialization.XmlElementAttribute;

                            if (elementAttribute != null)
                            {
                                statement = string.Format("for (std::vector<{0}*>::iterator itr = m_{1}s.begin(); itr != m_{1}s.end(); ++itr)",
                                    elementAttribute.Type.Name, elementAttribute.ElementName);
                                WriteStatement(statement);
                                WriteStatement("delete *itr;", 1);

                                statement = string.Format("m_{0}s.clear();", elementAttribute.ElementName);
                                WriteStatement(statement);
                            }
                        }
                    }
                    else if (prop.PropertyType.IsArray)
                    {
                        string typeName = prop.PropertyType.Name.Substring(0, prop.PropertyType.Name.Length - 2);
                        statement = string.Format("for (std::vector<{0}*>::iterator itr = m_{1}s.begin(); itr != m_{1}s.end(); ++itr)",
                                    typeName, prop.Name);
                        WriteStatement(statement);
                        WriteStatement("delete *itr;", 1);

                        statement = string.Format("m_{0}s.clear();", prop.Name);
                        WriteStatement(statement);
                    }
                    else
                    {
                        statement = string.Format("if (m_{0})", prop.Name);
                        WriteStatement(statement);
                        OpenSection();
                        statement = string.Format("delete m_{0};", prop.Name);
                        WriteStatement(statement);
                        statement = string.Format("m_{0} = NULL;", prop.Name);
                        WriteStatement(statement);
                        CloseSection();
                    }
                }
            }


            WriteStatement("return BR::ResultCode::SUCCESS;");
            CloseSection();
            // End - ClearTable
        }


        protected void DefineMemberData(Type typeInfo)
        {
            NewLine(1);

            _varNameSet.Clear();
            foreach (System.Reflection.PropertyInfo prop in typeInfo.GetProperties())
            {
                // Support Choice!
                if (prop.PropertyType.Name.StartsWith("Object"))
                {
                    foreach (object attribute in prop.GetCustomAttributes(true))
                    {
                        System.Xml.Serialization.XmlArrayItemAttribute arrayItemAttribute
                            = attribute as System.Xml.Serialization.XmlArrayItemAttribute;

                        if (arrayItemAttribute != null)
                        {
                            DefineVariable(arrayItemAttribute.Type, arrayItemAttribute.ElementName, true, ref _varNameSet);
                        }

                        System.Xml.Serialization.XmlElementAttribute elementAttribute
                           = attribute as System.Xml.Serialization.XmlElementAttribute;

                        if (elementAttribute != null)
                        {
                            DefineVariable(elementAttribute.Type, elementAttribute.ElementName, true, ref _varNameSet);
                        }
                    }
                }
                else if ((prop.PropertyType.IsEnum || prop.PropertyType.IsClass) &&
                    prop.PropertyType != "".GetType())
                {
                    DefineVariable(prop, false, ref _varNameSet);
                }
                else
                {
                    DefineVariable(prop, true, ref _varNameSet);
                }
            }
        }
    }
}