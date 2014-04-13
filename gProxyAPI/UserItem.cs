using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using System.Runtime.CompilerServices;

#pragma warning disable 1591
namespace gProxyAPI
{
    /// <summary>Structure of an item in a user's inventory or gear</summary>
    [StructLayout(LayoutKind.Sequential)]
    public struct UserItem
    {
        /// <summary>Item UID</summary>
        public UInt32 UID;
        /// <summary>Item Static ID</summary>
        public UInt32 StaticID;
        /// <summary>Item durability/amount</summary>
        public UInt16 Dura;
        /// <summary>Item max durability/amount</summary>
        public UInt16 MaxDura;
        /// <summary>Item position in gear/inventory see <see cref="ItemPosition"/></summary>
        public ItemPosition Position;
        /// <summary>Item extra attributes</summary>
        public UInt16 RebornEffect;
        /// <summary>Item first socket gem</summary>
        public byte FirstSocket;
        /// <summary>Item second socket gem</summary>
        public byte SecondSocket;
        /// <summary>Item +s</summary>
        public byte Plus;
        /// <summary>Item -%</summary>
        public byte Bless;
        /// <summary>Item bound or not</summary>
        public byte Free;
        /// <summary>Item enchanted HP</summary>
        public byte EnchantedHP;
        /// <summary>Item locked or not</summary>
        public byte Locked;
        /// <summary>Item color</summary>
        public byte Color;
        /// <summary>Item composition points</summary>
        public UInt32 CompositionProgress;
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
