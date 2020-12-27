#nullable enable

using System;

namespace Managed
{
    public class Singleton<T> where T : new()
    {
        private static readonly Lazy<T> m_instance
            = new Lazy<T>(() => new T());

        public static T Instance
        { 
            get { return m_instance.Value; }
        }
    }
}