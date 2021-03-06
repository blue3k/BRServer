﻿using System;
using System.Collections.Generic;

namespace TableBuilder
{
    internal partial class CPPDBTableBuilder
    {
        protected override void DeclareEnum(string name, Type typeInfo)
        {
            NewLine(1);
            string statement;
            statement = string.Format("// {0} enum class defition", name);
            WriteStatement(statement);

            OpenSection("class", name);
            WriteStatement("public:", -1);
            OpenSection("enum class", "Enum");

            // Declare enum
            foreach (string strEnum in Enum.GetNames(typeInfo))
            {
                statement = strEnum + ",";
                WriteStatement(statement);
            }
            WriteStatement("INVALID_ENUM,");
            CloseSection();

            // Declare enum class method
            NewLine(1);
            WriteStatement("private:", -1);
            OpenSection("class", "EnumMap : public std::unordered_map<std::string, Enum>");
            WriteStatement("public:", -1);
            WriteStatement("EnumMap();");
            CloseSection();
            NewLine(1);
            WriteStatement("typedef EnumMap::iterator EnumMapItr;", 0);
            NewLine(1);
            WriteStatement("public:", -1);
            WriteStatement("Enum operator=(int i);", 0);
            WriteStatement("operator int () const;", 0);
            WriteStatement("operator Enum () const;", 0);
            NewLine(1);
            WriteStatement("Enum operator=(const char* sz);", 0);
            WriteStatement("bool operator==(const char* sz);", 0);
            WriteStatement("operator const char* () const;", 0);
            NewLine(1);
            WriteStatement("static Enum AsEnum(const char* sz);", 0);
            WriteStatement("static Enum AsEnum(Enum e);", 0);
            WriteStatement("static const char* AsName(Enum e);", 0);

            // Declare enum class member
            NewLine(1);
            //WriteStatement("private:", -1);
            WriteStatement("union { int m_iEnum; Enum m_Enum; };", 0);
            WriteStatement("static EnumMap m_Map;", 0);

            CloseSection();
        }

        protected override void DeclareClass(string name, Type typeInfo)
        {
            NewLine(1);
            string statement;
            statement = string.Format("// {0} structure defition", name);
            WriteStatement(statement);

            OpenSection("struct", name);

            //WriteStatement("public:", -1);

            //OpenSection("", string.Format("{0}::{0}() : BR::XML::DOMElement(\"{0}\")", name));

            //CloseSection();

            foreach (System.Reflection.PropertyInfo prop in typeInfo.GetProperties())
            {
                string typeName;
                if (HasIgnoreAttribute(prop))
                    continue;

                if ((prop.PropertyType.IsEnum || prop.PropertyType.IsClass) &&
                    prop.PropertyType != "".GetType())
                {
                    if (prop.PropertyType.IsArray)
                    {
                        typeName = GetTypeName(prop.PropertyType.Name.Substring(0, prop.PropertyType.Name.Length - 2));
                        statement = string.Format("std::vector<{0}*> m_{1}s;", typeName, prop.Name);
                    }
                    else
                    {
                        typeName = GetTypeName(prop.PropertyType);
                        statement = typeName + "\t" + prop.Name + ";";
                    }

                    WriteStatement(statement);
                }
                else
                {
                    typeName = GetBuiltInType(prop.PropertyType.Name);
                    if(typeName == "std::string")
                        statement = "char \t" + prop.Name + "[256];";
                    else
                        statement = typeName + "\t" + prop.Name + ";";

                    WriteStatement(statement);
                }
            }

            NewLine(1);

            // constructor
            //WriteStatement(string.Format("{0}();",name));

            //WriteStatement("bool SetAttributeValue( const std::string& name, const std::string& value ) override;");
            //WriteStatement("void AddChild( DOMElement *pChild ) override;");


            CloseSection();
        }

