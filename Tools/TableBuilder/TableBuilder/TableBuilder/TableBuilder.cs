using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace TableBuilder
{
    internal abstract class TableBuilder : Builder
    {
        public TableBuilder(string strBasePath, KeyInfo keyinfo,
                        string baseParserName) :
            base(strBasePath, baseParserName)
        {
            _keyInfo            = keyinfo;
            _keyInfos["first"]  = keyinfo;
            _tableType          = TableType.TTYPE_SINGLEKEY;
        }

        public TableBuilder(string strBasePath, KeyInfo keyinfo1, 
            KeyInfo keyinfo2, string baseParserName) :
            base(strBasePath, baseParserName)
        {
            _keyInfos["first"]  = keyinfo1;
            _keyInfos["second"] = keyinfo2;
            _tableType = TableType.TTYPE_MULTIKEY;
        }

        protected TableType _tableType;
        protected TableType ThisTableType
        {
            get { return _tableType; }
        }
        
        protected string _rowTypeName;
        protected string RowTypeName
        {
            get { return _rowTypeName; }
            set { _rowTypeName = value; }
        }

        protected Type _rowTypeInfo;
        protected Type RowTypeInfo
        {
            get { return _rowTypeInfo; }
            set { _rowTypeInfo = value; }
        }

        protected KeyInfo   _keyInfo;
        protected KeyInfo KeyInformation
        {
            get { return _keyInfo; }
        }

        protected Dictionary<string, KeyInfo> _keyInfos = new Dictionary<string, KeyInfo>();
        protected KeyInfo this[string keyName]
        {
            get
            {
                return _keyInfos[keyName];
            }
            set
            {
                _keyInfos[keyName] = value;
            }
        }

        protected virtual bool IsValidKey(KeyInfo KeyInformation)
        {
            // Validate key info
            if (KeyInformation.keyType == KeyType.EKEY_UNIQUE ||
                KeyInformation.keyType == KeyType.EKEY_NONUNIQUE)
            {
                bool IsValidKey = false;
                foreach (System.Reflection.PropertyInfo prop in RowTypeInfo.GetProperties())
                {
                    if (prop.Name == KeyInformation._keyName1)
                    {
                        if (prop.PropertyType.Name != "UInt32" &&
                            prop.PropertyType.Name != "Int32" &&
                            prop.PropertyType.Name != "String" &&
                            prop.PropertyType.IsEnum == false)
                        {
                            Console.WriteLine("Invalid key value type!");
                            return false;
                        }

                        if (prop.PropertyType.IsEnum)
                            KeyInformation._keyTypeName1 = "UInt32";
                        else
                            KeyInformation._keyTypeName1 = prop.PropertyType.Name;

                        IsValidKey = true;
                    }
                }

                if (!IsValidKey)
                {
                    Console.WriteLine("Invalid key!");
                    return false;
                }
            }
            else if (KeyInformation.keyType == KeyType.EKEY_COMPOSIT)
            {
                if (KeyInformation._keyName1 == KeyInformation._keyName2)
                {
                    Console.WriteLine("Invalid key!");
                    return false;
                }

                bool IsValidKey1 = false, IsValidKey2 = false;
                foreach (System.Reflection.PropertyInfo prop in RowTypeInfo.GetProperties())
                {
                    if (prop.Name == KeyInformation._keyName1)
                    {
                        if (prop.PropertyType.Name != "UInt32" &&
                            prop.PropertyType.Name != "Int32" &&
                            prop.PropertyType.IsEnum == false)
                        {
                            Console.WriteLine("Invalid key value type!");
                            return false;
                        }

                        if (prop.PropertyType.IsEnum)
                            KeyInformation._keyTypeName1 = "UInt32";
                        else
                            KeyInformation._keyTypeName1 = prop.PropertyType.Name;
                        IsValidKey1 = true;
                    }

                    if (prop.Name == KeyInformation._keyName2)
                    {
                        if (prop.PropertyType.Name != "UInt32" &&
                            prop.PropertyType.Name != "Int32" &&
                            prop.PropertyType.IsEnum == false)
                        {
                            Console.WriteLine("Invalid key value type!");
                            return false;
                        }

                        if (prop.PropertyType.IsEnum)
                            KeyInformation._keyTypeName1 = "UInt32";
                        else
                            KeyInformation._keyTypeName1 = prop.PropertyType.Name;
                        KeyInformation._keyTypeName2 = prop.PropertyType.Name;
                        IsValidKey2 = true;
                    }
                }

                if (!(IsValidKey1 && IsValidKey2))
                {
                    Console.WriteLine("Invalid key!");
                    return false;
                }
            }
            else
            {
                Console.WriteLine("Invalid key type!");
                return false;
            }

            return true;
        }

        protected override bool Initialize(string tableNameSpace, bool isCpp)
        {
            if (!base.Initialize(tableNameSpace,isCpp))
                return false;

            Type typeInfo = GetTypeInfo(ClassName);

            // Build name map
            BuildNameMap(typeInfo, "", isCpp);

            //// Build element map
            BuildElementMap(typeInfo, "", "", isCpp, false, "");

            int     i = 0;
            foreach (System.Reflection.PropertyInfo prop in typeInfo.GetProperties())
            {
                if (prop.PropertyType.IsArray)
                {
                    string typeName = prop.PropertyType.Name.Substring(0, prop.PropertyType.Name.Length - 2);
                    RowTypeName = GetTypeName(typeName);
                    RowTypeInfo = GetTypeInfo(typeName);
                    ++i;
                }
            }
            if (RowTypeName.Length == 0 || i != 1 || RowTypeInfo == null )
            {
                Console.WriteLine("It is a non table type! or Row type don't exist!");
                return false;
            }

            if (ThisTableType == TableType.TTYPE_SINGLEKEY)
            {
                if (!IsValidKey(this["first"]))
                    return false;
            }
            else 
            {
                foreach (KeyValuePair<string, KeyInfo> kvp in _keyInfos)
                {
                    if (!IsValidKey(kvp.Value))
                        return false;
                }
            }
            

            return true;
        }
    }
}
