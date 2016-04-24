using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Mvc;
using System.Web.Security;

namespace EqnestrianBooking.Controllers
{
    [Authorize]
    public class LogInController : Controller
    {
        // GET: LogIn
        [AllowAnonymous]
        public ActionResult Index()
        {
            return View();
        }

        public ActionResult ABC() 
        {
            return View();
        }

       

        [HttpPost]
      //  [ValidateAntiForgeryToken]
        [AllowAnonymous]
        public ActionResult Login(string acc, string Password)
        {

            Session["usr"] = acc;
            Session.Timeout = 30;

            var ticket = new FormsAuthenticationTicket(
                 version: 1,
                 name: "111",
                 issueDate: DateTime.Now,
                 expiration: DateTime.Now.AddMinutes(30),
                 isPersistent: true,
                 userData: "123",
                 cookiePath: FormsAuthentication.FormsCookiePath);

            string userData = ticket.UserData;

            var encryptedTicket = FormsAuthentication.Encrypt(ticket);
            var cookie = new HttpCookie(FormsAuthentication.FormsCookieName, encryptedTicket);
            Response.Cookies.Add(cookie);

            if ("admin" == acc) 
            {
                
                return RedirectToAction("Index", "Home");
            }

            return Json(new { state = "error!" });
            

        } 
    }
}