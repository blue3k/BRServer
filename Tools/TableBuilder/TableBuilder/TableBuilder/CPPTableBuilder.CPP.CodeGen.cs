using System;
using System.Collections.Generic;

namespace TableBuilder
{
    internal partial class CPPTableBuilder
    {

        void ForeachSubElement(Action<ElementInfo> action)
        {
            HashSet<string> elementHashSet = new HashSet<string>();
            foreach (KeyValuePair<string, ElementInfo> kvp in ElementMap)
            {
                ElementInfo elementInfo = kvp.Value;

                if (elementHashSet.Contains(elementInfo.TypeName))
                    continue;

                elementHashSet.Add(elementInfo.TypeName);

                if (elementInfo.ParentTypeName == "")
                    continue;

                action(elementInfo);
            }
            elementHashSet.Clear();
        }



        // Define loader element
        void DefineLoaderElement()
        {
            string loaderElement = ClassName+"LoaderElement";
            string statement;


            NewLine(2);

            OpenSection("class", string.Format("{0} : public BR::XML::DOMElement", loaderElement));

            WriteStatement("public:", -1);
            WriteStatement(string.Format("{0}::{0}() : BR::XML::DOMElement(\"{0}\")", loaderElement));
            OpenSection();
            CloseSection();

            WriteStatement("void AddChild( DOMElement *pChild ) override");
            OpenSection();


            ForeachSubElement((elementInfo) =>
            {
                WriteStatement(string.Format("if( pChild->GetTypeName() == \"{0}\" )", elementInfo.TypeName));
                OpenSection();

                WriteStatement(string.Format("auto* p{1} = dynamic_cast<{0}*>(pChild);", elementInfo.OutputTypeName, elementInfo.TypeName));
                if (ThisTableType == TableType.TTYPE_SINGLEKEY)
                {
                    KeyInfo KeyInformation = this["first"];

                    if (KeyInformation.keyType == KeyType.EKEY_UNIQUE ||
                            KeyInformation.keyType == KeyType.EKEY_NONUNIQUE)
                    {
                        statement = string.Format("{0}::m_TableMap.insert(std::make_pair(p{1}->{2}, p{1}));",
                        ClassName, elementInfo.TypeName, KeyInformation._keyName1);
                        WriteStatement(statement);
                    }
                    else if (KeyInformation.keyType == KeyType.EKEY_COMPOSIT)
                    {
                        statement = string.Format("ULONGLONG ulCombined = BR::Access::Combine64(p{0}->{1}, p{0}->{2});",
                            elementInfo.TypeName, KeyInformation._keyName1, KeyInformation._keyName2);
                        WriteStatement(statement);
                        statement = string.Format("{0}::m_TableMap.insert(std::make_pair(ulCombined, p{1}));",
                            ClassName, elementInfo.TypeName);
                        WriteStatement(statement);
                    }
                }
                else
                {
                    foreach (KeyValuePair<string, KeyInfo> kvp1 in _keyInfos)
                    {
                        KeyInfo KeyInformation = kvp1.Value;
                        string tableName = KeyInformation._keyName1 + "Table";

                        if (KeyInformation.keyType == KeyType.EKEY_UNIQUE ||
                            KeyInformation.keyType == KeyType.EKEY_NONUNIQUE)
                        {
                            statement = string.Format("{0}::{3}.insert(std::make_pair(p{1}->{2}, p{1}));",
                            ClassName, elementInfo.TypeName, KeyInformation._keyName1, tableName);
                            WriteStatement(statement);
                        }
                        else if (KeyInformation.keyType == KeyType.EKEY_COMPOSIT)
                        {
                            statement = string.Format("ULONGLONG ulCombined = BR::Access::Combine64(p{0}->{1}, p{0}->{2});",
                                elementInfo.TypeName, KeyInformation._keyName1, KeyInformation._keyName2);
                            WriteStatement(statement);
                            statement = string.Format("{0}::{2}.insert(std::make_pair(ulCombined, p{1}));",
                                ClassName, elementInfo.TypeName, tableName);
                            WriteStatement(statement);
                        }
                    }
                }

                CloseSection();
            });


            CloseSection();

            CloseSection();
        }


