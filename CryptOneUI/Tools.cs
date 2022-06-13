using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.IO;

namespace CryptOneService
{
    public class Tools
    {
        public static string[] getAvailableDrives()
        {
            DriveInfo[] drives = DriveInfo.GetDrives();
            string[] result = new string[drives.Length];
            for(int i=0; i<drives.Length;i++)
            {
                result[i] = drives[i].Name;
            }

            return result;
        }

        public static string[] getRemovableDrives()
        {
            DriveInfo[] drives = DriveInfo.GetDrives();
            List<string> result = new List<string>();
            for (int i = 0; i < drives.Length; i++)
            {
                if (drives[i].DriveType == DriveType.Removable)
                   result.Add( drives[i].Name );
            }

            return result.ToArray();
        }

        public static string decodeFolder(string folder)
        {
            return folder.Replace("~", Form1.homeDir);
        }

        public static string getKeyFolder()
        {
            string[] removable = Tools.getRemovableDrives();
            if(removable== null || removable.Length == 0)
            {
                return "";
            }

            for(int i=0; i< removable.Length; i++)
            {
                string keyFileName = removable[i] + Form1.KEY_FILENAME;
                if (File.Exists(keyFileName))
                    return removable[i];
            }
            return "";
        }
    }
}
