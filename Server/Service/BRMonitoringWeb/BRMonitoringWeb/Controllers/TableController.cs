using BR.DB;
using MySql.Data.MySqlClient;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.IO;
using System.Text;
using System.Web.Mvc;
using System.Xml.Serialization;
using System.Reflection;

namespace BRMonitoringWeb.Controllers
{
    public class TableController : Controller
    {
        public class GameTableInformation
        {
            public string TableName;
            public string PrimaryKey;
            public string FileName { get { return TableName + ".xml"; } }
            public Type TableClass;

            public GameTableInformation(string tableName, string primaryKey, Type tableClass)
            {
                TableName = tableName;
                PrimaryKey = primaryKey;
                TableClass = tableClass;
            }
        }

        // table informations
        static GameTableInformation[] m_TableInfos = new GameTableInformation[]
        {
            new GameTableInformation("GameConfigTable", null, typeof(conspiracy.GameConfigTbl) ),
            new GameTableInformation("LevelTable", null, typeof(conspiracy.LevelTbl) ),
            new GameTableInformation("OrganicTable", null, typeof(conspiracy.OrganicTbl) ),
            new GameTableInformation("RewardTable", null, typeof(conspiracy.RewardTbl) ),
            new GameTableInformation("ShopTable", null, typeof(conspiracy.ShopTbl) ),
            new GameTableInformation("BotTalkTable", null, typeof(conspiracy.BotTalkTbl) ),
            //new GameTableInformation("StringTblBot", null, typeof(conspiracy.StringTblBot) ),
        };
        Dictionary<string, GameTableInformation> m_TableMap = new Dictionary<string, GameTableInformation>();



        public TableController()
        {
            foreach (var tableInfo in m_TableInfos)
            {
                m_TableMap.Add(tableInfo.TableName.ToLower(), tableInfo);
            }
        }

        // GET: Table
        public ActionResult Index()
        {
            List<string> tableNames = new List<string>();
            foreach( var table in m_TableInfos)
            {
                tableNames.Add(table.TableName);
            }

            ViewBag.FileNames = string.Join("/", tableNames);
            return View();
        }

        object LoadXMLTable(GameTableInformation tableInfo, Stream fileStream)
        {
            var serializer = new XmlSerializer(tableInfo.TableClass);
            return serializer.Deserialize(fileStream);
        }


        [HttpPost]
        public JsonResult UploadTableFile(string tableName, HttpPostedFileBase file)
        {
            string message = "";
            bool succeeded = false;

            try
            {
                GameTableInformation tableInfo;
                using (var myConn = DBConnectionManager.GetConnection("TableDB"))
                using (var dbTable = new DBConnectionConspiracyTable(myConn))
                {
                    if (!m_TableMap.TryGetValue(tableName.ToLower(), out tableInfo))
                    {
                        throw new InvalidOperationException("Invalid server table");
                    }

                    // find child property
                    PropertyInfo arrayProperty = null;
                    var tableProperties = tableInfo.TableClass.GetProperties();
                    foreach(var tableProperty in tableProperties)
                    {
                        if(tableProperty.PropertyType.IsArray)
                        {
                            arrayProperty = tableProperty;
                        }
                    }

                    if (arrayProperty == null) throw new InvalidCastException("Can't find Array element");
                    
                    dbTable.CreateTable(tableInfo.TableName, tableInfo.PrimaryKey, arrayProperty.PropertyType.GetElementType());

                    var loadedData = LoadXMLTable(tableInfo, file.InputStream);
                    var arrayElementObject = (Array)arrayProperty.GetValue(loadedData);

                    succeeded = dbTable.UploadTableData(tableInfo.TableName, arrayElementObject);
                }
            }
            catch (Exception exp)
            {
                message = exp.Message + exp.StackTrace;
                succeeded = false;
            }

            return Json(new
            {
                TableName = tableName,
                Succeeded = succeeded,
                Message = message,
            }, JsonRequestBehavior.AllowGet);
        }

    }
}