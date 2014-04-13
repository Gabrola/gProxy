using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using System.Runtime.CompilerServices;
using System.Threading;
using System.Drawing;

#pragma warning disable 1591
namespace gProxyAPI
{
    ///<summary>
    ///Main class for interacting with the client and server
    ///</summary>
    public class GameClient
    {
        private IntPtr Instance;
        private static Dictionary<IntPtr, gDictionary> pObjects = new Dictionary<IntPtr, gDictionary>();

        ///<summary>
        ///Initialize the class using the IntPtr client instance
        ///</summary>
        ///<param name="ClientInstance">IntPtr of the client, get it from your ClientToServer or ServerToClient methods</param>
        public GameClient(IntPtr ClientInstance)
        {
            this.Instance = ClientInstance;
        }

        ///<summary>
        ///Initialize the class using character name
        ///</summary>
        ///<param name="CharName">Character name</param>
        public GameClient(string CharName)
        {
            GameClient[] Clients = GetClients();
            var clients = from p in Clients
                          where p.CharacterName == CharName
                          select p;

            if(clients.Count() > 0)
            {
                this.Instance = clients.ElementAt(0).Instance;
            }
            else
            {
                throw new ArgumentException("No client found with this character name " + CharName, "CharName");
            }
        }

        ///<summary>
        ///Sends a packet to the client
        ///</summary>
        ///<param name="Packet">Packet byte array that includes the 8 byte TQClient</param>
        public void SendClient(byte[] Packet)
        {
            NativeAPI.SendClient(this.Instance, Packet, Packet.Length);
        }

        ///<summary>
        ///Sends a packet to the server
        ///</summary>
        ///<param name="Packet">Packet byte array that includes the 8 byte TQServer</param>
        public void SendServer(byte[] Packet)
        {
            NativeAPI.SendServer(this.Instance, Packet, Packet.Length);
        }

        ///<summary>
        ///Drops gold from your character
        ///</summary>
        ///<param name="Gold">Amount of gold</param>
        public void DropGold(UInt64 Gold)
        {
            //NativeAPI.DropGold(this.Instance, Gold);
        }

        ///<summary>
        ///Jumps to a location but does not update your screen client-side
        ///</summary>
        ///<param name="X">X Coordinate</param>
        ///<param name="Y">Y Coordinate</param>
        public void Jump(UInt16 X, UInt16 Y)
        {
            NativeAPI.Jump(this.Instance, X, Y);
        }

        ///<summary>
        ///Instant location shift
        ///</summary>
        ///<param name="X">X Coordinate</param>
        ///<param name="Y">Y Coordinate</param>
        public void Shift(UInt16 X, UInt16 Y)
        {
            NativeAPI.Shift(this.Instance, X, Y);
        }

        ///<summary>
        ///Casts a spell on an entity or yourself
        ///</summary>
        ///<param name="UID">Entity UID or your UID</param>
        ///<param name="SpellID">Spell ID</param>
        ///<remarks>If the spell ID is not found in the enum just enter the numerical ID and type cast it with a (SpellID)</remarks>
        public void CastSpell(UInt32 UID, SpellID SpellID)
        {
            NativeAPI.CastSpell(this.Instance, UID, (UInt16) SpellID);
        }

        ///<summary>
        ///Casts a spell on a coordinate
        ///</summary>
        ///<param name="X">X Coordinate</param>
        ///<param name="Y">Y Coordinate</param>
        ///<param name="SpellID">Spell ID</param>
        ///<remarks>If the spell ID is not found in the enum just enter the numerical ID and type cast it with a (SpellID)</remarks>
        public void CastSpell(UInt16 X, UInt16 Y, SpellID SpellID)
        {
            NativeAPI.CastSpellPos(this.Instance, X, Y, (UInt16)SpellID);
        }

        ///<summary>
        ///Starts mining
        ///</summary>
        ///<param name="Direction">Direction of mining</param>
        public void Mine(MiningDirection Direction)
        {
            NativeAPI.Mine(this.Instance, (byte)Direction);
        }