        protected override void DefineTableParser(Type rootTypeInfo)
        {
            NewLine(1);
            string statement;
            HashSet<string> elementHashSet = new HashSet<string>();


            // Parser class definition start.
            ParserName = string.Format("{0}Parser", ClassName);
            statement = string.Format("// {0} decl/impl", ParserName);
            WriteStatement(statement);

            statement = string.Format("{0} : public BR::{1}", ParserName, BaseParserName);
            OpenSection("class", statement);
            WriteStatement("public:", -1);


            // Define property enum start
            ForeachSubElement((elementInfo) =>
            {
                if (elementHashSet.Contains(elementInfo.TypeName))
                    return;

                elementHashSet.Add(elementInfo.TypeName);

                if (elementInfo.ParentTypeName == "")
                    return;

                statement = string.Format("ATT_ID_{0}", elementInfo.ElementName);
                OpenSection("enum", statement);
                foreach (System.Reflection.PropertyInfo prop in elementInfo.TypeInfo.GetProperties())
                {
                    if ((prop.PropertyType.IsClass && prop.PropertyType != "".GetType()) ||
                        HasIgnoreAttribute(prop))
                        continue;
                    statement = string.Format("EATT_{0},", prop.Name);
                    WriteStatement(statement);
                }
                CloseSection();
                NewLine(1);
            });
            // Define property enum end
            


            // Define parser constructor start
            NewLine(1);
            WriteStatement("public:", -1);
            statement = string.Format("{0}()", ParserName);
            WriteStatement(statement);
            OpenSection();
            foreach (KeyValuePair<string, ElementInfo> kvp in ElementMap)
            {
                ElementInfo elementInfo = kvp.Value;

                if (elementInfo.ElementName == ClassName)
                {
                    statement = string.Format("RegisterElementCreator( \"{0}\", []()-> BR::XML::DOMElement* {{ return new {1}LoaderElement; }} );",
                                    elementInfo.ElementName, ClassName);
                    WriteStatement(statement);
                }
                else
                {
                    statement = string.Format("RegisterElementCreator( \"{0}\", []()-> BR::XML::DOMElement* {{ return new {1}; }} );",
                                    elementInfo.ElementName, elementInfo.OutputTypeName);

                    WriteStatement(statement);
                }

            }

            //NewLine(1);
            //foreach (KeyValuePair<string, ElementInfo> kvp in ElementMap)
            //{
            //    ElementInfo elementInfo = kvp.Value;

            //    if (elementHashSet.Contains(elementInfo.TypeName))
            //        continue;

            //    elementHashSet.Add(elementInfo.TypeName);

            //    if (elementInfo.ParentTypeName == "")
            //        continue; 

            //    foreach (System.Reflection.PropertyInfo prop in elementInfo.TypeInfo.GetProperties())
            //    {
            //        if ((prop.PropertyType.IsClass && prop.PropertyType != "".GetType()) ||
            //            HasIgnoreAttribute(prop))
            //            continue;

            //        statement = string.Format("AddAttributeID( \"{0}\", EATT_{0} );", prop.Name);
            //        WriteStatement(statement);
            //    }
            //    NewLine(1);
            //}
            //elementHashSet.Clear();
            CloseSection();
            // Define parser constructor end


            //// Define element handler start
            //NewLine(1);
            //foreach (KeyValuePair<string, ElementInfo> kvp in ElementMap)
            //{
            //    ElementInfo elementInfo = kvp.Value;

            //    if (elementHashSet.Contains(elementInfo.TypeName))
            //        continue;
            //    elementHashSet.Add(elementInfo.TypeName);

            //    if (elementInfo.ParentTypeName == "")
            //        continue; 

            //    NewLine(1);
            //    statement = string.Format("void Start{0}(", elementInfo.TypeName);
            //    WriteStatement(statement);
            //    WriteStatement("const xmlChar * localname,", 1);
            //    WriteStatement("const xmlChar * prefix,", 1);
            //    WriteStatement("const xmlChar * URI,", 1);
            //    WriteStatement("int nb_namespaces,", 1);
            //    WriteStatement("const xmlChar ** namespaces,", 1);
            //    WriteStatement("int nb_attributes,", 1);
            //    WriteStatement("int nb_defaulted,", 1);
            //    WriteStatement("const xmlChar ** attributes )", 1);
            //    OpenSection();

            //    if (elementInfo.ElementName != ClassName)
            //    {
            //        statement = string.Format("m_p{0} = new {1}::{0};", elementInfo.TypeName, ClassName);
            //        WriteStatement(statement);
            //    }

            //    NewLine(1);
            //    WriteStatement("unsigned int index = 0;");
            //    WriteStatement("for ( int indexAttribute = 0;");
            //    WriteStatement("indexAttribute < nb_attributes;", 1);
            //    WriteStatement("++indexAttribute, index += 5 )", 1);
            //    OpenSection();
            //    WriteStatement("const xmlChar *attName = attributes[index];");
            //    WriteStatement("const xmlChar *prefix = attributes[index+1];");
            //    WriteStatement("const xmlChar *nsURI = attributes[index+2];");
            //    WriteStatement("const xmlChar *valueBegin = attributes[index+3];");
            //    WriteStatement("const xmlChar *valueEnd = attributes[index+4];");
            //    WriteStatement("std::string value( (const char *)valueBegin, (const char *)valueEnd );");
            //    NewLine(1);
            //    WriteStatement("switch( GetAttributeID((char*)attName) )");
            //    OpenSection();
            //    foreach (System.Reflection.PropertyInfo prop in elementInfo.TypeInfo.GetProperties())
            //    {
            //        if ((prop.PropertyType.IsClass && prop.PropertyType != "".GetType()) ||
            //            HasIgnoreAttribute(prop))
            //            continue;

            //        statement = string.Format("case EATT_{0}:", prop.Name);
            //        WriteStatement(statement, -1);
            //        Type typeInfo = prop.PropertyType;
            //        if (typeInfo.Name == "Int32" ||
            //            typeInfo.Name == "UInt32")
            //        {
            //            statement = string.Format("m_p{0}->{1} = atoi( value.c_str() );", elementInfo.TypeName, prop.Name);
            //            WriteStatement(statement);
            //        }
            //        else if (typeInfo.Name == "String")
            //        {
            //            statement = string.Format("m_p{0}->{1} = value;", elementInfo.TypeName, prop.Name);
            //            WriteStatement(statement);
            //        }
            //        else if (typeInfo.Name == "Single" ||
            //                typeInfo.Name == "Double")
            //        {
            //            statement = string.Format("m_p{0}->{1} = (float)atof(value.c_str() );", elementInfo.TypeName, prop.Name);
            //            WriteStatement(statement);
            //        }
            //        else if (typeInfo.Name == "Boolean")
            //        {
            //            statement = string.Format("m_p{0}->{1} = (value == \"true\");", elementInfo.TypeName, prop.Name);
            //            WriteStatement(statement);
            //        }
            //        else if (typeInfo.IsEnum)
            //        {
            //            statement = string.Format("m_p{0}->{1} = value.c_str();", elementInfo.TypeName, prop.Name);
            //            WriteStatement(statement);
            //            statement = string.Format("if (m_p{0}->{1} == \"INVALID_ENUM\")", elementInfo.TypeName, prop.Name);
            //            WriteStatement(statement);
            //            statement = string.Format("Error(\"GameTable: Invalid enum value(%s-%s-value:%s)!\", \"{0}\", \"{1}\", value.c_str());", ClassName, prop.Name);
            //            WriteStatement(statement, 1);
            //        }
            //        else
            //        {
            //            statement = "-------------- Error type not supported --------------------";
            //            WriteStatement(statement);
            //        }
            //        WriteStatement("break;");
            //    }
            //    CloseSection();
            //    CloseSection();
            //    CloseSection();

            //    NewLine(1);
            //    statement = string.Format("void End{0}( ", elementInfo.TypeName);
            //    WriteStatement(statement);
            //    WriteStatement("const xmlChar * localname,", 1);
            //    WriteStatement("const xmlChar * prefix,", 1);
            //    WriteStatement("const xmlChar * URI )", 1);
            //    OpenSection();
                
            //    // Insert row to table!
            //    if (elementInfo.ElementName != ClassName)
            //    {
            //        if (elementInfo.ParentTypeName == ClassName)
            //        {
            //            statement = string.Format("if (m_p{0})", elementInfo.TypeName);
            //            WriteStatement(statement);
            //            OpenSection();

            //            if (ThisTableType == TableType.TTYPE_SINGLEKEY)
            //            {
            //                KeyInfo KeyInformation = this["first"];

            //                if (KeyInformation.keyType == KeyType.EKEY_UNIQUE ||
            //                        KeyInformation.keyType == KeyType.EKEY_NONUNIQUE)
            //                {
            //                    statement = string.Format("{0}::m_TableMap.insert(std::make_pair(m_p{1}->{2}, m_p{1}));",
            //                    ClassName, elementInfo.TypeName, KeyInformation._keyName1);
            //                    WriteStatement(statement);
            //                }
            //                else if (KeyInformation.keyType == KeyType.EKEY_COMPOSIT)
            //                {
            //                    statement = string.Format("ULONGLONG ulCombined = BR::Access::Combine64(m_p{0}->{1}, m_p{0}->{2});",
            //                        elementInfo.TypeName, KeyInformation._keyName1, KeyInformation._keyName2);
            //                    WriteStatement(statement);
            //                    statement = string.Format("{0}::m_TableMap.insert(std::make_pair(ulCombined, m_p{1}));",
            //                        ClassName, elementInfo.TypeName);
            //                    WriteStatement(statement);
            //                }
            //            }
            //            else
            //            {
            //                foreach (KeyValuePair<string, KeyInfo> kvp1 in _keyInfos)
            //                {
            //                    KeyInfo KeyInformation = kvp1.Value;
            //                    string tableName = KeyInformation._keyName1 + "Table";

            //                    if (KeyInformation.keyType == KeyType.EKEY_UNIQUE ||
            //                        KeyInformation.keyType == KeyType.EKEY_NONUNIQUE)
            //                    {
            //                        statement = string.Format("{0}::{3}.insert(std::make_pair(m_p{1}->{2}, m_p{1}));",
            //                        ClassName, elementInfo.TypeName, KeyInformation._keyName1, tableName);
            //                        WriteStatement(statement);
            //                    }
            //                    else if (KeyInformation.keyType == KeyType.EKEY_COMPOSIT)
            //                    {
            //                        statement = string.Format("ULONGLONG ulCombined = BR::Access::Combine64(m_p{0}->{1}, m_p{0}->{2});",
            //                            elementInfo.TypeName, KeyInformation._keyName1, KeyInformation._keyName2);
            //                        WriteStatement(statement);
            //                        statement = string.Format("{0}::{2}.insert(std::make_pair(ulCombined, m_p{1}));",
            //                            ClassName, elementInfo.TypeName, tableName);
            //                        WriteStatement(statement);
            //                    }
            //                }
            //            }

            //            statement = string.Format("m_p{0} = NULL;", elementInfo.TypeName);
            //            WriteStatement(statement);

            //            CloseSection();
            //            WriteStatement("else");
            //            OpenSection();
            //            WriteStatement("// TODO: error string");
            //            CloseSection();
            //        }
            //        else
            //        {
            //            if (IsArray(GetTypeInfo(ClassName), elementInfo.TypeInfo.Name))
            //            {
            //                statement = string.Format("m_p{0}->m_{1}s.push_back(m_p{1});", 
            //                    elementInfo.ParentElementName, elementInfo.ElementName);
            //                WriteStatement(statement);
            //                statement = string.Format("m_p{0} = NULL;", elementInfo.ElementName);
            //                WriteStatement(statement);
            //            }
            //            else
            //            {
            //                statement = string.Format("m_p{0}->m_{1}s = m_p{1};",
            //                    elementInfo.ParentElementName, elementInfo.ElementName);
            //                WriteStatement(statement);
            //                statement = string.Format("m_p{0} = NULL;", elementInfo.ElementName);
            //                WriteStatement(statement);
            //            }
            //        }
            //    }

            //    CloseSection();
            //}
            //elementHashSet.Clear();
            //// Define element handler end


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
                statement = string.Format("m_Map.insert(std::make_pair(\"{0}\", {1}::Enum::{2}));", strEnum, name, strEnum);
                WriteStatement(statement);
            }
            CloseSection();

