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

    //TODO: function

    [UnmanagedFunctionPointer(CallingConvention.ThisCall)]
    unsafe delegate void UnmanagedAction(IntPtr thisPtr);
    public static void runActionMethod(IntPtr thisPtr, IntPtr methodPtr)
    {
        UnmanagedAction method = (UnmanagedAction)Marshal.GetDelegateForFunctionPointer(
            methodPtr,
            typeof(UnmanagedAction)
        );
        method(thisPtr);
    }

    [UnmanagedFunctionPointer(CallingConvention.ThisCall)]
    unsafe delegate void UnmanagedActionInt(IntPtr thisPtr, int arg);
    public static void runActionMethodInt(IntPtr thisPtr, IntPtr methodPtr)
    {
        UnmanagedActionInt method = (UnmanagedActionInt)Marshal.GetDelegateForFunctionPointer(
            methodPtr,
            typeof(UnmanagedActionInt)
        );
        method(thisPtr, 4);
    }

    [UnmanagedFunctionPointer(CallingConvention.ThisCall)]
    unsafe delegate void UnmanagedActionIntInt(IntPtr thisPtr, int arg0, int arg1);
    public static void runActionMethodIntInt(IntPtr thisPtr, IntPtr methodPtr)
    {
        UnmanagedActionIntInt method = (UnmanagedActionIntInt)Marshal.GetDelegateForFunctionPointer(
            methodPtr,
            typeof(UnmanagedActionIntInt)
        );
        method(thisPtr, 4, 4);
    }
}
