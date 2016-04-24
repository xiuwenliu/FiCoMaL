using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;
using System.Linq;
using System.Web;

namespace EqnestrianBooking.Models
{
    public class Reservation
    {
        [DisplayName("ID")]
        [Key]
        [Required]
        public int ID { get; set; }


        //此欄位可有可無
        [DisplayName("訂單名稱")]
        [Required]
        [StringLength(50)]
        [MinLength(1, ErrorMessage = "長度不可小於 1")]
        [MaxLength(100, ErrorMessage = "長度不可超過 100")]
        public string ReservationTitle { get; set; }

        [DisplayName("訂單申請人")]
        [Required]
        [StringLength(200)]
        [MinLength(1, ErrorMessage = "長度不可小於 5")]
        [MaxLength(200, ErrorMessage = "長度不可超過 200")]
        public string Applicant { get; set; }

        [DisplayName("訂單申請人的ID")]
        public int ApplicantID { get; set; }

        [DisplayName("教練名")]
        [Required]
        [StringLength(50)]
        [MinLength(1, ErrorMessage = "長度不可小於 1")]
        [MaxLength(50, ErrorMessage = "長度不可超過 50")]
        public string Coach { get; set; }

        [DisplayName("教練ID")]
        [Required(ErrorMessage = "請輸入選擇教練")] 
        public int CoachID { get; set; }

        [DisplayName("馬匹名")]
      
        [StringLength(200)]
        [MinLength(1, ErrorMessage = "長度不可小於 1")]
        [MaxLength(50, ErrorMessage = "長度不可超過 50")]
        public string Horse { get; set; }

        [DisplayName("馬匹名ID")]
        [Required(ErrorMessage="請輸入選擇馬匹")]      
        public int HorseID { get; set; }

        //0是現金, 1是票卷
        [DisplayName("付款方式")]
        [Required(ErrorMessage="請輸入付款方式")]
        public int HowPaid { get; set; }

        //true已付款
        [DisplayName("是否付款")]
        [Required]
        public bool IsPaid { get; set; }

        //此訂單是否完成?
        [DisplayName("是否完成")]
        [Required]
        public bool IsClean { get; set; }

        [DisplayName("建立時間訂單時間")]
        [Required]
        [DatabaseGenerated(DatabaseGeneratedOption.Computed)]
        public DateTime CreateOn { get; set; }

        //訂單開始時間
        [DisplayName("開始時間")]
        [Required(ErrorMessage = "請輸入預約開始時間")]
        public DateTime StartDate { get; set; }

        //訂單結束時間
        [DisplayName("結束時間")]
        [Required(ErrorMessage = "請輸入預約結束時間")]
        public DateTime EndDate { get; set; }

        //附註
        [DisplayName("備註")]
        public string Remark { get; set; }

        //附註
        [DisplayName("取消原因")]
        public string CancelReason { get; set; }

        public Reservation()
        {        
            this.IsPaid = false;
            this.IsClean = false;
            this.CreateOn = DateTime.Now;
            this.CancelReason = "No Cancel";
        }
    }
}