﻿using System;
using System.Collections.Generic;

namespace TableBuilder
{
    internal partial class CPPDBTableBuilder
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

        

        void WriteLoadTableList()
        {

            NewLine(1);
            string statement = string.Format("BR::Result {0}::LoadTable( const std::list<{1}>& rowList )", ClassName, RowTypeName);
            WriteStatement(statement);
            OpenSection();

            // prepare swap and new variables
            foreach (KeyValuePair<string, KeyInfo> itKeyInfo in _keyInfos)
            {
                KeyInfo keyInfo = itKeyInfo.Value;
                string tableTypeName = KeyInformation.GetMapTypeName();
                string tableVarName = KeyInformation.GetMapVarName();

                WriteStatement("auto pNew{0} = new {0};", tableTypeName);
            }

            NewLine(1);
            WriteStatement("for( auto rowItem : rowList )");
            OpenSection();

            ForeachSubElement((elementInfo) =>
            {
                WriteStatement("auto* p{1} = new {0};", elementInfo.OutputTypeName, elementInfo.TypeName);
                WriteStatement("*p{1} = rowItem;", elementInfo.OutputTypeName, elementInfo.TypeName);

                foreach (KeyValuePair<string, KeyInfo> kvp1 in _keyInfos)
                {
                    KeyInfo KeyInformation = kvp1.Value;
                    string tableTypeName = KeyInformation.GetMapTypeName();
                    string tableVarName = KeyInformation.GetMapVarName();

                    if (KeyInformation.keyType == KeyType.EKEY_UNIQUE ||
                        KeyInformation.keyType == KeyType.EKEY_NONUNIQUE)
                    {
                        WriteStatement("pNew{2}->insert(std::make_pair(p{0}->{1}, p{0}));",
                        elementInfo.TypeName, KeyInformation._keyName1, tableTypeName);
                    }
                    else if (KeyInformation.keyType == KeyType.EKEY_COMPOSIT)
                    {
                        WriteStatement("ULONGLONG ulCombined = BR::Access::Combine64(p{0}->{1}, p{0}->{2});",
                            elementInfo.TypeName, KeyInformation._keyName1, KeyInformation._keyName2);
                        WriteStatement("pNew{1}->insert(std::make_pair(ulCombined, p{0}));",
                            elementInfo.TypeName, tableTypeName);
                    }
                }

            });

            CloseSection();


            NewLine(1);
            // New data is ready, swap variables
            foreach (KeyValuePair<string, KeyInfo> itKeyInfo in _keyInfos)
            {
                KeyInfo keyInfo = itKeyInfo.Value;
                string tableTypeName = KeyInformation.GetMapTypeName();
                string tableVarName = KeyInformation.GetMapVarName();

                WriteStatement("if ({0}Prev != nullptr)", tableVarName);
                OpenSection();
                WriteStatement("for( auto itItem : *{0}Prev) {{ delete itItem.second; }} ;", tableVarName);
                WriteStatement("delete {0}Prev;", tableVarName);
                CloseSection();
                WriteStatement("{0}Prev = {0};", tableVarName);
                WriteStatement("{1} = pNew{0};", tableTypeName, tableVarName);
            }


            WriteStatement("return BR::ResultCode::SUCCESS;");
            CloseSection();

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

            // Declare static table map variables
            foreach (KeyValuePair<string, KeyInfo> kvp in _keyInfos)
            {
                string tableTypeName = kvp.Value.GetMapTypeName();
                string tableVarName = kvp.Value.GetMapVarName();
                WriteStatement("{0}::{1} *{0}::{2} = nullptr;", ClassName, tableTypeName, tableVarName);
                WriteStatement("{0}::{1} *{0}::{2}Prev = nullptr;", ClassName, tableTypeName, tableVarName);
            }

            // Load table
            WriteLoadTableList();

            NewLine(1);

            if (ThisTableType == TableType.TTYPE_SINGLEKEY)
            {
                // ClearTable
                //WriteClearTable(this["first"]);

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
                //NewLine(1);
                //WriteStatement(string.Format("// sub class {0} member implementations", elementInfo.TypeName));
                //WriteStatement(string.Format("{0}::{1}()", elementInfo.OutputTypeName, elementInfo.TypeName));
                //OpenSection();
                //CloseSection();

                //WriteStatement(string.Format("bool {0}::SetAttributeValue( const std::string& name, const std::string& value )", elementInfo.OutputTypeName));
                //OpenSection();
                //{
                //    string ifPrefix = "";
                    
                //    foreach (System.Reflection.PropertyInfo prop in elementInfo.TypeInfo.GetProperties())
                //    {
                //        if ((prop.PropertyType.IsClass && prop.PropertyType != "".GetType()) ||
                //            HasIgnoreAttribute(prop))
                //            continue;

                //        WriteStatement(string.Format("{0}if(name == \"{1}\")", ifPrefix, prop.Name));
                //        OpenSection();

                //        string statement;
                //        Type typeInfo = prop.PropertyType;
                //        if (typeInfo.Name == "Int32" ||
                //            typeInfo.Name == "UInt32")
                //        {
                //            statement = string.Format("this->{0} = atoi( value.c_str() );", prop.Name);
                //        }
                //        else if (typeInfo.Name == "String")
                //        {
                //            statement = string.Format("this->{0} = value;", prop.Name);
                //        }
                //        else if (typeInfo.Name == "Single" ||
                //                typeInfo.Name == "Double")
                //        {
                //            statement = string.Format("this->{0} = (float)atof(value.c_str() );", prop.Name);
                //        }
                //        else if (typeInfo.Name == "Boolean")
                //        {
                //            statement = string.Format("this->{0} = (value == \"true\");", prop.Name);
                //        }
                //        else if (typeInfo.IsEnum)
                //        {
                //            statement = string.Format("this->{0} = value.c_str();", prop.Name);
                //            WriteStatement(statement);
                //            statement = string.Format("if (this->{0} == \"INVALID_ENUM\") return false;", prop.Name);
                //            //statement = string.Format("throw new Invalid(\"GameTable: Invalid enum value(%s-%s-value:%s)!\", \"{0}\", \"{1}\", value.c_str());", ClassName, prop.Name);
                //        }
                //        else if (typeInfo.Name == "Int64")
                //        {
                //            statement = string.Format("this->{0} = _atoi64( value.c_str() );", prop.Name);
                //        }
                //        else
                //        {
                //            statement = "-------------- Error type not supported --------------------";
                //        }
                //        WriteStatement(statement);
                //        WriteStatement("return true;");

                //        CloseSection();
                //        ifPrefix = "else ";
                //    }

                //    WriteStatement("return __super::SetAttributeValue(name,value);");
                //}
                //CloseSection();

                //WriteStatement(string.Format("void {0}::AddChild( DOMElement *pChild )", elementInfo.OutputTypeName));
                //OpenSection();
                //    WriteStatement("return __super::AddChild(pChild);");
                //CloseSection();

            });

            NewLine(1);
        }
    }
}