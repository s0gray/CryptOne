using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CryptOneService
{
    public class Log
    {
        public static void Line(string msg)
        {
            Debug.WriteLine(DateTime.Now.ToString("HH:mm:ss.fff") + " " + msg);
        }

        public static void Data(byte[] data)
        {
            Line( Tools.bytesToHex(data) );
        }
        public static void Data(string msg, byte[] data)
        {
            Line(msg + " [" + Tools.bytesToHex(data) + "]");
        }
    }
}
