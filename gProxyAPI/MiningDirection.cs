using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

#pragma warning disable 1591
namespace gProxyAPI
{
    /// <summary>Mining directions</summary>
    public enum MiningDirection : byte
    {
        /// <summary>North</summary>
        North = 3,
        /// <summary>NorthEast</summary>
        NorthEast = 4,
        /// <summary>East</summary>
        East = 5,
        /// <summary>SouthEast</summary>
        SouthEast = 6,
        /// <summary>South</summary>
        South = 7,
        /// <summary>SouthWest</summary>
        SouthWest = 0,
        /// <summary>West</summary>
        West = 1,
        /// <summary>NorthWest</summary>
        NorthWest = 2
    }
}
