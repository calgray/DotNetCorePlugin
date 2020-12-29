/*
 *  Copyright (c) Hubert Jarosz. All rights reserved.
 *  Licensed under the MIT license. See LICENSE file in the project root for full license information.
 */

using System;
using System.Reflection;
using System.IO;
using System.Linq;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using Gtk;

namespace Managed
{
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    unsafe delegate void UnmanagedActionFunction();

    [UnmanagedFunctionPointer(CallingConvention.ThisCall)]
    unsafe delegate void UnmanagedAction(IntPtr thisPtr);

    public class UnmanagedPlugin : IPlugin, IDisposable
    {
        IntPtr m_thisPtr;
        UnmanagedAction m_handleDelete;
        UnmanagedAction m_handleUpdate;

        public UnmanagedPlugin(IntPtr thisPtr, IntPtr deletePtr, IntPtr updatePtr)
        {
            m_thisPtr = thisPtr;

            m_handleDelete = (UnmanagedAction)Marshal.GetDelegateForFunctionPointer(
                deletePtr,
                typeof(UnmanagedAction)
            );

            // Do whatever and delete once done
            m_handleUpdate = (UnmanagedAction)Marshal.GetDelegateForFunctionPointer(
                updatePtr,
                typeof(UnmanagedAction)
            );
        }

        public void Initialize() {}

        public void Update()
        {
            m_handleUpdate(m_thisPtr);
        }
        
        public void Dispose()
        {
            m_handleDelete(m_thisPtr);
        }
    }

    public class PluginManager : IDisposable
    {
        List<IPlugin> m_plugins = new List<IPlugin>();

        public void AddPlugin(IPlugin plugin)
        {
            m_plugins.Add(plugin);
            PrintPlugins();
        }

        public void PrintPlugins()
        {
            Console.WriteLine("nPlugins : " + m_plugins.Count);
        }

        public void Initialize()
        {
            foreach(var plugin in m_plugins)
            {
                plugin.Initialize();
            }
        }

        public void Update()
        {
            foreach(var plugin in m_plugins)
            {
                plugin.Update();
            }
        }

        public void Dispose()
        {
            foreach(var plugin in m_plugins)
            {
                plugin.Dispose();
            }
        }
    }

    public static class PluginManagerInterop // PluginEngineInterop
    {
        public static void AddPlugin(IntPtr thisPtr, IntPtr deletePtr, IntPtr updatePtr)
        {
            var plugin = new UnmanagedPlugin(thisPtr, deletePtr, updatePtr);
            Singleton<PluginManager>.Instance.AddPlugin(plugin);
        }

        /// <summary>
        /// Loads a CLR Plugin from a dll file
        /// </summary>
        public static void LoadCLRPlugin(string name)
        {
            Console.WriteLine(name);
            string filepath = Path.Combine(
                //Alternative: GetEntryAssembly
                Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location),
                name + ".dll"
            );
            var dll = Assembly.LoadFile(filepath);
            var pluginType = GetTypesWithPluginAttribute(dll).First();
            var plugin = (IPlugin)Activator.CreateInstance(pluginType, null);
            Singleton<PluginManager>.Instance.AddPlugin(plugin);
        }

        public static void LoadNativePlugin(string name)
        {
            Console.WriteLine(name);

#if WIN32
#else
            string filepath = Path.Combine(
                Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location),
                "lib" + name + ".so"
            );
            IntPtr module = dlopen(filepath, DlFlag.RTLD_LAZY);
            if(module == IntPtr.Zero)
            {
                throw new Exception();
            }


            IntPtr helloWorldPtr = dlsym(module, "HelloWorld");
            if(helloWorldPtr == IntPtr.Zero)
            {
                throw new Exception();
            }
            Console.WriteLine(helloWorldPtr);
            UnmanagedActionFunction helloWorld = (UnmanagedActionFunction)Marshal.GetDelegateForFunctionPointer(
                helloWorldPtr,
                typeof(UnmanagedActionFunction)
            );
            helloWorld();

            dlclose(module);
#endif
        }

        public static void UpdatePlugins()
        {
            Singleton<PluginManager>.Instance.Update();
        }

        public static void DisposePlugins()
        {
            Singleton<PluginManager>.Instance.Dispose();
        }

        private static IEnumerable<Type> GetTypesWithPluginAttribute(Assembly assembly)
        {
            foreach(Type type in assembly.GetTypes())
            {
                if (type.GetCustomAttributes(typeof(PluginAttribute), true).Length > 0)
                {
                    yield return type;
                }
            }
        }

        // Windows
        [DllImport("kernel32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        private static extern IntPtr LoadLibrary(string libName);
        
        [DllImport("kernel32.dll")]
        public static extern IntPtr GetProcAddress(IntPtr hModule, string procedureName);

        [DllImport("kernel32.dll")]
        public static extern bool FreeLibrary(IntPtr hModule);

        // Unix
        enum DlFlag
        {
            RTLD_LOCAL = 0x00000,
            RTLD_LAZY = 0x00001,
            RTLD_NOW = 0x00002,
            RTLD_NOLOAD = 0x00004,
            RTLD_DEEPBIND = 0x00008,
            RTLD_NODELETE = 0x01000,
            RTLD_GLOBAL = 0x00100,
        };
        [DllImport("libdl.so", CharSet = CharSet.Auto, SetLastError = true)]
        private static extern IntPtr dlopen(string libName, DlFlag flag);

        [DllImport("libdl.so", CharSet = CharSet.Auto, SetLastError = true)]
        private static extern IntPtr dlsym(IntPtr module, string name);

        [DllImport("libdl.so", CharSet = CharSet.Auto, SetLastError = true)]
        private static extern IntPtr dlclose(IntPtr module);

    }
}
