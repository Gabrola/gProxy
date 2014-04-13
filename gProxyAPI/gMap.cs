using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using System.Runtime.CompilerServices;
using System.IO;
using System.Windows.Forms;

namespace gProxyAPI
{
    /// <summary>
    /// Map structure
    /// </summary>
    [StructLayout(LayoutKind.Sequential)]
    public struct gMap
    {
        private unsafe fixed sbyte sName[100];
        /// <summary>Map width</summary>
        public UInt16 Width;
        /// <summary>Map height</summary>
        public UInt16 Height;
        private IntPtr Accessibility;

        /// <summary>Check if a coordinate is accessible</summary>
        public bool this[int x, int y]
        {
            get
            {
                byte Cell = (byte)Marshal.PtrToStructure((IntPtr)(Accessibility.ToInt32() + ((int) Math.Floor(((y * Width) + x) / 8d))), typeof(byte));
                int Offset = ((y * Width) + x) % 8;

                switch (Offset)
                {
                    case 0:
                        return ((Cell & 1) == 1);

                    case 1:
                        return ((Cell & 2) == 2);

                    case 2:
                        return ((Cell & 4) == 4);

                    case 3:
                        return ((Cell & 8) == 8);

                    case 4:
                        return ((Cell & 16) == 16);

                    case 5:
                        return ((Cell & 32) == 32);

                    case 6:
                        return ((Cell & 64) == 64);

                    case 7:
                        return ((Cell & 128) == 128);
                }

                return false;
            }
        }

        /// <summary>Map name</summary>
        public unsafe string Name
        {
            get
            {
                try
                {
                    fixed (sbyte* namePntr = this.sName)
                    {
                        return Path.GetFileNameWithoutExtension(new string(namePntr));
                    }
                }
                catch (System.Exception ex)
                {
                    MessageBox.Show(ex.ToString());
                    return "";
                }
            }
        }
    }
}
