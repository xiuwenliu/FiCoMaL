using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Drawing;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.IO;

namespace CLHY_weight_sensor
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {

        private SerialPort serialPort;
        private static string rxData;
        public MainWindow()
        {
            InitializeComponent();
            serialPort = new SerialPort();
            string[] serialPorts = SerialPort.GetPortNames();
            foreach (string sp in serialPorts)
            {
                comportCb.Items.Add(sp);
                if (comportCb.Items.Count > 0)
                {
                    comportCb.SelectedIndex = 0;
                }
            }

        }

        public byte[] BmpToBytes(Bitmap bmp)
        {
            System.IO.MemoryStream ms = new System.IO.MemoryStream();
            bmp.Save(ms, System.Drawing.Imaging.ImageFormat.Png);
            byte[] b = ms.GetBuffer();
            return b;
        }

        private void connectBtn_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                serialPort.PortName = comportCb.Text;
                serialPort.BaudRate = 9600;
                // make sure port isn't open	
                if (!serialPort.IsOpen)
                {

                    // set status
                   // readTextBox.Text = serialPort.PortName + " Ready!";
                    //readTextBox.AppendText(serialPortRead.PortName + " Ready!");
                    //open serial port 
                    serialPort.Open();
                    serialPort.DataReceived += new SerialDataReceivedEventHandler(serialPort1_DataReceived);
                    // serialPortRead.Open();
                    // prevent reinitiation 
                    //openPort.Enabled = false;
                }
                else
                    MessageBox.Show("Port isn't openned");
            }
            catch (UnauthorizedAccessException ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        static string pre;
        void serialPort1_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            string base64String = "";
            String data = serialPort.ReadLine();
            Dispatcher.BeginInvoke(new Action(() =>
            {
                //string s = data+ "g";

                if (!data.Contains('s'))
                    this.weightLb.Content = "重量:"+data.Substring(0, data.IndexOf('.'))+"克";
            }));

            if (data.Contains('s'))
            {
                Bitmap bmp = new Bitmap("C:\\FiCoMaL\\Fishackathon_Recognition\\Fishackathon_Recognition\\person\\capimg.jpg"); ;
              //  Graphics g = Graphics.FromImage(bmp);
               //  Bitmap bmpResize = new Bitmap(bmp,new Size(320,240));
                
                base64String = Convert.ToBase64String(BmpToBytes(bmp));

                string text = System.IO.File.ReadAllText("C:\\FiCoMaL\\Fishackathon_Recognition\\Fishackathon_Recognition\\person\\fishname.txt");

                string[] texts = text.Split(' ');


                //MessageBox.Show("123");
                string Url = "http://localhost:9605/FishIndex/UpdateFishData";
                HttpWebRequest request = HttpWebRequest.Create(Url) as HttpWebRequest;

                // request.ServicePoint.BindIPEndPointDelegate = new BindIPEndPoint(BindIPEndPoint1);
                string result = null;
                request.Method = "POST";    // 方法
                request.KeepAlive = true; //是否保持連線
                request.ContentType = "application/x-www-form-urlencoded";

                string param = "FishID="+texts[0]+"&FishLength="+texts[1]+"&FishWeight=" + pre + "&FishImageData=" + base64String;
                byte[] bs = Encoding.ASCII.GetBytes(param);

                using (Stream reqStream = request.GetRequestStream())
                {
                    reqStream.Write(bs, 0, bs.Length);
                }

                using (WebResponse response = request.GetResponse())
                {
                    StreamReader sr = new StreamReader(response.GetResponseStream());
                    result = sr.ReadToEnd();
                    MessageBox.Show(result);
                    sr.Close();
                }

                
            }

            if (!data.Contains('s'))
                pre = data.Substring(0, data.IndexOf('.'));

            // this.Invoke(new EventHandler(DisplayText));
        }
    }
}


