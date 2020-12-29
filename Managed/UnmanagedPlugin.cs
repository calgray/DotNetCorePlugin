using System;
using System.Reflection;
using System.IO;
using System.Linq;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace Managed
{
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    unsafe delegate IntPtr UnmanagedFuncFunctionIntPtr();

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    unsafe delegate void UnmanagedActionFunction();

    [UnmanagedFunctionPointer(CallingConvention.ThisCall)]
    unsafe delegate void UnmanagedActionMethod(IntPtr thisPtr);

    /**
     * Wraps an unmanaged plugin exposed via a C interface. Optionally handles
     * module unloading when disposed. 
     */
    class UnmanagedPlugin : IPlugin
    {
        IntPtr m_module;
        IntPtr m_thisPtr;
        UnmanagedActionMethod m_handleDelete;
        UnmanagedActionMethod m_handleUpdate;

        /// <summary>
        /// 
        /// </summary>
        /// <param name="module">Optional reference to native module</param>
        /// <param name="thisPtr">Pointer to ther unmanaged object</param>
        /// <param name="delete">Delegate to the destructor/delete method</param>
        /// <param name="update">Delegate to the update method</param>
        public UnmanagedPlugin(IntPtr module, IntPtr thisPtr, UnmanagedActionMethod delete, UnmanagedActionMethod update)
        {
            m_module = module;
            m_thisPtr = thisPtr;
            m_handleDelete = delete;
            m_handleUpdate = update;
        }

        /// <summary>
        /// Creates a plugin loaded from a platform dependant unmanaged
        /// shared library binary file.
        /// </summary>
        /// <param name="name"></param>
        /// <returns></returns>
        public static UnmanagedPlugin CreateNativePlugin(string name)
        {
#if WIN32
            string filepath = Path.Combine(
                Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location),
                name + ".dll"
            );
#else // UNIX
            string filepath = Path.Combine(
                Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location),
                "lib" + name + ".so"
            );
            IntPtr module = dlopen(filepath, DlFlag.RTLD_LAZY);
            if(module == IntPtr.Zero) { throw new Exception(); }
            IntPtr createPluginPtr = dlsym(module, "CreatePlugin");
            if(createPluginPtr == IntPtr.Zero) throw new Exception();
            IntPtr deletePtr = dlsym(module, "DeletePlugin");
            if(deletePtr == IntPtr.Zero) throw new Exception();
            IntPtr updatePtr = dlsym(module, "UpdatePlugin");
            if(updatePtr == IntPtr.Zero) throw new Exception();
#endif

            UnmanagedFuncFunctionIntPtr createPlugin = (UnmanagedFuncFunctionIntPtr)Marshal.GetDelegateForFunctionPointer(
                createPluginPtr,
                typeof(UnmanagedFuncFunctionIntPtr)
            );

            UnmanagedActionMethod delete = (UnmanagedActionMethod)Marshal.GetDelegateForFunctionPointer(
                deletePtr,
                typeof(UnmanagedActionMethod)
            );

            UnmanagedActionMethod update = (UnmanagedActionMethod)Marshal.GetDelegateForFunctionPointer(
                updatePtr,
                typeof(UnmanagedActionMethod)
            );

            return new UnmanagedPlugin(module, createPlugin(), delete, update);
        }

        public void Initialize() {}

        public void Update()
        {
            m_handleUpdate(m_thisPtr);
        }
        
        public void Dispose()
        {
            m_handleDelete(m_thisPtr);
            if(m_module != IntPtr.Zero)
            {
#if WIN32
                FreeLibrary(m_module);
#else // UNIX
                dlclose(m_module);
#endif
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
