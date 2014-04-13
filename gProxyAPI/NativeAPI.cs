using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

#pragma warning disable 1591
namespace gProxyAPI
{
    class NativeAPI
    {
        [DllImport("gProxyNativeAPI.dll")]
        public static extern void SendClient(IntPtr Client, [MarshalAs(UnmanagedType.LPArray)] byte[] Packet, int PacketLen);

        [DllImport("gProxyNativeAPI.dll")]
        public static extern void SendServer(IntPtr Client, [MarshalAs(UnmanagedType.LPArray)] byte[] Packet, int PacketLen);

        [DllImport("gProxyNativeAPI.dll")]
        public static unsafe extern IntPtr* GetClients(int* Num);

        [DllImport("gProxyNativeAPI.dll")]
        public static unsafe extern IntPtr GetMap(IntPtr Client);

        [DllImport("gProxyNativeAPI.dll")]
        public static unsafe extern IntPtr* GetEntities(IntPtr Client, int* Num);

        [DllImport("gProxyNativeAPI.dll")]
        public static unsafe extern IntPtr* GetNpcs(IntPtr Client, int* Num);

        [DllImport("gProxyNativeAPI.dll")]
        public static unsafe extern IntPtr* GetCharacterItems(IntPtr Client, int* Num);

        [DllImport("gProxyNativeAPI.dll")]
        public static unsafe extern IntPtr* GetGroundItems(IntPtr Client, int* Num);

        [DllImport("gProxyNativeAPI.dll")]
        public static unsafe extern byte* GetNpcChoicesNum(IntPtr Client, int* Num);

        [DllImport("gProxyNativeAPI.dll")]
        public static unsafe extern sbyte** GetNpcChoicesText(IntPtr Client, int* Num);

        [DllImport("gProxyNativeAPI.dll")]
        public static unsafe extern sbyte* GetNpcText(IntPtr Client);

        [DllImport("gProxyNativeAPI.dll")]
        public static unsafe extern void DeleteClientArray(IntPtr* Num);

        [DllImport("gProxyNativeAPI.dll")]
        public static unsafe extern void GetCharacterName(IntPtr Client, sbyte* buffer);

        [DllImport("gProxyNativeAPI.dll")]
        public static unsafe extern void GetSpouseName(IntPtr Client, sbyte* buffer);

        [DllImport("gProxyNativeAPI.dll")]
        public static extern UInt32 GetUID(IntPtr Client);

        [DllImport("gProxyNativeAPI.dll")]
        public static extern UInt32 GetModel(IntPtr Client);

        [DllImport("gProxyNativeAPI.dll")]
        public static extern UInt32 GetGold(IntPtr Client);

        [DllImport("gProxyNativeAPI.dll")]
        public static extern UInt32 GetConquerPoints(IntPtr Client);

        [DllImport("gProxyNativeAPI.dll")]
        public static extern UInt32 GetBoundConquerPoints(IntPtr Client);

        [DllImport("gProxyNativeAPI.dll")]
        public static extern UInt64 GetExperience(IntPtr Client);

        [DllImport("gProxyNativeAPI.dll")]
        public static extern UInt16 GetStrengthPoints(IntPtr Client);

        [DllImport("gProxyNativeAPI.dll")]
        public static extern UInt16 GetDexterityPoints(IntPtr Client);

        [DllImport("gProxyNativeAPI.dll")]
        public static extern UInt16 GetVitalityPoints(IntPtr Client);

        [DllImport("gProxyNativeAPI.dll")]
        public static extern UInt16 GetSpiritPoints(IntPtr Client);

        [DllImport("gProxyNativeAPI.dll")]
        public static extern UInt16 GetStatPoints(IntPtr Client);

        [DllImport("gProxyNativeAPI.dll")]
        public static extern UInt16 GetHP(IntPtr Client);

        [DllImport("gProxyNativeAPI.dll")]
        public static extern UInt16 GetMP(IntPtr Client);

        [DllImport("gProxyNativeAPI.dll")]
        public static extern UInt16 GetPKPoints(IntPtr Client);

