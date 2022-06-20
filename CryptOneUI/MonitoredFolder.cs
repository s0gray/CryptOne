using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Diagnostics;

namespace CryptOneService
{
    public class MonitoredFolder
    {
        public string path = "";

        FileSystemWatcher fileSystemWatcher;
        Form1 mainForm;

        public MonitoredFolder(string p, Form1 form)
        {
            this.path = p;
            this.mainForm = form;

            this.fileSystemWatcher = new FileSystemWatcher(p);
            this.fileSystemWatcher.EnableRaisingEvents = true;
            this.fileSystemWatcher.NotifyFilter = System.IO.NotifyFilters.LastWrite; //LastAccess;
            this.fileSystemWatcher.SynchronizingObject = mainForm;
            this.fileSystemWatcher.Changed += new System.IO.FileSystemEventHandler(this.fileSystemWatcher_Changed);
        }

        public string getArchiveFileNameWithoutExtension()
        {
            string lastFolderName = Path.GetFileName(path);
            return lastFolderName;
        }

        public string getArchiveFileName()
        {
            return getArchiveFileNameWithoutExtension() + ".tgz";
        }
        public string getInfoFileName()
        {
            return getArchiveFileName() + ".info";
        }
        public string getFullInfoFileName()
        {
            return CryptoOne.tempFolder + getInfoFileName();
        }
        public Dictionary<string,string> loadInfoFile()
        {
            string file = getFullInfoFileName();
            if (!File.Exists(file))
                return null;

            IniFile ini = new IniFile(file);
            Dictionary<string, string> dict = new Dictionary<string, string>();

            string tmp = ini.Read(CryptoOne.DIR_HASH_KEY, "CryptOne");
            if(tmp!=null)
                dict.Add(CryptoOne.DIR_HASH_KEY, tmp);

            tmp = ini.Read(CryptoOne.TGZ_HASH_KEY, "CryptOne");
            if (tmp != null)
                dict.Add(CryptoOne.TGZ_HASH_KEY, tmp);

            tmp = ini.Read(CryptoOne.ENC_HASH_KEY, "CryptOne");
            if (tmp != null)
                dict.Add(CryptoOne.ENC_HASH_KEY, tmp);

            return dict;
        }

        public string getFullArchiveFileName()
        {
            return CryptoOne.tempFolder + getArchiveFileName();
        }

        private void fileSystemWatcher_Changed(object sender, FileSystemEventArgs e)
        {
            Log.Line("File changed in folder ["+this.path+"]");
        }

        // in which file this folder will be stored in cloud
        public string getCloudFileName()
        {      
            return getArchiveFileName() + ".enc"; 
        }

        public string getFullEncryptedArchiveFileName()
        {
            return CryptoOne.tempFolder + getCloudFileName();
        }
    }
}
