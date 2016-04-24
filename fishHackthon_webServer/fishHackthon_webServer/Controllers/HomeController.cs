using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Mvc;
using EqnestrianBooking.Models;

namespace EqnestrianBooking.Controllers
{

    public class Good
    {
        public string SerNo { get; set; }
        public string StyleCode { get; set; }
        public string Code { get; set; }
        public string Name { get; set; }
        public string Status { get; set; }
        public string BrandCode { get; set; }
        public string BrandName { get; set; }
        public string ColorCode { get; set; }
        public string ColorName { get; set; }
        public string SizeCode { get; set; }
        public string SizeName { get; set; }
    }

    public class TestData {
        public int FinishFetch { get; set; }
        public List<Good> Goods { get; set; }
    }

  

    public class ChangeCoachData 
    {
        public string Name { get; set; }
        public int ID { get; set; }
    }

    public class AjaxState 
    {
        public string State { get; set; }
    }

    public class DeleteReservationId 
    {
        public string DeleteId { get; set; }
    }

    public class UpdateEventData
    {
        public int ReservationId { get; set; }
        public DateTime StartDate { get; set; }

        public DateTime EndDate { get; set; }
    }


    public class HomeController : Controller
    {
        [HttpPost]
        public ActionResult TestAPI()
        {
          /*  SampleContext sc = SampleContext.GetInstance();
            TestData t = new TestData() { FinishFetch = 1, Goods = new List<Good>() { new Good(){BrandCode="1",Code="2",BrandName="312",ColorCode="31",ColorName="3123",Name="3123",SerNo="3123"}} };
         //   Reservation r = sc.Reservations.Find(id);*/
         //   r.IsClean = true;
         //   sc.SaveChanges();
            return Json(null);
        }

       
        // GET: Home 
        //[Authorize]
        public ActionResult Index()
        {
           // SampleContext.LoadReservations();
            SampleContext sc = SampleContext.GetInstance();
            List<Reservation> r = sc.Reservations.ToList<Reservation>();
            List<Coach> c = sc.Coachs.ToList<Coach>();
            List<Horse> h = sc.Horses.ToList<Horse>();
            ViewBag.Coachs = c;
            ViewBag.Horses = h;
            ViewBag.Reservations = r;
            ChangeCoachID = -999;

         /*   var token = Guid.NewGuid().ToString();
            var hc = new HttpCookie("trip", "no");
            Response.Cookies.Add(hc);*/
            
            return View();
        }

        public ActionResult Admin()
        {
            // SampleContext.LoadReservations();
            SampleContext sc = SampleContext.GetInstance();
            List<Coach> c = sc.Coachs.ToList<Coach>();
            List<Horse> h = sc.Horses.ToList<Horse>();
            ViewBag.Coachs = c;
            ViewBag.Horses = h;
            return View(sc.Reservations);
        }

        public ActionResult CustomerManager() 
        {
            SampleContext sc = SampleContext.GetInstance();            
            return View(sc.Customers);
        }

        public ActionResult CoachManager() 
        {
            SampleContext sc = SampleContext.GetInstance();   
            return View(sc.Coachs);
        }

        public ActionResult HourseManager() 
        {
            SampleContext sc = SampleContext.GetInstance();   
            return View(sc.Horses);
        }

        //完成這筆訂單
        [HttpPost]
        public ActionResult FinishedReservation(int id)
        {         
            SampleContext sc = SampleContext.GetInstance();
            Reservation r = sc.Reservations.Find(id);
            r.IsClean = true;
            sc.SaveChanges();
            return Json(null);
        }