            // operator =(int i)
            NewLine(1);
            statement = string.Format("{0}::{1}::Enum {2}::{3}::operator =(int i)", ClassName, name, ClassName, name);
            WriteStatement(statement);
            OpenSection();
            WriteStatement("m_Enum = AsEnum((Enum)i);");
            WriteStatement("return m_Enum;");
            CloseSection();

            // operator int () const
            NewLine(1);
            statement = string.Format("{0}::{1}::operator int () const", ClassName, name);
            WriteStatement(statement);
            OpenSection();
            WriteStatement("return (int)m_Enum;");
            CloseSection();

            // operator Enum () const
            NewLine(1);
            statement = string.Format("{0}::{1}::operator Enum () const", ClassName, name);
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
            WriteStatement(string.Format("return {0}::Enum::INVALID_ENUM;", name),1);
            NewLine(1);
            WriteStatement("return itr->second;");
            CloseSection();

            // AsEnum(int e)
            NewLine(1);
            statement = string.Format("{0}::{1}::Enum {2}::{3}::AsEnum(Enum e)", ClassName, name, ClassName, name);
            WriteStatement(statement);
            OpenSection();
            WriteStatement("for (EnumMapItr itr = m_Map.begin(); itr != m_Map.end(); ++itr)");
            OpenSection();
            WriteStatement("if (itr->second == e)");
            WriteStatement("return itr->second;", 1);
            CloseSection();
            WriteStatement(string.Format("return {0}::Enum::INVALID_ENUM;", name));
            CloseSection();

