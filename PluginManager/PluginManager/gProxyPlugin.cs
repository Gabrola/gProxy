using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Reflection;
using System.IO;
using gProxyAPI;
using System.Runtime.InteropServices;
using System.Windows.Forms;

namespace PluginManager
{
    public class gProxyPlugin
    {
        private Assembly Assembly;
        private MethodInfo ClientToServer;
        private MethodInfo ServerToClient;
        private MethodInfo ClientEvent;

        public gProxyPlugin(string AssemblyFile, string MainType)
        {
            this.Assembly = Assembly.LoadFile(Directory.GetCurrentDirectory() + @"\" + AssemblyFile);
            Type type = this.Assembly.GetType(MainType);
            if (type == null)
            {
                throw new ArgumentException("Type not found", "MainType");
            }

            this.ClientToServer = type.GetMethod("ClientToServer");
            if (this.ClientToServer != null)
            {
                VerifyInterceptParams(this.ClientToServer);
            }

            this.ServerToClient = type.GetMethod("ServerToClient");
            if (this.ServerToClient != null)
            {
                VerifyInterceptParams(this.ServerToClient);
            }

            this.ClientEvent = type.GetMethod("ClientEvent");
            if (this.ClientEvent != null)
            {
                VerifyEventParams(this.ClientEvent);
            }

            MethodInfo method = type.GetMethod("Main");
            if (method != null)
            {
                if (method.GetParameters().Length > 0)
                {
                    throw new ArgumentException("Method " + method.Name + " should have no parameters");
                }
                method.Invoke(null, null);
            }
        }

        public bool InvokeClientToServer(IntPtr Instance, byte[] Packet)
        {
            if (this.ClientToServer != null)
            {
                return (bool)this.ClientToServer.Invoke(null, new object[] { Instance, Packet });
            }
            return true;
        }

        public bool InvokeServerToClient(IntPtr Instance, byte[] Packet)
        {
            if (this.ServerToClient != null)
            {
                return (bool)this.ServerToClient.Invoke(null, new object[] { Instance, Packet });
            }
            return true;
        }

        public void InvokeClientEvent(IntPtr Client, int EventType, object EventStruct)
        {
            if (this.ClientEvent != null)
            {
                this.ClientEvent.Invoke(null, new object[] { Client, EventType, EventStruct });
            }
        }

        private static void VerifyInterceptParams(MethodInfo methodInfo)
        {
            ParameterInfo[] parameters = methodInfo.GetParameters();
            if (parameters.Length != 2)
            {
                throw new ArgumentException("Method " + methodInfo.Name + " should have 2 parameters.");
            }
            if (parameters[0].ParameterType.Name != typeof(IntPtr).Name)
            {
                throw new ArgumentException("Method " + methodInfo.Name + " parameter 1 should be IntPtr");
            }
            if (parameters[1].ParameterType != typeof(byte[]))
            {
                throw new ArgumentException("Method " + methodInfo.Name + " parameter 2 should be Byte[]");
            }
        }

        private static void VerifyEventParams(MethodInfo methodInfo)
        {
            ParameterInfo[] parameters = methodInfo.GetParameters();
            if (parameters.Length != 3)
            {
                throw new ArgumentException("Method " + methodInfo.Name + " should have 3 parameters.");
            }
            if (parameters[0].ParameterType != typeof(IntPtr))
            {
                throw new ArgumentException("Method " + methodInfo.Name + " parameter 1 should be IntPtr");
            }
            if (parameters[1].ParameterType != typeof(int) && parameters[1].ParameterType.Name != typeof(EventType).Name)
            {
                throw new ArgumentException("Method " + methodInfo.Name + " parameter 2 should be int or EventType");
            }
            if (parameters[2].ParameterType != typeof(object))
            {
                throw new ArgumentException("Method " + methodInfo.Name + " parameter 3 should be object");
            }
        }
    }
}
