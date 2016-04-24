using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;
using System.Linq;
using System.Web;

namespace EqnestrianBooking.Models
{
    public class FishUpdateData
    {
        [DisplayName("ID")]
        [Key]
        [Required]
        public int ID { get; set; }


        
        [DisplayName("魚的辨識種類")]   
        public int FishID { get; set; }

        [DisplayName("魚的重量")]
        public int FishWeight { get; set; }

        [DisplayName("魚的長度 單位cm")]
        public int FishLength { get; set; }

        [DisplayName("系統拍攝到的截圖")]
        public byte[] FishImageData { get; set; }

        public string FishImageDataString { get; set; }

        [DisplayName("上傳時間")]
        public DateTime UpdateDate { get; set; }



        public FishUpdateData()
        {        
            /*this.IsPaid = false;
            this.IsClean = false;
            this.CreateOn = DateTime.Now;
            this.CancelReason = "No Cancel";*/
            UpdateDate = DateTime.Now;
        }
    }
}