        //取得訂單的詳細資訊
        [HttpPost]
        public ActionResult GetReservationDetial(int id) 
        {
            SampleContext sc = SampleContext.GetInstance();
            Reservation r = sc.Reservations.Find(id);
            var reservation = from e in sc.Reservations.ToList<Reservation>()
                              where e.ID == id
                              select new
                              {
                                  id = e.ID,
                                  Applicant = e.Applicant,
                                  ApplicantID = e.ApplicantID,
                                  StartDate = e.StartDate.ToString("yyyy'年'MM'月'dd'日'HH':'mm':'ss"),
                                  EndDate = e.EndDate.ToString("yyyy'年'MM'月'dd'日'HH':'mm':'ss"),
                                  HowPaid = e.HowPaid == 0 ? "現金" : "票卷",                                 
                                  Coach = e.Coach,
                                  CoachID= e.CoachID,
                                  Horse = e.Horse,
                                  HorseID = e.HorseID,
                                  IsClean = e.IsClean,
                                  Remark = e.Remark,
                                  CancelReason = e.CancelReason
                              };
            reservation.ToList();
            return Json(reservation.FirstOrDefault());
        }

        //取得消費者的詳細資訊
        [HttpPost]
        public ActionResult GetCustomerDetial(int id)
        {
            SampleContext sc = SampleContext.GetInstance();
            Customer r = sc.Customers.Find(id);
            var customer = from e in sc.Customers.ToList<Customer>()
                              where e.ID == id
                              select new
                              {
                                  id = e.ID,
                                  Address = e.Address,
                                  Birthday = e.Birthday.ToString("yyyy'年'MM'月'dd'日'HH':'mm':'ss"),
                                  CellPhone1 = e.CellPhone1,

                                  CellPhone2 = e.CellPhone2,
                                  CustomerName = e.CustomerName,
                                  EmailAddress = e.EmailAddress,
                                  IntroducedBy = e.IntroducedBy,
                                  LastPayDate = e.LastPayDate,
                                  RemainTickets = e.RemainTickets                               
                              };
            customer.ToList();
            return Json(customer.FirstOrDefault());
        }

        [HttpPost]
        public ActionResult GetCustomerHistory(int id) 
        {
            SampleContext sc = SampleContext.GetInstance();
            Customer r = sc.Customers.Find(id);
            var history = from e in sc.Reservations.ToList<Reservation>()
                          where e.ApplicantID == id
                          select new
                          {
                              id = e.ID,
                              Coach = e.Coach,
                              Horse = e.Horse,
                              HowPaid = e.HowPaid == 0 ? "現金" : "票卷",
                              StartDate = e.StartDate.ToString("yyyy'年'MM'月'dd'日'HH':'mm':'ss")/*,
                              EndDate = e.EndDate.ToString("yyyy'年'MM'月'dd'日'HH':'mm':'ss")*/
                          };
            history.ToList();
            return Json(history);
        }



        [HttpPost]
        public ActionResult UpdateEvent(UpdateEventData ue) 
        {
            SampleContext sc = SampleContext.GetInstance();
            Reservation r = sc.Reservations.Find(ue.ReservationId);
            r.StartDate = ue.StartDate;

            //利用nullDateString修正錯誤
            string nullDateString = "1/1/0001 12:00:00 AM";
            DateTime nullDate = DateTime.Parse(nullDateString,
                          System.Globalization.CultureInfo.InvariantCulture);
            if (ue.EndDate.CompareTo(nullDate)!=0)
                r.EndDate = ue.EndDate;

            sc.SaveChanges();
            return Json(ue);
        }

        [HttpPost]
        //[ValidateAntiForgeryToken]
        public ActionResult ResrvationDelete(DeleteReservationId id)
        {
            SampleContext sc = SampleContext.GetInstance();
            sc.Reservations.Remove(sc.Reservations.Find(int.Parse(id.DeleteId)));
            sc.SaveChanges();
            //到部份檢視
            return PartialView("_ReservationList", sc.Reservations);
        }


        [HttpPost]   
        public ActionResult CustomerDelete(int id)
        {
            SampleContext sc = SampleContext.GetInstance();
            sc.Customers.Remove(sc.Customers.Find(id));
            sc.SaveChanges();
            //到部份檢視
            return PartialView("_CustomerList", sc.Customers);
        }

