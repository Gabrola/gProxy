using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using System.Runtime.CompilerServices;

namespace gProxyAPI
{
    /// <summary>
    /// Structure for players and monsters
    /// </summary>
    [StructLayout(LayoutKind.Sequential)]
    public struct Entity
    {
        /// <summary>
        /// Entity X Coordinate
        /// </summary>
        public UInt16 X;

        /// <summary>
        /// Entity Y Coordinate
        /// </summary>
        public UInt16 Y;

        /// <summary>
        /// Entity unique identifier
        /// </summary>
        public UInt32 UID;

        /// <summary>
        /// Entity status flag
        /// </summary>
        public UInt64 StatusFlag;
        private unsafe fixed sbyte sName[16];

        /// <summary>
        /// Entity level (monsters only)
        /// </summary>
        public byte Level;

        /// <summary>
        /// Entity HP (monsters only)
        /// </summary>
        public UInt16 HP;


        /// <summary>
        /// Entity name
        /// </summary>
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

        /// <summary>
        /// Gets whether an entity is a monster or not
        /// </summary>
        public bool IsMonster()
        {
            return (this.UID < 1000000 && !IsGuard());
        }

        /// <summary>
        /// Gets whether an entity is a guard or not
        /// </summary>
        public bool IsGuard()
        {
            return (this.Name.Contains("Guard"));
        }

        /// <summary>
        /// Default constructor
        /// </summary>

        public Entity(uint _UID)
        {
            X = 0;
            Y = 0;
            UID = _UID;
            StatusFlag = 0;
            Level = 0;
            HP = 0;
        }
    }
}
