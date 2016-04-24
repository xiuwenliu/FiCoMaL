using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.ComponentModel.DataAnnotations;
using System.Linq;
using System.Web;

namespace EqnestrianBooking.Models
{
    public class Coach
    {
        [DisplayName("ID")]
        [Key]
        [Required]
        public int ID { get; set; }

        [DisplayName("教練名稱")]
        [Required]
        [StringLength(50)]
        [MinLength(1, ErrorMessage = "長度不可小於 1")]
        [MaxLength(100, ErrorMessage = "長度不可超過 100")]
        public string CoachName { get; set; }

        [DisplayName("電話")]
        [Required]
        [StringLength(200)]
        [MinLength(1, ErrorMessage = "長度不可小於 1")]
        [MaxLength(200, ErrorMessage = "長度不可超過 200")]
        public string CellPhone { get; set; }

     
        [DisplayName("值班時間")]
        [Required]
        [StringLength(200)]
        [MinLength(1, ErrorMessage = "長度不可小於 1")]
        [MaxLength(200, ErrorMessage = "長度不可超過 200")]
        public string OnDutyDay { get; set; }

        [DisplayName("顏色")]
        [Required]
        [StringLength(200)]
        [MinLength(1, ErrorMessage = "長度不可小於 1")]
        [MaxLength(200, ErrorMessage = "長度不可超過 200")]
        public string DisplayColor { get; set; }

        
        public Coach()
        {
        }
    }
}