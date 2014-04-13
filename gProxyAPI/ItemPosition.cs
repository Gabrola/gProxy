using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

#pragma warning disable 1591
namespace gProxyAPI
{
    /// <summary>Item's position in your gears or inventory</summary>
    public enum ItemPosition : byte
    {
        /// <summary>Inventory</summary>
        Inventory = 0,
        /// <summary>Headgear</summary>
        Head = 1,
        /// <summary>Necklace</summary>
        Necklace = 2,
        /// <summary>Armor</summary>
        Armor = 3,
        /// <summary>Right hand weapon</summary>
        RightHand = 4,
        /// <summary>Left hand weapon</summary>
        LeftHand = 5,
        /// <summary>Ring</summary>
        Ring = 6,
        /// <summary>Bottle talisman</summary>
        Bottle = 7,
        /// <summary>Boots</summary>
        Boots = 8,
        /// <summary>Garment</summary>
        Garment = 9,
        /// <summary>Fan talisman</summary>
        Fan = 10,
        /// <summary>Tower talisman</summary>
        Tower = 11,
        /// <summary>Steed</summary>
        Steed = 12,
        /// <summary>Alternative gear headgear</summary>
        AlternativeHead = 21,
        /// <summary>Alternative gear necklace</summary>
        AlternativeNecklace = 22,
        /// <summary>Alternative gear armor</summary>
        AlternativeArmor = 23,
        /// <summary>Alternative right hand weapon</summary>
        AlternativeRightHand = 24,
        /// <summary>Alternative left hand weapon</summary>
        AlternativeLeftHand = 25,
        /// <summary>Alternative gear ring</summary>
        AlternativeRing = 26,
        /// <summary>Alternative gear bottle talisman</summary>
        AlternativeBottle = 27,
        /// <summary>Alternative gear boots</summary>
        AlternativeBoots = 28,
        /// <summary>Alternative gear garment</summary>
        AlternativeGarment = 29
    }
}
