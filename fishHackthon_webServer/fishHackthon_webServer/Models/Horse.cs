using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.ComponentModel.DataAnnotations;
using System.Linq;
using System.Web;

namespace EqnestrianBooking.Models
{
    public class Horse
    {
        [DisplayName("ID")]
        [Key]
        [Required]
        public int ID { get; set; }

        [DisplayName("馬匹名稱")]
        [Required]
        [StringLength(50)]
        [MinLength(1, ErrorMessage = "長度不可小於 1")]
        [MaxLength(100, ErrorMessage = "長度不可超過 100")]
        public string HorseName { get; set; }

        [DisplayName("出生日期")]
        [Required]
        public DateTime Birthday { get; set; }

        [DisplayName("品種")]
        [Required]
        public int Breed { get; set; }

        [DisplayName("身高")]
        [Required]
        [StringLength(200)]
        public string Height { get; set; }

        [DisplayName("性別")]
        [Required]
        [StringLength(200)]
        public string Sex { get; set; }

        [DisplayName("性格")]
        [Required]
        [StringLength(200)]
        public string Temper { get; set; }


        [DisplayName("顯示的顏色")]
        [StringLength(200)]
        [MinLength(1, ErrorMessage = "長度不可小於 1")]
        [MaxLength(200, ErrorMessage = "長度不可超過 200")]
        public string DisplayColor { get; set; }


        public Horse()
        {
        }
    }
}