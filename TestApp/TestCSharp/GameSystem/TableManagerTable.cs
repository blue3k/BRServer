using System;
using System.Reflection;
using System.Collections;
using System.Collections.Generic;
using BR;

namespace BR.GameSystem
{
    // Generic table T:table, D:data, K:key
    public class GenericTable<TTable, TTableElement, TKey> : IEnumerable
        where TTableElement : class
        where TTable : class
    {
        private Dictionary<TKey, TTableElement> m_TableData = new Dictionary<TKey, TTableElement>();

        //public T Table { get { return table; }}
        public TTableElement[] SeqData { get; private set; }



        public GenericTable(string strElementName, string strKeyName, string strTablePath)
        {
            LoadTable(strElementName, strKeyName, strTablePath);
        }

        public void LoadTable(string strElementName, string strKeyName, string strTablePath)
        {
            TTable table = TableManager.LoadXML<TTable>(strTablePath);
            PropertyInfo propElements = table.GetType().GetProperty(strElementName, typeof(TTableElement[]));
            PropertyInfo propKey = typeof(TTableElement).GetProperty(strKeyName, typeof(TKey));
            if (propElements == null || propKey == null)
            {
                BRDebug.Error("Failed to load tabel(" + typeof(TTable).Name + "): Invalid property names, " + strElementName + ", " + strKeyName);
                return;
            }

            TTableElement[] elements = propElements.GetValue(table, null) as TTableElement[];
            SeqData = elements;

            // Register all data to table
            foreach (TTableElement data in elements)
            {
                TKey keyVal = (TKey)propKey.GetValue(data, null);
                if (m_TableData.ContainsKey(keyVal))
                {
                    BRDebug.Error("Tabel(" + typeof(TTable).Name + ") has duplicated key value(" + keyVal + ")!");
                    continue;
                }
                m_TableData.Add(keyVal, data);
            }
        }

        public TTableElement Find(TKey key)
        {
            TTableElement data;
            if (!m_TableData.TryGetValue(key, out data))
            {
                BRDebug.Warning("Table(" + typeof(TTable).Name + "):Invalid key(" + key + ")!");
                return null;
            }

            return data;
        }

        public bool Contain(TKey key)
        {
            if (!m_TableData.ContainsKey(key))
                return false;

            return true;
        }



        IEnumerator IEnumerable.GetEnumerator()
        {
            return GetEnumerator();
        }

        IEnumerator<TTableElement> GetEnumerator()
        {
            foreach (var pair in m_TableData)
            {
                yield return pair.Value;
            }
        }
    }

    // Composite key table T:table, D:data
    public class TableWithCompositeKey<T, D>
        where T : class
        where D : class
    {
        private T table = null;
        private Dictionary<UInt64, D> tableData = new Dictionary<UInt64, D>();

        public T Table { get { return table; } }

        public TableWithCompositeKey(T table, Action<T, Dictionary<UInt64, D>> Initializer)
        {
            if (table == null)
            {
                BRDebug.Error("Failed to load " + typeof(T).Name + " table!");
                return;
            }

            this.table = table;

            Initializer(this.table, tableData);
        }

        public D Find(uint key1, uint key2)
        {
            D data;
            if (!tableData.TryGetValue(TableManager.MakeCompositeKey(key1, key2), out data))
            {
                BRDebug.Error("Table(" + typeof(T).Name + "):Invalid key1(" + key1 + "), key2(" + key2 + ")!");
                return null;
            }

            return data;
        }

        public bool Contain(uint key1, uint key2)
        {
            if (!tableData.ContainsKey(TableManager.MakeCompositeKey(key1, key2)))
                return false;

            return true;
        }
    }

    // Second key table T:table, D:data, FK:first key, SK:second key
    public class TableWithSecondKey<T, D, FK, SK>
        where T : class
        where D : class
    {
        private T table = null;
        private Dictionary<FK, Dictionary<SK, D>> tableData = new Dictionary<FK, Dictionary<SK, D>>();

        public T Table { get { return table; } }

        public TableWithSecondKey(T table, Action<T, Dictionary<FK, Dictionary<SK, D>>> Initializer)
        {
            if (table == null)
            {
                BRDebug.Error("Failed to load " + typeof(T).Name + " table!");
                return;
            }

            this.table = table;

            Initializer(this.table, tableData);
        }

        public Dictionary<SK, D> Find(FK firstKey)
        {
            Dictionary<SK, D> dataList;
            if (!tableData.TryGetValue(firstKey, out dataList))
            {
                BRDebug.Error("Table(" + typeof(T).Name + "):Invalid firstKey(" + firstKey + ")!");
                return null;
            }

            return dataList;
        }

        public D Find(FK firstKey, SK secondKey)
        {
            Dictionary<SK, D> dataList;
            if (!tableData.TryGetValue(firstKey, out dataList))
            {
                BRDebug.Error("Table(" + typeof(T).Name + "):Invalid firstKey(" + firstKey + ")!");
                return null;
            }

            D data;
            if (!dataList.TryGetValue(secondKey, out data))
            {
                BRDebug.Error("Table(" + typeof(T).Name + "):Invalid secondKey(" + secondKey + ")!");
                return null;
            }

            return data;
        }

        public bool Contain(FK firstKey)
        {
            if (!tableData.ContainsKey(firstKey))
                return false;

            return true;
        }

        public bool Contain(FK firstKey, SK secondKey)
        {
            Dictionary<SK, D> dataList;
            if (!tableData.TryGetValue(firstKey, out dataList))
                return false;

            if (!dataList.ContainsKey(secondKey))
                return false;

            return true;
        }
    }


