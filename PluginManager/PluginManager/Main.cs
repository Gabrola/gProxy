using System;
using System.Xml;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using gProxyAPI;

namespace PluginManager
{
    public class Main
    {
        public static gProxyPlugin[] gProxyPlugins;

        public static void LoadPlugins()
        {
            try
            {
                XmlDocument xmlFile = new XmlDocument();
                xmlFile.Load("Plugins.xml");
                List<gProxyPlugin> list = new List<gProxyPlugin>();
                XmlNodeList Plugins = xmlFile.GetElementsByTagName("Plugin");
                foreach (XmlNode Node in Plugins)
                {
                    string FileName = Node.ChildNodes[0].InnerText;
                    string FileClass = Node.ChildNodes[1].InnerText;
                    if ((FileName == string.Empty) || (FileClass == string.Empty))
                    {
                        MessageBox.Show("Plugin: " + Node.Attributes[0].Value + " is damaged", "gProxy Plugin Error", MessageBoxButtons.OK, MessageBoxIcon.Hand);
                    }
                    else
                    {
                        try
                        {
                            list.Add(new gProxyPlugin(Node.FirstChild.InnerText, Node.ChildNodes[1].InnerText));
                        }
                        catch (Exception exception)
                        {
                            MessageBox.Show("Plugin: " + Node.Attributes[0].Value + " is damaged - " + exception.ToString(), "gProxy Plugin Error", MessageBoxButtons.OK, MessageBoxIcon.Hand);
                        }
                    }
                }

                gProxyPlugins = list.ToArray();
            }
            catch (Exception exception)
            {
                MessageBox.Show(exception.ToString());
            }
        }

        public static int ReceiveClientPacket(string pwzArgument)
        {
            string[] SplitArg = pwzArgument.Split('-');
            byte[] Buffer = ToByteArray(SplitArg[0]);
            IntPtr Instance = new IntPtr(int.Parse(SplitArg[1], System.Globalization.NumberStyles.HexNumber));

            int MainFlag = 1;
            foreach (gProxyPlugin Plugin in gProxyPlugins)
            {
                if (!Plugin.InvokeClientToServer(Instance, Buffer))
                {
                    MainFlag = 0;
                }
            }
            return MainFlag;
        }

        public static int ReceiveServerPacket(string pwzArgument)
        {
            string[] SplitArg = pwzArgument.Split('-');
            byte[] Buffer = ToByteArray(SplitArg[0]);
            IntPtr Instance = new IntPtr(int.Parse(SplitArg[1], System.Globalization.NumberStyles.HexNumber));

            int MainFlag = 1;
            foreach (gProxyPlugin Plugin in gProxyPlugins)
            {
                if (!Plugin.InvokeServerToClient(Instance, Buffer))
                {
                    MainFlag = 0;
                }
            }

            return MainFlag;
        }

        [DllImport("gProxyNativeAPI.dll")]
        private static unsafe extern void DeleteClientArray(IntPtr Num);

        public static void SendEvent(int _Client, int EventTypei, int _EventStruct)
        {
            IntPtr Client = new IntPtr(_Client);
            IntPtr EventStruct = new IntPtr(_EventStruct);
            object EventObject;
            if (EventTypei == 8)
            {
                unsafe
                {
                    EventObject = Marshal.PtrToStructure(EventStruct, typeof(ChatUnsafe));
                    ChatUnsafe chat = (ChatUnsafe)EventObject;
                    Chat SafeChat;
                    SafeChat.Color = chat.Color;
                    SafeChat.Send = chat.Send;
                    SafeChat.Type = chat.Type;
                    SafeChat.From = Marshal.PtrToStringAnsi((IntPtr)chat.sFrom);
                    SafeChat.Message = Marshal.PtrToStringAnsi((IntPtr)chat.sMessage);
                    SafeChat.To = Marshal.PtrToStringAnsi((IntPtr)chat.sTo);
                    DeleteClientArray((IntPtr)chat.sFrom);
                    DeleteClientArray((IntPtr)chat.sMessage);
                    DeleteClientArray((IntPtr)chat.sTo);
                    EventObject = (object)SafeChat;
                }
            }
            else
            {
                EventObject = Marshal.PtrToStructure(EventStruct, gProxyHelper.ConvertEventToType(EventTypei));
            }

            foreach (gProxyPlugin Plugin in gProxyPlugins)
            {
                Plugin.InvokeClientEvent(Client, EventTypei, EventObject);
            }
        }

        public static byte[] ToByteArray(String HexString)
        {
            int NumberChars = HexString.Length;
            byte[] bytes = new byte[NumberChars / 2];
            for (int i = 0; i < NumberChars; i += 2)
            {
                bytes[i / 2] = Convert.ToByte(HexString.Substring(i, 2), 16);
            }
            return bytes;
        }
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct ChatUnsafe
    {
        public unsafe sbyte* sFrom;
        public unsafe sbyte* sTo;
        public unsafe sbyte* sMessage;
        public uint Type;
        public uint Color;
        public byte Send;
    }
}
