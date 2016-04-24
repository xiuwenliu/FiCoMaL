using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.ComponentModel.DataAnnotations;
using System.Linq;
using System.Web;

namespace EqnestrianBooking.Models
{
    public class Breed
    {
        [DisplayName("ID")]
        [Key]
        [Required]
        public int ID { get; set; }

        [DisplayName("品種")]
        [Required]
        [StringLength(200)]
        public string BreedName { get; set; }

        Breed() { }
    }
}