        protected override void DefineMemberVariables(Type typeInfo)
        {
            NewLine(1);

            foreach (KeyValuePair<string, KeyInfo> kvp in _keyInfos)
            {
                KeyInfo KeyInformation = kvp.Value;
                string tableTypeName = kvp.Value.GetMapTypeName();
                string tableVarName = kvp.Value.GetMapVarName();

                if (KeyInformation.keyType == KeyType.EKEY_UNIQUE)
                    WriteStatement("typedef std::unordered_map<{0}, {1}*> {2};", 
                        GetBuiltInType(KeyInformation._keyTypeName1), RowTypeName, tableTypeName);
                else if (KeyInformation.keyType == KeyType.EKEY_NONUNIQUE)
                    WriteStatement("typedef std::unordered_multimap<{0}, {1}*> {2};", 
                        GetBuiltInType(KeyInformation._keyTypeName1), RowTypeName, tableTypeName);
                else if (KeyInformation.keyType == KeyType.EKEY_COMPOSIT)
                    WriteStatement("typedef std::unordered_map<ULONGLONG, {0}*> {1};", 
                        RowTypeName, tableTypeName);

                WriteStatement("typedef {0}::iterator {0}Itr;", tableTypeName);
                WriteStatement("static {0} *{1};", tableTypeName, tableVarName);
                WriteStatement("static {0} *{1}Prev;", tableTypeName, tableVarName);
            }
        }


        protected override void DeclareMemberFunction()
        {
            NewLine(1);
            string statement = "";


            NewLine(1);

            //WriteStatement("private:", -1);
            //WriteStatement("virtual void AddChild( BR::XML::DOMElement *pChild );");

            //NewLine(2);
            //WriteStatement("public:", -1);
            // constructor
            //WriteStatement(string.Format("{0}();", ClassName));
            NewLine(1);
            WriteStatement("// declare member function", 0);
            //WriteStatement("static BR::Result LoadTable( const char *strFileName );", 0);
            statement = string.Format("static BR::Result LoadTable( const std::list<{0}>& rowList );", RowTypeName);
            WriteStatement(statement, 0);
            NewLine(1);

            if (ThisTableType == TableType.TTYPE_SINGLEKEY)
            {
                if (KeyInformation.keyType == KeyType.EKEY_UNIQUE)
                {
                    statement = string.Format("static BR::Result FindItem( const {0}& Key, {1}*& pRow);",
                        GetBuiltInType(KeyInformation._keyTypeName1), RowTypeName);
                }
                else if (KeyInformation.keyType == KeyType.EKEY_NONUNIQUE)
                {
                    statement = string.Format("static BR::Result FindItem( const {0}& Key, std::tr1::unordered_set<{1}*>& rows);",
                        GetBuiltInType(KeyInformation._keyTypeName1), RowTypeName);
                }
                else if (KeyInformation.keyType == KeyType.EKEY_COMPOSIT)
                {
                    statement = string.Format("static BR::Result FindItem( ULONGLONG Key, {0}*& pRow);",
                        RowTypeName);
                }

                WriteStatement(statement);
            }
            else
            {
                foreach (KeyValuePair<string, KeyInfo> kvp in _keyInfos)
                {
                    NewLine(1);
                    KeyInfo KeyInformation  = kvp.Value;
                    string keyName          = KeyInformation._keyName1;
                    statement = string.Format("static BR::Result ClearTable{0}();", keyName);
                    WriteStatement(statement);
                    if (KeyInformation.keyType == KeyType.EKEY_UNIQUE)
                    {
                        statement = string.Format("static BR::Result FindItem{2}( const {0}& Key, {1}*& pRow);",
                            GetBuiltInType(KeyInformation._keyTypeName1), RowTypeName, keyName);
                    }
                    else if (KeyInformation.keyType == KeyType.EKEY_NONUNIQUE)
                    {
                        statement = string.Format("static BR::Result FindItem{2}( const {0}& Key, std::tr1::unordered_set<{1}*>& rows);",
                            GetBuiltInType(KeyInformation._keyTypeName1), RowTypeName, keyName);
                    }
                    else if (KeyInformation.keyType == KeyType.EKEY_COMPOSIT)
                    {
                        statement = string.Format("static BR::Result FindItem{1}( ULONGLONG Key, {0}*& pRow);",
                            RowTypeName, keyName);
                    }

                    WriteStatement(statement);
                }
            }
        }
    }
}
