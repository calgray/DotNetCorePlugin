
using System;

namespace Managed
{
    public interface IPlugin : IDisposable
    {
        void Initialize();
        void Update();

        // GameConstructionEvent
        // GamePreInitializationEvent
        // GameInitializationEvent
        // GamePostInitializationEvent
        // GameLoadCompleteEvent

        // GameAboutToStartServerEvent
        // GameStartingServerEvent
        // GameStartedServerEvent
        // GameStoppingServerEvent
        // GameStoppedServerEvent
    }
}