            // AsName(int e)
            NewLine(1);
            statement = string.Format("const char* {0}::{1}::AsName(Enum e)", ClassName, name);
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

            WriteStatement(string.Format("{0} {0}::m_Instance;", ClassName));

            // Define table map
            if (ThisTableType == TableType.TTYPE_SINGLEKEY)
            {
                statement = string.Format("{0}::TableMap {0}::m_TableMap;", ClassName);
                WriteStatement(statement);
            }
            else
            {
                foreach (KeyValuePair<string, KeyInfo> kvp in _keyInfos)
                {
                    string tableName        = kvp.Value._keyName1 + "Table";
                    string tableTypeName    = kvp.Value._keyName1 + "TableMap";
                    statement = string.Format("{0}::{2} {0}::{1};", ClassName, tableName, tableTypeName);
                    WriteStatement(statement);
                }
            }
            
            // Load table
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

            NewLine(1);

            if (ThisTableType == TableType.TTYPE_SINGLEKEY)
            {
                // ClearTable
                WriteClearTable(this["first"]);

                NewLine(1);
                // FindItem
                WriteFindItem(this["first"]);
            }
            else
            {
                foreach (KeyValuePair<string, KeyInfo> kvp in _keyInfos)
                {
                    // ClearTable
                    WriteMultiKeyClearTable(kvp.Value);
                    NewLine(1);

                    // FindItem
                    WriteMultiKeyFindItem(kvp.Value);
                    NewLine(1);
                }

                WriteMultiKeyClearTable();
            }
        }


