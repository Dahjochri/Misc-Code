using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace MyShell
{
    static class SystemInfo
    {
        public static string Hostname
        {
            get
            {
                return System.Environment.MachineName;
            }
        }
        public static string Username{
            get{
                return System.Security.Principal.WindowsIdentity.GetCurrent().Name.Split('\\')[1];
            }
        }
    }
}
