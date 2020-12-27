#nullable enable

using System;

namespace Managed
{
    public interface IPlugin
    {
        void Initialize();
        void Update();
        void Dispose();
    }

    public class Plugin1 : IPlugin
    {
        public Plugin1() { Initialize(); }

        public void Initialize()
        {
            Console.WriteLine("Plugin1 Initialize");
        }

        public void Update()
        {
            Console.WriteLine("Plugin1 Update");
        }

        public void Dispose()
        {
            Console.WriteLine("Plugin1 Dispose");
        }
    }

    public class Plugin1Singleton : Singleton<Plugin1> { }
}