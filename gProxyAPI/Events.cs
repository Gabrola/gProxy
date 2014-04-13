using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using System.Runtime.CompilerServices;

#pragma warning disable 1591
namespace gProxyAPI
{
    public enum EventType : int
    {
        HpChange = 1,
        MpChange = 2,
        Jump = 3,
        Walk = 4,
        StatusFlagChange = 5,
        EntitySpawn = 6,
        ItemDrop = 7,
        Chat = 8,
        Attack = 9,
        SpellCast = 10
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct HpChange
    {
        public uint UID;
        public uint HP;
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct MpChange
    {
        public uint UID;
        public uint MP;
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct Jump
    {
        public uint UID;
        public ushort X;
        public ushort Y;
        public ushort FromX;
        public ushort FromY;
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct Walk
    {
        public uint UID;
        public byte Direction;
        public bool Running;
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct StatusFlagChange
    {
        public uint UID;
        public ulong StatusFlag;
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct EntitySpawn
    {
        public uint UID;
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct ItemDrop
    {
        public uint GroundUID;
        public uint StaticID;
        public ushort X;
        public ushort Y;
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct Chat
    {
        public string From;
        public string To;
        public string Message;
        public uint Type;
        public uint Color;
        public byte Send;
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct Attack
    {
        public uint AttackerUID;
        public uint TargetUID;
        public ushort X;
        public ushort Y;
        public uint Damage;
        public ushort SpellID;
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct SpellCast
    {
        public uint AttackerUID;
        public uint TargetUID;
        public ushort X;
        public ushort Y;
        public uint Damage;
        public ushort SpellID;
    }
}