    // Second key table T:table, D:data, FK:first key, SK:second key
    public class TableWithSecondCompositeKey<T, D, FK>
        where T : class
        where D : class
    {
        private T table = null;
        private Dictionary<FK, Dictionary<UInt64, D>> tableData = new Dictionary<FK, Dictionary<UInt64, D>>();

        public T Table { get { return table; } }

        public TableWithSecondCompositeKey(T table, Action<T, Dictionary<FK, Dictionary<UInt64, D>>> Initializer)
        {
            if (table == null)
            {
                BRDebug.Error("Failed to load " + typeof(T).Name + " table!");
                return;
            }

            this.table = table;

            Initializer(this.table, tableData);
        }

        public Dictionary<UInt64, D> Find(FK firstKey)
        {
            Dictionary<UInt64, D> dataList;
            if (!tableData.TryGetValue(firstKey, out dataList))
            {
                BRDebug.Error("Table(" + typeof(T).Name + "):Invalid firstKey(" + firstKey + ")!");
                return null;
            }

            return dataList;
        }

        public D Find(FK firstKey, uint secondKey1, uint secondKey2)
        {
            Dictionary<UInt64, D> dataList;
            if (!tableData.TryGetValue(firstKey, out dataList))
            {
                BRDebug.Error("Table(" + typeof(T).Name + "):Invalid firstKey(" + firstKey + ")!");
                return null;
            }

            D data;
            if (!dataList.TryGetValue(TableManager.MakeCompositeKey(secondKey1, secondKey2), out data))
            {
                BRDebug.Error("Table(" + typeof(T).Name + "):Invalid secondKey1(" + secondKey1 + "), Invalid secondKey2(" + secondKey2 + ")!");
                return null;
            }

            return data;
        }

        public bool Contain(FK firstKey)
        {
            if (!tableData.ContainsKey(firstKey))
                return false;

            return true;
        }

        public bool Contain(FK firstKey, uint secondKey1, uint secondKey2)
        {
            Dictionary<UInt64, D> dataList;
            if (!tableData.TryGetValue(firstKey, out dataList))
                return false;

            if (!dataList.ContainsKey(TableManager.MakeCompositeKey(secondKey1, secondKey2)))
                return false;

            return true;
        }
    }


    // Third key table T:table, D:data, FK:first key, SK:second key, TK:third key
    public class TableWithThirdKey<T, D, FK, SK, TK>
        where T : class
        where D : class
    {
        private T table = null;
        private Dictionary<FK, Dictionary<SK, Dictionary<TK, D>>> tableData = new Dictionary<FK, Dictionary<SK, Dictionary<TK, D>>>();

        public T Table { get { return table; } }

        public TableWithThirdKey(T table, Action<T, Dictionary<FK, Dictionary<SK, Dictionary<TK, D>>>> Initializer)
        {
            if (table == null)
            {
                BRDebug.Error("Failed to load " + typeof(T).Name + " table!");
                return;
            }

            this.table = table;

            Initializer(this.table, tableData);
        }

        public Dictionary<SK, Dictionary<TK, D>> Find(FK firstKey)
        {
            Dictionary<SK, Dictionary<TK, D>> dataList;
            if (!tableData.TryGetValue(firstKey, out dataList))
            {
                BRDebug.Error("Table(" + typeof(T).Name + "):Invalid firstKey(" + firstKey + ")!");
                return null;
            }

            return dataList;
        }

        public Dictionary<TK, D> Find(FK firstKey, SK secondKey)
        {
            Dictionary<SK, Dictionary<TK, D>> dataList;
            if (!tableData.TryGetValue(firstKey, out dataList))
            {
                BRDebug.Error("Table(" + typeof(T).Name + "):Invalid firstKey(" + firstKey + ")!");
                return null;
            }

            Dictionary<TK, D> secondDataList;
            if (!dataList.TryGetValue(secondKey, out secondDataList))
            {
                BRDebug.Error("Table(" + typeof(T).Name + "):Invalid secondKey(" + secondKey + ")!");
                return null;
            }

            return secondDataList;
        }

        public D Find(FK firstKey, SK secondKey, TK thirdKey)
        {
            Dictionary<SK, Dictionary<TK, D>> dataList;
            if (!tableData.TryGetValue(firstKey, out dataList))
            {
                BRDebug.Error("Table(" + typeof(T).Name + "):Invalid firstKey(" + firstKey + ")!");
                return null;
            }

            Dictionary<TK, D> secondDataList;
            if (!dataList.TryGetValue(secondKey, out secondDataList))
            {
                BRDebug.Error("Table(" + typeof(T).Name + "):Invalid secondKey(" + secondKey + ")!");
                return null;
            }

            D data;
            if (!secondDataList.TryGetValue(thirdKey, out data))
            {
                BRDebug.Error("Table(" + typeof(T).Name + "):Invalid thirdKey(" + thirdKey + ")!");
                return null;
            }

            return data;
        }

        public bool Contain(FK firstKey)
        {
            if (!tableData.ContainsKey(firstKey))
                return false;

            return true;
        }

        public bool Contain(FK firstKey, SK secondKey)
        {
            Dictionary<SK, Dictionary<TK, D>> dataList;
            if (!tableData.TryGetValue(firstKey, out dataList))
                return false;

            if (!dataList.ContainsKey(secondKey))
                return false;

            return true;
        }

        public bool Contain(FK firstKey, SK secondKey, TK thirdKey)
        {
            Dictionary<SK, Dictionary<TK, D>> dataList;
            if (!tableData.TryGetValue(firstKey, out dataList))
                return false;

            Dictionary<TK, D> secondDataList;
            if (!dataList.TryGetValue(secondKey, out secondDataList))
                return false;

            if (!secondDataList.ContainsKey(thirdKey))
                return false;

            return true;
        }
    }
}