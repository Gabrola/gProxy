using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using System.Runtime.CompilerServices;

#pragma warning disable 1591
namespace gProxyAPI
{
    /// <summary>Structure for an item on the ground</summary>
    [StructLayout(LayoutKind.Sequential)]
    public struct GroundItem
    {
        /// <summary>Unique identifier, different from the one you get when you pickup the item</summary>
        public UInt32 GroundUID;
        /// <summary>Static identifier, the one in itemtype.dat</summary>
        public UInt32 StaticID;
        /// <summary>Item X coordinate</summary>
        public UInt16 X;
        /// <summary>Item Y coordinate</summary>
	    public UInt16 Y;
        private unsafe fixed sbyte sName[100];

        /// <summary>Item name</summary>
        public unsafe string Name
        {
            get
            {
                fixed (sbyte* namePntr = this.sName)
                {
                    return new string(namePntr);
                }
            }
        }
    }
}
