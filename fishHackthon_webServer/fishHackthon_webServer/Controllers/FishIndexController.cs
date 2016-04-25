using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Mvc;
using EqnestrianBooking.Models;

namespace EqnestrianBooking.Controllers
{
    public class FishIndexController : Controller
    {

        public class FishUpdateDataVM
        {
            public int FishID { get; set; }    
            public int FishWeight { get; set; }    
            public int FishLength { get; set; }      
            public string FishImageData { get; set; }

        }

        public class AjaxState 
        {
            public string State { get; set; }
        }

        //加速用
        static int lastFishID;
        // GET: FishIndex
        public ActionResult Index()
        {
            return View();
        }

        private Dictionary<int, string> fishNameTable = new Dictionary<int, string>()
        {
            {1,"藍倒吊"},
            {2,"黃鰭鮪魚"},
            {3,"小丑魚"},
            {4,"暫時"}
        };

        public ActionResult GetFishData() 
        {
            SampleContext sc = SampleContext.GetInstance();
            var ApptListForDate = sc.FishUpdateDatas.ToList<FishUpdateData>();

            if (lastFishID == ApptListForDate.LastOrDefault().ID)
            {
                return Json(new AjaxState() { State = "notRefresh" });
            }
            lastFishID = ApptListForDate.LastOrDefault().ID;
 
            var eventList = (from e in ApptListForDate
                             orderby e.ID descending
                            select new
                            {
                                ID = e.ID,
                                FishID = fishNameTable[e.FishID],
                                FishWeight = e.FishWeight,
                                FishLength = e.FishLength,
                                FishImageData = e.FishImageDataString
                            }).Take(3);
            var rows = eventList.ToArray();

            return Json(rows, JsonRequestBehavior.AllowGet);

         //   return Json(sc.FishUpdateDatas.ToList<FishUpdateData>());
        }

        [HttpPost]
        public ActionResult UpdateFishData(FishUpdateDataVM fud)
        {
            if (this.ModelState.IsValid)
            {
                SampleContext sc = SampleContext.GetInstance();

                sc.FishUpdateDatas.Add(new FishUpdateData()
                {
                    FishImageDataString = fud.FishImageData.Replace(' ','+'),
                    FishID = fud.FishID,
                    FishLength = fud.FishLength,
                    FishWeight = fud.FishWeight,
                });
                sc.SaveChanges();
                return Json(new AjaxState() { State = "success!" });
            }
            var errorList = ModelState.ToDictionary(kvp => kvp.Key,
                                                    kvp => kvp.Value.Errors
                                                    .Select(e => e.ErrorMessage).ToArray())
                                                    .Where(m => m.Value.Count() > 0);

            return Json(errorList);
        }

        
    }
}