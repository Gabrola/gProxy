using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace gProxyAPI
{
    /// <summary>
    /// Chat types for <see cref="GameClient.SendClientMessage"/>
    /// </summary>
    public enum ChatType : ushort
    {
        /// <summary>Talk chat</summary>
        Talk = 2000,

        /// <summary>Whisper chat</summary>
        Whisper = 2001,

        /// <summary>Action chat</summary>
        Action = 2002,

        /// <summary>Team chat</summary>
        Team = 2003,

        /// <summary>Guild chat</summary>
        Guild = 2004,

        /// <summary>Top left message</summary>
        Top = 2005,

        /// <summary>Clan chat</summary>
        Clan = 2006,

        /// <summary>Friend chat</summary>
        Friend = 2009,

        /// <summary>Center message</summary>
        Center = 2011,

        /// <summary>GM message</summary>
        Service = 2014,

        /// <summary>World chat</summary>
        World = 2021,

        /// <summary>Qualifier message</summary>
        Qualifier = 2022,

        /// <summary>Top right message</summary>
        MiniMap = 2108,

        /// <summary>Adds a message top right</summary>
        DisplayScores = 2109
    }
}
