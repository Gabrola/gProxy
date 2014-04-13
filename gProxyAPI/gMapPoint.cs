using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using System.Runtime.CompilerServices;

namespace gProxyAPI
{
    /// <summary>Single map point structure</summary>
    [StructLayout(LayoutKind.Sequential)]
    public struct gMapPoint
    {
        private byte _Accessible;
        /// <summary>Surface type (no possible use)</summary>
        public UInt16 SurfaceType;
        /// <summary>Tile height</summary>
        public UInt16 Height;

        /// <summary>Coordinate accessible or not</summary>
        public bool Accessible
        {
            get { return Convert.ToBoolean(_Accessible); }
        }
    }
}
