using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace TableBuilder
{
    internal partial class UnrealTreeBuilder
    {
        private void WriteClearTable(KeyInfo keyInfo)
        {
            string statement;
            if (keyInfo.keyType == KeyType.EKEY_UNIQUE)
            {
                statement = string.Format("UBOOL {0}::ClearTable()", UnrealGenerateClassName);
                WriteStatement(statement);
                OpenSection();
                statement = string.Format("for (TMap<{0}, {1}* >::TIterator it(TableMap); it; ++it)",
                    GetCPPBuiltInType(keyInfo._keyTypeName1), UnrealGenerateRowTypeName);
                WriteStatement(statement);
                OpenSection();
                WriteStatement("delete it.Value();");
                WriteStatement("it.Value() = NULL;");
                CloseSection();
                WriteStatement("TableMap.Empty();");
                WriteStatement("return TRUE;");
                CloseSection();
            }
            else if (keyInfo.keyType == KeyType.EKEY_NONUNIQUE)
            {
                //statement = string.Format("UBOOL {0}::ClearTable()", UnrealGenerateClassName);
                //WriteStatement(statement);
                //OpenSection();
                //statement = string.Format("for (TMap<{0}, TArray<F{1}*> >::TIterator it(TableMap); it; ++it)",
                //   GetCPPBuiltInType(keyInfo._keyTypeName1), RowTypeName);
                //WriteStatement(statement);
                //OpenSection();
                //WriteStatement("while(it.Value().Num())");
                //OpenSection();
                //statement = string.Format("F{0}* pRow = it.Value().Pop();", RowTypeName);
                //WriteStatement(statement);
                //WriteStatement("if (pRow)");
                //WriteStatement("delete pRow;", 1);
                //CloseSection();
                //WriteStatement("TableMap.Remove(it.Key());");
                //CloseSection();
                //WriteStatement("return TRUE;");
                //CloseSection();
            }
            else if (keyInfo.keyType == KeyType.EKEY_COMPOSIT)
            {
                statement = string.Format("UBOOL {0}::ClearTable()", UnrealGenerateClassName);
                WriteStatement(statement);
                OpenSection();
                statement = string.Format("for (TMap<ULONGLONG, {0}* >::TIterator it(TableMap); it; ++it)",
                    UnrealGenerateRowTypeName);
                WriteStatement(statement);
                OpenSection();
                WriteStatement("delete it.Value();");
                WriteStatement("it.Value() = NULL;");
                CloseSection();
                WriteStatement("TableMap.Empty();");
                WriteStatement("return TRUE;");
                CloseSection();
            }
        }

        private void WriteFindItem(KeyInfo keyInfo)
        {
            string statement;

            if (keyInfo.keyType == KeyType.EKEY_UNIQUE)
            {
                if (keyInfo._keyTypeName1 == "String")
                    statement = string.Format("UBOOL {0}::FindItem(const {1}& Key, struct {2}& Item)",
                           UnrealGenerateClassName, GetCPPBuiltInType(keyInfo._keyTypeName1),
                           UnrealGenerateRowTypeName);
                else
                    statement = string.Format("UBOOL {0}::FindItem({1} Key, struct {2}& Item)",
                       UnrealGenerateClassName, GetCPPBuiltInType(keyInfo._keyTypeName1),
                       UnrealGenerateRowTypeName);
                WriteStatement(statement);
                OpenSection();
                statement = string.Format("{0}** ppRow = TableMap.Find(Key);", UnrealGenerateRowTypeName);
                WriteStatement(statement);
                WriteStatement("if (!ppRow)");
                OpenSection();
                WriteStatement("// write error log");
                WriteStatement("return FALSE;");
                CloseSection();
                WriteStatement("Item = **ppRow;");
                WriteStatement("return TRUE;");
                CloseSection();
            }
            else if (keyInfo.keyType == KeyType.EKEY_NONUNIQUE)
            {
                //if (keyInfo._keyTypeName1 == "String")
                //    statement = string.Format("UBOOL {0}::FindItem(const {1}& Key, struct {2}& Item)",
                //           UnrealGenerateClassName, GetCPPBuiltInType(keyInfo._keyTypeName1),
                //           UnrealGenerateRowTypeName);
                //else
                //    statement = string.Format("UBOOL {0}::FindItem({1} Key,TArray<struct F{2}>& rows)",
                //    UnrealGenerateClassName, GetCPPBuiltInType(keyInfo._keyTypeName1),
                //    RowTypeName);
                //WriteStatement(statement);
                //OpenSection();
                //statement = string.Format("TArray<F{0}*>* pRows = TableMap.Find(Key);",
                //    RowTypeName);
                //WriteStatement(statement);

                //WriteStatement("if (!pRows)");
                //OpenSection();
                //WriteStatement("// error log");
                //WriteStatement("return FALSE;");
                //CloseSection();
                //NewLine(1);
                //statement = string.Format("for (TArray<F{0}*>::TIterator it(*pRows); it; ++it)",
                //    RowTypeName);
                //WriteStatement(statement);
                //WriteStatement("rows.AddItem(**it);", 1);
                //NewLine(1);
                //WriteStatement("return TRUE;");
                //CloseSection();
            }
            else if (keyInfo.keyType == KeyType.EKEY_COMPOSIT)
            {
                //statement = string.Format("UBOOL {0}::FindItem(INT key1,INT key2,struct F{1}& Row)",
                //    UnrealGenerateClassName, RowTypeName);
                //WriteStatement(statement);
                //OpenSection();
                //WriteStatement("ULONGLONG u64CompositKey =  BR::Access::Combine64(key1, key2);");
                //NewLine(1);
                //statement = string.Format("F{0}** ppRow = TableMap.Find(u64CompositKey);", RowTypeName);
                //WriteStatement(statement);
                //WriteStatement("if (!ppRow)");
                //OpenSection();
                //WriteStatement("// write error log");
                //WriteStatement("return FALSE;");
                //CloseSection();
                //WriteStatement("Row = **ppRow;");
                //WriteStatement("return TRUE;");
                //CloseSection();
            }
        }

        private void WriteAllRows(KeyInfo keyInfo)
        {
            //string statement;

            if (keyInfo.keyType == KeyType.EKEY_UNIQUE)
            {
                //statement = string.Format("void {0}::execAllRows( FFrame& Stack, RESULT_DECL )", UnrealGenerateClassName);
                //WriteStatement(statement);
                //OpenSection();
                //statement = string.Format("P_GET_STRUCT_INIT_REF(struct F{0},row);", RowTypeName);
                //WriteStatement(statement);
                //WriteStatement("P_FINISH;");
                //statement = string.Format("TMap<{0}, F{1}* >::TIterator itMap(TableMap);",
                //    GetCPPBuiltInType(keyInfo._keyTypeName1), RowTypeName);
                //WriteStatement(statement);
                //WriteStatement("PRE_ITERATOR;");
                //WriteStatement("// Fetch next row in the iteration.");
                //WriteStatement("if( itMap )");
                //OpenSection();
                //WriteStatement("row = *itMap.Value();");
                //WriteStatement("++itMap;");
                //CloseSection();
                //WriteStatement("else");
                //OpenSection();
                //WriteStatement("Stack.Code = &Stack.Node->Script(wEndOffset + 1);");
                //WriteStatement("break;");
                //CloseSection();
                //WriteStatement("POST_ITERATOR;");
                //CloseSection();
            }
            else if (keyInfo.keyType == KeyType.EKEY_NONUNIQUE)
            {
                //statement = string.Format("void {0}::execAllRows( FFrame& Stack, RESULT_DECL )", UnrealGenerateClassName);
                //WriteStatement(statement);
                //OpenSection();
                //statement = string.Format("P_GET_STRUCT_INIT_REF(TArray<struct F{0}>,ArgRows);", RowTypeName);
                //WriteStatement(statement);
                //WriteStatement("P_FINISH;");
                //statement = string.Format("TMap<{0}, TArray<F{1}*> >::TIterator itMap(TableMap);",
                //    GetCPPBuiltInType(keyInfo._keyTypeName1), RowTypeName);
                //WriteStatement(statement);
                //WriteStatement("PRE_ITERATOR;");
                //WriteStatement("// Fetch next row in the iteration.");
                //WriteStatement("if( itMap )");
                //OpenSection();
                //statement = string.Format("TArray<F{0}*> Rows = itMap.Value();", RowTypeName);
                //WriteStatement(statement);
                //statement = string.Format("for (TArray<F{0}*>::TIterator it(Rows); it; ++it)", RowTypeName);
                //WriteStatement(statement);
                //WriteStatement("ArgRows.AddItem(**it);", 1);
                //WriteStatement("++itMap;");
                //CloseSection();
                //WriteStatement("else");
                //OpenSection();
                //WriteStatement("Stack.Code = &Stack.Node->Script(wEndOffset + 1);");
                //WriteStatement("break;");
                //CloseSection();
                //WriteStatement("POST_ITERATOR;");
                //CloseSection();
            }
            else if (keyInfo.keyType == KeyType.EKEY_COMPOSIT)
            {
                //statement = string.Format("void {0}::execAllRows( FFrame& Stack, RESULT_DECL )", UnrealGenerateClassName);
                //WriteStatement(statement);
                //OpenSection();
                //statement = string.Format("P_GET_STRUCT_INIT_REF(struct F{0},row);", RowTypeName);
                //WriteStatement(statement);
                //WriteStatement("P_FINISH;");
                //statement = string.Format("TMap<ULONGLONG, F{0}* >::TIterator itMap(TableMap);",
                //        RowTypeName);
                //WriteStatement(statement);
                //WriteStatement("PRE_ITERATOR;");
                //WriteStatement("// Fetch next row in the iteration.");
                //WriteStatement("if( itMap )");
                //OpenSection();
                //WriteStatement("row = *itMap.Value();");
                //WriteStatement("++itMap;");
                //CloseSection();
                //WriteStatement("else");
                //OpenSection();
                //WriteStatement("Stack.Code = &Stack.Node->Script(wEndOffset + 1);");
                //WriteStatement("break;");
                //CloseSection();
                //WriteStatement("POST_ITERATOR;");
                //CloseSection();
            }
        }

        private void WriteMultiKeyClearTable(KeyInfo keyInfo, bool bFirstKey)
        {
            string statement;
            string tableName    = keyInfo._keyName1 + "Table";
            string keyName      = keyInfo._keyName1;
            if (keyInfo.keyType == KeyType.EKEY_UNIQUE)
            {
                statement = string.Format("UBOOL {0}::ClearTable{1}()", UnrealGenerateClassName, keyName);
                WriteStatement(statement);
                OpenSection();
                statement = string.Format("for (TMap<{0}, {1}* >::TIterator it({2}); it; ++it)",
                    GetCPPBuiltInType(keyInfo._keyTypeName1), UnrealGenerateRowTypeName, tableName);
                WriteStatement(statement);
                OpenSection();

                if (bFirstKey)
                    WriteStatement("delete it.Value();");
                
                WriteStatement("it.Value() = NULL;");
                CloseSection();
                statement = string.Format("{0}.Empty();", tableName);
                WriteStatement(statement);
                WriteStatement("return TRUE;");
                CloseSection();
            }
            else if (keyInfo.keyType == KeyType.EKEY_NONUNIQUE)
            {
                //statement = string.Format("UBOOL {0}::ClearTable{1}()", UnrealGenerateClassName, keyName);
                //WriteStatement(statement);
                //OpenSection();
                //statement = string.Format("for (TMap<{0}, TArray<F{1}*> >::TIterator it({2}); it; ++it)",
                //   GetCPPBuiltInType(keyInfo._keyTypeName1), RowTypeName, tableName);
                //WriteStatement(statement);
                //OpenSection();
                //WriteStatement("while(it.Value().Num())");
                //OpenSection();
                //statement = string.Format("F{0}* pRow = it.Value().Pop();", RowTypeName);
                //WriteStatement(statement);
                //WriteStatement("if (pRow)");
                //OpenSection();
                //if (bFirstKey)
                //    WriteStatement("delete pRow;", 1);
                //CloseSection();
                //CloseSection();
                //statement = string.Format("{0}.Remove(it.Key());", tableName);
                //WriteStatement(statement);
                //CloseSection();
                //WriteStatement("return TRUE;");
                //CloseSection();
            }
            else if (keyInfo.keyType == KeyType.EKEY_COMPOSIT)
            {
                statement = string.Format("UBOOL {0}::ClearTable{1}()", UnrealGenerateClassName, keyName);
                WriteStatement(statement);
                OpenSection();
                statement = string.Format("for (TMap<ULONGLONG, {0}* >::TIterator it({1}); it; ++it)",
                    UnrealGenerateRowTypeName, tableName);
                WriteStatement(statement);
                OpenSection();
                if (bFirstKey)
                    WriteStatement("delete it.Value();");
                WriteStatement("it.Value() = NULL;");
                CloseSection();
                statement = string.Format("{0}.Empty();", tableName);
                WriteStatement(statement);
                WriteStatement("return TRUE;");
                CloseSection();
            }
        }

        private void WriteMultiKeyFindItem(KeyInfo keyInfo)
        {
            string statement;
            string tableName    = keyInfo._keyName1 + "Table";
            string keyName      = keyInfo._keyName1;
            if (keyInfo.keyType == KeyType.EKEY_UNIQUE)
            {
                if (keyInfo._keyTypeName1 == "String")
                    statement = string.Format("UBOOL {0}::FindItem{3}(const {1}& Key, struct {2}& Item)",
                           UnrealGenerateClassName, GetCPPBuiltInType(keyInfo._keyTypeName1),
                           UnrealGenerateRowTypeName, keyName);
                else
                    statement = string.Format("UBOOL {0}::FindItem{3}({1} Key, struct {2}& Item)",
                       UnrealGenerateClassName, GetCPPBuiltInType(keyInfo._keyTypeName1),
                       UnrealGenerateRowTypeName, keyName);
                WriteStatement(statement);
                OpenSection();
                statement = string.Format("{0}** ppRow = {1}.Find(Key);", UnrealGenerateRowTypeName, tableName);
                WriteStatement(statement);
                WriteStatement("if (!ppRow)");
                OpenSection();
                WriteStatement("// write error log");
                WriteStatement("return FALSE;");
                CloseSection();
                WriteStatement("Item = **ppRow;");
                WriteStatement("return TRUE;");
                CloseSection();
            }
            else if (keyInfo.keyType == KeyType.EKEY_NONUNIQUE)
            {
                //if (keyInfo._keyTypeName1 == "String")
                //    statement = string.Format("UBOOL {0}::FindItem{3}(const {1}& Key, struct {2}& Item)",
                //           UnrealGenerateClassName, GetCPPBuiltInType(keyInfo._keyTypeName1),
                //           UnrealGenerateRowTypeName, keyName);
                //else
                //    statement = string.Format("UBOOL {0}::FindItem{3}({1} Key,TArray<struct F{2}>& rows)",
                //    UnrealGenerateClassName, GetCPPBuiltInType(keyInfo._keyTypeName1),
                //    RowTypeName, keyName);
                //WriteStatement(statement);
                //OpenSection();
                //statement = string.Format("TArray<F{0}*>* pRows = {1}.Find(Key);",
                //    RowTypeName, tableName);
                //WriteStatement(statement);

                //WriteStatement("if (!pRows)");
                //OpenSection();
                //WriteStatement("// error log");
                //WriteStatement("return FALSE;");
                //CloseSection();
                //NewLine(1);
                //statement = string.Format("for (TArray<F{0}*>::TIterator it(*pRows); it; ++it)",
                //    RowTypeName);
                //WriteStatement(statement);
                //WriteStatement("rows.AddItem(**it);", 1);
                //NewLine(1);
                //WriteStatement("return TRUE;");
                //CloseSection();
            }
            else if (keyInfo.keyType == KeyType.EKEY_COMPOSIT)
            {
                //statement = string.Format("UBOOL {0}::FindItem{2}(INT key1,INT key2,struct F{1}& Row)",
                //    UnrealGenerateClassName, RowTypeName, keyName);
                //WriteStatement(statement);
                //OpenSection();
                //WriteStatement("ULONGLONG u64CompositKey =  BR::Access::Combine64(key1, key2);");
                //NewLine(1);
                //statement = string.Format("F{0}** ppRow = {1}.Find(u64CompositKey);", 
                //    RowTypeName, tableName);
                //WriteStatement(statement);
                //WriteStatement("if (!ppRow)");
                //OpenSection();
                //WriteStatement("// write error log");
                //WriteStatement("return FALSE;");
                //CloseSection();
                //WriteStatement("Row = **ppRow;");
                //WriteStatement("return TRUE;");
                //CloseSection();
            }
        }

        private void WriteMultiKeyAllRows(KeyInfo keyInfo)
        {
            //string statement;
            string tableName    = keyInfo._keyName1 + "Table";
            string keyName      = keyInfo._keyName1;
            if (keyInfo.keyType == KeyType.EKEY_UNIQUE)
            {   
                //statement = string.Format("void {0}::execAllRows{1}( FFrame& Stack, RESULT_DECL )", UnrealGenerateClassName, keyName);
                //WriteStatement(statement);
                //OpenSection();
                //statement = string.Format("P_GET_STRUCT_INIT_REF(struct F{0},row);", RowTypeName);
                //WriteStatement(statement);
                //WriteStatement("P_FINISH;");
                //statement = string.Format("TMap<{0}, F{1}* >::TIterator itMap({2});",
                //    GetCPPBuiltInType(keyInfo._keyTypeName1), RowTypeName, tableName);
                //WriteStatement(statement);
                //WriteStatement("PRE_ITERATOR;");
                //WriteStatement("// Fetch next row in the iteration.");
                //WriteStatement("if( itMap )");
                //OpenSection();
                //WriteStatement("row = *itMap.Value();");
                //WriteStatement("++itMap;");
                //CloseSection();
                //WriteStatement("else");
                //OpenSection();
                //WriteStatement("Stack.Code = &Stack.Node->Script(wEndOffset + 1);");
                //WriteStatement("break;");
                //CloseSection();
                //WriteStatement("POST_ITERATOR;");
                //CloseSection();
            }
            else if (keyInfo.keyType == KeyType.EKEY_NONUNIQUE)
            {
                //statement = string.Format("void {0}::execAllRows{1}( FFrame& Stack, RESULT_DECL )", UnrealGenerateClassName, keyName);
                //WriteStatement(statement);
                //OpenSection();
                //statement = string.Format("P_GET_STRUCT_INIT_REF(TArray<struct F{0}>,ArgRows);", RowTypeName);
                //WriteStatement(statement);
                //WriteStatement("P_FINISH;");
                //statement = string.Format("TMap<{0}, TArray<F{1}*> >::TIterator itMap({2});",
                //    GetCPPBuiltInType(keyInfo._keyTypeName1), RowTypeName, tableName);
                //WriteStatement(statement);
                //WriteStatement("PRE_ITERATOR;");
                //WriteStatement("// Fetch next row in the iteration.");
                //WriteStatement("if( itMap )");
                //OpenSection();
                //statement = string.Format("TArray<F{0}*> Rows = itMap.Value();", RowTypeName);
                //WriteStatement(statement);
                //statement = string.Format("for (TArray<F{0}*>::TIterator it(Rows); it; ++it)", RowTypeName);
                //WriteStatement(statement);
                //WriteStatement("ArgRows.AddItem(**it);", 1);
                //WriteStatement("++itMap;");
                //CloseSection();
                //WriteStatement("else");
                //OpenSection();
                //WriteStatement("Stack.Code = &Stack.Node->Script(wEndOffset + 1);");
                //WriteStatement("break;");
                //CloseSection();
                //WriteStatement("POST_ITERATOR;");
                //CloseSection();
            }
            else if (keyInfo.keyType == KeyType.EKEY_COMPOSIT)
            {
                //statement = string.Format("void {0}::execAllRows{1}( FFrame& Stack, RESULT_DECL )", UnrealGenerateClassName, keyName);
                //WriteStatement(statement);
                //OpenSection();
                //statement = string.Format("P_GET_STRUCT_INIT_REF(struct F{0},row);", RowTypeName);
                //WriteStatement(statement);
                //WriteStatement("P_FINISH;");
                //statement = string.Format("TMap<ULONGLONG, F{0}* >::TIterator itMap({1});",
                //    RowTypeName, tableName);
                //WriteStatement(statement);
                //WriteStatement("PRE_ITERATOR;");
                //WriteStatement("// Fetch next row in the iteration.");
                //WriteStatement("if( itMap )");
                //OpenSection();
                //WriteStatement("row = *itMap.Value();");
                //WriteStatement("++itMap;");
                //CloseSection();
                //WriteStatement("else");
                //OpenSection();
                //WriteStatement("Stack.Code = &Stack.Node->Script(wEndOffset + 1);");
                //WriteStatement("break;");
                //CloseSection();
                //WriteStatement("POST_ITERATOR;");
                //CloseSection();
            }
        }

        private void WriteMultiKeyClearTable()
        {
            //string statement;
            //statement = string.Format("UBOOL {0}::ClearTable()", UnrealGenerateClassName);
            //WriteStatement(statement);
            //OpenSection();
            //foreach (KeyValuePair<string, KeyInfo> kvp in _keyInfos)
            //{
            //    string keyName = kvp.Value._keyName1;
            //    statement = string.Format("ClearTable{0}();", keyName);
            //    WriteStatement(statement);
            //}
            //WriteStatement("return TRUE;");
            //CloseSection();
        }
    }
}