        //創造新訂單的hook
        [HttpPost]
        public ActionResult CreateNewReservation(Reservation r)
        {
          
           // if(r.StartDate.)
            if (this.ModelState.IsValid)
            {
                SampleContext sc = SampleContext.GetInstance();

                Customer c = sc.Customers.Find(r.ApplicantID);

                if (r.HowPaid == 1)
                {
                    if (c.RemainTickets <= 0)
                    {
                        return Json(new AjaxState() { State = "no_ticket!" });
                    }
                    c.RemainTickets--;
                }

                c.LastPayDate = DateTime.Now;

                sc.Reservations.Add(new Reservation()
                {
                    ReservationTitle = "測試訂單",
                    Applicant = r.Applicant,
                    ApplicantID = r.ApplicantID,
                    Coach = r.Coach,
                    CoachID = r.CoachID,
                    Horse = r.Horse,
                    HorseID = r.HorseID,
                    StartDate = r.StartDate,
                    EndDate = r.EndDate,
                    HowPaid = r.HowPaid
                });



                sc.SaveChanges();
                return Json(new AjaxState() { State = "success!" });
            }

            var errorList = ModelState.ToDictionary(kvp => kvp.Key,
                                                    kvp => kvp.Value.Errors
                                                    .Select(e => e.ErrorMessage).ToArray())
                                                    .Where(m => m.Value.Count() > 0);

            return Json(errorList);

            return Json(new AjaxState() { State = "有東西沒填喔!!!" });
        }

        //修改訂單的hook
        [HttpPost]
        public ActionResult ModifyReservation(Reservation r)
        {
            if (this.ModelState.IsValid)
            {
                SampleContext sc = SampleContext.GetInstance();
                int id = r.ID;
                Reservation reservation = sc.Reservations.Where(i => i.ID == id).FirstOrDefault();
                reservation.ReservationTitle = r.ReservationTitle;
                reservation.Applicant = r.Applicant;
                reservation.ApplicantID = r.ApplicantID;
                reservation.Coach = r.Coach;
                reservation.CoachID = r.CoachID;
                reservation.Horse = r.Horse;
                reservation.HorseID = r.HorseID;
                reservation.IsPaid = r.IsPaid;
                reservation.IsClean = r.IsClean;
                reservation.StartDate = r.StartDate;
                reservation.EndDate = r.EndDate;
                sc.SaveChanges();
                return Json(new AjaxState() { State = "success!" });
            }
            return Json(new AjaxState() { State = "有東西沒填喔!!!" });
        }

        //修改顧客資料
        [HttpPost]
        public ActionResult ModifyCustomer(Customer r)
        {
            if (this.ModelState.IsValid)
            {
                SampleContext sc = SampleContext.GetInstance();
                int id = r.ID;
                Customer customer = sc.Customers.Where(i => i.ID == id).FirstOrDefault();
                customer.Address = r.Address;
                customer.Birthday = r.Birthday;
                customer.CellPhone1 = r.CellPhone1;
                customer.CellPhone2 = r.CellPhone2;
                customer.CustomerName = r.CustomerName;
                customer.EmailAddress = r.EmailAddress;
                customer.IntroducedBy = r.IntroducedBy;
           
                customer.RemainTickets = r.RemainTickets;              
                sc.SaveChanges();
                return Json(new AjaxState() { State = "success!" });
            }
            return Json(new AjaxState() { State = "有東西沒填喔!!!" });
        }


        public static string ChangeCoachName = "ALL";
        public static int ChangeCoachID = -999;
        [HttpPost]
        public ActionResult ChangeCoach(ChangeCoachData c)
        {
            ChangeCoachName = c.Name;
            ChangeCoachID = c.ID;
            return Json(c);
        }

