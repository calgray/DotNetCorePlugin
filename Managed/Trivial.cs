/*
 *  Copyright (c) Hubert Jarosz. All rights reserved.
 *  Licensed under the MIT license. See LICENSE file in the project root for full license information.
 */

using System;
using System.Runtime.InteropServices;
using Gtk;

public class Trivial
{
    public static void HelloWorld()
    {
        System.Console.WriteLine("Hello World from C#");
    }

    public static void HelloGtk()
    {
        Gtk.Application.Init();
        Gtk.Window window = new Gtk.Window("Hello World 2");
        window.DeleteEvent += (sender, args) => { 
            Application.Quit();
        }; 
        window.Show();
        Gtk.Application.Run();
        window.Close();
    }

    [UnmanagedFunctionPointer(CallingConvention.ThisCall)]
    unsafe delegate void UnmanagedAction(IntPtr thisPtr);

    /// <summary>
    /// Runs parameterless unmanaged lambda
    /// </summary>
    /// <param name="thisPtr"></param>
    /// <param name="memFun"></param>
    public static void runIt(IntPtr thisPtr, IntPtr memFun)
    {
        Console.WriteLine("Here's C# code:");

        UnmanagedAction fun = (UnmanagedAction)Marshal.GetDelegateForFunctionPointer(
            memFun,
            typeof(UnmanagedAction)
        );

        // first argument of member functions in C++ is "this", but it's hidden from us :-)
        fun(thisPtr);
    }
}
