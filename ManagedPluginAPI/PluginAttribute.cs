
using System;

namespace Managed
{
    /// <summary>
    /// Public plugins within an assembly should hold this attribute.
    /// </summary>
    [AttributeUsage(AttributeTargets.Class, Inherited = false)]
    public class PluginAttribute : Attribute { }
}