
using System;


[Managed.Plugin]
public class ManagedPlugin : Managed.IPlugin
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

namespace Managed
{
    public class ManagedAttributePlugin : IPlugin
    {
        public ManagedAttributePlugin()
        {
            Console.WriteLine("ManagedAttributePlugin Constructor");
            Initialize();
        }

        public void Initialize()
        {
            Console.WriteLine("ManagedAttributePlugin Initialize");
        }

        public void Update()
        {
            Console.WriteLine("ManagedAttributePlugin Update");
        }

        public void Dispose()
        {
            Console.WriteLine("ManagedAttributePlugin Dispose");
        }
    }
}

