using System;
using System.Runtime.CompilerServices;

namespace ChiraSharp {
    public class CE
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static public void Print(string message);
    }
}
