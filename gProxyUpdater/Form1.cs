using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Reflection;
using System.Net;
using System.IO;
using System.Threading;
using System.Diagnostics;

namespace gProxyUpdater
{
    public partial class Form1 : Form
    {
        public static string[] UserPass;

        public Form1()
        {
            InitializeComponent();
        }

        public static long HttpPost(string uri, string parameters)
        {
            WebRequest webRequest = WebRequest.Create(uri);
            webRequest.ContentType = "application/x-www-form-urlencoded";
            webRequest.Method = "POST";
            byte[] bytes = Encoding.ASCII.GetBytes(parameters);
            Stream os = null;
            try
            { 
                webRequest.ContentLength = bytes.Length;
                os = webRequest.GetRequestStream();
                os.Write(bytes, 0, bytes.Length);
            }
            catch (WebException ex)
            {
                MessageBox.Show(ex.Message, "HttpPost: Request error",
                   MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            finally
            {
                if (os != null)
                {
                    os.Close();
                }
            }

            try
            {
                WebResponse webResponse = webRequest.GetResponse();
                if (webResponse == null)
                { return 0; }
                return webResponse.ContentLength;
            }
            catch (WebException ex)
            {
                MessageBox.Show(ex.Message, "HttpPost: Response error",
                   MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            return 0;
        }

        public static string HttpGet(string uri)
        {
            WebRequest webRequest = WebRequest.Create(uri);
            webRequest.Method = "GET";
            try
            {
                WebResponse webResponse = webRequest.GetResponse();
                if (webResponse == null)
                { return null; }
                StreamReader sr = new StreamReader(webResponse.GetResponseStream());
                return sr.ReadToEnd().Trim();
            }
            catch (WebException ex)
            {
                MessageBox.Show(ex.Message, "HttpPost: Response error",
                   MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            return null;
        }

        public void WriteLine(string Value)
        {
            textBox1.Text = "> " + Value + "\r\n" + textBox1.Text;
        }

        void client_UploadProgressChanged(object sender, UploadProgressChangedEventArgs e)
        {
            progressBar1.Value = (int) (e.BytesReceived * 100 / e.TotalBytesToReceive);
        }

        void client_UploadDataCompleted(object sender, UploadDataCompletedEventArgs e)
        {
            try
            {
                File.WriteAllBytes("tempDownload.exe", e.Result);
                WriteLine("Download Complete!");
                Process[] Procs = Process.GetProcessesByName("gProxy");
                foreach (Process proc in Procs)
                {
                    proc.Kill();
                    proc.WaitForExit();
                }

                Process downProc = new Process();
                downProc.StartInfo = new ProcessStartInfo("tempDownload.exe");
                downProc.Start();
                downProc.WaitForExit();
                downProc.Close();

                File.Delete("tempDownload.exe");

                Process.Start("gProxy.exe");
                Process.GetCurrentProcess().Kill();
            }
            catch{}
        }

        private void Form1_Shown(object sender, EventArgs e)
        {
            new Thread
            (
                delegate()
                {
                    try
                    {
                        string YourVersion = System.Diagnostics.FileVersionInfo.GetVersionInfo(System.IO.Directory.GetCurrentDirectory() + "\\gProxy.exe").ProductVersion;
                        WriteLine("Your Version:\t\t" + YourVersion);

                        string CurrentVersion = HttpGet("http://g-proxy.info/version.dat");
                        WriteLine("Current Version:\t" + CurrentVersion);

                        if (YourVersion == CurrentVersion)
                        {
                            WriteLine("You are already up-to-date!");
                            return;
                        }

                        WriteLine("Downloading gProxy version " + CurrentVersion + ", please wait...");

                        byte[] gProxy = new byte[1];
                        progressBar1.Maximum = 100;
                        progressBar1.Value = 0;

                        WebClient client = new WebClient();
                        client.Headers.Add("Content-Type", "application/x-www-form-urlencoded");
                        client.UploadProgressChanged += new UploadProgressChangedEventHandler(client_UploadProgressChanged);
                        client.UploadDataCompleted += new UploadDataCompletedEventHandler(client_UploadDataCompleted);
                        client.UploadDataAsync(new Uri("http://g-proxy.info/update.php"), "POST", Encoding.ASCII.GetBytes("username=" + UserPass[0] + "&password=" + UserPass[1]));
                    }
                    catch(Exception ez)
                    {
                        WriteLine(ez.Message);
                    }
                }
            ).Start();
        }
    }
}
