using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Mvc;

using BR;

namespace BRMonitoringWeb.Controllers
{
    public class MonitoringController : Controller
    {
        public ActionResult Index()
        {
            ViewBag.Message = "BR Monitoring MainPage";

            return View();
        }

        class GetCounterInstanceListResult
        {
            public struct InstanceInfo
            {
                public string Name;
                public UInt64 UID;
                public uint NumCounters;
            }
            public List<InstanceInfo> instanceList = new List<InstanceInfo>();
        };

        public JsonResult GetCounterInstanceList()
        {
            var result = new GetCounterInstanceListResult();

            using(var instanceArray = BRMonitoring.CreatePerformanceCounterInstanceArray())
            {
                var numCounterList = PerformanceCounterServer.GetInstanceList(0, instanceArray);
                for (uint iInstance = 0; iInstance < numCounterList; iInstance++)
                {
                    var instance = instanceArray.GetAt(iInstance);
                    var pInstance = instance.GetObjectPtr();
                    GetCounterInstanceListResult.InstanceInfo info = new GetCounterInstanceListResult.InstanceInfo()
                    {
                        Name = pInstance.GetInstanceName(),
                        UID = pInstance.GetInstanceEntityUID().UID,
                        NumCounters = pInstance.GetCounters().GetSize()
                    };

                    result.instanceList.Add(info);
                }
            }

            return Json(result, JsonRequestBehavior.AllowGet);
        }

        class GetCounterValuesResult
        {
            public struct Counter
            {
                public string Name;
                public Int64 Value;
            }

            public List<Counter> CounterList = new List<Counter>();
        };

        public JsonResult GetCounterValues(UInt64 instanceUID)
        {
            var result = new GetCounterValuesResult();

            using(var instance = BRMonitoring.CreatePerformanceCounterInstance())
            {
                PerformanceCounterServer.GetInstance(instanceUID, instance);
                var pInstance = instance.GetObjectPtr();
                if (pInstance != null)
                {
                    var counterList = pInstance.GetCounters();
                    var numCounter = counterList.GetSize();
                    for (uint iCounter = 0; iCounter < numCounter; iCounter++)
                    {
                        var counter = counterList.GetAt(iCounter);
                        result.CounterList.Add(new GetCounterValuesResult.Counter()
                        {
                            Name = counter.GetCounterName(),
                            Value = counter.GetRawValue()
                        });
                    }
                }
            }



            return Json(result, JsonRequestBehavior.AllowGet);
        }


    }
}