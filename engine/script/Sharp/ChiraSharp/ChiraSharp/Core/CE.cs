using System;
using System.Runtime.CompilerServices;

namespace ChiraSharp {
    public static class CE
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static public void Print(string message);
    }
}