        ///<summary>
        ///Pickup an item from the ground
        ///</summary>
        ///<param name="GUID">GUID of item</param>
        public void Pickup(uint GUID)
        {
            NativeAPI.PickupItem(this.Instance, GUID);
        }

        ///<summary>
        ///Enters a portal from anywhere
        ///</summary>
        ///<param name="PortalID">Portal ID</param>
        public void Portal(ushort PortalID)
        {
            NativeAPI.Portal(this.Instance, PortalID);
            NativeAPI.SetTeleportTrue(this.Instance);
        }

        ///<summary>
        ///Enters a portal from anywhere and waits until teleport finishes
        ///</summary>
        ///<param name="PortalID">Portal ID</param>
        ///<param name="Timeout">Maximum waiting time in miliseconds</param>
        public void PortalWait(ushort PortalID, uint Timeout)
        {
            NativeAPI.Portal(this.Instance, PortalID);
            NativeAPI.SetTeleportTrue(this.Instance);

            DateTime Now = DateTime.Now;
            while(NativeAPI.GetTeleportStatus(this.Instance) == 1)
            {
                if (Now.AddMilliseconds(Timeout) < DateTime.Now)
                    break;
                Thread.Sleep(1);
            }
        }

        ///<summary>
        ///Updates your screen client-side with the current coordinates
        ///</summary>
        public void UpdateLocation()
        {
            NativeAPI.UpdateLocation(this.Instance);
        }

        ///<summary>
        ///Attacks an entity given the UID
        ///</summary>
        ///<param name="UID">UID of the entity</param>
        ///<param name="AttackType">The name says it all</param>
        public void Melee(UInt32 UID, AttackType AttackType)
        {
            NativeAPI.Melee(this.Instance, UID, (Int32)AttackType);
        }

        ///<summary>
        ///Attacks an entity given the UID
        ///</summary>
        ///<param name="UID">UID of the entity</param>
        ///<param name="AttackType">The name says it all</param>
        public void FSMelee(UInt32 UID, UInt16 X, UInt16 Y)
        {
            NativeAPI.Melee(this.Instance, UID, (int) AttackType.CloseRange);
            NativeAPI.SetPosition(this.Instance, X, Y);
        }

        ///<summary>
        ///Initiates an NPC dialog
        ///</summary>
        ///<param name="ID">NPC ID</param>
        public void NPCInitiate(UInt32 ID)
        {
            NativeAPI.SpeakNpc(this.Instance, ID);
        }

        ///<summary>
        ///Replies to an NPC
        ///</summary>
        ///<param name="Num">Reply number</param>
        public void NPCReply(byte Num)
        {
            NativeAPI.ReplyNpc(this.Instance, Num);
            NativeAPI.EndNpc(this.Instance);
        }

        ///<summary>
        ///Ends an NPC dialog
        ///</summary>
        public void NPCEnd()
        {
            NativeAPI.EndNpc(this.Instance);
            NativeAPI.EndNpc(this.Instance);
        }

        ///<summary>
        ///Aborts a thread if the client disconnects.
        ///Make sure to <see cref="RemoveAbortThread"/> right before the thread ends running
        ///</summary>
        ///<param name="thread">Thread instance</param>
        ///<returns>Thread handle</returns>
        public IntPtr AddAbortThread(Thread thread)
        {
            IntPtr tHandle = GCHandle.ToIntPtr(GCHandle.Alloc(thread));
            NativeAPI.AddAbortThreadOnDC(this.Instance, tHandle);
            return tHandle;
        }

        ///<summary>
        ///Removes thread from abort on disconnect list
        ///</summary>
        ///<param name="tHandle">Thread handle obtained from <see cref="AddAbortThread"/></param>
        public void RemoveAbortThread(IntPtr tHandle)
        {
            NativeAPI.RemoveAbortThreadOnDC(this.Instance, tHandle);
            GCHandle.FromIntPtr(tHandle).Free();
        }

        ///<summary>
        ///Sends a message to the client
        ///</summary>
        ///<param name="From">Message sender</param>
        ///<param name="To">Message recipient</param>
        ///<param name="Message">The message itself</param>
        ///<param name="Type">Chat type from ChatType enum</param>
        ///<param name="Clr">Color of text</param>
        public void SendClientMessage(string From, string To, string Message, ChatType Type, Color Clr)
        {
            NativeAPI.SendClientMessage(this.Instance, From, To, Message, (ushort)Type, Clr.ToArgb());
        }

