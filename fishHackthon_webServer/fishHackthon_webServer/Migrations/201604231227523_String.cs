namespace EqnestrianBooking.Migrations
{
    using System;
    using System.Data.Entity.Migrations;
    
    public partial class String : DbMigration
    {
        public override void Up()
        {
            AddColumn("dbo.FishUpdateDatas", "FishImageDataString", c => c.String());
        }
        
        public override void Down()
        {
            DropColumn("dbo.FishUpdateDatas", "FishImageDataString");
        }
    }
}
