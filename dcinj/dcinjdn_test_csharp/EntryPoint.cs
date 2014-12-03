using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Windows.Forms;

using DCInjDn;

namespace dcinjdn_test_csharp
{
    internal static class EntryPoint
    {
        internal static void Main()
        {
            using (var process = System.Diagnostics.Process.Start("notepad.exe"))
            {
                var type = typeof(Hook);
                var typeName = type.Namespace + "." + type.Name;
                process.WaitForInputIdle();
                var injector = Injector.Create(process);
                injector.Inject(type.Assembly.Location, typeName);
                byte[] buffer = new byte[1024];
                injector.Command(typeName, buffer);
                MessageBox.Show(System.Text.Encoding.Unicode.GetString(buffer));
                var i = injector.Count();
                var s = injector.Item(0);
                injector.Eject(typeName);
                injector.Close();
                process.WaitForExit();
            }
        }
    }
}
