namespace EqnestrianBooking.Migrations
{
    using System;
    using System.Data.Entity.Migrations;
    
    public partial class AddFishUpdateDate : DbMigration
    {
        public override void Up()
        {
            CreateTable(
                "dbo.Breeds",
                c => new
                    {
                        ID = c.Int(nullable: false, identity: true),
                        BreedName = c.String(nullable: false, maxLength: 200),
                    })
                .PrimaryKey(t => t.ID);
            
            CreateTable(
                "dbo.Coaches",
                c => new
                    {
                        ID = c.Int(nullable: false, identity: true),
                        CoachName = c.String(nullable: false, maxLength: 100),
                        CellPhone = c.String(nullable: false, maxLength: 200),
                        OnDutyDay = c.String(nullable: false, maxLength: 200),
                        DisplayColor = c.String(nullable: false, maxLength: 200),
                    })
                .PrimaryKey(t => t.ID);
            
            CreateTable(
                "dbo.Customers",
                c => new
                    {
                        ID = c.Int(nullable: false, identity: true),
                        CustomerName = c.String(nullable: false, maxLength: 100),
                        CellPhone1 = c.String(nullable: false, maxLength: 200),
                        CellPhone2 = c.String(maxLength: 200),
                        EmailAddress = c.String(nullable: false, maxLength: 200),
                        Address = c.String(nullable: false, maxLength: 200),
                        IntroducedBy = c.String(maxLength: 200),
                        RemainTickets = c.Int(nullable: false),
                        LastPayDate = c.DateTime(nullable: false),
                        Birthday = c.DateTime(nullable: false),
                    })
                .PrimaryKey(t => t.ID);
            
            CreateTable(
                "dbo.FishUpdateDatas",
                c => new
                    {
                        ID = c.Int(nullable: false, identity: true),
                        FishID = c.Int(nullable: false),
                        FishWeight = c.Int(nullable: false),
                        FishLength = c.Int(nullable: false),
                        FishImageData = c.Binary(),
                        UpdateDate = c.DateTime(nullable: false),
                    })
                .PrimaryKey(t => t.ID);
            
            CreateTable(
                "dbo.Horses",
                c => new
                    {
                        ID = c.Int(nullable: false, identity: true),
                        HorseName = c.String(nullable: false, maxLength: 100),
                        Birthday = c.DateTime(nullable: false),
                        Breed = c.Int(nullable: false),
                        Height = c.String(nullable: false, maxLength: 200),
                        Sex = c.String(nullable: false, maxLength: 200),
                        Temper = c.String(nullable: false, maxLength: 200),
                        DisplayColor = c.String(maxLength: 200),
                    })
                .PrimaryKey(t => t.ID);
            
            CreateTable(
                "dbo.Reservations",
                c => new
                    {
                        ID = c.Int(nullable: false, identity: true),
                        ReservationTitle = c.String(nullable: false, maxLength: 100),
                        Applicant = c.String(nullable: false, maxLength: 200),
                        ApplicantID = c.Int(nullable: false),
                        Coach = c.String(nullable: false, maxLength: 50),
                        CoachID = c.Int(nullable: false),
                        Horse = c.String(maxLength: 50),
                        HorseID = c.Int(nullable: false),
                        HowPaid = c.Int(nullable: false),
                        IsPaid = c.Boolean(nullable: false),
                        IsClean = c.Boolean(nullable: false),
                        CreateOn = c.DateTime(nullable: false),
                        StartDate = c.DateTime(nullable: false),
                        EndDate = c.DateTime(nullable: false),
                        Remark = c.String(),
                        CancelReason = c.String(),
                    })
                .PrimaryKey(t => t.ID);
            
        }
        
        public override void Down()
        {
            DropTable("dbo.Reservations");
            DropTable("dbo.Horses");
            DropTable("dbo.FishUpdateDatas");
            DropTable("dbo.Customers");
            DropTable("dbo.Coaches");
            DropTable("dbo.Breeds");
        }
    }
}
