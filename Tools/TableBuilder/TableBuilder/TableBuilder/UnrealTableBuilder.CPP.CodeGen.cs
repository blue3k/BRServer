using System;
using System.Collections.Generic;

namespace TableBuilder
{
    internal partial class UnrealTableBuilder
    {
        protected override void DefineTableParser(Type rootTypeInfo)
        {
            NewLine(1);
            string statement;
            HashSet<string> elementHashSet = new HashSet<string>();

            // Define parser class start
            ParserName = string.Format("{0}Parser", ClassName);
            statement = string.Format("// {0} decl/impl", ParserName);
            WriteStatement(statement);

            statement = string.Format("{0} : public BR::{1}", ParserName, BaseParserName);
            OpenSection("class", statement);
            WriteStatement("public:", -1);
            // Define property enum start
            foreach (KeyValuePair<string, ElementInfo> kvp in ElementMap)
            {
                ElementInfo elementInfo = kvp.Value;

                if (elementHashSet.Contains(elementInfo.TypeName))
                    continue;

                elementHashSet.Add(elementInfo.TypeName);

                if (elementInfo.ParentTypeName == "")
                    continue; 

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
            }
            elementHashSet.Clear();
            // Define property enum end


            // Define member data start
            WriteStatement("private:", -1);
            foreach (KeyValuePair<string, ElementInfo> kvp in ElementMap)
            {
                ElementInfo elementInfo = kvp.Value;

                if (elementInfo.ElementName == ClassName)
                {
                    statement = string.Format("U{0}* m_p{0};", ClassName);
                }
                else
                {
                    statement = string.Format("F{0}* m_p{0};", elementInfo.TypeName);
                }
                WriteStatement(statement);
            }
            // Define member data end


            // Define parser constructor start
            NewLine(1);
            WriteStatement("public:", -1);
            statement = string.Format("{0}({1}* {2}) : {3}({2})", ParserName, UnrealGenerateClassName,
                   "p" + ClassName, "m_p" + ClassName);
            WriteStatement(statement);
            OpenSection();

            foreach (KeyValuePair<string, ElementInfo> kvp in ElementMap)
            {
                ElementInfo elementInfo = kvp.Value;

                if (elementInfo.ParentTypeName == "")
                    continue;

                statement = string.Format("AddElementHandler( \"{0}\", ElementStartHandler(&{1}::Start{0}), ElementEndHandler(&{1}::End{0}) );",
                                elementInfo.ElementName, ParserName);
                WriteStatement(statement);
            }

            NewLine(1);
            foreach (KeyValuePair<string, ElementInfo> kvp in ElementMap)
            {
                ElementInfo elementInfo = kvp.Value;

                if (elementHashSet.Contains(elementInfo.TypeName))
                    continue;

                elementHashSet.Add(elementInfo.TypeName);

                if (elementInfo.ParentTypeName == "")
                    continue;

                foreach (System.Reflection.PropertyInfo prop in elementInfo.TypeInfo.GetProperties())
                {
                    if ((prop.PropertyType.IsClass && prop.PropertyType != "".GetType()) ||
                        HasIgnoreAttribute(prop))
                        continue;

                    statement = string.Format("AddAttributeID( \"{0}\", EATT_{0} );", prop.Name);
                    WriteStatement(statement);
                }
                NewLine(1);
            }
            elementHashSet.Clear();
            CloseSection();
            // Define parser constructor end


            // Define element handler start
           NewLine(1);
           foreach (KeyValuePair<string, ElementInfo> kvp in ElementMap)
           {
               ElementInfo elementInfo = kvp.Value;

               if (elementHashSet.Contains(elementInfo.TypeName))
                   continue;
               elementHashSet.Add(elementInfo.TypeName);

               if (elementInfo.ParentTypeName == "")
                   continue;

               statement = string.Format("void Start{0}(", elementInfo.ElementName);
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
               statement = string.Format("m_p{0} = new F{1}(EC_EventParm);", elementInfo.TypeName, elementInfo.TypeName);
               WriteStatement(statement);
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
               WriteStatement("switch( GetAttributeID((char*)attName) )");
               OpenSection();
               foreach (System.Reflection.PropertyInfo prop in elementInfo.TypeInfo.GetProperties())
               {
                   if ((prop.PropertyType.IsClass && prop.PropertyType != "".GetType()) ||
                        HasIgnoreAttribute(prop))
                       continue;

                   statement = string.Format("case EATT_{0}:", prop.Name);
                   WriteStatement(statement, -1);
                   OpenSection();
                   Type typeInfo = prop.PropertyType;
                   if (typeInfo.Name == "Int32" ||
                       typeInfo.Name == "UInt32")
                   {
                       statement = string.Format("m_p{0}->{1} = atoi( value.c_str() );", elementInfo.TypeName, prop.Name);
                       WriteStatement(statement);
                   }
                   else if (typeInfo.Name == "String")
                   {
                       statement = string.Format("m_p{0}->{1} = UTF8_TO_TCHAR(value.c_str());", elementInfo.TypeName, prop.Name);
                       WriteStatement(statement);
                   }
                   else if (typeInfo.Name == "Single" ||
                           typeInfo.Name == "Double")
                   {
                       statement = string.Format("m_p{0}->{1} = atof(value.c_str() );", elementInfo.TypeName, prop.Name);
                       WriteStatement(statement);
                   }
                   else if (typeInfo.Name == "Boolean")
                   {
                       statement = string.Format("m_p{0}->{1} = (value == \"true\");", elementInfo.TypeName, prop.Name);
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
                       statement = string.Format("m_p{0}->{1} = eVar;", elementInfo.TypeName, prop.Name);
                       WriteStatement(statement);
                   }
                   else if (typeInfo.IsClass)
                   {
                       // Pass Element
                   }
                   CloseSection();
                   WriteStatement("break;");
               }
               CloseSection();
               CloseSection();
               CloseSection();

               NewLine(1);
               statement = string.Format("void End{0}( ", elementInfo.ElementName);
               WriteStatement(statement);
               WriteStatement("const xmlChar * localname,", 1);
               WriteStatement("const xmlChar * prefix,", 1);
               WriteStatement("const xmlChar * URI )", 1);
               OpenSection();

               // Insert row to table!
               if (elementInfo.ElementName != ClassName)
               {
                   if (elementInfo.ParentTypeName == ClassName)
                   {
                       statement = string.Format("if (m_p{0})", elementInfo.TypeName);
                       WriteStatement(statement);
                       OpenSection();


                       // Insert row to table!
                       if (ThisTableType == TableType.TTYPE_SINGLEKEY)
                       {
                           KeyInfo KeyInformation = this["first"];

                           if (KeyInformation.keyType == KeyType.EKEY_UNIQUE)
                           {
                               statement = string.Format("m_p{0}->TableMap.Set(m_p{1}->{2}, m_p{1});",
                               ClassName, elementInfo.TypeName, KeyInformation._keyName1);
                               WriteStatement(statement);
                           }
                           else if (KeyInformation.keyType == KeyType.EKEY_NONUNIQUE)
                           {
                               statement = string.Format("TArray<F{0}*>* pRows = m_p{1}->TableMap.Find(m_p{0}->{2});",
                                   RowTypeName, ClassName, KeyInformation._keyName1);
                               WriteStatement(statement);
                               WriteStatement("if (!pRows)");
                               OpenSection();
                               statement = string.Format("pRows = &m_p{0}->TableMap.Set(m_p{1}->{2}, TArray<F{1}*>());",
                                   ClassName, RowTypeName, KeyInformation._keyName1);
                               WriteStatement(statement);
                               CloseSection();
                               statement = string.Format("pRows->AddItem(m_p{0});", RowTypeName);
                               WriteStatement(statement);
                           }
                           else if (KeyInformation.keyType == KeyType.EKEY_COMPOSIT)
                           {
                               statement = string.Format("ULONGLONG u64CompoisteKey = BR::Access::Combine64(m_p{0}->{1}, m_p{0}->{2});",
                                           RowTypeName, KeyInformation._keyName1, KeyInformation._keyName2);
                               WriteStatement(statement);
                               statement = string.Format("m_p{0}->TableMap.Set(u64CompoisteKey, m_p{1});", ClassName, RowTypeName);
                               WriteStatement(statement);
                           }

                           NewLine(1);
                       }
                       else
                       {
                           foreach (KeyValuePair<string, KeyInfo> kvp1 in _keyInfos)
                           {
                               KeyInfo KeyInformation = kvp1.Value;
                               string tableName = KeyInformation._keyName1 + "Table";

                               if (KeyInformation.keyType == KeyType.EKEY_UNIQUE)
                               {
                                   statement = string.Format("m_p{0}->{3}.Set(m_p{1}->{2}, m_p{1});",
                                   ClassName, elementInfo.TypeName, KeyInformation._keyName1, tableName);
                                   WriteStatement(statement);
                               }
                               else if (KeyInformation.keyType == KeyType.EKEY_NONUNIQUE)
                               {
                                   statement = string.Format("TArray<F{0}*>* pRows = m_p{1}->{3}.Find(m_p{0}->{2});",
                                       RowTypeName, ClassName, KeyInformation._keyName1, tableName);
                                   WriteStatement(statement);
                                   WriteStatement("if (!pRows)");
                                   OpenSection();
                                   statement = string.Format("pRows = &m_p{0}->{3}.Set(m_p{1}->{2}, TArray<F{1}*>());",
                                       ClassName, RowTypeName, KeyInformation._keyName1, tableName);
                                   WriteStatement(statement);
                                   CloseSection();
                                   statement = string.Format("pRows->AddItem(m_p{0});", RowTypeName);
                                   WriteStatement(statement);
                               }
                               else if (KeyInformation.keyType == KeyType.EKEY_COMPOSIT)
                               {
                                   statement = string.Format("ULONGLONG u64CompoisteKey = BR::Access::Combine64(m_p{0}->{1}, m_p{0}->{2});",
                                               RowTypeName, KeyInformation._keyName1, KeyInformation._keyName2);
                                   WriteStatement(statement);
                                   statement = string.Format("m_p{0}->{2}.Set(u64CompoisteKey, m_p{1});", ClassName, RowTypeName, tableName);
                                   WriteStatement(statement);
                               }
                           }
                       }

                       statement = string.Format("m_p{0} = NULL;", elementInfo.TypeName);
                       WriteStatement(statement);
                       CloseSection();
                       WriteStatement("else");
                       OpenSection();
                       WriteStatement("// TODO: error string");
                       CloseSection();
                   }
                   else
                   {
                       if (IsArray(GetTypeInfo(ClassName), elementInfo.TypeInfo.Name))
                       {
                           statement = string.Format("m_p{0}->{1}s.AddItem(*m_p{2});",
                               elementInfo.ParentTypeName, elementInfo.ElementName, elementInfo.TypeName);
                           WriteStatement(statement);
                           statement = string.Format("m_p{0} = NULL;", elementInfo.TypeName);
                           WriteStatement(statement);
                       }
                       else
                       {
                           statement = string.Format("m_p{0}->m_{1}s = m_p{1};",
                               elementInfo.ParentTypeName, elementInfo.TypeName);
                           WriteStatement(statement);
                           statement = string.Format("m_p{0} = NULL;", elementInfo.TypeName);
                           WriteStatement(statement);
                       }
                   }
                   
               }

               CloseSection();
           }
           elementHashSet.Clear();
           // Define element handler end


           // Define parser member function start
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
            // Define parser member function end

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

            if (ThisTableType == TableType.TTYPE_SINGLEKEY)
            {
                WriteClearTable(this["first"]);
                NewLine(1);

                WriteFindItem(this["first"]);
                NewLine(1);

                WriteAllRows(this["first"]);
            }
            else
            {
                foreach (KeyValuePair<string, KeyInfo> kvp in _keyInfos)
                {
                    // ClearTable
                    if (kvp.Value == _keyInfos["first"])
                        WriteMultiKeyClearTable(kvp.Value, true);
                    else
                        WriteMultiKeyClearTable(kvp.Value, false);

                    NewLine(1);

                    // FindItem
                    WriteMultiKeyFindItem(kvp.Value);
                    NewLine(1);

                    // AllRows
                    WriteMultiKeyAllRows(kvp.Value);
                }

                // MultiKeyClearTable
                WriteMultiKeyClearTable();
            }
        }
    }
}