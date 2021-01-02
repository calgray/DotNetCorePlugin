
using System;
using System.Reflection;
using System.IO;
using System.Linq;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace Managed
{
    /// <summary>
    /// Provides shared library entry points for plugin interaction
    /// </summary>
    public static class PluginManagerInterop // PluginEngineInterop
    {
        public static void AddPlugin(
            IntPtr thisPtr,
            IntPtr deletePtr,
            IntPtr initializePtr, IntPtr buffer0,
            IntPtr updatePtr, IntPtr buffer1)
        {
            if(thisPtr == IntPtr.Zero) throw new Exception("thisPtr");
            if(deletePtr == IntPtr.Zero) throw new Exception("deletePtr");
            if(initializePtr == IntPtr.Zero) throw new Exception("initializePtr");
            if(updatePtr == IntPtr.Zero) throw new Exception("updatePtr");

            var module = IntPtr.Zero; // The calling module will handle it own lifecycle.


            var delete = (UnmanagedActionMethod)Marshal.GetDelegateForFunctionPointer(
                deletePtr,
                typeof(UnmanagedActionMethod)
            );
            var initialize = (UnmanagedActionMethod)Marshal.GetDelegateForFunctionPointer(
                initializePtr,
                typeof(UnmanagedActionMethod)
            );
            var update = (UnmanagedActionMethod)Marshal.GetDelegateForFunctionPointer(
                updatePtr,
                typeof(UnmanagedActionMethod)
            );
            var plugin = new UnmanagedPlugin(module, thisPtr, delete, initialize, update);
            Singleton<PluginManager>.Instance.AddPlugin(plugin);
        }

        /// <summary>
        /// Loads a CLR Plugin from a dll file
        /// </summary>
        /// <param name="name">name of the local dll file</param>
        public static void LoadCLRPlugin(string name)
        {
            string filepath = Path.Combine(
                //Alternative: GetEntryAssembly
                Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location),
                name + ".dll"
            );
            var assembly = Assembly.LoadFile(filepath);
            var pluginType = GetTypesWithPluginAttribute(assembly).First();
            var plugin = (IPlugin)Activator.CreateInstance(pluginType, null);
            if(plugin == null)
            {
                throw new Exception("plugin is null");
            }
            Singleton<PluginManager>.Instance.AddPlugin(plugin);
        }

        /// <summary>
        /// Loads a CLR Plugin from a dll file where plugin class matches assembly name 
        /// </summary>
        /// <param name="name">name of the local dll file</param>
        /// <param name="namespace">name of the class namespace</param>
        public static void LoadCLRPluginExplicit(string name, string? namespaceName)
        {
            string filepath = Path.Combine(
                //Alternative: GetEntryAssembly
                Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location),
                name + ".dll"
            );
            var assembly = Assembly.LoadFile(filepath);
            string className = namespaceName == null ? name : namespaceName + "." + name;
            Console.WriteLine(className);
            var plugin = (IPlugin)assembly.CreateInstance(className, false);
            if(plugin == null)
            {
                throw new Exception("plugin is null");
            }
            Singleton<PluginManager>.Instance.AddPlugin(plugin);
        }

        /// <summary>
        /// Loads a native plugin from a native shared library 
        /// </summary>
        /// <param name="name"></param>
        public static void LoadNativePlugin(string name)
        {
            var plugin = UnmanagedPlugin.CreateNativePlugin(name);
            Singleton<PluginManager>.Instance.AddPlugin(plugin);
        }

        public static void InitializePlugins()
        {
            Singleton<PluginManager>.Instance.Initialize();
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
    }
}