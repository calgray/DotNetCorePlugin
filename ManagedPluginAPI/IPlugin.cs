#nullable enable

using System;

namespace Managed
{
    public interface IPlugin : IDisposable
    {
        void Initialize();
        void Update();
        void Dispose();
    }
}