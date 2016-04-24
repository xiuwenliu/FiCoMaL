using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.ComponentModel.DataAnnotations;
using System.Linq;
using System.Web;

namespace EqnestrianBooking.Models
{
    public class Customer
    {
        [DisplayName("ID")]
        [Key]
        [Required]
        public int ID { get; set; }

        [DisplayName("消費者名稱")]
        [Required]
        [StringLength(50)]
        [MinLength(1, ErrorMessage = "長度不可小於 1")]
        [MaxLength(100, ErrorMessage = "長度不可超過 100")]
        public string CustomerName { get; set; }

        [DisplayName("電話1")]
        [Required]
        [StringLength(200)]
        [MinLength(1, ErrorMessage = "長度不可小於 1")]
        [MaxLength(200, ErrorMessage = "長度不可超過 200")]
        [Phone]
        public string CellPhone1 { get; set; }

        [DisplayName("電話2")]
        [StringLength(200)]
        [MinLength(1, ErrorMessage = "長度不可小於 1")]
        [MaxLength(200, ErrorMessage = "長度不可超過 200")]
        [Phone]
        public string CellPhone2 { get; set; }


        [DisplayName("電子郵件")]
        [Required(ErrorMessage = "The email address is required")]
        [StringLength(200)]
        [MinLength(1, ErrorMessage = "長度不可小於 1")]
        [MaxLength(200, ErrorMessage = "長度不可超過 200")]
        [EmailAddress(ErrorMessage = "Invalid Email Address")]
        public string EmailAddress { get; set; }

        [DisplayName("住家地址")]
        [Required(ErrorMessage = "The address is required")]
        [StringLength(200)]
        [MinLength(1, ErrorMessage = "長度不可小於 1")]
        [MaxLength(200, ErrorMessage = "長度不可超過 200")]
        public string Address { get; set; }

        [DisplayName("介紹人")]
        [StringLength(200)]
        [MinLength(1, ErrorMessage = "長度不可小於 1")]
        [MaxLength(200, ErrorMessage = "長度不可超過 200")]
        public string IntroducedBy { get; set; }

        [DisplayName("剩餘票卷")]   
        public int RemainTickets { get; set; }

        [DisplayName("最後消費日")]      
        public DateTime LastPayDate { get; set; }

        [DisplayName("出生日期")]
        [Required]
        public DateTime Birthday { get; set; }


        public Customer()
        {
        }
    }
}