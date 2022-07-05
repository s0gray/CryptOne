using System.Diagnostics;

namespace CryptOneService
{
    public class Log
    {
        private static TextBox logBox = null;
        public static void setTextBox(TextBox obj)
        {
            logBox = obj;
        }
        public static void Line(string msg)
        {
            string message = DateTime.Now.ToString("HH:mm:ss.fff") + " " + msg;
            Debug.WriteLine(message);

            if (logBox != null)
            {
                logBox.Text += message + "\r\n";
            }
        }

        public static void Data(byte[] data)
        {
            Line(Tools.bytesToHex(data));
        }
        public static void Data(string msg, byte[] data)
        {
            Line(msg + " [" + Tools.bytesToHex(data) + "]");
        }
    }
}
