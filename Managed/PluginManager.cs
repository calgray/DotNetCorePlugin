
using System;
using System.Reflection;
using System.IO;
using System.Linq;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using Gtk;

namespace Managed
{
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
}
