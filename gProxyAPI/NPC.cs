using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using System.Runtime.CompilerServices;

#pragma warning disable 1591
namespace gProxyAPI
{
    /// <summary>NPC structure</summary>
    [StructLayout(LayoutKind.Sequential)]
    public struct NPC
    {
        /// <summary>NPC unique identifier</summary>
        public UInt32 ID;
        /// <summary>NPC X coordinate</summary>
        public UInt16 X;
        /// <summary>NPC Y coordinate</summary>
        public UInt16 Y;
        /// <summary>NPC model/type</summary>
        public UInt16 Type;
        private unsafe fixed sbyte sName[50];

        /// <summary>NPC name</summary>
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
