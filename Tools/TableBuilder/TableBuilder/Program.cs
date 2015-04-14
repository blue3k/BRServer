///////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2014 The Braves
// 
// Author : KyungKun Ko
//
// Description : Table builder main
//
////////////////////////////////////////////////////////////////////////////////

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Data;

using System.Reflection;


namespace TableBuilder
{
    internal enum KeyType
    {
        EKEY_NONE,
        EKEY_UNIQUE,
        EKEY_NONUNIQUE,
        EKEY_COMPOSIT,
    }

    internal class KeyInfo
    {
        public KeyInfo(KeyType keyType, string keyName)
        {
            _keyType = keyType;
            _keyName1 = keyName;
            _keyName2 = "";
        }

        public KeyInfo(KeyType keyType, string keyName1, string keyName2)
        {
            _keyType = keyType;
            _keyName1 = keyName1;
            _keyName2 = keyName2;
        }

        public KeyType keyType
        {
            get { return _keyType; }
        }

        public KeyType _keyType;
        public string _keyName1;
        public string _keyTypeName1;
        public string _keyName2;
        public string _keyTypeName2;
    }


    internal enum TableType
    {
        TTYPE_SINGLEKEY,
        TTYPE_MULTIKEY,
    }

    internal class TableInfo
    {
        public TableType _tableType;
        public string _name;
        public KeyInfo _keyInfo;

        public Dictionary<string, KeyInfo> _keyInfos = new Dictionary<string, KeyInfo>();

        public bool _isTable;
        public string _baseParserName = "XML::XMLParserHandler";

        public TableInfo(string inName)
        {
            _isTable = false;
            _name = inName;
        }

        public TableInfo(string inName, KeyInfo keyInfo)
        {
            _isTable = true;
            _name = inName;

            _tableType = TableType.TTYPE_SINGLEKEY;
            _keyInfo = keyInfo;
            _keyInfos["first"] = keyInfo;
        }


        public TableInfo(string inName, KeyInfo keyInfo1, KeyInfo keyInfo2)
        {
            _isTable = true;
            _name = inName;

            _tableType = TableType.TTYPE_MULTIKEY;
            _keyInfos["first"] = keyInfo1;
            _keyInfos["second"] = keyInfo2;
        }
    };

    class Program
    {
        // Base path
        static string m_BasePath;

        // target namespace 
        static string m_targetNameSpace = "GameTbl";

        // Build table list
        static TableInfo[] TableInfoList = { //New Table 
                                             new TableInfo("RewardTbl"              , new KeyInfo(KeyType.EKEY_UNIQUE, "Role") ),
                                             new TableInfo("LevelTbl"               , new KeyInfo(KeyType.EKEY_UNIQUE, "Level") ),
                                             new TableInfo("ShopTbl"                , new KeyInfo(KeyType.EKEY_UNIQUE, "ShopItemID") ),
                                             new TableInfo("GameConfigTbl"          , new KeyInfo(KeyType.EKEY_UNIQUE, "PresetID") ),
                                           };

        /// <summary>
        /// Parse arguments
        /// </summary>
        /// <param name="args">argument </param>
        static bool ParseArgument(string[] args)
        {
            if (args.Length == 0)
            {
                Console.WriteLine("Invalid usage! : TableBuilder.exe -dir:<folder name>");
                return false;
            }
            // Parse argument
            for (int iArg = 0; iArg < args.Length; iArg++)
            {
                string strArg = args[iArg];

                if (strArg.StartsWith("-dir:", true, null))
                {
                    strArg.ToLower();
                    m_BasePath = strArg.Remove(0, ("-dir:").Length);
                }
                else if (strArg.StartsWith("-namespace:", true, null))
                {
                    m_targetNameSpace = strArg.Remove(0, ("-namespace:").Length);
                }
                else
                {
                    Console.WriteLine("Invalid usage! : TableBuilder.exe -dir:<folder name>");
                    return false;
                }
            }

            return true;
        }

        /// <summary>
        /// Main function
        /// </summary>
        /// <param name="args"></param>
        static void Main(string[] args)
        {

            // Parse argument
            if (!ParseArgument(args))
                return;


            // Generate
            try
            {
                Builder.UpdateClassList(m_targetNameSpace);

                foreach (TableInfo tableInfo in TableInfoList)
                {
                    if (tableInfo._isTable)
                    {
                        if (tableInfo._tableType == TableType.TTYPE_SINGLEKEY)
                        {
                            if (tableInfo._keyInfo._keyType == KeyType.EKEY_COMPOSIT &&
                            tableInfo._keyInfo._keyName2.Length == 0)
                            {
                                Console.WriteLine("{0} is Invalid tableInfo!", tableInfo._name);
                                continue;
                            }

                            CPPTableBuilder builder = new CPPTableBuilder(m_BasePath, tableInfo._name, tableInfo._keyInfo,
                                                    tableInfo._baseParserName);
                            builder.Build(m_targetNameSpace);

                            //UnrealTableBuilder ubuilder = new UnrealTableBuilder(m_BasePath, tableInfo._name, tableInfo._keyInfo,
                            //                        tableInfo._baseParserName);
                            //ubuilder.Build();
                        }
                        else
                        {
                            if ((tableInfo._keyInfos["first"]._keyType == KeyType.EKEY_COMPOSIT &&
                            tableInfo._keyInfos["first"]._keyName2.Length == 0) ||
                                (tableInfo._keyInfos["second"]._keyType == KeyType.EKEY_COMPOSIT &&
                            tableInfo._keyInfos["second"]._keyName2.Length == 0))
                            {
                                Console.WriteLine("{0} is Invalid tableInfo!", tableInfo._name);
                                continue;
                            }

                            CPPTableBuilder builder = new CPPTableBuilder(m_BasePath, tableInfo._name,
                                tableInfo._keyInfos["first"], tableInfo._keyInfos["second"],
                                tableInfo._baseParserName);
                            builder.Build(m_targetNameSpace);

                            //UnrealTableBuilder ubuilder = new UnrealTableBuilder(m_BasePath, tableInfo._name,
                            //    tableInfo._keyInfos["first"], tableInfo._keyInfos["second"],
                            //                        tableInfo._baseParserName);
                            //ubuilder.Build();
                        }
                    }
                    else
                    {
                        CPPTreeBuilder builder = new CPPTreeBuilder(m_BasePath, tableInfo._name, tableInfo._baseParserName);
                        builder.Build(m_targetNameSpace);

                        //UnrealTreeBuilder uBuilder = new UnrealTreeBuilder(m_BasePath, tableInfo._name, tableInfo._baseParserName);
                        //uBuilder.Build(m_targetNameSpace);
                    }
                }
            }
            catch (System.IO.IOException sysex)
            {
                Console.WriteLine("System IO exception: " + sysex.Message);
            }
            catch (InvalidOperationException ioe)
            {
                Console.WriteLine("Invalid Operation: " + ioe.Message);
            }
            catch (SystemException ioe)
            {
                Console.WriteLine("System Exception: " + ioe.Message);
            }
            finally
            {
                Console.WriteLine("Work Done!");
            }
        }

    }
}
