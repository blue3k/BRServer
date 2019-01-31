using System;
using System.Collections.Generic;

namespace TableBuilder
{
    internal partial class UnrealTableBuilder
    {
        protected override void DeclareEnum(string name, Type typeInfo)
        {
            NewLine(1);
            string statement;
            OpenSection("enum", name);

            foreach (string strEnum in Enum.GetNames(typeInfo))
            {
                statement = name + "_" + strEnum + ",";
                WriteStatement(statement);
            }
            CloseSection(); 
        }

        protected override void DeclareClass(string name, Type typeInfo)
        {
            NewLine(1);
            string statement;
            statement = string.Format("// {0} structure defition", name);
            WriteStatement(statement);

            OpenSection("struct native", name);

            foreach (System.Reflection.PropertyInfo prop in typeInfo.GetProperties())
            {
                string typeName;
                if ((prop.PropertyType.IsEnum || prop.PropertyType.IsClass) &&
                    prop.PropertyType != "".GetType())
                {
                    if (prop.PropertyType.IsArray)
                    {
                        typeName = prop.PropertyType.Name.Substring(0, prop.PropertyType.Name.Length - 2);
                        statement = string.Format("var array<{0}> {1}s;", typeName, prop.Name);
                    }
                    else
                    {
                        typeName = GetTypeName(prop.PropertyType);
                        statement = "var " + typeName + "\t" + prop.Name + ";";
                    }
                  
                    WriteStatement(statement);
                }
                else
                {
                    typeName = GetBuiltInType(prop.PropertyType.Name);
                    statement = "var " + typeName + "\t" + prop.Name + ";";
                    WriteStatement(statement);
                }
            }
            CloseSection();
        }

        protected override void DefineMemberVariables(Type typeInfo)
        {
            NewLine(1);
            string statement = "";

            if (ThisTableType == TableType.TTYPE_SINGLEKEY)
            {
                KeyInfo KeyInformation = this["first"];

                if (KeyInformation.keyType == KeyType.EKEY_UNIQUE)
                {
                    statement = string.Format("var native map{{{0}, F{1}*}} TableMap;",
                        GetCPPBuiltInType(KeyInformation._keyTypeName1), RowTypeName);
                }
                else if (KeyInformation.keyType == KeyType.EKEY_NONUNIQUE)
                {
                    statement = string.Format("var native map{{{0}, TArray<F{1}*>}} TableMap;",
                        GetCPPBuiltInType(KeyInformation._keyTypeName1), RowTypeName);
                }
                else if (KeyInformation.keyType == KeyType.EKEY_COMPOSIT)
                {
                    statement = string.Format("var native map{{ULONGLONG, F{0}*}} TableMap;",
                        RowTypeName);
                }

                WriteStatement(statement);
            }
            else
            {
                foreach (KeyValuePair<string, KeyInfo> kvp in _keyInfos)
                {
                    KeyInfo KeyInformation = kvp.Value;
                    string tableName =  KeyInformation._keyName1 + "Table";

                    if (KeyInformation.keyType == KeyType.EKEY_UNIQUE)
                    {
                        statement = string.Format("var native map{{{0}, F{1}*}} {2};",
                            GetCPPBuiltInType(KeyInformation._keyTypeName1), RowTypeName, tableName);
                    }
                    else if (KeyInformation.keyType == KeyType.EKEY_NONUNIQUE)
                    {
                        statement = string.Format("var native map{{{0}, TArray<F{1}*>}} {2};",
                            GetCPPBuiltInType(KeyInformation._keyTypeName1), RowTypeName, tableName);
                    }
                    else if (KeyInformation.keyType == KeyType.EKEY_COMPOSIT)
                    {
                        statement = string.Format("var native map{{ULONGLONG, F{0}*}} {1};",
                            RowTypeName, tableName);
                    }

                    WriteStatement(statement);
                }
            }
         
            
        }

        protected override void DeclareMemberFunction()
        {
            NewLine(1);
            string statement = "";

            WriteStatement("native function bool LoadTable( string strFileName );");
            WriteStatement("native function bool ClearTable();");

            if (ThisTableType == TableType.TTYPE_SINGLEKEY)
            {
                KeyInfo KeyInformation = this["first"];

                if (KeyInformation.keyType == KeyType.EKEY_UNIQUE)
                    statement = string.Format("native function bool FindItem( {0} key, out {1} row );",
                        GetBuiltInType(KeyInformation._keyTypeName1), RowTypeName);
                else if (KeyInformation.keyType == KeyType.EKEY_NONUNIQUE)
                    statement = string.Format("native function bool FindItem( {0} key, out array<{1}> rows );",
                        GetBuiltInType(KeyInformation._keyTypeName1), RowTypeName);
                else if (KeyInformation.keyType == KeyType.EKEY_COMPOSIT)
                    statement = string.Format("native function bool FindItem( int key1, int key2,  out {0} row);", RowTypeName);
                WriteStatement(statement);

                if (KeyInformation.keyType == KeyType.EKEY_UNIQUE || KeyInformation.keyType == KeyType.EKEY_COMPOSIT)
                    statement = string.Format("native noexport final iterator function AllRows( out {0} rows );", RowTypeName);
                else if (KeyInformation.keyType == KeyType.EKEY_NONUNIQUE)
                    statement = string.Format("native noexport final iterator function AllRows( out array<{0}> rows );", RowTypeName);
                WriteStatement(statement);
            }
            else
            {
                foreach (KeyValuePair<string, KeyInfo> kvp in _keyInfos)
                {
                    KeyInfo KeyInformation = kvp.Value;
                    string keyName = KeyInformation._keyName1;

                    statement = string.Format("native function bool ClearTable{0}();", keyName);
                    WriteStatement(statement);

                    if (KeyInformation.keyType == KeyType.EKEY_UNIQUE)
                        statement = string.Format("native function bool FindItem{2}( {0} key, out {1} row );",
                            GetBuiltInType(KeyInformation._keyTypeName1), RowTypeName, keyName);
                    else if (KeyInformation.keyType == KeyType.EKEY_NONUNIQUE)
                        statement = string.Format("native function bool FindItem{2}( {0} key, out array<{1}> rows );",
                            GetBuiltInType(KeyInformation._keyTypeName1), RowTypeName, keyName);
                    else if (KeyInformation.keyType == KeyType.EKEY_COMPOSIT)
                        statement = string.Format("native function bool FindItem{1}( int key1, int key2,  out {0} row);", RowTypeName, keyName);
                    WriteStatement(statement);

                    if (KeyInformation.keyType == KeyType.EKEY_UNIQUE || KeyInformation.keyType == KeyType.EKEY_COMPOSIT)
                        statement = string.Format("native noexport final iterator function AllRows{1}( out {0} rows );", RowTypeName, keyName);
                    else if (KeyInformation.keyType == KeyType.EKEY_NONUNIQUE)
                        statement = string.Format("native noexport final iterator function AllRows{1}( out array<{0}> rows );", RowTypeName, keyName);
                    WriteStatement(statement);
                }
            }
        }
    }
}