        ///<summary>
        ///Sends a message to the client
        ///</summary>
        ///<param name="From">Message sender</param>
        ///<param name="To">Message recipient</param>
        ///<param name="Message">The message itself</param>
        ///<param name="Type">Chat type from ChatType enum</param>
        public void SendClientMessage(string From, string To, string Message, ChatType Type)
        {
            NativeAPI.SendClientMessage(this.Instance, From, To, Message, (ushort)Type, Color.White.ToArgb());
        }

        ///<summary>
        ///Sends a message to the client
        ///</summary>
        ///<param name="Message">The message itself</param>
        ///<param name="Type">Chat type from ChatType enum</param>
        ///<param name="Clr">Color of text</param>
        public void SendClientMessage(string Message, ChatType Type, Color Clr)
        {
            NativeAPI.SendClientMessage(this.Instance, "SYSTEM", "ALL", Message, (ushort)Type, Clr.ToArgb());
        }

        ///<summary>
        ///Sends a message to the client
        ///</summary>
        ///<param name="Message">The message itself</param>
        ///<param name="Type">Chat type from ChatType enum</param>
        public void SendClientMessage(string Message, ChatType Type)
        {
            NativeAPI.SendClientMessage(this.Instance, "SYSTEM", "ALL", Message, (ushort)Type, Color.White.ToArgb());
        }

        /// <summary>
        /// Removes the entity from the entity list
        /// </summary>
        ///<param name="EntityUID">Entity UID</param>
        public void RemoveEntity(UInt32 EntityUID)
        {
            NativeAPI.RemoveEntity(this.Instance, EntityUID);
        }

        /// <summary>
        /// Waits until map is loaded
        /// </summary>
        public void WaitLoad(UInt16 OldMap, uint Timeout)
        {
            DateTime Now = DateTime.Now;
            while (NativeAPI.GetMapLoaded(this.Instance) == 0 || MapID == OldMap)
            {
                if (Now.AddMilliseconds(Timeout) < DateTime.Now)
                    break;
                Thread.Sleep(1);
            }
        }

        ///<summary>
        ///Gets all connected clients
        ///</summary>
        public static unsafe GameClient[] GetClients()
        {
            int count;
            IntPtr* clients = NativeAPI.GetClients(&count);
            GameClient[] Clients = new GameClient[count];
            for (int i = 0; i < count; i++)
            {
                Clients[i] = new GameClient(clients[i]);
            }
            NativeAPI.DeleteClientArray(clients);
            return Clients;
        }

        ///<summary>
        ///Gets all entities (monsters or characters) on the screen
        ///</summary>
        public unsafe Entity[] Entities
        {
            get
            {
                int count;
                IntPtr* entities = NativeAPI.GetEntities(this.Instance, &count);
                Entity[] Entities = new Entity[count];
                for (int i = 0; i < count; i++)
                {
                    Entities[i] = (Entity)Marshal.PtrToStructure(entities[i], typeof(Entity));
                }
                NativeAPI.DeleteClientArray(entities);

                return Entities;
            }
        }

        ///<summary>
        ///Gets all NPCs on the screen
        ///</summary>
        public unsafe NPC[] NPCs
        {
            get
            {
                int count;
                IntPtr* npcs = NativeAPI.GetNpcs(this.Instance, &count);
                NPC[] NPCs = new NPC[count];
                for (int i = 0; i < count; i++)
                {
                    NPCs[i] = (NPC)Marshal.PtrToStructure(npcs[i], typeof(NPC));
                }
                NativeAPI.DeleteClientArray(npcs);

                return NPCs;
            }
        }

        ///<summary>
        ///Gets all items in the character's inventory and equipped items
        ///</summary>
        public unsafe UserItem[] UserItems
        {
            get
            {
                int count;
                IntPtr* entities = NativeAPI.GetCharacterItems(this.Instance, &count);
                UserItem[] Items = new UserItem[count];
                for (int i = 0; i < count; i++)
                {
                    Items[i] = (UserItem)Marshal.PtrToStructure(entities[i], typeof(UserItem));
                }
                NativeAPI.DeleteClientArray(entities);

                return Items;
            }
        }

