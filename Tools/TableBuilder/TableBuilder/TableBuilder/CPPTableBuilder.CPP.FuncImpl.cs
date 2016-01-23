using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace TableBuilder
{
    internal partial class CPPTableBuilder
	{
        private void WriteClearTable(KeyInfo keyInfo)
        {
            string statement;
            statement = string.Format("HRESULT {0}::ClearTable()", ClassName);
            WriteStatement(statement);
            OpenSection();
            WriteStatement("for (TableMapItr itr = m_TableMap.begin(); itr != m_TableMap.end(); ++itr)");
            WriteStatement("delete itr->second;", 1);
            NewLine(1);
            WriteStatement("m_TableMap.clear();");
            WriteStatement("return S_SYSTEM_OK;");
            CloseSection();
        }

        private void WriteFindItem(KeyInfo keyInfo)
        {
            string statement;
            string tableName        = keyInfo._keyName1 + "Table";
            string tableTypeName    = keyInfo._keyName1 + "TableMap";

            if (keyInfo.keyType == KeyType.EKEY_UNIQUE)
            {
                statement = string.Format("HRESULT {0}::FindItem( const {1}& Key, {2}*& pRow)", 
                    ClassName, GetBuiltInType(keyInfo._keyTypeName1), RowTypeName);
                WriteStatement(statement);
                OpenSection();
                WriteStatement("TableMapItr itr = m_TableMap.find(Key);");
                WriteStatement("if (itr == m_TableMap.end())");
                OpenSection();
                WriteStatement("// write error log");
                WriteStatement("return E_SYSTEM_FAIL;");
                CloseSection();

                WriteStatement("pRow = itr->second;");
                WriteStatement("return S_SYSTEM_OK;");
                CloseSection();
            }
            else if (keyInfo.keyType == KeyType.EKEY_NONUNIQUE)
            {
                statement = string.Format("HRESULT {0}::FindItem( const {1}& Key, std::tr1::unordered_set<{2}*>& rows)",
                    ClassName, GetBuiltInType(keyInfo._keyTypeName1), RowTypeName);
                WriteStatement(statement);
                OpenSection();
                statement = string.Format("std::pair<{0}::TableMapItr, {0}::TableMapItr> pair1 = m_TableMap.equal_range(Key);", ClassName);
                WriteStatement(statement);
                WriteStatement("if (pair1.first == pair1.second)");
                OpenSection();
                WriteStatement("// write error log");
                WriteStatement("return E_SYSTEM_FAIL;");
                CloseSection();
                WriteStatement("for (; pair1.first != pair1.second; ++pair1.first)");
                WriteStatement("rows.insert(pair1.first->second);", 1);
                WriteStatement("return S_SYSTEM_OK;");
                CloseSection();
            }
            else if (keyInfo.keyType == KeyType.EKEY_COMPOSIT)
            {
                statement = string.Format("HRESULT {0}::FindItem( ULONGLONG Key, {1}*& pRow)", 
                    ClassName, RowTypeName);
                WriteStatement(statement);
                OpenSection();
                WriteStatement("TableMapItr itr = m_TableMap.find(Key);");
                WriteStatement("if (itr == m_TableMap.end())");
                OpenSection();
                WriteStatement("// write error log");
                WriteStatement("return E_SYSTEM_FAIL;");
                CloseSection();

                WriteStatement("pRow = itr->second;");
                WriteStatement("return S_SYSTEM_OK;");
                CloseSection();
            }
        }

        private void WriteMultiKeyClearTable(KeyInfo keyInfo)
        {
            string statement;
            string tableName = keyInfo._keyName1 + "Table";
            string tableTypeName = keyInfo._keyName1 + "TableMap";
            string keyName = keyInfo._keyName1;

            statement = string.Format("HRESULT {0}::ClearTable{1}()", ClassName, keyName);
            WriteStatement(statement);
            OpenSection();
            statement = string.Format("for ({0}Itr itr = {1}.begin(); itr != {1}.end(); ++itr)",
                tableTypeName, tableName);
            WriteStatement(statement);
            WriteStatement("delete itr->second;", 1);
            NewLine(1);
            statement = string.Format("{0}.clear();", tableName);
            WriteStatement(statement);
            WriteStatement("return S_SYSTEM_OK;");
            CloseSection();
        }

        private void WriteMultiKeyFindItem(KeyInfo keyInfo)
        {
            string statement;

            string keyName = keyInfo._keyName1;
            string tableName        = keyInfo._keyName1 + "Table";
            string tableTypeName    = keyInfo._keyName1 + "TableMap";

            if (keyInfo.keyType == KeyType.EKEY_UNIQUE)
            {
                statement = string.Format("HRESULT {0}::FindItem{3}( const {1}& Key, {2}*& pRow)", 
                    ClassName, GetBuiltInType(keyInfo._keyTypeName1), RowTypeName, keyName);
                WriteStatement(statement);
                OpenSection();
                statement = string.Format("{0}Itr itr = {1}.find(Key);", tableTypeName, tableName);
                WriteStatement(statement);
                statement = string.Format("if (itr == {0}.end())", tableName);
                WriteStatement(statement);
                OpenSection();
                WriteStatement("// write error log");
                WriteStatement("return E_SYSTEM_FAIL;");
                CloseSection();

                WriteStatement("pRow = itr->second;");
                WriteStatement("return S_SYSTEM_OK;");
                CloseSection();
            }
            else if (keyInfo.keyType == KeyType.EKEY_NONUNIQUE)
            {
                statement = string.Format("HRESULT {0}::FindItem{3}( const {1}& Key, std::tr1::unordered_set<{2}*>& rows)", 
                    ClassName, GetBuiltInType(keyInfo._keyTypeName1), RowTypeName, keyName);
                WriteStatement(statement);
                OpenSection();
                statement = string.Format("std::pair<{0}::{2}Itr, {0}::{2}Itr> pair1 = {1}.equal_range(Key);", 
                    ClassName, tableName, tableTypeName);
                WriteStatement(statement);
                WriteStatement("if (pair1.first == pair1.second)");
                OpenSection();
                WriteStatement("// write error log");
                WriteStatement("return E_SYSTEM_FAIL;");
                CloseSection();
                WriteStatement("for (; pair1.first != pair1.second; ++pair1.first)");
                WriteStatement("rows.insert(pair1.first->second);", 1);
                WriteStatement("return S_SYSTEM_OK;");
                CloseSection();
            }
            else if (keyInfo.keyType == KeyType.EKEY_COMPOSIT)
            {
                statement = string.Format("HRESULT {0}::FindItem{2}( ULONGLONG Key, {1}*& pRow)", 
                    ClassName, RowTypeName, keyName);
                WriteStatement(statement);
                OpenSection();
                statement = string.Format("{0}Itr itr = {1}.find(Key);", tableTypeName, tableName);
                WriteStatement(statement);
                statement = string.Format("if (itr == {0}.end())", tableName);
                WriteStatement(statement);
                OpenSection();
                WriteStatement("// write error log");
                WriteStatement("return E_SYSTEM_FAIL;");
                CloseSection();

                WriteStatement("pRow = itr->second;");
                WriteStatement("return S_SYSTEM_OK;");
                CloseSection();
            }
        }

        private void WriteMultiKeyClearTable()
        {
            string statement;

            statement = string.Format("HRESULT {0}::ClearTable()", ClassName);
            WriteStatement(statement);
            OpenSection();
            foreach (KeyValuePair<string, KeyInfo> kvp in _keyInfos)
            {
                string keyName = kvp.Value._keyName1;
                statement = string.Format("ClearTable{0}();", keyName);
                WriteStatement(statement);
            }
            WriteStatement("return S_SYSTEM_OK;");
            CloseSection();
        }
	}
}
