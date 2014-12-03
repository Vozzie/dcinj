using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Windows.Forms;

using DCInjDn;

namespace dcinjdn_test_csharp
{   

    class Hook : IInjectable
    {

        private WindowProc m_windowProc;
        private Process m_process;
        private IntPtr m_prevProc;
        
        public Hook()
        {
            m_windowProc = new WindowProc(WndProc);
        }

        public void OnLoad()
        {
            m_process = Process.GetCurrentProcess();
            if (m_process.MainWindowHandle == IntPtr.Zero)
                return;
            m_prevProc = SetWndProc(Marshal.GetFunctionPointerForDelegate(m_windowProc));
            if (m_prevProc == IntPtr.Zero)
            {
                MessageBox.Show(Marshal.GetExceptionForHR(Marshal.GetHRForLastWin32Error()).ToString());
            }
        }

        public void OnUnload()
        {
            if(m_prevProc != IntPtr.Zero) SetWndProc(m_prevProc);
            m_process.Dispose();
        }

        public void OnCommand(byte[] buffer)
        {
            if (m_prevProc != IntPtr.Zero)
            {
                Encoding.Unicode.GetBytes("Hooked.").CopyTo(buffer, 0);
            }
            else
            {
                Encoding.Unicode.GetBytes("Hook failed!").CopyTo(buffer, 0);
            }
        }

        private IntPtr WndProc(IntPtr hwnd, uint uMsg, IntPtr wParam, IntPtr lParam)
        {
            // WM_CLOSE then cancel the message
            if (uMsg == 0x10)
                return IntPtr.Zero;
            return User32.CallWindowProc(m_prevProc, hwnd, uMsg, wParam, lParam);
        }

        private IntPtr SetWndProc(IntPtr wndProc)
        {
            if (IntPtr.Size == 8)
            {
                return User32.SetWindowLongPtr(m_process.MainWindowHandle, User32.GWLP_WNDPROC, wndProc);
            }
            else
            {
                return User32.SetWindowLong(m_process.MainWindowHandle, User32.GWLP_WNDPROC, wndProc);
            }
        }

    }
}