        ///<summary>
        ///Gets all items on the ground
        ///</summary>
        public unsafe GroundItem[] GroundItems
        {
            get
            {
                int count;
                IntPtr* entities = NativeAPI.GetGroundItems(this.Instance, &count);
                GroundItem[] Items = new GroundItem[count];
                for (int i = 0; i < count; i++)
                {
                    Items[i] = (GroundItem)Marshal.PtrToStructure(entities[i], typeof(GroundItem));
                }
                NativeAPI.DeleteClientArray(entities);

                return Items;
            }
        }

        ///<summary>
        ///Gets NPC reply choices
        ///</summary>
        public unsafe NPCReply[] NPCReplies
        {
            get
            {
                int count1, count2;
                sbyte** messages = NativeAPI.GetNpcChoicesText(this.Instance, &count1);
                byte* numbers = NativeAPI.GetNpcChoicesNum(this.Instance, &count2);
                NPCReply[] Items = new NPCReply[count1];
                for (int i = 0; i < count1; i++)
                {
                    Items[i] = new NPCReply();
                    Items[i].Message = new string(messages[i]);
                    Items[i].Number = numbers[i];
                }
                NativeAPI.DeleteClientArray((IntPtr*)messages);
                NativeAPI.DeleteClientArray((IntPtr*)numbers);

                return Items;
            }
        }

        ///<summary>
        ///Gets character name
        ///</summary>
        public unsafe string CharacterName
        {
            get
            {
                byte* buffer = stackalloc byte[16];
                sbyte* sBuffer = (sbyte*)buffer;
                NativeAPI.GetCharacterName(this.Instance, sBuffer);
                return new string(sBuffer);
            }
        }

        ///<summary>
        ///Gets current message from NPC
        ///</summary>
        public unsafe string NPCMessage
        {
            get
            {
                return new string(NativeAPI.GetNpcText(this.Instance));
            }
        }

        ///<summary>
        ///Gets character UID
        ///</summary>
        public UInt32 UID
        {
            get
            {
                return NativeAPI.GetUID(this.Instance);
            }
        }

        ///<summary>
        ///Gets character model/mesh
        ///</summary>
        public UInt32 Model
        {
            get
            {
                return NativeAPI.GetModel(this.Instance);
            }
        }

        ///<summary>
        ///Gets character's amount of gold
        ///</summary>
        public UInt32 Gold
        {
            get
            {
                return NativeAPI.GetGold(this.Instance);
            }
        }

        ///<summary>
        ///Gets character's amount of CPs
        ///</summary>
        public UInt32 ConquerPoints
        {
            get
            {
                return NativeAPI.GetConquerPoints(this.Instance);
            }
        }

        ///<summary>
        ///Gets character's amount of Bound CPs
        ///</summary>
        public UInt32 BoundConquerPoints
        {
            get
            {
                return NativeAPI.GetBoundConquerPoints(this.Instance);
            }
        }

        ///<summary>
        ///Gets character's experience
        ///</summary>
        public UInt64 Experience
        {
            get
            {
                return NativeAPI.GetExperience(this.Instance);
            }
        }

        ///<summary>
        ///Gets character's strength
        ///</summary>
        public UInt16 StrengthPoints
        {
            get
            {
                return NativeAPI.GetStrengthPoints(this.Instance);
            }
        }

        ///<summary>
        ///Gets character's dexterity/agility
        ///</summary>
        public UInt16 DexterityPoints
        {
            get
            {
                return NativeAPI.GetDexterityPoints(this.Instance);
            }
        }

        ///<summary>
        ///Gets character's vitality
        ///</summary>
        public UInt16 VitalityPoints
        {
            get
            {
                return NativeAPI.GetVitalityPoints(this.Instance);
            }
        }

        ///<summary>
        ///Gets character's spirit
        ///</summary>
        public UInt16 SpiritPoints
        {
            get
            {
                return NativeAPI.GetSpiritPoints(this.Instance);
            }
        }

