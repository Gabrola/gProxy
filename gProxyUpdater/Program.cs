using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;

namespace gProxyUpdater
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main(string[] Args)
        {
            if (Args.Length == 2)
                Form1.UserPass = Args;
            else
                System.Diagnostics.Process.GetCurrentProcess().Kill();

            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new Form1());
        }
    }
}
