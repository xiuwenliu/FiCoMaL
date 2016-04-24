using System;
using System.Collections.Generic;
using System.Data;
using System.Data.Entity;
using System.Data.Entity.Validation;
using System.Data.SqlClient;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Web;

namespace EqnestrianBooking.Models
{
    public class SampleContext : DbContext
    {
        

        public DbSet<Reservation> Reservations { get; set; }
        public DbSet<Coach> Coachs { get; set; }
        public DbSet<Customer> Customers { get; set; }
        public DbSet<Horse> Horses { get; set; }
        public DbSet<Breed> Breeds { get; set; }

        public DbSet<FishUpdateData> FishUpdateDatas { get; set; }

        public static SampleContext Instance = null;

        public SampleContext()
            : base("name=fishHackathon")
        {           
        }

        //Sington為了不讓controller一直去new
        public static SampleContext GetInstance()
        {
            return (Instance == null) ? new SampleContext() : Instance;
        }

        //覆寫DbContext的SaveChanges()
        public override int SaveChanges()
        {
            try
            {
                return base.SaveChanges();
            }
            catch (DbEntityValidationException ex)
            {
                var sb = new StringBuilder();

                foreach (var failure in ex.EntityValidationErrors)
                {
                    sb.AppendFormat("{0} failed validation\n", failure.Entry.Entity.GetType());
                    foreach (var error in failure.ValidationErrors)
                    {
                        sb.AppendFormat("- {0} : {1}", error.PropertyName, error.ErrorMessage);
                        sb.AppendLine();
                    }
                }

                throw new DbEntityValidationException(
                    "Entity Validation Failed - errors follow:\n" +
                    sb.ToString(), ex
                    ); // Add the original exception as the innerException
            }
        }
    }
}