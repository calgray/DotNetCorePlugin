#nullable enable

using System;

namespace Managed
{
    public class PluginInterop
    {
        //TODO: Use attribute on ManagedPlugin
        public static Type GetPluginType()
        {
            return typeof(ManagedPlugin);
        }
    }

    [Plugin]
    public class ManagedPlugin : IPlugin
    {
        public ManagedPlugin()
        {
            Console.WriteLine("ManagedPlugin Constructor");
            Initialize();
        }

        public void Initialize()
        {
            Console.WriteLine("ManagedPlugin Initialize");
        }

        public void Update()
        {
            Console.WriteLine("ManagedPlugin Update");
        }

        public void Dispose()
        {
            Console.WriteLine("ManagedPlugin Dispose");
        }
    }
}