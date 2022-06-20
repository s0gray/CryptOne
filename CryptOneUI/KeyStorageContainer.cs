using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CryptOneService
{
    public class KeyStorageContainer
    {
        public bool autodetectKeyStorage = true;

        KeyStorage keyStorage = null; // new KeyStorage(); // so far one but can be few in future
        public KeyStorageContainer()
        {

        }

        internal void load(IniFile ini)
        {
            var keyFolder = ini.Read(Form1.KEY_FOLDER);
            if (keyFolder == null || keyFolder.Length == 0)
            {
               // Debug.WriteLine("Auto keyFolder");
                autodetectKeyStorage = true;
            }
            else
            {
               // Debug.WriteLine("keyFolder = [" + keyFolder + "]");
                autodetectKeyStorage = false;
                keyStorage = new KeyStorage(keyFolder);

            }
        }

        public string getKeyFolder()
        {
            if(keyStorage!=null)
            {
                return keyStorage.keyFolder;
            }
            return "";
        }

        internal void setAutodetectKeyStorage(bool value, string keyfolder)
        {
            this.autodetectKeyStorage = value;
            if (value)
            {
                keyStorage = null;
            } else
            {
                keyStorage = new KeyStorage(keyfolder);
            }
        }

        public void save(IniFile ini)
        {
            if (!autodetectKeyStorage)
            {
                ini.Write(Form1.KEY_FOLDER, getKeyFolder(), Form1.SECTION);
            }
        }

        public void setKeyFolder(string text)
        {
            if(keyStorage!=null)
            {
                keyStorage.keyFolder = text;
            }
        }
    }
}
