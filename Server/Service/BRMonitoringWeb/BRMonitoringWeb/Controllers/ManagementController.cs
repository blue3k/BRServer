using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Mvc;
using MySql.Data.MySqlClient;
using BR;
using BR.DB;

namespace BRMonitoringWeb.Controllers
{
    public class ManagementController : Controller
    {
        const string AccountDB = "managementDB";
        const string ManagementDB = "managementDB";

        public ActionResult Index()
        {
            ViewBag.Message = "BR Management MainPage";

            return View();
        }

        public JsonResult GetPlayerInfoByID(UInt64 playerID)
        {
            using(var monDB = DBConnectionManager.GetConnection(AccountDB, 1))
            {
                //return Json(monDB.QueryFindPlayer(playerID), JsonRequestBehavior.AllowGet);
                return null;
            }

            //return Json((object)"", JsonRequestBehavior.AllowGet);
        }

        class GetPlayerInfoByNameResult
        {
            public List<PlayerInfo> PlayerList = new List<PlayerInfo>();
        };

        public JsonResult GetPlayerInfoByName(string pattern)
        {
            var result = new GetPlayerInfoByNameResult();

            //using (var monDB = DBPool.MonitoringDB.GetConnection())
            //{
            //    var list = monDB.QueryFindPlayers(pattern);
            //    return Json(list, JsonRequestBehavior.AllowGet);
            //}
            return null;
        }

        public JsonResult GetPlayerGameLogs(UInt64 playerID, DateTime from, DateTime to)
        {
            var result = new GetPlayerInfoByNameResult();

            //using (var monDB = DBPool.MonitoringDB.GetConnection())
            //{
            //    var list = monDB.QueryGameLogs(playerID, from, to);
            //    return Json(list, JsonRequestBehavior.AllowGet);
            //}
            return null;
        }


    }
}