        ///<summary>
        ///Gets character's amount of stat points
        ///</summary>
        public UInt16 StatPoints
        {
            get
            {
                return NativeAPI.GetStatPoints(this.Instance);
            }
        }

        ///<summary>
        ///Gets character's HP
        ///</summary>
        public UInt16 HP
        {
            get
            {
                return NativeAPI.GetHP(this.Instance);
            }
        }

        ///<summary>
        ///Gets character's MP
        ///</summary>
        public UInt16 MP
        {
            get
            {
                return NativeAPI.GetMP(this.Instance);
            }
        }

        ///<summary>
        ///Gets character's PK Points
        ///</summary>
        public UInt16 PKPoints
        {
            get
            {
                return NativeAPI.GetPKPoints(this.Instance);
            }
        }

        ///<summary>
        ///Gets character's level
        ///</summary>
        public byte Level
        {
            get
            {
                return NativeAPI.GetLevel(this.Instance);
            }
        }

        ///<summary>
        ///Gets character's class
        ///</summary>
        public CharacterClass Class
        {
            get
            {
                return (CharacterClass)NativeAPI.GetClass(this.Instance);
            }
        }

        ///<summary>
        ///Gets character's reborn status
        ///</summary>
        public byte Reborn
        {
            get
            {
                return NativeAPI.GetReborn(this.Instance);
            }
        }

        ///<summary>
        ///Gets character's quiz show points
        ///</summary>
        public UInt32 QuizPoints
        {
            get
            {
                return NativeAPI.GetQuizPoints(this.Instance);
            }
        }

        ///<summary>
        ///Gets character's current map ID
        ///</summary>
        public UInt16 MapID
        {
            get
            {
                return NativeAPI.GetMapID(this.Instance);
            }
        }

        ///<summary>
        ///Gets character's stamina
        ///</summary>
        public UInt16 Stamina
        {
            get
            {
                return NativeAPI.GetStamina(this.Instance);
            }
        }

        ///<summary>
        ///Gets character's status flag
        ///</summary>
        public UInt64 StatusFlag
        {
            get
            {
                return NativeAPI.GetStatusFlag(this.Instance);
            }
        }

        ///<summary>
        ///Gets character's X coordinate
        ///</summary>
        public UInt16 X
        {
            get
            {
                return NativeAPI.GetX(this.Instance);
            }
        }

        ///<summary>
        ///Gets character's Y coordinate
        ///</summary>
        public UInt16 Y
        {
            get
            {
                return NativeAPI.GetY(this.Instance);
            }
        }

        ///<summary>
        ///Gets the connection latency/ping
        ///</summary>
        public UInt32 Ping
        {
            get
            {
                return NativeAPI.GetPing(this.Instance);
            }
        }

        ///<summary>
        ///Gets whether the character is on alternative gear or not
        ///</summary>
        public bool IsOnAlternativeGear
        {
            get
            {
                return Convert.ToBoolean(NativeAPI.GetAlternativeGear(this.Instance));
            }
        }

        ///<summary>
        ///Get map structure
        ///</summary>
        public gMap Map
        {
            get
            {
                gMap nMap = (gMap)Marshal.PtrToStructure(NativeAPI.GetMap(this.Instance), typeof(gMap));

                return nMap;
            }
        }

        ///<summary>
        ///Custom objects for each client
        ///</summary>
        public gDictionary MyObjects
        {
            get
            {
                if (GameClient.pObjects.ContainsKey(this.Instance))
                {
                    return GameClient.pObjects[this.Instance];
                }
                else
                {
                    GameClient.pObjects.Add(this.Instance, new gDictionary());
                    return GameClient.pObjects[this.Instance];
                }
            }
        }

        public class gDictionary : Dictionary<string, object>
        {
            new public object this[string key]
            {
                get
                {
                    if(this.ContainsKey(key))
                    {
                        return base[key];
                    }
                    else
                    {
                        return null;
                    }
                }
                set
                {
                    if (this.ContainsKey(key))
                    {
                        base[key] = value;
                    }
                    else
                    {
                        this.Add(key, value);
                    }
                }
            }
        }
    }
}
