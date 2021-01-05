
using System;

[Managed.Plugin]
public class ManagedPlugin : Managed.IPlugin
{
    public ManagedPlugin()
    {
        Console.WriteLine("ManagedPlugin Constructor");
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
