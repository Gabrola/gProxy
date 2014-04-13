using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using System.Threading;

#pragma warning disable 1591
namespace gProxyAPINative
{
    public class StaticNative
    {
        public static void AbortThread(int hThread)
        {
            GCHandle Handle = GCHandle.FromIntPtr(new IntPtr(hThread));
            Thread thread = (Thread) Handle.Target;
            thread.Abort();
        }
    }
}