        //取得訂單資訊放到calendar
        public JsonResult GetDiaryEvents(DateTime start, DateTime end)
        {
            SampleContext sc = SampleContext.GetInstance();
       
            var ApptListForDate = sc.Reservations.ToList<Reservation>();
            var eventList = from e in ApptListForDate
                            where (ChangeCoachID == -999) ? true : e.CoachID == sc.Coachs.Where(i => i.ID == ChangeCoachID).First<Coach>().ID
                            select new
                            {
                                id = e.ID,
                                title = e.Applicant + "的訂單",
                                //
                                //這個format極度重要
                                //
                                start = e.StartDate.ToString("yyyy'-'MM'-'dd'T'HH':'mm':'ss'.'fff'Z'"),
                                end = e.EndDate.ToString("yyyy'-'MM'-'dd'T'HH':'mm':'ss'.'fff'Z'"),
                                // howPaid = e.HowPaid == 0 ? "現金" : "票卷",
                                color = sc.Coachs.Where(i => i.ID == e.CoachID).First<Coach>().DisplayColor,
                                //ClassName = "Content/ajax-loader.gif",
                                //someKey = e.SomeImportantKeyID,
                                allDay = false,
                                className = (e.IsClean) ? "完成" : (e.CancelReason == "No Cancel") ? "進行中" : "取消"
                            };
            var rows = eventList.ToArray();
            return Json(rows, JsonRequestBehavior.AllowGet);
        }

       
        //取消訂單
        [HttpPost]
        public ActionResult CancelReservation(int id, string reason)
        {
            AjaxState state = new AjaxState();
            SampleContext sc = SampleContext.GetInstance();
            Reservation r = sc.Reservations.Find(id);
            if (r != null)
            {            
                r.CancelReason = reason;
                sc.SaveChanges();                     
                return Json(r);
            }
            else
                state.State = "No reservation in database!";
            return Json(state);
        }

        //確認這位消費者有沒有在資料庫裏面
        [HttpPost]
        public ActionResult CheckCustomer(string checkCustomerPhone, string checkCustomerName)
        {
            AjaxState state = new AjaxState();
            SampleContext sc = SampleContext.GetInstance();
            Customer c = sc.Customers.Where(x => x.CustomerName == checkCustomerName || x.CellPhone1 == checkCustomerPhone).FirstOrDefault();
            if (c != null)
            {
                state.State = c.ID.ToString();
                //  c.LastPayDate = c.LastPayDate.ToString("yyyy'-'MM'-'dd'T'HH':'mm':'ss'.'fff'Z'");
                return Json(c);
            }
            else
                state.State = "No customer in database!";
            return Json(state);
        }

        //新增消費者
        [HttpPost]
        public ActionResult CreateCustomer(Customer c)
        {
          
            if (this.ModelState.IsValid)
            {
                SampleContext sc = SampleContext.GetInstance();
                //尚未驗證
                sc.Customers.Add(new Customer()
                {
                    CustomerName = c.CustomerName,
                    CellPhone1 = c.CellPhone1,
                    CellPhone2 = c.CellPhone2,
                    EmailAddress = c.EmailAddress,
                    Address = c.Address,
                    IntroducedBy = c.IntroducedBy,
                    RemainTickets = c.RemainTickets,
                    Birthday = c.Birthday,
                    LastPayDate = DateTime.Now
                });
                sc.SaveChanges();
                return Json(new AjaxState() { State = "success!" });
            }

            var errorList = ModelState.ToDictionary(kvp => kvp.Key,
                                                    kvp => kvp.Value.Errors
                                                    .Select(e => e.ErrorMessage).ToArray())
                                                    .Where(m => m.Value.Count() > 0);
            return Json(errorList);

            return Json(new AjaxState() { State = "有東西沒填喔!!!" });    
        }

        //客製化filter 目前只有admin可以進入
        protected override void OnActionExecuting(ActionExecutingContext filterContext)
        {   
         /*   var usr = Session["usr"];
            if ((string)usr != "admin")
            {
                filterContext.HttpContext.Response.Redirect("~/LogIn/Index");
            }

            base.OnActionExecuting(filterContext);*/
        }
    }
}