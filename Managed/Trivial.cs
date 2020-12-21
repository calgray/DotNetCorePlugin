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
        // System.Console.WriteLine(new SharpDX.Vector3(1,2,3).ToString());
    }

    public static void HelloGtk()
    {
        Gtk.Application.Init();
        Gtk.Window window = new Gtk.Window("Hello World");
        window.DeleteEvent += (sender, args) => { 
            Application.Quit();
        }; 
        window.Show();
        Gtk.Application.Run();
        window.Close();
    }

    [UnmanagedFunctionPointer(CallingConvention.ThisCall)]
    unsafe delegate void UnmanagedAction(IntPtr thisPtr);
    public static void runActionMethod(IntPtr thisPtr, IntPtr methodPtr)
    {
        Console.WriteLine("Here's C# code:");

        UnmanagedAction method = (UnmanagedAction)Marshal.GetDelegateForFunctionPointer(
            methodPtr,
            typeof(UnmanagedAction)
        );

        // first argument of member functions in C++ is "this", but it's hidden from us :-)
        method(thisPtr);
    }

    [UnmanagedFunctionPointer(CallingConvention.ThisCall)]
    unsafe delegate void UnmanagedActionInt(IntPtr thisPtr, int arg);
    public static void runActionMethodInt(IntPtr thisPtr, IntPtr methodPtr)
    {
        Console.WriteLine("Here's C# code:");

        UnmanagedActionInt method = (UnmanagedActionInt)Marshal.GetDelegateForFunctionPointer(
            methodPtr,
            typeof(UnmanagedActionInt)
        );

        method(thisPtr, 4);
    }
}
