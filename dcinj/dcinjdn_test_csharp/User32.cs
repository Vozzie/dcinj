using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;

namespace dcinjdn_test_csharp
{
    internal delegate IntPtr WindowProc(IntPtr hwnd, uint uMsg, IntPtr wParam, IntPtr lParam);

    internal sealed class User32
    {
        private const string DLLNAME = "User32.dll";

        internal const int GWLP_WNDPROC = -4;

        [DllImport(DLLNAME, SetLastError = true)]
        internal extern static IntPtr CallWindowProc(IntPtr lpPrevWndFunc, IntPtr hWnd, uint uMsg, IntPtr wParam, IntPtr lParam);
        //End Function

        [DllImport(DLLNAME, SetLastError = true)]
        internal extern static IntPtr SetWindowLongPtr(IntPtr hWnd, int nIndex, IntPtr lpNewLong);

        [DllImport(DLLNAME, SetLastError = true)]
        internal extern static IntPtr SetWindowLong(IntPtr hWnd, int nIndex, IntPtr lpNewLong);

    }
}
