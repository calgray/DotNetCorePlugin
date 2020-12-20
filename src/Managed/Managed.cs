/*
 *  Copyright (c) Hubert Jarosz. All rights reserved.
 *  Licensed under the MIT license. See LICENSE file in the project root for full license information.
 */

using System;
using System.Runtime.InteropServices;

public class Managed
{
    [UnmanagedFunctionPointer(CallingConvention.ThisCall)]
    unsafe delegate void myDelegate( IntPtr thisptr );

    public static void runIt(IntPtr thisPtr, IntPtr memFun)
    {
        Console.WriteLine("Here's C# code:");

        myDelegate fun = (myDelegate)Marshal.GetDelegateForFunctionPointer(memFun, typeof(myDelegate));

        // first argument of member functions in C++ is "this", but it's hidden from us :-)
        fun(thisPtr);
    }
}