        void DefineSubElementMemberFunction()
        {
            ForeachSubElement((elementInfo) =>
            {
                NewLine(1);
                WriteStatement(string.Format("// sub class {0} member implementations", elementInfo.TypeName));
                WriteStatement(string.Format("{0}::{1}() : BR::XML::DOMElement(\"{1}\")", elementInfo.OutputTypeName, elementInfo.TypeName));
                OpenSection();
                CloseSection();

                WriteStatement(string.Format("bool {0}::SetAttributeValue( const std::string& name, const std::string& value )", elementInfo.OutputTypeName));
                OpenSection();
                {
                    string ifPrefix = "";
                    
                    foreach (System.Reflection.PropertyInfo prop in elementInfo.TypeInfo.GetProperties())
                    {
                        if ((prop.PropertyType.IsClass && prop.PropertyType != "".GetType()) ||
                            HasIgnoreAttribute(prop))
                            continue;

                        WriteStatement(string.Format("{0}if(name == \"{1}\")", ifPrefix, prop.Name));
                        OpenSection();

                        string statement;
                        Type typeInfo = prop.PropertyType;
                        if (typeInfo.Name == "Int32" ||
                            typeInfo.Name == "UInt32")
                        {
                            statement = string.Format("this->{0} = atoi( value.c_str() );", prop.Name);
                        }
                        else if (typeInfo.Name == "String")
                        {
                            statement = string.Format("this->{0} = value;", prop.Name);
                        }
                        else if (typeInfo.Name == "Single" ||
                                typeInfo.Name == "Double")
                        {
                            statement = string.Format("this->{0} = (float)atof(value.c_str() );", prop.Name);
                        }
                        else if (typeInfo.Name == "Boolean")
                        {
                            statement = string.Format("this->{0} = (value == \"true\");", prop.Name);
                        }
                        else if (typeInfo.IsEnum)
                        {
                            statement = string.Format("this->{0} = value.c_str();", prop.Name);
                            WriteStatement(statement);
                            statement = string.Format("if (this->{0} == \"INVALID_ENUM\") return false;", prop.Name);
                            //statement = string.Format("throw new Invalid(\"GameTable: Invalid enum value(%s-%s-value:%s)!\", \"{0}\", \"{1}\", value.c_str());", ClassName, prop.Name);
                        }
                        else if (typeInfo.Name == "Int64")
                        {
                            statement = string.Format("this->{0} = _atoi64( value.c_str() );", prop.Name);
                        }
                        else
                        {
                            statement = "-------------- Error type not supported --------------------";
                        }
                        WriteStatement(statement);
                        WriteStatement("return true;");

                        CloseSection();
                        ifPrefix = "else ";
                    }

                    WriteStatement("return __super::SetAttributeValue(name,value);");
                }
                CloseSection();

                WriteStatement(string.Format("void {0}::AddChild( DOMElement *pChild )", elementInfo.OutputTypeName));
                OpenSection();
                    WriteStatement("return __super::AddChild(pChild);");
                CloseSection();

            });

            NewLine(1);
        }
    }
}