        [DllImport("gProxyNativeAPI.dll")]
        public static extern byte GetLevel(IntPtr Client);

        [DllImport("gProxyNativeAPI.dll")]
        public static extern byte GetClass(IntPtr Client);

        [DllImport("gProxyNativeAPI.dll")]
        public static extern byte GetReborn(IntPtr Client);

        [DllImport("gProxyNativeAPI.dll")]
        public static extern UInt32 GetQuizPoints(IntPtr Client);

        [DllImport("gProxyNativeAPI.dll")]
        public static extern UInt16 GetMapID(IntPtr Client);

        [DllImport("gProxyNativeAPI.dll")]
        public static extern UInt16 GetStamina(IntPtr Client);

        [DllImport("gProxyNativeAPI.dll")]
        public static extern UInt64 GetStatusFlag(IntPtr Client);

        [DllImport("gProxyNativeAPI.dll")]
        public static extern UInt16 GetX(IntPtr Client);

        [DllImport("gProxyNativeAPI.dll")]
        public static extern UInt16 GetY(IntPtr Client);

        [DllImport("gProxyNativeAPI.dll")]
        public static extern UInt32 GetPing(IntPtr Client);

        [DllImport("gProxyNativeAPI.dll")]
        public static extern byte GetAlternativeGear(IntPtr Client);

        [DllImport("gProxyNativeAPI.dll")]
        public static extern void Jump(IntPtr Client, UInt16 X, UInt16 Y);

        [DllImport("gProxyNativeAPI.dll")]
        public static extern void UpdateLocation(IntPtr Client);

        [DllImport("gProxyNativeAPI.dll")]
        public static extern void Melee(IntPtr Client, UInt32 UID, Int32 AttackType);

        [DllImport("gProxyNativeAPI.dll")]
        public static extern void CastSpell(IntPtr Client, UInt32 UID, UInt16 SpellID);

        [DllImport("gProxyNativeAPI.dll")]
        public static extern void CastSpellPos(IntPtr Client, UInt16 X, UInt16 Y, UInt16 SpellID);

        [DllImport("gProxyNativeAPI.dll")]
        public static extern void Mine(IntPtr Client, byte Direction);

        [DllImport("gProxyNativeAPI.dll")]
        public static extern void PickupItem(IntPtr Client, UInt32 GUID);

        [DllImport("gProxyNativeAPI.dll")]
        public static extern void Shift(IntPtr Client, UInt16 X, UInt16 Y);

        [DllImport("gProxyNativeAPI.dll")]
        public static extern void Portal(IntPtr Client, UInt16 PortalNum);

        [DllImport("gProxyNativeAPI.dll")]
        public static extern void SpeakNpc(IntPtr Client, UInt32 ID);

        [DllImport("gProxyNativeAPI.dll")]
        public static extern void ReplyNpc(IntPtr Client, byte Num);

        [DllImport("gProxyNativeAPI.dll")]
        public static extern void EndNpc(IntPtr Client);

        [DllImport("gProxyNativeAPI.dll")]
        public static extern void SetTeleportTrue(IntPtr Client);

        [DllImport("gProxyNativeAPI.dll")]
        public static extern byte GetTeleportStatus(IntPtr Client);

        [DllImport("gProxyNativeAPI.dll")]
        public static extern void AddAbortThreadOnDC(IntPtr Client, IntPtr tHandle);

        [DllImport("gProxyNativeAPI.dll")]
        public static extern void RemoveAbortThreadOnDC(IntPtr Client, IntPtr tHandle);

        [DllImport("gProxyNativeAPI.dll", CharSet=CharSet.Ansi)]
        public static extern void SendClientMessage(IntPtr Client, string From, string To, string Message, UInt16 Type, int Color);

        [DllImport("gProxyNativeAPI.dll")]
        public static extern void SetPosition(IntPtr Client, UInt16 X, UInt16 Y);

        [DllImport("gProxyNativeAPI.dll")]
        public static extern void RemoveEntity(IntPtr Client, UInt32 UID);

        [DllImport("gProxyNativeAPI.dll")]
        public static extern byte GetMapLoaded(IntPtr Client);
    }
}
